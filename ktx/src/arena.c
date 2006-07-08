/*
 * Copyright (C) 1997 David 'crt' Wright
 *
 * $Id: arena.c,v 1.1 2006/07/08 01:45:07 qqshka Exp $
 */

// arena.c - rocket arena stuff

#include "g_local.h"

void k_respawn( gedict_t *p );

void setnowep( gedict_t *anent );
void setfullwep( gedict_t *anent );

void SetWinner( gedict_t *p );
void SetLoser( gedict_t *p );
void SetNone( gedict_t *p );

// { ra queue stuff

gedict_t	*ra_que[MAX_CLIENTS];

void ra_init_que()
{
	memset(ra_que, 0, sizeof(ra_que));
}

// return first element in ra queue, return NULL if queue empty
gedict_t *ra_que_first()
{
	return ra_que[0];
}

// add element to end of ra queue
void ra_in_que( gedict_t *p )
{
	int i;

	if ( !p )
		G_Error ("ra_in_que: null");

	for ( i = 0; i < MAX_CLIENTS; i++ )
		if ( !ra_que[i] ) { // ok, we found empty space, add
			ra_que[i] = p;
			p->ra_pt = raQue; // mark player - he in queue now

			if ( CountPlayers() > 2 ) // if only two players, no reason of such messages
				G_sprint(p, PRINT_HIGH, "You are %s in line\n", (!i ? "next": "last"));

			return;
		}

	G_Error ("ra_in_que: full");
}

// remove element from ra queue 
void ra_out_que( gedict_t *p )
{
	int i;

	if ( !p )
		G_Error ("ra_out_que: null");

	for ( i = 0; i < MAX_CLIENTS; i++ )
		if ( ra_que[i] == p ) { // ok, we found element
			SetNone( p ); // mark player - he NOT in queue now
			for ( ; i < MAX_CLIENTS && ra_que[i]; i++ ) // move along other elements
				ra_que[i] = ( (i + 1 < MAX_CLIENTS) ? ra_que[i + 1] : NULL );

			return;
		}

	G_Error ("ra_out_que: not found");
}

// check if element is in ra queue
qboolean ra_isin_que( gedict_t *p )
{
	int i;

	if ( !p )
		G_Error ("ra_isin_que: null");

	for ( i = 0; i < MAX_CLIENTS && ra_que[i]; i++ )
		if ( ra_que[i] == p ) // ok, we found
			return true;

	return false;
}

// return element postion in ra queue, -1 if not in que
int ra_pos_que( gedict_t *p )
{
	int i;

	if ( !p )
		G_Error ("ra_pos_que: null");

	for ( i = 0; i < MAX_CLIENTS && ra_que[i]; i++ )
		if ( ra_que[i] == p ) // ok, we found
			return i;

	return -1;
}

// }

// ra is just modificator of duel
qboolean isRA( )
{
	return ( isDuel() && cvar("k_rocketarena") );
}

qboolean isWinner( gedict_t *p )
{
	return ( p->ra_pt == raWinner );
}

qboolean isLoser( gedict_t *p )
{
	return ( p->ra_pt == raLoser );
}

gedict_t *getWinner()
{
	gedict_t *p;

	for( p = g_edicts + 1; p <= g_edicts + MAX_CLIENTS; p++ )
		if ( p->k_player && isWinner( p ) )
			return p;

	return NULL;
}

gedict_t *getLoser()
{
	gedict_t *p;

	for( p = g_edicts + 1; p <= g_edicts + MAX_CLIENTS; p++ )
		if ( p->k_player && isLoser( p ) )
			return p;

	return NULL;
}

void SetWinner( gedict_t *p )
{
	p->ra_pt = raWinner;
}

void SetLoser( gedict_t *p )
{
	p->ra_pt = raLoser;
}

void SetNone( gedict_t *p )
{
	p->ra_pt = raNone;
}

void ra_ClientDisconnect()
{
	gedict_t *p = NULL;

	if ( !isRA() )
		return;

	if ( isWinner( self ) )
	{
		G_bprint (PRINT_HIGH, "The winner has left\n");
		if ( (p = getLoser()) )
			p->s.v.takedamage = DAMAGE_NO; // no damage to loser since winner is left game

		ra_match_fight = 0;
	}
	else if ( isLoser( self ) )
	{
		G_bprint (PRINT_HIGH, "The challenger has left\n");
		if ( (p = getWinner()) )
			p->s.v.takedamage = DAMAGE_NO; // no damage to winner since loser is left game

		ra_match_fight = 0;
	}
	else if ( self == ra_que_first() )
		G_bprint (PRINT_HIGH, "The line leader has left\n");

	if ( ra_isin_que( self ) ) // in the queue
		ra_out_que( self );	// remove from queue

	SetNone( self );
}

void ra_ClientObituary( gedict_t *targ, gedict_t *attacker )
{
	gedict_t *loser, *winner;

	if ( !isRA() )
		return;

	if ( !targ->k_player )
		return; // so below targ is player

	ra_match_fight = 0;

	if ( (loser = getLoser()) ) // stop them from attacking during countdown
	{
		loser->s.v.takedamage = DAMAGE_NO;
		PlayerStopFire( loser );
	}

	if ( (winner = getWinner()) ) // stop them from attacking during countdown
	{
		winner->s.v.takedamage = DAMAGE_NO;
		PlayerStopFire( winner );
	}

	if ( !loser || !winner ) {
		// just wanna know is that possible somehow
		if ( !loser )
			G_bprint (PRINT_HIGH, "BUG: ra_ClientObituary without loser\n");
		if ( !winner )
			G_bprint (PRINT_HIGH, "BUG: ra_ClientObituary without winner\n");

		return; // require both
	}

	if ( targ == winner )
	{
		winner->ps.loses += 1;
		loser->ps.wins   += 1;
		
		G_bprint (PRINT_HIGH, "The winner %s has been defeated\n", getname(winner));

		if ( targ == attacker )
		{
			G_bprint (PRINT_HIGH, "by %s!\n", g_himself(winner));
			winner->s.v.frags -= 1;
		}

		ra_in_que( winner ); // move to que winner
	}
	else if ( targ == loser )
	{
		loser->ps.loses += 1;
		winner->ps.wins += 1;

		G_bprint (PRINT_HIGH, "The challenger %s has failed\n", getname(loser));

		if ( targ == attacker )
		{
			G_bprint (PRINT_HIGH, "because %s became bored with life!\n", g_he(loser));
			loser->s.v.frags -= 1;
		}

		ra_in_que( loser ); // move to que loser
	}
	else {
		// just wanna know is that possible somehow
		G_bprint (PRINT_HIGH, "BUG: ra_ClientObituary unknown targ\n");
	}

	if ( attacker->k_player )
	{
		if ( attacker != targ )
		{
			int ah = attacker->s.v.health;
			int aa = attacker->s.v.armorvalue;

			if ( ah == 100 && aa == 200 )
			{
				G_bprint (PRINT_HIGH, "%s\n", redtext("FLAWLESS Victory!"));
				
				playallsound("play ra/flawless.wav\n");
			}
			else
			{
				G_bprint (PRINT_HIGH, "%s had %d health and %d armor left\n", getname(attacker), ah, aa);

				if ( ah >= 75 && aa >= 100 )
					playallsound("play ra/excelent.wav\n");				
			}

			attacker->s.v.frags += 1;
		}

		logfrag (attacker, targ);
	}
}   

void ra_PutClientInServer()
{
	if ( !isRA() )
		return;

	setnowep( self ); // basic shit, even for qued player

	if ( isWinner( self ) || isLoser( self ) ) {
		VectorScale( g_globalvars.v_forward, 300, self->s.v.velocity );
		setfullwep( self ); // shit for winner or loser
	}
}

// actually may stuff command to all clients consoles, not just play some sound
void playallsound( char *playstr )
{
	gedict_t *p;
	int from;

	for( from = 0, p = world; (p = find_plrspc (p, &from)); )
		stuffcmd(p, playstr);
}

void setnowep( gedict_t *anent )
{
	gedict_t *swap;

	anent->s.v.ammo_shells	= 10; // wtf 10 ?
	anent->s.v.ammo_nails	= 0;
	anent->s.v.ammo_shells	= 0;
	anent->s.v.ammo_rockets	= 0;
	anent->s.v.ammo_cells	= 0;
	anent->s.v.takedamage	= DAMAGE_NO;
	anent->s.v.items		= IT_AXE;

	anent->s.v.armorvalue	= 0;
	anent->s.v.armortype	= 0;
	anent->s.v.health		= 100;
	anent->s.v.weapon		= IT_AXE;
	anent->idletime			= 0;
	anent->lasttime			= 0;
	anent->laststattime		= 0;

	swap = self;
	self = anent;

	// drop down to best weapon actually hold
	if ( !( (int)self->s.v.weapon & (int)self->s.v.items ) )
		self->s.v.weapon = W_BestWeapon();

	W_SetCurrentAmmo ();
	self = swap;
}

void setfullwep( gedict_t *anent )
{
	gedict_t *swap;

// ammo
	anent->s.v.ammo_nails   = min(200, anent->s.v.ammo_nails   + 80);
	anent->s.v.ammo_shells  = min(100, anent->s.v.ammo_shells  + 30);
	anent->s.v.ammo_rockets = min(100, anent->s.v.ammo_rockets + 30);
	anent->s.v.ammo_cells   = min(100, anent->s.v.ammo_cells   + 30);
// weapons
	anent->s.v.items = IT_AXE | IT_SHOTGUN | IT_SUPER_SHOTGUN | IT_NAILGUN | IT_SUPER_NAILGUN |
					   IT_GRENADE_LAUNCHER | IT_ROCKET_LAUNCHER | IT_LIGHTNING;
// armor
	anent->s.v.items = (int)anent->s.v.items | IT_ARMOR3;
	anent->s.v.armorvalue = 200;
	anent->s.v.armortype = 0.8;
// health
	anent->s.v.health = 100;
// powerups
	anent->super_damage_finished = 0;
	anent->super_time			 = 0;
	anent->radsuit_finished		 = 0;
	anent->rad_time				 = 0;
	anent->invisible_finished	 = 0;
	anent->invisible_time		 = 0;
	anent->invincible_finished	 = 0;
	anent->invincible_time		 = 0;
	anent->k_666	   = 0;	// team
	anent->s.v.effects = 0;

	swap = self;
	self = anent;

	// drop down to best weapon actually hold
	if ( !( (int)self->s.v.weapon & (int)self->s.v.items ) )
		self->s.v.weapon = W_BestWeapon();

	W_SetCurrentAmmo ();
	self = swap;
}

qboolean readytostart()
{
	if ( !isRA() || ( time_to_start && g_globalvars.time > time_to_start && getWinner() && getLoser() ) )
		return true;
	else
		return false;
}

char *TeamSetStatRes( gedict_t *who )
{
	if (who->statstate > 7) // 768 
		return "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
	else if (who->statstate == 7) // 600 
		return "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
	else if (who->statstate == 6) // 480 
		return "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
	else if (who->statstate == 5) // 400
		return "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
	else if (who->statstate == 4) // 384 
		return "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
	else if (who->statstate == 3) // 350 
		return "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
	else if (who->statstate == 2) // 300 
		return "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
	else if (who->statstate == 1) // 240 
		return "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
	// 200 
	return "\n\n\n\n\n\n\n\n\n\n\n\n";
}

void PrintStats( gedict_t *who )
{
	gedict_t *winner = getWinner(), *loser = getLoser();

	if ( !winner || !loser )
		return;

	who->laststattime = g_globalvars.time + PLAYERSTATTIME;

	G_centerprint(who, "%s%s          %s              ", TeamSetStatRes( who ), getname(winner), getname(loser));

	if ( winner->s.v.health > 0 )
		self->s.v.armorvalue = winner->s.v.health;
	if ( loser->s.v.health > 0 )
		self->s.v.health = loser->s.v.health;

	who->s.v.currentammo = ra_pos_que( who ) + 1;
}

void ra_Frame ()
{
	static int last_r;

	int	r;
	gedict_t *winner, *loser;

	if ( !isRA() || match_over )
		return;

 	winner = getWinner();
	loser  = getLoser();

//	G_bprint (PRINT_HIGH, "winner %s\n", winner ? getname(winner) : "none");
//	G_bprint (PRINT_HIGH, "loser  %s\n", loser  ? getname(loser)  : "none");

	if ( !winner || !loser ) {
		ra_match_fight = 0;

		if ( !winner && loser ) { // promote loser since winner not present for some reason
			winner = loser;
			loser  = NULL;
			G_bprint (PRINT_HIGH, "The new winner is %s\n", getname(winner));
			SetWinner( winner );
			setfullwep( winner ); // because we not resapawn him, set full manually
		}
    
		if ( !winner && (winner = ra_que_first()) ) { // still lack of winner
			ra_out_que( winner );
			G_bprint (PRINT_HIGH, "The new winner is %s\n", getname(winner));
			SetWinner( winner );
			k_respawn( winner ); // respawn player
		}
    
		if ( !loser && (loser = ra_que_first()) ) { // lack of loser
			ra_out_que( loser );
			G_bprint (PRINT_HIGH, "The new challenger is %s\n", getname(loser));
			SetLoser( loser );
			k_respawn( loser ); // respawn player
		}

		if ( !winner || !loser )
			return;
	}

	if ( ra_match_fight == 2 || match_in_progress != 2 )
		return;

	if ( !ra_match_fight ) { // ok start ra timer
		ra_match_fight = 1; // ra countdown
		last_r = 99999;
		time_to_start  = g_globalvars.time + 10;
	}

	r = Q_rint(time_to_start - g_globalvars.time);

	if ( r <= 0 )
	{
		char *fight = redtext("FIGHT!");
		gedict_t *first = ra_que_first();

		stuffcmd(winner, "play ra/fight.wav\n");
		stuffcmd(loser,  "play ra/fight.wav\n");
		G_bprint (PRINT_HIGH, "%s vs. %s\n", getname(winner), getname(loser));

		if ( first )
			G_bprint (PRINT_HIGH, "%s is next in line\n", getname(first));

		setfullwep( winner );
		setfullwep( loser );

		winner->s.v.takedamage = DAMAGE_AIM;
		loser->s.v.takedamage  = DAMAGE_AIM;

		G_centerprint (winner, "%s", fight);
		G_centerprint (loser,  "%s", fight);

		ra_match_fight = 2;
	}
	else if ( r != last_r )
	{
		last_r = r;

		if ( r <= 3 )
		{
			stuffcmd(winner, "play ra/%d.wav\n", r);
			stuffcmd(loser,  "play ra/%d.wav\n", r);
		}

		G_centerprint (winner, "%s\n\n%d", getname(loser),  r);
		G_centerprint (loser,  "%s\n\n%d", getname(winner), r);
	}
}

void RocketArenaPre()
{
	if ( !isRA() )
		return;

	if ( self->idletime != 0 )
	{
		int r = Q_rint(self->idletime - g_globalvars.time);

		if ( r != self->lasttime )
		{
			self->lasttime = r;

			if ( r == 60 )
			{
				G_sprint (self, PRINT_HIGH,"You have 1 minute left\n"
										   "%s to get back in line\n", redtext("tline"));
				stuffcmd(self, "play player/axhit1.wav\n");
			}
			else if ( r == 30 )
			{
				G_sprint (self, PRINT_HIGH,"You have 30 seconds left\n"
										   "%s to get back in line\n", redtext("tline"));
				stuffcmd(self, "play player/axhit1.wav\n");
			}
			else if ( r > 0 && r <= 10 )
			{
				G_sprint (self, PRINT_HIGH,"You have %d second%s left to get in line\n", r, count_s( r ));
				stuffcmd(self, "play player/axhit1.wav\n");
			}
			else if ( r <= 0 )
			{
				self->idletime = 0;
				G_sprint (self, PRINT_HIGH,"Sorry, your wait time has expired!\n");
				G_bprint (PRINT_HIGH, "%s stood around too long\n", getname(self));

				stuffcmd(self, "play player/death1.wav\n");

				stuffcmd(self, "disconnect\n"); // FIXME: stupid way

				return;
			}

		}
	}

	if ( !isWinner( self ) && !isLoser( self ) && self->laststattime < g_globalvars.time  )
		PrintStats( self );
}

// { ra commands

void ra_PrintStat( gedict_t *e )
{
	float ratio = ( ( e->ps.loses + e->ps.wins ) ? ( e->ps.wins * 100.0f ) / ( e->ps.loses + e->ps.wins ) : 0 );

	G_sprint(self, PRINT_HIGH, "%10.10s W:%3d L:%3d S:%5.1f%%\n", getname( e ), e->ps.wins, e->ps.loses, ratio);
}
		
void ra_PrintStats()
{
	int from;
	gedict_t *p;

	if ( !isRA() )
		return;

	if ( (p = getWinner()) )
		ra_PrintStat( p );

	if ( (p = getLoser()) )
		ra_PrintStat( p );

	for( from = 0, p = world; (p = find_plrghst ( p, &from )); ) {
		if ( isWinner( p ) || isLoser( p ) )
			continue;

		ra_PrintStat( p );
	}
	
}

void ra_PrintPos()
{
	int pos;

	if ( !isRA() || isWinner( self ) || isLoser( self ) )
		return;

	if ( (pos = ra_pos_que( self )) < 0 ) {
		G_sprint (self, PRINT_HIGH,"You are out of line\n"
								   "%s to return\n", redtext("tline"));
		return;
	}

	if ( !pos )	{
		G_sprint (self, PRINT_HIGH,"You are next\n");
		return;
	}

	if ( pos = 1 ) {
		G_sprint (self, PRINT_HIGH,"There is 1 person ahead of you\n");
		return;
	}

	if ( pos > 1 ) {
		G_sprint (self, PRINT_HIGH,"There are %d people ahead of you\n", pos);
		return;
	}
}

void ra_tLine()
{
	if ( !isRA() || isWinner( self ) || isLoser( self ) )
		return;

	if ( ra_isin_que( self ) ) // take OUT of line
	{
		G_sprint (self, PRINT_HIGH,"You can have up to a 5 minute break\n"
								   "%s to get back in line\n", redtext("tline"));	

		self->idletime = g_globalvars.time + MAXIDLETIME;
		ra_out_que( self );
	}
	else // put INTO line
	{
		self->idletime = 0;
		ra_in_que( self );
	}
}

// }
