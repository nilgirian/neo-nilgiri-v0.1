/* **************************************************************************
    Nilgiri the Forgotten World
    (C) 2004
   ************************************************************************** */
/** \file
    \brief logging helper source
 */
#include"err.h"
#include"tinyworld_character.h"
#include"descriptor.h"
#include"player.h"
#include"container.h"

#define LOG_OFFENSIVE			"/PLAYERS/player:%s/sessions/::$s:$S/::$U:$T:offensive %s v. CID#%08lX"
#define LOG_DEATH					"/PLAYERS/player:%s/sessions/::$s:$S/::$U:$T:death by CID#%08lX"
#define LOG_LINK_LOSS			"/PLAYERS/player:%s/sessions/::$s:$S/::$U:$T:link loss"
#define LOG_IN_PLAYER			"/PLAYERS/player:%s/sessions/::$s:$S/::$U:$T:enters-%s"
#define LOG_RECONNECT_PLAYER	"/PLAYERS/player:%s/sessions/::$s:$S/::$U:$T:reconnect-%s"
#define LOG_SHUT_PLAYER			"/PLAYERS/player:%s/sessions/::$s:$S/::$U:$T:shutout"
#define LOG_CREATE_PLAYER		"/PLAYERS/player:%s/CID#%08lX/session:$S/time:$T/IP:$I%08lX/Hostname:%s"
#define LOG_ADVANCE_IMM			"/PLAYERS/player:%s/levels/::$U:advanced player:%s/session:$S/time:$T/by player:%s"
#define LOG_DEMOTE_IMM			"/PLAYERS/player:%s/levels/::$U:demoted player:%s/session:$S/time:$T/by player:%s"
#define LOG_GRANT_IMM			"/PLAYERS/player:%s/commands/::$U:granted player:%s/session:$S/time:$T/by player:%s"
#define LOG_REVOKE_IMM			"/PLAYERS/player:%s/commands/::$U:revoked player:%s/session:$S/time:$T/by player:%s"
#define LOG_GRANT_BUG			"/PLAYERS/player:%s/bug permit/::$U:granted player:%s/session:$S/time:$T/by player:%s"
#define LOG_REVOKE_BUG			"/PLAYERS/player:%s/bug permit/::$U:revoked player:%s/session:$S/time:$T/by player:%s"

#define LOG_CREATION1		"/TINYWORLDB/%s/::$s:::$U:save %s/%s"
#define LOG_CREATION2		"/TINYWORLDB/%s/%s/::$s:::$U:save %s/%s"

#define LOG_NEW_ITEM			"/TINYWORLD/ITEM/VID#%u/CID#%08lX/created by CID#%08lX/$S/$T"

#define LOG_NEW_CHARACTER	"/TINYWORLD/CHARACTER/VID#%u/CID#%08lX/created by CID#%08lX/$S/$T"
#define LOG_DEATH_C			"/TINYWORLD/CHARACTER/VID#%u/CID#%08lX/death by CID#%08lX/$S/$T"

#define LOG_NEXT_CID			"/SESSIONS/::$s:$S/$T:next cid:%08lX"
#define LOG_NEW_DB			"/SESSIONS/::$s:$S/$T:new tinyworldB"
#define LOG_SIG_INT			"/SESSIONS/::$s:$S/::$U:$T:sigterm"
#define LOG_SHUTDOWN			"/SESSIONS/::$s:$S/::$U:$T:shutdown"
#define LOG_HOST				"/SESSIONS/::$s:$S/::$U:$T:host:%s port:%u"
#define LOG_START				"/SESSIONS/::$s:$S/::$U:$T:start"

#define LOG_CONNECT			"/CONNECT/$I%08lX/$S-$T"

extern const char *command[];
extern void addlog(const char *, ...);

void addlog_offensive(Character *ch, Character *vict, cmd_t cmd)
{
	addlog(LOG_OFFENSIVE, ch->getName(), command[cmd], vict->getCID());
}

void addlog_death(Character *ch, Character *killer)
{
	Player *player;

	if ((player = ch->objPlayer()))
	{
		addlog(LOG_DEATH, player->getName(), killer->getCID());
	}
	else
	{
		addlog(LOG_DEATH_C, ch->getVID(), ch->getCID(), killer->getCID());
	}
}

void addlog_create_player(Player *p)
{
	addlog(LOG_CREATE_PLAYER,
			p->getName(),
			p->getCID(),
			p->getDescriptor()->getIP(),
			p->getBirthHost());
}

void _x_addlog_creation_x_(const char *function, const char *id, Character *ch)
{
	addlog(LOG_CREATION1, function, id, ch->getName());
}

void addlog_creation(const char *function, const char *table, const char *id,
		Character *ch)
{
	addlog(LOG_CREATION2, function, table, id, ch->getName());
}


void addlog_next_cid(cid_t cid)
{
	addlog(LOG_NEXT_CID, cid);
}

void addlog_new_db(void)
{
	addlog(LOG_NEW_DB);
}

void addlog_connect(unsigned long int ip)
{
	addlog(LOG_CONNECT, ip);
}

void addlog_link_loss(Character *ch)
{
	addlog(LOG_LINK_LOSS, ch->getName());
}

void addlog_sig_int(void)
{
	addlog(LOG_SIG_INT);
}

void addlog_shutdown(void)
{
	addlog(LOG_SHUTDOWN);
}

void addlog_host(const char *hostname, unsigned long int port)
{
	addlog(LOG_HOST, hostname, port);
}

void addlog_in_player(Character *ch)
{
	addlog(LOG_IN_PLAYER, ch->getName(), ch->getDescriptor()->getHostname());
}

void addlog_reconnect_player(Player *ch)
{
	addlog(LOG_RECONNECT_PLAYER,
			ch->getName(),
			ch->getDescriptor()->getHostname());
}

void addlog_shut_player(Character *ch)
{
	if (ch)
	{
		addlog(LOG_SHUT_PLAYER, ch->getName());
	}
}

void addlog_start(void)
{
	addlog(LOG_START);
}

void addlog_new_item(Item *item, cid_t loader_cid)
{
	addlog(LOG_NEW_ITEM, item->getVID(), item->getCID(), loader_cid);
}

void addlog_new_character(Character *ch, cid_t loader_cid)
{
	addlog(LOG_NEW_CHARACTER, ch->getVID(), ch->getCID(), loader_cid);
}

void addlog_advance_imm(Character *vict, Character *ch, level_t level)
{
	addlog(LOG_ADVANCE_IMM,
			vict->getName(), 
			str_immortal_types_u[level],
			ch->getName());
}

void addlog_demote_imm(Character *vict, Character *ch, level_t level)
{
	addlog(LOG_DEMOTE_IMM,
			vict->getName(),
			str_immortal_types_u[level],
			ch->getName());
}

void addlog_grant_immcmd(Character *vict, Character *ch, const char *cmd)
{
	addlog(LOG_GRANT_IMM, vict->getName(), cmd, ch->getName());
}

void addlog_revoke_immcmd(Character *vict, Character *ch, const char *cmd)
{
	addlog(LOG_REVOKE_IMM, vict->getName(), cmd, ch->getName());
}

void addlog_grant_bug(Character *ch, const char *owner, const char *submitter)
{
	addlog(LOG_GRANT_BUG, owner, submitter, ch->getName());
}

void addlog_revoke_bug(Character *ch, const char *owner, const char *submitter)
{
	addlog(LOG_REVOKE_BUG, owner, submitter, ch->getName());
}
