/* **************************************************************************
    Nilgiri the Forgotten World
    (C) 2004
   ************************************************************************** */
/** \file
    \brief in game command interpreter source
 */

#ifndef _x_INTERPRETER_INCLUDE_x_
#define _x_INTERPRETER_INCLUDE_x_
#include "constants.h"
#include "command.h"

class Character;

typedef struct _commander
{
	cmd_t cmd;
	const char *str;
	void (*fn)(Character *, char *, cmd_t);
}
COMMANDER;

typedef struct _social_data
{
	const char *verb;
	position_t min_vict_pos;
	boolean_t can_see_unseen;
	char *to_char_msg;
	char *to_others_msg;
	char *to_char_vict_msg;
	char *to_others_vict_msg;
	char *to_vict_vict_msg;
	char *to_char_vict_not_found_msg;
	char *to_char_self_msg;
	char *to_others_self_msg;
}
SOCIAL_DATA;

extern void init_interpreter_module(void);
extern void do_look(Character *, char *, cmd_t);
extern void do_dir(Character *, char *, cmd_t);
extern void do_say(Character *, char *, cmd_t);
extern int do_command(Character *, char *);
extern int social_exists(const char *verb);
extern void free_social(SOCIAL_DATA *);
extern void load_social(const char *, SOCIAL_DATA *);
extern void save_social(SOCIAL_DATA *);
extern void add_social(SOCIAL_DATA *);
extern void destroy_social(const char *);

extern COMMANDER *commander;
extern COMMANDER *imm_commander;

#endif
