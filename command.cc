/* **************************************************************************
    Nilgiri the Forgotten World
    (C) 2004
   ************************************************************************** */
/** \file
    \brief general game commands header
 */
#include<stdio.h>
#include"command.h"
#include"interpreter.h"

const int *command_alpha_index = NULL;
const int *imm_command_alpha_index = NULL;

const char *command[NO_COMMAND+1] =
{
	"north",
	"east",
	"west",
	"south",
	"up",
	"down",
	"look",
	"say",
	"who",
	"commands",
	"socials",
	"'",
	"quit",
	"hit",
	"kill",
	"environment",
	"time",
	"date",
	"attributes",
	"get",
	"drop",
	"inventory",
	"write",
	"read",
	"help",
	"exits",
	"\n"
};

const char *imm_command[NO_IMM_COMMAND+1] =
{
	"users",
	"wizcommands",
	"wizhelp",
	"wizard",
	"\"",
	"shutdown",
	"%select",
	"goto",
	"load",
	"stat",
	"purge",
	"advance",
	"unlink",
	"grant",
	"index",
	"dictionary",
	"bug",
	"bugs",
	"expunge",

	"@new",
	"@examine",
	"@help",
	"@wizhelp",
	"@delete",
	"@save",
	"@destroy",
	"@load",
	"@social",
	"@cdata",
	"@cdescr",
	"@cstats",
	"@idata",
	"@idescr",
	"@istats",
	"@room",
	"@zone",
	"\n"
};

void init_command_module(void)
{
	TYPE_CHECK(NO_COMMAND, cmd_t);
	TYPE_CHECK(NO_IMM_COMMAND, cmd_t);
	FLAG_CHECK(NO_IMM_COMMAND, cmd_flags_t);

	int cmd;
	command_alpha_index = order_list_index(command, NO_COMMAND);
	imm_command_alpha_index = order_list_index(imm_command, NO_IMM_COMMAND);

	MALLOC(commander, COMMANDER, NO_COMMAND);
	for (cmd = 0; cmd < NO_COMMAND; cmd++)
	{
		commander[cmd].cmd = cmd;
		commander[cmd].str = command[cmd];
		commander[cmd].fn = NULL;
	}
	commander[CMD_LOOK			].fn = do_look;
	commander[CMD_NORTH			].fn = do_dir;
	commander[CMD_EAST			].fn = do_dir;
	commander[CMD_SOUTH			].fn = do_dir;
	commander[CMD_WEST			].fn = do_dir;
	commander[CMD_UP				].fn = do_dir;
	commander[CMD_DOWN			].fn = do_dir;
	commander[CMD_SAY				].fn = do_say;
	commander[CMD_WHO				].fn = do_who;
	commander[CMD_COMMANDS		].fn = do_commands;
	commander[CMD_SOCIALS		].fn = do_socials;
	commander[CMD__APOSTROPHE	].fn = do_say;
	commander[CMD_QUIT			].fn = do_quit;
	commander[CMD_HIT				].fn = do_hit;
	commander[CMD_KILL			].fn = do_hit;
	commander[CMD_ENVIRONMENT	].fn = do_environment;
	commander[CMD_TIME			].fn = do_time;
	commander[CMD_DATE			].fn = do_time;
	commander[CMD_ATTRIBUTES	].fn = do_attributes;
	commander[CMD_GET				].fn = do_get;
	commander[CMD_DROP			].fn = do_drop;
	commander[CMD_INVENTORY		].fn = do_inventory;
	commander[CMD_WRITE			].fn = do_write;
	commander[CMD_READ			].fn = do_read;
	commander[CMD_HELP			].fn = do_help;
	commander[CMD_EXITS			].fn = do_exits;

	MALLOC(imm_commander, COMMANDER, NO_IMM_COMMAND);
	for (cmd = 0; cmd < NO_IMM_COMMAND; cmd++)
	{
		imm_commander[cmd].cmd = cmd;
		imm_commander[cmd].str = imm_command[cmd];
		imm_commander[cmd].fn = NULL;
	}
	imm_commander[CMD_GOTO			].fn = do_goto;
	imm_commander[CMD_USERS			].fn = do_users;
	imm_commander[CMD_WIZCOMMANDS	].fn = do_wizcommands;
	imm_commander[CMD_WIZHELP		].fn = do_wizhelp;
	imm_commander[CMD_WIZARD		].fn = do_wizard;
	imm_commander[CMD__QUOTES		].fn = do_wizard;
	imm_commander[CMD_SHUTDOWN		].fn = do_shutdown;
	imm_commander[CMD__SELECT		].fn = do__select;
	imm_commander[CMD_LOAD			].fn = do_load;
	imm_commander[CMD_STAT			].fn = do_stat;
	imm_commander[CMD_PURGE			].fn = do_purge;
	imm_commander[CMD_ADVANCE		].fn = do_advance;
	imm_commander[CMD_UNLINK		].fn = do_unlink;
	imm_commander[CMD_GRANT			].fn = do_grant;
	imm_commander[CMD_INDEX			].fn = do_index;
	imm_commander[CMD_DICTIONARY	].fn = do_dictionary;
	imm_commander[CMD_BUG			].fn = do_bug;
	imm_commander[CMD_BUGS			].fn = do_bugs;
	imm_commander[CMD_EXPUNGE		].fn = do_expunge;

	imm_commander[CMD_CR_NEW		].fn = do_cr_new;
	imm_commander[CMD_CR_EXAMINE	].fn = do_cr_examine;
	imm_commander[CMD_CR_HELP		].fn = do_cr_help;
	imm_commander[CMD_CR_WIZHELP	].fn = do_cr_wizhelp;
	imm_commander[CMD_CR_DELETE	].fn = do_cr_delete;
	imm_commander[CMD_CR_SAVE		].fn = do_cr_save;
	imm_commander[CMD_CR_DESTROY	].fn = do_cr_destroy;
	imm_commander[CMD_CR_LOAD		].fn = do_cr_load;
	imm_commander[CMD_CR_SOCIAL	].fn = do_cr_social;
	imm_commander[CMD_CR_CDATA		].fn = do_cr_cdata;
	imm_commander[CMD_CR_CDESCR	].fn = do_cr_cdescr;
	imm_commander[CMD_CR_CSTATS	].fn = do_cr_cstats;
	imm_commander[CMD_CR_IDATA		].fn = do_cr_idata;
	imm_commander[CMD_CR_IDESCR	].fn = do_cr_idescr;
	imm_commander[CMD_CR_ISTATS	].fn = do_cr_istats;
	imm_commander[CMD_CR_ROOM		].fn = do_cr_room;
	imm_commander[CMD_CR_ZONE		].fn = do_cr_zone;
}
