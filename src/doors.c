/*
 *  QWProgs-DM
 *  Copyright (C) 2004  [sd] angel
 *
 *  This code is based on QuakeWorld DM mod code by Id Software, Inc.
 *
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *
 *  $Id$
 */

#include "g_local.h"
#include "fb_globals.h"

#define DOOR_START_OPEN   1
#define DOOR_DONT_LINK    4
#define DOOR_GOLD_KEY     8
#define DOOR_SILVER_KEY  16
#define DOOR_TOGGLE      32

/*

Doors are similar to buttons, but can spawn a fat trigger field around them
to open without a touch, and they link together to form simultanious
double/quad doors.
 
Door.owner is the master door.  If there is only one door, it points to itself.
If multiple doors, all will point to a single one.

Door.enemy chains from the master door through all doors linked in the chain.

*/

/*
=============================================================================

THINK FUNCTIONS

=============================================================================
*/

//#define   STATE_TOP                0;
//#define   STATE_BOTTOM             1;
//#define   STATE_UP                 2;
//#define   STATE_DOWN               3;
void            door_blocked();
void            door_hit_top();
void            door_hit_bottom();
void            door_go_down();
void            door_go_up();
void            door_fire();

void door_blocked()
{
	other->deathtype = dtSQUISH;
	T_Damage( other, self, PROG_TO_EDICT( self->s.v.goalentity ), self->dmg );

// if a door has a negative wait, it would never come back if blocked,
// so let it just squash the object to death real fast
	if ( self->wait >= 0 )
	{
		if ( self->state == STATE_DOWN )
			door_go_up();
		else
			door_go_down();
	}
}


void door_hit_top()
{
	sound( self, CHAN_NO_PHS_ADD + CHAN_VOICE, self->noise1, 1, ATTN_NORM );
	self->state = STATE_TOP;

#ifdef BOT_SUPPORT
	if (bots_enabled()) {
		BotEventDoorHitTop(self);
	}
#endif

	if ( ( int ) ( self->s.v.spawnflags ) & DOOR_TOGGLE )
		return;		// don't come down automatically

	self->think = ( func_t ) door_go_down;
	self->s.v.nextthink = self->s.v.ltime + self->wait;
}

void door_hit_bottom()
{
	sound( self, CHAN_NO_PHS_ADD + CHAN_VOICE, self->noise1, 1, ATTN_NORM );
	self->state = STATE_BOTTOM;

#ifdef BOT_SUPPORT
	if (bots_enabled()) {
		BotEventDoorHitBottom(self);
	}
#endif
}

void door_go_down()
{
	sound( self, CHAN_VOICE, self->noise2, 1, ATTN_NORM );
	if ( self->s.v.max_health )
	{
		self->s.v.takedamage = DAMAGE_YES;
		self->s.v.health = self->s.v.max_health;
	}

	self->state = STATE_DOWN;
	SUB_CalcMove( self->pos1, self->speed, door_hit_bottom );
}

void door_go_up()
{

	if ( self->state == STATE_UP )
		return;		// allready going up

	if ( self->state == STATE_TOP )
	{			// reset top wait time

		self->s.v.nextthink = self->s.v.ltime + self->wait;
		return;
	}

	sound( self, CHAN_VOICE, self->noise2, 1, ATTN_NORM );
	self->state = STATE_UP;

	SUB_CalcMove( self->pos2, self->speed, door_hit_top );
	SUB_UseTargets();
}


/*
=============================================================================

ACTIVATION FUNCTIONS

=============================================================================
*/

void door_fire()
{
	gedict_t       *oself, *starte;

	if ( PROG_TO_EDICT( self->s.v.owner ) != self )
		G_Error( "door_fire: self.owner != self" );

// play use key sound

	if ( self->s.v.items )
		sound( self, CHAN_VOICE, self->noise4, 1, ATTN_NORM );

	self->message = 0;	// no more message
	oself = self;

	if ( ( int ) ( self->s.v.spawnflags ) & DOOR_TOGGLE )
	{
		if ( self->state == STATE_UP || self->state == STATE_TOP )
		{
			starte = self;
			do
			{
				door_go_down();

				self = PROG_TO_EDICT( self->s.v.enemy );
			}
			while ( ( self != starte ) && ( self != world ) );
			self = oself;
			return;
		}
	}
// trigger all paired doors
	starte = self;

	do
	{
		self->s.v.goalentity = EDICT_TO_PROG( activator );	// Who fired us
		door_go_up();
		self = PROG_TO_EDICT( self->s.v.enemy );
	}
	while ( ( self != starte ) && ( self != world ) );
	self = oself;
}


void door_use()
{
	gedict_t       *oself;

	self->message = "";	// door message are for touch only
	PROG_TO_EDICT( self->s.v.owner )->message = "";
	PROG_TO_EDICT( self->s.v.enemy )->message = "";

	oself = self;
	self = PROG_TO_EDICT( self->s.v.owner );
	door_fire();
	self = oself;
}


void door_trigger_touch()
{
// return if countdown or map frozen
	if( !k_practice ) // #practice mode#
	if( match_in_progress == 1 
		|| ( !match_in_progress && cvar( "k_freeze" ) )
	  )
		 return;

	if ( ISDEAD( other ) )
		return;

	if ( g_globalvars.time < self->attack_finished )
		return;
	self->attack_finished = g_globalvars.time + 1;

	activator = other;

	self = PROG_TO_EDICT( self->s.v.owner );
	door_use();
}


void door_killed()
{
	gedict_t       *oself;

	if( !k_practice ) // #practice mode#
	if( match_in_progress != 2 ) 
       return;

	oself = self;
	self = PROG_TO_EDICT( self->s.v.owner );
	self->s.v.health = self->s.v.max_health;
	self->s.v.takedamage = DAMAGE_NO;	// wil be reset upon return

	door_use();
	self = oself;
}


/*
================
door_touch

Prints messages and opens key doors
================
*/
void door_touch()
{
	char           *msg;

// return if countdown or map frozen
	if( !k_practice ) // #practice mode#
	if( match_in_progress == 1
		|| ( !match_in_progress && cvar( "k_freeze" ) )
	  ) 
		return;

	if ( other->ct != ctPlayer )
		return;

	if ( PROG_TO_EDICT( self->s.v.owner )->attack_finished > g_globalvars.time )
		return;

	PROG_TO_EDICT( self->s.v.owner )->attack_finished = g_globalvars.time + 2;
	msg = PROG_TO_EDICT( self->s.v.owner )->message;

	if ( msg && msg[0] )
	{
		G_centerprint( other, "%s", PROG_TO_EDICT( self->s.v.owner )->message );
		sound( other, CHAN_VOICE, "misc/talk.wav", 1, ATTN_NORM );
	}
// key door stuff
	if ( self->s.v.items == 0 )
		return;

// FIXME: blink key on player's status bar
	if ( ( ( int ) self->s.v.items & ( int ) other->s.v.items ) != self->s.v.items )
	{
		if ( PROG_TO_EDICT( self->s.v.owner )->s.v.items == IT_KEY1 )
		{
			if ( world->worldtype == 2 )
			{
				G_centerprint( other, "You need the silver keycard" );
				sound( self, CHAN_VOICE, self->noise3, 1,
					    ATTN_NORM );
			} else if ( world->worldtype == 1 )
			{
				G_centerprint( other, "You need the silver runekey" );
				sound( self, CHAN_VOICE, self->noise3, 1,
					    ATTN_NORM );
			} else if ( world->worldtype == 0 )
			{
				G_centerprint( other, "You need the silver key" );
				sound( self, CHAN_VOICE, self->noise3, 1,
					    ATTN_NORM );
			}
		} else
		{
			if ( world->worldtype == 2 )
			{
				G_centerprint( other, "You need the gold keycard" );
				sound( self, CHAN_VOICE, self->noise3, 1,
					    ATTN_NORM );
			} else if ( world->worldtype == 1 )
			{
				G_centerprint( other, "You need the gold runekey" );
				sound( self, CHAN_VOICE, self->noise3, 1,
					    ATTN_NORM );
			} else if ( world->worldtype == 0 )
			{
				G_centerprint( other, "You need the gold key" );
				sound( self, CHAN_VOICE, self->noise3, 1,
					    ATTN_NORM );
			}
		}
		return;
	}

	other->s.v.items -= self->s.v.items;
	self->touch = ( func_t ) SUB_Null;

	if ( self->s.v.enemy )
		PROG_TO_EDICT( self->s.v.enemy )->touch = ( func_t ) SUB_Null;	// get paired door

	door_use();
}

/*
=============================================================================

SPAWNING FUNCTIONS

=============================================================================
*/


gedict_t       *spawn_field( vec3_t fmins, vec3_t fmaxs )
{
	gedict_t       *trigger;

	trigger = spawn();
	trigger->s.v.movetype = MOVETYPE_NONE;
	trigger->s.v.solid = SOLID_TRIGGER;
	trigger->s.v.owner = EDICT_TO_PROG( self );
	trigger->touch = ( func_t ) door_trigger_touch;

	setsize( trigger, fmins[0] - 60, fmins[1] - 60, fmins[2] - 8, fmaxs[0] + 60,
		      fmaxs[1] + 60, fmaxs[2] + 8 );
	return ( trigger );
}


qbool EntitiesTouching( gedict_t * e1, gedict_t * e2 )
{
	if ( e1->s.v.mins[0] > e2->s.v.maxs[0] )
		return false;

	if ( e1->s.v.mins[1] > e2->s.v.maxs[1] )
		return false;

	if ( e1->s.v.mins[2] > e2->s.v.maxs[2] )
		return false;

	if ( e1->s.v.maxs[0] < e2->s.v.mins[0] )
		return false;

	if ( e1->s.v.maxs[1] < e2->s.v.mins[1] )
		return false;

	if ( e1->s.v.maxs[2] < e2->s.v.mins[2] )
		return false;

	return true;
}


/*
=============
LinkDoors


=============
*/
void LinkDoors()
{
	gedict_t       *t, *starte;
	vec3_t          cmins, cmaxs;

	if ( self->s.v.enemy )
		return;		// already linked by another door

	if ( ( int ) ( self->s.v.spawnflags ) & 4 )
	{
		self->s.v.owner = self->s.v.enemy = EDICT_TO_PROG( self );
		return;		// don't want to link this door
	}

	VectorCopy( self->s.v.mins, cmins );
	VectorCopy( self->s.v.maxs, cmaxs );
	//cmins = self->s.v.mins;
	//cmaxs = self->s.v.maxs;

	starte = self;
	t = self;

	do
	{
		self->s.v.owner = EDICT_TO_PROG( starte );	// master door

		if ( ISLIVE( self ) )
			starte->s.v.health = self->s.v.health;

		if ( self->targetname )
			starte->targetname = self->targetname;

		if ( strneq( self->message, "" ) )
			starte->message = self->message;

		t = find( t, FOFCLSN, self->classname );

		if ( !t )
		{
			self->s.v.enemy = EDICT_TO_PROG( starte );	// make the chain a loop

			// shootable, fired, or key doors just needed the owner/enemy links,
			// they don't spawn a field

			self = PROG_TO_EDICT( self->s.v.owner );

			if ( ISLIVE( self ) )
				return;
			if ( self->targetname )
				return;
			if ( self->s.v.items )
				return;
			PROG_TO_EDICT( self->s.v.owner )->trigger_field =
			    spawn_field( cmins, cmaxs );

			return;
		}

		if ( EntitiesTouching( self, t ) )
		{
			if ( t->s.v.enemy )
				G_Error( "cross connected doors" );

			self->s.v.enemy = EDICT_TO_PROG( t );
			self = t;

			if ( t->s.v.mins[0] < cmins[0] )
				cmins[0] = t->s.v.mins[0];

			if ( t->s.v.mins[1] < cmins[1] )
				cmins[1] = t->s.v.mins[1];

			if ( t->s.v.mins[2] < cmins[2] )
				cmins[2] = t->s.v.mins[2];

			if ( t->s.v.maxs[0] > cmaxs[0] )
				cmaxs[0] = t->s.v.maxs[0];

			if ( t->s.v.maxs[1] > cmaxs[1] )
				cmaxs[1] = t->s.v.maxs[1];

			if ( t->s.v.maxs[2] > cmaxs[2] )
				cmaxs[2] = t->s.v.maxs[2];
		}
	}
	while ( 1 );

}


/*QUAKED func_door (0 .5 .8) ? START_OPEN x DOOR_DONT_LINK GOLD_KEY SILVER_KEY TOGGLE
if two doors touch, they are assumed to be connected and operate as a unit.

TOGGLE causes the door to wait in both the start and end states for a trigger event.

START_OPEN causes the door to move to its destination when spawned, and operate in reverse.  It is used to temporarily or permanently close off an area when triggered (not usefull for touch or takedamage doors).

Key doors are allways wait -1.

"message"       is printed when the door is touched if it is a trigger door and it hasn't been fired yet
"angle"         determines the opening direction
"targetname" if set, no touch field will be spawned and a remote button or trigger field activates the door.
"health"        if set, door must be shot open
"speed"         movement speed (100 default)
"wait"          wait before returning (3 default, -1 = never return)
"lip"           lip remaining at end of move (8 default)
"dmg"           damage to inflict when blocked (2 default)
"sounds"
0)      no sound
1)      stone
2)      base
3)      stone chain
4)      screechy metal
*/

void SP_func_door()
{
	float           tmp;

	if ( world->worldtype == 0 )
	{
		trap_precache_sound( "doors/medtry.wav" );
		trap_precache_sound( "doors/meduse.wav" );
		self->noise3 = "doors/medtry.wav";
		self->noise4 = "doors/meduse.wav";
	} else if ( world->worldtype == 1 )
	{
		trap_precache_sound( "doors/runetry.wav" );
		trap_precache_sound( "doors/runeuse.wav" );
		self->noise3 = "doors/runetry.wav";
		self->noise4 = "doors/runeuse.wav";
	} else if ( world->worldtype == 2 )
	{
		trap_precache_sound( "doors/basetry.wav" );
		trap_precache_sound( "doors/baseuse.wav" );
		self->noise3 = "doors/basetry.wav";
		self->noise4 = "doors/baseuse.wav";
	} else
	{
		G_Printf( "no worldtype set!\n" );
	}
	if ( self->s.v.sounds == 0 )
	{
//		trap_precache_sound( "misc/null.wav" );
//		trap_precache_sound( "misc/null.wav" );
//		self->noise1 = "misc/null.wav";
//		self->noise2 = "misc/null.wav";
// qqshka: shut up null.wav, have some artefact, we can hear
		self->noise1 = "";
		self->noise2 = "";
	}
	if ( self->s.v.sounds == 1 )
	{
		trap_precache_sound( "doors/drclos4.wav" );
		trap_precache_sound( "doors/doormv1.wav" );
		self->noise1 = "doors/drclos4.wav";
		self->noise2 = "doors/doormv1.wav";
	}
	if ( self->s.v.sounds == 2 )
	{
		trap_precache_sound( "doors/hydro1.wav" );
		trap_precache_sound( "doors/hydro2.wav" );
		self->noise2 = "doors/hydro1.wav";
		self->noise1 = "doors/hydro2.wav";
	}
	if ( self->s.v.sounds == 3 )
	{
		trap_precache_sound( "doors/stndr1.wav" );
		trap_precache_sound( "doors/stndr2.wav" );
		self->noise2 = "doors/stndr1.wav";
		self->noise1 = "doors/stndr2.wav";
	}
	if ( self->s.v.sounds == 4 )
	{
		trap_precache_sound( "doors/ddoor1.wav" );
		trap_precache_sound( "doors/ddoor2.wav" );
		self->noise1 = "doors/ddoor2.wav";
		self->noise2 = "doors/ddoor1.wav";
	}


	SetMovedir();

	self->s.v.max_health = self->s.v.health;
	self->s.v.solid = SOLID_BSP;
	self->s.v.movetype = MOVETYPE_PUSH;

	setorigin( self, PASSVEC3( self->s.v.origin ) );
	setmodel( self, self->model );

	self->classname = "door";

	self->blocked = ( func_t ) door_blocked;
	self->use = ( func_t ) door_use;

	if ( ( int ) ( self->s.v.spawnflags ) & DOOR_SILVER_KEY )
		self->s.v.items = IT_KEY1;

	if ( ( int ) ( self->s.v.spawnflags ) & DOOR_GOLD_KEY )
		self->s.v.items = IT_KEY2;

	if ( !self->speed )
		self->speed = 100;

	if ( !self->wait )
		self->wait = 3;

	if ( !self->lip )
		self->lip = 8;
	
	if ( !self->dmg )
		self->dmg = 2;

	VectorCopy( self->s.v.origin, self->pos1 );

	//
	tmp = fabsf( DotProduct( self->s.v.movedir, self->s.v.size ) ) - self->lip;

	self->pos2[0] = self->pos1[0] + self->s.v.movedir[0] * tmp;
	self->pos2[1] = self->pos1[1] + self->s.v.movedir[1] * tmp;
	self->pos2[2] = self->pos1[2] + self->s.v.movedir[2] * tmp;

// DOOR_START_OPEN is to allow an entity to be lighted in the closed position
// but spawn in the open position
	if ( ( int ) ( self->s.v.spawnflags ) & DOOR_START_OPEN )
	{
		setorigin( self, PASSVEC3( self->pos2 ) );
		VectorCopy( self->pos1, self->pos2 );
		VectorCopy( self->s.v.origin, self->pos1 );
	}

	self->state = STATE_BOTTOM;

	if ( ISLIVE( self ) )
	{
		self->s.v.takedamage = DAMAGE_YES;
		self->th_die = door_killed;
	}

	if ( self->s.v.items )
		self->wait = -1;

	self->touch = ( func_t ) door_touch;

// LinkDoors can't be done until all of the doors have been spawned, so
// the sizes can be detected properly.
	self->think = ( func_t ) LinkDoors;
	self->s.v.nextthink = self->s.v.ltime + 0.1;
}

/*
=============================================================================

SECRET DOORS

=============================================================================
*/

void            fd_secret_move1();
void            fd_secret_move2();
void            fd_secret_move3();
void            fd_secret_move4();
void            fd_secret_move5();
void            fd_secret_move6();
void            fd_secret_done();

#define SECRET_OPEN_ONCE 1	// stays open
#define SECRET_1ST_LEFT 2	// 1st move is left of arrow
#define SECRET_1ST_DOWN 4	// 1st move is down from arrow
#define SECRET_NO_SHOOT 8	// only opened by trigger
#define SECRET_YES_SHOOT 16	// shootable even if targeted
#define SECRET_NEVER 32 // lock it shut, CTF ONLY.

void fd_secret_use( gedict_t * attacker, float take )
{
	float           temp;

	if( !k_practice ) // #practice mode#
	if( match_in_progress != 2 )
        return;

	self->s.v.health = 10000;

	// exit if still moving around...
	if ( !VectorCompare( self->s.v.origin, self->s.v.oldorigin ) )
		return;

	self->message = 0;	// no more message
	//activator=attacker;
	SUB_UseTargets();	// fire all targets / killtargets

	if ( isCTF() && ( ( int ) ( self->s.v.spawnflags ) & SECRET_NEVER ) )
		return; // it never opens

	if ( !( ( int ) ( self->s.v.spawnflags ) & SECRET_NO_SHOOT ) )
	{
		self->th_pain = (th_pain_funcref_t) (0);	//SUB_Null;
		self->s.v.takedamage = DAMAGE_NO;
	}

	VectorClear( self->s.v.velocity );

	// Make a sound, wait a little...

	sound( self, CHAN_VOICE, self->noise1, 1, ATTN_NORM );
	self->s.v.nextthink = self->s.v.ltime + 0.1;

	temp = 1 - ( ( int ) ( self->s.v.spawnflags ) & SECRET_1ST_LEFT );	// 1 or -1
	trap_makevectors( self->mangle );

	if ( self->t_width == 0 )
	{
		if ( ( int ) ( self->s.v.spawnflags ) & SECRET_1ST_DOWN )
			self->t_width =  fabsf( DotProduct( g_globalvars.v_up, self->s.v.size ) );
		else
			self->t_width =  fabsf( DotProduct( g_globalvars.v_right, self->s.v.size ));
	}

	if ( self->t_length == 0 )
		self->t_length =
		    fabsf( DotProduct( g_globalvars.v_forward, self->s.v.size ) );

	if ( ( int ) ( self->s.v.spawnflags ) & SECRET_1ST_DOWN )
	{
		self->dest1[0] =
		    self->s.v.origin[0] - g_globalvars.v_up[0] * self->t_width;
		self->dest1[1] =
		    self->s.v.origin[1] - g_globalvars.v_up[1] * self->t_width;
		self->dest1[2] =
		    self->s.v.origin[2] - g_globalvars.v_up[2] * self->t_width;
	} else
	{
		self->dest1[0] =
		    self->s.v.origin[0] +
		    g_globalvars.v_right[0] * ( self->t_width * temp );
		self->dest1[1] =
		    self->s.v.origin[1] +
		    g_globalvars.v_right[1] * ( self->t_width * temp );
		self->dest1[2] =
		    self->s.v.origin[2] +
		    g_globalvars.v_right[2] * ( self->t_width * temp );
	}

	self->dest2[0] = self->dest1[0] + g_globalvars.v_forward[0] * self->t_length;
	self->dest2[1] = self->dest1[1] + g_globalvars.v_forward[1] * self->t_length;
	self->dest2[2] = self->dest1[2] + g_globalvars.v_forward[2] * self->t_length;

	SUB_CalcMove( self->dest1, self->speed, fd_secret_move1 );
	sound( self, CHAN_VOICE, self->noise2, 1, ATTN_NORM );
}

// Wait after first movement...
void fd_secret_move1()
{
	self->s.v.nextthink = self->s.v.ltime + 1.0;
	self->think = ( func_t ) fd_secret_move2;
	sound( self, CHAN_VOICE, self->noise3, 1, ATTN_NORM );
}

// Start moving sideways w/sound...
void fd_secret_move2()
{
	sound( self, CHAN_VOICE, self->noise2, 1, ATTN_NORM );
	SUB_CalcMove( self->dest2, self->speed, fd_secret_move3 );
}

// Wait here until time to go back...
void fd_secret_move3()
{
	sound( self, CHAN_VOICE, self->noise3, 1, ATTN_NORM );
	if ( !( ( int ) ( self->s.v.spawnflags ) & SECRET_OPEN_ONCE ) )
	{
		self->s.v.nextthink = self->s.v.ltime + self->wait;
		self->think = ( func_t ) fd_secret_move4;
	}
}

// Move backward...
void fd_secret_move4()
{
	sound( self, CHAN_VOICE, self->noise2, 1, ATTN_NORM );
	SUB_CalcMove( self->dest1, self->speed, fd_secret_move5 );
}

// Wait 1 second...
void fd_secret_move5()
{
	self->s.v.nextthink = self->s.v.ltime + 1.0;
	self->think = ( func_t ) fd_secret_move6;
	sound( self, CHAN_VOICE, self->noise3, 1, ATTN_NORM );
}

void fd_secret_move6()
{
	sound( self, CHAN_VOICE, self->noise2, 1, ATTN_NORM );
	SUB_CalcMove( self->s.v.oldorigin, self->speed, fd_secret_done );
}

void fd_secret_done()
{
	if ( !self->targetname
	     || ( int ) ( self->s.v.spawnflags ) & SECRET_YES_SHOOT )
	{
		self->s.v.health = 10000;
		self->s.v.takedamage = DAMAGE_YES;
		self->th_pain = fd_secret_use;
		self->th_die = ( void ( * )() ) fd_secret_use;
	}
	sound( self, CHAN_NO_PHS_ADD + CHAN_VOICE, self->noise3, 1, ATTN_NORM );
}

void secret_blocked()
{
	if (g_globalvars.time < self->attack_finished) {
		return;
	}

	self->attack_finished = g_globalvars.time + 0.5;
	other->deathtype = dtSQUISH;

	if (self->think1 == fd_secret_move1) {
		T_Damage(other, self, self, self->dmg * 100);
	}
	else {
		T_Damage(other, self, self, self->dmg);
	}
}

/*
================
secret_touch

Prints messages
================
*/
void secret_touch()
{
// return if countdown or map frozen
	if( !k_practice ) // #practice mode#
	if( match_in_progress == 1
		|| ( !match_in_progress && cvar( "k_freeze" ) )
	  )
		return;

	if ( other->ct != ctPlayer )
		return;
	
	if ( self->attack_finished > g_globalvars.time )
		return;

	self->attack_finished = g_globalvars.time + 2;

	if ( self->message )
	{
		G_centerprint( other, "%s", self->message );
		sound( other, CHAN_BODY, "misc/talk.wav", 1, ATTN_NORM );
	}
}


/*QUAKED func_door_secret (0 .5 .8) ? open_once 1st_left 1st_down no_shoot always_shoot
Basic secret door. Slides back, then to the side. Angle determines direction.
wait  = # of seconds before coming back
1st_left = 1st move is left of arrow
1st_down = 1st move is down from arrow
always_shoot = even if targeted, keep shootable
t_width = override WIDTH to move back (or height if going down)
t_length = override LENGTH to move sideways
"dmg"           damage to inflict when blocked (2 default)

If a secret door has a targetname, it will only be opened by it's botton or trigger, not by damage.
"sounds"
1) medieval
2) metal
3) base
*/

void SP_func_door_secret()
{
	if ( self->s.v.sounds == 0 )
		self->s.v.sounds = 3;
	if ( self->s.v.sounds == 1 )
	{
		trap_precache_sound( "doors/latch2.wav" );
		trap_precache_sound( "doors/winch2.wav" );
		trap_precache_sound( "doors/drclos4.wav" );
		self->noise1 = "doors/latch2.wav";
		self->noise2 = "doors/winch2.wav";
		self->noise3 = "doors/drclos4.wav";
	}
	if ( self->s.v.sounds == 2 )
	{
		trap_precache_sound( "doors/airdoor1.wav" );
		trap_precache_sound( "doors/airdoor2.wav" );
		self->noise2 = "doors/airdoor1.wav";
		self->noise1 = "doors/airdoor2.wav";
		self->noise3 = "doors/airdoor2.wav";
	}
	if ( self->s.v.sounds == 3 )
	{
		trap_precache_sound( "doors/basesec1.wav" );
		trap_precache_sound( "doors/basesec2.wav" );
		self->noise2 = "doors/basesec1.wav";
		self->noise1 = "doors/basesec2.wav";
		self->noise3 = "doors/basesec2.wav";
	}

	if ( self->dmg == 0 )
		self->dmg = 2;

	// Magic formula...
	VectorCopy( self->s.v.angles, self->mangle );
	SetVector( self->s.v.angles, 0, 0, 0 );
	self->s.v.solid = SOLID_BSP;
	self->s.v.movetype = MOVETYPE_PUSH;
	self->classname = "door";
	setmodel( self, self->model );
	setorigin( self, PASSVEC3( self->s.v.origin ) );

	self->touch   = ( func_t ) secret_touch;
	self->blocked = ( func_t ) secret_blocked;
	self->speed = 50;
	self->use     = ( func_t ) fd_secret_use;
	if ( !self->targetname
	     || ( int ) ( self->s.v.spawnflags ) & SECRET_YES_SHOOT )
	{
		self->s.v.health = 10000;
		self->s.v.takedamage = DAMAGE_YES;
		self->th_pain = fd_secret_use;
	}
	VectorCopy( self->s.v.origin, self->s.v.oldorigin );
// self.oldorigin = self.origin;
	if ( self->wait == 0 )
		self->wait = 5;	// 5 seconds before closing
}
