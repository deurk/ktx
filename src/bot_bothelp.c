/*
bot/bothelp.qc

Copyright (C) 1999-2000 Numb
Copyright (C) 2000-2007 ParboiL
*/

// Converted from .qc on 05/02/2016

#ifdef BOT_SUPPORT

#include "g_local.h"
#include "fb_globals.h"

// Checks if self > point is not blocked
static qbool PointVisible(vec3_t vec) {
	traceline(PASSVEC3(self->s.v.origin), PASSVEC3(vec), true, self);
	if ((g_globalvars.trace_fraction == 1) && !(g_globalvars.trace_inopen && g_globalvars.trace_inwater)) {
		return true;
	}
	return false;
}

// FIXME: Doesn't take view offset into consideration when tracing
// FIXME: Don't run extra traces if they are further away
gedict_t* IdentifyMostVisibleTeammate(gedict_t* me) {
	gedict_t* p, *g = NULL;
	unsigned long clientFlag = ClientFlag (me);
	float closeness = -1;
	vec3_t diff, point;
	float currclose;

	for (p = world; (p = find_plr (p)); ) {
		if (me != p && (p->visclients & clientFlag) && SameTeam(me, p)) {
			// Find difference in angles between aim & aiming at teammate
			VectorSubtract(p->s.v.origin, me->s.v.origin, diff);
			VectorNormalize(diff);
			normalize(me->s.v.angles, point);
			VectorSubtract(diff, point, diff);
			currclose = vlen(diff);

			// If we have direct visibility
			traceline(PASSVEC3(me->s.v.origin), PASSVEC3(p->s.v.origin), false, me);
			if (PROG_TO_EDICT(g_globalvars.trace_ent) == p) {
				if (closeness == -1) {
					closeness = currclose;
					g = p;
				}
				else if (currclose < closeness) {
					closeness = currclose;
					g = p;
				}
			}
		}
	}

	return g ? g : world;
}

qbool VisibleEntity(gedict_t* ent) {
	vec3_t vec;
	if (PointVisible(ent->s.v.origin)) {
		return true;
	}
	VectorCopy(ent->s.v.origin, vec);
	vec[2] = ent->s.v.absmin[2];
	if (PointVisible(vec)) {
		return true;
	}
	vec[2] = ent->s.v.absmax[2];
	return PointVisible(vec);
}

gedict_t* HelpTeammate() {
	gedict_t* goalent;
	gedict_t* head, *selected1, *selected2;
	float d,
	      bdist,
	      best_dist1,
	      best_dist2;

	if (!teamplay) {
		return NULL;
	}
	if (self->fb.state & WAIT) {
		return NULL;
	}
	if (self->fb.state & RUNAWAY) {
		return NULL;
	}
	if (self->fb.state & NOTARGET_ENEMY) {
		return NULL;
	}

	goalent = &g_edicts[self->s.v.goalentity];
	if (goalent->s.v.goalentity == NUM_FOR_EDICT(self)) {
		return NULL;
	}
	if ((goalent->ct == ctPlayer) && (goalent != self)) {
		if (SameTeam(goalent, self)) {
			if ((goalent->s.v.health < 30) && !((int)goalent->s.v.items & IT_INVULNERABILITY) && (goalent->s.v.waterlevel == 0)) {
				if (((int)self->s.v.items & (IT_ROCKET_LAUNCHER | IT_LIGHTNING)) && (self->s.v.health > 65)) {
					if ((self->s.v.ammo_rockets > 2) || (self->s.v.ammo_cells > 10)) {
						if (VisibleEntity(goalent)) {
							self->fb.state |= HELP_TEAMMATE;
							return goalent;
						}
					}
				}
			}
		}
	}
	bdist = 500;
	if (g_globalvars.time < self->fb.help_teammate_time) {
		return NULL;
	}
	self->fb.help_teammate_time = g_globalvars.time + 20 + 3 * g_random();
	selected1 = NULL;
	selected2 = NULL;
	best_dist1 = 99999999.0;
	best_dist2 = 99999999.0;
	for (head = world; (head = trap_findradius(head, self->s.v.origin, bdist)); ) {
		if (head->ct == ctPlayer) {
			if (SameTeam(head, self)) {
				if (head != self) {
					d = VectorDistance(head->s.v.origin, self->s.v.origin);
					if (NUM_FOR_EDICT(self) != head->s.v.goalentity) {
						if (d < best_dist1) {
							if (VisibleEntity(head) && !((int)head->s.v.items & IT_INVULNERABILITY) && (head->s.v.health < 40) && (head->s.v.armorvalue < 11) && (head->s.v.waterlevel == 0)) {
								if ((int)self->s.v.items & (IT_ROCKET_LAUNCHER | IT_LIGHTNING)) {
									if ((self->s.v.ammo_cells > 10) || (self->s.v.ammo_rockets > 2)) {
										selected1 = head;
										self->fb.state |= HELP_TEAMMATE;
										best_dist1 = d;
									}
								}
							}
						}
					}
					if (SameTeam(head, self)) {
						if (NUM_FOR_EDICT(self) != head->s.v.goalentity) {
							if (d < best_dist2) {
								if (VisibleEntity(head) && !((int)head->s.v.items & IT_INVULNERABILITY) && (head->s.v.health < 30) && (head->s.v.armorvalue < 20) && (head->s.v.waterlevel == 0)) {
									if ((int)self->s.v.items & (IT_ROCKET_LAUNCHER | IT_LIGHTNING)) {
										if ((self->s.v.ammo_cells > 10) || (self->s.v.ammo_rockets > 2)) {
											selected2 = head;
											self->fb.state |= HELP_TEAMMATE;
											best_dist2 = d;
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	if (selected1) {
		return selected1;
	}
	else if (selected2) {
		return selected2;
	}
	return NULL;
}

#endif
