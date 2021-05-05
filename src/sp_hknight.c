/*  Copyright (C) 1996-1997  Id Software, Inc.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA

    See file, 'COPYING', for details.
*/
/*
==============================================================================

HELL KNIGHT

==============================================================================
*/

#include "g_local.h"

enum {

stand1, stand2, stand3, stand4, stand5, stand6, stand7, stand8, stand9,

walk1, walk2, walk3, walk4, walk5, walk6, walk7, walk8, walk9, walk10, walk11,
walk12, walk13, walk14, walk15, walk16, walk17, walk18, walk19, walk20,

run1, run2, run3, run4, run5, run6, run7, run8,

pain1, pain2, pain3, pain4, pain5,

death1, death2, death3, death4, death5, death6, death7, death8, death9, death10, death11, death12,

deathb1, deathb2, deathb3, deathb4, deathb5, deathb6, deathb7, deathb8, deathb9,

char_a1, char_a2, char_a3, char_a4, char_a5, char_a6, char_a7, char_a8,
char_a9, char_a10, char_a11, char_a12, char_a13, char_a14, char_a15, char_a16,

magica1, magica2, magica3, magica4, magica5, magica6, magica7, magica8,
magica9, magica10, magica11, magica12, magica13, magica14,

magicb1, magicb2, magicb3, magicb4, magicb5, magicb6, magicb7, magicb8, magicb9, magicb10, magicb11, magicb12, magicb13,

char_b1, char_b2, char_b3, char_b4, char_b5, char_b6,

slice1, slice2, slice3, slice4, slice5, slice6, slice7, slice8, slice9, slice10,

smash1, smash2, smash3, smash4, smash5, smash6, smash7, smash8, smash9, smash10, smash11,

w_attack1, w_attack2, w_attack3, w_attack4, w_attack5, w_attack6, w_attack7,
w_attack8, w_attack9, w_attack10, w_attack11, w_attack12, w_attack13, w_attack14,
w_attack15, w_attack16, w_attack17, w_attack18, w_attack19, w_attack20, w_attack21, w_attack22,

magicc1, magicc2, magicc3, magicc4, magicc5, magicc6, magicc7, magicc8, magicc9, magicc10, magicc11,

};

void hknight_stand1();
void hknight_stand2();
void hknight_stand3();
void hknight_stand4();
void hknight_stand5();
void hknight_stand6();
void hknight_stand7();
void hknight_stand8();
void hknight_stand9();
void hknight_walk1();
void hknight_walk2();
void hknight_walk3();
void hknight_walk4();
void hknight_walk5();
void hknight_walk6();
void hknight_walk7();
void hknight_walk8();
void hknight_walk9();
void hknight_walk10();
void hknight_walk11();
void hknight_walk12();
void hknight_walk13();
void hknight_walk14();
void hknight_walk15();
void hknight_walk16();
void hknight_walk17();
void hknight_walk18();
void hknight_walk19();
void hknight_walk20();
void hknight_run1();
void hknight_run2();
void hknight_run3();
void hknight_run4();
void hknight_run5();
void hknight_run6();
void hknight_run7();
void hknight_run8();
void hknight_pain1();
void hknight_pain2();
void hknight_pain3();
void hknight_pain4();
void hknight_pain5();
void hknight_die1();
void hknight_die2();
void hknight_die3();
void hknight_die4();
void hknight_die5();
void hknight_die6();
void hknight_die7();
void hknight_die8();
void hknight_die9();
void hknight_die10();
void hknight_die11();
void hknight_die12();
void hknight_dieb1();
void hknight_dieb2();
void hknight_dieb3();
void hknight_dieb4();
void hknight_dieb5();
void hknight_dieb6();
void hknight_dieb7();
void hknight_dieb8();
void hknight_dieb9();
void hknight_magica1();
void hknight_magica2();
void hknight_magica3();
void hknight_magica4();
void hknight_magica5();
void hknight_magica6();
void hknight_magica7();
void hknight_magica8();
void hknight_magica9();
void hknight_magica10();
void hknight_magica11();
void hknight_magica12();
void hknight_magica13();
void hknight_magica14();
void hknight_magicb1();
void hknight_magicb2();
void hknight_magicb3();
void hknight_magicb4();
void hknight_magicb5();
void hknight_magicb6();
void hknight_magicb7();
void hknight_magicb8();
void hknight_magicb9();
void hknight_magicb10();
void hknight_magicb11();
void hknight_magicb12();
void hknight_magicb13();
void hknight_magicc1();
void hknight_magicc2();
void hknight_magicc3();
void hknight_magicc4();
void hknight_magicc5();
void hknight_magicc6();
void hknight_magicc7();
void hknight_magicc8();
void hknight_magicc9();
void hknight_magicc10();
void hknight_magicc11();
void hknight_char_a1();
void hknight_char_a2();
void hknight_char_a3();
void hknight_char_a4();
void hknight_char_a5();
void hknight_char_a6();
void hknight_char_a7();
void hknight_char_a8();
void hknight_char_a9();
void hknight_char_a10();
void hknight_char_a11();
void hknight_char_a12();
void hknight_char_a13();
void hknight_char_a14();
void hknight_char_a15();
void hknight_char_a16();
void hknight_char_b1();
void hknight_char_b2();
void hknight_char_b3();
void hknight_char_b4();
void hknight_char_b5();
void hknight_char_b6();
void hknight_slice1();
void hknight_slice2();
void hknight_slice3();
void hknight_slice4();
void hknight_slice5();
void hknight_slice6();
void hknight_slice7();
void hknight_slice8();
void hknight_slice9();
void hknight_slice10();
void hknight_smash1();
void hknight_smash2();
void hknight_smash3();
void hknight_smash4();
void hknight_smash5();
void hknight_smash6();
void hknight_smash7();
void hknight_smash8();
void hknight_smash9();
void hknight_smash10();
void hknight_smash11();
void hknight_watk1();
void hknight_watk2();
void hknight_watk3();
void hknight_watk4();
void hknight_watk5();
void hknight_watk6();
void hknight_watk7();
void hknight_watk8();
void hknight_watk9();
void hknight_watk10();
void hknight_watk11();
void hknight_watk12();
void hknight_watk13();
void hknight_watk14();
void hknight_watk15();
void hknight_watk16();
void hknight_watk17();
void hknight_watk18();
void hknight_watk19();
void hknight_watk20();
void hknight_watk21();
void hknight_watk22();

//===========================================================================

// used by both walk and run

void hk_idle_sound ()
{
	if ( g_random() < 0.2 )
		sound( self, CHAN_VOICE, "hknight/idle.wav", 1, ATTN_NORM );
}

//===========================================================================

ANIM(hknight_stand1, stand1, hknight_stand2; ai_stand();)
ANIM(hknight_stand2, stand2, hknight_stand3; ai_stand();)
ANIM(hknight_stand3, stand3, hknight_stand4; ai_stand();)
ANIM(hknight_stand4, stand4, hknight_stand5; ai_stand();)
ANIM(hknight_stand5, stand5, hknight_stand6; ai_stand();)
ANIM(hknight_stand6, stand6, hknight_stand7; ai_stand();)
ANIM(hknight_stand7, stand7, hknight_stand8; ai_stand();)
ANIM(hknight_stand8, stand8, hknight_stand9; ai_stand();)
ANIM(hknight_stand9, stand9, hknight_stand1; ai_stand();)

//===========================================================================

void _hknight_walk1( void )
{
	hk_idle_sound();

	ai_walk( 2 );
}
ANIM(hknight_walk1,  walk1,  hknight_walk2;  _hknight_walk1();)
ANIM(hknight_walk2,  walk2,  hknight_walk3;  ai_walk(5);)
ANIM(hknight_walk3,  walk3,  hknight_walk4;  ai_walk(5);)
ANIM(hknight_walk4,  walk4,  hknight_walk5;  ai_walk(4);)
ANIM(hknight_walk5,  walk5,  hknight_walk6;  ai_walk(4);)
ANIM(hknight_walk6,  walk6,  hknight_walk7;  ai_walk(2);)
ANIM(hknight_walk7,  walk7,  hknight_walk8;  ai_walk(2);)
ANIM(hknight_walk8,  walk8,  hknight_walk9;  ai_walk(3);)
ANIM(hknight_walk9,  walk9,  hknight_walk10; ai_walk(3);)
ANIM(hknight_walk10, walk10, hknight_walk11; ai_walk(4);)
ANIM(hknight_walk11, walk11, hknight_walk12; ai_walk(3);)
ANIM(hknight_walk12, walk12, hknight_walk13; ai_walk(4);)
ANIM(hknight_walk13, walk13, hknight_walk14; ai_walk(6);)
ANIM(hknight_walk14, walk14, hknight_walk15; ai_walk(2);)
ANIM(hknight_walk15, walk15, hknight_walk16; ai_walk(2);)
ANIM(hknight_walk16, walk16, hknight_walk17; ai_walk(4);)
ANIM(hknight_walk17, walk17, hknight_walk18; ai_walk(3);)
ANIM(hknight_walk18, walk18, hknight_walk19; ai_walk(3);)
ANIM(hknight_walk19, walk19, hknight_walk20; ai_walk(3);)
ANIM(hknight_walk20, walk20, hknight_walk1;  ai_walk(2);)

//===========================================================================

void CheckForCharge ()
{
	vec3_t delta;

	// check for mad charge
	if ( !enemy_vis )
		return;

	if ( g_globalvars.time < self->attack_finished )
		return;

	if ( fabsf( self->s.v.origin[2] - PROG_TO_EDICT( self->s.v.enemy )->s.v.origin[2] ) > 20 )
		return;		// too much height change

	VectorSubtract( PROG_TO_EDICT( self->s.v.enemy )->s.v.origin, self->s.v.origin, delta );	

	if ( vlen( delta ) < 80 )
		return;		// use regular attack

	// charge
	SUB_AttackFinished( 2 );
	hknight_char_a1();
}

void _hknight_run1( void )
{
	hk_idle_sound();

	ai_run( 20 );

	CheckForCharge ();
}
ANIM(hknight_run1, run1, hknight_run2; _hknight_run1();)
ANIM(hknight_run2, run2, hknight_run3; ai_run(25);)
ANIM(hknight_run3, run3, hknight_run4; ai_run(18);)
ANIM(hknight_run4, run4, hknight_run5; ai_run(16);)
ANIM(hknight_run5, run5, hknight_run6; ai_run(14);)
ANIM(hknight_run6, run6, hknight_run7; ai_run(25);)
ANIM(hknight_run7, run7, hknight_run8; ai_run(21);)
ANIM(hknight_run8, run8, hknight_run1; ai_run(13);)

//============================================================================

void _hknight_pain1( void )
{
	sound( self, CHAN_VOICE, "hknight/pain1.wav", 1, ATTN_NORM );
}
ANIM(hknight_pain1, pain1, hknight_pain2; _hknight_pain1();)
ANIM(hknight_pain2, pain2, hknight_pain3)
ANIM(hknight_pain3, pain3, hknight_pain4)
ANIM(hknight_pain4, pain4, hknight_pain5)
ANIM(hknight_pain5, pain5, hknight_run1)

void hknight_pain( struct gedict_s *attacker, float damage )
{
	if ( self->pain_finished > g_globalvars.time )
		return;

	if ( g_globalvars.time - self->pain_finished > 5 )
	{
		// always go into pain frame if it has been a while
		hknight_pain1();
		self->pain_finished = g_globalvars.time + 1;
		return;
	}

	if ( g_random() * 30 > damage )
		return;		// didn't flinch

	hknight_pain1 ();
	self->pain_finished = g_globalvars.time + 1;
}

//============================================================================

ANIM(hknight_die1,  death1,	 hknight_die2; ai_forward(10);)
ANIM(hknight_die2,  death2,	 hknight_die3; ai_forward(8);)
ANIM(hknight_die3,  death3,	 hknight_die4; self->s.v.solid = SOLID_NOT; ai_forward(7);)
ANIM(hknight_die4,  death4,	 hknight_die5;)
ANIM(hknight_die5,  death5,	 hknight_die6;)
ANIM(hknight_die6,  death6,	 hknight_die7;)
ANIM(hknight_die7,  death7,	 hknight_die8;)
ANIM(hknight_die8,  death8,	 hknight_die9;  ai_forward(10);)
ANIM(hknight_die9,  death9,	 hknight_die10; ai_forward(11);)
ANIM(hknight_die10, death10, hknight_die11;)
ANIM(hknight_die11, death11, hknight_die12;)
ANIM(hknight_die12, death12, hknight_die12;)

ANIM(hknight_dieb1, deathb1, hknight_dieb2;)
ANIM(hknight_dieb2, deathb2, hknight_dieb3;)
ANIM(hknight_dieb3, deathb3, hknight_dieb4; self->s.v.solid = SOLID_NOT;)
ANIM(hknight_dieb4, deathb4, hknight_dieb5;)
ANIM(hknight_dieb5, deathb5, hknight_dieb6;)
ANIM(hknight_dieb6, deathb6, hknight_dieb7;)
ANIM(hknight_dieb7, deathb7, hknight_dieb8;)
ANIM(hknight_dieb8, deathb8, hknight_dieb9;)
ANIM(hknight_dieb9, deathb9, hknight_dieb9;)

void hknight_die ()
{
	// check for gib
	if ( self->s.v.health < -40 )
	{
		sound( self, CHAN_VOICE, "player/udeath.wav", 1, ATTN_NORM );
		ThrowHead( "progs/h_hellkn.mdl", self->s.v.health );
		ThrowGib( "progs/gib1.mdl", self->s.v.health );
		ThrowGib( "progs/gib2.mdl", self->s.v.health );
		ThrowGib( "progs/gib3.mdl", self->s.v.health );

		self->s.v.nextthink = -1;

		return;
	}

	// regular death
	sound( self, CHAN_VOICE, "hknight/death1.wav", 1, ATTN_NORM );

	if ( g_random() > 0.5 )
		hknight_die1();
	else
		hknight_dieb1();
}


//============================================================================
// magic

void hknight_shot( float offset )
{
	vec3_t	offang, delta;
	vec3_t	org, vec;

	VectorSubtract( PROG_TO_EDICT( self->s.v.enemy )->s.v.origin, self->s.v.origin, delta );	
	vectoangles( delta, offang );
	offang[1] += offset * 6;

	trap_makevectors( offang );

//	org = self.origin + self.mins + self.size * 0.5 + v_forward * 20;
	VectorAdd(self->s.v.origin, self->s.v.mins, org);
	VectorMA (org, 0.5, self->s.v.size, org);
	VectorMA (org, 20, g_globalvars.v_forward, org);

	// set missile speed
	normalize(g_globalvars.v_forward, vec);
	vec[2] = -vec[2] + ( g_random() - 0.5 ) * 0.1;

	launch_spike( org, vec );
	newmis->classname = "knightspike";
	setmodel( newmis, "progs/k_spike.mdl" );
	setsize( newmis, PASSVEC3( VEC_ORIGIN ), PASSVEC3( VEC_ORIGIN ) );
	VectorScale( vec, 300, newmis->s.v.velocity );
	sound( self, CHAN_WEAPON, "hknight/attack1.wav", 1, ATTN_NORM );
}

ANIM(hknight_magica1,  magica1,  hknight_magica2;  ai_face();)
ANIM(hknight_magica2,  magica2,  hknight_magica3;  ai_face();)
ANIM(hknight_magica3,  magica3,  hknight_magica4;  ai_face();)
ANIM(hknight_magica4,  magica4,  hknight_magica5;  ai_face();)
ANIM(hknight_magica5,  magica5,  hknight_magica6;  ai_face();)
ANIM(hknight_magica6,  magica6,  hknight_magica7;  ai_face();)
ANIM(hknight_magica7,  magica7,  hknight_magica8;  hknight_shot(-2);)
ANIM(hknight_magica8,  magica8,  hknight_magica9;  hknight_shot(-1);)
ANIM(hknight_magica9,  magica9,  hknight_magica10; hknight_shot(0);)
ANIM(hknight_magica10, magica10, hknight_magica11; hknight_shot(1);)
ANIM(hknight_magica11, magica11, hknight_magica12; hknight_shot(2);)
ANIM(hknight_magica12, magica12, hknight_magica13; hknight_shot(3);)
ANIM(hknight_magica13, magica13, hknight_magica14; ai_face();)
ANIM(hknight_magica14, magica14, hknight_run1;     ai_face();)

ANIM(hknight_magicb1,  magicb1,  hknight_magicb2;  ai_face();)
ANIM(hknight_magicb2,  magicb2,  hknight_magicb3;  ai_face();)
ANIM(hknight_magicb3,  magicb3,  hknight_magicb4;  ai_face();)
ANIM(hknight_magicb4,  magicb4,  hknight_magicb5;  ai_face();)
ANIM(hknight_magicb5,  magicb5,  hknight_magicb6;  ai_face();)
ANIM(hknight_magicb6,  magicb6,  hknight_magicb7;  ai_face();)
ANIM(hknight_magicb7,  magicb7,  hknight_magicb8;  hknight_shot(-2);)
ANIM(hknight_magicb8,  magicb8,  hknight_magicb9;  hknight_shot(-1);)
ANIM(hknight_magicb9,  magicb9,  hknight_magicb10; hknight_shot(0);)
ANIM(hknight_magicb10, magicb10, hknight_magicb11; hknight_shot(1);)
ANIM(hknight_magicb11, magicb11, hknight_magicb12; hknight_shot(2);)
ANIM(hknight_magicb12, magicb12, hknight_magicb13; hknight_shot(3);)
ANIM(hknight_magicb13, magicb13, hknight_run1;     ai_face();)

ANIM(hknight_magicc1,  magicc1,  hknight_magicc2;  ai_face();)
ANIM(hknight_magicc2,  magicc2,  hknight_magicc3;  ai_face();)
ANIM(hknight_magicc3,  magicc3,  hknight_magicc4;  ai_face();)
ANIM(hknight_magicc4,  magicc4,  hknight_magicc5;  ai_face();)
ANIM(hknight_magicc5,  magicc5,  hknight_magicc6;  ai_face();)
ANIM(hknight_magicc6,  magicc6,  hknight_magicc7;  hknight_shot(-2);)
ANIM(hknight_magicc7,  magicc7,  hknight_magicc8;  hknight_shot(-1);)
ANIM(hknight_magicc8,  magicc8,  hknight_magicc9;  hknight_shot(0);)
ANIM(hknight_magicc9,  magicc9,  hknight_magicc10; hknight_shot(1);)
ANIM(hknight_magicc10, magicc10, hknight_magicc11; hknight_shot(2);)
ANIM(hknight_magicc11, magicc11, hknight_run1;     hknight_shot(3);)

//===========================================================================
// melee

ANIM(hknight_char_a1,  char_a1,  hknight_char_a2;  ai_charge(20);)
ANIM(hknight_char_a2,  char_a2,  hknight_char_a3;  ai_charge(25);)
ANIM(hknight_char_a3,  char_a3,  hknight_char_a4;  ai_charge(18);)
ANIM(hknight_char_a4,  char_a4,  hknight_char_a5;  ai_charge(16);)
ANIM(hknight_char_a5,  char_a5,  hknight_char_a6;  ai_charge(14);)
ANIM(hknight_char_a6,  char_a6,  hknight_char_a7;  ai_charge(20); ai_melee();)
ANIM(hknight_char_a7,  char_a7,  hknight_char_a8;  ai_charge(21); ai_melee();)
ANIM(hknight_char_a8,  char_a8,  hknight_char_a9;  ai_charge(13); ai_melee();)
ANIM(hknight_char_a9,  char_a9,  hknight_char_a10; ai_charge(20); ai_melee();)
ANIM(hknight_char_a10, char_a10, hknight_char_a11; ai_charge(20); ai_melee();)
ANIM(hknight_char_a11, char_a11, hknight_char_a12; ai_charge(18); ai_melee();)
ANIM(hknight_char_a12, char_a12, hknight_char_a13; ai_charge(16);)
ANIM(hknight_char_a13, char_a13, hknight_char_a14; ai_charge(14);)
ANIM(hknight_char_a14, char_a14, hknight_char_a15; ai_charge(25);)
ANIM(hknight_char_a15, char_a15, hknight_char_a16; ai_charge(21);)
ANIM(hknight_char_a16, char_a16, hknight_run1;     ai_charge(13);)

void CheckContinueCharge()
{
	if ( g_globalvars.time > self->attack_finished )
	{
		SUB_AttackFinished( 3 );
		hknight_run1();
		return;		// done charging
	}

	if ( g_random() > 0.5 )
		sound( self, CHAN_WEAPON, "knight/sword2.wav", 1, ATTN_NORM );
	else
		sound( self, CHAN_WEAPON, "knight/sword1.wav", 1, ATTN_NORM );
}

ANIM(hknight_char_b1, char_b1, hknight_char_b2; CheckContinueCharge(); ai_charge(23); ai_melee();)
ANIM(hknight_char_b2, char_b2, hknight_char_b3; ai_charge(17); ai_melee();)
ANIM(hknight_char_b3, char_b3, hknight_char_b4; ai_charge(12); ai_melee();)
ANIM(hknight_char_b4, char_b4, hknight_char_b5; ai_charge(22); ai_melee();)
ANIM(hknight_char_b5, char_b5, hknight_char_b6; ai_charge(18); ai_melee();)
ANIM(hknight_char_b6, char_b6, hknight_char_b1; ai_charge(8);  ai_melee();)


ANIM(hknight_slice1,  slice1,  hknight_slice2;  ai_charge(9);)
ANIM(hknight_slice2,  slice2,  hknight_slice3;  ai_charge(6);)
ANIM(hknight_slice3,  slice3,  hknight_slice4;  ai_charge(13);)
ANIM(hknight_slice4,  slice4,  hknight_slice5;  ai_charge(4);)
ANIM(hknight_slice5,  slice5,  hknight_slice6;  ai_charge(7);  ai_melee();)
ANIM(hknight_slice6,  slice6,  hknight_slice7;  ai_charge(15); ai_melee();)
ANIM(hknight_slice7,  slice7,  hknight_slice8;  ai_charge(8);  ai_melee();)
ANIM(hknight_slice8,  slice8,  hknight_slice9;  ai_charge(2);  ai_melee();)
ANIM(hknight_slice9,  slice9,  hknight_slice10; ai_melee();)
ANIM(hknight_slice10, slice10, hknight_run1;    ai_charge(3);)


ANIM(hknight_smash1,  smash1,  hknight_smash2;  ai_charge(1);)
ANIM(hknight_smash2,  smash2,  hknight_smash3;  ai_charge(13);)
ANIM(hknight_smash3,  smash3,  hknight_smash4;  ai_charge(9);)
ANIM(hknight_smash4,  smash4,  hknight_smash5;  ai_charge(11);)
ANIM(hknight_smash5,  smash5,  hknight_smash6;  ai_charge(10); ai_melee();)
ANIM(hknight_smash6,  smash6,  hknight_smash7;  ai_charge(7);  ai_melee();)
ANIM(hknight_smash7,  smash7,  hknight_smash8;  ai_charge(12); ai_melee();)
ANIM(hknight_smash8,  smash8,  hknight_smash9;  ai_charge(2);  ai_melee();)
ANIM(hknight_smash9,  smash9,  hknight_smash10; ai_charge(3);  ai_melee();)
ANIM(hknight_smash10, smash10, hknight_smash11; ai_charge(0);)
ANIM(hknight_smash11, smash11, hknight_run1;    ai_charge(0);)


ANIM(hknight_watk1,  w_attack1,  hknight_watk2;  ai_charge(2);)
ANIM(hknight_watk2,  w_attack2,  hknight_watk3;  ai_charge(0);)
ANIM(hknight_watk3,  w_attack3,  hknight_watk4;  ai_charge(0);)
ANIM(hknight_watk4,  w_attack4,  hknight_watk5;  ai_melee();)
ANIM(hknight_watk5,  w_attack5,  hknight_watk6;  ai_melee();)
ANIM(hknight_watk6,  w_attack6,  hknight_watk7;  ai_melee();)
ANIM(hknight_watk7,  w_attack7,  hknight_watk8;  ai_charge(1);)
ANIM(hknight_watk8,  w_attack8,  hknight_watk9;  ai_charge(4);)
ANIM(hknight_watk9,  w_attack9,  hknight_watk10; ai_charge(5);)
ANIM(hknight_watk10, w_attack10, hknight_watk11; ai_charge(3); ai_melee();)
ANIM(hknight_watk11, w_attack11, hknight_watk12; ai_charge(2); ai_melee();)
ANIM(hknight_watk12, w_attack12, hknight_watk13; ai_charge(2); ai_melee();)
ANIM(hknight_watk13, w_attack13, hknight_watk14; ai_charge(0);)
ANIM(hknight_watk14, w_attack14, hknight_watk15; ai_charge(0);)
ANIM(hknight_watk15, w_attack15, hknight_watk16; ai_charge(0);)
ANIM(hknight_watk16, w_attack16, hknight_watk17; ai_charge(1);)
ANIM(hknight_watk17, w_attack17, hknight_watk18; ai_charge(1); ai_melee();)
ANIM(hknight_watk18, w_attack18, hknight_watk19; ai_charge(3); ai_melee();)
ANIM(hknight_watk19, w_attack19, hknight_watk20; ai_charge(4); ai_melee();)
ANIM(hknight_watk20, w_attack20, hknight_watk21; ai_charge(6);)
ANIM(hknight_watk21, w_attack21, hknight_watk22; ai_charge(7);)
ANIM(hknight_watk22, w_attack22, hknight_run1;   ai_charge(3);)

//============================================================================

static int	hknight_type; // FIXME; may be made it per one knight, but not shared for all knights right it now?

void hknight_melee ()
{
	hknight_type++;
	hknight_type %= 3;

	sound (self, CHAN_WEAPON, "hknight/slash1.wav", 1, ATTN_NORM);

	switch ( hknight_type )
	{
		case 1:  hknight_slice1(); break;
		case 2:  hknight_smash1(); break;
		default: hknight_watk1 (); break;
	}
}

//==============================================================================

/*QUAKED monster_hell_knight (1 0 0) (-16 -16 -24) (16 16 40) Ambush
*/
void SP_monster_hell_knight()
{
	if ( deathmatch )
	{
		ent_remove( self );
		return;
	}

	safe_precache_model( "progs/hknight.mdl" );
	safe_precache_model( "progs/k_spike.mdl" );
	safe_precache_model( "progs/h_hellkn.mdl" );

	safe_precache_sound( "hknight/attack1.wav" );
	safe_precache_sound( "hknight/death1.wav" );
	safe_precache_sound( "hknight/pain1.wav" );
	safe_precache_sound( "hknight/sight1.wav" );
	safe_precache_sound( "hknight/hit.wav" );
	safe_precache_sound( "hknight/slash1.wav" );
	safe_precache_sound( "hknight/idle.wav" );
	safe_precache_sound( "hknight/grunt.wav" );

	safe_precache_sound( "knight/sword1.wav" );
	safe_precache_sound( "knight/sword2.wav" );

	setsize( self, -16, -16, -24, 16, 16, 40 );
	self->s.v.health = 250;

	self->th_stand   = hknight_stand1;
	self->th_walk    = hknight_walk1;
	self->th_run     = hknight_run1;
	self->th_melee   = hknight_melee;
	self->th_missile = hknight_magicc1;
	self->th_pain    = hknight_pain;
	self->th_die     = hknight_die;

	self->th_respawn = SP_monster_hell_knight;

	walkmonster_start( "progs/hknight.mdl" );
}
