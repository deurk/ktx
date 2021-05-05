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

//============================================================================

#define START_OFF 1

void light_use()
{
	if ( ( int ) ( self->s.v.spawnflags ) & START_OFF )
	{
		trap_lightstyle( self->style, "m" );
		self->s.v.spawnflags -= START_OFF;
	} else
	{
		trap_lightstyle( self->style, "a" );
		self->s.v.spawnflags += START_OFF;
	}
}

/*QUAKED light (0 1 0) (-8 -8 -8) (8 8 8) START_OFF
Non-displayed light.
Default light value is 300
Default style is 0
If targeted, it will toggle between on or off.
*/
void SP_light()
{

	if ( !self->targetname )
	{			// inert light
		ent_remove( self );
		return;
	}

	if ( self->style >= 32 )
	{
		self->use = ( func_t ) light_use;
		if ( ( int ) self->s.v.spawnflags & START_OFF )
			trap_lightstyle( self->style, "a" );
		else
			trap_lightstyle( self->style, "m" );
	}
}

/*QUAKED light_fluoro (0 1 0) (-8 -8 -8) (8 8 8) START_OFF
Non-displayed light.
Default light value is 300
Default style is 0
If targeted, it will toggle between on or off.
Makes steady fluorescent humming sound
*/
void SP_light_fluoro()
{
	if ( self->style >= 32 )
	{
		self->use = ( func_t ) light_use;
		if ( ( int ) self->s.v.spawnflags & START_OFF )
			trap_lightstyle( self->style, "a" );
		else
			trap_lightstyle( self->style, "m" );
	}

	trap_precache_sound( "ambience/fl_hum1.wav" );
	trap_ambientsound( PASSVEC3( self->s.v.origin ), "ambience/fl_hum1.wav", 0.5,
			   ATTN_STATIC );
}


/*QUAKED light_fluorospark (0 1 0) (-8 -8 -8) (8 8 8)
Non-displayed light.
Default light value is 300
Default style is 10
Makes sparking, broken fluorescent sound
*/
void SP_light_fluorospark()
{
	if ( !self->style )
		self->style = 10;

	trap_precache_sound( "ambience/buzz1.wav" );
	trap_ambientsound( PASSVEC3( self->s.v.origin ), "ambience/buzz1.wav", 0.5,
			   ATTN_STATIC );
}

/*QUAKED light_globe (0 1 0) (-8 -8 -8) (8 8 8)
Sphere globe light.
Default light value is 300
Default style is 0
*/
void SP_light_globe()
{
	trap_precache_model( "progs/s_light.spr" );
	setmodel( self, "progs/s_light.spr" );
	makestatic( self );
}

void FireAmbient()
{
	trap_precache_sound( "ambience/fire1.wav" );
// attenuate fast
	trap_ambientsound( PASSVEC3( self->s.v.origin ), "ambience/fire1.wav", 0.5,
			   ATTN_STATIC );
}

/*QUAKED light_torch_small_walltorch (0 .5 0) (-10 -10 -20) (10 10 20)
Short wall torch
Default light value is 200
Default style is 0
*/
void SP_light_torch_small_walltorch()
{
	trap_precache_model( "progs/flame.mdl" );
	setmodel( self, "progs/flame.mdl" );
	FireAmbient();
	makestatic( self );
}

/*QUAKED light_flame_large_yellow (0 1 0) (-10 -10 -12) (12 12 18)
Large yellow flame ball
*/
void SP_light_flame_large_yellow()
{
	trap_precache_model( "progs/flame2.mdl" );
	setmodel( self, "progs/flame2.mdl" );
	self->s.v.frame = 1;
	FireAmbient();
	makestatic( self );
}

/*QUAKED light_flame_small_yellow (0 1 0) (-8 -8 -8) (8 8 8) START_OFF
Small yellow flame ball
*/
void SP_light_flame_small_yellow()
{
	trap_precache_model( "progs/flame2.mdl" );
	setmodel( self, "progs/flame2.mdl" );
	FireAmbient();
	makestatic( self );
}

/*QUAKED light_flame_small_white (0 1 0) (-10 -10 -40) (10 10 40) START_OFF
Small white flame ball
*/
void SP_light_flame_small_white()
{
	trap_precache_model( "progs/flame2.mdl" );
	setmodel( self, "progs/flame2.mdl" );
	FireAmbient();
	makestatic( self );
}

//============================================================================


/*QUAKED misc_fireball (0 .5 .8) (-8 -8 -8) (8 8 8)
Lava Balls
*/

void            fire_fly();
void            fire_touch();

void SP_misc_fireball()
{
	trap_precache_model( "progs/lavaball.mdl" );
	self->classname = "fireball";
	self->s.v.nextthink = g_globalvars.time + ( g_random() * 5 );
	self->think = ( func_t ) fire_fly;

	if ( !self->speed )
		self->speed = 1000;
}

void fire_fly()
{
	gedict_t       *fireball;

	fireball = spawn();
	fireball->s.v.solid = SOLID_TRIGGER;
	fireball->s.v.movetype = MOVETYPE_TOSS;
	fireball->isMissile = true; // well, you can really treat fireball as missilie, nothing bad gonna heppen.
	SetVector( fireball->s.v.velocity,
		   ( g_random() * 100 ) - 50,
		   ( g_random() * 100 ) - 50, self->speed + ( g_random() * 200 ) );

	fireball->classname = "fireball";

	setmodel( fireball, "progs/lavaball.mdl" );
	setsize( fireball, 0, 0, 0, 0, 0, 0 );
	setorigin( fireball, PASSVEC3( self->s.v.origin ) );

	fireball->s.v.nextthink = g_globalvars.time + 5;
	fireball->think = ( func_t ) SUB_Remove;
	fireball->touch = ( func_t ) fire_touch;

	self->s.v.nextthink = g_globalvars.time + ( g_random() * 5 ) + 3;
	self->think = ( func_t ) fire_fly;
}


void fire_touch()
{
	// Yawnmode: no damage from fireall
	if ( !k_yawnmode ) {
		other->deathtype = dtFIREBALL;
		T_Damage( other, self, self, 20 );
	}

	ent_remove( self );
}

//============================================================================


void barrel_explode()
{
	self->s.v.takedamage = DAMAGE_NO;
	self->classname = "explo_box";

	// did say self.owner
	T_RadiusDamage(self, self, 160, world, dtEXPLO_BOX);

	WriteByte( MSG_MULTICAST, SVC_TEMPENTITY );
	WriteByte( MSG_MULTICAST, TE_EXPLOSION );
	WriteCoord( MSG_MULTICAST, self->s.v.origin[0] );
	WriteCoord( MSG_MULTICAST, self->s.v.origin[1] );
	WriteCoord( MSG_MULTICAST, self->s.v.origin[2] + 32 );

	trap_multicast( PASSVEC3( self->s.v.origin ), MULTICAST_PHS );

	ent_remove( self );
}



/*QUAKED misc_explobox (0 .5 .8) (0 0 0) (32 32 64)
TESTING THING
*/

void SP_misc_explobox()
{
	float           oldz;

	self->s.v.solid = SOLID_BBOX;
	self->s.v.movetype = MOVETYPE_NONE;

	trap_precache_model( "maps/b_explob.bsp" );
	setmodel( self, "maps/b_explob.bsp" );
	setsize( self, 0, 0, 0, 32, 32, 64 );
	trap_precache_sound( "weapons/r_exp3.wav" );

	self->s.v.health = 20;
	self->th_die = barrel_explode;
	self->s.v.takedamage = DAMAGE_AIM;

	self->s.v.origin[2] += 2;
	oldz = self->s.v.origin[2];
	droptofloor( self );
	if ( oldz - self->s.v.origin[2] > 250 )
	{
		G_Printf( "item fell out of level at '%f %f %f'\n",
			  PASSVEC3( self->s.v.origin ) );
		ent_remove( self );
	}
}




/*QUAKED misc_explobox2 (0 .5 .8) (0 0 0) (32 32 64)
Smaller exploding box, REGISTERED ONLY
*/

void SP_misc_explobox2()
{
	float           oldz;

	self->s.v.solid = SOLID_BBOX;
	self->s.v.movetype = MOVETYPE_NONE;

	trap_precache_model( "maps/b_exbox2.bsp" );
	setmodel( self, "maps/b_exbox2.bsp" );
	setsize( self, 0, 0, 0, 32, 32, 32 );
	trap_precache_sound( "weapons/r_exp3.wav" );

	self->s.v.health = 20;
	self->th_die = barrel_explode;
	self->s.v.takedamage = DAMAGE_AIM;

	self->s.v.origin[2] += 2;
	oldz = self->s.v.origin[2];

	droptofloor( self );

	if ( oldz - self->s.v.origin[2] > 250 )
	{
		G_Printf( "item fell out of level at '%f %f %f'\n",
			  PASSVEC3( self->s.v.origin ) );
		ent_remove( self );
	}
}

//============================================================================

#define SPAWNFLAG_SUPERSPIKE      1
#define SPAWNFLAG_LASER  2

void Laser_Touch()
{
	vec3_t          org;

	if ( other == PROG_TO_EDICT( self->s.v.owner ) )
		return;		// don't explode on owner

	if ( trap_pointcontents( PASSVEC3( self->s.v.origin ) ) == CONTENT_SKY )
	{
		ent_remove( self );
		return;
	}

	sound( self, CHAN_WEAPON, "enforcer/enfstop.wav", 1, ATTN_STATIC );
	
	normalize( self->s.v.velocity, org );
	VectorScale( org, 8, org );
	VectorSubtract( self->s.v.origin, org, org );
	//org = self->s.v.origin - 8*normalize(self->s.v.velocity);

	if ( ISLIVE( other ) )
	{
		SpawnBlood( org, 15 );
		other->deathtype = dtLASER;
		T_Damage( other, self, PROG_TO_EDICT( self->s.v.owner ), 15 );
	} else
	{
		WriteByte( MSG_MULTICAST, SVC_TEMPENTITY );
		WriteByte( MSG_MULTICAST, TE_GUNSHOT );
		WriteByte( MSG_MULTICAST, 5 );
		WriteCoord( MSG_MULTICAST, org[0] );
		WriteCoord( MSG_MULTICAST, org[1] );
		WriteCoord( MSG_MULTICAST, org[2] );

		trap_multicast( PASSVEC3( org ), MULTICAST_PVS );
	}

	ent_remove( self );
}

gedict_t       *newmis; // FIXME: funny place for such global

void LaunchLaser( vec3_t org, vec3_t vec )
{
	//vec3_t  vec;

	if ( streq( self->classname, "monster_enforcer" ) )
		sound( self, CHAN_WEAPON, "enforcer/enfire.wav", 1, ATTN_NORM );

	normalize( vec, vec );

	newmis = spawn();
	g_globalvars.newmis = EDICT_TO_PROG( newmis );
	newmis->s.v.owner = EDICT_TO_PROG( self );
	newmis->s.v.movetype = MOVETYPE_FLY;
	newmis->isMissile = true;
	newmis->s.v.solid = SOLID_BBOX;
	newmis->s.v.effects = EF_DIMLIGHT;

	setmodel( newmis, "progs/laser.mdl" );
	setsize( newmis, 0, 0, 0, 0, 0, 0 );

	setorigin( newmis, PASSVEC3( org ) );

	//newmis->s.v.velocity = vec * 600;
	VectorScale( vec, 600, newmis->s.v.velocity );

	vectoangles( newmis->s.v.velocity, newmis->s.v.angles );

	newmis->s.v.nextthink = g_globalvars.time + 5;
	newmis->think = ( func_t ) SUB_Remove;
	newmis->touch = ( func_t ) Laser_Touch;
}

void            spike_touch();
void            superspike_touch();

void spikeshooter_use()
{
	if ( ( int ) ( self->s.v.spawnflags ) & SPAWNFLAG_LASER )
	{
		sound( self, CHAN_VOICE, "enforcer/enfire.wav", 1, ATTN_NORM );
		LaunchLaser( self->s.v.origin, self->s.v.movedir );
	} else
	{
		sound( self, CHAN_VOICE, "weapons/spike2.wav", 1, ATTN_NORM );
		launch_spike( self->s.v.origin, self->s.v.movedir );
		VectorScale( self->s.v.movedir, 500,
			     PROG_TO_EDICT( g_globalvars.newmis )->s.v.velocity );
//  newmis->s.v.velocity = self->s.v.movedir * 500;
		if ( ( int ) ( self->s.v.spawnflags ) & SPAWNFLAG_SUPERSPIKE )
			PROG_TO_EDICT( g_globalvars.newmis )->touch =
			    ( func_t ) superspike_touch;
	}
}

void shooter_think()
{
	spikeshooter_use();
	self->s.v.nextthink = g_globalvars.time + self->wait;
	VectorScale( self->s.v.movedir, 500,
		     PROG_TO_EDICT( g_globalvars.newmis )->s.v.velocity );
// newmis->s.v.velocity = self->s.v.movedir * 500;
}


/*QUAKED trap_spikeshooter (0 .5 .8) (-8 -8 -8) (8 8 8) superspike laser
When triggered, fires a spike in the direction set in QuakeEd.
Laser is only for REGISTERED.
*/

void SP_trap_spikeshooter()
{
	SetMovedir();
	self->use = ( func_t ) spikeshooter_use;
	if ( ( int ) ( self->s.v.spawnflags ) & SPAWNFLAG_LASER )
	{
		trap_precache_model( "progs/laser.mdl" );

		trap_precache_sound( "enforcer/enfire.wav" );
		trap_precache_sound( "enforcer/enfstop.wav" );
	} else
		trap_precache_sound( "weapons/spike2.wav" );
}


/*QUAKED trap_shooter (0 .5 .8) (-8 -8 -8) (8 8 8) superspike laser
Continuously fires spikes.
"wait" g_globalvars.time between spike (1.0 default)
"nextthink" delay before firing first spike, so multiple shooters can be stagered.
*/
void SP_trap_shooter()
{
	SP_trap_spikeshooter();

	if ( self->wait == 0 )
		self->wait = 1;
	self->s.v.nextthink = self->s.v.nextthink + self->wait + self->s.v.ltime;
	self->think = ( func_t ) shooter_think;
}

/*
===============================================================================


===============================================================================
*/


void            make_bubbles();
void            bubble_remove();
void            bubble_bob();

/*QUAKED air_bubbles (0 .5 .8) (-8 -8 -8) (8 8 8)

testing air bubbles
*/

void SP_air_bubbles()
{
	ent_remove( self );
}

void make_bubbles()
{
	gedict_t       *bubble;

	bubble = spawn();
	setmodel( bubble, "progs/s_bubble.spr" );
	setorigin( bubble, PASSVEC3( self->s.v.origin ) );
	bubble->s.v.movetype = MOVETYPE_NOCLIP;
	bubble->s.v.solid = SOLID_NOT;

	SetVector( bubble->s.v.velocity, 0, 0, 15 );
	bubble->s.v.nextthink = g_globalvars.time + 0.5;
	bubble->think = ( func_t ) bubble_bob;
	bubble->touch = ( func_t ) bubble_remove;
	bubble->classname = "bubble";
	bubble->s.v.frame = 0;
	bubble->cnt = 0;

	setsize( bubble, -8, -8, -8, 8, 8, 8 );

	self->s.v.nextthink = g_globalvars.time + g_random() + 0.5;
	self->think = ( func_t ) make_bubbles;
}

void bubble_split()
{
	gedict_t       *bubble;

	bubble = spawn();
	setmodel( bubble, "progs/s_bubble.spr" );
	setorigin( bubble, PASSVEC3( self->s.v.origin ) );

	bubble->s.v.movetype = MOVETYPE_NOCLIP;
	bubble->s.v.solid = SOLID_NOT;

	VectorCopy( self->s.v.velocity, bubble->s.v.velocity );

	bubble->s.v.nextthink = g_globalvars.time + 0.5;
	bubble->think = ( func_t ) bubble_bob;
	bubble->touch = ( func_t ) bubble_remove;
	bubble->classname = "bubble";
	bubble->s.v.frame = 1;
	bubble->cnt = 10;

	setsize( bubble, -8, -8, -8, 8, 8, 8 );

	self->s.v.frame = 1;
	self->cnt = 10;

	if ( self->s.v.waterlevel != 3 )
		ent_remove( self );
}

void bubble_remove()
{
	if ( streq( other->classname, self->classname ) )
	{
//              dprint ("bump");
		return;
	}
	ent_remove( self );
}

void bubble_bob()
{
	float           rnd1, rnd2, rnd3;

//vec3_t    vtmp1, modi;

	self->cnt = self->cnt + 1;
	if ( self->cnt == 4 )
		bubble_split();
	if ( self->cnt == 20 )
		ent_remove( self );

	rnd1 = self->s.v.velocity[0] + ( -10 + ( g_random() * 20 ) );
	rnd2 = self->s.v.velocity[1] + ( -10 + ( g_random() * 20 ) );
	rnd3 = self->s.v.velocity[2] + 10 + g_random() * 10;

	if ( rnd1 > 10 )
		rnd1 = 5;
	if ( rnd1 < -10 )
		rnd1 = -5;

	if ( rnd2 > 10 )
		rnd2 = 5;
	if ( rnd2 < -10 )
		rnd2 = -5;

	if ( rnd3 < 10 )
		rnd3 = 15;
	if ( rnd3 > 30 )
		rnd3 = 25;

	self->s.v.velocity[0] = rnd1;
	self->s.v.velocity[1] = rnd2;
	self->s.v.velocity[2] = rnd3;

	self->s.v.nextthink = g_globalvars.time + 0.5;
	self->think = ( func_t ) bubble_bob;
}

/*~>~<~>~<~>~<~>~<~>~<~>~<~>~<~>~<~>~<~>~<~>~<~>~<~>~<~>~<~>~<~>~<~>~<~>
~>~<~>~<~>~<~>~<~>~<~>~<~>~<~>~<~>~<~>~<~>~<~>~<~>~<~>~<~>~<~>~<~>~<~>~*/

/*QUAKED viewthing (0 .5 .8) (-8 -8 -8) (8 8 8)

Just for the debugging level.  Don't use
*/

void viewthing()
{
	self->s.v.movetype = MOVETYPE_NONE;
	self->s.v.solid = SOLID_NOT;
	trap_precache_model( "progs/player.mdl" );
	setmodel( self, "progs/player.mdl" );
}



/*
==============================================================================

SIMPLE BMODELS

==============================================================================
*/

void func_wall_use()
{				// change to alternate textures
	self->s.v.frame = 1 - self->s.v.frame;
}

/*QUAKED func_wall (0 .5 .8) ?
This is just a solid wall if not inhibitted
*/
void SP_func_wall()
{
	SetVector( self->s.v.angles, 0, 0, 0 );
	self->s.v.movetype = MOVETYPE_PUSH;	// so it doesn't get pushed by anything
	self->s.v.solid = SOLID_BSP;
	self->use = ( func_t ) func_wall_use;
	setmodel( self, self->model );
}


/*QUAKED func_illusionary (0 .5 .8) ?
A simple entity that looks solid but lets you walk through it.
*/
void SP_func_illusionary()
{
	SetVector( self->s.v.angles, 0, 0, 0 );
	self->s.v.movetype = MOVETYPE_NONE;
	self->s.v.solid = SOLID_NOT;
	setmodel( self, self->model );
	makestatic( self );
}

/*QUAKED func_episodegate (0 .5 .8) ? E1 E2 E3 E4
This bmodel will appear if the episode has allready been completed, so players can't reenter it.
*/
void SP_func_episodegate()
{
	if ( !( ( int ) ( g_globalvars.serverflags ) & ( int ) ( self->s.v.spawnflags ) ) )
		return;		// can still enter episode

	SetVector( self->s.v.angles, 0, 0, 0 );
	self->s.v.movetype = MOVETYPE_PUSH;	// so it doesn't get pushed by anything
	self->s.v.solid = SOLID_BSP;
	self->use = ( func_t ) func_wall_use;
	setmodel( self, self->model );
}

/*QUAKED func_bossgate (0 .5 .8) ?
This bmodel appears unless players have all of the episode sigils.
*/
void SP_func_bossgate()
{
	if ( ( ( int ) ( g_globalvars.serverflags ) & 15 ) == 15 )
		return;		// all episodes completed
	SetVector( self->s.v.angles, 0, 0, 0 );
	self->s.v.movetype = MOVETYPE_PUSH;	// so it doesn't get pushed by anything
	self->s.v.solid = SOLID_BSP;
	self->use = ( func_t ) func_wall_use;
	setmodel( self, self->model );
}

//============================================================================
/*QUAKED ambient_suck_wind (0.3 0.1 0.6) (-10 -10 -8) (10 10 8)
*/
void SP_ambient_suck_wind()
{
	trap_precache_sound( "ambience/suck1.wav" );
	trap_ambientsound( PASSVEC3( self->s.v.origin ), "ambience/suck1.wav", 1,
			   ATTN_STATIC );
}

/*QUAKED ambient_drone (0.3 0.1 0.6) (-10 -10 -8) (10 10 8)
*/
void SP_ambient_drone()
{
	trap_precache_sound( "ambience/drone6.wav" );
	trap_ambientsound( PASSVEC3( self->s.v.origin ), "ambience/drone6.wav", 0.5,
			   ATTN_STATIC );
}

/*QUAKED ambient_flouro_buzz (0.3 0.1 0.6) (-10 -10 -8) (10 10 8)
*/
void SP_ambient_flouro_buzz()
{
	trap_precache_sound( "ambience/buzz1.wav" );
	trap_ambientsound( PASSVEC3( self->s.v.origin ), "ambience/buzz1.wav", 1,
			   ATTN_STATIC );
}

/*QUAKED ambient_drip (0.3 0.1 0.6) (-10 -10 -8) (10 10 8)
*/
void SP_ambient_drip()
{
	trap_precache_sound( "ambience/drip1.wav" );
	trap_ambientsound( PASSVEC3( self->s.v.origin ), "ambience/drip1.wav", 0.5,
			   ATTN_STATIC );
}

/*QUAKED ambient_comp_hum (0.3 0.1 0.6) (-10 -10 -8) (10 10 8)
*/
void SP_ambient_comp_hum()
{
	trap_precache_sound( "ambience/comp1.wav" );
	trap_ambientsound( PASSVEC3( self->s.v.origin ), "ambience/comp1.wav", 1,
			   ATTN_STATIC );
}

/*QUAKED ambient_thunder (0.3 0.1 0.6) (-10 -10 -8) (10 10 8)
*/
void SP_ambient_thunder()
{
	trap_precache_sound( "ambience/thunder1.wav" );
	trap_ambientsound( PASSVEC3( self->s.v.origin ), "ambience/thunder1.wav", 0.5,
			   ATTN_STATIC );
}

/*QUAKED ambient_light_buzz (0.3 0.1 0.6) (-10 -10 -8) (10 10 8)
*/
void SP_ambient_light_buzz()
{
	trap_precache_sound( "ambience/fl_hum1.wav" );
	trap_ambientsound( PASSVEC3( self->s.v.origin ), "ambience/fl_hum1.wav", 0.5,
			   ATTN_STATIC );
}

/*QUAKED ambient_swamp1 (0.3 0.1 0.6) (-10 -10 -8) (10 10 8)
*/
void SP_ambient_swamp1()
{
	trap_precache_sound( "ambience/swamp1.wav" );
	trap_ambientsound( PASSVEC3( self->s.v.origin ), "ambience/swamp1.wav", 0.5,
			   ATTN_STATIC );
}

/*QUAKED ambient_swamp2 (0.3 0.1 0.6) (-10 -10 -8) (10 10 8)
*/
void SP_ambient_swamp2()
{
	trap_precache_sound( "ambience/swamp2.wav" );
	trap_ambientsound( PASSVEC3( self->s.v.origin ), "ambience/swamp2.wav", 0.5,
			   ATTN_STATIC );
}

//============================================================================

void noise_think()
{
	self->s.v.nextthink = g_globalvars.time + 0.5;
	sound( self, 1, "enforcer/enfire.wav", 1, ATTN_NORM );
	sound( self, 2, "enforcer/enfstop.wav", 1, ATTN_NORM );
	sound( self, 3, "enforcer/sight1.wav", 1, ATTN_NORM );
	sound( self, 4, "enforcer/sight2.wav", 1, ATTN_NORM );
	sound( self, 5, "enforcer/sight3.wav", 1, ATTN_NORM );
	sound( self, 6, "enforcer/sight4.wav", 1, ATTN_NORM );
	sound( self, 7, "enforcer/pain1.wav", 1, ATTN_NORM );
}

/*QUAKED misc_noisemaker (1 0.5 0) (-10 -10 -10) (10 10 10)

For optimzation testing, starts a lot of sounds.
*/

void SP_misc_noisemaker()
{
	trap_precache_sound( "enforcer/enfire.wav" );
	trap_precache_sound( "enforcer/enfstop.wav" );
	trap_precache_sound( "enforcer/sight1.wav" );
	trap_precache_sound( "enforcer/sight2.wav" );
	trap_precache_sound( "enforcer/sight3.wav" );
	trap_precache_sound( "enforcer/sight4.wav" );
	trap_precache_sound( "enforcer/pain1.wav" );
	trap_precache_sound( "enforcer/pain2.wav" );
	trap_precache_sound( "enforcer/death1.wav" );
	trap_precache_sound( "enforcer/idle1.wav" );

	self->s.v.nextthink = g_globalvars.time + 0.1 + g_random();
	self->think = ( func_t ) noise_think;
}
