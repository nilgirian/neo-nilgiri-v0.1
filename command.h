/* **************************************************************************
    Nilgiri the Forgotten World
    (C) 2004
   ************************************************************************** */
/** \file
    \brief general game commands header
 */

#ifndef _x_COMMAND_INCLUDE_x_
#define _x_COMMAND_INCLUDE_x_
#include"utils.h"

class Character;

#define CMDR_PROTO(fn) void fn(Character *ch, char *arg, cmd_t cmd)

/** \sa COMMAND_TYPES */
typedef ubyte1 cmd_t;
typedef ubyte8 cmd_flags_t;

/** This should be a 2-byte integer and no more than 65534 of these. */
enum COMMAND_TYPES
{
	CMD_NORTH, ///<- north must have the same cardinal value in DIR_TYPES
	CMD_EAST, ///<- east must have the same carinal value in DIR_TYPES
	CMD_WEST, ///<- west must the same cardinal value in DIR_TYPES
	CMD_SOUTH, ///<- south must have the same cardinal value in DIR_TYPES
	CMD_UP, ///<- up must have the same cardinal value in DIR_TYPES
	CMD_DOWN, ///<- down must have the same cardinal value in DIR_TYPES
	CMD_LOOK, ///<-
	CMD_SAY, ///<-
	CMD_WHO, ///<-
	CMD_COMMANDS, ///<-
	CMD_SOCIALS, ///<-
	CMD__APOSTROPHE, ///<-
	CMD_QUIT, ///<-
	CMD_HIT, ///<-
	CMD_KILL, ///<-
	CMD_ENVIRONMENT, ///<-
	CMD_TIME, ///<-
	CMD_DATE, ///<-
	CMD_ATTRIBUTES, ///<-
	CMD_GET, ///<-
	CMD_DROP, ///<-
	CMD_INVENTORY, ///<-
	CMD_WRITE, ///<-
	CMD_READ, ///<-
	CMD_HELP, ///<-
	CMD_EXITS, ///<-
	NO_COMMAND
};

extern const char *command[];

/** This should be an 8-byte integer and no more than 63 of these. */
enum IMMORTAL_COMMAND_FLAGS
{
	CMD_USERS, ///<-
	CMD_WIZCOMMANDS, ///<-
	CMD_WIZHELP, ///<-
	CMD_WIZARD, ///<-
	CMD__QUOTES, ///<-
	CMD_SHUTDOWN, ///<-
	CMD__SELECT, ///<-
	CMD_GOTO, ///<- this cannot conflict with mortal movement command
	CMD_LOAD, ///<-
	CMD_STAT, ///<-
	CMD_PURGE, ///<-
	CMD_ADVANCE, ///<-
	CMD_UNLINK, ///<-
	CMD_GRANT, ///<-
	CMD_INDEX, ///<-
	CMD_DICTIONARY, ///<-
	CMD_BUG, ///<-
	CMD_BUGS, ///<-
	CMD_EXPUNGE, ///<-

	CMD_CR_NEW, ///<-
	CMD_CR_EXAMINE, ///<-
	CMD_CR_HELP, ///<-
	CMD_CR_WIZHELP, ///<-
	CMD_CR_DELETE, ///<-
	CMD_CR_SAVE, ///<-
	CMD_CR_DESTROY, ///<-
	CMD_CR_LOAD, ///<-
	CMD_CR_SOCIAL, ///<-
	CMD_CR_CDATA, ///<-
	CMD_CR_CDESCR, ///<-
	CMD_CR_CSTATS, ///<-
	CMD_CR_IDATA, ///<-
	CMD_CR_IDESCR, ///<-
	CMD_CR_ISTATS, ///<-
	CMD_CR_ROOM, ///<-
	CMD_CR_ZONE, ///<-
	NO_IMM_COMMAND
};

extern const char *imm_command[];
extern const int creator_command[];

//act.comm.cc
CMDR_PROTO(	do_say			);

//act.immortal.cc
CMDR_PROTO(	do_goto			);
CMDR_PROTO(	do_users			);
CMDR_PROTO(	do_wizcommands	);
CMDR_PROTO(	do_wizhelp		);
CMDR_PROTO(	do_wizard		);
CMDR_PROTO(	do_shutdown		);
CMDR_PROTO(	do__select		);
CMDR_PROTO(	do_load			);
CMDR_PROTO(	do_stat			);
CMDR_PROTO(	do_purge			);
CMDR_PROTO(	do_advance		);
CMDR_PROTO(	do_unlink		);
CMDR_PROTO(	do_grant			);
CMDR_PROTO(	do_index			);
CMDR_PROTO( do_dictionary	);
CMDR_PROTO( do_bug			);
CMDR_PROTO( do_bugs			);
CMDR_PROTO(	do_expunge		);

//act.informative.cc
CMDR_PROTO(	do_look			);
CMDR_PROTO(	do_who			);
CMDR_PROTO(	do_commands		);
CMDR_PROTO(	do_environment	);
CMDR_PROTO(	do_time			);
CMDR_PROTO(	do_attributes	);
CMDR_PROTO(	do_inventory	);
CMDR_PROTO(	do_help			);
CMDR_PROTO(	do_exits			);

//act.item.cc
CMDR_PROTO(	do_get			);
CMDR_PROTO(	do_drop			);
CMDR_PROTO(	do_write			);
CMDR_PROTO(	do_read			);

//act.modofiy.cc
CMDR_PROTO(	do_cr_new		);
CMDR_PROTO(	do_cr_examine	);
CMDR_PROTO(	do_cr_delete	);
CMDR_PROTO(	do_cr_save		);
CMDR_PROTO(	do_cr_destroy	);
CMDR_PROTO(	do_cr_load		);

//act.modify2.cc
CMDR_PROTO(	do_cr_help		);
CMDR_PROTO(	do_cr_wizhelp	);
CMDR_PROTO(	do_cr_social	);
CMDR_PROTO(	do_cr_cdata		);
CMDR_PROTO(	do_cr_cdescr	);
CMDR_PROTO(	do_cr_cstats	);
CMDR_PROTO(	do_cr_idata		);
CMDR_PROTO(	do_cr_idescr	);
CMDR_PROTO(	do_cr_istats	);
CMDR_PROTO(	do_cr_room		);
CMDR_PROTO(	do_cr_zone		);

//act.movement.cc
CMDR_PROTO(	do_dir			);

//act.offensive.cc
CMDR_PROTO(	do_hit			);

//act.other.cc
CMDR_PROTO(	do_quit			);

//interpreteer.cc
CMDR_PROTO(	do_socials		);

extern const int *command_alpha_index;
extern const int *imm_command_alpha_index;
extern void init_command_module(void);
#endif
