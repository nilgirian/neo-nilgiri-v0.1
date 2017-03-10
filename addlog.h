/* **************************************************************************
    Nilgiri the Forgotten World
    (C) 2004
   ************************************************************************** */
/** \file
    \brief logging helper header
 */
#ifndef _x_ADDLOG_INCLUDE_x_
#define _x_ADDLOG_INCLUDE_x_
#include"command.h"
#include"container.h"
#include"utils.h"

class Character;
class Player;

void addlog_offensive(Character *, Character *, cmd_t);
void addlog_death(Character *, Character *);
void addlog_create_player(Player *);
void _x_addlog_creation_x_(const char *, const char *, Character *);
void addlog_next_cid(cid_t);
void addlog_new_db(void);
void addlog_connect(unsigned long int);
void addlog_link_loss(Character *);
void addlog_sig_int(void);
void addlog_shutdown(void);
void addlog_host(const char *, unsigned long int);
void addlog_in_player(Character *);
void addlog_reconnect_player(Player *);
void addlog_shut_player(Character *);
void addlog_start(void);
void addlog_new_item(Item *, cid_t);
void addlog_new_character(Character *, cid_t);
void addlog_advance_imm(Character *, Character *, level_t);
void addlog_demote_imm(Character *, Character *, level_t);
void addlog_revoke_immcmd(Character *, Character *, const char *);
void addlog_grant_immcmd(Character *, Character *, const char *);
void addlog_revoke_bug(Character *, const char *, const char *);
void addlog_grant_bug(Character *, const char *, const char *);

inline void _x_addlog_creation_x_(const char *func, int n, Character *ch)
{
	_x_addlog_creation_x_(func, itoa(n), ch);
}

#define addlog_creation(item, ch) _x_addlog_creation_x_(__FUNCTION__, item, ch)

#endif
