
#ifdef BOT_SUPPORT

#include "g_local.h"
#include "fb_globals.h"

/*
Okay, some explanation required here.

Previous Frogbot logic was:

Entity list: PrePhysics, <frogbot 1... n>, PostPhysics
Each entity processed in turn...
  PostPhysics would check all previous frogbots to see if they were obstructed, and move in different direction instead
In PR2 bots, we each bot sends command to server, and physics then match standard player movement, no cheating
  This makes finding obstruction more difficult, so instead mvdsv runs user command then calls Blocked, giving
  the bot a chance to send a new command and execute that instead
When blocked() function is called, velocity/position/flags of where the bot would have ended up are stored:
  origin:   g_globalvars.trace_endpos
  velocity: g_globalvars.trace_plane_normal
  flags:    g_globalvars.trace_allsolid
This is temporary kludge until I work out better solution.  Really the bot should do this calculation first, or
  perhaps extend API to have G_SIMULATE_COMMAND?  This is very basic attempt to not break API, while effectively
  (under the covers) changing the API.  Ho hum.
Long term goal is to replace this kind of "try...fail...try-something-else" logic with a navigation mesh, there would
  still need to be some collision detection for colliding with other players tho.
*/

static qbool obstruction (gedict_t* self, vec3_t new_velocity, vec3_t new_origin, int newFlags, vec3_t velocity_normal);

// Gives us a chance to replace bot's command with another.
void BotBlocked (void)
{
	vec3_t blocked_velocity;
	vec3_t blocked_position;
	vec3_t velocity_normal;
	int blocked_flags = (int)g_globalvars.trace_allsolid;

	VectorCopy (g_globalvars.trace_endpos, blocked_position);
	VectorCopy (g_globalvars.trace_plane_normal, blocked_velocity);

	// Detect if we were obstructed & should try to change direction
	if (obstruction (self, blocked_velocity, blocked_position, blocked_flags, velocity_normal)) {
		vec3_t new_velocity;
		float yaw = vectoyaw (self->fb.obstruction_normal);
		float dist = 0;

		//removed: seems to be just scaling velocity to see if we can find new spot...
		// old code set velocity specifically which we don't need to do (?)
		VectorAdd(blocked_velocity, velocity_normal, new_velocity);

		//dist = ((new_velocity * frametime) - (self.origin - self.oldorigin)) * self.obstruction_normal;
		{
			vec3_t temp, originDiff;

			VectorScale (new_velocity, g_globalvars.frametime, temp);
			VectorSubtract (blocked_position, self->s.v.origin, originDiff);
			VectorSubtract (temp, originDiff, temp);
			dist = DotProduct (temp, self->fb.obstruction_normal);
		}

		VectorCopy(self->s.v.origin, dropper->s.v.origin);
		dropper->s.v.flags = FL_ONGROUND_PARTIALGROUND;

		if (walkmove(dropper, yaw, dist)) {
			// Replace the command with moving in new direction
			new_velocity[2] = 0;
			//G_bprint (2, ".. [%f %f %f]", PASSVEC3(self->fb.dir_move_));
			SetDirectionMove (self, self->fb.obstruction_normal, "Obstruction");
			//G_bprint (2, "> [%f %f %f]\n", PASSVEC3(self->fb.dir_move_));
			VectorClear (self->fb.obstruction_normal);
		}
		else {
			AvoidHazards (self);
		}
		BotSetCommand (self);
		return;
	}
}

//Sets self.obstruction_normal to be horizontal normal direction into wall obstruction encountered
// during quake physics (ie. between PlayerPreThink and PlayerPostThink)
//
static qbool obstruction(gedict_t* self, vec3_t new_velocity, vec3_t new_origin, int newFlags, vec3_t velocity_normal)
{
	vec3_t old_velocity;
	qbool onGround = (newFlags & FL_ONGROUND);
	qbool waterJump = (newFlags & FL_WATERJUMP);
	vec3_t delta_velocity = { 0 };

	VectorCopy(self->fb.last_cmd_direction, old_velocity);

	VectorClear (velocity_normal);
	VectorSubtract (old_velocity, new_velocity, delta_velocity);
	if (fabsf (delta_velocity[0]) < 0.1 && fabsf (delta_velocity[1]) < 0.1)
	{
		VectorClear(self->fb.obstruction_normal);
		return false;
	}

	// In mid-air and not swimming?  Nothing we can do about it, wait to hit the ground...
	if (! onGround && ((!self->s.v.waterlevel) || waterJump))
	{
		VectorClear(self->fb.obstruction_normal);
		return false;
	}

	// If not hitting ground...
	if (fabsf(delta_velocity[2]) < 0.1)
	{
		vec3_t hor_velocity;
		VectorCopy (new_velocity, hor_velocity);
		hor_velocity[2] = 0;
		if (hor_velocity[0] != 0 || hor_velocity[1] != 0)
		{
			VectorSubtract (old_velocity, hor_velocity, velocity_normal);
			velocity_normal[2] = 0;
			normalize (velocity_normal, self->fb.obstruction_normal);
			return true;
		}

		VectorCopy (old_velocity, hor_velocity);
		hor_velocity[2] = 0;
		if (hor_velocity[0] != 0 || hor_velocity[1] != 0)
		{
			float cross_product = DotProduct (self->fb.obstruction_normal, old_velocity);

			VectorMA (old_velocity, -cross_product, self->fb.obstruction_normal, velocity_normal);
			velocity_normal[2] = 0;

			normalize (velocity_normal, self->fb.obstruction_normal);
			return true;
		}
	}

	// No obstruction
	VectorClear (self->fb.obstruction_normal);
	return false;
}

#endif
