/* **************************************************************************
    Nilgiri the Forgotten World
    (C) 2004
   ************************************************************************** */
/** \file
    \brief general immortal game commands module
 */
#include"tinyworld_character.h"
#include"tinyworld_item.h"
#include"descriptor.h"
#include"bug.h"
#include"sqlite.h"
#include"utils.h"
#include"addlog.h"

void do_goto(Character *ch, char *arg, cmd_t cmd)
{
	assert(ch->inContainer());
	vid_t id = atoi(arg);

	if (id == 0 || ((id = roomIndex.get(id))==(vid_t) -1))
	{
		ch->echo("That room number does not exist.");
		return;
	}
	ch->moveTo(world[id], cmd);
}

/** For do_user().
    \param ch Person receiving the information
	 \param d The descriptor to show user information of
 */
static inline void user(Character *ch, Descriptor *d)
{
	ch->echo("%-2d- %-10s : %-15s : %6u",
			d->getDesc(),
			d->getName(),
			d->getState(),
			d->getIdleTime());
}

/** For do_user().
    \param ch Person receiving the information
	 \param d The descriptor to show user information of
 */
static inline void user_assignment(Character *ch, Descriptor *d)
{
	ch->echo("%-2d- %-10s : %-64s",
			d->getDesc(),
			d->getName(),
			d->getEmail());
}

/** For do_user().
    \param ch Person receiving the information
	 \param d The descriptor to show user information of
 */
static inline void user_ip(Character *ch, Descriptor *d)
{
	unsigned long int ipr = d->getIP();
	ubyte1 ip[4], count;
	char sp[9];
	ip[0] = *((unsigned char *) &ipr);
	ip[1] = *((unsigned char *) &ipr+1);
	ip[2] = *((unsigned char *) &ipr+2);
	ip[3] = *((unsigned char *) &ipr+3);
	count = 0;
	if (ip[0] < 100)
	{
		sp[count] = ' ';
		count++;
	}
	if (ip[0] < 10)
	{
		sp[count] = ' ';
		count++;
	}
	if (ip[1] < 100)
	{
		sp[count] = ' ';
		count++;
	}
	if (ip[1] < 10)
	{
		sp[count] = ' ';
		count++;
	}
	if (ip[2] < 100)
	{
		sp[count] = ' ';
		count++;
	}
	if (ip[2] < 10)
	{
		sp[count] = ' ';
		count++;
	}
	if (ip[3] < 100)
	{
		sp[count] = ' ';
		count++;
	}
	if (ip[3] < 10)
	{
		sp[count] = ' ';
		count++;
}
	sp[count] = '\0';
	ch->echo("%-2d- %-10s : [%d.%d.%d.%d] %s[%s]",
			d->getDesc(),
			d->getName(),
			ip[0], ip[1], ip[2], ip[3],
			sp,
			d->getHostname());
}

void do_users(Character *ch, char *arg, cmd_t cmd)
{
	size_t count=0;
	size_t arglen = strlen(arg);
	Descriptor *d;

	if (*arg)
	{
		if (strncasecmp(arg, "assignments", arglen)==0)
		{
			ch->put("Assignments:\r\n");
			ch->put("------------\r\n");
			for (d = descriptor_list; d; d = d->getNext())
			{
				count++;
				user_assignment(ch, d);
			}
			ch->echo("\r\nNumber of connections: %u", count);
		}
		else if (strncasecmp(arg, "ip", arglen)==0)
		{
			ch->put("IP adresses\r\n");
			ch->put("-----------\r\n");
			for (d = descriptor_list; d; d = d->getNext())
			{
				count++;
				user_ip(ch, d);
			}
			ch->echo("\r\nNumber of connections: %u", count);
		}
		else
		{
			ch->put("user (assignment|ip)");
		}
		return;
	}
	ch->put("Connections:\r\n");
	ch->put("------------\r\n");
	for (d = descriptor_list; d; d = d->getNext())
	{
		count++;
		user(ch, d);
	}
	ch->echo("\r\nNumber of connections: %u", count);
}

#define COMMANDS_HEADER \
"*******************************************************************************\r\n    COMMANDS COMMANDS COMMANDS COMMANDS COMMANDS COMMANDS COMMANDS COMMANDS\r\n*******************************************************************************\r\n"
void do_wizcommands(Character *ch, char *arg, cmd_t cmd)
{
	int i, j, n;
	int len;
	char sp[16];
	extern const char *imm_command[];
	extern const int *imm_command_alpha_index;


	ch->put(COMMANDS_HEADER);
	n = 0;
	for (i = 0; i < NO_IMM_COMMAND; i++)
	{
		if (ch->Imm->cannotDo(imm_command_alpha_index[i]))
		{
			continue;
		}
		ch->put(imm_command[imm_command_alpha_index[i]]);
		ch->put(" ");
		if ((len = 15-strlen(imm_command[imm_command_alpha_index[i]])) > 0)
		{
			for (j = 0; j < len; j++)
			{
				sp[j] = ' ';
			}
			sp[j] = '\0';
			ch->put(sp);
		}
		if ((n+1) % 5 == 0)
		{
			ch->put("\r\n");
		}
		n++;
	}
	if (n % 5)
	{
		ch->put("\r\n");
	}
}

void do_wizhelp(Character *ch, char *arg, cmd_t cmd)
{
	HELP_ENTRY hentry;

	if (*arg == '\0')
	{
		char *keywords = immhelpTable->list();
		ch->m_put(keywords);
		return;
	}
	hentry.keyword = NULL;
	hentry.entry = NULL;
	immhelpTable->load(arg, &hentry);
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

void do_wizard(Character *ch, char *arg, cmd_t cmd)
{
	if (*arg=='\0')
	{
		ch->put("What do you wish to wizard?\r\n");
		return;
	}
	for (Player *p = player_character_list;
			p; p = p->getNextPlayer())
	{
		if (p->inContainer())
		{
			if (ch == p)
			{
				ch->echo(K_BLD_YEL "$n: %s", arg);
			}
			else
			{
				ch->echoTo(p, K_BLD_YEL "$n: %s", arg);
			}
		}
	}
}

void do_shutdown(Character *ch, char *arg, cmd_t cmd)
{
	extern void shutdown_now(void);
	extern void queue_shutdown(void *);
	extern int shutdown_time;

	time_t t = atoi(arg);

	if (*arg == '\0')
	{
		shutdown_now();
		return;
	}
	if (is_not_number(arg))
	{
		ch->put("shutdown (time in seconds)\r\n");
		return;
	}
	shutdown_time = time0 + t;
	ch->put("Ok.");
	queue_shutdown((void *) (time0 + t));
}

static int callback_do__select(void *pArg,
		int argc, char **argv, char **columnName)
{
	Character *ch = (Character *) pArg;

	for (int i = 0; i < argc; i++)
	{
		if (i)
		{
			ch->put(" | ");
		}
		ch->put(argv[i] ? argv[i] : "null");
	}
	ch->put("\r\n");

	return 0;
}

#define MAX_QUERY_LEN 1024
extern VidIndex cdataIndex;
void do__select(Character *ch, char *arg, cmd_t cmd)
{
	char *dbErrMsg = NULL;
	char query[MAX_QUERY_LEN+1];

	if (snprintf(query, MAX_QUERY_LEN, "SELECT %s", arg) == MAX_QUERY_LEN)
	{
		ch->put("Your SQL select statement is too long.\r\n");
		return;
	}

	if (sqlite_exec(db, query, callback_do__select, ch, &dbErrMsg) != SQLITE_OK)
	{
		ch->print("SQL error for: %s\r\n", dbErrMsg);
	}
	ch->put("Ok.\r\n");
}

void do_oload(Character *ch, char *arg, cmd_t cmd)
{
	vid_t virtual_id;
	int rn;
	Item *item;

	if (arg && is_number(arg) && (virtual_id = atoi(arg)) > 0)
	{
		item = create_item(virtual_id, ch->getCID(), ch->inContainer());
		ch->print("You load %s.\r\n", item->getName());
		ch->inContainer()->echo(ch, item,
				"$g$n makes a strange magical gesture.\r\n$n has created $N!");
		return;
	}
	if (cmd == CMD_LOAD)
	{
		ch->put("load object <virtual-id>\r\n");
		return;
	}
	ch->put("oload <virtual-id>\r\n");
}

void do_cload(Character *ch, char *arg, cmd_t cmd)
{
	vid_t virtual_id;
	int rn;
	Character *character;

	if (arg && is_number(arg) && (virtual_id = atoi(arg)) > 0)
	{
		character = create_character(virtual_id, ch->getCID(), ch->inContainer());
		ch->print("You load %s.\r\n", character->getName());
		ch->inContainer()->echo(ch, character,
				"$g$n makes a quaint, magical gesture with one hand.\r\n"
				"$n has created $N!");
		return;
	}
	if (cmd == CMD_LOAD)
	{
		ch->put("load character <virtual-id>\r\n");
		return;
	}
	ch->put("cload <virtual-id>\r\n");
}

void do_load(Character *ch, char *arg, cmd_t cmd)
{
	TOKENIZER(arg, argv);
	if (argv[0] && argv[1])
	{
		if (is_abbrev(argv[0], "character"))
		{
			do_cload(ch, argv[1], cmd);
			return;
		}
		if (is_abbrev(argv[0], "object"))
		{
			do_oload(ch, argv[1], cmd);
			return;
		}
	}
	ch->put("load (character | object) <virtual-id>\r\n");
}

void do_ostat(Character *ch, char *arg, cmd_t cmd)
{
	extern VidIndex idataIndex;
	Item *item;

	if (arg && *arg)
	{
		if ((item = searchItem(arg, ch))==NULL)
		{
			ch->put("No item found by that name.\r\n");
			return;
		}

		ch->print("ITEM - %s : [CID: #%08X (%u)] [VID: %u (%u)]\r\n",
				item->getName(),
				item->getCID(),
				item->getCID(),
				item->getVID(),
				idataIndex.get(item->getVID()));
		item->printStats(ch);
		return;
	}
	if (cmd == CMD_STAT)
	{
		ch->put("stat object <name>\r\n");
		return;
	}
	ch->put("ostat <name>\r\n");
}

void do_pstat(Character *ch, char *arg, cmd_t cmd)
{
	extern const char *mtime_ago(time_t);

	Player *vict;

	if (arg && *arg)
	{
		if ((vict = ch->findPlayer(arg))==NULL)
		{
			ch->put("No player found by that name.\r\n");
			return;
		}

		ch->print("%s PLAYER - %s : [CID: #%08X (%u)] [VID: %u (%u)]\r\n",
				vict->Imm ? str_immortal_types_u[vict->Imm->getLevel()]
				: str_gender_u[vict->getGender()],
				vict->getName(),
				vict->getCID(),
				vict->getCID(),
				ch->getCharDataVid(),
				cdataIndex.get(vict->getCharDataVid()));
		ch->print("[ %s ] [ %s ] [ %s ] [ %s ]\r\n",
				str_homeland_p[ch->getHomeland()],
				str_race[ch->getRace()],
				str_gender[ch->getGender()],
				str_profession[ch->getPrimaryProfession()]);
		ch->print("Birth : [Host %s] [Date %s]\r\n",
				vict->getBirthHost(),
				time_str(vict->getBirth()));
		ch->print("Email: %s\r\n",
				vict->getEmail());
		ch->print("Natural: [str %2u/%3u] [dex %2u] [int %2u] [wis %2u] [con %2u] [cha %2u]\r\n",
				vict->getStat(STRENGTH),
				vict->getStat(STRENGTH_ADD),
				vict->getStat(DEXTERITY),
				vict->getStat(INTELLIGENCE),
				vict->getStat(WISDOM),
				vict->getStat(CONSTITUTION),
				vict->getStat(CHARISMA));
		ch->print("Current: [str %2u/%3u] [dex %2u] [int %2u] [wis %2u] [con %2u] [cha %2u]\r\n",
				vict->getNaturalStat(STRENGTH),
				vict->getNaturalStat(STRENGTH_ADD),
				vict->getNaturalStat(DEXTERITY),
				vict->getNaturalStat(INTELLIGENCE),
				vict->getNaturalStat(WISDOM),
				vict->getNaturalStat(CONSTITUTION),
				vict->getNaturalStat(CHARISMA));
		ch->print("Age: %s (%us)\r\n",
				mtime_ago(vict->getAge()),
				vict->getAgeMod());
		ch->print("Playing Time: %s (%u)\r\n",
				time_ago(vict->getPlayed()),
				vict->getPlayed());
		ch->print("Last Logon: %s\r\n"
				    "            (%s)\r\n",
					 time_str(vict->getLastLogon()),
					 time_ago(time0-vict->getLastLogon()));
		vict->printStats(ch);
		return;
	}
	if (cmd == CMD_STAT)
	{
		ch->put("stat player <name>\r\n");
		return;
	}
	ch->put("pstat <name>\r\n");
}

void do_rstat(Character *ch, char *arg, cmd_t cmd)
{
	Room *rm;

	if ((rm = ch->inContainer()->objRoom())==NULL)
	{
		ch->put("You are not in a room.\r\n");
		return;
	}

	ch->print("[ROOM - %s : [CID: #%08X (%u)]\r\n",
			rm->getName(),
			rm->getCID(),
			rm->getCID());
	rm->printStats(ch);
}

void do_wstat(Character *ch, char *arg, cmd_t cmd)
{
	ch->print("boot time: %s\r\n"
			    "           (%s)\r\n",
			time_str(game_stat.boot_time),
			time_ago(time0-game_stat.boot_time));
	ch->print("items: %d created - %d destroyed = %d\r\n",
			game_stat.items_created,
			game_stat.items_destroyed,
			game_stat.items_created - game_stat.items_destroyed);
	ch->print("characters: %d + created - %d destroyed = %d\r\n",
			game_stat.characters_created,
			game_stat.characters_destroyed,
			game_stat.characters_created - game_stat.characters_destroyed);
	ch->print("players: %d new + %d old - %d destroyed = %d\r\n",
			game_stat.new_player_characters_created,
			game_stat.old_player_characters_created,
			game_stat.player_characters_destroyed,
			game_stat.new_player_characters_created
			+ game_stat.old_player_characters_created
			- game_stat.player_characters_destroyed);
	ch->print("descriptors: %d created - %d destroyed = %d\r\n",
			game_stat.descriptors_created,
			game_stat.descriptors_destroyed,
			game_stat.descriptors_created - game_stat.descriptors_destroyed);
	ch->print("rooms: %d created - %d destroyed = %d\r\n",
			game_stat.rooms_created,
			game_stat.rooms_destroyed,
			game_stat.rooms_created - game_stat.rooms_destroyed);
	ch->print("last cid: #%08lX (%ld) session id: #%02hX (%hd)\r\n",
			game_stat.last_cid, game_stat.last_cid,
			game_stat.session_id, game_stat.session_id);
	ch->print("dictionary: %d words\r\n", game_stat.dictionary_words);
	ch->print("first boot at: %s\r\n"
				 "               (%s)\r\n",
			time_str(game_stat.time_zero),
			time_ago(time0-game_stat.time_zero));
	ch->print("load average 1/5/15m: %d  %d  %d\r\n",
			game_stat.load_average[0],
			(game_stat.load_average[0]
			 +game_stat.load_average[1]
			 +game_stat.load_average[2]
			 +game_stat.load_average[3]
			 +game_stat.load_average[4])/5,
			(game_stat.load_average[0]
			 +game_stat.load_average[1]
			 +game_stat.load_average[2]
			 +game_stat.load_average[3]
			 +game_stat.load_average[4]
			 +game_stat.load_average[5]
			 +game_stat.load_average[6]
			 +game_stat.load_average[7]
			 +game_stat.load_average[8]
			 +game_stat.load_average[9]
			 +game_stat.load_average[10]
			 +game_stat.load_average[11]
			 +game_stat.load_average[12]
			 +game_stat.load_average[13]
			 +game_stat.load_average[14])/15);
}

void do_stat(Character *ch, char *arg, cmd_t cmd)
{
	TOKENIZER(arg, argv);
	if (argv[0])
	{
		if (is_abbrev(argv[0], "object"))
		{
			do_ostat(ch, argv[1], cmd);
			return;
		}
		if (is_abbrev(argv[0], "player"))
		{
			do_pstat(ch, argv[1], cmd);
			return;
		}
		if (is_abbrev(argv[0], "room"))
		{
			do_rstat(ch, argv[1], cmd);
			return;
		}
		if (is_abbrev(argv[0], "world"))
		{
			do_wstat(ch, argv[1], cmd);
			return;
		}
	}
	ch->put("stat (character | object | player | room | world)\r\n");
}

void do_purge(Character *ch, char *arg, cmd_t cmd)
{
	if (*arg)
	{
		Container *c;
		if ((c = searchContent(arg, ch))==NULL)
		{
			ch->put("Nothing found by that name.\r\n");
			return;
		}
		ch->print("You purge %s.\r\n", c->getName());
		ch->inContainer()->echo(ch, c, "$g$n destroys $N.");
		delete c;
		return;
	}
	ch->put("purge <object|character>\r\n");
}

void do_advance(Character *ch, char *arg, cmd_t cmd)
{
	TOKENIZER(arg, argv);
	level_t level;
	Player *vict;

	if (argv[0] && argv[1])
	{
		if ((vict = ch->findPlayer(argv[0]))==NULL)
		{
			ch->put("No player found by that name.\r\n");
			return;
		}
		if (vict->inContainer() != vict->inContainer())
		{
			ch->put("That person is not here right now.\r\n");
			return;
		}
		if ((level = find_index(argv[1], str_immortal_types)) == (ubyte1) -1)
		{
			ch->put("That is not an immortal rank.\r\n");
			return;
		}
		if (level == IMPLEMENTOR)
		{
			ch->put("You cannot advance an Implementor this way.\r\n");
			return;
		}
		if (level == ch->Imm->getLevel())
		{
			ch->put("You cannot advance someone as your peer.\r\n");
			return;
		}
		if (level > ch->Imm->getLevel())
		{
			ch->put("You cannot advance someone a greater rank than you.\r\n");
			return;
		}
		if (vict->Imm && vict->Imm->getLevel() == level)
		{
			ch->put("That immortal is already advanced to that rank of immortality.\r\n");
			return;
		}

		if (vict->Imm && vict->Imm->getLevel() > level)
		{
			ch->echo(vict, "A column of total darkness befallls $N momentarily.");
			ch->inContainer()->echo(ch, vict,
					"A column of total darkness engulfs $N momentarily.");
			ch->echoTo(vict, "A column of darkness engulfs you momentarily.");
			addlog_demote_imm(vict, ch, level);
		}
		else
		{
			ch->echo(vict, K_BLD_YEL "A bright column of light shines from eternity engulfing $N in its glow!");
			ch->inContainer()->echo(ch, vict, K_BLD_YEL "A bright column of light shines from eternity engulfing $N in its glow!");
			ch->echoTo(vict, K_BLD_YEL "A bright column of light shines from eternity engulfing you in its glow!\r\nYou feel much better...");
			addlog_advance_imm(vict, ch, level);
		}
		vict->Imm = new ImmortalHelper(level);
		return;
	}
	ch->put("advance <player> <immortal-type>\r\n");
}

void do_unlink(Character *ch, char *arg, cmd_t cmd)
{
	Character *vict;
	if ((vict = ch->inContainer()->findCharacter(arg, ch))==NULL)
	{
		ch->put("No character found by that name.\r\n");
		return;
	}
	if (vict->getDescriptor()==NULL)
	{
		ch->put("That character is not linked.\r\n");
		return;
	}
	ch->echo(vict, "You unlink $N.");
	ch->inContainer()->echo(ch, vict, "$n unlink $N.");
	vict->getDescriptor()->extract();
}

/** This is essentially the function list_flags_to().  However, this is
    different because immcommands use a seperate array to order the commands.
	 \param ch The character to echo the commands to
	 \param bits The commands that are set, to see all the commands use
	 (cmd_flags_t) -1 to set all the bits.
 */
static void list_immcommands_to(Character *ch, cmd_flags_t bits)
{
	char sp[16];
	int len, i, j, x;

	i = 0;
	for (x = 0; x < NO_IMM_COMMAND; x++)
	{
		if (IS_SET(f_(imm_command_alpha_index[x]), bits))
		{
			ch->put(imm_command[imm_command_alpha_index[x]]);
			ch->put(" ");
			if ((len = 15-strlen(imm_command[imm_command_alpha_index[x]])) > 0)
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
			i++;
		}
	}
	if (i % 5)
	{
		ch->put("\r\n");
	}
}

void grant_wizcommand(Character *ch, const char *victname, char *cmd,
		char *wizcmd)
{
	Player *vict;
	cmd_t immcmd;

	if (victname && cmd && wizcmd)
	{
		if ((vict = ch->findPlayer(victname))==NULL)
		{
			ch->put("No player found by that name.\r\n");
			return;
		}
		if (vict == ch)
		{
			ch->put("You can neither grant nor revoke your own commands!\r\n");
			return;
		}
		if (vict->Imm==NULL)
		{
			ch->put("That player is not immortal.\r\n");
			return;
		}
		if (vict->Imm->getLevel() >= ch->Imm->getLevel())
		{
			ch->put("Nice try.\r\n");
			return;
		}
		if ((immcmd = find_index(wizcmd, imm_command)) == (ubyte1) -1)
		{
			ch->put("That wizcommand command does not exist."
					" Try one of these:\r\n");
			list_immcommands_to(ch, (cmd_flags_t)-1);
			return;
		}
		if (is_name(cmd, "add"))
		{
			if (vict->Imm->canDo(immcmd))
			{
				ch->print("%s already has the '%s' command.\r\n",
						vict->getName(),
						imm_command[immcmd]);
				return;
			}
			vict->Imm->setCmdFlag(immcmd);
			ch->print("%s has been granted the '%s' command.\r\n",
					vict->getName(),
					imm_command[immcmd]);
			ch->echoTo(vict, "$n has granted you the '%s' command",
					imm_command[immcmd]);
			ch->inContainer()->echo(ch, vict, "$n seems more powerful!");
			addlog_grant_immcmd(vict, ch, imm_command[immcmd]);
			return;
		}
		if (is_name(cmd, "delete"))
		{
			if (vict->Imm->cannotDo(immcmd))
			{
				ch->print("%s does not have the '%s' command.\r\n",
						vict->getName(),
						imm_command[immcmd]);
				return;
			}
			vict->Imm->resetCmdFlag(immcmd);
			ch->print("%s has been revoked the '%s' command.\r\n",
					vict->getName(),
					imm_command[immcmd]);
			ch->echoTo(vict, "$n has revoked your '%s' command.",
					imm_command[immcmd]);
			ch->inContainer()->echo(ch, vict, "$n seems less powerful.");
			addlog_revoke_immcmd(vict, ch, imm_command[immcmd]);
			return;
		}
	}
	else if (vict && cmd == NULL & wizcmd == NULL)
	{
		ch->print("%s is granted the following commands:\r\n",
				vict->getName());
		list_immcommands_to(ch, vict->Imm->cmdFlags());
		return;
	}
	ch->put("grant wizcommand <player> (add | delete) (<command>)\r\n");
}

void grant_bug_permit(Character *ch,
		const char *owner, char *cmd, const char *submitter)
{
	if (owner && cmd && submitter)
	{
		if ((owner = get_player_name(owner)) == NULL)
		{
			ch->put("The owner does not exist.\r\n");
			return;
		}

		if ((submitter = get_player_name(submitter)) == NULL)
		{
			ch->put("The submitter does not exist.\r\n");
			return;
		}

		if (is_name(cmd, "add"))
		{
			if (BugPermit::allowed(owner, submitter))
			{
				ch->put("That entry already exists.\r\n");
				return;
			}
			BugPermit::add(owner, submitter);
			BugPermit::save(owner, submitter);
			ch->print("Ok. '%s' is allowed to bug '%s'.\r\n", submitter, owner);
			addlog_grant_bug(ch, owner, submitter);
			return;
		}
		if (is_name(cmd, "delete"))
		{
			if (BugPermit::allowed(owner, submitter) == NULL)
			{
				ch->put("That entry does not exist.\r\n");
				return;
			}
			BugPermit::del(owner, submitter);
			BugPermit::delDB(owner, submitter);
			ch->print("Ok. '%s' is no longer allowed to bug '%s'.\r\n",
					submitter, owner);
			addlog_revoke_bug(ch, owner, submitter);
			return;
		}
	}
	ch->put("grant bug_permit <owner> [add | delete] <submitter>\r\n");
}

void do_grant(Character *ch, char *arg, cmd_t cmd)
{
	TOKENIZER(arg, argv);
	if (argv[0])
	{
		if (is_name(argv[0], "wizcommand"))
		{
			grant_wizcommand(ch, argv[1], argv[2], argv[3]);
			return;
		}
		if (is_name(argv[0], "bug_permit"))
		{
			grant_bug_permit(ch, argv[1], argv[2], argv[3]);
			return;
		}
	}
	ch->put("grant (wizcommand | bug_permit)\r\n");
}

void index_cdata(Character *ch, char *arg)
{
	extern CHAR_DATA **char_data;
	extern top_t char_data_top;
	char buf[LONG_LINE_LENGTH+1];

	for (top_t i = 0; i <= char_data_top; i++)
	{
		sprintf(buf, "V#%-5lu [%2u/%2d] [D#%-5lu] [S#%-5lu] %-32s\r\n",
				char_data[i]->virtual_id,
				char_data[i]->count,
				char_data[i]->limit == (ubyte2) -1 ?
				-1 : (int) char_data[i]->limit,
				char_data[i]->descr->virtual_id,
				char_data[i]->stats->virtual_id,
				char_data[i]->descr->proper);
		if (arg && is_substr(arg, buf)==0)
		{
			continue;
		}
		ch->put(buf);
	}
}

void index_cdescr(Character *ch, char *arg)
{
	extern CHAR_DESCR_DATA **char_descr;
	extern top_t char_descr_top;
	char buf[LONG_LINE_LENGTH+1];

	for (top_t i = 0; i <= char_descr_top; i++)
	{
		sprintf(buf, "D#%-5lu %-39s %-32s\r\n",
				char_descr[i]->virtual_id,
				char_descr[i]->key->words(),
				char_descr[i]->proper);
		if (arg && is_substr(arg, buf)==0)
		{
			continue;
		}
		ch->put(buf);
	}
}

void index_cstats(Character *ch, char *arg)
{
	extern CHAR_STATS_DATA **char_stats;
	extern top_t char_stats_top;
	extern const char *str_race[];
	extern const char *str_profession_index(profession_flags_t);
	char buf[LONG_LINE_LENGTH+1];

	for (top_t i = 0; i <= char_stats_top; i++)
	{
		sprintf(buf, "S#%-5lu [%-10s] [%-10s] [%d]\r\n",
				char_stats[i]->virtual_id,
				str_race[char_stats[i]->race],
				str_profession_index(char_stats[i]->profession),
				0 /* MARK formula */);
		if (arg && is_substr(arg, buf)==0)
		{
			continue;
		}
		ch->put(buf);
	}
}

void index_idata(Character *ch, char *arg)
{
	extern ITEM_DATA **item_data;
	extern top_t item_data_top;
	char buf[LONG_LINE_LENGTH+1];

	for (top_t i = 0; i <= item_data_top; i++)
	{
		sprintf(buf, "V#%-5lu [%-2u/%-2d] [D#%-5lu] [S#%-5lu] %-32s\r\n",
			item_data[i]->virtual_id,
			item_data[i]->count,
			item_data[i]->limit == (ubyte2) -1 ?
				-1 : (int) item_data[i]->limit,
			item_data[i]->descr->virtual_id,
			item_data[i]->stats->virtual_id,
			item_data[i]->descr->proper);
		if (arg && is_substr(arg, buf)==0)
		{
			continue;
		}
		ch->put(buf);
	}
}

void index_idescr(Character *ch, char *arg)
{
	extern ITEM_DESCR_DATA **item_descr;
	extern top_t item_descr_top;
	char buf[LONG_LINE_LENGTH+1];

	for (top_t i = 0; i <= item_descr_top; i++)
	{
		sprintf(buf, "D#%-5lu %-39s %-32s\r\n",
				item_descr[i]->virtual_id,
				item_descr[i]->key->words(),
				item_descr[i]->proper);
		if (arg && is_substr(arg, buf)==0)
		{
			continue;
		}
		ch->put(buf);
	}
}

void index_istats(Character *ch, char *arg)
{
	extern ITEM_STATS_DATA **item_stats;
	extern top_t item_stats_top;
	char buf[LONG_LINE_LENGTH+1];

	for (top_t i = 0; i <= item_stats_top; i++)
	{
		sprintf(buf, "S#%-5lu [%-12s] [%u]\r\n",
				item_stats[i]->virtual_id,
				str_item_type[item_stats[i]->item_type],
				0 /* MARK calculate cost */);
		if (arg && is_substr(arg, buf)==0)
		{
			continue;
		}
		ch->put(buf);
	}
}

void do_index(Character *ch, char *arg, cmd_t cmd)
{
	TOKENIZER(arg, argv);
	if (argv[0])
	{
		if (is_name(argv[0], "cdata"))
		{
			index_cdata(ch, argv[1]);
			return;
		}
		if (is_name(argv[0], "cdescr"))
		{
			index_cdescr(ch, argv[1]);
			return;
		}
		if (is_name(argv[0], "cstats"))
		{
			index_cstats(ch, argv[1]);
			return;
		}
		if (is_name(argv[0], "idata"))
		{
			index_idata(ch, argv[1]);
			return;
		}
		if (is_name(argv[0], "idescr"))
		{
			index_idescr(ch, argv[1]);
			return;
		}
		if (is_name(argv[0], "istats"))
		{
			index_istats(ch, argv[1]);
			return;
		}
	}
	ch->put("index [cdata|cdescr|cstats|idata|idescr|istats] <search string>\r\n");
}

void bug_assign(Character *ch, const char *owner, const char *priority,
		const char *title, const char *null)
{
	if (owner && priority && title && null == NULL)
	{
		if ((owner = BugPermit::allowed(owner, ch->getName())) == NULL)
		{
			ch->put("You cannot bug that person.\r\n");
			return;
		}
		priority_t p;
		if ((p = find_index(priority, str_priority)) == (ubyte1) -1)
		{
			ch->put("Priority needs to be one of the following:\r\n");
			list_flags_to(ch, (ubyte8) -1, str_priority);
			return;
		}
		if (strlen(title) > MAX_BUG_TITLE_LEN)
		{
			ch->put("Your title is too long.");
			return;
		}

		bug_writer(ch, owner, p, title);
		return;
	}
	ch->put("bug assign <player> <priority> \"<title>\"\r\n");
}

void bug_list(Character *ch, char *lim_status, char *type, char *arg)
{
	const char *lim_owner = NULL, *lim_submitter = NULL,
			*lim_priority = NULL, *lim_title = NULL;
	
	if (lim_status && type && arg
			&& find_index(lim_status, str_status) != (ubyte1) -1)
	{
		if (is_name(type, "owner"))
		{
			if ((lim_owner = get_player_name(arg)) == NULL)
			{
				ch->put("There is no such player with that name.\r\n");
				return;
			}
		}
		else if (is_name(type, "submitter"))
		{
			if ((lim_submitter = get_player_name(arg)) == NULL)
			{
				ch->put("There is no such player with that name.\r\n");
				return;
			}
		}
		else if (is_name(type, "priority"))
		{
			if (find_index(arg, str_priority) == (ubyte1) -1)
			{
				ch->put("Priority must be one of the following:");
				list_flags_to(ch, (ubyte8) -1, str_priority);
				return;
			}
			lim_priority = arg;
		}
		else if (is_name(type, "title"))
		{
			if (is_equalstr(arg, "*"))
			{
				*arg = '%';
			}
			lim_title = arg;
		}
		if (lim_submitter || lim_owner || lim_priority || lim_title)
		{
			list_bugs(ch, lim_owner, lim_submitter,
					lim_status, lim_priority, lim_title);
			return;
		}
	}
	ch->put("bug list (open|fixed|closed) (owner|submitter|priority|title) <argument>\r\n");
}

void bug_followup(Character *ch, const char *bugid)
{
	int id;

	if (bugid)
	{
		if (is_not_number(bugid) || ((id = atoi(bugid)) < 1))
		{
			ch->put("The bug-id must be a number greater than zero.\r\n");
			return;
		}
		if (bug_exists(id) == 0)
		{
			ch->put("That bug does not exist.\r\n");
			return;
		}
		followup_writer(ch, id);
		return;
	}
	ch->put("bug followup <bug-id>\r\n");
}

void bug_detail(Character *ch, const char *bugid)
{
	int id;

	if (bugid)
	{
		if (is_not_number(bugid) || ((id = atoi(bugid)) < 1))
		{
			ch->put("The bug-id must be a number greater than zero.\r\n");
			return;
		}
		if (bug_exists(id) == 0)
		{
			ch->put("That bug does not exist.\r\n");
			return;
		}
		bug_detail_to(ch, id);
		return;
	}
	ch->put("bug detail <bug-id>\r\n");
}

void bug_fix(Character *ch, const char *bugid)
{
	int id;

	if (bugid)
	{
		if (is_not_number(bugid) || ((id = atoi(bugid)) < 1))
		{
			ch->put("The bug-id must be a number greater than zero.\r\n");
			return;
		}
		if (bug_exists(id) == 0)
		{
			ch->put("That bug does not exist.\r\n");
			return;
		}
		status_t status = get_bug_status(id);
		if (status == CLOSED)
		{
			ch->put("The bug is already closed.\r\n");
			return;
		}
		if (status == FIXED)
		{
			ch->put("The bug is already fixed.\r\n");
			return;
		}
		if (ch->Imm->getLevel() < IMPLEMENTOR
				&& is_bug_owner(id, ch->getName()) == 0)
		{
			ch->put("You are not assigned to fix that bug.\r\n");
			return;
		}
		new_status_writer(ch, id, FIXED);
		return;
	}
	ch->put("bug fix <bug-id>\r\n");
}

void bug_close(Character *ch, const char *bugid)
{
	int id;

	if (bugid)
	{
		if (is_not_number(bugid) || ((id = atoi(bugid)) < 1))
		{
			ch->put("The bug-id must be a number greater than zero.\r\n");
			return;
		}
		if (bug_exists(id) == 0)
		{
			ch->put("That bug does not exist.\r\n");
			return;
		}
		status_t status = get_bug_status(id);
		if (status == OPEN)
		{
			ch->put("That bug is not yet fixed.\r\n");
			return;
		}
		if (status == CLOSED)
		{
			ch->put("That bug is already closed.\r\n");
			return;
		}
		if (ch->Imm->getLevel() < IMPLEMENTOR
				&& is_bug_submitter(id, ch->getName()) == 0)
		{
			ch->put("You did not assign that bug.\r\n");
			return;
		}
		new_status_writer(ch, id, CLOSED);
		return;
	}
	ch->put("bug close <bug-id>\r\n");
}

void bug_reopen(Character *ch, const char *bugid)
{
	int id;

	if (bugid)
	{
		if (is_not_number(bugid) || ((id = atoi(bugid)) < 1))
		{
			ch->put("The bug-id must be a number greater than zero.\r\n");
			return;
		}
		if (bug_exists(id) == 0)
		{
			ch->put("That bug does not exist.\r\n");
			return;
		}
		status_t status = get_bug_status(id);
		if (status == OPEN)
		{
			ch->put("That bug already is open.\r\n");
			return;
		}
		if (ch->Imm->getLevel() < IMPLEMENTOR
				&& can_reopen_bug(id, ch->getName()) == 0)
		{
			ch->put("You do not have permission to reopen that bug.\r\n");
			return;
		}
		new_status_writer(ch, id, OPEN);
		return;
	}
	ch->put("bug open <bug-id>\r\n");
}

void do_bug(Character *ch, char *arg, cmd_t cmd)
{
	TOKENIZER(arg, argv);
	if (argv[0])
	{
		if (is_equalstr(argv[0], "assign"))
		{
			bug_assign(ch, argv[1], argv[2], argv[3], argv[4]);
			return;
		}
		if (is_equalstr(argv[0], "close"))
		{
			bug_close(ch, argv[1]);
			return;
		}
		if (is_equalstr(argv[0], "detail"))
		{
			bug_detail(ch, argv[1]);
			return;
		}
		if (is_equalstr(argv[0], "fix"))
		{
			bug_fix(ch, argv[1]);
			return;
		}
		if (is_equalstr(argv[0], "index"))
		{
			BugPermit::indexTo(ch);
			return;
		}
		if (is_equalstr(argv[0], "followup"))
		{
			bug_followup(ch, argv[1]);
			return;
		}
		if (is_equalstr(argv[0], "list"))
		{
			bug_list(ch, argv[1], argv[2], argv[3]);
			return;
		}
		if (is_equalstr(argv[0], "reopen"))
		{
			bug_reopen(ch, argv[1]);
			return;
		}
	}
	ch->put("bug (assign | close | detail | index | followup | list | reopen)\r\n");
}

void do_bugs(Character *ch, char *arg, cmd_t cmd)
{
	const char *owner = NULL;
	if (*arg)
	{
		if ((owner = get_player_name(arg)) == NULL)
		{
			ch->put("That player does not exist.\r\n");
			return;
		}
	}
	list_bugs_owner(ch, owner ? owner : ch->getName());
}

void do_dictionary(Character *ch, char *arg, cmd_t cmd)
{
	ch->put("Dictionary:\r\n");
	dictionary.listTo(ch);
}

void do_expunge(Character *ch, char *arg, cmd_t cmd)
{
	ch->put("expunge <player>\r\n");
}
