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
void            button_wait();
void            button_return();

void button_wait()
{
	self->state = STATE_TOP;
	self->s.v.nextthink = self->s.v.ltime + self->wait;
	self->think = ( func_t ) button_return;
	activator = PROG_TO_EDICT( self->s.v.enemy );

	SUB_UseTargets();

	self->s.v.frame = 1;	// use alternate textures
}

void button_done()
{
	self->state = STATE_BOTTOM;
}

void button_return()
{
	self->state = STATE_DOWN;

	SUB_CalcMove( self->pos1, self->speed, button_done );

	self->s.v.frame = 0;	// use normal textures

	if ( ISLIVE(self) )
		self->s.v.takedamage = DAMAGE_YES;	// can be shot again
}


void button_blocked()
{// do nothing, just don't ome all the way back out
}


void button_fire()
{
	if( !k_practice ) // #practice mode#
   	if( match_in_progress != 2 )
       	return;

	if ( self->state == STATE_UP || self->state == STATE_TOP )
		return;

	sound( self, CHAN_VOICE, self->noise, 1, ATTN_NORM );

	self->state = STATE_UP;

	SUB_CalcMove( self->pos2, self->speed, button_wait );
}


void button_use()
{
	// #practice mode#
	if (!k_practice && match_in_progress != 2) {
		return;
	}

	self->s.v.enemy = EDICT_TO_PROG( activator );
	button_fire();
}

void button_touch()
{
	// #practice mode#
	if (!k_practice && match_in_progress != 2) {
		return;
	}

	if ( other->ct != ctPlayer )
		return;

	self->s.v.enemy = EDICT_TO_PROG( other );

	button_fire();
}

void button_killed()
{
	if (!k_practice && match_in_progress != 2) {
		return;
	}

	if (lgc_enabled()) {
		return;
	}

	self->s.v.enemy = EDICT_TO_PROG( damage_attacker );
	self->s.v.health = self->s.v.max_health;
	self->s.v.takedamage = DAMAGE_NO;	// wil be reset upon return

	button_fire();
}


/*QUAKED func_button (0 .5 .8) ?
When a button is touched, it moves some distance in the direction of it's angle, triggers all of it's targets, waits some time, then returns to it's original position where it can be triggered again.

"angle"  determines the opening direction
"target" all entities with a matching targetname will be used
"speed"  override the default 40 speed
"wait"  override the default 1 second wait (-1 = never return)
"lip"  override the default 4 pixel lip remaining at end of move
"health" if set, the button must be killed instead of touched
"sounds"
0) steam metal
1) wooden clunk
2) metallic click
3) in-out
*/
void SP_func_button()
{
	float           ftemp;

	if ( self->s.v.sounds == 0 )
	{
		trap_precache_sound( "buttons/airbut1.wav" );
		self->noise = "buttons/airbut1.wav";
	}
	if ( self->s.v.sounds == 1 )
	{
		trap_precache_sound( "buttons/switch21.wav" );
		self->noise = "buttons/switch21.wav";
	}
	if ( self->s.v.sounds == 2 )
	{
		trap_precache_sound( "buttons/switch02.wav" );
		self->noise = "buttons/switch02.wav";
	}
	if ( self->s.v.sounds == 3 )
	{
		trap_precache_sound( "buttons/switch04.wav" );
		self->noise = "buttons/switch04.wav";
	}

	SetMovedir();

	self->s.v.movetype = MOVETYPE_PUSH;
	// if button does't have model ( mapper fault? ),
	// we must set it to something safe, because with SOLID_BSP and null model server crashed
	self->s.v.solid = (strnull(self->model) ? SOLID_NOT : SOLID_BSP);
	setmodel( self, self->model );

	self->blocked = ( func_t ) button_blocked;
	self->use = ( func_t ) button_use;

	if ( ISLIVE(self) )
	{
		self->s.v.max_health = self->s.v.health;
		self->th_die = button_killed;
		self->s.v.takedamage = DAMAGE_YES;
	} else
		self->touch = ( func_t ) button_touch;

	if ( !self->speed )
		self->speed = 40;

	if ( !self->wait )
		self->wait = 1;

	if ( !self->lip )
		self->lip = 4;

	self->state = STATE_BOTTOM;

// self->pos1 = self->s.v.origin;
	VectorCopy( self->s.v.origin, self->pos1 );
	ftemp = ( fabsf( DotProduct( self->s.v.movedir, self->s.v.size ) ) - self->lip );

	self->pos2[0] = self->pos1[0] + self->s.v.movedir[0] * ftemp;
	self->pos2[1] = self->pos1[1] + self->s.v.movedir[1] * ftemp;
	self->pos2[2] = self->pos1[2] + self->s.v.movedir[2] * ftemp;
}
