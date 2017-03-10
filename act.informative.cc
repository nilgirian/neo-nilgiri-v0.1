/* **************************************************************************
    Nilgiri the Forgotten World
    (C) 2004
   ************************************************************************** */
/** \file
    \brief general game informative commands source
 */
#include"descriptor.h"
#include"tinyworld_character.h"
#include"interpreter.h"
#include"tinyworld_time.h"
#include"library.h"

void do_look(Character *ch, char *arg, cmd_t cmd)
{
	if (*arg)
	{
		Container *c;
		char victname[MAX_SHORT_DESCR_SIZE];

		one_argument(arg, victname);
		if ((c = searchContent(victname, ch))==NULL)
		{
			ch->put("Nothing found by that name.\r\n");
			return;
		}
		ch->look(c);
		return;
	}
	ch->look();
}

#define WHO_HEADER \
K_GRN "************************************************************\r\n" \
K_BLD_YEL "           Nilgirians in the Forgotten World\r\n" \
K_GRN "************************************************************\r\n"

/**
    \todo This crashes if there is no space before the \r\n
 */
void do_who(Character *ch, char *arg, cmd_t cmd)
{
	Player *p;
	size_t count = 0;
	ch->put(WHO_HEADER);
	for (p = player_character_list; p; p = p->getNextPlayer())
	{
		count++;
		ch->echo(p, "%s %s%s", p->header(), c_bld_sex(p), p->getName());
	}
	if (count==1)
	{
		ch->echo(K_GRN "\r\nVisible players: 1 mighty Nilgirian rules the realm");
	}
	else
	{
		ch->echo(K_GRN " \r\nVisible players: %u", count);
	}
}

#define ENVIRONMENT_HEADER \
K_GRN     "*************************\r\n" \
K_BLD_YEL "  Character Environment\r\n" \
K_GRN     "*************************\r\n" 

void do_environment(Character *ch, char *arg, cmd_t cmd)
{
	int type;

	if (*arg)
	{
		if ((type = find_index(arg, str_player_env)) != (ubyte1) -1)
		{
			if (((Player *) ch)->envFlag(type))
			{
				((Player *) ch)->resetEnvFlag(type);
				ch->print("The %s flag is switched 'off.'\r\n",
						str_player_env[type]);
			}
			else
			{
				((Player *) ch)->setEnvFlag(type);
				ch->print("The %s flag is switched 'On.'\r\n",
						str_player_env[type]);
			}
			return;
		}

		if ((type = find_index(arg, str_desc_env)) != (ubyte1) -1)
		{
			if (ch->getDescriptor()->envFlag(type))
			{
				ch->getDescriptor()->resetEnvFlag(type);
				ch->getDescriptor()->print(
						"The %s flag is switched 'off.'\r\n", str_desc_env[type]);
				if (type == DENV_SPLITSCREEN)
				{
					ch->getDescriptor()->finitSplitScreen();
				}
			}
			else
			{
				ch->getDescriptor()->setEnvFlag(type);
				ch->getDescriptor()->print(
						"The %s flag is switched 'On.'\r\n", str_desc_env[type]);
				if (type == DENV_SPLITSCREEN)
				{
					ch->getDescriptor()->initSplitScreen();
				}
			}
			return;
		}
		ch->put("That flag is not found.\r\n");
		return;
	}

	ch->put(ENVIRONMENT_HEADER);
	for (type  = 0; type < NO_PLAYER_ENVIRONMENT_FLAG; type++)
	{
		ch->print("%-11s [%-3s]   ",
				str_player_env[type],
				(((Player *) ch)->envFlag(type)) ? "On" : "off");


		if (type % 4 == 3)
		{
			ch->put("\r\n");
		}
	}
	if (type % 4 != 0)
	{
		ch->put("\r\n");
	}
	ch->put("\r\n");
	for (type  = 0; type < NO_DESC_ENVIRONMENT_FLAG; type++)
	{
		ch->print("%-11s [%-3s]   ",
				str_desc_env[type],
				ch->getDescriptor()->envFlag(type) ? "On" : "off");


		if (type % 4 == 3)
		{
			ch->put("\r\n");
		}
	}
	if (type % 4 != 0)
	{
		ch->put("\r\n");
	}
}

#define COMMANDS_HEADER \
"*******************************************************************************\r\n    COMMANDS COMMANDS COMMANDS COMMANDS COMMANDS COMMANDS COMMANDS COMMANDS\r\n*******************************************************************************\r\n"

void do_commands(Character *ch, char *arg, cmd_t cmd)
{
	int i, j;
	int len;
	char sp[16];

	ch->put(COMMANDS_HEADER);
	for (i = 0; i < NO_COMMAND; i++)
	{
		ch->put(command[command_alpha_index[i]]);
		ch->put(" ");
		if ((len = 15-strlen(command[command_alpha_index[i]])) > 0)
		{
			for (j = 0; j < len; j++)
			{
				sp[j] = ' ';
			}
			sp[j] = '\0';
			ch->put(sp);
		}
		if ((i+1) % 5 == 0)
		{
			ch->put("\r\n");
		}
	}
	if (i % 5)
	{
		ch->put("\r\n");
	}
}

void do_time(Character *ch, char *arg, cmd_t cmd)
{
	extern const char *mtime_str(time_t);

	ch->print("%s\r\n", mtime_str(0));
}

#define ATTRIBUTE_HEADER \
K_GRN     "*************************\r\n" \
K_BLD_YEL "  Character Attributes\r\n" \
K_GRN     "*************************\r\n"

void do_attributes(Character *ch, char *arg, cmd_t cmd)
{
	ch->put(ATTRIBUTE_HEADER);
	ch->print("Name: %s              Title: %s\r\n",
			ch->getName(),
			ch->getTitle());
	ch->print("e-mail: %s\r\n", ((Player *)ch)->getEmail());
	ch->print("Homeland: %s  Race: %s  Gender: %s  Profession: %s\r\n",
			str_homeland[ch->getHomeland()],
			str_race[ch->getRace()],
			str_gender[ch->getGender()],
			str_profession[ch->getPrimaryProfession()]);
	ch->print("Birthdate: %s\r\n",
			cmdate(((Player *)ch)->getBirthDate()));
	ch->print("Age: %s\r\n",
			mtime_ago(((Player *)ch)->getAge()));
}

void do_inventory(Character *ch, char *arg, cmd_t cmd)
{
	ch->listItemsTo();
}

void do_help(Character *ch, char *arg, cmd_t cmd)
{
	HELP_ENTRY hentry;

	if (*arg == '\0')
	{
		char *keywords = helpTable->list();
		ch->m_put(keywords);
		return;
	}
	hentry.keyword = NULL;
	hentry.entry = NULL;
	helpTable->load(arg, &hentry);
	if (hentry.entry)
	{
		ch->print("%s%s\r\n%s",
				HELP_HEADER,
				hentry.keyword,
				hentry.entry);
		return;
	}
	ch->put("There is no help information on that.\r\n");
}

void do_exits(Character *ch, char *arg, cmd_t cmd)
{
	Room *room, *rmdir;

	if ((room = ch->inContainer()->objRoom())==NULL)
	{
		ch->put("You are not sure where the exit is.\r\n");
		return;
	}
	ch->put("Obvious exits:\r\n");
	int cnt = 0;
	for (dir_t dir=NORTH; dir < NO_DIR; dir++)
	{
		if ((rmdir = room->getRoomDir(dir)))
		{
			cnt++;
			if (ch->Imm)
			{
				ch->print("%-5s - %s [%u]\n",
						str_Dir[dir],
						rmdir->getName(),
						rmdir->getVID());
			}
			else
			{
				ch->print("%-5s - %s\n",
						str_Dir[dir],
						rmdir->getName());
			}
		}
	}
	if (cnt == 0)
	{
		ch->put("None.\r\n");
	}
}
