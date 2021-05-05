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

/*
QuakeEd only writes a single float for angles (bad idea), so up and down are
just constant angles.
*/
void SetMovedir()
{
	if ( VectorCompareF( self->s.v.angles, 0, -1, 0 ) )
		SetVector( self->s.v.movedir, 0, 0, 1 );
	else if ( VectorCompareF( self->s.v.angles, 0, -2, 0 ) )
		SetVector( self->s.v.movedir, 0, 0, -1 );
	else
	{
		trap_makevectors( self->s.v.angles );
		VectorCopy( g_globalvars.v_forward, self->s.v.movedir );
	}
	SetVector( self->s.v.angles, 0, 0, 0 );
}

/*
================
InitTrigger
================
*/
void InitTrigger()
{
// trigger angles are used for one-way touches.  An angle of 0 is assumed
// to mean no restrictions, so use a yaw of 360 instead.
	if ( !VectorCompareF( self->s.v.angles, 0, 0, 0 ) )
		SetMovedir();
	self->s.v.solid = SOLID_TRIGGER;
	setmodel( self, self->model );	// set size and link into world
	self->s.v.movetype = MOVETYPE_NONE;
	self->s.v.modelindex = 0;
	self->model = "";
}


/*
=============
SUB_CalcMove

calculate self.velocity and self.nextthink to reach dest from
self.origin traveling at speed
===============
*/
void            SUB_CalcMoveDone();
void SUB_CalcMoveEnt( gedict_t * ent, vec3_t tdest, float tspeed, void ( *func ) () )
{
	gedict_t       *stemp;

	stemp = self;
	self = ent;

	SUB_CalcMove( tdest, tspeed, func );
	self = stemp;
}

void SUB_CalcMove( vec3_t tdest, float tspeed, void ( *func ) () )
{
	vec3_t          vdestdelta;
	float           len, traveltime;

	if ( !tspeed )
		G_Error( "No speed is defined!" );

	self->think1 = func;
	VectorCopy( tdest, self->finaldest );
	self->think = ( func_t ) SUB_CalcMoveDone;

	if ( VectorCompare( tdest, self->s.v.origin ) )
	{
		SetVector( self->s.v.velocity, 0, 0, 0 );
		self->s.v.nextthink = self->s.v.ltime + 0.1;
		return;
	}
// set destdelta to the vector needed to move
	VectorSubtract( tdest, self->s.v.origin, vdestdelta )
// calculate length of vector
    len = vlen( vdestdelta );

// divide by speed to get time to reach dest
	traveltime = len / tspeed;

	if ( traveltime < 0.03 )
		traveltime = 0.03;

// set nextthink to trigger a think when dest is reached
	self->s.v.nextthink = self->s.v.ltime + traveltime;

// scale the destdelta vector by the time spent traveling to get velocity
	VectorScale( vdestdelta, ( 1 / traveltime ), self->s.v.velocity );
	//self.velocity = vdestdelta * (1/traveltime); // qcc won't take vec/float 
}

/*
============
After moving, set origin to exact final destination
============
*/
void SUB_CalcMoveDone()
{
	setorigin( self, PASSVEC3( self->finaldest ) );

	SetVector( self->s.v.velocity, 0, 0, 0 );
	//self->s.v.nextthink = -1;
	if ( self->think1 )
		self->think1();
}


/*
=============
SUB_CalcAngleMove

calculate self.avelocity and self.nextthink to reach destangle from
self.angles rotating 

The calling function should make sure self.think is valid
===============
*/
/*void(entity ent, vector destangle, float tspeed, void() func) SUB_CalcAngleMoveEnt =
{
local entity  stemp;
 stemp = self;
 self = ent;
 SUB_CalcAngleMove (destangle, tspeed, func);
 self = stemp;
}

void SUB_CalcAngleMove(vector destangle, float tspeed, void() func) 
{
local vector destdelta;
local float  len, traveltime;

 if (!tspeed)
  objerror("No speed is defined!");
  
// set destdelta to the vector needed to move
 destdelta = destangle - self.angles;
 
// calculate length of vector
 len = vlen (destdelta);
 
// divide by speed to get time to reach dest
 traveltime = len / tspeed;

// set nextthink to trigger a think when dest is reached
 self.nextthink = self.ltime + traveltime;

// scale the destdelta vector by the time spent traveling to get velocity
 self.avelocity = destdelta * (1 / traveltime);
 
 self.think1 = func;
 self.finalangle = destangle;
 self.think = SUB_CalcAngleMoveDone;
}*/

/*
============
After rotating, set angle to exact final angle
============
*/
/*void SUB_CalcAngleMoveDone(gedict_t*self)
{

 VectorCopy(self->finalangle,self->s.v.angles ); 
 SetVector(self->s.v.avelocity,0,0,0);
 self->s.v.nextthink = -1;
 if (self->think1)
  self->think1();
}*/

//=============================================================================
void            SUB_UseTargets();
gedict_t       *activator;
void DelayThink()
{

	activator = PROG_TO_EDICT( self->s.v.enemy );

	SUB_UseTargets();
	ent_remove( self );
}


/*
==============================
SUB_UseTargets

the global "activator" should be set to the entity that initiated the firing.

If self.delay is set, a DelayedUse entity will be created that will actually
do the SUB_UseTargets after that many seconds have passed.

Centerprints any self.message to the activator.

Removes all entities with a targetname that match self.killtarget,
and removes them, so some events can remove other triggers.

Search for (string)targetname in all entities that
match (string)self.target and call their .use function

==============================
*/
void SUB_UseTargets()
{
	gedict_t       *t, *stemp, *otemp, *act;

//
// check for a delay
//
	if ( self->delay )
	{
		// create a temp object to fire at a later time
		t = spawn();
		t->classname = "DelayedUse";
		t->s.v.nextthink = g_globalvars.time + self->delay;
		t->think = ( func_t ) DelayThink;
		t->s.v.enemy = EDICT_TO_PROG( activator );
		t->message = self->message;
		t->killtarget = self->killtarget;
		t->target = self->target;
		return;
	}

//
// print the message
//activator->classname && 
	if ( activator->ct == ctPlayer && self->message )
		if ( strneq( self->message, "" ) )
		{
			G_centerprint( activator, "%s", self->message );
			if ( !self->noise )
				sound( activator, CHAN_VOICE, "misc/talk.wav", 1,
					    ATTN_NORM );
		}
//
// kill the killtagets
//
	if ( self->killtarget )
	{
		t = world;
		do
		{
			t = find( t, FOFS( targetname ), self->killtarget );
			if ( !t )
				return;
			ent_remove( t );
		}
		while ( 1 );
	}
//
// fire targets
//
	if ( self->target )
	{
		act = activator;
		t = world;
		do
		{

			t = find( t, FOFS( targetname ), self->target );
			if ( !t )
			{
				return;
			}
			stemp = self;
			otemp = other;
			self = t;
			other = stemp;
			//if (self.use != SUB_Null)
			{
				if ( self->use )
					( ( void ( * )() ) ( self->use ) ) ();
			}
			self = stemp;
			other = otemp;
			activator = act;
		}while ( 1 );
	}


}
