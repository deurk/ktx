/*
bot/botjump.qc

Copyright (C) 1997-1999 Robert 'Frog' Field
Copyright (C) 1998-2000 Matt 'asdf' McChesney
Copyright (C) 2001 Justice
Copyright (C) 2000-2007 ParboiL
*/

// Converted from .qc on 05/02/2016

#ifdef BOT_SUPPORT

#include "g_local.h"
#include "fb_globals.h"

void DemoMark ();

#define FB_LAVAJUMP_NOT    0      // not lava-jumping
#define FB_LAVAJUMP_SINK   1      // deliberately sinking, waiting for waterlevel == 3
#define FB_LAVAJUMP_RISE   2      // ready to fire on first sign of waterlevel == 2

static vec3_t straight_up = { 0, 0, 1 };
static vec3_t straight_down = { 0, 0, -1 };

/*
// Returns true if the bot is travelling in the 'right' direction (with 90 degrees of target)
static qbool right_direction(gedict_t* self)
{
	vec3_t test_direction,
	       direction_to_test_marker;
	float current_direction,
		  desired_direction;
	float min_one,
	      min_two;

	// current direction
	normalize(self->fb.oldvelocity, test_direction);
	current_direction = vectoyaw(test_direction);
	
	// desired direction
	VectorSubtract(self->fb.linked_marker->s.v.origin, self->s.v.origin, direction_to_test_marker);
	normalize(direction_to_test_marker, test_direction);
	desired_direction = vectoyaw(test_direction);

	min_one = fabsf(desired_direction - current_direction);

	// Reduce angle size and try again, incase one was 20 and the other was 340.. 
	if (desired_direction >= 180) {
		desired_direction -= 360;
	}
	if (current_direction >= 180) {
		current_direction -= 360;
	}
	min_two = fabsf(desired_direction - current_direction);

	return (qbool)(min(min_one, min_two) <= 90);
}*/

// Returns true if space above bot
static float BotCheckSpaceAbove(gedict_t* self)
{
	traceline(self->s.v.origin[0], self->s.v.origin[1], self->s.v.origin[2], self->s.v.origin[0], self->s.v.origin[1], self->s.v.origin[2] + 140, true, self);
	return (g_globalvars.trace_fraction == 1);
}

/*
// Returns true if ground directly in front of bot
static float checkground(gedict_t* self)
{
	trap_makevectors(self->s.v.v_angle);
	g_globalvars.v_forward[2] = 0;
	VectorNormalize(g_globalvars.v_forward);
	VectorScale(g_globalvars.v_forward, 10, g_globalvars.v_forward);
	traceline(self->s.v.origin[0], self->s.v.origin[1], self->s.v.origin[2], self->s.v.origin[0] + g_globalvars.v_forward[0], self->s.v.origin[1] + g_globalvars.v_forward[1], self->s.v.origin[2] + g_globalvars.v_forward[2] - 40, true, self);
	return (g_globalvars.trace_fraction != 1);
}
*/

void BotCanRocketJump(gedict_t* self)
{
	qbool has_quad = self->super_damage_finished > g_globalvars.time;
	qbool tp_damage = teamplay != 1 && teamplay != 5;
	//float health_after = TotalStrengthAfterDamage(self->s.v.health, self->s.v.armorvalue, self->s.v.armortype, tp_damage ? 55 * (has_quad ? 4 : 1) : 0);
	qbool has_rl = (qbool)(((int)self->s.v.items & IT_ROCKET_LAUNCHER) && (self->s.v.ammo_rockets >= 1));
	qbool has_pent = (qbool)((int)self->s.v.items & IT_INVULNERABILITY);
	qbool will_pickup = self->fb.linked_marker && !WaitingToRespawn(self->fb.linked_marker);
	qbool onground = (int)self->s.v.flags & FL_ONGROUND;

	if (self->fb.debug_path) {
		self->fb.canRocketJump = self->fb.debug_path_rj;
	}
	else if (has_rl && self->s.v.waterlevel > 1 && trap_pointcontents(self->s.v.origin[0], self->s.v.origin[1], self->s.v.origin[2]) == CONTENT_LAVA) {
		// In lava, is our main priority...
		self->fb.canRocketJump = true;
	}
	else if (!has_rl || self->fb.be_quiet || !onground) {
		self->fb.canRocketJump = false;
	}
	else if (has_pent) {
		// If invincible, can always rocket jump
		self->fb.canRocketJump = true;
	}
	else {
		// work out how much health we'll have in next marker
		float health_after = TotalStrengthAfterDamage(self->s.v.health, self->s.v.armorvalue, self->s.v.armortype, tp_damage ? 55 * (has_quad ? 4 : 1) : 0);
		if (health_after >= 0 && will_pickup) {
			// FIXME: This is broken, need to know how much health/armor they'd have left after initial impact
			//    Also take into account the goal entity, not just next link in path
			if (self->fb.linked_marker->tp_flags & (it_mh | it_health)) {
				health_after += 50;
			}
		}
		// FIXME: this threshold should be flexible depending on fuzzy logic
		// - depending on enemy status (maybe higher or lower depending on strength of enemy)
		// - might be higher or lower depending on running away from enemy
		// - might be lower if we need to beat enemy to item (quad for instance)
		self->fb.canRocketJump = health_after > 110;
	}
}

/*
// FIXME: If teammate is strong (RA etc), fire anyway?  Also if boomsticker and we are strong
// FIXME: Use find_radius?  Currently ignores teammates behind them.
static qbool CouldHurtNearbyTeammate(gedict_t* me) {
	gedict_t* p;

	// if the bot can't kill them, then don't worry about it
	if (teamplay != 2) {
		return false;
	}

	p = IdentifyMostVisibleTeammate(me);
	return VectorDistance(p->s.v.origin, me->s.v.origin) < 140;
}
*/

// Checks that (x=>z, y=>z) direction is sufficiently similar
static qbool DirectionCheck (vec3_t start, vec3_t end, vec3_t vel, float threshold)
{
	vec3_t path, hor_vel;

	VectorSubtract (end, start, path);
	path[2] = 0;
	VectorCopy (vel, hor_vel);
	hor_vel[2] = 0;

	VectorNormalize (path);
	VectorNormalize (hor_vel);

	return DotProduct (path, hor_vel) >= threshold;
}

static void BotLavaJumpRise(gedict_t* self)
{
	self->fb.arrow = BACK;
	NewVelocityForArrow (self, straight_up, "LavaJump1");
	self->fb.path_state |= ROCKET_JUMP;
	self->fb.lavaJumpState = FB_LAVAJUMP_RISE;
	self->fb.desired_angle[PITCH] = 78.25;
}

static void BotLavaJumpSink(gedict_t* self)
{
	self->fb.swim_arrow = DOWN;
	self->fb.rocketJumping = false;
	NewVelocityForArrow (self, straight_down, "LavaJump3");
	// Moving backwards...
	self->fb.lavaJumpState = FB_LAVAJUMP_SINK;
	self->fb.desired_angle[PITCH] = 78.25;
	self->fb.up_finished = 0;
}

static void SetRocketJumpAngles(gedict_t* self)
{
	if (self->fb.path_state & ROCKET_JUMP) {
		if (self->fb.rocketJumpAngles[PITCH] > 0) {
			self->fb.desired_angle[PITCH] = self->fb.rocketJumpAngles[PITCH];
		}
		else {
			self->fb.desired_angle[PITCH] = 78.25;
		}
		if (self->fb.rocketJumpAngles[YAW] > 0) {
			self->fb.desired_angle[YAW] = self->fb.rocketJumpAngles[YAW];
		}
	}
}

static void BotPerformLavaJump(gedict_t* self)
{
	vec3_t point;
	int content;

	VectorSet(point, self->s.v.origin[0], self->s.v.origin[1], self->s.v.origin[2] - 24);
	content = self->s.v.waterlevel > 1 ? trap_pointcontents(PASSVEC3(point)) : CONTENT_EMPTY;

	// Have fired but still in lava
	if ((self->fb.path_state & BOTPATH_RJ_IN_PROGRESS) && content == CONTENT_LAVA) {
		BotLavaJumpRise(self);
		self->fb.desired_angle[PITCH] = 78.25;
		return;
	}

	// Waiting to fire, still in lava
	if (content == CONTENT_LAVA && self->fb.rocketJumpFrameDelay) {
		BotLavaJumpRise(self);
		self->fb.desired_angle[PITCH] = 78.25;
		return;
	}

	if (self->fb.canRocketJump && content == CONTENT_LAVA) {
		switch (self->fb.lavaJumpState) {
		case FB_LAVAJUMP_NOT:
			if (self->s.v.waterlevel >= 2 && BotCheckSpaceAbove(self)) {
				vec3_t linked_direction;
				float best_yaw = 0;

				// Face away from target
				VectorSubtract(self->s.v.origin, self->fb.linked_marker->s.v.origin, linked_direction);
				best_yaw = vectoyaw(linked_direction);

				if (self->s.v.waterlevel == 3) {
					BotLavaJumpRise(self);

					self->fb.desired_angle[YAW] = best_yaw;
				}
				else {
					BotLavaJumpSink(self);
					self->fb.desired_angle[YAW] = best_yaw;
				}
			}
			else {
				// Move as standard, hopefully to somewhere with space
				return;
			}
			break;
		case FB_LAVAJUMP_SINK:
			if (self->s.v.waterlevel == 3 || self->s.v.velocity[2] == 0) {
				// That's far enough, start rising again
				BotLavaJumpRise(self);
			}
			else {
				// Keep going
				BotLavaJumpSink(self);
			}
			break;
		case FB_LAVAJUMP_RISE:
			if (self->fb.up_finished && g_globalvars.time > self->fb.up_finished) {
				// Lava jump failed, back to sinking...
				BotLavaJumpSink(self);
			}
			else if (self->s.v.waterlevel == 3) {
				BotLavaJumpRise(self);
			}
			else {
				// Broken surface, time to fire... queue up a delay...
				vec3_t explosion_point;
				float distance_frac;
				float frames_to_explosion;

				SetRocketJumpAngles(self);
				FindRocketExplosionPoint(self->s.v.origin, self->fb.desired_angle, explosion_point, &distance_frac);
				BotLavaJumpRise(self);

				frames_to_explosion = (VectorDistance(self->s.v.origin, explosion_point) / 1000) / g_globalvars.frametime;

				self->fb.rocketJumping = true;
				self->fb.rocketJumpFrameDelay = 12 - (int)frames_to_explosion;
				self->fb.lavaJumpState = FB_LAVAJUMP_NOT;
				self->fb.up_finished = g_globalvars.time + 0.1;
			}
			break;
		}
	}
}

// Performs rocket jump
void BotPerformRocketJump(gedict_t* self) {
	if (!(self->fb.touch_marker && self->fb.linked_marker)) {
		return;
	}

	BotPerformLavaJump(self);

	if (self->fb.rocketJumping) {
		// In middle of rocket jumping
		--self->fb.rocketJumpFrameDelay;
	}
	else if (self->fb.canRocketJump && ((int)self->s.v.flags & FL_ONGROUND)) {
		qbool path_is_rj   = self->fb.path_state & ROCKET_JUMP;
		qbool ok_to_rj     = match_in_progress == 2 || (self->fb.debug_path && self->fb.debug_path_rj);
		float touch_dist   = VectorDistance (self->s.v.origin, self->fb.touch_marker->s.v.origin);
		// Close enough to marker
		qbool ok_distance  = touch_dist >= 10 && touch_dist <= 100;
		// Travelling towards target
		qbool ok_direction = DirectionCheck (self->fb.touch_marker->s.v.origin, self->fb.linked_marker->s.v.origin, self->s.v.velocity, 0.8f); // FIXME: TODO
		// Going at reasonable pace
		qbool ok_vel       = VectorLength (self->s.v.velocity) > sv_maxspeed * 0.9;
		// Fire will trigger (technically we could jump anyway if framedelay enabled, but leave for now)
		qbool ok_to_fire   = !self->s.v.button0 && self->attack_finished < g_globalvars.time;

		// FIXME: TODO: Also used to check they wouldn't kill teammate, but think that should be fuzzy logic
		qbool can_start_rj = ok_to_rj && path_is_rj && ok_distance && ok_direction && ok_to_fire && ok_vel;

		if (can_start_rj) {
			// Perform the jump
			//G_bprint (PRINT_HIGH, "rocket-jumping: %s, distance %s, dir %s, fire %s\n", path_is_rj ? "yes" : "no", ok_distance ? "yes" : "no", ok_direction ? "yes" : "no", ok_to_fire ? "yes" : "no");
			SetJumpFlag (self, true, "RocketJump");
			self->fb.rocketJumping = true;
			self->fb.rocketJumpFrameDelay = self->fb.rocketJumpPathFrameDelay;
			self->fb.path_state |= DELIBERATE_AIR_WAIT_GROUND;
			if (FrogbotOptionEnabled (FB_OPTION_DEMOMARK_ROCKETJUMPS)) {
				DemoMark ();
			}
		}
		else if (self->fb.debug_path && ok_to_rj) {
			//G_bprint (PRINT_HIGH, "path_is_rj: %s, distance %s, dir %s, fire %s\n", path_is_rj ? "yes" : "no", ok_distance ? "yes" : "no", ok_direction ? "yes" : "no", ok_to_fire ? "yes" : "no");
		}
	}

	// Is it time to fire
	if (self->fb.rocketJumping) {
		if (self->fb.rocketJumpFrameDelay <= 0) {
			SetRocketJumpAngles(self);
			self->fb.desired_weapon_impulse = 7;
			self->fb.firing = true;

			// Finish rocket jumping
			self->fb.rocketJumping = false;
			self->fb.path_state &= ROCKET_JUMP;
			self->fb.path_state |= BOTPATH_RJ_IN_PROGRESS;
		}
		else {
			// Make sure we don't start firing
			self->fb.firing = false;
		}
	}
}

static qbool PlayerFiringLG (gedict_t* player)
{
	return player && player->s.v.button0 && ((int)player->s.v.weapon & IT_LIGHTNING) && player->s.v.ammo_cells > 0;
}

void CheckCombatJump(gedict_t* self)
{
	qbool inWater = self->s.v.waterlevel && self->fb.allowedMakeNoise;
	qbool onGround = ((int)self->s.v.flags & FL_ONGROUND);
	qbool lookingAtEnemy = self->fb.look_object && NUM_FOR_EDICT(self->fb.look_object) == self->s.v.enemy;
	qbool lookObjectFiringLG = PlayerFiringLG (self->fb.look_object);

	// Never combat jump when debugging route execution
	if (self->fb.debug_path) {
		return;
	}

	// Or immediately after spawning...
	if (FUTURE(min_fire_time)) {
		return;
	}

	// Or during prewar (noisy)
	if (match_in_progress < 2) {
		return;
	}

	// If jumping makes sense...
	if (!onGround || inWater) {
		return;
	}

	// Never jump in midair
	if (cvar ("k_midair")) {
		return;
	}

	// Rocket jumping decisions made elsewhere
	if (self->fb.rocketJumping) {
		return;
	}

	// If path is expecting an edge then no combat jump...
	if (self->fb.path_state & (JUMP_LEDGE | BOTPATH_CURLJUMP_HINT)) {
		return;
	}

	// If surprised or player firing LG, don't jump
	if (!lookingAtEnemy || lookObjectFiringLG) {
		return;
	}

	// Now just down to bot characteristics
	SetJumpFlag (self, (g_random () < self->fb.skill.combat_jump_chance), "CombatJump");
}

#endif
