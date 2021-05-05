/*
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

// commands.c

#include "g_local.h"

int max_cmd_len = 0;

qbool is_rules_change_allowed( void );
void SendMessage(char *name);
float CountRPlayers();
float CountTeams();
void PlayerReady ();
void PlayerBreak ();
void ReqAdmin ();
void AdminForceStart ();
void AdminForceBreak ();
void AdminSwapAll ();
void TogglePreWar ();
void ToggleMapLock ();
void AdminKick ();
void m_kick ();
void YesKick ();
void DontKick ();
void VoteAdmin();
void VoteYes();
void VoteNo();
void VoteCaptain();
void VoteCoach();
void nospecs();
void teamoverlay();
void votecoop();
void RandomPickup();
void ShowDMM();
void ChangeDM(float dmm);
void ChangeLock();
void ChangeOvertime();
void ChangeOvertimeUp();
void ChangeTP();
void ToggleFallBunny ();
// { CTF
void FlagStatus();
void TossFlag();
void norunes();
void nohook();
void crhook();
void noga();
void mctf();
void CTFBasedSpawn();
// also: TossRune()
//       swapall()
// } CTF
void FragsDown();
void FragsUp();
void ListWhoNot();
void ModStatus();
void ModStatus2();
void ModStatusVote();
void PlayerStats();
void PlayerStatus();
void PlayerStatusN();
void PlayerStatusS();
void PrintScores();
void ReportMe();
void SendKillerMsg();
void SendNewcomerMsg();
void SendVictimMsg();
void ShowNick();
void ShowCmds();
void ShowMaps();
//void ShowMessages();
void ShowOpts();
void ShowQizmo();
void ShowRules();
void ShowVersion();
void ShowCharsetTableOctal();
void ShowCharsetTableHexa();
void killquad();
void bloodfest();
void antilag();
void ToggleDischarge();
void ToggleDropPack();
void ToggleDropQuad();
void ToggleDropRing();
void ToggleFairPacks();
void ToggleFreeze();
void ToggleMidair();
void SetMidairMinHeight();
void ToggleInstagib();
void ToggleLGC(void);
void ToggleCGKickback();
void TogglePowerups();
void TogglePuPickup();
void ToggleQEnemy();
void ToggleQLag();
void ToggleQPoint();
/* new FDP bits https://www.quakeworld.nu/wiki/FPD
void ToggleSkinForcing();
void ToggleColorForcing();
void TogglePitchSpeedLimit();
void ToggleYawSpeedLimit();
*/
void ToggleRespawns();
void ToggleBerzerk();
void ToggleSpecTalk();
void ToggleSpeed();
void VotePickup();
void UserMode(float umode);
void Wp_Reset ();
void Wp_Stats(float on);
void Sc_Stats(float on);
void t_jump (float j_type);
void klist ();
void hdptoggle ();
void handicap ();
void noweapon ();
void tracklist ();
void fpslist ();
void krnd ();
void agree_on_map ();

void favx_add ( float fav_num );
void xfav_go ( float fav_num );
void fav_add ();
void fav_del ();
void fav_all_del ();
void fav_next ();
void fav_show ();
void AutoTrack ( float autoTrackType );
void next_best ();
void next_pow ();
void Pos_Show ();
void Pos_Save ();
void Pos_Move ();
void Pos_Set ( float set_type );
void Sh_Speed ();
void lastscores ();

void motd_show ();

void TogglePractice();

// { yawn mode
void ToggleYawnMode();
void setTeleportCap();
// }

void infolock ();
void infospec ();
void moreinfo ();

void s_p_cmd();
void s_lr_cmd( float l );
void s_t_cmd();
void mmode();
void multi();
void cmdinfo();
void cmduinfo();
void cmd_wreg();

void ClientKill();

void sv_time();
void GrenadeMode();
void ToggleReady();

void fp_toggle ( float type );

void dlist ();
void dinfo ();

void sv_lock ();
void force_spec();
void teleteam ();
void upplayers ( float type );
void downplayers ( float type );
void iplist ();
void dmgfrags ();
void no_lg ();
void no_gl ();
void mv_cmd_playback ();
void mv_cmd_record ();
void mv_cmd_stop ();
void callalias ();
void fcheck ();
void mapcycle ();
void airstep();
void ToggleExclusive();
void ToggleNewCoopNm();
void ToggleVwep();
void TogglePause();
void ToggleArena();

void Spawn666Time();

void noitems();

// spec
void ShowCamHelp();

void TeamSay(float fsndname);
void TimeDown(float t);
void TimeUp(float t);
void TimeSet(float t);

void cmdslist_dl();
void mapslist_dl();

// { RACE
void r_cdel( );
void r_clear_route( void );
void r_Xset( float t );
void r_changestatus( float t );
void r_changefollowstatus( float t );

void r_timeout( );
void r_falsestart( );
void r_mode( );
void r_all_break( );

void r_route( );
void r_print( );

void race_display_line( );
void display_scores( );
void display_record_details( );
void race_chasecam_change( );
void race_chasecam_freelook_change( );
void race_download_record_demo( );
void race_pacemaker(void);
void race_simultaneous_toggle(void);
void race_match_toggle(void);
qbool race_match_mode(void);
void race_switch_usermode(const char* displayName, int players_per_team);
void race_scoring_system_toggle(void);
void race_hide_players_toggle(void);
// }

// { CHEATS
void giveme( );
static void dropitem( );
static void removeitem( );
static void dumpent( );
// }

// { Clan Arena
void ToggleCArena();
// }

// { Frogbots
void FrogbotsCommand(void);
// }

// { Private games
void private_game_vote(void);
void private_game_toggle(qbool enable);
// }

// Save the first 5 demo markers to print at the end.
demo_marker_t demo_markers[10];
int demo_markers_count = 10;
int demo_marker_index = 0;

void ClearDemoMarkers()
{
	demo_marker_index = 0;
}

void DemoMark()
{
	stuffcmd( self, "//demomark\n" );

	if ( match_in_progress <= 1 )
		return;

	// Do not add marker if there was already a marker just a few seconds ago.
	if ( demo_marker_index > 0 && (g_globalvars.time - demo_markers[demo_marker_index - 1].time) < 5 )
		return;

	if ( demo_marker_index < demo_markers_count )
	{
		int total = (int)(g_globalvars.time - match_start_time);

		demo_markers[demo_marker_index].time = g_globalvars.time;
		strlcpy(demo_markers[demo_marker_index].markername, getname(self), sizeof(demo_markers[demo_marker_index].markername));
		demo_marker_index++;

		G_sprint(self, 2, "Added demo marker: \220%d:%02d\221\n", (total / 60), (total % 60));
	}
	else
	{
		G_sprint(self, 2, "Demo markers full!\n");
	}
}

// CD - commands descriptions

const char CD_NODESC[] = "no desc";

#define CD_VOTEMAP    "alternative map vote system"
#define CD_COMMANDS   "show commands list"
#define CD_SCORES     "print match time/scores"
#define CD_STATS      "show player stats"
#define CD_EFFI       "show player efficiencies"
#define CD_OPTIONS    "match control commands"
#define CD_READY      "when you feel ready"
#define CD_BREAK      "unready / vote matchend"
#define CD_STATUS     "show server settings"
#define CD_STATUS2    "more server settings"
#define CD_WHO        "player teamlist"
#define CD_WHOSKIN    "player skinlist"
#define CD_WHONOT     "players not ready"
#define CD_LIST       "whonot to everyone"
#define CD_WHOVOTE    "info on received votes"
#define CD_SPAWN      "toggle spawn modes"
#define CD_POWERUPS   "quad, \230\230\230, ring & suit"
#define CD_PUPICKUP   "change powerups pickup policy"
#define CD_ANTILAG    "toggle antilag"
#define CD_DISCHARGE  "underwater discharges"
#define CD_DM         "show deathmatch mode"
#define CD_DMM1       "set deathmatch mode 1"
#define CD_DMM2       "set deathmatch mode 2"
#define CD_DMM3       "set deathmatch mode 3"
#define CD_DMM4       "set deathmatch mode 4"
#define CD_DMM5       "set deathmatch mode 5"
#define CD_TP         "change teamplay mode"
#define CD_TIMEDOWN1  "-1 mins match time"
#define CD_TIMEUP1    "+1 mins match time"
#define CD_TIMEDOWN   "-5 mins match time"
#define CD_TIMEUP     "+5 mins match time"
#define CD_FALLBUNNY  "toggle fallbunny"
#define CD_FRAGSDOWN  "-10 fraglimit"
#define CD_FRAGSUP    "+10 fraglimit"
#define CD_KILLQUAD   "kill the quad mode"
#define CD_BLOODFEST  "blood fest mode (coop/single only)"
#define CD_DROPQUAD   "drop quad when killed"
#define CD_DROPRING   "drop ring when killed"
#define CD_DROPPACK   "drop pack when killed"
#define CD_SILENCE    "toggle spectator talk"
#define CD_REPORT     "simple teamplay report"
#define CD_RULES      "show game rules"
#define CD_LOCKMODE   "change locking mode"
#define CD_MAPS       "list custom maps"
#define CD_ADMIN      "toggle admin-mode"
#define CD_FORCESTART "force match to start"
#define CD_FORCEBREAK "force match to end"
#define CD_PICKUP     "vote for pickup game"
#define CD_PREWAR     "playerfire before game"
#define CD_LOCKMAP    "(un)lock current map"
#define CD_SPEED      "toggle sv_maxspeed"
#define CD_FAIRPACKS  "best/last weapon dropped"
#define CD_ABOUT      "mod's info"
#define CD_CTOCT	  "Show octal charset table"
#define CD_CTHEX	  "Show hexadecimal charset table"
#define CD_SHOWNICK   "pointed player's info"
#define CD_TIME5      "set timelimit to 5 mins"
#define CD_TIME10     "set timelimit to 10 mins"
#define CD_TIME15     "set timelimit to 15 mins"
#define CD_TIME20     "set timelimit to 20 mins"
#define CD_TIME25     "set timelimit to 25 mins"
#define CD_TIME30     "set timelimit to 30 mins"
#define CD_KSOUND1    (CD_NODESC) // useless command now
#define CD_KSOUND2    (CD_NODESC) // useless command now
#define CD_KSOUND3    (CD_NODESC) // useless command now
#define CD_KSOUND4    (CD_NODESC) // useless command now
#define CD_KSOUND5    (CD_NODESC) // useless command now
#define CD_KSOUND6    (CD_NODESC) // useless command now
#define CD_QIZMO      "qizmo related commands"
#define CD_MESSAGES   "fun message commands"
#define CD_KILLER     "message to killer"
#define CD_VICTIM     "message to victim"
#define CD_NEWCOMER   "message to last player joined"
#define CD_QLAG       "lag settings"
#define CD_QENEMY     "enemy vicinity reporting"
#define CD_QPOINT     "point function"
/* new FDP bits https://www.quakeworld.nu/wiki/FPD
#define CD_SFORCING   "skin forcing"
#define CD_CFORCING   "color forcing"
#define CD_PITCHSP    "pitch speed limiting"
#define CD_YAWSP      "yaw speed limiting"
*/
#define CD_KICK       "toggle kick mode"
#define CD_MKICK      "multi kick"
#define CD_Y          "yes kick"
#define CD_N          "don't kick"
#define CD_OVERTIME   "toggle overtime mode"
#define CD_OVERTIMEUP "change overtime length"
#define CD_ELECT      "toggle admin election"
#define CD_YES        "give vote"
#define CD_NO         "withdraws vote"
#define CD_CAPTAIN    "toggle captain election"
#define CD_COACH      "toggle coach election"
#define CD_FREEZE     "(un)freeze the map"
#define CD_RPICKUP    "vote random team pickup"
#define CD_1ON1       "duel settings"
#define CD_1ON1HM     "HoonyMode settings"
#define CD_2ON2BLITZ  "Blitz 2v2"
#define CD_4ON4BLITZ  "Blitz 4v4"
#define CD_HMSTATS    "show stats per hoonymode point"
#define CD_2ON2       "2 on 2 settings"
#define CD_3ON3       "3 on 3 settings"
#define CD_4ON4       "4 on 4 settings"
#define CD_10ON10     "10 on 10 settings"
#define CD_FFA        "FFA settings"
#define CD_CTF        "CTF settings"
#define CD_PRACTICE   "toggle practice mode"
#define CD_WP_RESET   "clear weapon stats"
#define CD_PLS_WP_STATS "start print weapon stats"
#define CD_MNS_WP_STATS (CD_NODESC) // obvious
#define CD_TKFJUMP      "toggle allow kfjump"
#define CD_TKRJUMP      "toggle allow krjump"
#define CD_KLIST        "mod's list of users"
#define CD_HDPTOGGLE    "toggle allow handicap"
#define CD_HANDICAP     "toggle handicap level"
#define CD_NOWEAPON     "toggle allow any weapon"
#define CD_CAM          "camera help text"
#define CD_TRACKLIST    "trackers list"
#define CD_FPSLIST      "fps list"
#define CD_FAV1_ADD     "add player to slot  1"
#define CD_FAV2_ADD     "add player to slot  2"
#define CD_FAV3_ADD     "........etc.........."
#define CD_FAV4_ADD     (CD_NODESC) // skip
#define CD_FAV5_ADD     (CD_NODESC) // skip
#define CD_FAV6_ADD     (CD_NODESC) // skip 
#define CD_FAV7_ADD     (CD_NODESC) // skip 
#define CD_FAV8_ADD     (CD_NODESC) // skip 
#define CD_FAV9_ADD     (CD_NODESC) // skip 
#define CD_FAV10_ADD    (CD_NODESC) // skip
#define CD_FAV11_ADD    (CD_NODESC) // skip
#define CD_FAV12_ADD    (CD_NODESC) // skip
#define CD_FAV13_ADD    (CD_NODESC) // skip
#define CD_FAV14_ADD    (CD_NODESC) // skip
#define CD_FAV15_ADD    (CD_NODESC) // skip
#define CD_FAV16_ADD    (CD_NODESC) // skip
#define CD_FAV17_ADD    (CD_NODESC) // skip
#define CD_FAV18_ADD    (CD_NODESC) // skip
#define CD_FAV19_ADD    "add player to slot 19"
#define CD_FAV20_ADD    "add player to slot 20"
#define CD_1FAV_GO      "set pov to slot  1"
#define CD_2FAV_GO      "set pov to slot  2"
#define CD_3FAV_GO      ".......etc........"
#define CD_4FAV_GO      (CD_NODESC) // skip
#define CD_5FAV_GO      (CD_NODESC) // skip 
#define CD_6FAV_GO      (CD_NODESC) // skip 
#define CD_7FAV_GO      (CD_NODESC) // skip 
#define CD_8FAV_GO      (CD_NODESC) // skip 
#define CD_9FAV_GO      (CD_NODESC) // skip 
#define CD_10FAV_GO     (CD_NODESC) // skip
#define CD_11FAV_GO     (CD_NODESC) // skip
#define CD_12FAV_GO     (CD_NODESC) // skip
#define CD_13FAV_GO     (CD_NODESC) // skip
#define CD_14FAV_GO     (CD_NODESC) // skip
#define CD_15FAV_GO     (CD_NODESC) // skip
#define CD_16FAV_GO     (CD_NODESC) // skip
#define CD_17FAV_GO     (CD_NODESC) // skip
#define CD_18FAV_GO     (CD_NODESC) // skip
#define CD_19FAV_GO     "set pov to slot 19"
#define CD_20FAV_GO     "set pov to slot 20"
#define CD_FAV_ADD      "add player to fav list"
#define CD_FAV_DEL      "remove player from fav list"
#define CD_FAV_ALL_DEL  "clear fav list"
#define CD_FAV_NEXT     "set pov to next fav"
#define CD_FAV_SHOW     "show fav list"
#define CD_PLS_SCORES   "show match time and score"
#define CD_MNS_SCORES   (CD_NODESC) // obvious
#define CD_AUTOTRACK    "auto tracking"
#define CD_AUTOTRACKKTX "auto tracking, ktx version"
#define CD_AUTO_POW     "auto tracking powerups"
#define CD_NEXT_BEST    "set pov to next best player"
#define CD_NEXT_POW     "set pov to next powerup"
#define CD_LASTSCORES   "print last games scores"
#define CD_RND          "select random value"
#define CD_AGREE        "agree on last map vote"
#define CD_POS_SHOW     "info about saved position"
#define CD_POS_SAVE     "save current position"
#define CD_POS_MOVE     "move to saved position"
#define CD_POS_ORIGIN   "set position origin"
#define CD_POS_ANGLES   "set position angles"
#define CD_POS_VELOCITY "set position velocity"
#define CD_SH_SPEED     "toggle use show speed"
#define CD_TOSSRUNE     "drop rune (CTF)"
#define CD_TOSSFLAG     "drop flag (CTF)"
#define CD_FLAGSTATUS   "show flags status (CTF)"
#define CD_NOHOOK       "toggle hook (CTF)"
#define CD_CRHOOK       "toggle CRCTF 3.0 Hook settings (CTF)"
#define CD_NORUNES      "toggle runes (CTF)"
#define CD_NOGA         "toggle green armor on spawn (CTF)"
#define CD_MCTF         "disable hook+runes (CTF)"
#define CD_CTFBASEDSPAWN "spawn players on the base (CTF)"
#define CD_MOTD         "show motd"
#define CD_INFOLOCK     "toggle specinfo perms"
#define CD_INFOSPEC     "toggle spectator infos"
#define CD_MOREINFO     "receiving more info"
#define CD_S_P			"direct player say"
#define CD_S_L          "continue last s-p u done"
#define CD_S_R          "reply to last s-p u got"
#define CD_S_T          "say to group of players"
#define CD_S_M          "multi say" // anyone have better description?
#define CD_MMODE        "switch message mode"
#define CD_MULTI        "change/print multi set"
#define CD_KINFO        "set self params for mod"
#define CD_KUINFO       "examine someone params"
#define CD_WREG         "register reliable wpns"
#define CD_KILL         "invoke suicide"
#define CD_MIDAIR       "turn midair mode on/off"
#define CD_MIDAIR_MINHEIGHT "midair minimum frag height"
#define CD_INSTAGIB     "instagib settings"
#define CD_BERZERK      "berzerk settings"
#define CD_LGC          "lgc mode"
#define CD_CG_KB        "toggle coilgun kickback in instagib"
#define CD_TIME         "show server time"
#define CD_GREN_MODE    "grenades mode"
#define CD_TOGGLEREADY  "just toggle ready"
#define CD_FP           "change floodprot level for players"
#define CD_FP_SPEC      "change floodprot level for specs"
#define CD_DLIST        "show demo list"
#define CD_DINFO        "show demo info"
#define CD_LOCK         "temprorary lock server"
#define CD_SWAPALL      "swap teams for ctf"
// { RA
#define CD_RA_BREAK     "toggle RA line status"
#define CD_RA_POS       "RA line position"
#define CD_ARENA        "toggle rocket arena"
// }
// { Clan Arena
#define CD_CARENA       "toggle clan arena"
// }
#define CD_FORCE_SPEC   "force spec players"
// { server side bans
#define CD_BAN          "timed ban by uid/nick"
#define CD_BANIP        "timed ban by ip"
#define CD_BANREM       "remove ban / banlist"
// }
#define CD_TELETEAM     "team telefrag behaviour"
#define CD_UPPLAYERS    "increase maxclients"
#define CD_DOWNPLAYERS  "decrease maxclients"
#define CD_UPSPECS      "increase maxspectators"
#define CD_DOWNSPECS    "decrease maxspectators"
#define CD_IPLIST       "list clients ips"
#define CD_DMGFRAGS     "toggle damage frags"
#define CD_NO_LG        "alias for /noweapon lg"
#define CD_NO_GL		"alias for /noweapon gl"
// {
#define CD_TRX_REC      "trick tmp record"
#define CD_TRX_PLAY     "trick tmp playback"
#define CD_TRX_STOP     "stop playback/recording"
// }
#define CD_CALLALIAS    "call alias after few secs"
#define CD_CHECK        "better f_checks handle"
#define CD_NEXT_MAP     "vote for next map"
#define CD_MAPCYCLE     "list map cycle"
#define CD_YAWNMODE     "toggle yawnmode"
#define CD_FALLBUNNYCAP "set fallbunny cap (yawn)"
#define CD_TELEPORTCAP  "set teleport cap (yawn)"
#define CD_AIRSTEP      "toggle airstep"
#define CD_EXCLUSIVE    "toggle exclusive mode"
#define CD_VWEP         "toggle vweps"
#define CD_PAUSE        "toggle pause"
// { RACE
#define CD_RACE       	"toggle race mode"
#define CD_R_SSET       "set race start checkpoint"
#define CD_R_CSET       "set race checkpoint"
#define CD_R_ESET       "set race end checkpoint"
#define CD_R_CDEL       "remove race current checkpoint"
#define CD_R_ROUTE      "load predefined routes for map"
#define CD_C_ROUTE      "clear current route completely"
#define CD_R_PRINT      "show race route info"
#define CD_RREADY       "ready for race"
#define CD_RBREAK       "not ready for race"
#define CD_RBREAKALL    "force all racers to break"
#define CD_RTOGGLE      "toggle ready status for race"
#define CD_RCANCEL      "cancel current race, for racer"
#define CD_RTIMEOUT     "set race timeout"
#define CD_RFALSESTART  "set race starting mode"
#define CD_RMODE        "set race weapon mode"
#define CD_RFOLLOW      "follow racers with chasecam while waiting in line"
#define CD_RNOFOLLOW    "don't follow racers with chasecam while waiting in line"
#define CD_RFTOGGLE	    "toggle chasecam status"
#define CD_RCHASECAM	"cycle between chasecam views"
#define CD_RCHASECAMFL	"toggle chasecam freelook"
#define CD_RLINEUP		"show current race line-up"
#define CD_RSCORES		"show top race times for current map"
#define CD_RSCOREDETAIL "show details about a record"
#define CD_RDLDEMO      "download demo for a record"
#define CD_RPACEMAKER   "set pacemaker"
#define CD_RSIMULMODE   "toggle simultaneous racing"
#define CD_RMATCHMODE   "toggle race match mode"
#define CD_RSCORINGMODE "toggle between scoring systems"
#define CD_RHIDEPLAYERS "toggle visible players during race"
// }

#define CD_NOSPECS      "allow/disallow spectators"
#define CD_NOITEMS      "allow/disallow items in game"
#define CD_TEAMOVERLAY  "allow/disallow teamoverlay"

#define CD_SPAWN666TIME "set spawn pent time (dmm4 atm)"

#define CD_GIVEME       (CD_NODESC) // skip
#define CD_DROPITEM     (CD_NODESC) // skip
#define CD_REMOVEITEM   (CD_NODESC) // skip
#define CD_DUMPENT      (CD_NODESC) // skip

#define CD_VOTECOOP     "vote for coop on/off"
#define CD_COOPNMPU     "new nightmare mode (pu drops) on/off"

#define CD_MAPSLIST_DL  (CD_NODESC) // skip
#define CD_CMDSLIST_DL  (CD_NODESC) // skip

#define CD_DEMOMARK     "put mark in the demo"

#define CD_BOTCOMMAND   "bot configuration"

#define CD_PRIVATEGAME  "private game (logged in users only)"

#define CD_TEAMPLAYMESSAGE "teamplay messages"

#define CD_PICKSPAWN    "nominate hoonymode spawn"
#define CD_ROUNDSUP     "increase rounds in match"
#define CD_ROUNDSDOWN   "decrease rounds in match"

void dummy() {}
void redirect();

#define DEF(ptr) ((void (*)())(ptr))

cmd_t cmds[] = {
	{ "race",		 ToggleRace,				0    , CF_PLAYER | CF_SPC_ADMIN, CD_RACE },
	{ "cm",          SelectMap,                 0    , CF_BOTH | CF_MATCHLESS | CF_NOALIAS, CD_NODESC },
	{ "mapslist_dl", mapslist_dl,               0    , CF_BOTH | CF_MATCHLESS | CF_PARAMS | CF_NOALIAS | CF_CONNECTION_FLOOD, CD_MAPSLIST_DL },
	{ "cmdslist_dl", cmdslist_dl,               0    , CF_BOTH | CF_MATCHLESS | CF_PARAMS | CF_NOALIAS | CF_CONNECTION_FLOOD, CD_CMDSLIST_DL },
	{ "votemap",     VoteMap,                   0    , CF_BOTH | CF_MATCHLESS | CF_PARAMS, CD_VOTEMAP },
	{ "commands",    ShowCmds,                  0    , CF_BOTH | CF_MATCHLESS | CF_PARAMS, CD_COMMANDS },
	{ "scores",      PrintScores,               0    , CF_BOTH | CF_MATCHLESS, CD_SCORES },
	{ "stats",       PlayerStats,               0    , CF_BOTH | CF_MATCHLESS, CD_STATS },
	{ "effi",        PlayerStats,               0    , CF_BOTH | CF_MATCHLESS, CD_EFFI },
	{ "options",     ShowOpts,                  0    , CF_PLAYER, CD_OPTIONS },
	{ "ready",       PlayerReady,               0    , CF_BOTH | CF_MATCHLESS, CD_READY },
	{ "break",       PlayerBreak,               0    , CF_BOTH | CF_MATCHLESS, CD_BREAK },
	{ "status",      ModStatus,                 0    , CF_BOTH | CF_MATCHLESS, CD_STATUS },
	{ "status2",     ModStatus2,                0    , CF_BOTH | CF_MATCHLESS, CD_STATUS2 },
	{ "who",         PlayerStatus,              0    , CF_BOTH, CD_WHO },
	{ "whoskin",     PlayerStatusS,             0    , CF_BOTH | CF_MATCHLESS, CD_WHOSKIN },
	{ "whonot",      PlayerStatusN,             0    , CF_BOTH, CD_WHONOT },
	{ "list",        ListWhoNot,                0    , CF_PLAYER | CF_SPC_ADMIN, CD_LIST },
	{ "whovote",     ModStatusVote,             0    , CF_BOTH | CF_MATCHLESS, CD_WHOVOTE },
	{ "spawn",       ToggleRespawns,            0    , CF_PLAYER | CF_SPC_ADMIN, CD_SPAWN },
	{ "powerups",    TogglePowerups,            0    , CF_PLAYER | CF_SPC_ADMIN | CF_PARAMS, CD_POWERUPS },
	{ "powerups_pickup", TogglePuPickup,  	    0    , CF_PLAYER | CF_SPC_ADMIN | CF_PARAMS, CD_PUPICKUP },
	{ "antilag",     antilag,                   0    , CF_PLAYER | CF_SPC_ADMIN , CD_ANTILAG },
	{ "discharge",   ToggleDischarge,           0    , CF_PLAYER | CF_SPC_ADMIN , CD_DISCHARGE },
	{ "dm",          ShowDMM,                   0    , CF_PLAYER | CF_SPC_ADMIN , CD_DM },
	{ "dmm1",        DEF(ChangeDM),             1    , CF_PLAYER | CF_SPC_ADMIN, CD_DMM1 },
	{ "dmm2",        DEF(ChangeDM),             2    , CF_PLAYER | CF_SPC_ADMIN, CD_DMM2 },
	{ "dmm3",        DEF(ChangeDM),             3    , CF_PLAYER | CF_SPC_ADMIN, CD_DMM3 },
	{ "dmm4",        DEF(ChangeDM),             4    , CF_PLAYER | CF_SPC_ADMIN, CD_DMM4 },
	{ "dmm5",        DEF(ChangeDM),             5    , CF_PLAYER | CF_SPC_ADMIN, CD_DMM5 },
	{ "tp",          ChangeTP,                  0    , CF_PLAYER | CF_SPC_ADMIN, CD_TP },
	{ "timedown1",   DEF(TimeDown),           1.0f   , CF_PLAYER | CF_SPC_ADMIN, CD_TIMEDOWN1 },
	{ "timeup1",     DEF(TimeUp),             1.0f   , CF_PLAYER | CF_SPC_ADMIN, CD_TIMEUP1 },
	{ "timedown",    DEF(TimeDown),           5.0f   , CF_PLAYER | CF_SPC_ADMIN, CD_TIMEDOWN },
	{ "timeup",      DEF(TimeUp),             5.0f   , CF_PLAYER | CF_SPC_ADMIN, CD_TIMEUP },
	{ "fallbunny",   ToggleFallBunny,           0    , CF_PLAYER | CF_SPC_ADMIN, CD_FALLBUNNY },
	{ "fragsdown",   FragsDown,                 0    , CF_PLAYER | CF_SPC_ADMIN, CD_FRAGSDOWN },
	{ "fragsup",     FragsUp,                   0    , CF_PLAYER | CF_SPC_ADMIN, CD_FRAGSUP },
	{ "killquad",    killquad,                  0    , CF_PLAYER | CF_SPC_ADMIN, CD_KILLQUAD },
// qqshka: Pointless to have it, XonX command will turn it off anyway.
//	{ "bloodfest",   bloodfest,                 0    , CF_PLAYER | CF_SPC_ADMIN, CD_BLOODFEST },
	{ "dropquad",    ToggleDropQuad,            0    , CF_PLAYER | CF_SPC_ADMIN, CD_DROPQUAD },
	{ "dropring",    ToggleDropRing,            0    , CF_PLAYER | CF_SPC_ADMIN, CD_DROPRING },
	{ "droppack",    ToggleDropPack,            0    , CF_PLAYER | CF_SPC_ADMIN, CD_DROPPACK },

	{ "silence",     ToggleSpecTalk,            0    , CF_PLAYER | CF_SPC_ADMIN, CD_SILENCE },
	{ "report",      ReportMe,                  0    , CF_PLAYER, CD_REPORT },
	{ "rules",       ShowRules,                 0    , CF_PLAYER | CF_MATCHLESS, CD_RULES },
	{ "lockmode",    ChangeLock,                0    , CF_PLAYER | CF_SPC_ADMIN, CD_LOCKMODE },
	{ "maps",        ShowMaps,                  0    , CF_BOTH | CF_MATCHLESS | CF_PARAMS, CD_MAPS},
	{ "admin",       ReqAdmin,                  0    , CF_BOTH | CF_MATCHLESS | CF_PARAMS, CD_ADMIN },
	{ "forcestart",  AdminForceStart,           0    , CF_BOTH_ADMIN, CD_FORCESTART },
	{ "forcebreak",  AdminForceBreak,           0    , CF_BOTH_ADMIN, CD_FORCEBREAK },
	{ "pickup",      VotePickup,                0    , CF_PLAYER, CD_PICKUP }, 
	{ "prewar",      TogglePreWar,              0    , CF_BOTH_ADMIN, CD_PREWAR },
	{ "lockmap",     ToggleMapLock,             0    , CF_BOTH_ADMIN, CD_LOCKMAP },
	{ "speed",       ToggleSpeed,               0    , CF_PLAYER, CD_SPEED },
	{ "fairpacks",   ToggleFairPacks,           0    , CF_PLAYER, CD_FAIRPACKS },
	{ "sct_oct",  	 ShowCharsetTableOctal,     0    , CF_BOTH, CD_CTOCT },
	{ "sct_hex",  	 ShowCharsetTableHexa,      0    , CF_BOTH, CD_CTHEX },
	{ "about",       ShowVersion,               0    , CF_BOTH | CF_MATCHLESS, CD_ABOUT },
	{ "shownick",    ShowNick,                  0    , CF_PLAYER | CF_PARAMS, CD_SHOWNICK },
	{ "time5",       DEF(TimeSet),            5.0f   , CF_PLAYER | CF_SPC_ADMIN, CD_TIME5 },
	{ "time10",      DEF(TimeSet),           10.0f   , CF_PLAYER | CF_SPC_ADMIN, CD_TIME10 },
	{ "time15",      DEF(TimeSet),           15.0f   , CF_PLAYER | CF_SPC_ADMIN, CD_TIME15 },
	{ "time20",      DEF(TimeSet),           20.0f   , CF_PLAYER | CF_SPC_ADMIN, CD_TIME20 },
	{ "time25",      DEF(TimeSet),           25.0f   , CF_PLAYER | CF_SPC_ADMIN, CD_TIME25 },
	{ "time30",      DEF(TimeSet),           30.0f   , CF_PLAYER | CF_SPC_ADMIN, CD_TIME30 },

	{ "ksound1",     DEF(TeamSay),              1    , CF_PLAYER, CD_KSOUND1 },
	{ "ksound2",     DEF(TeamSay),              2    , CF_PLAYER, CD_KSOUND2 },
	{ "ksound3",     DEF(TeamSay),              3    , CF_PLAYER, CD_KSOUND3 },
	{ "ksound4",     DEF(TeamSay),              4    , CF_PLAYER, CD_KSOUND4 },
	{ "ksound5",     DEF(TeamSay),              5    , CF_PLAYER, CD_KSOUND5 },
	{ "ksound6",     DEF(TeamSay),              6    , CF_PLAYER, CD_KSOUND6 },

	{ "qizmo",       ShowQizmo,                 0    , CF_PLAYER, CD_QIZMO },

//	{ "messages",    ShowMessages,              0    , CF_PLAYER | CF_MATCHLESS, CD_MESSAGES },
	{ "killer",      SendKillerMsg,             0    , CF_PLAYER | CF_MATCHLESS, CD_KILLER },
	{ "victim",      SendVictimMsg,             0    , CF_PLAYER | CF_MATCHLESS, CD_VICTIM },
	{ "newcomer",    SendNewcomerMsg,           0    , CF_BOTH | CF_MATCHLESS, CD_NEWCOMER },

	{ "qlag",        ToggleQLag,                0    , CF_PLAYER | CF_SPC_ADMIN, CD_QLAG },
	{ "qenemy",      ToggleQEnemy,              0    , CF_PLAYER | CF_SPC_ADMIN, CD_QENEMY },
	{ "qpoint",      ToggleQPoint,              0    , CF_PLAYER | CF_SPC_ADMIN, CD_QPOINT },
/* new FDP bits https://www.quakeworld.nu/wiki/FPD
	{ "skinforce",   ToggleSkinForcing,         0    , CF_PLAYER | CF_SPC_ADMIN, CD_SFORCING },
	{ "colorforce",  ToggleColorForcing,        0    , CF_PLAYER | CF_SPC_ADMIN, CD_CFORCING },
	{ "pitchsl",     TogglePitchSpeedLimit,     0    , CF_PLAYER | CF_SPC_ADMIN, CD_PITCHSP },
	{ "yawsl",       ToggleYawSpeedLimit,       0    , CF_PLAYER | CF_SPC_ADMIN, CD_YAWSP },
*/

	{ "kick",        AdminKick,                 0    , CF_BOTH_ADMIN/* FIXME: interference with ezq server kick command | CF_PARAMS */, CD_KICK },
	{ "mkick",       m_kick,                    0    , CF_BOTH_ADMIN | CF_PARAMS, CD_MKICK },
	{ "y",           YesKick,                   0    , CF_BOTH_ADMIN, CD_Y },
	{ "n",           DontKick,                  0    , CF_BOTH_ADMIN, CD_N },
	{ "overtime",    ChangeOvertime,            0    , CF_PLAYER | CF_SPC_ADMIN, CD_OVERTIME },
	{ "overtimeup",  ChangeOvertimeUp,          0    , CF_PLAYER | CF_SPC_ADMIN, CD_OVERTIMEUP },
	{ "elect",       VoteAdmin,                 0    , CF_BOTH | CF_MATCHLESS, CD_ELECT },
	{ "yes",         VoteYes,                   0    , CF_PLAYER | CF_MATCHLESS, CD_YES },
	{ "no",          VoteNo,                    0    , CF_PLAYER | CF_MATCHLESS, CD_NO },
	{ "captain",     VoteCaptain,               0    , CF_PLAYER, CD_CAPTAIN },
	{ "coach",       VoteCoach,                 0    , CF_SPECTATOR, CD_COACH },
	{ "freeze",      ToggleFreeze,              0    , CF_PLAYER | CF_SPC_ADMIN, CD_FREEZE },
	{ "rpickup",     RandomPickup,              0    , CF_PLAYER | CF_SPC_ADMIN, CD_RPICKUP },

	{ "1on1",         DEF(UserMode),             1   , CF_PLAYER | CF_SPC_ADMIN | CF_PARAMS, CD_1ON1 },
	{ "2on2",         DEF(UserMode),             2   , CF_PLAYER | CF_SPC_ADMIN | CF_PARAMS, CD_2ON2 },
	{ "3on3",         DEF(UserMode),             3   , CF_PLAYER | CF_SPC_ADMIN | CF_PARAMS, CD_3ON3 },
	{ "4on4",         DEF(UserMode),             4   , CF_PLAYER | CF_SPC_ADMIN | CF_PARAMS, CD_4ON4 },
	{ "10on10",       DEF(UserMode),             5   , CF_PLAYER | CF_SPC_ADMIN | CF_PARAMS, CD_10ON10 },
	{ "ffa",          DEF(UserMode),             6	 , CF_PLAYER | CF_SPC_ADMIN | CF_PARAMS, CD_FFA },
	{ "ctf",          DEF(UserMode),             7   , CF_PLAYER | CF_SPC_ADMIN | CF_PARAMS, CD_CTF },
	{ "hoonymode",    DEF(UserMode),             8   , CF_PLAYER | CF_SPC_ADMIN | CF_PARAMS, CD_1ON1HM },
	{ "blitz2v2",     DEF(UserMode),             9   , CF_PLAYER | CF_SPC_ADMIN | CF_PARAMS, CD_2ON2BLITZ },
	{ "blitz4v4",     DEF(UserMode),             10  , CF_PLAYER | CF_SPC_ADMIN | CF_PARAMS, CD_4ON4BLITZ },

	{ "practice",    TogglePractice,            0    , CF_PLAYER | CF_SPC_ADMIN, CD_PRACTICE },
	{ "wp_reset",    Wp_Reset,                  0    , CF_PLAYER, CD_WP_RESET },
	{ "+wp_stats",   DEF(Wp_Stats),             2    , CF_BOTH | CF_MATCHLESS, CD_PLS_WP_STATS },
	{ "-wp_stats",   DEF(Wp_Stats),             1    , CF_BOTH | CF_MATCHLESS, CD_MNS_WP_STATS },
	{ "tkfjump",     DEF(t_jump),               1    , CF_BOTH_ADMIN, CD_TKFJUMP },
	{ "tkrjump",     DEF(t_jump),               2    , CF_BOTH_ADMIN, CD_TKRJUMP },
	{ "klist",       klist,                     0    , CF_BOTH | CF_MATCHLESS, CD_KLIST },
	{ "hdptoggle",   hdptoggle,                 0    , CF_BOTH_ADMIN, CD_HDPTOGGLE },
	{ "handicap",    handicap,                  0    , CF_PLAYER | CF_PARAMS | CF_MATCHLESS, CD_HANDICAP },
	{ "noweapon",    noweapon,                  0    , CF_PLAYER | CF_PARAMS | CF_SPC_ADMIN, CD_NOWEAPON },

	{ "cam",         ShowCamHelp,               0    , CF_SPECTATOR | CF_MATCHLESS, CD_CAM },

	{ "tracklist",   tracklist,                 0    , CF_BOTH | CF_MATCHLESS, CD_TRACKLIST },
	{ "fpslist",     fpslist,                   0    , CF_BOTH | CF_MATCHLESS, CD_FPSLIST },

	{ "fav1_add",    DEF(favx_add),             1    , CF_SPECTATOR, CD_FAV1_ADD },
	{ "fav2_add",    DEF(favx_add),             2    , CF_SPECTATOR, CD_FAV2_ADD },
	{ "fav3_add",    DEF(favx_add),             3    , CF_SPECTATOR, CD_FAV3_ADD },
	{ "fav4_add",    DEF(favx_add),             4    , CF_SPECTATOR, CD_FAV4_ADD },
	{ "fav5_add",    DEF(favx_add),             5    , CF_SPECTATOR, CD_FAV5_ADD },
	{ "fav6_add",    DEF(favx_add),             6    , CF_SPECTATOR, CD_FAV6_ADD },
	{ "fav7_add",    DEF(favx_add),             7    , CF_SPECTATOR, CD_FAV7_ADD },
	{ "fav8_add",    DEF(favx_add),             8    , CF_SPECTATOR, CD_FAV8_ADD },
	{ "fav9_add",    DEF(favx_add),             9    , CF_SPECTATOR, CD_FAV9_ADD },
	{ "fav10_add",   DEF(favx_add),            10    , CF_SPECTATOR, CD_FAV10_ADD },
	{ "fav11_add",   DEF(favx_add),            11    , CF_SPECTATOR, CD_FAV11_ADD },
	{ "fav12_add",   DEF(favx_add),            12    , CF_SPECTATOR, CD_FAV12_ADD },
	{ "fav13_add",   DEF(favx_add),            13    , CF_SPECTATOR, CD_FAV13_ADD },
	{ "fav14_add",   DEF(favx_add),            14    , CF_SPECTATOR, CD_FAV14_ADD },
	{ "fav15_add",   DEF(favx_add),            15    , CF_SPECTATOR, CD_FAV15_ADD },
	{ "fav16_add",   DEF(favx_add),            16    , CF_SPECTATOR, CD_FAV16_ADD },
	{ "fav17_add",   DEF(favx_add),            17    , CF_SPECTATOR, CD_FAV17_ADD },
	{ "fav18_add",   DEF(favx_add),            18    , CF_SPECTATOR, CD_FAV18_ADD },
	{ "fav19_add",   DEF(favx_add),            19    , CF_SPECTATOR, CD_FAV19_ADD },
	{ "fav20_add",   DEF(favx_add),            20    , CF_SPECTATOR, CD_FAV20_ADD },
	{ "1fav_go",     DEF(xfav_go),              1    , CF_SPECTATOR, CD_1FAV_GO },
	{ "2fav_go",     DEF(xfav_go),              2    , CF_SPECTATOR, CD_2FAV_GO },
	{ "3fav_go",     DEF(xfav_go),              3    , CF_SPECTATOR, CD_3FAV_GO },
	{ "4fav_go",     DEF(xfav_go),              4    , CF_SPECTATOR, CD_4FAV_GO },
	{ "5fav_go",     DEF(xfav_go),              5    , CF_SPECTATOR, CD_5FAV_GO },
	{ "6fav_go",     DEF(xfav_go),              6    , CF_SPECTATOR, CD_6FAV_GO },
	{ "7fav_go",     DEF(xfav_go),              7    , CF_SPECTATOR, CD_7FAV_GO },
	{ "8fav_go",     DEF(xfav_go),              8    , CF_SPECTATOR, CD_8FAV_GO },
	{ "9fav_go",     DEF(xfav_go),              9    , CF_SPECTATOR, CD_9FAV_GO },
	{ "10fav_go",    DEF(xfav_go),             10    , CF_SPECTATOR, CD_10FAV_GO },
	{ "11fav_go",    DEF(xfav_go),             11    , CF_SPECTATOR, CD_11FAV_GO },
	{ "12fav_go",    DEF(xfav_go),             12    , CF_SPECTATOR, CD_12FAV_GO },
	{ "13fav_go",    DEF(xfav_go),             13    , CF_SPECTATOR, CD_13FAV_GO },
	{ "14fav_go",    DEF(xfav_go),             14    , CF_SPECTATOR, CD_14FAV_GO },
	{ "15fav_go",    DEF(xfav_go),             15    , CF_SPECTATOR, CD_15FAV_GO },
	{ "16fav_go",    DEF(xfav_go),             16    , CF_SPECTATOR, CD_16FAV_GO },
	{ "17fav_go",    DEF(xfav_go),             17    , CF_SPECTATOR, CD_17FAV_GO },
	{ "18fav_go",    DEF(xfav_go),             18    , CF_SPECTATOR, CD_18FAV_GO },
	{ "19fav_go",    DEF(xfav_go),             19    , CF_SPECTATOR, CD_19FAV_GO },
	{ "20fav_go",    DEF(xfav_go),             20    , CF_SPECTATOR, CD_20FAV_GO },
	{ "fav_add",     fav_add,                   0    , CF_SPECTATOR | CF_MATCHLESS, CD_FAV_ADD },
	{ "fav_del",     fav_del,                   0    , CF_SPECTATOR | CF_MATCHLESS, CD_FAV_DEL },
	{ "fav_all_del", fav_all_del,               0    , CF_SPECTATOR | CF_MATCHLESS, CD_FAV_ALL_DEL },
	{ "fav_next",    fav_next,                  0    , CF_SPECTATOR | CF_MATCHLESS, CD_FAV_NEXT },
	{ "fav_show",    fav_show,                  0    , CF_SPECTATOR | CF_MATCHLESS, CD_FAV_SHOW },
	{ "+scores",     DEF(Sc_Stats),             2    , CF_BOTH | CF_MATCHLESS, CD_PLS_SCORES },
	{ "-scores",     DEF(Sc_Stats),             1    , CF_BOTH | CF_MATCHLESS, CD_MNS_SCORES },
	{ "autotrack",   DEF(AutoTrack),       atKTPRO   , CF_SPECTATOR | CF_MATCHLESS, CD_AUTOTRACK },
	{ "autotrackktx",DEF(AutoTrack),       atBest    , CF_SPECTATOR | CF_MATCHLESS, CD_AUTOTRACKKTX },
	{ "auto_pow",    DEF(AutoTrack),        atPow    , CF_SPECTATOR | CF_MATCHLESS, CD_AUTO_POW },
	{ "next_best",   next_best,                 0    , CF_SPECTATOR | CF_MATCHLESS, CD_NEXT_BEST },
	{ "next_pow",    next_pow,                  0    , CF_SPECTATOR | CF_MATCHLESS, CD_NEXT_POW },
	{ "lastscores",  lastscores,                0    , CF_BOTH | CF_MATCHLESS | CF_PARAMS, CD_LASTSCORES },
	{ "rnd",         krnd,                      0    , CF_BOTH | CF_PARAMS, CD_RND },
	{ "agree",       agree_on_map,              0    , CF_PLAYER | CF_MATCHLESS, CD_AGREE },
	{ "pos_show",    Pos_Show,                  0    , CF_BOTH | CF_PARAMS, CD_POS_SHOW },
	{ "pos_save",    Pos_Save,                  0    , CF_BOTH | CF_PARAMS, CD_POS_SAVE },
	{ "pos_move",    Pos_Move,                  0    , CF_BOTH | CF_PARAMS, CD_POS_MOVE },
// VVD: For trick chiters! :-)
// Need to think out how to limit using Pos_Set for tricking.
// May be to ban pos_velocity?
	{ "pos_origin",  DEF(Pos_Set),              1    , CF_BOTH | CF_PARAMS, CD_POS_ORIGIN },
	{ "pos_angles",  DEF(Pos_Set),              2    , CF_BOTH | CF_PARAMS, CD_POS_ANGLES },
//	{ "pos_velocity", DEF(Pos_Set),                   3    , CF_BOTH | CF_PARAMS, CD_POS_VELOCITY },
	{ "sh_speed",    Sh_Speed,                  0    , CF_BOTH, CD_SH_SPEED },
// { CTF commands
	{ "tossrune",    TossRune,                  0    , CF_PLAYER | CF_MATCHLESS, CD_TOSSRUNE },
	{ "tossflag",    TossFlag,                  0    , CF_PLAYER | CF_MATCHLESS, CD_TOSSFLAG },
	{ "nohook",      nohook,                    0    , CF_BOTH_ADMIN | CF_MATCHLESS, CD_NOHOOK },
	{ "crhook",      crhook,                    0    , CF_BOTH_ADMIN | CF_MATCHLESS, CD_CRHOOK },
	{ "norunes",     norunes,                   0    , CF_BOTH_ADMIN | CF_MATCHLESS, CD_NORUNES },
	{ "noga",        noga,                      0    , CF_BOTH_ADMIN | CF_MATCHLESS, CD_NOGA },
	{ "mctf",        mctf,                      0    , CF_BOTH_ADMIN | CF_MATCHLESS, CD_MCTF },
	{ "flagstatus",  FlagStatus,                0    , CF_BOTH | CF_MATCHLESS, CD_FLAGSTATUS },
	{ "swapall",     AdminSwapAll,              0    , CF_BOTH_ADMIN, CD_SWAPALL },

	{ "ctfbasedspawn", CTFBasedSpawn,           0    , CF_PLAYER | CF_SPC_ADMIN | CF_MATCHLESS, CD_CTFBASEDSPAWN },
// }
	{ "motd",        motd_show,                 0    , CF_BOTH | CF_MATCHLESS, CD_MOTD },
	{ "infolock",    infolock,                  0    , CF_BOTH_ADMIN, CD_INFOLOCK },
	{ "infospec",    infospec,                  0    , CF_PLAYER | CF_SPC_ADMIN, CD_INFOSPEC },
	{ "moreinfo",    moreinfo,                  0    , CF_SPECTATOR | CF_MATCHLESS, CD_MOREINFO },
	{ "s-p",         dummy,                     0    , CF_BOTH | CF_MATCHLESS | CF_PARAMS, CD_S_P },
	{ "s-l",         dummy,                     0    , CF_BOTH | CF_MATCHLESS | CF_PARAMS, CD_S_L },
	{ "s-r",         dummy,                     0    , CF_BOTH | CF_MATCHLESS | CF_PARAMS, CD_S_R },
	{ "s-t",         dummy,                     0    , CF_BOTH | CF_MATCHLESS | CF_PARAMS, CD_S_T },
	{ "s-m",         dummy,                     0    , CF_BOTH | CF_MATCHLESS | CF_PARAMS, CD_S_M },
	{ "mmode",       mmode,                     0    , CF_BOTH | CF_MATCHLESS | CF_PARAMS, CD_MMODE },
	{ "multi",       multi,                     0    , CF_BOTH | CF_MATCHLESS | CF_PARAMS, CD_MULTI },
	{ "kinfo",       cmdinfo,                   0    , CF_BOTH | CF_MATCHLESS | CF_PARAMS, CD_KINFO },
	{ "kuinfo",      cmduinfo,                  0    , CF_BOTH | CF_MATCHLESS | CF_PARAMS, CD_KUINFO },
// { saved for ktpro compatibility
	{ "info",        cmdinfo,                   0    , CF_BOTH | CF_MATCHLESS | CF_PARAMS | CF_NOALIAS, CD_NODESC },
	{ "uinfo",       cmduinfo,                  0    , CF_BOTH | CF_MATCHLESS | CF_PARAMS | CF_NOALIAS, CD_NODESC },
// }
	{ "wreg",        cmd_wreg,                  0    , CF_BOTH | CF_MATCHLESS | CF_PARAMS, CD_WREG },
	{ "kill",        ClientKill,                0    , CF_PLAYER | CF_MATCHLESS, CD_KILL },
	{ "midair",      ToggleMidair,              0    , CF_PLAYER | CF_SPC_ADMIN, CD_MIDAIR },
	{ "midair_minheight", SetMidairMinHeight,   0    , CF_PLAYER | CF_SPC_ADMIN, CD_MIDAIR_MINHEIGHT },
	{ "instagib",    ToggleInstagib,            0    , CF_PLAYER | CF_SPC_ADMIN, CD_INSTAGIB },
	{ "berzerk",     ToggleBerzerk,             0    , CF_PLAYER | CF_SPC_ADMIN, CD_BERZERK },
	{ "lgcmode",     ToggleLGC,                 0    , CF_PLAYER | CF_SPC_ADMIN, CD_LGC },
	{ "instagib_coilgun_kickback", ToggleCGKickback, 0, CF_PLAYER | CF_SPC_ADMIN, CD_CG_KB },
	{ "time",        sv_time,                   0    , CF_BOTH | CF_MATCHLESS, CD_TIME },
	{ "gren_mode",   GrenadeMode,               0    , CF_PLAYER | CF_SPC_ADMIN, CD_GREN_MODE },
	{ "toggleready", ToggleReady,               0    , CF_BOTH | CF_MATCHLESS, CD_TOGGLEREADY },
	{ "fp",          DEF(fp_toggle),            1    , CF_BOTH_ADMIN, CD_FP },
	{ "fp_spec",     DEF(fp_toggle),            2    , CF_BOTH_ADMIN, CD_FP_SPEC },
	{ "dlist",       dlist,                     0    , CF_BOTH | CF_MATCHLESS | CF_PARAMS, CD_DLIST },
	{ "dinfo",       dinfo,                     0    , CF_BOTH | CF_MATCHLESS | CF_PARAMS, CD_DINFO },
	{ "lock",        sv_lock,                   0    , CF_BOTH_ADMIN, CD_LOCK },
// { RA
	{ "ra_break",    ra_break,                  0    , CF_PLAYER, CD_RA_BREAK },
	{ "ra_pos",      ra_PrintPos,               0    , CF_PLAYER, CD_RA_POS },
	{ "arena",       ToggleArena,               0    , CF_PLAYER | CF_SPC_ADMIN, CD_ARENA },
// }
// { Clan Arena
	{ "carena",      ToggleCArena,              0    , CF_PLAYER | CF_SPC_ADMIN, CD_CARENA },
// }
	{ "force_spec",  force_spec,                0    , CF_BOTH_ADMIN | CF_PARAMS, CD_FORCE_SPEC },
// { bans
	{ "ban",         redirect,                  0    , CF_BOTH_ADMIN | CF_MATCHLESS | CF_PARAMS | CF_REDIRECT, CD_BAN },
	{ "banip",       redirect,                  0    , CF_BOTH_ADMIN | CF_MATCHLESS | CF_PARAMS | CF_REDIRECT, CD_BANIP },
	{ "banrem",      redirect,                  0    , CF_BOTH_ADMIN | CF_MATCHLESS | CF_PARAMS | CF_REDIRECT, CD_BANREM },
// }
	{ "teleteam",    teleteam,                  0    , CF_PLAYER | CF_SPC_ADMIN, CD_TELETEAM },
	{ "upplayers",   DEF(upplayers),            1    , CF_PLAYER | CF_SPC_ADMIN, CD_UPPLAYERS },
	{ "downplayers", DEF(downplayers),          1    , CF_PLAYER | CF_SPC_ADMIN, CD_DOWNPLAYERS },
	{ "upspecs",     DEF(upplayers),            2    , CF_PLAYER | CF_SPC_ADMIN, CD_UPSPECS },
	{ "downspecs",   DEF(downplayers),          2    , CF_PLAYER | CF_SPC_ADMIN, CD_DOWNSPECS },
	{ "iplist",      iplist,                    0    , CF_BOTH, CD_IPLIST },
	{ "dmgfrags",    dmgfrags,                  0    , CF_PLAYER | CF_SPC_ADMIN, CD_DMGFRAGS },
	{ "no_lg",       no_lg,                     0    , CF_PLAYER | CF_SPC_ADMIN, CD_NO_LG },
	{ "no_gl",       no_gl,                     0    , CF_PLAYER | CF_SPC_ADMIN, CD_NO_GL },
// {
	{ "trx_rec",     mv_cmd_record,             0    , CF_PLAYER, CD_TRX_REC },
	{ "trx_play",    mv_cmd_playback,           0    , CF_PLAYER, CD_TRX_PLAY },
	{ "trx_stop",    mv_cmd_stop,               0    , CF_PLAYER, CD_TRX_STOP },
// }
	{ "callalias",   callalias,                 0    , CF_BOTH | CF_MATCHLESS | CF_PARAMS, CD_CALLALIAS },
	{ "check",       fcheck,                    0    , CF_BOTH | CF_PARAMS, CD_CHECK },
	{ "next_map",    PlayerBreak,               0    , CF_PLAYER | CF_MATCHLESS_ONLY, CD_NEXT_MAP },
	{ "mapcycle",    mapcycle,                  0    , CF_BOTH | CF_MATCHLESS, CD_MAPCYCLE },
	{ "yawnmode",    ToggleYawnMode,            0    , CF_PLAYER | CF_SPC_ADMIN, CD_YAWNMODE },
	{ "teleportcap", setTeleportCap,            0    , CF_PLAYER | CF_SPC_ADMIN | CF_PARAMS, CD_TELEPORTCAP },
	{ "airstep",     airstep,                   0    , CF_PLAYER | CF_SPC_ADMIN, CD_AIRSTEP },
	{ "exclusive",   ToggleExclusive,           0    , CF_BOTH_ADMIN, CD_EXCLUSIVE },
	{ "vwep",        ToggleVwep,                0    , CF_PLAYER | CF_SPC_ADMIN, CD_VWEP },
	{ "pause",       TogglePause,               0    , CF_PLAYER | CF_MATCHLESS | CF_SPC_ADMIN, CD_PAUSE },
// { RACE
	{ "race_ready",                 DEF(r_changestatus),            1,  CF_PLAYER,                              CD_RREADY },
	{ "race_break",                 DEF(r_changestatus),            2,  CF_PLAYER,                              CD_RBREAK },
	{ "race_break_all",             r_all_break,                    0,  CF_BOTH_ADMIN,                          CD_RBREAKALL },
	{ "race_toggle",                DEF(r_changestatus),            3,  CF_PLAYER,                              CD_RTOGGLE },
	{ "race_cancel",                DEF(r_changestatus),            4,  CF_PLAYER,                              CD_RCANCEL },
	{ "race_show_lineup",           race_display_line,              0,  CF_BOTH,                                CD_RLINEUP },
	{ "race_show_toptimes",         display_scores,                 0,  CF_BOTH,                                CD_RSCORES },
	{ "race_show_record_details",   display_record_details,         0,  CF_BOTH | CF_PARAMS,                    CD_RSCOREDETAIL },
	{ "race_show_route",            r_print,                        0,  CF_BOTH,                                CD_R_PRINT },
	{ "race_set_start",             DEF(r_Xset),                    1,  CF_PLAYER | CF_SPC_ADMIN,               CD_R_SSET },
	{ "race_set_finish",            DEF(r_Xset),                    3,  CF_PLAYER | CF_SPC_ADMIN,               CD_R_ESET },
	{ "race_set_checkpoint",        DEF(r_Xset),                    2,  CF_PLAYER | CF_SPC_ADMIN,               CD_R_CSET },
	{ "race_del_checkpoint",        r_cdel,                         0,  CF_PLAYER | CF_SPC_ADMIN,               CD_R_CDEL },
	{ "race_set_timeout",           r_timeout,                      0,  CF_PLAYER | CF_SPC_ADMIN | CF_PARAMS,   CD_RTIMEOUT },
	{ "race_set_falsestart",        r_falsestart,                   0,  CF_PLAYER | CF_SPC_ADMIN | CF_PARAMS,   CD_RFALSESTART },
	{ "race_set_weapon_mode",       r_mode,                         0,  CF_PLAYER | CF_SPC_ADMIN,               CD_RMODE },
	{ "race_route_switch",          r_route,                        0,  CF_PLAYER | CF_SPC_ADMIN,               CD_R_ROUTE },
	{ "race_route_clear",           r_clear_route,                  0,  CF_PLAYER | CF_SPC_ADMIN,               CD_C_ROUTE },
	{ "race_chasecam",              DEF(r_changefollowstatus),      3,  CF_PLAYER,                              CD_RFTOGGLE },
	{ "race_chasecam_view",         race_chasecam_change,           0,  CF_PLAYER,                              CD_RCHASECAM },
	{ "race_chasecam_freelook",     race_chasecam_freelook_change,  0,  CF_PLAYER,                              CD_RCHASECAMFL },
	{ "race_dl_record_demo",        race_download_record_demo,      0,  CF_BOTH | CF_PARAMS,                    CD_RDLDEMO },
	{ "race_pacemaker",             race_pacemaker,                 0,  CF_PLAYER | CF_PARAMS,                  CD_RPACEMAKER },
	{ "race_simultaneous",          race_simultaneous_toggle,       0,  CF_PLAYER,                              CD_RSIMULMODE },
	{ "race_match",                 race_match_toggle,              0,  CF_PLAYER,                              CD_RMATCHMODE },
	{ "race_scoring",               race_scoring_system_toggle,     0,  CF_PLAYER,                              CD_RSCORINGMODE },
	{ "race_hide_players",          race_hide_players_toggle,       0,  CF_PLAYER,                              CD_RHIDEPLAYERS },
// }
	{ "nospecs",     nospecs,                   0    , CF_PLAYER | CF_SPC_ADMIN, CD_NOSPECS },
	{ "noitems",     noitems,                   0    , CF_PLAYER | CF_SPC_ADMIN, CD_NOITEMS },
	{ "teamoverlay", teamoverlay,               0    , CF_PLAYER | CF_SPC_ADMIN, CD_TEAMOVERLAY },
	{ "spawn666time",Spawn666Time,              0    , CF_PLAYER | CF_SPC_ADMIN | CF_PARAMS, CD_SPAWN666TIME },
	{ "giveme",      giveme,                    0    , CF_PLAYER | CF_MATCHLESS | CF_PARAMS, CD_GIVEME },
	{ "dropitem",    dropitem,                  0    , CF_BOTH | CF_PARAMS, CD_DROPITEM },
	{ "removeitem",  removeitem,                0    , CF_BOTH | CF_PARAMS, CD_REMOVEITEM },
	{ "dumpent",     dumpent,                   0    , CF_BOTH | CF_PARAMS, CD_DUMPENT },
	{ "votecoop",    votecoop,                  0    , CF_PLAYER | CF_MATCHLESS, CD_VOTECOOP },
	{ "coop_nm_pu",	 ToggleNewCoopNm,           0    , CF_PLAYER | CF_MATCHLESS, CD_COOPNMPU },
	{ "demomark",	 DemoMark,                  0    , CF_BOTH, CD_DEMOMARK },

#ifdef BOT_SUPPORT
// { FROGBOTS
	{ "botcmd",      FrogbotsCommand,           0    , CF_BOTH | CF_MATCHLESS | CF_PARAMS, CD_BOTCOMMAND },
// }
#endif

// { TEAMPLAY
	{ "tpmsg",       TeamplayMessage,           0    , CF_PLAYER | CF_PARAMS | CF_MATCHLESS, CD_TEAMPLAYMESSAGE },

// { HOONYMODE
	{ "pickspawn",   HM_pick_spawn,             0    , CF_PLAYER, CD_PICKSPAWN },
	{ "roundsup",    HM_roundsup,               0    , CF_PLAYER, CD_ROUNDSUP },
	{ "roundsdown",  HM_roundsdown,             0    , CF_PLAYER, CD_ROUNDSDOWN },
// }

	{ "voteprivate", private_game_vote,         0    , CF_PLAYER, CD_PRIVATEGAME }
};

#undef DEF

int cmds_cnt = sizeof( cmds ) / sizeof( cmds[0] );

int DoCommand(int icmd)
{
	int spc = self->ct == ctSpec;

	if ( !( icmd >= 0 && icmd < cmds_cnt ) )
		return DO_OUT_OF_RANGE_CMDS;

	if ( k_matchLess && !(cmds[icmd].cf_flags & CF_MATCHLESS) )
		return DO_CMD_DISALLOWED_MATCHLESS; // cmd isn't allowed in matchLess mode

	if ( !k_matchLess && (cmds[icmd].cf_flags & CF_MATCHLESS_ONLY) )
		return DO_CMD_MATCHLESS_ONLY; // cmd allowed in matchLess mode _only_

	if ( spc ) { // spec
		if ( !(cmds[icmd].cf_flags & CF_SPECTATOR) )
			return DO_WRONG_CLASS; // cmd not for spectator
		if ( (cmds[icmd].cf_flags & CF_SPC_ADMIN) && !is_adm(self) ) {
			G_sprint(self, 2, "You are not an admin\n");
			return DO_ACCESS_DENIED; // admin rights required
		}
	}
	else { // player
		if ( !(cmds[icmd].cf_flags & CF_PLAYER) )
			return DO_WRONG_CLASS; // cmd not for player
		if ( (cmds[icmd].cf_flags & CF_PLR_ADMIN) && !is_adm(self) ) {
			G_sprint(self, 2, "You are not an admin\n");
			return DO_ACCESS_DENIED; // admin rights required
		}
	}

	if ( strnull( cmds[icmd].name ) || !( cmds[icmd].f ) )
		return DO_FUNCTION_IS_WRONG;

	if ( (cmds[icmd].cf_flags & CF_CONNECTION_FLOOD) && self->connect_time + 30 > g_globalvars.time )
	{
		; // ignore flood check for first 30 seconds after connect, for commands marked with CF_CONNECTION_FLOOD flag
	}
	else if ( isCmdFlood( self ) )
	{
		return DO_FLOOD_PROTECT;
	}

	if (cmds[icmd].arg)
		( ( void ( * )(float) ) ( cmds[icmd].f ) ) ( cmds[icmd].arg );
	else
		( cmds[icmd].f )  ();

	return icmd;
}

// VVD: Need for executing commands by 'cmd <CMD_NAME> <ARG1> ... <ARGn>',
// because '<CMD_NAME> <ARG1> ... <ARGn>' work only with last ezQuake qw client.
int DoCommand_Name(char *cmd_name)
{
	int i;

	if ( strnull( cmd_name ) )
		return DO_OUT_OF_RANGE_CMDS;

	for ( i = 0; i < cmds_cnt; ++i ) {
		if ( streq(cmds[i].name, cmd_name) ) {
			if (cmds[i].cf_flags & CF_REDIRECT)
				return DO_OUT_OF_RANGE_CMDS; // imitate we does't found command in redirect case

			return DoCommand( i );
		}
	}

	return DO_OUT_OF_RANGE_CMDS;
}

qbool isCmdFlood(gedict_t *p)
{
	int idx;
	float cmd_time;

	if ( k_cmd_fp_disabled || p->connect_time + 5 > g_globalvars.time )
		return false; // cmd flood protect is disabled or skip near connect time due to tons of "cmd info" commands is done

	if (cvar("sv_paused"))	// FIXME: g_globalvars.time does not increase when paused, so if you
		return false;		// triggered floodprot you wouldn't be able to unpause

	idx = bound(0, p->fp_c.last_cmd, MAX_FP_CMDS-1);
	cmd_time = p->fp_c.cmd_time[idx];

	if ( g_globalvars.time < p->fp_c.locked )
	{
		G_sprint(p, 2, "command floodprot (%d sec)\n",
										(int)(p->fp_c.locked - g_globalvars.time + 1));
		return true; // flooder
	}

	if ( cmd_time && ( g_globalvars.time - cmd_time < k_cmd_fp_per ) )
	{
		G_sprint(p, 2, "You are a command flooder man!\n");

		p->fp_c.locked = g_globalvars.time + k_cmd_fp_for;

		if ( !k_cmd_fp_dontkick ) {
			if ( k_cmd_fp_kick - p->fp_c.warnings > 1 ) {
				G_sprint(p, 2, "%d warnings to kick\n", k_cmd_fp_kick - p->fp_c.warnings);
			}
			else if ( k_cmd_fp_kick - p->fp_c.warnings == 1 ) {
				G_sprint(p, 2, "next time you will be kicked\n");
			}
			else if ( k_cmd_fp_kick - p->fp_c.warnings < 1 ) {
				if ( p->ct == ctPlayer || ( p->ct == ctSpec && !match_in_progress ) )
					G_bprint(2,"%s is a command flooooder!!!\nand will be kicked\n", getname(p));

				G_sprint(p, 2, "Go away!\n");

				stuffcmd(p, "disconnect\n"); // FIXME: stupid way
			}
		}

		p->fp_c.warnings += 1;

		return true; // flooder
	}

	p->fp_c.cmd_time[idx] = g_globalvars.time;

	if ( ++idx >= k_cmd_fp_count )
		idx = 0;

	p->fp_c.last_cmd = idx;

	return false;
}

void redirect()
{
	int i;
	char	cmd_command[1024];

	trap_CmdArgv( 0, cmd_command, sizeof( cmd_command ) );
	if ( !only_digits(cmd_command) || !((i = atoi(cmd_command)) >= 0 && i < cmds_cnt) )
		return; // sanity

	stuffcmd_flags(self, STUFFCMD_IGNOREINDEMO, "cmd %s %s\n", cmds[i].name, params_str(1, -1));
}

// check if players client support params in aliases
qbool isSupport_Params(gedict_t *p)
{
	// seems only ezQuake support 
	return (p->ezquake_version > 0 ? true : false); // have no idea at which point ezquake start support it
}

void StuffAliases( gedict_t *p )
{
	int i;

	for ( i = 1; i <= MAX_CLIENTS; i++ )
		stuffcmd_flags(p, STUFFCMD_IGNOREINDEMO, "alias %d impulse %d\n", i, i);

	if ( p->ct == ctSpec )
	{
		stuffcmd_flags(p, STUFFCMD_IGNOREINDEMO, "alias next_fav fav_next\n");
	}
	else
	{
		stuffcmd_flags(p, STUFFCMD_IGNOREINDEMO, "alias notready break\n");
		stuffcmd_flags(p, STUFFCMD_IGNOREINDEMO, "alias kfjump \"impulse 156;+jump;wait;-jump\"\n");
		stuffcmd_flags(p, STUFFCMD_IGNOREINDEMO, "alias krjump \"impulse 164;+jump;wait;-jump\"\n");
	}
}

// just check is this cmd valid for class of this player
// admin rights skipped here
qbool isValidCmdForClass( int icmd, qbool isSpec )
{
	if ( icmd < 0 || icmd >= cmds_cnt )
		return false;

	if ( k_matchLess && !(cmds[icmd].cf_flags & CF_MATCHLESS) )
		return false; // cmd does't allowed in matchLess mode

	if ( !k_matchLess && (cmds[icmd].cf_flags & CF_MATCHLESS_ONLY) )
		return false; // cmd allowed in matchLess mode _only_

	// split class
	if ( isSpec ) { // spec
		if ( !(cmds[icmd].cf_flags & CF_SPECTATOR) )
			return false; // cmd not for spec
	}
	else { // player
		if ( !(cmds[icmd].cf_flags & CF_PLAYER) )
			return false; // cmd not for player
	}

	return true;
}

// check if this cmd require admin rights
qbool isCmdRequireAdmin( int icmd, qbool isSpec )
{
	if ( icmd < 0 || icmd >= cmds_cnt )
		return false;

	// split class
	if ( isSpec ) { // spec
		if ( cmds[icmd].cf_flags & CF_SPC_ADMIN )
			return true; // cmd require admin rights
	}
	else { // player
		if ( cmds[icmd].cf_flags & CF_PLR_ADMIN )
			return true; // cmd require admin rights
	}

	return false;
}

void cmdslist_dl()
{
	int i, from, to;
	char *name, *params, arg_2[32];
	qbool spc = ( self->ct == ctSpec );
	qbool support_params = isSupport_Params( self );

	// seems we alredy done that
	if ( self->k_stuff & STUFF_COMMANDS )
	{
		G_sprint( self, 2, "cmdslist alredy stuffed\n" );
		return;
	}

	// no arguments
	if ( trap_CmdArgc() == 1 )
	{
		G_sprint( self, 2, "cmdslist without arguments\n" );
		return;
	}

	trap_CmdArgv( 1, arg_2, sizeof( arg_2 ) );

	from = bound( 0, atoi( arg_2 ), cmds_cnt );
	to   = bound( from, from + MAX_STUFFED_ALIASES_PER_FRAME, cmds_cnt );

	// stuff portion of aliases
	for ( i = from; i < to; i++ )
	{
		name = cmds[i].name;
		
		if ( i == 0 )
			G_sprint( self, 2, "Loading commands list...\n" );

		if (
			!isValidCmdForClass( i, spc )   // cmd does't valid for this class of player or matchless mode does't have this command
			|| cmds[i].f == dummy				// cmd have't function, ie u must not stuff alias for this cmd
			|| (cmds[i].cf_flags & CF_NOALIAS) // no alias for such command, may be accessed only via /cmd commandname
			)
		{
			to = min( to + 1, cmds_cnt );
			continue;
		}

		params = ( (cmds[i].cf_flags & CF_PARAMS) && support_params ) ? " %0" : "";

		stuffcmd_flags( self, STUFFCMD_IGNOREINDEMO, "alias %s cmd %03d%s\n", name, (int)i, params );
	}

	if ( i < cmds_cnt )
	{
		// request next stuffing
		stuffcmd_flags( self, STUFFCMD_IGNOREINDEMO, "cmd cmdslist_dl %d\n", i );
		return;
	}

	// we done
	self->k_stuff = self->k_stuff | STUFF_COMMANDS; // add flag
	G_sprint( self, 2, "Commands loaded\n" );
}

void StuffModCommands( gedict_t *p )
{
	// stuff impulses based aliases, or just aliases, not that much...
	StuffAliases( p );

	p->k_stuff = p->k_stuff & ~STUFF_COMMANDS; // remove flag

	stuffcmd_flags( p, STUFFCMD_IGNOREINDEMO, "cmd cmdslist_dl %d\n", 0 );
}

void Init_cmds(void)
{
	int i = 0;

	for( ; i < cmds_cnt; i++ ) {
		if ( strnull( cmds[i].name ) || !( cmds[i].f ) )
			G_Error("Init_cmds: null");

		if ( (int)strlen( cmds[i].name ) > max_cmd_len )
			max_cmd_len = strlen( cmds[i].name );

		if ( cmds[i].cf_flags & CF_PLR_ADMIN )
			cmds[i].cf_flags |= CF_PLAYER;    // this let simplify cmds[] table

		if ( cmds[i].cf_flags & CF_SPC_ADMIN )
			cmds[i].cf_flags |= CF_SPECTATOR; // this let simplify cmds[] table

		if ( cmds[i].cf_flags & CF_MATCHLESS_ONLY )
			cmds[i].cf_flags |= CF_MATCHLESS; // this let simplify cmds[] table
	}
}   

void Do_ShowCmds( qbool adm_req )
{
	qbool first = true;
	int i, l;
	char *name, dots[64];
	char	arg_1[1024];

	trap_CmdArgv( 1, arg_1, sizeof( arg_1 ) );

	for( i = 0; i >= 0 && i < cmds_cnt; i++ )
	{
		name = cmds[i].name;

		if ( strnull(cmds[i].description) || cmds[i].description == CD_NODESC )
			continue; // command does't have description

		if ( !isValidCmdForClass( i, self->ct == ctSpec ) )
			continue; // cmd does't valid for this class of player or matchless mode does't have this command

		if ( adm_req != isCmdRequireAdmin( i, self->ct == ctSpec ) )
			continue; 

		if ( arg_1[0] && !strstr(name, arg_1) )
			continue;

		l = max_cmd_len - strlen(name);
		l = bound( 0, l, sizeof(dots)-1 );
		memset( (void*)dots, (int)'.', l);
		dots[l] = 0;

		if ( first )
		{
			first = false;

			G_sprint(self, 2, "\n%s commands for %s:\n\n", 
				( adm_req ? redtext("admin") : redtext("common") ),
				( self->ct == ctSpec ? redtext("spectator") : redtext("player") ));
		}

		G_sprint(self, 2, "%s%s %s\n", redtext(name), dots, cmds[i].description);
	}
}

void ShowCmds()
{
	Do_ShowCmds( false ); // show common commands
	Do_ShowCmds( true );  // show admin commands
}

qbool check_perm(gedict_t *p, int perm)
{
	switch ( perm ) {
		case 0:
			G_sprint(p, 2, "%s can use this command\n", redtext("no one"));
			return false;
		case 1:
			if ( !is_real_adm( p ) ) {
				G_sprint(p, 2, "you must be a %s\n", redtext("real admin"));
				return false;
			}
			break;
		case 2:
			if ( !is_adm( p ) ) {
				G_sprint(p, 2, "you must be an %s\n", redtext("admin"));
				return false;
			}
			break;
		case 3:
		case 4:
			G_sprint(p, 2, "%s is not implemented in this mode\n", redtext("judges"));
			return false;
		case 5:
			break;
		default:
			G_sprint(p, 2, "server is misconfigured, command %s\n", redtext("skipped"));
			return false;
	}

	return true;
}

void ShowOpts()
{
	G_sprint(self, 2,
		"%s... -1 mins match time\n"
		"%s..... +1 mins match time\n"
		"%s.... -5 mins match time\n"
		"%s...... +5 mins match time\n"
		"%s... -10 fraglimit\n"
		"%s..... +10 fraglimit\n"
		"%s.......... change deathmatch mode\n"
		"%s.......... change teamplay mode\n"
		"%s.... drop quad when killed\n"
		"%s.... drop ring when killed\n"
		"%s... drop pack when killed\n"
		"%s........ change locking mode\n"
		"%s....... change spawntype\n"
		"%s....... toggle sv_maxspeed\n"
		"%s.... quad, %s, ring & suit\n"
		"%s... best/last weapon dropped\n"
		"%s... underwater discharges\n"
		"%s..... toggle spectator talk\n"
		"%s...... toggle midair mode\n"
		"%s... toggle grenade mode\n"
		"%s.... toggle instagib mode\n"
		"%s..... toggle berzerk mode\n",
		redtext("timedown1"), redtext("timeup1"), redtext("timedown"), redtext("timeup"),
		redtext("fragsdown"), redtext("fragsup"), redtext("dm"), redtext("tp"), redtext("dropquad"),
		redtext("dropring"), redtext("droppacks"), redtext("lock"), redtext("spawn"),
		redtext("speed"), redtext("powerups"), redtext("666"), redtext("fairpacks"),
		redtext("discharge"), redtext("silence"), redtext("midair"), redtext("gren_mode"),
		redtext("instagib"), redtext("berzerk"));
}

void ShowQizmo()
{
	G_sprint(self, 2,
		"%s....... lagsettings\n"
		"%s..... enemy vicinity reporting\n"
		"%s..... point function\n", redtext("qlag"), redtext("qenemy"), redtext("qpoint"));
}

/*
// ShowMessages and SendMessage command implementations added
void ShowMessages()
{
	G_sprint(self, 2,
		"%s..... who killed you last\n"
		"%s..... who you last killed\n"
		"%s... last player joined\n, redtext("killer"), redtext("victim"), redtext("newcomer")");
}
*/

/*
// Display all printable characters to console in octal table
*/	
void ShowCharsetTableHexa()
{
	int i;
	G_sprint(self, 2, "\n%s\n\n   0123456789ABCDEF\n   ................\n", redtext("Hexadecimal charset table:"));
	for( i = 16; i < 256; i++) {
		if ( i % 16 == 0) 
			G_sprint(self, 2, "%1X..", i / 16);
		G_sprint(self, 2, "%c", i);
		if ( i % 16 == 15 || i == 255)
			G_sprint(self, 2, "\n");
	}
	G_sprint(self, 2, "\n");
}

/*
// Display all printable characters to console in hexadecimal table
*/	
void ShowCharsetTableOctal()
{
	int i;
	G_sprint(self, 2, "\n%s\n\n    01234567\n    ........\n", redtext("Octal charset table:"));
	for( i = 16; i < 256; i++) {
		if ( i % 8 == 0) 
			G_sprint(self, 2, "%02o..", i / 8);
		G_sprint(self, 2, "%c", i);
		if ( i % 8 == 7 || i == 255)
			G_sprint(self, 2, "\n");
	}
	G_sprint(self, 2, "\n");
}

void ShowVersion()
{
	// limiting to 38 chars for VVD...
	G_sprint(self, 2, "\n\235\236\237 %s \235\236\236\236\236\236\236\236\236"
		"\236\236\236\236\236\236\236\236\236\236\236\236\236\236\236\236\236\236\237\n", redtext("ABOUT"));

	G_sprint(self, 2, "\n\213\212\212\212%s\212\212\212\212\213\n", "QUAKEWORLD SERVER INFORMATION");
	if (strlen(cvar_string("qws_fullname")))
		G_sprint(self, 2, "%s....: %28s\n", redtext("Name"), cvar_string("qws_fullname"));
	else
		G_sprint(self, 2, "%s....: %28s\n", redtext("Name"), cvar_string("version"));
	if (strlen(cvar_string("qws_version")))
		G_sprint(self, 2, "%s.: %28s\n", redtext("Version"), dig3s(cvar_string("qws_version")));
	if (strlen(cvar_string("qws_buildnum")))
		G_sprint(self, 2, "%s...: %26s-%1s\n", redtext("Build"), cvar_string("qws_buildnum"), strlen(cvar_string("qws_platform")) ? cvar_string("qws_platform") : "u");
	if (strlen(cvar_string("qws_builddate")))
		G_sprint(self, 2, "%s....: %28s\n", redtext("Date"), dig3s(cvar_string("qws_builddate")));
	if (strlen(cvar_string("qws_homepage")))
		G_sprint(self, 2, "%s.: %28s\n", redtext("Webpage"), cvar_string("qws_homepage"));

	G_sprint(self, 2, "\n\213\212\212\212\212\212%s\212\212\212\212\212\213\n", "QUAKEWORLD MOD INFORMATION");
	G_sprint(self, 2, "%s....: %28s\n", redtext("Name"), cvar_string("qwm_fullname"));
	G_sprint(self, 2, "%s.: %28s\n", redtext("Version"), dig3s(cvar_string("qwm_version")));
	if (strlen(cvar_string("qwm_buildnum")))
		G_sprint(self, 2, "%s...: %26s-%1s\n", redtext("Build"), cvar_string("qwm_buildnum"), strlen(cvar_string("qwm_platform")) ? cvar_string("qwm_platform") : "u");
	G_sprint(self, 2, "%s....: %28s\n", redtext("Date"), dig3s(cvar_string("qwm_builddate")));
	G_sprint(self, 2, "%s.: %28s\n", redtext("Webpage"), cvar_string("qwm_homepage"));
	
	G_sprint(self, 2, "\n%s\n\n%s\n", MOD_RELEASE_QUOTE, redtext(MOD_RELEASE_HASHTAGS));

	if ( (int)cvar( "sv_specprint" ) & SPECPRINT_SPRINT )
		G_sprint(self, PRINT_CHAT, "\n\207\207\207 WARNING: Spectators may see team\nmessages (mm2) on this server!\n");

	G_sprint(self, 2, "\n\235\236\236\236\236\236\236\236\236\236\236\236\236\236\236\236\236\236\236"
		"\236\236\236\236\236\236\236\236\236\236\236\236\236\236\236\236\236\236\237\n\n");
}

void ChangeOvertime()
{
	int f1, f2;

	if ( match_in_progress )
		return;

    f1 = bound(0, cvar( "k_overtime" ), 3);
    f2 = bound(0, cvar( "k_exttime" ), 999);

    if( !f1 )
    {
		cvar_fset("k_overtime", 1);

		if( !f2 ) 
			cvar_fset("k_exttime", (f2 = 1));

		G_bprint(2, "%s: time based\n", redtext("Overtime"));
		G_bprint(2, "%s: %d minute%s\n", redtext("Overtime length"), (int)f2, count_s(f2));
    }
    else if( f1 == 1 )
	{
		cvar_fset("k_overtime", 2);
		G_bprint(2, "%s: sudden death\n", redtext("Overtime"));
	}
    else if( f1 == 2 )
    {
		cvar_fset("k_overtime", 3);
		G_bprint(2, "%s: tie-break\n", redtext("Overtime"));
    }
    else if( f1 == 3 )
    {
		cvar_fset("k_overtime", 0);
		G_bprint(2, "%s: off\n", redtext("Overtime"));
    }

}

void ChangeOvertimeUp ()
{
	int k_exttime = cvar( "k_exttime" );

	if( match_in_progress )
		return;

	k_exttime++;

	if ( k_exttime >= 11 || k_exttime <= 0 )
		k_exttime = 1;

	cvar_fset( "k_exttime", k_exttime );

	G_bprint(2, "%s %d %s%s\n", 
		redtext("Overtime length set to"), k_exttime, redtext("minute"), redtext(count_s( k_exttime )));
}


void SendKillerMsg()
{
	SendMessage(self->killer);
}

void SendVictimMsg()
{
	SendMessage(self->victim);
}

void SendNewcomerMsg()
{
	SendMessage(newcomer->netname);
}

void SendMessage(char *name)
{
	char *s;
	gedict_t *p;

	for( p = world; (p = find_client( p )); ) {
		if ( p == self )
			continue;

		if ( !strnull( name ) && streq( p->netname, name ) ) {
			stuffcmd_flags(self, STUFFCMD_IGNOREINDEMO, "say ");
			if ((s = ezinfokey(self, "premsg")))
				stuffcmd_flags(self, STUFFCMD_IGNOREINDEMO, " %s ", s);
			stuffcmd_flags(self, STUFFCMD_IGNOREINDEMO, "%s", name);
			if ((s = ezinfokey(self, "postmsg")))
				stuffcmd_flags(self, STUFFCMD_IGNOREINDEMO, " %s", s);
			stuffcmd_flags(self, STUFFCMD_IGNOREINDEMO, "\n");

			return;
		}
	}

	G_sprint(self, 2, "No name to display\n");
}

static char *get_frp_str ()
{
	switch( get_fair_pack() ) {
		case  0: return "off";
		case  1: return "on";
		case  2: return "lst";
		default: return "unk";
	}
}

void ModStatus ()
{
	int votes;
	gedict_t *p;

	G_sprint(self, 2, "%-14.14s %-4d\n",   redtext("Maxspeed"),			(int)k_maxspeed);
	G_sprint(self, 2, "%-14.14s %-4d ",    redtext("Deathmatch"),		(int)deathmatch);
	G_sprint(self, 2, "%-14.14s %-3d\n",   redtext("Teamplay"),			(int)tp_num());
	G_sprint(self, 2, "%-14.14s %-4d ",    redtext("Timelimit"),		(int)timelimit);
	G_sprint(self, 2, "%-14.14s %-3d\n",   redtext("Fraglimit"),		(int)fraglimit);
	G_sprint(self, 2, "%-14.14s %-4.4s ",  redtext("Powerups"),			Get_PowerupsStr());
	G_sprint(self, 2, "%-14.14s %-3.3s\n", redtext("Discharge"),		OnOff(cvar("k_dis")));
	G_sprint(self, 2, "%-14.14s %-4.4s ",  redtext("Drop Quad"),		OnOff(cvar("dq")));
	G_sprint(self, 2, "%-14.14s %-3.3s\n", redtext("Drop Ring"),		OnOff(cvar("dr")));
	G_sprint(self, 2, "%-14.14s %-4.4s ",  redtext("Fair Backpacks"),	get_frp_str());
	G_sprint(self, 2, "%-14.14s %-3.3s\n", redtext("Drop Backpacks"),	OnOff(cvar("dp")));
	G_sprint(self, 2, "%-14.14s %-4.4s ",  redtext("spec info perm"),	mi_adm_only() ? "adm" : "all");
	G_sprint(self, 2, "%-14.14s %-3.3s\n", redtext("more spec info"),	OnOff(mi_on()));
	G_sprint(self, 2, "%-14.14s %-4.4s\n", redtext("teleteam"),			OnOff(cvar("k_tp_tele_death")));
	G_sprint(self, 2, "%-14.14s %-3.3s\n", redtext("Berzerk"),		    OnOff(cvar("k_bzk")));

	if( match_in_progress == 1 )
	{
		p = find(world, FOFCLSN, "timer" );
		if ( p )
			G_sprint(self, 2, "The match will start in %d second%s\n", (int)p->cnt2, count_s(p->cnt2));
		return;
	}

	if( (votes = get_votes( OV_ELECT )) )
		G_sprint(self, 2, "%s election in progress:\220%d/%d\221 vote%s\n",
			redtext(get_elect_type_str()), votes,
			get_votes_req( OV_ELECT, false ), count_s(votes));

	if( k_captains == 2 )
		G_sprint(self, 2, "%s in progress\n", redtext("Team picking"));

	if( floor( k_captains ) == 1 ) 
		G_sprint(self, 2, "\2201\221 %s present\n", redtext("captain"));

	if( floor( k_coaches ) == 1 )
	    G_sprint(self, 2, "\2201\221 %s present\n", redtext("coach"));

	if( match_in_progress == 2 )
	{
		if( k_sudden_death )
		{
			G_sprint(self, 2, "%s overtime in progress\n", redtext(SD_type_str()));
		}
		else
		{
			p = find(world, FOFCLSN, "timer");
			if ( p )
			{
				G_sprint(self, 2, "Match in progress\n"
					"\220%s\221 full minute%s left\n", dig3(p->cnt - 1), count_s(p->cnt));
			}
		}
	}
}

void ModStatus2()
{
	int i;
	char *ot = "";

	G_sprint(self, 2, "%s\n", redtext( respawn_model_name( cvar( "k_spw" ) ) ));

	if( isDuel() )
		G_sprint(self, 2, "%s: duel\n", redtext("Server mode"));
	else if ( isFFA() )
		G_sprint(self, 2, "%s:  FFA\n", redtext("Server mode"));
	else if ( isCTF() ) {
		G_sprint(self, 2, "%s:  CTF\n", redtext("Server mode"));
		G_sprint(self, 2, "%s: %s\n", redtext("Server locking"),
			(!cvar("k_lockmode") ? "off" : (cvar("k_lockmode") == 2 ? "all" : (cvar("k_lockmode") == 1 ? "team" : "unknown"))));
		G_sprint(self, 2, "%s: hook %s, runes %s, ga %s\n", redtext("CTF settings"),
			OnOff(cvar("k_ctf_hook")), OnOff(cvar("k_ctf_runes")), OnOff(cvar("k_ctf_ga")));
	}
	else if ( isTeam() ) {
		G_sprint(self, 2, "%s: team\n", redtext("Server mode"));
		G_sprint(self, 2, "%s: %s\n", redtext("Server locking"),
					(!cvar("k_lockmode") ? "off" : (cvar("k_lockmode") == 2 ? "all" : (cvar("k_lockmode") == 1 ? "team" : "unknown"))));
	}
	else
		G_sprint(self, 2, "%s: unknown\n", redtext("Server mode"));

	if( !match_in_progress )
		G_sprint(self, 2, "%s (%s: %d %s: %d %s: %d)\n", 
					redtext("Teaminfo"), redtext("cur"), (int)CountRTeams(),
					redtext("min"), (int)cvar("k_lockmin"),
					redtext("max"), (int)cvar("k_lockmax"));

	G_sprint(self, 2, "%s: %s\n", redtext("Spectalk"), OnOff( cvar("k_spectalk") ));

	i = cvar( "k_exttime" );
	switch ( (int)cvar( "k_overtime" ) ) {
		case 0:  ot = "off"; break;
		case 1:  ot = va("%d minute%s", i, count_s(i)); break;
		case 2:  ot = "sudden death"; break;
		case 3:  ot = va("%d tie-break", tiecount()); break;
		default: ot	= "unknown"; break;
	}
	G_sprint(self, 2, "%s: %s\n", redtext("Overtime"), ot);

	i = iKey( world, "fpd" );

	G_sprint(self, 2, "%s: %s\n", redtext("QiZmo lag"), OnOff(i & 8));
	G_sprint(self, 2, "%s: %s\n", redtext("QiZmo timers"), OnOff(i & 2));
	G_sprint(self, 2, "%s: %s\n", redtext("QiZmo enemy reporting"), OnOff(i & 32));
	G_sprint(self, 2, "%s: %s\n", redtext("QiZmo pointing"), OnOff(i & 128));
/* new FDP bits https://www.quakeworld.nu/wiki/FPD
	G_sprint(self, 2, "%s: %s\n", redtext("Skin forcing"),          OnOff(! (i & 256) ));
	G_sprint(self, 2, "%s: %s\n", redtext("Color forcing"),         OnOff(! (i & 512) ));
	G_sprint(self, 2, "%s: %s\n", redtext("Pitch speed limiting"),  OnOff( i & 16384 ));
	G_sprint(self, 2, "%s: %s\n", redtext("Yaw speed limiting"),    OnOff( i & 32768 ));
*/

	G_sprint(self, 2, "%s: %s\n", redtext("Admin election"),
		Allowed(cvar( "k_allowvoteadmin" )));

	G_sprint(self, 2, "%s: %s\n", redtext("Check frametimes"), Enabled( framechecks ));

	switch ( (int)cvar("k_prewar") ) {
		case  1: ot = "players may fire before match"; break;
		case  2: ot = "players may fire and jump when ready"; break;
		case  0:
		default: ot = "players may not fire before match"; break;
	}
	G_sprint(self, 2, "%s: %s\n", redtext("Prewar"), ot);

	if ( k_sv_locktime ) {
		int seconds = k_sv_locktime - g_globalvars.time;
		G_sprint(self, 2, "%s: %d second%s\n",
			redtext("server is temporary locked"), seconds, count_s(seconds));
	}

	if ( k_cmd_fp_disabled ) 
		G_sprint(self, 2, "%s: off\n", redtext("Command floodprot"));
	else {
		G_sprint(self, 2, "%s: %d commands allowed per %d sec.,"
			" skip commands for %d sec., ", redtext("Command floodprot"), 
			k_cmd_fp_count, (int)k_cmd_fp_per, (int) k_cmd_fp_for);

		if ( k_cmd_fp_dontkick ) 
			G_sprint(self, 2, "cmdfp kick disabled\n");
		else
			G_sprint(self, 2, "kick after %d warn.\n", k_cmd_fp_kick);
	}
}

void ModStatusVote()
{
	qbool voted = false;
	int votes, i;
	gedict_t *p;

	if ( k_matchLess || !match_in_progress )
	if ( vote_get_maps () >= 0 ) {
		voted = true;

		G_sprint(self, 2, "%s:\n", redtext("Map voting"));
		
		for( i = 0; i < MAX_CLIENTS; i++)
		{
			if (!maps_voted[i].map_id)
				break;

			G_sprint(self, 2, "\220%s\221 %2d vote%s\n", GetMapName( maps_voted[i].map_id ),
				maps_voted[i].map_votes, count_s(maps_voted[i].map_votes) );

			for( p = world; (p = find_client( p )); )
				if ( p->v.map == maps_voted[i].map_id )
					G_sprint(self, 2, " %s\n", p->netname);
		}
	}

	if( !(get_elect_type() == etCaptain && match_in_progress) ) // doesn't show captain election in game
	if( (votes = get_votes( OV_ELECT )) ) {
		voted = true;

		G_sprint(self, 2, "\220%d/%d\221 vote%s for %s election:\n", votes, 
			get_votes_req( OV_ELECT, false ), count_s(votes), redtext(get_elect_type_str()) );

		for( p = world; (p = find_client( p )); )
			if ( p->v.elect )
				G_sprint(self, 2, "%s%s\n", 
				(p->v.elect_type != etNone) ? "\207" : " ", p->netname);
	}

	if ( !match_in_progress )
	if ( (votes = get_votes( OV_PICKUP ))) {
		voted = true;

		G_sprint(self, 2, "\220%d/%d\221 vote%s for a %s game:\n", votes,
			get_votes_req( OV_PICKUP, false ), count_s(votes), redtext("pickup"));

		for( p = world; (p = find_client( p )); )
			if ( p->v.pickup )
				G_sprint(self, 2, " %s\n", p->netname);
	}

	if ( !match_in_progress )
	if ( (votes = get_votes( OV_RPICKUP ))) {
		voted = true;

		G_sprint(self, 2, "\220%d/%d\221 vote%s for a %s game:\n", votes,
			get_votes_req( OV_RPICKUP, false ), count_s(votes), redtext("rpickup"));

		for( p = world; (p = find_client( p )); )
			if ( p->v.rpickup )
				G_sprint(self, 2, " %s\n", p->netname);
	}

	if( k_matchLess || match_in_progress == 2 )
	if ( (votes = get_votes( OV_BREAK )) ) {
		voted = true;

		G_sprint(self, 2, "\220%d/%d\221 vote%s for %s:\n", votes,
			get_votes_req( OV_BREAK, false ), count_s(votes), (k_matchLess ? "next map" : "stopping"));

		for( p = world; (p = find_client( p )); )
			if ( p->v.brk )
				G_sprint(self, 2, " %s\n", p->netname);
	}

	if ( !match_in_progress )
	if ( (votes = get_votes( OV_ANTILAG ))) {
		voted = true;

		G_sprint(self, 2, "\220%d/%d\221 vote%s for a %s mode change:\n", votes,
			get_votes_req( OV_ANTILAG, false ), count_s(votes), redtext("antilag"));

		for( p = world; (p = find_client( p )); )
			if ( p->v.antilag )
				G_sprint(self, 2, " %s\n", p->netname);
	}

	if ( !match_in_progress )
	if ( (votes = get_votes( OV_NOSPECS ))) {
		voted = true;

		G_sprint(self, 2, "\220%d/%d\221 vote%s for a %s mode change:\n", votes,
			get_votes_req( OV_NOSPECS, false ), count_s(votes), redtext("no spec"));

		for( p = world; (p = find_client( p )); )
			if ( p->v.nospecs )
				G_sprint(self, 2, " %s\n", p->netname);
	}

	if ( !match_in_progress )
	if ( (votes = get_votes( OV_TEAMOVERLAY ))) {
		voted = true;

		G_sprint(self, 2, "\220%d/%d\221 vote%s for a %s change:\n", votes,
			get_votes_req( OV_TEAMOVERLAY, false ), count_s(votes), redtext("teamoverlay"));

		for( p = world; (p = find_client( p )); )
			if ( p->v.teamoverlay )
				G_sprint(self, 2, " %s\n", p->netname);
	}

	if (!match_in_progress) {
		if ((votes = get_votes(OV_PRIVATE))) {
			qbool enable = !cvar("sv_login");

			voted = true;
			
			G_sprint(self, 2, "\220%d/%d\221 vote%s for a %s:\n", votes,
				get_votes_req(OV_PRIVATE, false), count_s(votes), enable ? redtext("private game") : redtext("public game"));

			for (p = world; (p = find_client(p)); ) {
				if (p->v.privategame) {
					G_sprint(self, 2, " %s\n", p->netname);
				}
			}
		}
	}

	if ( voted )
		G_sprint(self, 2, "%s\n", redtext("--------------"));
	else
		G_sprint(self, 2, "%s\n", redtext("No election going on"));
}

char *OnePlayerStatus( gedict_t *p, gedict_t *e_self )
{
	char *team_str = (isTeam() ? va(" \220%4.4s\221", getteam( p )) : "");

	e_self = (e_self ? e_self : world);

	return va( "%s%s%s %s%s",
		( p->ready ? "\206" : "\207" ),	( is_adm( p ) ? "\xC1" : " " ),
		team_str, getname( p ), ( p == e_self ? redtext( " \x8D you" ) : "" ) );
}

void PlayerStatus()
{
	qbool found = false;
	gedict_t *p;

	if( match_in_progress ) {
		G_sprint(self, 2, "Game in progress\n");
		return;
	}
	
	for ( p = world; (p = find_plr( p )); ) {
		if ( !found )
			G_sprint(self, 2, "Players list:\n"
				"\235\236\236\236\236\236\236\236\236\236\236\236\236\236\236\236\236\236\236\236\236\236\237\n");
		G_sprint(self, 2, "%s\n", OnePlayerStatus( p, self ));
		found = true;
	}
			
	G_sprint(self, 2, "%s\n", (found ? "" : "no players"));
}

void PlayerStatusS()
{
	qbool found = false;
	gedict_t *p;

	for ( p = world; (p = find_plr( p )); ) {
		if ( !found )
			G_sprint(self, 2, "Players skins list:\n"
				"\235\236\236\236\236\236\236\236\236\236\236\236\236\236\236\236\236\236\236\236\236\236\237\n");
		G_sprint(self, 2, "\220%10s\221 %s\n", ezinfokey(p, "skin"), p->netname);
		found = true;
	}
			
	G_sprint(self, 2, "%s\n", (found ? "" : "no players"));
}

void PlayerStatusN()
{
	qbool found = false;
	gedict_t *p;

	if( match_in_progress ) {
		G_sprint(self, 2, "Game in progress\n");
		return;
	}

	if( CountRPlayers() == CountPlayers() ) {
		G_sprint(self, 2, "All players ready\n");
		return;
	}

	for ( p = world; (p = find_plr( p )); ) {
		if ( p->ready )
			continue;

		if ( !found )
			G_sprint(self, 2, "Players %s ready:\n"
				"\235\236\236\236\236\236\236\236\236\236\236\236\236\236\236\236\236\236\236\236\236\236\237\n",
				redtext("not"));

		G_sprint(self, 2, "%s\n", OnePlayerStatus( p, self ));
		found = true;
	}
			
	G_sprint(self, 2, "%s\n", (found ? "" : "can't find not ready players"));
}

// broadcast not ready players

void ListWhoNot()
{
	qbool found = false;
	gedict_t *p, *p2;

	if( match_in_progress ) {
		G_sprint(self, 2, "Game in progress\n");
		return;
	}

	if( CountRPlayers() == CountPlayers() ) {
		G_sprint(self, 2, "All players ready\n");
		return;
	}

	if( self->ct == ctPlayer && !self->ready ) 
	{
		G_sprint(self, 2, "Ready yourself first\n");
		return;
	}	

	if( k_whonottime && g_globalvars.time < k_whonottime + 10 )
	{
        G_sprint(self, 2, "Only one %s in 10 seconds\n", redtext("list"));
		return;
	}

	k_whonottime = g_globalvars.time;

	for ( p = world; (p = find_plr( p )); ) {
		if ( p->ready )
			continue;

		if ( !found )
			G_bprint(2, "Players %s ready:\n"
				"\235\236\236\236\236\236\236\236\236\236\236\236\236\236\236\236\236\236\236\236\236\236\237\n",
				redtext("not")); // broadcast

		for ( p2 = world; (p2 = find_client( p2 )); )
			G_sprint(p2, 2, "%s\n", OnePlayerStatus( p, p2 ));

		found = true;
	}

	if ( found )			
		G_bprint(2, "\n"); // broadcast
	else
		G_sprint(self, 2, "can't find not ready players\n"); // self
}

void VotePickup()
{
	int votes;

	if( match_in_progress )
		return;

	if( k_captains ) {
		G_sprint(self, 2, "No pickup when captain stuffing\n");
		return;
	}

	self->v.pickup = !self->v.pickup;

	G_bprint(2, "%s %s %s%s\n", self->netname, 
		redtext("says"), (self->v.pickup ? "pickup!" : "no pickup"),
		((votes = get_votes_req( OV_PICKUP, true )) ? va(" (%d)", votes) : ""));

	vote_check_pickup ();
}

void ReportMe()
{
	gedict_t *p;
	char *t1, *t2 , *wt, *pa1, *pa2;
	float f1, flag = 0;

	if( !strnull( ezinfokey(self, "k_nick") ) || !strnull( ezinfokey(self, "k") ) )
		flag = 1;

	pa1 = "";
	pa2 = ": ";

	wt = "axe:";
	f1 = 0;

	if( (int)self->s.v.items & 1 ) {
		wt = "sg:";
		f1 = self->s.v.ammo_shells;
	}
	if( (int)self->s.v.items & 4) {
		wt = "ng:";
		f1 = self->s.v.ammo_nails;
	}
	if( (int)self->s.v.items & 2) {
		wt = "ssg:";
		f1 = self->s.v.ammo_shells;
	}
	if( (int)self->s.v.items & 8) {
		wt = "sng:";
		f1 = self->s.v.ammo_nails;
	}
	if( (int)self->s.v.items & 16) {
		wt = "gl:";
		f1 = self->s.v.ammo_rockets;
	}
	if( (int)self->s.v.items & 64) {
		wt = "lg:";
		f1 = self->s.v.ammo_cells;
	}
	if( (int)self->s.v.items & 32) {
		wt = "rl:";
		f1 = self->s.v.ammo_rockets;
	}

	t1 = getteam( self );

	for( p = world; (p = find_plr( p )); ) {
		if( strneq( t1, t2 = getteam( p ) ) )
			continue;

		if( flag ) {
			t1 = ezinfokey(self, "k_nick");

			if ( strnull( t1 ) )
				t1 = ezinfokey(self, "k");

			G_sprint(p, 3, "%s: ", t1);
		}
		else
			G_sprint(p, 3, "%s%s%s", pa1, self->netname, pa2);

		if( self->s.v.armorvalue )
			G_sprint(p, 3, "%s:%d", armor_type((int)self->s.v.items),
				(int)self->s.v.armorvalue);
		else
			G_sprint(p, 3, "a:0");

		G_sprint(p, 3, "  h:%d  %s%d", (int)self->s.v.health, wt, (int)f1);

		if( (int)self->s.v.items & 524288)
			G_sprint(p, 3, "  \220\212%s\212\221", redtext("eyes"));
		if( (int)self->s.v.items & 1048576)
			G_sprint(p, 3, "  \220\212%s\212\221", redtext("666"));
		if( (int)self->s.v.items & 4194304)
			G_sprint(p, 3, "  \220\212%s\212\221", redtext("quad"));

		G_sprint(p, 3, "\n");
	}
}

void ToggleRespawns()
{
	int k_spw = bound(0, cvar( "k_spw" ), 4);

	if ( match_in_progress )
		return;

	if ( ++k_spw > 4 )
		k_spw = 0;

	cvar_fset( "k_spw", k_spw );

	G_bprint(2, "%s\n", respawn_model_name( k_spw ));
}

void TogglePowerups()
{
	char arg[64];
	int i;
	qbool changed = false;

	if ( match_in_progress )
		return;

	if ( cvar("k_instagib") )
	{
		G_bprint(2, "%s are disabled with Instagib\n", redtext("Powerups"));
		return;
	}

	if ( cvar("k_midair") )
	{
		G_bprint(2, "%s are disabled with Midair\n", redtext("Powerups"));
		return;
	}

	if ( trap_CmdArgc() <= 1 )
	{	// no arguments, just toggle on/off powerups
		cvar_toggle_msg( self, "k_pow", redtext("powerups") );
		cvar_fset("k_pow_q", cvar("k_pow"));
		cvar_fset("k_pow_p", cvar("k_pow"));
		cvar_fset("k_pow_r", cvar("k_pow"));
		cvar_fset("k_pow_s", cvar("k_pow"));
		return;
	}

	// at least one argument
	for ( i = 1; i < min(1 + 4, trap_CmdArgc()); i++ )
	{
		trap_CmdArgv( i, arg, sizeof( arg ) );
		if ( streq("q", arg) )
		{
			cvar_toggle_msg( self, "k_pow_q", redtext("quad") );
			changed = true;
		}
		else if ( streq("p", arg) )
		{
			cvar_toggle_msg( self, "k_pow_p", redtext("pent") );
			changed = true;
		}
		else if ( streq("r", arg) )
		{
			cvar_toggle_msg( self, "k_pow_r", redtext("ring") );
			changed = true;
		}
		else if ( streq("s", arg) )
		{
			cvar_toggle_msg( self, "k_pow_s", redtext("suit") );
			changed = true;
		}
	}

	// enable k_pow if at least one powerup was turned on and vice versa turn off k_pow if all powerups was turned off
	if ( changed )
		cvar_fset( "k_pow", ( cvar("k_pow_q") || cvar("k_pow_p") || cvar("k_pow_r") || cvar("k_pow_s") ) );
}

void TogglePuPickup()
{
	if( match_in_progress )
		return;

	cvar_toggle_msg( self, "k_pow_pickup", redtext("new powerups pickup (no multi pickup)") );
}

void ToggleDischarge()
{
	if( match_in_progress )
		return;

	cvar_toggle_msg( self, "k_dis", redtext("discharges") );
}

void ShowDMM()
{
	G_sprint(self, 2, "Deathmatch %s\n", dig3(deathmatch));
}

void ChangeDM(float dmm)
{
	if ( !is_rules_change_allowed() )
		return;

	if ( deathmatch == (int)dmm ) {
		G_sprint(self, 2, "%s%s already set\n", redtext("dmm"), dig3(deathmatch));
		return;
	}

	deathmatch = bound(1, (int)dmm, 5);

	cvar_set("deathmatch", va("%d", (int)deathmatch));

	if ( dmm != 4 )				// if leaving dmm4
	{		
		cvar_set( "k_midair", "0" );	// force midair off
		cvar_set( "k_instagib", "0" );	// force instagib off
	}					
	else				// if entering dmm4
	{
		cvar_set( "timelimit", "3");			// Set match length to 3 minutes
	}

	G_bprint(2, "Deathmatch %s\n", dig3(deathmatch));
}

void ChangeTP()
{
	if ( match_in_progress )
		return;

	if( !isTeam() && !isCTF() ) {
		G_sprint(self, 3, "console: non team mode disallows you to change teamplay setting\n");
		return;
	}

	teamplay = bound (1, teamplay, 4);

	teamplay++;

	if ( teamplay == 5 )
		teamplay = 1;

	cvar_fset("teamplay", (int)teamplay);

	G_bprint(2, "Teamplay %s\n", dig3(teamplay));
}

void TimeDown(float t)
{
	int tl = timelimit;

	if (match_in_progress) {
		return;
	}

	if (t == 5 && isHoonyModeAny()) {
		t = 2;
	}

	if (t == 5 && timelimit == 5) {
		timelimit = 3;
	}
	else if (t == 5 && timelimit == 3) {
		timelimit = 1;
	}
	else {
		timelimit -= t;
	}

	timelimit = bound(0, timelimit, cvar( "k_timetop" ));

	if ( timelimit <= 0 && fraglimit <= 0 ) {
		G_sprint(self, 2, "You need some timelimit or fraglimit at least\n");
		timelimit = tl;
	}

	if ( tl == timelimit ) {
		G_sprint(self, 2, "%s still %s\n", redtext("timelimit"), dig3(timelimit));
		return;
	}

	cvar_set("timelimit", va("%d", (int)timelimit));
	G_bprint(2, "%s %s %s%s\n", redtext("Match length set to"), dig3(timelimit), redtext("minute"), redtext(count_s(timelimit)));
}

void TimeUp(float t)
{
	int tl = timelimit;

	if ( match_in_progress )
		return;

	if (t == 5 && timelimit == 0)
		timelimit = 1;
	else if (t == 5 && timelimit == 1)
		timelimit = 3;
	else if (t == 5 && timelimit == 3)
		timelimit = 5;
	else
		timelimit += t;

	timelimit = bound(0, timelimit, cvar( "k_timetop" ));

	if ( tl == timelimit ) {
		G_sprint(self, 2, "%s still %s\n", redtext("timelimit"), dig3(timelimit));
		return;
	}

	cvar_fset( "timelimit", (int)timelimit );
	G_bprint(2, "%s %s %s%s\n", redtext("Match length set to"), dig3(timelimit), redtext("minute"), redtext(count_s(timelimit)));
}

void TimeSet(float t)
{
	int tl = timelimit;

	if ( match_in_progress )
		return;

	timelimit = bound(0, t, cvar( "k_timetop" ));

	if ( tl == timelimit ) {
		G_sprint(self, 2, "%s still %s\n", redtext("timelimit"), dig3(timelimit));
		return;
	}

	cvar_fset( "timelimit", (int)timelimit );
	G_bprint(2, "%s %s %s%s\n", redtext("Match length set to"), dig3(timelimit), redtext("minute"), redtext(count_s(timelimit)));
}

void AdjustFragLimit(int delta)
{
	fraglimit += delta * (isHoonyModeAny() ? 2 : 10);

	fraglimit = bound(isHoonyModeAny() ? 0 : 1, fraglimit, isHoonyModeDuel() ? 20 : 100);
}

void FragsDown()
{
	if (match_in_progress) {
		return;
	}
	else if (isHoonyModeAny()) {
		G_sprint(self, PRINT_HIGH, "No fraglimit in hoonymode\n");
		return;
	}
	else {
		int fl = fraglimit;

		if (fraglimit == 1) {
			// allow fraglimit "1" (instead of going from 10 directly to 0) as a type of minimal hoonymode
			fraglimit = 0;
		}
		else if (fraglimit == 0) {
			// avoid cycling between 0 and 1 (this happens due to below shortcut using bound())
			fraglimit = 0;
		}
		else {
			AdjustFragLimit(-1);
		}

		if (timelimit <= 0 && fraglimit <= 0) {
			G_sprint(self, 2, "You need some timelimit or fraglimit at least\n");
			fraglimit = fl;
		}

		if (fl == fraglimit) {
			G_sprint(self, 2, "%s still %s\n", redtext("fraglimit"), dig3(fraglimit));
			return;
		}

		cvar_set("fraglimit", va("%d", (int)(fraglimit)));
		G_bprint(2, "%s %s\n", redtext("Fraglimit set to"), dig3(fraglimit));
	}
}

void FragsUp()
{
	if (match_in_progress) {
		return;
	}
	else if (isHoonyModeAny()) {
		G_sprint(self, PRINT_HIGH, "No fraglimit in hoonymode\n");
	}
	else {
		int fl = fraglimit;

		AdjustFragLimit(1);

		if (fl == fraglimit) {
			G_sprint(self, 2, "%s still %s\n", redtext("fraglimit"), dig3(fraglimit));
			return;
		}

		cvar_set("fraglimit", va("%d", (int)(fraglimit)));
		G_bprint(2, "%s %s\n", redtext("Fraglimit set to"), dig3(fraglimit));
	}
}

void killquad()
{
	if ( match_in_progress )
		return;

	cvar_toggle_msg( self, "k_killquad", redtext("KillQuad") );
	k_killquad = cvar( "k_killquad" );
}

void bloodfest()
{
	if ( match_in_progress )
		return;

	cvar_toggle_msg( self, "k_bloodfest", redtext("Blood Fest mode (for coop/single only)") );
	k_bloodfest = cvar( "k_bloodfest" );
}

void ToggleDropQuad()
{
	if ( match_in_progress )
		return;

	cvar_toggle_msg( self, "dq", redtext("DropQuad") );
}

void ToggleDropRing()
{
	if ( match_in_progress )
		return;

	cvar_toggle_msg( self, "dr", redtext("DropRing") );
}

void ToggleDropPack()
{
	if ( match_in_progress )
		return;

	cvar_toggle_msg( self, "dp", redtext("DropPacks") );
}

void ToggleFairPacks()
{
	int k_frp = bound(0, cvar("k_frp"), 2);

    if ( match_in_progress )
		return;

	if( k_yawnmode ) {
		k_frp = get_fair_pack(); // Yawnmode: hardcoded to 2
	}
	else {

		if ( ++k_frp > 2 )
			k_frp = 0;

		cvar_fset( "k_frp", k_frp );
	}

	if( !k_frp )
		G_bprint(2, "%s disabled\n", redtext("Fairpacks"));
	else if( k_frp == 1 )
		G_bprint(2, "%s enabled - drop best weapon\n", redtext("Fairpacks"));
	else if( k_frp == 2 )
		G_bprint(2, "%s enabled - drop last fired weapon\n", redtext("Fairpacks"));
	else
		G_bprint(2, "%s - unknown\n", redtext("Fairpacks"));
}

void ToggleSpeed()
{
	gedict_t *p;

	if ( match_in_progress )
		return;

	if( k_maxspeed != 320 )
		k_maxspeed = 320;
	else
		k_maxspeed = bound(0, cvar( "k_highspeed" ), 9999);

	G_bprint(2, "%s %d\n", redtext("Maxspeed set to"), (int)k_maxspeed);
	cvar_fset("sv_maxspeed", k_maxspeed);

	for( p = world; (p = find_plr( p )); )
		p->maxspeed = k_maxspeed;
}

void ToggleBerzerk()
{
	if (match_in_progress)
		return;

	cvar_toggle_msg( self, "k_bzk", redtext("Berzerk mode") );
};

void ToggleSpecTalk()
{
	int k_spectalk = !cvar( "k_spectalk" ), fpd = iKey( world, "fpd" );

	if ( match_in_progress && !is_adm( self ) )
		return;

	k_spectalk = bound(0, k_spectalk, 1);

	if( match_in_progress == 2 )
	{

		fpd = ( k_spectalk ) ? (fpd & ~64) : (fpd | 64);

		localcmd("serverinfo fpd %d\n", fpd );
		cvar_fset( "sv_spectalk", k_spectalk );
		cvar_fset(  "k_spectalk", k_spectalk );

		if( k_spectalk )
			G_bprint(2, "Spectalk on: %s\n", redtext("players can now hear spectators"));
		else
			G_bprint(2, "Spectalk off: %s\n", redtext("players can no longer hear spectators"));

		return;

	}
	else
	{
		cvar_fset( "k_spectalk", k_spectalk );

		if( k_spectalk )
			G_bprint(2, "Spectalk on: %s\n", redtext("players can hear spectators during game"));
		else
			G_bprint(2, "Spectalk off: %s\n", redtext("players cannot hear spectators during game"));
	}
}

void ShowRules()
{
	if( isDuel() )
		G_sprint(self, 2, "Server is in duel mode.\n");
	else if ( isCTF() )
		G_sprint(self, 2, "Server is in CTF mode.\n"
			"Additional commands/impulses:\n"
			"impulse 22 : Grappling Hook\n"
			"tossrune   : Toss your current rune\n"
			"tossflag   : Toss carried flag\n"
			"flagstatus : Displays flag information\n");
	else if ( isFFA() )
		G_sprint(self, 2, "Server is in FFA mode.\n");
	else if ( isTeam() )
		G_sprint(self, 2,
			"Server is in team mode.\n"
			"Typing %s during game\n"
			"will print time left and teamscores.\n"
			"Also available during game\n"
			"are %s and %s.\n", redtext("scores"), redtext("stats"), redtext("efficiency"));
	else
		G_sprint(self, 2, "Server is in unknown mode.\n");

	if( cvar( "k_bzk" ) ) {
		G_sprint(self, 2,
			"\nBERZERK mode is activated!\n"
			"This means that when only %d seconds\n"
			"remains of the game, all players\n"
			"gets QUAD/OCTA powered.\n", (int)cvar( "k_btime" ));
	}

	G_sprint(self, 2, "\n");
}

void ChangeLock()
{
	int lock = bound(0, cvar("k_lockmode"), 2);

	if ( match_in_progress )
		return;

	lock++;

    if( lock > 2 )
		lock = 0;
	if( lock == 0 )
		G_bprint(2, "%s off\n", redtext("Server locking"));
	else if( lock == 2 )
		G_bprint(2, "%s - players cannot connect during game\n", redtext("Server locked"));
	else if( lock == 1 )
		G_bprint(2, "%s - only players in existing teams can connect during game\n", redtext("Teamlock on"));

	cvar_fset("k_lockmode", lock);
}

void TeamSay(float fsndname)
{
	gedict_t *p;
	char *sndname = va("ktsound%d.wav", (int)fsndname);

	for( p = world; (p = find_plr(p)); ) {
		if (
			p != self && (isTeam() || isCTF()) && !strnull( p->netname )
			&& ( iKey( p, "kf" ) & KF_KTSOUNDS ) 
			) 
		{
			if ( streq( getteam( self ), getteam( p ) ) ) {
				char *t1 = ezinfokey(p, "k_sdir");
				stuffcmd(p, "play %s%s\n", (strnull( t1 ) ? "" : va("%s/", t1)), sndname);
			}
		}
	}
}

void PrintScores()
{
	int minutes, seconds;
	gedict_t *p;

	if ( intermission_running ) {
		G_sprint(self, 2, "Intermission\n");
		return;
	}

	if( !match_in_progress ) {
		G_sprint(self, 2, "no game - no scores\n");
		return;
	}

	if( match_in_progress == 1 ) {
		G_sprint(self, 2, "Countdown\n");
		return;
	}

	if( k_sudden_death ) {
		G_sprint(self, 2, "%s %s\n", SD_type_str(), redtext("overtime in progress"));
	}
	else {
		if ( fraglimit && (p = get_ed_scores1()) ) {
			int diff = fraglimit - p->s.v.frags;

			if ( diff >= 0 )
				G_sprint(self, 2, "Frags left: \220%s\221\n", dig3s("%2d", diff));
		}
	}

	if( (p = find(world, FOFCLSN, "timer")) ) {
		minutes = p->cnt;
		seconds = p->cnt2;
		if( seconds == 60 )
			seconds = 0;
		else
			minutes--;

		// we can't use dig3 here because of zero padding, so using dig3s
		G_sprint(self, 2, "\220%s:%s\221 remaining\n", 
			dig3s("%02d", minutes), dig3s("%02d", seconds));
	}

	if( k_showscores )
	{
		if ( isCA() )
		{
			CA_PrintScores();
		}
		else
		{
			int s1 = get_scores1();
			int s2 = get_scores2();
			char *t1 = cvar_string( "_k_team1" );
			char *t2 = cvar_string( "_k_team2" );

			G_sprint(self, 2, "%s \220%s\221 = %s\n", redtext("Team"),
				(s1 > s2 ? t1 : t2), dig3(s1 > s2 ? s1 : s2));
			G_sprint(self, 2, "%s \220%s\221 = %s\n", redtext("Team"),
				(s1 > s2 ? t2 : t1), dig3(s1 > s2 ? s2 : s1));
		}
	}
}

void PlayerStats()
{
	gedict_t *p, *p2;
	char *tmp, *stats;
	int i, pL = 0, tL = 0;

	if ( isRA() ) {
		ra_PlayerStats();
		return;
	}

	if( match_in_progress != 2 ) {
		G_sprint(self, 2, "no game - no statistics\n");
		return;
	}

	for ( p = world; (p = find_plr( p )); )
		p->k_flag = 0;

	for ( p = world; (p = find_plr( p )); ) {
		pL = max(pL, strlen(p->netname));
		tL = max(tL, strlen(getteam(p)));
	}
	pL = bound( 0, pL, 10 );
	tL = bound( 0, tL, 4 );

	G_sprint(self, 2, "%s:\n"
		"%s %s %s \217  %s\n",
		redtext("Player statistics"),
		redtext("Frags"), redtext("rank"), isTeam() ? redtext("friendkills ") : "  ",
		redtext("efficiency"));

	G_sprint(self, 2, "\235\236\236\236\236\236\236\236\236\236\236"
		"\236\236\236\236\236\236\236\236\236\236\236\236\236\236\236%s\237\n",
		((isTeam() || isCTF()) ? "\236\236\236\236\236\236\236\236\236\236" : ""));

	for ( p = world; (p = find_plr( p )); ) {
		if( p->k_flag )
			continue; // already served

		tmp = getteam( p );

		for ( p2 = world; (p2 = find_plr( p2 )); )
		{
			if ( p2->k_flag || strneq( tmp, getteam( p2 ) ) )
				continue; // already served or not on the same team

			if ( isTeam() || isCTF() ) {
				G_sprint(self, 2, "\220%.4s\221 ", tmp);
				for ( i = strlen(tmp); i < tL; i++ )
					G_sprint(self, 2, " ");
			}

			G_sprint(self, 2, "%.10s ", p2->netname); // player name
			for ( i = strlen(p2->netname); i < pL; i++ )
				G_sprint(self, 2, " ");

			stats = va("%d", ( !isCTF() ? (int)p2->s.v.frags : (int)(p2->s.v.frags - p2->ps.ctf_points)));
			G_sprint(self, 2, "%3s", stats); // frags

			stats = va("%d", ( !isCTF() ? (int)(p2->s.v.frags - p2->deaths) : (int)(p2->s.v.frags - p2->ps.ctf_points - p2->deaths)));
			G_sprint(self, 2, "%4s ", stats); // rank

			if ( isTeam() ) { // friendkills
				stats = va("%d", (int)p2->friendly);
				G_sprint(self, 2, "%2s ", stats);
			}

			if ( isCTF() )
			{
				if ( p2->s.v.frags - p2->ps.ctf_points < 1 )
					p2->efficiency = 0;
				else
					p2->efficiency = (p2->s.v.frags - p2->ps.ctf_points) / (p2->s.v.frags - p2->ps.ctf_points + p2->deaths) * 100;
			}
			else
			{
				if( p2->s.v.frags < 1 )
					p2->efficiency = 0;
				else
					p2->efficiency = p2->s.v.frags / (p2->s.v.frags + p2->deaths) * 100;
			}
			
			stats = va("%3.1f", p2->efficiency);
			G_sprint(self, 2, "\217 %5s%%\n", stats); // effi

			p2->k_flag = 1; // mark as served
		}
	}

	for( p = world; (p = find_plr( p )); )
		p->k_flag = 0;
}

void ToggleQLag()
{
	int fpd = iKey( world, "fpd" );

	if ( match_in_progress )
		return;

	fpd ^= 8;

	localcmd("serverinfo fpd %d\n", fpd);

	G_bprint(2, "%s %s\n", 
		redtext("QiZmo lag settings"), ( (fpd & 8) ? "in effect" : "not in effect" ));
}

void ToggleQEnemy()
{
	int fpd = iKey( world, "fpd" );

	if ( match_in_progress )
		return;

	fpd ^= 32;

	localcmd("serverinfo fpd %d\n", fpd);

	G_bprint(2, "%s %s\n", 
		redtext("QiZmo enemy reporting"), Allowed( fpd & 32 ));
}

void ToggleQPoint()
{
	int fpd = iKey( world, "fpd" );

	if ( match_in_progress )
		return;

	fpd ^= 128;

	localcmd("serverinfo fpd %d\n", fpd);

	G_bprint(2, "%s %s\n", 
		redtext("QiZmo pointing"), Enabled( fpd & 128 ));
}

/* new FDP bits https://www.quakeworld.nu/wiki/FPD
void ToggleSkinForcing()
{
	int fpd = iKey( world, "fpd" );

	if ( match_in_progress )
		return;

	fpd ^= 256;

	localcmd("serverinfo fpd %d\n", fpd);

	G_bprint(2, "%s %s\n", 
			redtext("Skin forcing"), Enabled( !(fpd & 256) ));
}

void ToggleColorForcing()
{
	int fpd = iKey( world, "fpd" );

	if ( match_in_progress )
		return;

	fpd ^= 512;

	localcmd("serverinfo fpd %d\n", fpd);

	G_bprint(2, "%s %s\n", 
			redtext("Color forcing"), Enabled( !(fpd & 512) ));
}

void TogglePitchSpeedLimit()
{
	int fpd = iKey( world, "fpd" );

	if ( match_in_progress )
		return;

	fpd ^= 16384;

	localcmd("serverinfo fpd %d\n", fpd);

	G_bprint(2, "%s %s\n", 
			redtext("Pitch speed limit"), Enabled( fpd & 16384 ));
}

void ToggleYawSpeedLimit()
{
	int fpd = iKey( world, "fpd" );

	if ( match_in_progress )
		return;

	fpd ^= 32768;

	localcmd("serverinfo fpd %d\n", fpd);

	G_bprint(2, "%s %s\n", 
			redtext("Yaw speed limit"), Enabled( fpd & 32768 ));
}
*/

void ToggleFreeze()
{
	if ( match_in_progress )
		return;

	cvar_toggle_msg( self, "k_freeze", redtext("map freeze"));
}

// qqshka: pointing code stolen from Zquake

void ShowNick()
{
	gedict_t	*p, *bp = NULL;
	char		*s1, *s2, *pups, *kn, buf[256] = {0};
	char		arg_1[32];
	float		best;
	vec3_t		ang;
	vec3_t		vieworg, entorg;
	int			itms, i, ln, version;

	if ( !match_in_progress )
		;  // allow shownick in prewar anyway
	else if ( !isTeam() && !isCTF() )
		return;

	trap_CmdArgv( 1, arg_1, sizeof( arg_1 ) );

	ang[0] = self->s.v.v_angle[0];
	ang[1] = self->s.v.v_angle[1];
	ang[2] = 0;

	trap_makevectors(ang);

	VectorCopy (self->s.v.origin, vieworg);
	vieworg[2] += 16 /* was 22 */;	// adjust for view height

	best = -1;

	s1 = getteam( self );

	for ( p = world; (p = find_plr( p )); )
	{
		vec3_t	v, v2, v3;
		float dist, miss, rank;

		if ( ISDEAD(p) )
			continue; // ignore dead

		if ( p == self )
			continue; // ignore self

		if ( strnull( p->netname ) )
			continue; // ignore not really players

		if ( p->s.v.modelindex != modelindex_player )
			continue; // ignore non player mdl index (EYES)

		s2 = getteam( p );

		if ( !match_in_progress )
			;  // allow shownick in prewar anyway
		else if ( strneq( s1, s2 ) )
			continue; // ignore non teammaters


		VectorCopy (p->s.v.origin, entorg);
		entorg[2] += 30;
		VectorSubtract (entorg, vieworg, v);

		dist = DotProduct (v, g_globalvars.v_forward);
		if ( dist < 10 )
			continue;

		VectorScale (g_globalvars.v_forward, dist, v2);
		VectorSubtract (v2, v, v3);
		miss = VectorLength (v3);
		if (miss > 300)
			continue;
		if (miss > dist*1.7)
			continue;		// over 60 degrees off
		if (dist < 3000.0/8.0)
			rank = miss * (dist*8.0*0.0002f + 0.3f);
		else
			rank = miss;
		
		if (rank < best || best < 0) {
			// check if we can actually see the object
			vec3_t	end;
			float	radius;

			radius = 27;
			if ((int)p->s.v.effects & (EF_DIMLIGHT | EF_BRIGHTLIGHT | EF_BLUE | EF_RED | EF_GREEN))
				radius = 200;

			if (dist <= radius)
				goto ok;

			VectorSubtract (vieworg, entorg, v);
			VectorNormalize (v);
			VectorMA (entorg, radius, v, end);
			traceline( PASSVEC3( vieworg ), PASSVEC3( end ), true, self );
			if ( g_globalvars.trace_fraction == 1 )
				goto ok;

			VectorMA (entorg, radius, g_globalvars.v_right, end);
			VectorSubtract (vieworg, end, v);
			VectorNormalize (v);
			VectorMA (end, radius, v, end);
			traceline( PASSVEC3( vieworg ), PASSVEC3( end ), true, self );
			if ( g_globalvars.trace_fraction == 1 )
				goto ok;

			VectorMA (entorg, -radius, g_globalvars.v_right, end);
			VectorSubtract (vieworg, end, v);
			VectorNormalize (v);
			VectorMA (end, radius, v, end);
			traceline( PASSVEC3( vieworg ), PASSVEC3( end ), true, self );
			if ( g_globalvars.trace_fraction == 1 )
				goto ok;

			VectorMA (entorg, radius, g_globalvars.v_up, end);
			VectorSubtract (vieworg, end, v);
			VectorNormalize (v);
			VectorMA (end, radius, v, end);
			traceline( PASSVEC3( vieworg ), PASSVEC3( end ), true, self );
			if ( g_globalvars.trace_fraction == 1 )
				goto ok;

			// use half the radius, otherwise it's possible to see
			// through floor in some places
			VectorMA (entorg, -radius/2, g_globalvars.v_up, end);
			VectorSubtract (vieworg, end, v);
			VectorNormalize (v);
			VectorMA (end, radius, v, end);
			traceline( PASSVEC3( vieworg ), PASSVEC3( end ), true, self );
			if ( g_globalvars.trace_fraction == 1 )
				goto ok;

			continue;	// not visible
ok:
			best = rank;
			bp = p;
		}
	}

	if ( best < 0 || !bp )
		return;

	version = atoi(arg_1);

	version = bound(0, version, 1);

	switch ( version )
	{
		case 1:
		{
			int h, a;

			if ( strnull( kn = ezinfokey(bp, "k_nick") ) ) // get nick, if any, do not send name, client can guess it too
				kn = ezinfokey(bp, "k");

			if (kn[0] && kn[1] && kn[2] && kn[3])
				kn[4] = 0; // truncate nick to 4 symbols

			i = NUM_FOR_EDICT( bp ) - 1;
			h = bound(0, (int)bp->s.v.health, 999);
			a = bound(0, (int)bp->s.v.armorvalue, 999);

			stuffcmd( self, "//sn %d %d %d %d %d %d %d %d \"%s\"\n", version, i,
				(int)bp->s.v.origin[0], (int)bp->s.v.origin[1], (int)bp->s.v.origin[2], h, a, (int)bp->s.v.items, kn );

			return;
		}
	}

	itms = (int)bp->s.v.items;

	pups = "";

	if ( bp->invincible_finished >= g_globalvars.time )
		pups = va("%s\220%s\221", ( *pups ? va("%s ", pups) : ""), redtext("Pent"));
	if ( bp->super_damage_finished > g_globalvars.time )
		pups = va("%s\220%s\221", ( *pups ? va("%s ", pups) : ""), redtext("Quad"));
	if ( bp->radsuit_finished > g_globalvars.time )
		pups = va("%s\220%s\221", ( *pups ? va("%s ", pups) : ""), redtext("Suit"));

	if ( *pups )
		pups = va("%s\n", pups);

	if ((int)bp->s.v.armorvalue)
		s1 = va("%s:%d ", redtext( armor_type( itms ) ), (int)bp->s.v.armorvalue);
	else
		s1 = "";

	s2 = "";
	if ((itms & IT_RL_AND_LG) == IT_RL_AND_LG) s2 = va(" %s:%d %s:%d", redtext("rl"), (int)bp->s.v.ammo_rockets, redtext("lg"), (int)bp->s.v.ammo_cells);
	else if (itms & IT_ROCKET_LAUNCHER)        s2 = va(" %s:%d", redtext("rl"), (int)bp->s.v.ammo_rockets);
	else if ( itms & IT_LIGHTNING )            s2 = va(" %s:%d", redtext("lg"), (int)bp->s.v.ammo_cells);
	else if ( itms & IT_GRENADE_LAUNCHER )     s2 = va(" %s:%d", redtext("gl"), (int)bp->s.v.ammo_rockets);
	else if ( itms & IT_SUPER_NAILGUN )        s2 = va(" %s:%d", redtext("sng"),(int)bp->s.v.ammo_nails);
	else if ( itms & IT_SUPER_SHOTGUN )        s2 = va(" %s:%d", redtext("ssg"),(int)bp->s.v.ammo_shells);
	else if ( itms & IT_NAILGUN )              s2 = va(" %s:%d", redtext("ng"), (int)bp->s.v.ammo_nails);
	else if ( itms & IT_SHOTGUN )              s2 = va(" %s:%d", redtext("sg"), (int)bp->s.v.ammo_shells);
	else if ( itms & IT_AXE )                  s2 = redtext(" axe!"); // just in case :]

	kn = ezinfokey( bp, "k_nick" );
	if ( strnull( kn ) )
		kn = ezinfokey( bp, "k" );
	if ( strnull( kn ) )
		kn = bp->netname;

	ln = iKey( self, "ln" );
	ln = (iKey( self, "ktpl" ) ? abs(ln + 2) : ln); // NOTE: ktpro does't allow negative "ln", muhaha

	if ( (i = ln) > 0 ) {
		i = bound(0, i, sizeof(buf)-1 );
		memset( (void*)buf, (int)'\n', i);
		buf[i] = 0;
	}

	if ( match_in_progress != 2 )	// simple shownick in prewar
		strlcat(buf, va( "%s\n", kn ), sizeof(buf));
	else
		strlcat(buf, va(
						"%s" // if powerups present \n is too
						"%s%s:%d%s\n"
						"%s" , pups,
						s1, redtext("h"), (int)bp->s.v.health, s2, kn
						), sizeof(buf));

	if ( (i = ln) < 0 ) {
		int offset = strlen(buf);
		i = bound(0, -i, (int)sizeof(buf) - offset - 1);
		memset( (void*)(buf + offset), (int)'\n', i);
		buf[i+offset] = 0;
	}

	G_centerprint(self, "%s", buf);

	self->need_clearCP  = 1;
	self->shownick_time = g_globalvars.time + 0.8; // clear centerprint at this time
}

// qqshka

// below predefined settings for usermodes

// this settings used when server desire general rules reset: last player disconnects / race toggled / etc.
const char _reset_settings[] =
	"serverinfo matchtag \"\"\n" // Hint for QTV what type of event it is. Like: "EQL semifinal" etc.
	"k_teamoverlay 0\n"          // Teamoverlay off
	"serverinfo ktxmode \"\"\n"; // No special mode

// common settings for all user modes
const char common_um_init[] =
	"k_pow_pickup 0\n"
	"sv_loadentfiles_dir \"\"\n"
	"sv_antilag 2\n"			// antilag on
	"k_bloodfest 0\n"
	"k_killquad 0\n"
	"pm_airstep \"\"\n"			// airstep off by default
	"samelevel 1\n"				// change levels off
//	"k_vwep 1\n"				// disable VWEP by default
	"maxclients 8\n"			// maxclients
	"k_yawnmode 0\n"			// disable SHITMODE by default (c)Renzo
	"k_instagib 0\n"			// instagib off
	"k_cg_kb 1\n"				// coilgun kickback in instagib
	"k_disallow_weapons 16\n"	// disallow gl in dmm4 by default

	"floodprot 10 1 1\n"		// 10 messages in 1 seconds, 1 second silence
	"k_fp 1\n"					// floodprot for players
	"k_fp_spec 1\n"				// floodprot for specs

	"dmm4_invinc_time \"\"\n"	// reset to default

	"k_noitems \"\"\n"			// reset to default

//	"localinfo k_new_mode 0\n"	// UNKNOWN ktpro
//	"localinfo k_fast_mode 0\n	// UNKNOWN ktpro
//	"localinfo k_safe_rj 0\n"	// UNKNOWN ktpro
//	"localinfo k_new_spw 0\n"	// ktpro feature

	"k_clan_arena 0\n"			// disable Clan Arena by default
	"k_rocketarena 0\n"			// disable Rocket Arena by default
	"k_race 0\n"				// disable Race  by default
	"k_hoonymode 0\n"			// disable HoonyMode by default

	"k_spec_info 1\n"			// allow spectators receive took info during game
	"k_midair 0\n"				// midair off
	LGCMODE_VARIABLE " 0\n"     // LGC mode off

	"fraglimit 0\n"				// fraglimit %)
	"dp 1\n"					// drop pack
	"dq 0\n"					// drop quad
	"dr 0\n"					// drop ring
	"k_frp 0\n"					// fairpacks
	"k_spectalk 0\n"			// silence
	"k_dis 1\n"					// discharge on
	"k_spw 4\n"					// affect spawn type
	"k_dmgfrags 0\n"			// damage frags off
	"k_dmm4_gren_mode 0\n"		// dmm4 grenade mode off
	"k_tp_tele_death 1\n"		// affect frags on team telefrags or not
	"k_allowcountchange 1\n"	// permissions for upplayers, only real admins
	"k_maxspectators 4\n"		// some default value
	"k_ip_list 1\n"				// permissions for iplist, only real admins

	"k_idletime 0\n"			// idlebot
	"k_membercount 0\n"			// some unlimited values
	"k_lockmin 0\n"				// some unlimited values
	"k_lockmax 64\n"			// some unlimited values
	"k_lockmode 1\n"			// server lockmode
	"k_pow_q 1\n"				// powerups - quad
	"k_pow_p 1\n"				// powerups - pent
	"k_pow_r 1\n"				// powerups - ring
	"k_pow_s 1\n"				// powerups - suit
	"qtv_sayenabled 0\n"		// default blocking of qtv chat
	"k_bzk 0\n"					// berzerk mode off
;

const char _1on1_um_init[] =
	"coop 0\n"					// no coop
	"maxclients 2\n"			// duel = two players
	"k_maxclients 2\n"			// duel = two players
	"timelimit  10\n"			// 10 minute rounds
	"teamplay   0\n"			// hurt yourself, no teammates here
	"deathmatch 3\n"			// weapons stay
	"k_overtime 1\n"			// overtime type = time based
	"k_exttime 3\n"				// overtime 3mins
	"k_pow 0\n"					// powerups
	"k_membercount 0\n"			// no efect in duel
	"k_lockmin 0\n"				// no efect in duel
	"k_lockmax 0\n"				// no efect in duel
	"k_mode 1\n";				//

const char _1on1hm_um_init[] =
	"coop 0\n"                  // no coop
	"maxclients 2\n"            // duel = two players
	"k_maxclients 2\n"          // duel = two players
	"fraglimit 1\n"             // hoonymode - every 1 frag we toggle spawns
	"timelimit 0\n"             // hoonymode - timelimit 0
	"k_hoonymode 1\n"
	"k_hoonyrounds 12\n"        // first to seven
	"teamplay 0\n"              // hurt yourself, no teammates here
	"deathmatch 3\n"            // weapons stay
	"k_overtime 1\n"            // overtime type = time based
	"k_exttime 3\n"             // overtime 3mins
	"k_pow 0\n"                 // powerups
	"k_membercount 0\n"         // no efect in duel
	"k_lockmin 0\n"             // no efect in duel
	"k_lockmax 0\n"             // no efect in duel
	"k_mode 1\n";               //

// 'blitz' now
const char _2on2hm_um_init[] =
	"coop 0\n"					// no coop
	"maxclients 4\n"			// 2on2 = 4 players
	"k_maxclients 4\n"			// 2on2 = 4 players
	"timelimit  3\n"			// 3 minute rounds
	"k_hoonyrounds 4\n"         // 4 rounds (2 sets of spawns)
	"fraglimit  0\n"            // hoonymode - no fraglimit, time-based
	"k_hoonymode 1\n"
	"teamplay   2\n"			// hurt teammates and yourself
	"deathmatch 3\n"			// weapons stay
	"k_overtime 0\n"			// time based
	"k_exttime 3\n"				// overtime 3mins
	"k_pow 1\n"					// use powerups
	"k_membercount 1\n"			// minimum number of players in each team
	"k_lockmin 1\n"				//
	"k_lockmax 2\n"				//
	"k_mode 2\n";				//

const char _2on2_um_init[] =
	"coop 0\n"					// no coop
	"maxclients 4\n"			// 2on2 = 4 players
	"k_maxclients 4\n"			// 2on2 = 4 players
	"timelimit  10\n"			// 10 minute rounds
	"teamplay   2\n"			// hurt teammates and yourself
	"deathmatch 3\n"			// weapons stay
	"k_overtime 1\n"			// time based
	"k_exttime 3\n"				// overtime 3mins
	"k_pow 1\n"					// use powerups
	"k_membercount 1\n"			// minimum number of players in each team
	"k_lockmin 1\n"				//
	"k_lockmax 2\n"				//
	"k_mode 2\n";				//

const char _3on3_um_init[] =
	"coop 0\n"					// no coop
	"maxclients 6\n"			// 3on3 = 6 players
	"k_maxclients 6\n"			// 3on3 = 6 players
	"timelimit  15\n"			// 15 minute rounds
	"teamplay   2\n"			// hurt teammates and yourself
	"deathmatch 1\n"			// weapons wont stay on pickup
	"k_pow 1\n"					// use powerups
	"k_membercount 2\n"			// minimum number of players in each team
	"k_lockmin 1\n"				//
	"k_lockmax 2\n"				//
	"k_overtime 1\n"			// time based
	"k_exttime 5\n"				// overtime 5mins
	"k_mode 2\n";				//

// 'blitz' now
const char _4on4hm_um_init[] =
	"coop 0\n"					// no coop
	"maxclients 8\n"			// 4on4 = 8 players
	"k_maxclients 8\n"			// 4on4 = 8 players
	"timelimit  5\n"			// 5 minute rounds
	"k_hoonyrounds 4\n"         // 4 rounds (2 sets of spawns)
	"fraglimit  0\n"            // no fraglimit, time-based
	"teamplay   2\n"			// hurt teammates and yourself
	"deathmatch 1\n"			// weapons wont stay on pickup
	"k_hoonymode 1\n"
	"k_pow 1\n"					// use powerups
	"k_membercount 3\n"			// minimum number of players in each team
	"k_lockmin 1\n"				//
	"k_lockmax 2\n"				//
	"k_overtime 1\n"			// time based
	"k_exttime 5\n"				// overtime 5mins
	"k_mode 2\n";				//

const char _4on4_um_init[] =
	"coop 0\n"					// no coop
	"maxclients 8\n"			// 4on4 = 8 players
	"k_maxclients 8\n"			// 4on4 = 8 players
	"timelimit  20\n"			// 20 minute rounds
	"teamplay   2\n"			// hurt teammates and yourself
	"deathmatch 1\n"			// weapons wont stay on pickup
	"k_pow 1\n"					// use powerups
	"k_membercount 3\n"			// minimum number of players in each team
	"k_lockmin 1\n"				//
	"k_lockmax 2\n"				//
	"k_overtime 1\n"			// time based
	"k_exttime 5\n"				// overtime 5mins
	"k_mode 2\n";				//

const char _10on10_um_init[] =
	"coop 0\n"					// no coop
	"maxclients 20\n"			// 10on10 = 20 players
	"k_maxclients 20\n"			// 10on10 = 20 players
	"timelimit  20\n"			// 20 minute rounds
	"teamplay   2\n"			// hurt yourself and teammates
	"deathmatch 1\n"			// wpons dowont stay on pickup
	"k_pow 1\n"					// user powerups
	"k_membercount 5\n"			// minimum number of players in each team
	"k_lockmin 1\n"				//
	"k_lockmax 2\n"				//
	"k_overtime 1\n"			// time based
	"k_exttime 5\n"				// overtime 5mins
	"k_mode 2\n";				//

const char ffa_um_init[] =
//	"coop 0\n"					// NO WE CAN'T DO IT SO, FFA MATCHLESS USED IN COOP MODE
	"maxclients 26\n"			// some limit
	"k_maxclients 26\n"			// some limit
	"timelimit  20\n"			// some limit
	"teamplay   0\n"			// hurt yourself, no teammates
	"deathmatch 3\n"			// weapons stay
	"dq 1\n"					// drop quad
	"dr 1\n"					// drop ring
	"k_pow 1\n"					// use powerups
	"k_membercount 0\n"			// no effect in ffa
	"k_lockmin 0\n"				// no effect in ffa
	"k_lockmax 0\n"				// no effect in ffa
	"k_overtime 1\n"			// time based
	"k_exttime 5\n"				// overtime 5mins
	"k_mode 3\n"				//
	"k_bzk 0\n";				// berzerk mode on ;)

const char ctf_um_init[] =
	"sv_loadentfiles_dir ctf\n"
	"pm_airstep 1\n"
	"coop 0\n"
	"maxclients 16\n"
	"k_maxclients 16\n"
	"timelimit 10\n"
	"teamplay 4\n"
	"deathmatch 3\n"
	"k_dis 2\n"					// no out of water discharges in ctf
	"k_pow 1\n"
	"k_spw 1\n"
	"k_membercount 0\n"
	"k_lockmin 1\n"
	"k_lockmax 2\n"
	"k_overtime 1\n"
	"k_exttime 5\n"
	"k_mode 4\n"
	"k_ctf_based_spawn 1\n"		// team based spawn
	"k_ctf_hook 0\n"			// hook off
	"k_ctf_runes 0\n"			// runes off
	"k_ctf_ga 1\n";				// green armor on


usermode um_list[] =
{
	{ "1on1",          "\223 on \223",          _1on1_um_init,      UM_1ON1,    1 },
	{ "2on2",          "\224 on \224",          _2on2_um_init,      UM_2ON2,    2 },
	{ "3on3",          "\225 on \225",          _3on3_um_init,      UM_3ON3,    3 },
	{ "4on4",          "\226 on \226",          _4on4_um_init,      UM_4ON4,    4 },
	{ "10on10",        "\223\222 on \223\222",  _10on10_um_init,    UM_10ON10, 10 },
	{ "ffa",           "ffa",                   ffa_um_init,        UM_FFA,    -1 },
	{ "ctf",           "ctf",                   ctf_um_init,        UM_CTF,     0 },
	{ "hoonymode",     "HoonyMode",		        _1on1hm_um_init,    UM_1ON1HM,  0 },
	{ "blitz-2v2",     "Blitz (2v2)",           _2on2hm_um_init,    UM_1ON1HM,  0 },
	{ "blitz-4v4",     "Blitz (4v4)",           _4on4hm_um_init,    UM_1ON1HM,  0 }
};

int um_cnt = sizeof (um_list) / sizeof (um_list[0]);

// return -1 if not found
int um_idx_byname(char *name)
{
	int i;

	if ( strnull( name ) )
		return -1; // not found

	for( i = 0; i < um_cnt; i++ )
		if ( streq( name, um_list[i].name) )
			return i;

	return -1; // not found
}

extern int skip_fixrules;

void UserMode_SetMatchTag(char * matchtag)
{
	char matchtag_old[20] = { 0 }, matchtag_new[20] = { 0 };

	// get current serverinfo matchtag.
	infokey(world, "matchtag", matchtag_old, sizeof(matchtag_old));
	// set new matchtag.
	localcmd("serverinfo matchtag \"%s\"\n", clean_string(matchtag));
	trap_executecmd(); // <- this really needed
	// check what we get in serverinfo after all.
	infokey(world, "matchtag", matchtag_new, sizeof(matchtag_new));

	if (matchtag_new[0]) {
		G_bprint(2, "\n" "%s is %s\n", redtext("matchtag"), matchtag_new);
	}
	else if (matchtag_old[0]) {
		G_bprint(2, "\n" "%s %s\n", redtext("matchtag"), redtext("disabled"));
	}
	else {
		G_bprint(2, "\n" "%s not set\n", redtext("matchtag"));
	}
}

// for user call this like UserMode( 1 )
// for server call like UserMode( -1 )
void UserMode(float umode)
{
	char matchtag[20] = {0};
	const char *um;
	char buf[1024*4];
	char *cfg_name;
	qbool sv_invoked = false;

	int k_free_mode = ( k_matchLess ? 5 : cvar( "k_free_mode" ) );

	if ( umode < 0 ) {
		sv_invoked = true;
		umode *= -1;
	}
	else {
		if ( world->hoony_timelimit || ! strnull(world->hoony_defaultwinner) ) {
			G_sprint( self, 2, "This map is designed for hoonymode only\n" );
			return;
		}

		if ( cvar("k_auto_xonx") ) {
			G_sprint(self, 2, "Command blocked due to k_auto_xonx\n");
			return;
		}
	}

	umode = (int)umode - 1;

	if ( umode < 0 || umode >= um_cnt ) {
		G_bprint(2, "UserMode: unknown mode\n");
		return;
	}

	// get user supplied matchtag if any.
	trap_CmdArgs( matchtag, sizeof( matchtag ) );

	um = um_list[(int)umode].name;

	if (isRACE()) {
		if (!sv_invoked) {
			race_switch_usermode(um, um_list[(int)umode].race_plrs_per_team);
		}
		return;
	}

	if ( !k_matchLess ) // allow for matchless mode
		if ( !is_rules_change_allowed() )
			return;

	if ( streq(um, "ffa") && k_matchLess && cvar("k_use_matchless_dir") )
		um = "matchless"; // use configs/usermodes/matchless instead of configs/usermodes/ffa in matchless mode

	if (streq(um, "ctf") && bots_enabled() && !sv_invoked) {
		if (bots_enabled()) {
			G_sprint(self, PRINT_HIGH, "Disable bots first with %s\n", redtext("/botcmd disable"));
			return;
		}
	}

	//for 1on1 / 2on2 / 4on4 and ffa commands manipulation
	//0 - no one, 1 - admins, 2 elected admins too
	//3 - only real real judges, 4 - elected judges too
	//5 - all players

	// hmm, I didn't understand how k_free_mode affect this command,
	// so implement how i think this must be, it is like some sort of access control
	if ( sv_invoked ) {
		if ( k_free_mode != 5 ) {
			G_bprint(2, "UserMode: sv %s discarded due to k_free_mode\n", um);
			return;
		}
	}
	else if ( !check_perm(self, k_free_mode) )
		return;

// ok u have access, but is this command really allowed at all?

	if ( !(um_list[(int)umode].um_flags & k_allowed_free_modes) ) {

		if ( sv_invoked )
			G_bprint(2, "UserMode: sv %s discarded due to k_allowed_free_modes\n", um);
		else
			G_sprint(self, 2, "Server %s this command\n", redtext("disallows"));

		return;
	}

#ifdef CTF_RELOADMAP

	if (   ( !isCTF() &&  (um_list[(int)umode].um_flags & UM_CTF) )
		|| (  isCTF() && !(um_list[(int)umode].um_flags & UM_CTF) )
	)
		skip_fixrules = 2; // skip FixRules for 2 frames, or we get some teamplay warning

#else
	// We invoke ctf command.
	// So we need check ready players, and if they have wrong teams, discard ctf command
	// untill they type break or fix team names.
	// After ctf mode activated no one can have wrong team and be ready at the same time.
	if ( !isCTF() && (um_list[(int)umode].um_flags & UM_CTF) ) {
		gedict_t	*p;

		for( p = world; (p = find_plr( p )); )
			if ( p->ready && (!streq(getteam(p), "blue") && !streq(getteam(p), "red")) )
			{
				if ( sv_invoked )
					G_bprint(2, "UserMode: sv %s discarded due to ready players have not red or blue team\n", um);
				else
					G_sprint(self, 2, "command discarded due to ready players have not red or blue team\n"
						"either force they fix team or be not ready\n" );

				return;
			}
	}
#endif

	if ( !k_matchLess ) { // do not show for matchless mode
		if ( sv_invoked )
			G_bprint(2, "%s %s\n", redtext(va("%s", um_list[(int)umode].displayname)), redtext("settings enabled") );
		else
			G_bprint(2, "%s %s %s\n", redtext(va("%s", um_list[(int)umode].displayname)), redtext("settings enabled by"), self->netname );
	}

	trap_readcmd( common_um_init, buf, sizeof(buf) );
	G_cprint("%s", buf);

	trap_readcmd( um_list[(int)umode].initstring, buf, sizeof(buf) );
	G_cprint("%s", buf);

	cfg_name = "configs/usermodes/default.cfg";
	if ( can_exec( cfg_name ) ) {
		trap_readcmd( va("exec %s\n", cfg_name), buf, sizeof(buf) );
		G_cprint("%s", buf);
	}
	cfg_name = va("configs/usermodes/%s/default.cfg", um);
	// If CTF matchless mode, load config from /matchless/ctf.cfg instead of /matchless/default.cfg
	// isCTF() and k_mode aren't reliable to use here, so define "k_use_matchless_dir 2" for this purpose
	if ( streq(um, "matchless") && (cvar("k_use_matchless_dir") == 2) )
		cfg_name = va("configs/usermodes/%s/ctf.cfg", um);
	if ( can_exec( cfg_name ) ) {
		trap_readcmd( va("exec %s\n", cfg_name), buf, sizeof(buf) );
		G_cprint("%s", buf);
	}
	cfg_name = va("configs/usermodes/%s.cfg", g_globalvars.mapname);
	if ( can_exec( cfg_name ) ) {
		trap_readcmd( va("exec %s\n", cfg_name), buf, sizeof(buf) );
		G_cprint("%s", buf);
	}
	cfg_name = va("configs/usermodes/%s/%s.cfg", um, g_globalvars.mapname);
	if ( can_exec( cfg_name ) ) {
		trap_readcmd( va("exec %s\n", cfg_name), buf, sizeof(buf) );
		G_cprint("%s", buf);
	}

	HM_unpick_all_spawns();
	apply_CA_settings();

	G_cprint("\n");

	// apply matchtag if not executed by server.
	if (!sv_invoked)
		UserMode_SetMatchTag( matchtag );

	cvar_fset("_k_last_xonx", umode+1); // save last XonX command
}

void execute_rules_reset(void)
{
	char *cfg_name = "configs/reset.cfg";
	char buf[1024*4];
	int um_idx;

	cvar_fset("_k_last_xonx", 0); // forget last XonX command

	// execute hardcoded reset settings.
	trap_readcmd( _reset_settings, buf, sizeof(buf) );
	G_cprint("%s", buf);

	if (is_private_game() != private_game_by_default() && private_game_voteable()) {
		private_game_toggle(private_game_by_default());
	}

	// execute configs/reset.cfg.
	if ( can_exec( cfg_name ) )
	{
		trap_readcmd( va("exec %s\n", cfg_name), buf, sizeof(buf) );
		G_cprint("%s", buf);
	}

	G_cprint("\n");

	// execute usermode.
	if ( ( um_idx = um_idx_byname( k_matchLess ? "ffa" : cvar_string("k_defmode") ) ) >= 0 )
		UserMode( -(um_idx + 1) ); // force exec configs for default user mode
}

// ok, a bit complicated
// this routine may change map if srv_practice_mode == 0 and mapname is not NULL
void SetPractice(int srv_practice_mode, const char *mapname)
{
	if ( match_in_progress )
		G_Error ("SetPractice: match_in_progress");

	k_practice = srv_practice_mode;
	cvar_fset("srv_practice_mode", srv_practice_mode);

	if ( k_practice )
		G_bprint(2, "%s\n", redtext("Server in practice mode"));
	else {
		G_bprint(2, "%s\n", redtext("Server in normal mode"));
		if ( mapname ) // mapname may be "" i.e empty, reload current map in this case
			changelevel( ( strnull( mapname ) ? g_globalvars.mapname : mapname ) );
	}
}

void TogglePractice()
{
	int lock_practice         = cvar( "lock_practice" );
	int allow_toggle_practice = cvar( "allow_toggle_practice" );

	if ( match_in_progress )
		return;

	if ( lock_practice == 2 /* server locked in current practice mode */
		|| (lock_practice != 0 && lock_practice != 1) /* unknown lock type, ignore command */
		) {
		G_sprint(self, 3, "console: command is locked\n");
		return;
	}

	if ( k_force || find ( world, FOFCLSN, "idlebot" ) )
		return;  // cmon, no practice if forcestart or idlebot active

//0 - no one, 1 - admins, 2 elected admins too
//3 - only real real judges, 4 - elected judges too
//5 - all players

// implement how i think this must be, it is like some sort of access control

	switch ( allow_toggle_practice ) {
		case 0:	
			G_sprint(self, 2, "%s can use this command\n", redtext("no one"));
			return;
		case 1:
		case 2:
			if ( !is_adm( self ) ) {
				G_sprint(self, 2, "you must be an %s\n", redtext("admin"));
				return;
			}
			break;
		case 3:
		case 4:
			if ( !is_adm( self ) ) {
				G_sprint(self, 2, "%s is not implemented in this mode\n", redtext("judges"));
				G_sprint(self, 2, "you must be an %s\n", redtext("admin"));
				return;
			}
			break;
		case 5:
			break;
		default:
			G_sprint(self, 2, "server is misconfigured, command %s\n", redtext("skipped"));
			return;
	}

// ok u have access
	SetPractice( !k_practice, "" ); // reload current map if needed
}

// allow reset weapon stats in prewar
void Wp_Reset ( )
{
	if ( match_in_progress )
		return;

	memset(self->ps.wpn, 0, sizeof(self->ps.wpn));
}

void Wp_Stats(float on)
{
	on--;

	self->wp_stats = (int)on;
	self->wp_stats_time = g_globalvars.time; // force show/hide
}

void Sc_Stats(float on)
{
	on--;

	self->sc_stats = (int)on;
	self->sc_stats_time = g_globalvars.time; // force show/hide
}

void W_WeaponFrame();

void kfjump ()
{
	int button0 = self->s.v.button0;

	if ( cvar( "k_disallow_kfjump" ) ) {
		G_sprint(self, 2, "%s is disabled\n", redtext("kfjump"));
		return;
	}

	if ( g_globalvars.time < self->attack_finished )
		return; // sanity

	self->s.v.impulse = 7;		 // select switch to rl
	self->s.v.button0 = 1;		 // force attack button
	self->s.v.v_angle[1] += 180; // turn 180
	W_WeaponFrame ();			 // switch to rl and fire
	self->s.v.v_angle[1] -= 180; // turn back
	self->s.v.button0 = button0; // restore button state
}

void krjump ()
{
	int button0 = self->s.v.button0;
	float va_x = self->s.v.v_angle[0];

	if ( cvar( "k_disallow_krjump" ) ) {
		G_sprint(self, 2, "%s is disabled\n", redtext("krjump"));
		return;
	}

	if ( g_globalvars.time < self->attack_finished )
		return; // sanity

	self->s.v.impulse = 7;		 // select switch to rl
	self->s.v.button0 = 1;		 // force attack button
	self->s.v.v_angle[0] = 80;   // look down much as possible, qw block this at 80
	W_WeaponFrame ();			 // switch to rl and fire
	self->s.v.v_angle[0] = va_x; // restore
	self->s.v.button0 = button0; // restore button state
}

void t_jump (float j_type)
{
	char *jt, *cv_jt, cjt = j_type == 1 ? 'f' : 'r';

	if ( match_in_progress )
		return;

	jt    = va("k%cjump", cjt);
	cv_jt = va("k_disallow_k%cjump", cjt);

	trap_cvar_set_float( cv_jt, !cvar( cv_jt ) );
	G_bprint(2, "%s %s %s\n", self->netname, redtext( Enables( !cvar( cv_jt ) ) ),
		redtext( jt ) );
}

void klist ( )
{
	int i, hdc;
	gedict_t *p = world;
	char *track;

	for( i = 0, p = world; (p = find_plr( p )); i++ ) {
		if ( !i ) {
			G_sprint(self, 2, "Clients list: %s\n", redtext( "players" ) );
			G_sprint(self, 2, "%s %s %s %s %s %s\n",
				redtext( "id" ), redtext( "ad" ), redtext( "vip" ),
				redtext( "hdp" ), redtext( "team" ), redtext( "name" ) );
		}

		hdc = GetHandicap(p);

		G_sprint(self, 2, "%2d|%2s|%3d|%3s|%4.4s|%s\n", GetUserID( p ),
			(is_real_adm( p ) ? redtext("A") : is_adm( p ) ? redtext("a") : ""), VIP( p ),
			(hdc == 100 ? "off" : va("%d", hdc)), getteam( p ), getname( p ));
	}

	if (i)
		G_sprint(self, 2, "%s %2d found %s\n", redtext("--"), i, redtext("-------------") );

	for( i = 0, p = world; (p = find_spc( p )); i++ ) {
		if ( !i ) {
			G_sprint(self, 2, "Clients list: %s\n", redtext( "spectators" ) );
			G_sprint(self, 2, "%s %s %s %s\n",
				redtext( "id" ), redtext( "ad" ), redtext( "vip" ), redtext( "co" ),
				redtext( "name" ) );
		}

		track = TrackWhom( p );

		G_sprint(self, 2, "%2d|%2s|%3d|%2s|%s%s\n", GetUserID( p ),
			(is_real_adm( p ) ? redtext("A") : is_adm( p ) ? redtext("a") : ""),
			VIP( p ), (is_coach(p) ? redtext("c") : ""), getname( p ),
			(strnull(track) ? "" : va(" \x8D %s", track)) );
	}

	if (i)
		G_sprint(self, 2, "%s %2d found %s\n", redtext("--"), i, redtext("-------------") );

	for( i = 0, p = world; (p = find(p, FOFCLSN, "ghost")); i++ ) {
		if ( !i ) {
			G_sprint(self, 2, "Clients list: %s\n", redtext( "ghosts" ) );
			G_sprint(self, 2, "%s %s %s\n",
				redtext( "frags" ), redtext( "team" ), redtext( "name" ) );
		}
		G_sprint(self, 2, "%5d|%4.4s|%s\n",	(int)p->s.v.frags, getteam( p ), getname( p ));
	}

	if (i)
		G_sprint(self, 2, "%s %2d found %s\n", redtext("--"), i, redtext("-------------") );

	for( i = 0, p = world + 1; p <= world + MAX_CLIENTS; p++ ) {
		if ( streq(track = ezinfokey(p, "*state"), "zombie") )
			;
		else if ( streq(track, "preconnected") || streq(track, "connected") )
			track = "connecting";
		else
			continue; // continue due to player spawned(connected) or free or in unknown state

		if ( !i ) {
			G_sprint(self, 2, "Clients list: %s\n", redtext( "unconnected" ) );
			G_sprint(self, 2, "%s %s %-10s %s\n",
				redtext( "id" ), redtext( "vip" ), redtext( "state" ), redtext( "name" ) );
		}

		G_sprint(self, 2, "%2d|%3d|%-10.10s|%s\n", 
			iKey(p, "*userid"), // can't use GetUserID here
			VIP( p ), track, (strnull( p->netname ) ? "!noname!" : p->netname));

		i++;
	}

	if (i)
		G_sprint(self, 2, "%s %2d found %s\n", redtext("--"), i, redtext("-------------") );
}

void hdptoggle ()
{
	if ( match_in_progress )
		return;

	trap_cvar_set_float( "k_lock_hdp", !cvar( "k_lock_hdp" ) );
	G_bprint(2, "%s %s %s\n", self->netname,
				redtext( Allows( !cvar( "k_lock_hdp" ) ) ), redtext( "handicap" ) );
}

void handicap ()
{
	char arg_2[1024];

	if ( trap_CmdArgc() != 2 )
	{
		G_sprint(self, 2, "use: /handicap value, value from 50 to 150\n");
		return;
	}

	trap_CmdArgv( 1, arg_2, sizeof( arg_2 ) );

	SetHandicap( self, atoi( arg_2 ) );
}

void show_disallowed_weapons( int k_disallow_weapons )
{
	char *dwp = str_noweapon( k_disallow_weapons );

	G_sprint(self, 2, "weapons disallowed:%s\n", 
				( strnull( dwp ) ? redtext( " none" ) : redtext( dwp ) ) );
}

void noweapon ()
{
	char arg_2[1024];
	int	k_disallow_weapons = (int)cvar("k_disallow_weapons") & DA_WPNS;

	if ( match_in_progress ) {
		if ( deathmatch == 4 ) // match started, show info and return
			show_disallowed_weapons( k_disallow_weapons );
		return;
	}

	if ( deathmatch != 4 ) {
		G_sprint(self, 2, "command allowed in %s only\n", redtext("dmm4"));
		return;
	}

	if ( trap_CmdArgc() == 1 ) { // no arguments, show info and return
		show_disallowed_weapons( k_disallow_weapons );
		return;
	}

	// one argument
	if ( trap_CmdArgc() == 2 ) {
		char *nwp = NULL;
		int bit = 0;

		trap_CmdArgv( 1, arg_2, sizeof( arg_2 ) );

		if ( streq( nwp = "axe", arg_2 ) || streq( "1", arg_2 ) )
			k_disallow_weapons ^= bit = IT_AXE;
		else if ( streq( nwp = "sg", arg_2 ) || streq( "2", arg_2 ) )
			k_disallow_weapons ^= bit = IT_SHOTGUN;
		else if ( streq( nwp = "ssg", arg_2 ) || streq( "3", arg_2 ) )
			k_disallow_weapons ^= bit = IT_SUPER_SHOTGUN;
		else if ( streq( nwp = "ng", arg_2 ) || streq( "4", arg_2 ) )
			k_disallow_weapons ^= bit = IT_NAILGUN;
		else if ( streq( nwp = "sng", arg_2 ) || streq( "5", arg_2 ) )
			k_disallow_weapons ^= bit = IT_SUPER_NAILGUN;
		else if ( streq( nwp = "gl", arg_2 ) || streq( "6", arg_2 ) )
			k_disallow_weapons ^= bit = IT_GRENADE_LAUNCHER;
		else if ( streq( nwp = "rl", arg_2 ) || streq( "7", arg_2 ) )
			k_disallow_weapons ^= bit = IT_ROCKET_LAUNCHER;
		else if ( streq( nwp = "lg", arg_2 ) || streq( "8", arg_2 ) )
			k_disallow_weapons ^= bit = IT_LIGHTNING;

		if ( bit ) {
			G_bprint(2, "%s %s %s\n", self->netname,
				redtext( Allows( !(k_disallow_weapons & bit) ) ), redtext( nwp ) );
			trap_cvar_set_float( "k_disallow_weapons", k_disallow_weapons );
		}
		else {
			G_sprint(self, 2, "unknown weapon name %s\n", redtext(arg_2));
		}
		return;
	}
}

void no_lg()
{
	stuffcmd_flags(self, STUFFCMD_IGNOREINDEMO, "cmd noweapon lg\n");
}

void no_gl()
{
	stuffcmd_flags(self, STUFFCMD_IGNOREINDEMO, "cmd noweapon gl\n");
}

void tracklist ( )
{
	int i;
	gedict_t *p;
	char *track;
	char *nt = redtext(" not tracking");

	for( i = 0, p = world; (p = find_spc( p )); i++ ) {
		if ( !i )
			G_sprint(self, 2, "%s:\n", redtext( "Trackers list" ) );

		track = TrackWhom( p );

		G_sprint(self, 2, "%15s%s\n", getname( p ),
							(strnull(track) ? nt : va(" \x8D %s", track)) );
	}

	if ( !i )
		G_sprint(self, 2, "No spectators present\n" );
}

void fpslist ( )
{
	int i;
	gedict_t *p;
	float cur, max, min, avg;

	for( i = 0, p = world; (p = find_plr( p )); i++ ) {
		if ( !i ) {
			G_sprint(self, 2, "Players %s list:\n", redtext("FPS") );
			G_sprint(self, 2, "         name:(cur \217 max \217 min \217 avg)\n");
			G_sprint(self, 2, "\235\236\236\236\236\236\236\236\236\236\236"
				"\236\236\236\236\236\236\236\236\236\236"
				"\236\236\236\236\236\236\236\236\236\236"
				"\236\236\236\236\236\237\n");
		}

		cur = p->fCurrentFrameTime ? ( 1.0f / p->fCurrentFrameTime ) : 0;
		max = p->fLowestFrameTime ? ( 1.0f / p->fLowestFrameTime ) : 0;
		min = p->fHighestFrameTime ? ( 1.0f / p->fHighestFrameTime ) : 0;

		avg = p->fFrameCount ? ( p->fAverageFrameTime / p->fFrameCount ) : 0;
		avg = avg ? (1.0f / avg) : 0;

		G_sprint(self, 2, "%13s: %3d \217 %3d \217 %3d \217%5.1f\n", getname( p ),
				Q_rint(cur), Q_rint(max), Q_rint(min), avg);
	}

	if ( !i )
		G_sprint(self, 2, "No players present\n" );
}

// This is designed for pickup games and creates totally random teams(ish)
// It creates teams thus :
// Team red  color  4 skin ""
// team blue color 13 skin ""
void RandomPickup ()
{
    int votes;
	
	if( match_in_progress )
        return;

	if( k_captains ) {
		G_sprint(self, 2, "No random pickup when captain stuffing\n");
		return;
	}

	if( k_coaches ) {
		G_sprint(self, 2, "No random pickup when coach stuffing\n");
		return;
	}

    // Dont need to bother if less than 4 players
    if( CountPlayers() < 4 )
    {
        G_sprint(self, 2, "You need at least 4 players to do this.\n");
        return;
    }

	self->v.rpickup = !self->v.rpickup;

	G_bprint(2, "%s %s!%s\n", self->netname, 
			( self->v.rpickup ? redtext("votes for rpickup") :
			redtext(va("withdraws %s rpickup vote", g_his(self)))),
			((votes = get_votes_req( OV_RPICKUP, true )) ? va(" (%d)", votes) : ""));

	vote_check_rpickup (MAX_RPICKUP_RECUSION);
}

// { spec tracking stuff 

qbool fav_del_do(gedict_t *s, gedict_t *p, char *prefix);
qbool favx_del_do(gedict_t *s, gedict_t *p, char *prefix);

// this is called from ClientDisconnect - so disconnected players
// removed from spectators favourites
void del_from_specs_favourites(gedict_t *rm)
{
	gedict_t *p;

	for( p = world; (p = find_spc( p )); ) {
		fav_del_do(p, rm, "auto: ");
		favx_del_do(p, rm, "auto: ");
	}
}

void fav_add( )
{
	int fav_num, free_num;
	gedict_t *goal = PROG_TO_EDICT(self->s.v.goalentity);
	int diff = (int)(goal - world);

	if ( goal->ct != ctPlayer || diff < 1 || diff > MAX_CLIENTS ) {
		G_sprint(self, 2, "fav_add: you are %s player!\n", redtext("not tracking"));
		return;
	}

	for ( free_num = -1, fav_num = 0; fav_num < MAX_CLIENTS; fav_num++ )
		if ( self->fav[fav_num] == diff ) {
			G_sprint(self, 2, "fav_add: %s %s added to favourites\n", goal->netname,
				redtext("already"));
			return;
		}
		else if ( free_num < 0 && !self->fav[fav_num] ) { // ok - found free slot
			free_num = fav_num;
		}

	fav_num = free_num + 1;

	if ( fav_num < 1 || fav_num > MAX_CLIENTS ) { // must not happen
		G_sprint(self, 2, "fav_add: oops, all slots busy? Can't add.\n");
		return;
	}
	
	G_sprint(self, 2, "fav_add: %s added to favourites\n", goal->netname);

	self->fav[(int)fav_num - 1] = diff;
}


// s - for whom remove
// p - who removed
qbool fav_del_do(gedict_t *s, gedict_t *p, char *prefix)
{
	qbool removed = false;
	int fav_num;

	if ( !s || !p )
		return false;

	for ( fav_num = 0; fav_num < MAX_CLIENTS; fav_num++ )
		if ( s->fav[fav_num] && (world + s->fav[fav_num]) == p ) {
			if ( removed == false ) // show info one time
				G_sprint(s, 2, "%s%s removed from favourites\n", 
					prefix, (strnull(p->netname) ? "-someone-" : p->netname));

			s->fav[fav_num] = 0;
			removed = true; // does't break, so if this player multiple times in favourites
							// he will removed anyway, must not happend really
		}

	return removed;
}

// s - for whom remove
// p - who removed
qbool favx_del_do(gedict_t *s, gedict_t *p, char *prefix)
{
	qbool removed = false;
	int fav_num;

	if ( !s || !p )
		return false;

	for ( fav_num = 0; fav_num < MAX_CLIENTS; fav_num++ )
		if ( s->favx[fav_num] && (world + s->favx[fav_num]) == p ) {
			G_sprint(s, 2, "%s%s removed from \220slot %2d\221\n", 
				prefix, (strnull(p->netname) ? "-someone-" : p->netname), fav_num + 1);

			s->favx[fav_num] = 0;
			removed = true; // does't break, so if this player multiple times in favourites
							// he will removed anyway
		}

	return removed;
}

void fav_del( )
{
	gedict_t *goal = PROG_TO_EDICT(self->s.v.goalentity);
	int diff = (int)(goal - world);

	if ( goal->ct != ctPlayer || diff < 1 || diff > MAX_CLIENTS ) {
		G_sprint(self, 2, "fav_del: you are %s player!\n", redtext("not tracking"));
		return;
	}

	if ( fav_del_do(self, goal, "fav_del: ") )
		return;

	G_sprint(self, 2, "fav_del: %s is %s favourites\n", goal->netname, redtext("not in"));
}

void fav_all_del( )
{
	qbool deleted = false;
	int fav_num;

	for ( fav_num = 0; fav_num < MAX_CLIENTS; fav_num++ )
		if ( self->fav[fav_num] ) {
			self->fav[fav_num] = 0;
			deleted = true;
		}

	G_sprint(self, 2, "Favourites list %sdeleted\n", (deleted ? "" : redtext("already ")));
}

void favx_add( float fav_num )
{
	gedict_t *goal = PROG_TO_EDICT(self->s.v.goalentity);
	int diff = (int)(goal - world);

	if ( fav_num < 1 || fav_num > MAX_CLIENTS )
		return;

	if ( goal->ct != ctPlayer || diff < 1 || diff > MAX_CLIENTS ) {
		G_sprint(self, 2, "fav add: you are %s player!\n", redtext("not tracking"));
		return;
	}
	
	G_sprint(self, 2, "fav add: %s added to \220slot %d\221\n", goal->netname, (int)fav_num);

	self->favx[(int)fav_num - 1] = diff;
}

void fav_next( )
{
	int pl_num, fav_num, first_fav, desired_fav;
	gedict_t *goal = PROG_TO_EDICT(self->s.v.goalentity), *p;
	int diff = (int)(goal - world);

	for ( fav_num = 0; fav_num < MAX_CLIENTS; fav_num++ )
		if ( self->fav[fav_num] )
			break;

	if ( fav_num >= MAX_CLIENTS ) {
		G_sprint(self, 2, "fav_next: favourites list is %s\n", redtext("empty"));
		return;
	}

	desired_fav = -2;
	first_fav = fav_num; // remember

	if ( !( goal->ct != ctPlayer || diff < 1 || diff > MAX_CLIENTS ) ) {
 		// ok - tracking player, so if goal in favourites switch to the next favourite,
 		// if goal not in favourites switch to the first favourite
		for ( fav_num = first_fav; fav_num < MAX_CLIENTS; fav_num++ )
			if ( desired_fav == -2 && self->fav[fav_num] == diff ) {
				desired_fav = -1; // found goal in favourites, search now next fav
			}
			else if ( desired_fav == -1 && self->fav[fav_num] ) {
				desired_fav = fav_num; // found next fav in favourites
				break;
			}
	}

	if ( desired_fav >= 0 )
		fav_num = desired_fav + 1;
	else
		fav_num = first_fav + 1;

	if ( fav_num < 1 || fav_num > MAX_CLIENTS ) {
		G_sprint(self, 2, "fav_next: internal error, fav_num %d\n", fav_num);
		return; // sanity - must not happen
	}

	pl_num = self->fav[fav_num - 1];

	if ( pl_num < 1 || pl_num > MAX_CLIENTS ) {
		G_sprint(self, 2, "fav_next: internal error, slot %d\n", pl_num);
		return;
	}

	p = world + pl_num;

	if ( p->ct != ctPlayer ) {
		G_sprint(self, 2, "fav_next: can't find player\n");
		return;
	}

	if ( PROG_TO_EDICT( self->s.v.goalentity ) == p ) {
		G_sprint(self, 2, "fav_next: already observing...\n");
		return;
	}
	
	stuffcmd_flags( self, STUFFCMD_IGNOREINDEMO, "track %d\n", GetUserID( p ) );
}

void xfav_go( float fav_num )
{
	gedict_t *p;
	int pl_num;

	if ( fav_num < 1 || fav_num > MAX_CLIENTS )
		return;

	pl_num = self->favx[(int)fav_num - 1];

	if ( pl_num < 1 || pl_num > MAX_CLIENTS ) {
		G_sprint(self, 2, "fav go: \220slot %d\221 is not defined\n", (int)fav_num);
		return;
	}

	p = world + pl_num;

	if ( p->ct != ctPlayer ) {
		G_sprint(self, 2, "fav go: \220slot %d\221 can't find player\n", (int)fav_num);
		return;
	}

	if ( PROG_TO_EDICT( self->s.v.goalentity ) == p ) {
		G_sprint(self, 2, "fav go: already observing...\n");
		return;
	}
	
	stuffcmd_flags( self, STUFFCMD_IGNOREINDEMO, "track %d\n", GetUserID( p ) );
}

void fav_show( )
{
	gedict_t *p;
	qbool first, showed = false;
	int fav_num, diff;

	for ( first = true, fav_num = 0; fav_num < MAX_CLIENTS; fav_num++ )
		if ( (diff = self->favx[fav_num]) ) {
			p = world + diff;
			if ( p->ct != ctPlayer || strnull( p->netname ) )
				continue;

			if ( first ) {
				G_sprint(self, 2, "%s \220%s\221 %s:\n", redtext("Favourites"),
							redtext("slots based"), redtext("list"));
				first = false;
			}

			G_sprint(self, 2, " \220slot %2d\221 \x8D %s\n", fav_num + 1, p->netname);
			showed = true;
		}

	if ( showed )
		G_sprint(self, 2, "\n");

	for ( first = true, fav_num = 0; fav_num < MAX_CLIENTS; fav_num++ )
		if ( (diff = self->fav[fav_num]) ) {
			p = world + diff;
			if ( p->ct != ctPlayer || strnull( p->netname ) )
				continue;

			if ( first ) {
				G_sprint(self, 2, "%s:\n", redtext("Favourites list"));
				first = false;
			}

			G_sprint(self, 2, " %s\n", p->netname);
			showed = true;
		}

	if ( !showed )
		G_sprint(self, 2, "Favourites list %s or nothing to show\n", redtext("empty"));
}

// { // this is used for autotrack which saved in demo
static gedict_t *autotrack_last = NULL;
static gedict_t *autotrack_hint = NULL;
static qbool autotrack_update = false;
static char *autotrack_reason = "";
// }

// relax autotrack attempts
static void ResetMVDAutoTrack( gedict_t *p )
{
	autotrack_update	= false;
	autotrack_last		= (p && p->ct == ctPlayer) ? p : NULL;
	autotrack_hint		= NULL;
	autotrack_reason	= "";
}

void DoMVDAutoTrack( void )
{
	gedict_t *p = NULL;
	int id;

	if ( !autotrack_update )
		return; // autotrack was not requested

	if ( autotrack_hint && autotrack_hint->ct == ctPlayer && ISLIVE( autotrack_hint ) )
		p = autotrack_hint; // we have hint to whom we should switch

#if 0
	// no we don't need it
	if ( !match_in_progress )
	{
		ResetMVDAutoTrack( p );
		return; // we don't need much in prewar
	}
#endif

	// search best only in case when we do not have hint above "autotrack_hint"
	if ( !p && !(p = get_ed_best1()) )
	{
		// MEAG: We can fall in here if everyone is dead (e.g. frag-trade in duel)... if we can,
		//   keep tracking current player for now and evaluate again after respawns
		if (!autotrack_last || autotrack_last->ct != ctPlayer) {
			ResetMVDAutoTrack(NULL);
		}
		return; // can't find best
	}

	// do not switch instantly autotrack pov after tracked player die, wait a few seconds or untill them respawn
	if ( autotrack_last && autotrack_last->ct == ctPlayer && ISDEAD( autotrack_last ) && g_globalvars.time - autotrack_last->dead_time < 2 )
	{
		// no, we do not reset it, we need repeat autotrack apply after some time...
		//ResetMVDAutoTrack( p );
		return;
	}

	if ( autotrack_last == p )
	{
		ResetMVDAutoTrack( p );
		return; // already track this player
	}

	if ( ( id = GetUserID( p ) ) > 0 )
	{
		// with "properly" haxed server this does't go to player but goes to mvd demo only
		stuffcmd_flags(p, STUFFCMD_DEMOONLY, "//at_dbg %s\n",  autotrack_reason);
		stuffcmd_flags(p, STUFFCMD_DEMOONLY, "//at %d\n", id );
	}

	ResetMVDAutoTrack( p );
}

// relax ktpro's autotrack
static void ResetNormalAutoTrack( gedict_t *cl )
{
	cl->apply_ktpro_autotrack = false;
	cl->autotrack_hint = NULL;
}

void DoAutoTrack( )
{
	gedict_t *p = NULL, *goal;
	int id;

	switch ( self->autotrack )
	{
		case atBest:	p = get_ed_best1();		break;	// ktx's autotrack
		case atPow:		p = get_ed_bestPow();	break;	// powerups autotrack
		case atKTPRO:
		{	// "ktpro's" autotrack
			if ( self->apply_ktpro_autotrack )
			{
				// do not try switch pov to dead player even we have it as hint
				if ( self->autotrack_hint && self->autotrack_hint->ct == ctPlayer && ISLIVE( self->autotrack_hint ) )
					p = self->autotrack_hint;
				else
					p = get_ed_best1();
			}

			break;
		}

		case atNone:
		default:
		{
			ResetNormalAutoTrack( self );
 			return; // unknow or off so ignore
		}
	}

	if ( !p )
	{
		ResetNormalAutoTrack( self );
		return;
	}

    goal = PROG_TO_EDICT( self->s.v.goalentity );

	// do not switch instantly autotrack pov after tracked player die, wait a few seconds or untill them respawn
	if ( goal->ct == ctPlayer && ISDEAD( goal ) && g_globalvars.time - goal->dead_time < 2 )
	{
		// no, we do not reset it here since we need repeat autotrack apply later
		//ResetNormalAutoTrack( self );
		return;
	}

	if ( goal == p )
	{
		ResetNormalAutoTrack( self );
		return; // already track this player
	}

	if ( ( id = GetUserID( p ) ) > 0 )
		stuffcmd_flags( self, STUFFCMD_IGNOREINDEMO, "track %d\n", id );

	ResetNormalAutoTrack( self );
}

void AutoTrack( float autoTrackType )
{
	autoTrackType_t at = self->autotrack; // save auto track type before turn off or switch to other type
	char *at_txt;

	if ( autoTrackType == self->autotrack || autoTrackType == atNone )
		self->autotrack = atNone; // turn off
	else
		self->autotrack = at = autoTrackType; // switch auto track type

	self->apply_ktpro_autotrack = (self->autotrack == atKTPRO); // select some first best pov if we use ktpro's autotrack too

	SetUserInfo(self, "*at", va("%d", self->autotrack), SETUSERINFO_STAR); // so we can restore it on level change

	switch ( at ) {
		case atBest:	at_txt = "Autotrack_ktx";	break; // ktx's autotrack
		case atPow:		at_txt = "Auto_pow";		break; // powerups autotrack
		case atKTPRO:	at_txt = "Autotrack";		break; // "ktpro's" autotrack

		case atNone:
		default: 		at_txt = "AutoUNKNOWN";
	}

	G_sprint(self, 2, "%s %s\n", redtext(at_txt), OnOff(self->autotrack));
}

void AutoTrackRestore ()
{
	autoTrackType_t at = iKey(self, "*at");

	if ( self->ct == ctPlayer )
		return;

	if ( at != atNone && at != self->autotrack )
		AutoTrack( at );
}
// >> start ktpro compatible autotrack

// When issueing this command, KTeams Pro will switch the view to the next_best 
// player. The view will then automtically switch to the next_best player when 
// one of the following events occurs: 

// 1. the first rl in the game is taken
// 2. the player currently being observed dies
// 3. any player takes a powerup
// 4. when the currently observed player has a powerup which runs out and he has 
//    neither the rocket launcher nor the lightning gun

// will force spec who used ktpro autotrack switch track
static void ktpro_autotrack_mark_spec(gedict_t *spec, gedict_t *dude)
{
	if ( spec->autotrack == atKTPRO )
	{
		spec->apply_ktpro_autotrack = true; // this will be used in DoAutoTrack( )
		spec->autotrack_hint = dude; // in some cases dude == NULL, its OK
	}
}

// will force specs who used ktpro autotrack switch track
static void ktpro_autotrack_mark_all(char *reason, gedict_t *dude)
{
	gedict_t *p;

	autotrack_update = true; // this is for mvd autotrack
	autotrack_hint   = dude; // in some cases dude == NULL, its OK
	autotrack_reason = reason;

	for ( p = world; (p = find_spc( p )); )
		ktpro_autotrack_mark_spec( p, dude );
}

// first rl was taken, mark specs to switch pov to best player, that may be not even this rl dude :P
void ktpro_autotrack_on_first_rl (gedict_t *dude)
{
	ktpro_autotrack_mark_all( "first_rl", /*dude*/ NULL ); // can't use dued, this may overwrite pent or quad
}

// player just died, switch pov to other if spec track this player and used ktpro's autotrack
void ktpro_autotrack_on_death (gedict_t *dude)
{
	gedict_t *p;
	int goal = EDICT_TO_PROG( dude ); // so we can compare with ->s.v.goal

	if (dude == autotrack_last)
	{
		autotrack_update = true; // this is for mvd autotrack
		autotrack_hint   = NULL; // yeah, we have no idea whom will be tracked
		autotrack_reason = "death";
	}

	for ( p = world; (p = find_spc( p )); )
		if ( p->s.v.goalentity == goal )
			ktpro_autotrack_mark_spec( p, NULL );
}

// change pov to someone who most close to powerup, right before powerup spawned, atm it 2 seconds.
// players which too far to powerup not counted.
void ktpro_autotrack_on_powerup_predict (gedict_t *dude)
{
	gedict_t *p = get_ed_bestPow();

	// do not allow powerup predict if there still some powruped player running around,
	// we apply it to quad and pent only, ring and suit are ignored
	if ( p )
	{
		if ( p->invincible_finished || p->super_damage_finished )
			return;
	}

	ktpro_autotrack_mark_all( "powerup_predict", dude );
}

// some powerup taken, mark specs to switch pov to best player, that may be not even this poweruped dude :P
void ktpro_autotrack_on_powerup_take (gedict_t *dude)
{
	ktpro_autotrack_mark_all( "powerup_take", /*dude*/ NULL ); // we can't use "dude", since quad may overwrite pent
}

// some powerup out, and he has neither the rocket launcher nor the lightning gun, mark specs to switch pov to best player
void ktpro_autotrack_on_powerup_out (gedict_t *dude)
{
/* qqshka: I turned this out, because sassa think its correct, lets test it...

	gedict_t *p;
	int goal = EDICT_TO_PROG( dude ); // so we can compare with ->s.v.goal

	if ( ((int)dude->s.v.items & IT_ROCKET_LAUNCHER) || ((int)dude->s.v.items & IT_LIGHTNING) )
		return; // dude have weapon, continue track him, may be add check for ammo?

	if (dude == autotrack_last)
	{
		autotrack_update = true; // this is for mvd autotrack
		autotrack_hint   = NULL; // yeah, we have no idea whom will be tracked
		autotrack_reason = "powerup_out";
	}

	for ( p = world; (p = find_spc( p )); )
		if ( p->s.v.goalentity == goal )
			ktpro_autotrack_mark_spec( p, NULL );
*/
}

// change pov to racer
void ktpro_autotrack_on_race_status_changed (void)
{
	ktpro_autotrack_mark_all( "race_status_changed", NULL );
}

void ktpro_autotrack_predict_powerup( void )
{
	extern float visible( gedict_t *targ );

	gedict_t *p, *best;
	float len, best_len;
	vec3_t org;

	if ( self->s.v.items != IT_QUAD && self->s.v.items != IT_INVULNERABILITY )
		return; // we use this function for quad and pent only, ring and suit is not interesting for us

	best = NULL;
	best_len = 99999999.0;

    for( p = world; (p = find_plr( p )); )
	{
		if ( ISDEAD( p ) )
			continue; // we are not interested in dead players

		VectorSubtract( p->s.v.origin, self->s.v.origin, org );
		len = vlen( org );

		if ( len > 500 )
		{
//			G_bprint(2, "too far %f\n", len);
			continue; // player too far from this powerup
		}

		if ( len >= best_len )
			continue; // not interesting, we alredy have someone with similar closeness to powerup

		if ( !visible( p ) )
		{
//			G_bprint(2, "not visible\n");
			continue; // powerup not visible for this player
		}

		best = p;
	}

	if ( !best )
		return; // noone was found

	ktpro_autotrack_on_powerup_predict( best );
}

// << end  ktpro compatible autotrack 

void next_best ()
{
	gedict_t *b1 = get_ed_best1(), *b2 = get_ed_best2();
	gedict_t *goal = PROG_TO_EDICT( self->s.v.goalentity ), *to;
	int id;

	if ( !b1 ) {
		G_sprint(self, 2, "%s: can't do this now\n", redtext("next_best"));
		return;
	}

	b2 = b2 ? b2 : b1;

	to = b1;
	if ( goal == b1 )
		to = b2;
	else if ( goal == b2 )
		to = b1;

	if ( ( id = GetUserID( to ) ) > 0 )
		stuffcmd_flags( self, STUFFCMD_IGNOREINDEMO, "track %d\n", id );
}

void next_pow ()
{
	gedict_t *goal = PROG_TO_EDICT( self->s.v.goalentity ), *to, *first, *p;
	qbool nextBreak = false;
	int id;

	to = first = NULL;

	for ( p = world; (p = find_plr( p )); ) {

		if ( ISDEAD(p) )
			continue;

		if ( !(    ( p->invincible_finished >= g_globalvars.time )
				|| ( p->super_damage_finished >= g_globalvars.time )
				|| ( p->invisible_finished >= g_globalvars.time )
				|| ( p->radsuit_finished >= g_globalvars.time )
		))
			continue;

		if ( nextBreak ) {
			to = p;
			break;
		}

		if ( !first )
			first = p;

		if ( goal == p ) {
			nextBreak = true;
			continue;
		}

	}

	to = to ? to: first;

	if ( !to ) {
		G_sprint(self, 2, "%s: can't find poweruped player\n", redtext("next_pow"));
		return;
	}

	if ( ( id = GetUserID( to ) ) > 0 )
		stuffcmd_flags( self, STUFFCMD_IGNOREINDEMO, "track %d\n", id );
}

// }  spec tracking stuff 


//================================================
// pos_show/pos_save/pos_move/pos_set_* commands {
//================================================
// common functions
#define Pos_Disallowed()	(match_in_progress || intermission_running || cvar( "sv_paused" ) || (isRACE() && race.status))
// parse pos_show/pos_save/pos_move <number>
int Pos_Get_idx()
{
	char arg_2[1024];

	if ( trap_CmdArgc() == 2 ) {
		trap_CmdArgv( 1, arg_2, sizeof( arg_2 ) );
		return bound( 0, atoi(arg_2) - 1, MAX_POSITIONS - 1 );
	}

	return 0;
}
// Show functions
void Pos_Show ()
{
	int idx;
	pos_t *pos;

	pos = &(self->pos[idx = Pos_Get_idx()]);

	G_sprint(self, 2, "Position: %d\n", idx + 1);
	G_sprint(self, 2, "velocity: %9.2f %9.2f %9.2f\n", PASSVEC3(pos->velocity));
	G_sprint(self, 2, "  origin: %9.2f %9.2f %9.2f\n", PASSVEC3(pos->origin));
	G_sprint(self, 2, " v_angle: %9.2f %9.2f %9.2f\n", PASSVEC3(pos->v_angle));

	G_sprint(self, 2, "    Self:\n");
	G_sprint(self, 2, "velocity: %9.2f %9.2f %9.2f\n", PASSVEC3(self->s.v.velocity));
	G_sprint(self, 2, "  origin: %9.2f %9.2f %9.2f\n", PASSVEC3(self->s.v.origin));
	G_sprint(self, 2, " v_angle: %9.2f %9.2f %9.2f\n", PASSVEC3(self->s.v.v_angle));
}
// Save
#define Pos_Save_origin(pos)	VectorCopy(self->s.v.origin, (pos)->origin)
#define Pos_Save_angles(pos)	VectorCopy(self->s.v.v_angle, (pos)->v_angle)
#define Pos_Save_velocity(pos)	VectorCopy(self->s.v.velocity, (pos)->velocity)
// pos_save
void Pos_Save ()
{
	int idx;
	pos_t *pos;

	if ( Pos_Disallowed() ) 
		return;

	pos = &(self->pos[idx = Pos_Get_idx()]);

	Pos_Save_origin (pos);
	Pos_Save_angles (pos);
	Pos_Save_velocity (pos);

	G_sprint(self, 2, "Position %d was saved\n", idx + 1);
}

// Move & Set functions
qbool Pos_Set_origin (pos_t *pos)
{
	gedict_t *p;

	if ( VectorCompare(pos->origin, VEC_ORIGIN) ) {
		G_sprint(self, 2, "Save your position first\n");
		return true;
	}

	if ( VectorCompare(pos->origin, self->s.v.origin) )
		return true;

    if (self->ct == ctPlayer)
	{
		TraceCapsule( PASSVEC3( pos->origin ), PASSVEC3( pos->origin ), false, self,
			PASSVEC3(VEC_HULL_MIN), PASSVEC3(VEC_HULL_MAX));

		p = PROG_TO_EDICT( g_globalvars.trace_ent );

		if (
			g_globalvars.trace_startsolid
			|| ( p != self && p != world && (p->s.v.solid == SOLID_BSP || p->s.v.solid == SOLID_SLIDEBOX) )
		) {
			G_sprint(self, 2, "Can't move, location occupied\n");
			return true;
		}
	}

	setorigin (self, PASSVEC3( pos->origin ) ); // u can't just copy, use setorigin

	return false;
}
#define Pos_Set_angles(pos)		{ \
	VectorCopy((pos)->v_angle, self->s.v.angles); \
	VectorCopy((pos)->v_angle, self->s.v.v_angle); \
	self->s.v.fixangle = true; \
}
#define Pos_Set_velocity(pos)	VectorCopy((pos)->velocity, self->s.v.velocity)

// pos_move
void Pos_Move ()
{
	int idx;
	pos_t *pos;

	if ( Pos_Disallowed() ) 
		return;

	if ( self->pos_move_time && self->pos_move_time + 1 > g_globalvars.time ) {
		G_sprint(self, 2, "Only one move per second allowed\n");
		return;
	}
	self->pos_move_time = g_globalvars.time;

	pos = &(self->pos[idx = Pos_Get_idx()]);

	if (Pos_Set_origin (pos))
		return;
	Pos_Set_angles (pos);
	Pos_Set_velocity (pos);

	G_sprint(self, 2, "Position %d was restored\n", idx + 1);
}

// parse arguments for pos_set_*
void Pos_Parse_Set (vec3_t *x)
{
	char arg[1024];
	int i;

	for (i = 0; i < 3; ++i) {
		trap_CmdArgv( i + 1, arg, sizeof( arg ) );

		if ( strneq(arg, "*") )
			(*x)[i] = atof(arg);
	}
}

// pos_set_origin/pos_set_angles/pos_set_velocity
void Pos_Set (float set_type)
{
// VVD: For trick chiters! :-)
// Need to think out how to limit using Pos_Set for tricking.
// May be to ban pos_set_velocity?
	pos_t pos;

	if ( Pos_Disallowed() ) 
		return;

	if ( trap_CmdArgc() != 4 ) {
		G_sprint(self, 2, "Usage: pos_{origin|angles"/*|velocity*/"} x1 x2 x3\n"
			"use '*' for no changes\n");
		return;
	}

	if ( self->pos_move_time && self->pos_move_time + 1 > g_globalvars.time ) {
		G_sprint(self, 2, "Only one move per second allowed\n");
		return;
	}
	self->pos_move_time = g_globalvars.time;

	switch ((int)set_type)
	{
		case 1:
			Pos_Save_origin(&pos);
			Pos_Parse_Set(&(pos.origin));
			Pos_Set_origin(&pos);
			break;
		case 2:
			Pos_Save_angles(&pos);
			Pos_Parse_Set(&(pos.v_angle));
			Pos_Set_angles(&pos);
			break;
		case 3:
			Pos_Save_velocity(&pos);
			Pos_Parse_Set(&(pos.velocity));
			Pos_Set_velocity(&pos);
			break;
		default:
			return;
	}

	G_sprint(self, 2, "Position set\n");
}
//================================================
// pos_show/pos_save/pos_move/pos_set_* commands }
//================================================

void Sh_Speed ()
{
	stuffcmd_flags(self, STUFFCMD_IGNOREINDEMO, "cmd info kf %d\n", (iKey( self, "kf" ) ^ KF_SPEED));
}

// /motd command

void PMOTDThink();
void SMOTDThink();

void motd_show ()
{
	gedict_t *motd;
	int owner = EDICT_TO_PROG( self );

	if ( !k_matchLess ) // show motd in matchLess mode even match in progress
	if ( match_in_progress )
		return;

	for( motd = world; (motd = find(motd, FOFCLSN, "motd")); )
		if ( owner == motd->s.v.owner ) {
			G_sprint(self, 2, "Already showing motd\n");
			return;
		}

	motd = spawn();
	motd->classname = "motd";
	motd->s.v.owner = EDICT_TO_PROG( self );
	// select MOTD for spectator or player
	motd->think = ( func_t ) ( self->ct == ctSpec ? SMOTDThink : PMOTDThink );
	motd->s.v.nextthink = g_globalvars.time + 0.1;
	motd->attack_finished = g_globalvars.time + 10;
}

void krnd ()
{
	int argc, i;
	char arg_x[1024], buf[2048] = {0};

	if ( match_in_progress ) 
		return;

	if ( ( argc = trap_CmdArgc() ) < 2 ) {
		G_sprint(self, 2, "usage: rnd <1st 2nd ...>\n");
		return;
	}
	
	for( buf[0] = 0, i = 1; i < argc; i++ ) {
		trap_CmdArgv( i, arg_x, sizeof( arg_x ) );

		strlcat(buf, arg_x, sizeof(buf));
		strlcat(buf, ( i + 1 < argc ? ", " : "" ), sizeof(buf));
	}

	G_bprint(2, "%s %s %s:\n"
		"\220%s\221\n", redtext("Random select by"), getname( self ), redtext("from"), buf );

	trap_CmdArgv( i_rnd(1, argc-1) , arg_x, sizeof( arg_x ) );

	G_bprint(2, "selected: \220%s\221\n", arg_x);
}

void agree_on_map ( )
{
	if ( !k_lastvotedmap )
		return; // no map voted

	// emulate as we select last voted map
	DoSelectMap( k_lastvotedmap ); // <- there will be all checks about match_in_progress and etc
}

// { lastscores stuff

char *lastscores2str( lsType_t lst )
{
	switch( lst ) {
		case lsDuel: return "duel"; // I was going to change this to "Duel" but maybe some stuff
		case lsTeam: return "team"; // is case sensitive out there already, to process it..? --phil
		case lsFFA:  return "FFA";
		case lsCTF:  return "CTF";
		case lsRA:   return "RA";
		case lsHM:   return "HoonyMode";
		case lsRACE: return "race";

		default:	 return "unknown";
	}
}

void lastscore_add ()
{
	gedict_t *p, *ed1 = get_ed_scores1(), *ed2 = get_ed_scores2();
	int from;
	int i, s1 = 0, s2 = 0;
	int k_ls = bound(0, cvar("__k_ls"), MAX_LASTSCORES-1);
	char *e1, *e2, t1[128] = {0}, t2[128] = {0}, *name, date[64], *extra;
	lsType_t lst = lsUnknown;

	e1 = e2 = extra = "";
	
	if ( ( isRA() || isFFA() ) && ed1 && ed2 )
	{ // not the best way since get_ed_scores do not serve ghosts, so...
		lst = (isRA() ? lsRA : lsFFA);
		e1 = getname( ed1 );
		s1 = ed1->s.v.frags;
		e2 = getname( ed2 );
		s2 = ed2->s.v.frags;
	}
	else if (isHoonyModeAny()) {
		if (HM_current_point_type() != HM_PT_FINAL) {
			return;
		}

		lst = lsHM;
		for (i = from = 0, p = world; (p = find_plrghst(p, &from)) && i < 2; i++) {
			if (!i) {
				// info about first dueler
				e1 = getname(p);
				s1 = p->s.v.frags;
			}
			else {
				// about second
				e2 = getname(p);
				s2 = p->s.v.frags;
			}
		}
	}
	else if ( isDuel() )
	{
		lst = lsDuel;

		for( i = from = 0, p = world; (p = find_plrghst( p, &from )) && i < 2; i++ )
		{
			if ( !i )
			{ // info about first dueler
				e1 = getname( p );
				s1 = p->s.v.frags;
			}
			else
			{	   // about second
				e2 = getname( p );
				s2 = p->s.v.frags;
			}
		}
	}
	else if ( ( isTeam() || isCTF() ) && k_showscores )
	{
		lst = isTeam() ? lsTeam : lsCTF;

		e1 = cvar_string( "_k_team1" );
		s1 = get_scores1();
		e2 = cvar_string( "_k_team2" );
		s2 = get_scores2();

		// players from first team
		for( t1[0] = from = 0, p = world; (p = find_plrghst( p, &from )); )
			if ( streq( getteam( p ), e1 ) && !strnull( name = getname( p ) ) )
				strlcat(t1, va(" %s", name), sizeof(t1));

		// players from second team
		for( t2[0] = from = 0, p = world; (p = find_plrghst( p, &from )); )
			if ( streq( getteam( p ), e2 ) && !strnull( name = getname( p ) ) )
				strlcat(t2, va(" %s", name), sizeof(t2));
	}

	if ( strnull( e1 ) || strnull( e2 ) )
		lst = lsUnknown;

	if ( lst == lsUnknown ) // sorry but something wrong
		return;

	if ( isRACE() ) {
		lst = lsRACE;
	}

	if ( !QVMstrftime(date, sizeof(date), "%b %d, %H:%M:%S %Y", 0) )
		date[0] = 0;

	cvar_fset(va("__k_ls_m_%d", k_ls), lst);
	cvar_set(va("__k_ls_e1_%d", k_ls), e1);
	cvar_set(va("__k_ls_e2_%d", k_ls), e2);
	cvar_set(va("__k_ls_t1_%d", k_ls), t1);
	cvar_set(va("__k_ls_t2_%d", k_ls), t2);
	cvar_set(va("__k_ls_s_%d", k_ls), va("%3d:%-3d \x8D %-8.8s %13.13s%s", s1, s2, g_globalvars.mapname, date, extra));

	cvar_fset("__k_ls", ++k_ls % MAX_LASTSCORES);

	// this is a HACK for QTV, ok EZTV
	{
		char qtvdate[64];
		gedict_t *cl = find_client( world );

		if ( !QVMstrftime(qtvdate, sizeof(qtvdate), "%b %d, %H:%M", 0) )
			qtvdate[0] = 0;

		if ( cl && !strnull( qtvdate ) )
		{
			stuffcmd(cl, "//finalscores \"%s\" \"%s\" \"%s\" \"%s\" %d \"%s\" %d\n", qtvdate, lastscores2str(lst), g_globalvars.mapname, e1, s1, e2, s2);
		}
	}
}

void lastscores ()
{
	int i, j, cnt;
	int k_ls = bound(0, cvar("__k_ls"), MAX_LASTSCORES-1);
	char *e1, *e2, *le1, *le2, *t1, *t2, *lt1, *lt2, *sc;
	qbool extended = (trap_CmdArgc() > 1); // if they specified some params, then use extended version
	lsType_t last = lsUnknown;
	lsType_t cur  = lsUnknown;

	e1 = e2 = le1 = le2 = t1 = t2 = lt1 = lt2 = "";

	for ( j = k_ls, cnt = i = 0; i < MAX_LASTSCORES; i++,	j = (j+1 >= MAX_LASTSCORES) ? 0: j+1 ) {

		cur = cvar(va("__k_ls_m_%d", j));
		e1 = cvar_string(va("__k_ls_e1_%d", j));
		e2 = cvar_string(va("__k_ls_e2_%d", j));
		t1 = cvar_string(va("__k_ls_t1_%d", j));
		t2 = cvar_string(va("__k_ls_t2_%d", j));
		sc = cvar_string(va("__k_ls_s_%d", j));

		if ( cur == lsUnknown || strnull( e1 ) || strnull( e2 ) )
			continue;

		if (
			cur != last // changed game mode
			|| (strneq(le1 , e1) || strneq(le2 , e2)) // changed teams, duelers
		) {
			lt1 = lt2 = ""; // force show teams members again
			G_sprint(self, 2, "\220%s %s %s\221 %s\n", e1, redtext("vs"), e2, redtext( lastscores2str( cur ) ));
		}

		// if team mode show members.
		// generally show members one time while show scores for each played map,
		// but if squad changed from previuos map, show members again,
		// so we know which squad played each map.
		if ( extended && ( cur == lsTeam || cur == lsCTF ) ) {
			if ( strneq(lt1 , t1) ) // first team
				G_sprint(self, 2, " %4.4s:%s\n", e1, t1);

			if ( strneq(lt2 , t2) ) // second team
				G_sprint(self, 2, " %4.4s:%s\n", e2, t2);
		}

		G_sprint(self, 2, "  %s\n", sc);

		last = cur;
		le1 = e1;
		le2 = e2;
		lt1 = t1;
		lt2 = t2;
		cnt++;
	}

	if ( cnt )
		G_sprint(self, 2, "\n"
			"Lastscores: %d entr%s found\n", cnt, cnt ? "y" : "ies");
	else
		G_sprint(self, 2, "Lastscores data empty\n");
}

// } lastscores stuff

// { spec moreinfo

qbool mi_on ()
{
	return ( (int)cvar("k_spec_info") & MI_ON );
}

qbool mi_adm_only ()
{
	return ( (int)cvar("k_spec_info") & MI_ADM_ONLY );
}

#define MI_POW (IT_INVISIBILITY | IT_INVULNERABILITY | IT_SUIT | IT_QUAD)
#define MI_ARM (IT_ARMOR1 | IT_ARMOR2 | IT_ARMOR3)
#define MI_WPN (IT_SHOTGUN | IT_SUPER_SHOTGUN | IT_NAILGUN | IT_SUPER_NAILGUN \
	| IT_GRENADE_LAUNCHER | IT_ROCKET_LAUNCHER | IT_LIGHTNING )
#define MI_WPN3 (IT_GRENADE_LAUNCHER | IT_ROCKET_LAUNCHER | IT_LIGHTNING)

typedef struct mi_levels_s {
	int			items;
	const char  *desc;
} mi_levels_t;

mi_levels_t mi_levels[] = {
	{ 0, "Receiving extra infos: \xcf\xe6\xe6" },
	{ MI_POW | MI_ARM | IT_SUPERHEALTH | IT_ROCKET_LAUNCHER, "Receiving powerups\217armors\217mh\217rl" },
	{ MI_POW | MI_ARM | IT_SUPERHEALTH | MI_WPN3, "Receiving powerups\217armors\217mh\217rl\217gl\217lg" },
	{ MI_POW | MI_ARM | IT_SUPERHEALTH | MI_WPN, "Receiving powerups\217armors\217mh\217weapons"},
	{ MI_POW, "Receiving only powerups" }
};

int mi_levels_cnt = sizeof( mi_levels ) / sizeof( mi_levels[0] );

void mi_print( gedict_t *tooker, int it, char *msg )
{
	char *t_team;
	gedict_t *p;
	int level;
	qbool adm = mi_adm_only ();

	if ( !mi_on() )
		return; // spec info is turned off

	t_team = getteam( tooker );

	for( p = world; (p = find_spc ( p )); ) {
		if ( adm && !is_adm( p ) )
			continue; // configured send only for admins

		level = iKey(p, "mi"); // get spec setup

		if (level < 0)
			level = 0;
		if (level > mi_levels_cnt-1 )
			level = 0;

		if ( !(it & mi_levels[level].items) )
			continue;

		if ( isTeam() || isCTF() )
			G_sprint(p, 2, "\204\220%4.4s\221 %s\n", t_team, msg);
		else
			G_sprint(p, 2, "%s\n", msg);
	}
}

void moreinfo()
{
	int level;

	if ( !mi_on() ) {
		G_sprint(self, 2, "Spec info is turned off by server\n");
		return;
	}

	level = iKey(self, "mi") + 1;

	if ( level > mi_levels_cnt - 1 )
		level = 0;
	if ( level < 0 )
		level = 0;

	SetUserInfo( self, "mi", va("%d", level), 0 );
}

void info_mi_update( gedict_t *p, char *from, char *to )
{
	int level  = atoi(to);
	int olevel = atoi(from);

	if ( !mi_on() )
		return;

	if ( level == olevel )
		return;

	if ( level > mi_levels_cnt - 1 )
		level = 0;
	if ( level < 0 )
		level = 0;

	G_sprint(p, 2, "%s\n", mi_levels[level].desc);
}

void infolock ( )
{
	int k_spec_info = cvar("k_spec_info");

	if ( match_in_progress ) 
		return;

	if ( !is_adm( self ) ) {
		G_sprint(self, 2, "You are not an admin\n");
		return;
	}

	k_spec_info ^= MI_ADM_ONLY;
	cvar_fset("k_spec_info", k_spec_info);

	if ( mi_adm_only() ) 
		G_bprint(2, "Only %s can receive specinfos\n", redtext("admins"));
	else 
		G_bprint(2, "All %s can receive specinfos\n", redtext("spectators"));
}

void infospec ( )
{
	int k_spec_info = cvar("k_spec_info");

	if ( match_in_progress )
		return;

	k_spec_info ^= MI_ON;
	cvar_fset("k_spec_info", k_spec_info);

	G_bprint(2, "Extra info for spectators %s\n", redtext(OnOff(mi_on())));
}

// }

// { wreg

void wreg_usage()
{
	G_sprint(self, 2, "usage: cmd wreg [[char] [[+/-]weapon order]]\n");
}

void wreg_showslot(	wreg_t *w, int slot )
{
	int i;
	char *sign, order[MAX_WREG_IMP+1];

	if ( !w->init ) {
		G_sprint(self, 2, "slot \"%c\" - unregistered\n", (char)slot);
		return;
	}

	sign = "";
	if (w->attack > 0 )
		sign = "+";
	else if (w->attack < 0 )
		sign = "-";

	for ( order[0] = i = 0; i < MAX_WREG_IMP && w->impulse[i]; i++ )
		order[i] = '0' + w->impulse[i];
	order[i] = 0;

	G_sprint(self, 2, "slot \"%c\" - \"%s%s\"\n", (char)slot, sign, order);
}

void cmd_wreg()
{
	int		argc = trap_CmdArgc(), attack = 0, imp[MAX_WREG_IMP], i, cnt;
	char	arg_1[64], arg_2[64], *tmp = arg_2;
	byte    c;
	wreg_t  *w;

	if ( !self->wreg )
		return;

	if ( argc == 1 ) {
		qbool found = false;

		G_sprint(self, 2, "list of registered weapons:\n");

		for ( i = 0; i < MAX_WREGS; i++ ) {
			w = &(self->wreg[i]);
			if ( !w->init )
				continue;

			found = true;
			wreg_showslot( w, i );
		}

		if ( !found )
			G_sprint(self, 2, "none\n");

		return;
	}

	trap_CmdArgv( 1, arg_1, sizeof( arg_1 ) );

	if ( strnull(arg_1) ) {
		wreg_usage();
		G_sprint(self, 2, "empty char\n");
		return;
	}

	if ( strlen(arg_1) > 1 ) {
		wreg_usage();
		G_sprint(self, 2, "char can be only one byte\n");
		return;
	}

	c = arg_1[0];

	if ( c == 0 || c > 175 || c > MAX_WREGS-2 ) { // zzz, MAX_WREGS-2 so gcc is silent even that wrong
		wreg_usage();
		G_sprint(self, 2, "\"%c\" - illegal char!\n", (char)c);
		return;
	}

	w = &(self->wreg[c]);

	if ( argc == 2 ) {
		wreg_showslot( w, c );
		return;
	}

	if ( argc != 3 ) {
		wreg_usage();
		return; // something wrong
	}

	trap_CmdArgv( 2, arg_2, sizeof( arg_2 ) );

	if ( strnull(arg_2) ) {
		if ( w->init ) {
			memset( w, 0, sizeof( wreg_t ) ); // clear
			w->init = false;
			G_sprint(self, 2, "slot \"%c\" - unregistered\n", (char)c);
		}
		else {
			wreg_usage();
			G_sprint(self, 2, "empty weapon order\n");
		}
		return;
	}

	for ( cnt = i = 0; i < MAX_WREGS; i++ ) {
		if ( !(self->wreg[i].init) )
			continue;

		if ( ++cnt >= 20 ) {
			G_sprint(self, 2, "too many wregs, discard registration\n");
			return;
		}
	}

	if ( strlen(arg_2) > 10 ) { // 10 == strlen("+987654321")
		wreg_usage();
		G_sprint(self, 2, "too long weapon order\n");
		return;
	}

	if ( tmp[0] == '+' ) {
		tmp++;
		attack = 1;
	}
	else if ( tmp[0] == '-' ) {
		tmp++;
		attack = -1;
	}

	if ( !strnull( tmp ) && !only_digits( tmp ) ) {
		wreg_usage();
		G_sprint(self, 2, "illegal character in weapon order\n");
		return;
	}

	for ( i = 0; i < MAX_WREG_IMP && !strnull(tmp); tmp++ ) {
		if ( tmp[0] == '0' ) // do not confuse with '\0'
			continue;

		imp[i] = tmp[0] - '0';
		i++;
	}

	// ok we parse wreg command, and all ok, init it
	memset( w, 0, sizeof( wreg_t ) ); // clear

	w->init   = true;
	w->attack = attack;

	for ( i--; i >= 0 && i < MAX_WREG_IMP; i-- )
		w->impulse[i] = imp[i];

	G_sprint(self, 2, "slot \"%c\" - registered\n", (char)c);
}

void cmd_wreg_do( byte c )
{
	qbool warn;
	int j;
	wreg_t *w;

	if ( !self->wreg || c > MAX_WREGS-2 ) // zzz MAX_WREGS-2 so gcc happy, even that wrong
		return;

	w = &(self->wreg[c]);

	if ( !w->init ) {
		G_sprint(self, 2, "unregistered wreg char - \"%c\"\n", (char)c);
		return;
	}

//	G_sprint(self, 2, "wreg char - %c, i - %d %d %d\n", (char)c, w->impulse[0], w->impulse[1], w->impulse[2]);

	if ( w->attack > 0 ) {
		self->wreg_attack = 1;

		if( self->ct == ctSpec )
			stuffcmd_flags(self, STUFFCMD_IGNOREINDEMO, "+attack\n");
	}
	else if ( w->attack < 0 ) {
		self->wreg_attack = 0;

		if( self->ct == ctSpec )
			stuffcmd_flags(self, STUFFCMD_IGNOREINDEMO, "-attack\n");
	}

	if( self->ct == ctSpec )
		return;

	for ( j = 0; j < MAX_WREG_IMP && w->impulse[j]; j++ ) {

		if ( j + 1 >= MAX_WREG_IMP || !w->impulse[j + 1] )
			warn = true; // warn about no weapon or ammo if this last impulse in array
		else
			warn = false;

		if ( W_CanSwitch( w->impulse[j], warn ) ) {
			self->s.v.impulse = w->impulse[j];
			return;
		}
	}
}

// }

void ToggleMidair()
{
	if ( !is_rules_change_allowed() )
		return;

	// Can't enable midair unless dmm4 is set first
	if ( !cvar("k_midair") && deathmatch != 4 ) {
		G_sprint( self, 2, "Midair requires dmm4\n");
		return;
	}

	// If midair is enabled, disable instagib
	if (cvar("k_instagib"))
		cvar_set("k_instagib", "0");
	if ( cvar(LGCMODE_VARIABLE) )
		cvar_set(LGCMODE_VARIABLE, "0");

	if ( cvar("k_dmm4_gren_mode") )
		cvar_set("k_dmm4_gren_mode", "0"); // If midair is enabled, disable gren_mode

	cvar_toggle_msg( self, "k_midair", redtext("Midair") );
}

void SetMidairMinHeight()
{
	int k_midair_minheight = bound(0, cvar( "k_midair_minheight" ), 4); 

	if ( !is_rules_change_allowed() )
		return;

	// Can't set minheight if midair is not turned on 
	if ( !cvar("k_midair") ) {
		G_sprint( self, 2, "Midair must be turned on to set minimal frag height\n");
		return;
	}

	if ( ++k_midair_minheight > 4 )
		k_midair_minheight = 0;

	cvar_fset("k_midair_minheight", k_midair_minheight);

	if ( k_midair_minheight == 1 )
		G_bprint(2, "Midair minimum height set to %s enabled level\n", redtext("bronze"));
	else if ( k_midair_minheight == 2 )
		G_bprint(2, "Midair minimum height set to %s enabled level\n", redtext("silver"));
	else if ( k_midair_minheight == 3 )
		G_bprint(2, "Midair minimum height set to %s enabled level\n", redtext("gold"));
	else if ( k_midair_minheight == 4 )
		G_bprint(2, "Midair minimum height set to %s enabled level\n", redtext("platinum"));
	else
		G_bprint(2, "Midair minimum height set to %s enabled level\n", redtext("ground"));
}


void W_SetCurrentAmmo();
void ToggleInstagib()
{
	int k_instagib = bound(0, cvar( "k_instagib" ), 3); 
	char buf[1024*4];
	char *cfg_name;

	if ( !is_rules_change_allowed() )
		return;

	// Can't enable instagib unless dmm4 is set first
	if ( !cvar("k_midair") && deathmatch != 4 )
	{
		G_sprint( self, 2, "Instagib requires dmm4\n");
		return;
	}

	cfg_name = va("configs/usermodes/instagib/default.cfg");
	if ( can_exec( cfg_name ) )
	{
		trap_readcmd( va("exec %s\n", cfg_name), buf, sizeof(buf) );
		G_cprint("%s", buf);
	}

	cfg_name = va("configs/usermodes/instagib/%s.cfg", g_globalvars.mapname);
	if ( can_exec( cfg_name ) )
	{
		trap_readcmd( va("exec %s\n", cfg_name), buf, sizeof(buf) );
		G_cprint("%s", buf);
	}

	G_cprint("\n");

	if ( cvar("k_midair") )
		cvar_set("k_midair", "0"); // If instagib is enabled, disable midair
	if ( cvar(LGCMODE_VARIABLE) )
		cvar_set(LGCMODE_VARIABLE, "0");

	if ( cvar("k_dmm4_gren_mode") )
		cvar_set("k_dmm4_gren_mode", "0"); // If instagib is enabled, disable gren_mode

	if ( k_instagib == 0 )
		cvar_fset("dmm4_invinc_time", 1.0f); // default invic respawn time is 1s in instagib 

	if ( ++k_instagib > 3 )
		k_instagib = 0;

	cvar_fset("k_instagib", k_instagib);

	if ( !k_instagib )
	{
		G_bprint(2, "%s disabled\n", redtext("Instagib"));
	}
	else if ( k_instagib == 1 )
	{
		if ( cvar("k_instagib_custom_models") )
			G_bprint(2, "%s enabled (slow coilgun mode)\n", redtext("Instagib"));
		else
			G_bprint(2, "%s enabled (slow mode)\n", redtext("Instagib"));
	}
	else if ( k_instagib == 2 )
	{
		if ( cvar("k_instagib_custom_models") )
			G_bprint(2, "%s enabled (fast coilgun mode)\n", redtext("Instagib"));
		else
			G_bprint(2, "%s enabled (fast mode)\n", redtext("Instagib"));
	}
	else if ( k_instagib == 3 )
	{
		if ( cvar("k_instagib_custom_models") )
			G_bprint(2, "%s enabled (extreme coilgun mode)\n", redtext("Instagib"));
		else
			G_bprint(2, "%s enabled (extreme mode)\n", redtext("Instagib"));
	}
	else
	{
		G_bprint(2, "%s unknown\n", redtext("Instagib"));
	}

	if ( k_instagib )
		cvar_set("k_cg_kb", "1");

	W_SetCurrentAmmo();
}

void ToggleLGC(void)
{
	qbool k_lgc = cvar(LGCMODE_VARIABLE) != 0;

	if (!is_rules_change_allowed()) {
		return;
	}

	// Can't enable instagib unless dmm4 is set first
	if (!k_lgc && deathmatch != 4) {
		G_sprint(self, 2, "LGC mode requires dmm4\n");
		return;
	}

	// Disable incompatible modes
	if (cvar("k_midair")) {
		cvar_set("k_midair", "0");
	}
	if (cvar("k_instagib")) {
		cvar_set("k_instagib", "0");
	}

	cvar_set(LGCMODE_VARIABLE, k_lgc ? "1" : "0");

	cvar_toggle_msg( self, LGCMODE_VARIABLE, redtext("LGC mode") );

	W_SetCurrentAmmo();
}

void ToggleCGKickback()
{
	if ( match_in_progress )
		return;

	if ( !cvar("k_instagib") ) {
		G_sprint( self, 2, "cg_kb requires Instagib\n");
		return;
	}

	cvar_toggle_msg( self, "k_cg_kb", redtext("Coilgun kickback") );
}

void sv_time()
{
	char date[64];

	if ( QVMstrftime(date, sizeof(date), "%a %b %d, %H:%M:%S %Y", 0) )
		G_sprint(self, 2, "%s\n", date);
}

void GrenadeMode()
{
	if ( !is_rules_change_allowed() )
		return;

	// Can't toggle unless dmm4 is set first
	if ( deathmatch != 4 )
	{
		G_sprint( self, 2, "gren_mode requires dmm4\n");
		return;
	}

	cvar_toggle_msg( self, "k_dmm4_gren_mode", redtext("grenade mode") );

	if (cvar("k_dmm4_gren_mode"))
	{
		// disallow any weapon except gl
		trap_cvar_set_float( "k_disallow_weapons", DA_WPNS & ~IT_GRENADE_LAUNCHER );
	}
}

void ToggleReady()
{
	if ( isRACE() )
	{
		r_changestatus( 3 ); // race_toggle
		return;
	}

	if ( self->ready )
		PlayerBreak();
	else 
		PlayerReady();
}

void dlist()
{
	stuffcmd_flags(self, STUFFCMD_IGNOREINDEMO, "cmd demolist %s\n", params_str(1, -1));
}

void dinfo()
{
	stuffcmd_flags(self, STUFFCMD_IGNOREINDEMO, "cmd demoinfo %s\n", params_str(1, -1));
}

// ktpro (c)
void teleteam ()
{
	int k_tp_tele_death = bound(0, cvar("k_tp_tele_death"), 1);

	if ( match_in_progress )
		return;

	if ( ( k_tp_tele_death = (k_tp_tele_death ? 0 : 1) ) ) 
		G_bprint(2, "%s turn teamtelefrag %s\n", self->netname, redtext("affects frags"));
	else
		G_bprint(2, "%s turn teamtelefrag does %s\n", self->netname, redtext("not affect frags"));

	cvar_fset("k_tp_tele_death", k_tp_tele_death);
}

// ktpro (c)
void ChangeClientsCount( int type, int value )
{
	char *sv_max = "maxclients", *k_max = "k_maxclients";
	int cl_count = 0;

	if ( match_in_progress )
		return;

	if ( !check_perm(self, cvar("k_allowcountchange")) )
		return;

	type = bound(1, type, 2); // 1 - players, 2 - specs
	if ( type == 2 ) {
		sv_max = "maxspectators";
		k_max  = "k_maxspectators";
	}

	if ( cvar( sv_max ) >= cvar( k_max ) && value > 0 ) {
		G_sprint(self, 2, "%s reached\n", redtext(sv_max));
		return;
	}

	cl_count = bound(1, cvar( sv_max ) + value, max(1, cvar( k_max )));

	if ( cvar( sv_max ) == cl_count ) // does't change
		return;

	cvar_fset(sv_max, cl_count);
	G_bprint(2, "%s set %s to %d\n", self->netname, redtext(sv_max), cl_count);
}

void upplayers ( float type )
{
	ChangeClientsCount( type, 1 );
}

void downplayers ( float type )
{
	ChangeClientsCount( type, -1 );
}

void iplist_one(gedict_t *s, gedict_t *p)
{
	G_sprint(s, 2, "%15.15s %s %-18.18s\n", cl_ip( p ), is_adm( p ) ? "A" : " ", p->netname);
}

// ktpro (c)
void iplist ()
{
	int i;
	gedict_t *p;

	if ( !check_perm(self, cvar("k_ip_list")) ) {
		G_sprint(self, 2, "%s %s\n", redtext("Your IP is:"), cl_ip(self));
		return;
	}

	for( i = 0, p = world; (p = find_plr( p )); ) {
		if ( !i )
			G_sprint(self, 2, "\x9xIPs list\x9x %s\n", redtext("players:"));
		iplist_one(self, p);
		i++;
	}

	for( i = 0, p = world; (p = find_spc( p )); ) {
		if ( !i )
			G_sprint(self, 2, "\x9xIPs list\x9x %s\n", redtext("spectators:"));
		iplist_one(self, p);
		i++;
	}
}

void dmgfrags ()
{
	if ( !is_rules_change_allowed() )
		return;

	cvar_toggle_msg( self, "k_dmgfrags", redtext("damage frags") );
}

// { movie, for trix record in memory
// code is partially wroten by Tonik

void mv_stop_record ();
qbool mv_is_recording ();

qbool mv_is_playback ()
{
	return self->is_playback;
}

void mv_stop_playback ()
{
	if ( !mv_is_playback() )
		return;

	if ( self->pb_ent ) {
		ent_remove( self->pb_ent );
		self->pb_ent = NULL;
	}

	G_sprint(self, 2, "playback finished\n");
	self->pb_frame = 0;
	self->is_playback = false;
}

qbool mv_can_playback ()
{
	if ( match_in_progress || intermission_running )
		return false;

	if ( mv_is_recording() )
		return false; // sanity

	if ( self->pb_frame >= self->rec_count || self->pb_frame < 0 )
		return false;

	return true;
}

void mv_playback ()
{
	gedict_t *pb_ent = self->pb_ent;
	float scale;
	int s, i;
	plrfrm_t *ftmp, *fp;

	if ( !mv_is_playback() )
		return;

	if ( !pb_ent || !mv_can_playback() || self->pb_frame == self->rec_count - 1 ) {
		mv_stop_playback();
		return;
	}

	scale = ( (s = bound(0, iKey(self, "pbspeed"), 200) ) ? s / 100.0f : 1.0f );

	self->pb_time += (g_globalvars.time - self->pb_old_time) * scale;
	self->pb_old_time = g_globalvars.time;

	fp = ftmp = &(self->plrfrms[self->pb_frame]);

	for( i = self->pb_frame + 1; i < self->rec_count; i++ ) {
		ftmp = &(self->plrfrms[i]);
		if ( ftmp->time > self->pb_time )
			break;
		fp = ftmp;
	}

	i = fp - self->plrfrms;

	if ( i == self->pb_frame || fp->time > self->pb_time )
		return;

	self->pb_frame = i;

	setorigin( pb_ent, PASSVEC3( fp->origin ) );
	VectorCopy(fp->angles, pb_ent->s.v.angles);
	pb_ent->s.v.frame    = fp->frame;
	pb_ent->s.v.effects  = fp->effects;
	pb_ent->s.v.colormap = fp->colormap;
}

void mv_cmd_playback ()
{
	mv_stop_record();   // stop record first
	mv_stop_playback(); // stop playback first

	self->pb_frame = 0;

	if ( !mv_can_playback() ) {
		G_sprint(self, 2, "can't playback now\n");
		return;
	}

	G_sprint(self, 2, "playback\n");

	self->pb_ent = spawn ();
	self->pb_ent->classname = "pb_ent";
	setmodel (self->pb_ent, "progs/player.mdl");

	self->pb_time = 0;
	self->pb_old_time = g_globalvars.time;
	self->is_playback = true;
}

qbool mv_is_recording ()
{
	return self->is_recording;
}

void mv_stop_record ()
{
	if ( !mv_is_recording() )
		return;

	G_sprint(self, 2, "recording finished (%d) frames\n", self->rec_count);

	self->is_recording = false;
}

qbool mv_can_record ()
{
	if ( match_in_progress || intermission_running )
		return false;

	if ( mv_is_playback() )
		return false; // sanity

	if ( self->rec_count >= MAX_PLRFRMS || self->rec_count < 0 )
		return false;

	return true;
}

void mv_record ()
{
	plrfrm_t *f;

	if ( !mv_is_recording() )
		return;

	if ( !mv_can_record() ) {
		mv_stop_record();
		return;
	}

	f = &(self->plrfrms[self->rec_count]);

	f->time = g_globalvars.time - self->rec_start_time;
	VectorCopy(self->s.v.origin,  f->origin);
	VectorCopy(self->s.v.angles,  f->angles);
//	VectorCopy(self->s.v.v_angle, f->v_angle); // FIXME: usefull ?
	f->frame    = self->s.v.frame;
	f->effects  = self->s.v.effects;
	f->colormap = self->s.v.colormap;
//	f->s.v.modelindex = self.modelindex;

	self->rec_count++;
}

void mv_cmd_record ()
{
	mv_stop_record();   // stop record first
	mv_stop_playback(); // stop playback first

	self->rec_count = 0;

	if ( !mv_can_record() ) {
		G_sprint(self, 2, "can't record now\n");
		return;
	}

	G_sprint(self, 2, "recording\n");

	self->rec_start_time = g_globalvars.time;
	self->is_recording = true;
}

void mv_cmd_stop ()
{
	mv_stop_record();   // stop record
	mv_stop_playback(); // stop playback
}

// }

// ktpro (c)
// /cmd callalias <aliasname time>
void callalias ()
{
	const int ca_limit = 15, ca_limit2 = 30;
	char arg_x[1024];
	float tm;

	if ( trap_CmdArgc() != 3 ) {
		G_sprint(self, 2, "usage: cmd callalias <aliasname time>\n");
		return;
	}

	if ( self->connect_time + ca_limit < g_globalvars.time ) {
		G_sprint(self, 2, "you can use \"callalias\" only during %d sec after connect\n", ca_limit);
		return;
	}

	trap_CmdArgv( 2, arg_x, sizeof( arg_x ) );
	tm = fabsf( atof(arg_x) );

	if ( tm <= 0 || tm > ca_limit2 ) {
		G_sprint(self, 2, "calling time can't be longer than %d seconds\n", ca_limit2);
		return;
	}

	if ( self->callalias_time ) {
		G_sprint(self, 2, "you can't install more than 1 alias before previous will execute\n");
		return;
	}

	trap_CmdArgv( 1, arg_x, sizeof( arg_x ) );
	if ( strnull(arg_x) ) {
		G_sprint(self, 2, "you can't install an alias with an empty name\n");
		return;
	}

	G_sprint(self, 2, "installing %s alias (%.1f)\n", arg_x, tm);

	strlcpy(self->callalias, arg_x, CALLALIAS_SIZE);
	self->callalias_time = g_globalvars.time + tm;
}

void check_callalias ()
{
	if ( !self->callalias_time || self->callalias_time > g_globalvars.time )
		return;

	stuffcmd_flags(self, STUFFCMD_IGNOREINDEMO, "%s\n", self->callalias);
	self->callalias_time = 0;
}

// ktpro (c)
// /cmd check <f_query>

char fcheck_name[128];

void fcheck ()
{
	char arg_x[1024];
	int i;

	if ( match_in_progress )
		return;

	if ( trap_CmdArgc() != 2 ) {
		G_sprint(self, 2, "usage: cmd check <f_query>\n"
			"for example: cmd check f_version\n");
		return;
	}

	if ( f_check ) {
		G_sprint(self, 2, "Waiting from previous reply\n");
		return;
	}

	trap_CmdArgv( 1, arg_x, sizeof( arg_x ) );

	if ( !is_real_adm( self ) ) {
		if ( strneq(arg_x, "f_version") && strneq(arg_x, "f_modified") && strneq(arg_x,  "f_server") ) {
			G_sprint(self, 2, "You are not allowed to check \020%s\021\n"
				"available checks are: f_version, f_modified and f_server\n", arg_x);
			return;
		}
	}

	for ( i = 1; i <= MAX_CLIENTS; i++ )
		if ( g_edicts[i].f_checkbuf )
			g_edicts[i].f_checkbuf[0] = 0; // clear bufs

	f_check = g_globalvars.time + 3;
	strlcpy(fcheck_name, arg_x, sizeof(fcheck_name)); // remember check name

	G_bprint(2, "%s is checking \020%s\021\n", self->netname, arg_x);
	if ( streq(arg_x, "f_version") || streq(arg_x, "f_modified") )
		G_bprint(3, "%s: %s %d%d\n", self->netname, arg_x, i_rnd(1, 9999), i_rnd(0, 9999));
	else
		G_bprint(3, "%s: %s\n", self->netname, arg_x);
}

void check_fcheck ()
{
	gedict_t *p;
	char *nl, *tmp;

	if ( !f_check || f_check > g_globalvars.time )
		return;

	G_bprint(2, "player's \020%s\021 replies:\n", fcheck_name);

	for( p = world; (p = find_plr( p )); ) {
		if ( strnull( tmp = p->f_checkbuf ) ) {
			G_bprint(3, "%s did not reply!\n", p->netname);
			continue;
		}

		while ( !strnull( tmp ) ) {
			if ( ( nl = strchr(tmp, '\n') ) )
				nl[0] = 0;
			G_bprint(3, "%s: %s\n", p->netname, tmp);

			tmp = (nl ? nl+1: NULL);
		}
	}

	G_bprint(2, "end of player's \020%s\021 replies\n", fcheck_name);

	f_check = 0;
}

void mapcycle ()
{
	char var[128], *newmap = "";
	int i;

	for ( i = 0; i < 999; i++ ) {
		snprintf(var, sizeof(var), "k_ml_%d", i);
		if ( strnull( newmap = cvar_string( var ) ) )
			break;

		if ( !i )
			G_sprint(self, 2, 	"%s:\n"
				"%3.3s | %s\n", redtext("Map cycle"), redtext("id"), redtext("name"));

		G_sprint(self, 2, "%3.3d | %s%s\n", i + 1, newmap, streq(newmap, g_globalvars.mapname) ? " \x8D current" : "");
	}

	if ( !i ) {
		G_sprint(self, 2, 	"\n%s: %s\n", redtext("Map cycle"), redtext("empty"));
		return;
	}

	if ( trap_cvar( "samelevel" ) )
		G_sprint(self, 2, 	"\n%s: %s\n", redtext("Map cycle"), redtext("not active"));
}

void airstep()
{
	if ( match_in_progress || isRACE() )
		return;

	cvar_toggle_msg( self, "pm_airstep", redtext("pm_airstep") );
}

void ToggleVwep()
{
	gedict_t *p, *oself;

	if ( match_in_progress )
		return;

	if (!vw_available || !cvar("k_allow_vwep"))
		return;

	cvar_toggle_msg( self, "k_vwep", redtext("vwep") );
	vw_enabled = vw_available && cvar("k_allow_vwep") && cvar("k_vwep");

	oself = self;
	for( p = world; (p = find_client( p )); )
		if ( p->ct == ctPlayer ) {
			self = p;
			W_SetCurrentAmmo();
		}
	self = oself;
}

void ToggleExclusive()
{
	if ( match_in_progress )
		return;

	cvar_toggle_msg( self, "k_exclusive", redtext("exclusive mode") );
}

void ToggleNewCoopNm()
{
	if ( match_in_progress )
		return;

	cvar_toggle_msg( self, "k_nightmare_pu", redtext("New Nightmare mode (drops powerups)") );
}

// { yawn mode related
// { yawn mode related

void FixYawnMode()
{
	k_yawnmode		= cvar( "k_yawnmode");
	k_teleport_cap 	= bound( 0, cvar( "k_teleport_cap" ),  100 );
}

// Toggle yawnmode, implemented by Molgrum
void ToggleYawnMode()
{
	if ( !is_rules_change_allowed() )
		return;

	cvar_toggle_msg( self, "k_yawnmode", redtext("yawnmode") );

	FixYawnMode(); // apply changes ASAP
}

void setTeleportCap()
{
	char arg[256];

	if ( !k_yawnmode )
	{
		G_sprint( self, 2, "%s required to be on\n", redtext("Yawn mode") );
		return;
	}

	if ( match_in_progress || trap_CmdArgc() < 1 )
	{
		G_sprint( self, 2, "%s is %d%%\n", redtext("Teleport cap"), k_teleport_cap );
		return;
	}

	trap_CmdArgv( 1, arg, sizeof( arg ) );
	k_teleport_cap = atoi( arg ); // get user input
	k_teleport_cap = bound( 0, k_teleport_cap, 100 ); // bound
	cvar_fset( "k_teleport_cap", k_teleport_cap ); // set

	FixYawnMode(); // apply changes ASAP

	G_bprint( 2, "%s set %s to %d%%\n", self->netname, redtext("Teleport cap"), k_teleport_cap ); // and print
}

// }

int when_to_unpause;
int pauseduration;

void PausedTic( int duration )
{
	pauseduration = duration;

	// Unpause on schedule, or if the game has ended for some reason
	if ((!k_matchLess && match_in_progress != 2) || (when_to_unpause && duration >= when_to_unpause))
	{
		when_to_unpause = pauseduration = 0; // reset our globals

		G_bprint (2, "game unpaused\n");
		trap_setpause (0);
	}
}

void TogglePause()
{
	if (FTE_sv)
		return; // unsupported

	if ( !k_matchLess )
	{ // NON matchless
		if ( match_in_progress != 2 )
			return; // apply TogglePause only during actual game
	}

	if ( (int)cvar("sv_paused") & 1 )
	{
		// UNPAUSE

		// pause release is not applied immediately, but after a countdown
		if ( when_to_unpause )
		{  // unpause is pending alredy
			int sec = max(0, (when_to_unpause - pauseduration) / 1000);

			G_sprint(self, 2, "Unpause is pending, %d second%s\n", sec, count_s(sec));
			return;
		}

		when_to_unpause = pauseduration + 2000; // schedule unpause in 2000 ms

		G_bprint(2, "%s unpaused the game (will resume in 2 seconds)\n", self->netname);
	}
	else
	{
		// PAUSE

		// admins may ignore not allowed pause
		if( !cvar( "pausable" ) && !is_adm(self) && !PlayerCanPause(self))
		{
			G_sprint(self, 2, "Pause is not allowed\n");
			return;
		}

		pauseduration = when_to_unpause = 0; // reset our globals

		G_bprint(2, "%s paused the game. He has %d remaining request(s).\n", self->netname, self->k_pauseRequests);
		trap_setpause (1);
	}
}

void ToggleArena()
{
	if ( !is_rules_change_allowed() )
		return;

	if ( !isRA() )
	{
		// seems we trying turn RA on.
		if ( !isDuel() )
		{
			G_sprint(self, 2, "Set %s mode first\n", redtext("\223 on \223"));
			return;
		}
	}

	cvar_toggle_msg( self, "k_rocketarena", redtext("Rocket Arena") );

	if ( isRA() )
	{
		char buf[1024*4];
		char *cfg_name;

		char *um = "1on1";

		cfg_name = va("configs/usermodes/%s/ra/default.cfg", um);
		if ( can_exec( cfg_name ) )
		{
			trap_readcmd( va("exec %s\n", cfg_name), buf, sizeof(buf) );
			G_cprint("%s", buf);
		}

		cfg_name = va("configs/usermodes/%s/ra/%s.cfg", um, g_globalvars.mapname);
		if ( can_exec( cfg_name ) )
		{
			trap_readcmd( va("exec %s\n", cfg_name), buf, sizeof(buf) );
			G_cprint("%s", buf);
		}

		G_cprint("\n");

		// avoid spawn bug with safe spawn mode
		cvar_fset( "k_spw", 1);
	}
}

void Spawn666Time()
{
	char arg_2[1024];
	float dmm4_invinc_time;

	if ( deathmatch != 4 )
	{
		G_sprint(self, 2, "command allowed in %s only\n", redtext("dmm4"));
		return;
	}

	// no arguments, show info and return
	if ( match_in_progress || trap_CmdArgc() == 1 )
	{
		dmm4_invinc_time = cvar("dmm4_invinc_time");
		dmm4_invinc_time = dmm4_invinc_time ? bound(0, dmm4_invinc_time, DMM4_INVINCIBLE_MAX) : DMM4_INVINCIBLE_DEFAULT;

		G_sprint(self, 2, "%s is %.1fs\n", redtext("spawn invincibility time"), dmm4_invinc_time);
		return;
	}

	trap_CmdArgv( 1, arg_2, sizeof( arg_2 ) );

	dmm4_invinc_time = bound(0, atof( arg_2 ), DMM4_INVINCIBLE_DEFAULT);

	G_bprint(2, "%s set %s to %.1fs\n", self->netname, redtext("spawn invincibility time"), dmm4_invinc_time);

	// to actualy disable dmm4_invinc_time we need set it to negative value
	trap_cvar_set_float( "dmm4_invinc_time", dmm4_invinc_time ? dmm4_invinc_time : -1 );
}

void noitems()
{
	if ( match_in_progress )
		return;

	cvar_toggle_msg( self, "k_noitems", redtext("noitems mode") );
}

void giveme_usage(void)
{
	G_sprint(self, 2, "giveme <q|p|r|s> [seconds]\n"
		"giveme rune [1|2|3|4]\n"
		"giveme runes\n"
		"giveme norunes\n");
}

void giveme()
{
	char arg_2[128];
	char arg_3[128];
	char *got = "BUG";
	float seconds;

	if ( strnull( ezinfokey(world, "*cheats") ) )
	{
		G_sprint(self, 2, "Cheats are disabled on this server, so use the force, Luke... err %s\n", self->netname);
		return; // FU!
	}

	// no arguments, show info and return
	if ( trap_CmdArgc() == 1 )
	{
		giveme_usage();
		return;
	}

	trap_CmdArgv( 1, arg_2, sizeof( arg_2 ) );
	trap_CmdArgv( 2, arg_3, sizeof( arg_3 ) );

	seconds = max(0, atof(arg_3));
	if (!seconds)
		seconds = 30;	

	if ( streq(arg_2, "q") )
	{
		self->super_time = 1;
		self->super_damage_finished = g_globalvars.time + seconds;
		self->s.v.items = (int)self->s.v.items | IT_QUAD;
		got = "quad";
	}
	else if ( streq(arg_2, "p") )
	{
		self->invincible_time = 1;
		self->invincible_finished = g_globalvars.time + seconds;
		self->s.v.items = (int)self->s.v.items | IT_INVULNERABILITY;
		got = "pent";
	}
	else if ( streq(arg_2, "r") )
	{
		self->invisible_time = 1;
		self->invisible_finished = g_globalvars.time + seconds;
		self->s.v.items = (int)self->s.v.items | IT_INVISIBILITY;
		got = "ring";
	}
	else if ( streq(arg_2, "s") )
	{
		self->rad_time = 1;
		self->radsuit_finished = g_globalvars.time + seconds;
		self->s.v.items = (int)self->s.v.items | IT_SUIT;
		got = "suit";
	}
	else if ( streq(arg_2, "rune")  )
	{
		int rune = bound( 0, seconds - 1, 3 );

		g_globalvars.serverflags = (int)g_globalvars.serverflags | ( 1 << rune );

		return;
	}
	else if ( streq(arg_2, "runes")  )
	{
		g_globalvars.serverflags = (int)g_globalvars.serverflags | 15;

		return;
	}
	else if ( streq(arg_2, "norunes")  )
	{
		g_globalvars.serverflags = (int)g_globalvars.serverflags & ~15;

		return;
	}
	else
	{
		giveme_usage();
		return;
	}

	G_sprint(self, 2, "You got %s for %.1fs\n", got, seconds );
}

qbool is_rules_change_allowed( void )
{
	if ( match_in_progress )
	{
		G_sprint( self, 2, "Command is locked while %s is in progress\n", redtext( "match" ) );
		return false;
	}

	if ( isRACE() )
	{
		G_sprint( self, 2, "%s is on, please toggle it off by using %s command first\n", redtext( "race mode" ), redtext( "race" ) );
		return false;
	}

	return true;
}

typedef struct
{
	char           *name;
	char           *classname;
	int				spawnflags;
	int				angle;			// should we set angles or not.
	void            ( *spawn ) ();  // custom spawn function, called after actual spawn.
} dropitem_spawn_t;

#define  WEAPON_BIG2  1

static void dropitem_spawn_spawnpoint()
{
	int effects = EF_GREEN | EF_RED; // default effects.

	self->s.v.flags = ( int )self->s.v.flags | FL_ITEM;
	setmodel( self, "progs/w_g_key.mdl" );

	if ( streq( self->classname, "info_player_team1" ) )
		effects = EF_RED;
	else if ( streq( self->classname, "info_player_team2" ) )
		effects = EF_BLUE;

	self->s.v.effects = ( int ) self->s.v.effects | effects;
	setorigin( self, PASSVEC3( self->s.v.origin ) );
}

static dropitem_spawn_t dropitems[] = 
{
	{"h15",		"item_health",						H_ROTTEN},
	{"h25",		"item_health",						0},
	{"h100",	"item_health",						H_MEGA},
	{"ga",		"item_armor1",						0},
	{"ya",		"item_armor2",						0},
	{"ra",		"item_armorInv",					0},
	{"ssg",		"weapon_supershotgun",				0},
	{"ng",		"weapon_nailgun",					0},
	{"sng",		"weapon_supernailgun",				0},
	{"gl",		"weapon_grenadelauncher",			0},
	{"rl",		"weapon_rocketlauncher",			0},
	{"lg",		"weapon_lightning",					0},
	{"sh20",	"item_shells",						0},
	{"sh40",	"item_shells",						WEAPON_BIG2},
	{"sp25",	"item_spikes",						0},
	{"sp50",	"item_spikes",						WEAPON_BIG2},
	{"ro5",		"item_rockets",						0},
	{"ro10",	"item_rockets",						WEAPON_BIG2},
	{"ce6",		"item_cells",						0},
	{"ce12",	"item_cells",						WEAPON_BIG2},
	{"p",		"item_artifact_invulnerability",	0},
	{"s",		"item_artifact_envirosuit",			0},
	{"r",		"item_artifact_invisibility",		0},
	{"q",		"item_artifact_super_damage",		0},
	{"fl_r",	"item_flag_team1",					0,					1},
	{"fl_b",	"item_flag_team2",					0,					1},
	{"sp_r",	"info_player_team1",				0,					1, dropitem_spawn_spawnpoint},
	{"sp_b",	"info_player_team2",				0,					1, dropitem_spawn_spawnpoint},
};

static const int dropitems_count = sizeof(dropitems) / sizeof(dropitems[0]);

static dropitem_spawn_t * dropitem_find_by_name(const char * name)
{
	int i;

	for ( i = 0; i < dropitems_count; i++ )
	{
		if ( streq(dropitems[i].name, name) )
			return &dropitems[i];
	}

	return NULL;
}

// spawn item.
static gedict_t * dropitem_spawn_item(gedict_t *spot, dropitem_spawn_t * di)
{
	extern qbool G_CallSpawn( gedict_t * ent );

	gedict_t *	oself;
	gedict_t *	p = spawn();

	p->dropitem = true;
	p->classname = di->classname;
	p->s.v.spawnflags = di->spawnflags;
	VectorCopy(spot->s.v.origin, p->s.v.origin);
//	VectorCopy(spot->s.v.angles, p->s.v.angles);
	if (di->angle)
		p->s.v.angles[1] = spot->s.v.angles[1]; // seems we should set angles for this entity.

	setorigin( p, PASSVEC3(p->s.v.origin) );

	// G_CallSpawn will change 'self', so we have to do trick about it.
	oself = self;	// save!!!

	if ( !G_CallSpawn( p ) || strnull( p->classname ) )
	{
		// failed to call spawn function, so remove it ASAP.
		ent_remove( p );
		p = NULL;
	}
	else if ( di->spawn )
	{
		di->spawn(); // call custom spawn function if we succeed with main spawn function.
	}

	self = oself;	// restore!!!

	return p;
}

static void dropitem_usage(void)
{
	int i;
	char tmp[1024] = {0};

// dropitem < x | y >

	for ( i = 0; i < dropitems_count; i++ )
	{
		if ( !(i % 3) && *tmp )
		{
			G_sprint(self, 2, "dropitem < %s >\n", tmp);		
			*tmp = 0;
		}

		if ( *tmp )
			strlcat(tmp, " | ", sizeof(tmp));
		strlcat(tmp, dropitems[i].name, sizeof(tmp));
	}

	if ( *tmp )
		G_sprint(self, 2, "dropitem < %s >\n", tmp);
}

static void dropitem()
{
	dropitem_spawn_t * di;
	char arg_1[128];

	if ( match_in_progress )
		return;

	if ( strnull( ezinfokey(world, "*cheats") ) )
	{
		G_sprint(self, 2, "Cheats are disabled on this server, so use the force, Luke... err %s\n", self->netname);
		return; // FU!
	}

	// no arguments, show info and return
	if ( trap_CmdArgc() < 2 )
	{
		dropitem_usage();
		return;
	}

	trap_CmdArgv( 1, arg_1, sizeof( arg_1 ) );

	if ( (di = dropitem_find_by_name( arg_1 )) )
	{
		if (dropitem_spawn_item( self, di ))
		{
			G_sprint(self, 2, "Spawned %s\n", di->classname );
		}
		else
		{
			G_sprint(self, 2, "Can't spawn %s\n", di->classname );
		}
	}
	else
	{
		dropitem_usage();
		return;
	}
}

static void removeitem()
{
	gedict_t *	ent;
	gedict_t *	p = NULL;
	float		best_distance = 10e+32;//should be FLT_MAX but it is absent of some systems.

	if ( match_in_progress )
		return;

	if ( strnull( ezinfokey(world, "*cheats") ) )
	{
		G_sprint(self, 2, "Cheats are disabled on this server, so use the force, Luke... err %s\n", self->netname);
		return; // FU!
	}

    for( ent = world; ( ent = nextent( ent ) ); )
	{
		float	distance = 0;
		int     j;

		if ( !ent->dropitem )
			continue; // not our item.

		for ( j = 0; j < 3; j++ )
		{
			float c = self->s.v.origin[j] - ( ent->s.v.origin[j] + ( ent->s.v.mins[j] + ent->s.v.maxs[j] ) * 0.5 );
			distance += c * c;
		}

		// check if that item closer to us.
		if ( distance > best_distance )
			continue;

		p = ent;
		best_distance = distance;
	}

	if ( p )
	{
		G_sprint(self, 2, "Removed %s\n", p->classname );
		ent_remove( p );
	}
	else
	{
		G_sprint(self, 2, "Nothing found around\n");	
	}
}

static void dump_print( fileHandle_t file_handle, const char *fmt, ... )
{
	va_list argptr;
	char	text[1024] = {0};

	if ( file_handle < 0 )
		return;
        
	va_start( argptr, fmt );
	Q_vsnprintf( text, sizeof(text), fmt, argptr );
	va_end( argptr );

	text[sizeof(text)-1] = 0;

	trap_FS_WriteFile( text, strlen(text), file_handle );
}

static void dumpent()
{
	int cnt = 0;
	gedict_t *p;
	fileHandle_t file_handle = -1;

	if ( match_in_progress )
		return;

	if ( strnull( ezinfokey(world, "*cheats") ) )
	{
		G_sprint(self, 2, "Cheats are disabled on this server, so use the force, Luke... err %s\n", self->netname);
		return;
	}

	if ( trap_FS_OpenFile( "dump.ent", &file_handle, FS_WRITE_BIN ) < 0 )
	{
		G_sprint(self, 2, "Can't open file for write\n");
		return;
	}

    for( p = world; ( p = nextent( p ) ); )
    {
		if ( !p->dropitem )
			continue; // not our item.

		if ( strnull( p->classname ) )
			continue; // null class name.

		dump_print(file_handle, "{\n");
		dump_print(file_handle, "\t" "\"classname\" \"%s\"" "\n", p->classname);
		dump_print(file_handle, "\t" "\"origin\" \"%d %d %d\"" "\n", (int)p->s.v.origin[0], (int)p->s.v.origin[1], (int)p->s.v.origin[2]);

		if ( p->s.v.angles[0] || p->s.v.angles[2] )
			dump_print(file_handle, "\t" "\"angles\" \"%d %d %d\"" "\n", (int)p->s.v.angles[0], (int)p->s.v.angles[1], (int)p->s.v.angles[2]);
		else if ( p->s.v.angles[1] )
			dump_print(file_handle, "\t" "\"angle\" \"%d\"" "\n", (int)p->s.v.angles[1]);

		if ( p->s.v.spawnflags )
			dump_print(file_handle, "\t" "\"spawnflags\" \"%d\"" "\n", (int)p->s.v.spawnflags);
		dump_print(file_handle, "}\n");

		cnt++;
    }

	trap_FS_CloseFile( file_handle );

	G_sprint(self, 2, "Dumped %d entities\n", cnt);
}

qbool lgc_enabled(void)
{
	return cvar(LGCMODE_VARIABLE) != 0;
}

void lgc_register_hit(vec3_t start, gedict_t* player, gedict_t* victim)
{
	if (victim && victim->ct == ctPlayer) {
		player->lgc_state = lgcNormal;

		if (isDuel()) {
			float distance = bound(0, VectorDistance(start, victim->s.v.origin), LGCMODE_MAX_DISTANCE - 1);
			int bucket = bound(0, (int)(distance / LGCMODE_BUCKET_DISTANCE), LGCMODE_DISTANCE_BUCKETS - 1);

			player->lgc_distance_hits[bucket]++;
		}
	}
	else {
		lgc_register_miss(start, player);
	}
}

void lgc_register_miss(vec3_t start, gedict_t* player)
{
	if (player->lgc_state == lgcUndershaft) {
		++player->ps.lgc_undershaft;
	}
	else if (player->lgc_state == lgcOvershaft) {
		++player->ps.lgc_overshaft;
	}

	if (isDuel()) {
		// Find opponent and register miss according to distance
		gedict_t* p;
		for (p = world; (p = find_plr(p)); ) {
			if (p != player) {
				float distance = bound(0, VectorDistance(start, p->s.v.origin), LGCMODE_MAX_DISTANCE - 1);
				int bucket = bound(0, (int)(distance / LGCMODE_BUCKET_DISTANCE), LGCMODE_DISTANCE_BUCKETS - 1);

				player->lgc_distance_misses[bucket]++;

				break;
			}
		}
	}
}

void lgc_register_kill(gedict_t* player)
{
	player->lgc_state = lgcOvershaft;
}

void lgc_register_fire_stop(gedict_t* player)
{
	player->lgc_state = lgcUndershaft;
}
