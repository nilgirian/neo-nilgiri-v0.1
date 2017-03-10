/* **************************************************************************
    Nilgiri the Forgotten World
    (C) 2004
   ************************************************************************** */
/** \file
    \brief in game command interpreter source
 */
#include"tinyworld_character.h"
#include"interpreter.h"
#include"command.h"
#include"utils.h"
#include"err.h"

COMMANDER *commander;
COMMANDER *imm_commander;

static SOCIAL_DATA **social_list = NULL;
static int social_list_top = -1;

inline void social_act(SOCIAL_DATA *social, Container *subj,
		int has_target, Container *vict)
{
	if (has_target)
	{
		if (vict == NULL)
		{
			subj->echo(social->to_char_vict_not_found_msg
					? social->to_char_vict_not_found_msg
					: "There's no one by that name around.");
			return;
		}
		else if (vict == subj)
		{
			subj->echo(social->to_char_self_msg
					? social->to_char_self_msg
					: "You cannot socialize in that way.");
			if (social->to_others_self_msg)
			{
				(subj->inContainer())->echo(subj,
													 "$g%s", social->to_others_self_msg);
			}
		}
		else
		{
			subj->echo(vict, social->to_char_vict_msg
					? social->to_char_vict_msg
					: "You cannot socialize in that way.");

			if (social->to_others_vict_msg)
			{
				subj->inContainer()->echo(subj, vict, "$g%s",
						social->to_others_vict_msg);
			}
			if (social->to_vict_vict_msg)
			{
				vict->put(c_sex(subj));
				subj->echoTo(vict, social->to_vict_vict_msg);
			}
		}
		return;
	}
	subj->echo(social->to_char_msg);
	if (social->to_others_msg)
	{
		subj->inContainer()->echo(subj, "$g%s", social->to_others_msg);
	}
}

#define SOCIALS_HEADER \
"*******************************************************************************\r\n    SOCIALS SOCIALS SOCIALS SOCIALS SOCIALS SOCIALS SOCIALS SOCIALS SOCIALS\r\n*******************************************************************************\r\n"

void do_socials(Character *ch, char *arg, cmd_t cmd)
{
	char sp[16];
	int i, j, len;

	ch->put(SOCIALS_HEADER);
	for (i = 0; i < social_list_top+1; i++)
	{
		ch->put(social_list[i]->verb);
		ch->put(" ");
		if ((len = 15-strlen(social_list[i]->verb)) > 0)
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

static SOCIAL_DATA *get_social(const char *verb)
{
	int bot, top, mid;
	const char *p;
	size_t verblen;

	p = verb;
	verblen = 0;

	while (*p) //strlen function
	{
		p++;
		verblen++;
	}

	if (social_list_top == -1)
	{
		return NULL;
	}

	bot = 0;
	top = social_list_top;

	while (1)
	{
		mid = (bot + top) / 2;

		if (strncasecmp(social_list[mid]->verb, verb, verblen)==0)
		{
			return social_list[mid];
		}
		if (bot >= top)
		{
			//MARK need to redo the cmp() in Dictionary/Word
			for (; mid <= social_list_top; mid++) 
			{
				if (strncasecmp(social_list[mid]->verb, verb, verblen)==0)
				{
					return social_list[mid];
				}
			}
			return NULL;
		}
		if (strncasecmp(social_list[mid]->verb, verb, verblen) > 0)
		{
			top = mid - 1;
		}
		else
		{
			bot = mid + 1;
		}
	}
}

int social_exists(const char *verb)
{
	int bot, top, mid;
	
	assert(verb && *verb);

	if (social_list_top == -1)
	{
		return 0;
	}

	bot = 0;
	top = social_list_top;

	while (1)
	{
		mid = (bot + top) / 2;

		if (strcmp(social_list[mid]->verb, verb)==0)
		{
			return 1;
		}
		if (bot >= top)
		{
			return 0;
		}
		if (strcmp(social_list[mid]->verb, verb)==0)
		{
			top = mid - 1;
		}
		else
		{
			bot = mid + 1;
		}
	}
}

/** This function echos appropriate social messages to involved parties for
    a given verb.
	 \param *verb a one-word verb indicating a social action
	 \param *arg an argument string that will be analyzed for an object of the
	 social.  This is assumed not to be null.
	 \return 0 - social not found, 1 - social is found
	 \todo a better search algorithm for verbs here
 */
static int cmd_social(Container *subj, const char *verb, char *arg)
{
	char victname[MAX_SHORT_DESCR_SIZE];
	Container *vict = NULL;
	size_t verblen = strlen(verb);

	assert(arg);

	*victname = '\0';

	if (*arg)
	{
		one_argument(arg, victname);
		if (*victname)
		{
			vict = subj->inContainer()->findContent(victname, subj);
		}
	}

	SOCIAL_DATA *s;

	if ((s = get_social(verb)))
	{
		social_act(s, subj, *arg, vict);
		return 1;
	}
	return 0;
}

void free_social(SOCIAL_DATA *s)
{
	free(s->to_char_msg);
	free(s->to_others_msg);
	free(s->to_char_vict_msg);
	free(s->to_others_vict_msg);
	free(s->to_vict_vict_msg);
	free(s->to_char_vict_not_found_msg);
	free(s->to_char_self_msg);
	free(s->to_others_self_msg);
	free(s);
}

void add_social(SOCIAL_DATA *s)
{
	int bot, top, i, j;
	char *w;

	if (social_list_top == -1)
	{
		MALLOC(social_list, SOCIAL_DATA *, 1);
		social_list_top++;
		social_list[social_list_top] = s;
		return;
	}
	bot = 0;
	top = social_list_top;

	while (1)
	{
		i = (bot + top) / 2;

		if (strcmp(social_list[i]->verb, s->verb)==0)
		{
			free_social(social_list[i]);
			social_list[i] = s;
			return;
		}
		if (bot >= top)
		{
			social_list_top++;
			REALLOC(social_list, SOCIAL_DATA *, social_list_top+1);
			for (; i <= social_list_top-1; i++)
			{
				if (strcmp(social_list[i]->verb, s->verb) > 0)
				{
					for (j = social_list_top; j > i; j--)
					{
						social_list[j] = social_list[j-1];
					}
					break;
				}
			}
			social_list[i] = s;
			return;
		}
		if (strcmp(social_list[i]->verb, s->verb) > 0)
		{
			top = i - 1;
		}
		else
		{
			bot = i + 1;
		}
	}
}

inline cmd_t get_command(const char *arg,
		COMMANDER *cmdr, cmd_t last, cmd_flags_t cmds=(cmd_flags_t) -1)
{
	const char *p;
	assert(arg);

	for (cmd_t c = 0; c < last; c++)
	{
		p = arg;
		for (const char *q = cmdr[c].str; *q; q++)
		{
			if (*p == '\0')
			{
				break;
			}
			if (*p != *q)
			{
				break;
			}
			p++;
		}
		if (*p == '\0' && IS_SET(cmds, f_(c)))
		{
			return c;
		}
	}
	return last;
}

/** takes a string argument and runs an associated function.  This function
    assumes that all binary characters have been stripped.
 */
int do_command(Character *ch, char *head)
{
	cmd_t cmd;
	char *tail=NULL, *p;
	ubyte1 s;
  
	assert(head);
	//find head and tail
	s = 0;
	for (p = head; *p; p++)
	{
		if (s==0)
		{
			if (*p=='\0')
			{
				//no head
				return 0;
			}
			if (*p==' ') //MARK check to see we do not get binary at this point
			{
				*p = '\0';
				s = 1;
				continue;
			}
			continue;
		}
		else if (s==1)
		{
			if (*p=='\0')
			{
				//only haed exists
				tail = p;
				break;
			}
			if (*p==' ')
			{
				continue;
			}
			tail = p;
			break;
		}
	}
	if (tail==NULL)
	{
		tail = p;
	}
	//get rid of trailing space
	if (tail)
	{
		s = 0;
		for (p = tail; *p; p++)
		{
			s++;
		}
		for (p = (tail+s); *p; p--)
		{
			if (*p==' ')
			{
				p = '\0';
				continue;
			}
			break;
		}
	}
	if ((cmd = get_command(head, commander, NO_COMMAND))!=NO_COMMAND)
	{
		if (commander[cmd].fn==NULL)
		{
			errlog("/no function for command[%s]", command[cmd]);
		}
		else
		{
			commander[cmd].fn(ch, tail, cmd);
		}
		return 1;
	}
	if (ch->Imm)
	{
		if ((cmd = get_command(head, imm_commander, NO_IMM_COMMAND,
						ch->Imm->cmdFlags())) !=NO_IMM_COMMAND)
		{
			if (imm_commander[cmd].fn == NULL)
			{
				errlog("/no function for imm_command[%s]", imm_command[cmd]);
			}
			else
			{
				imm_commander[cmd].fn(ch, tail, cmd);
			}
			return 1;
		}
	}
	if (cmd_social(ch, head, tail))
	{
		return 1;
	}
	return 0;
}

#define INIT_SOCIAL_COUNT 3

/** Initialize the first few socials.  This should be in alphabetical order.
  */
static void create_tinyworld_socials(void)
{
	REALLOC(social_list, SOCIAL_DATA *, INIT_SOCIAL_COUNT);

	social_list_top++;
	MALLOC(social_list[social_list_top], SOCIAL_DATA, 1);
	social_list[social_list_top]->verb = dictionary.add("kiss");
	social_list[social_list_top]->min_vict_pos = DEAD;
	social_list[social_list_top]->can_see_unseen = FALSE;
	STRDUP(social_list[social_list_top]->to_char_msg, 
		"Isn't there someone you want to kiss?");
	social_list[social_list_top]->to_others_msg = NULL;
	STRDUP(social_list[social_list_top]->to_char_vict_msg, "You kiss $M.");
	STRDUP(social_list[social_list_top]->to_others_vict_msg, "$n kisses $N.");
	STRDUP(social_list[social_list_top]->to_vict_vict_msg, "$n kisses you.");
	STRDUP(social_list[social_list_top]->to_char_vict_not_found_msg, 
		"Never around when required.");
	STRDUP(social_list[social_list_top]->to_char_self_msg,
		"All the lonely people :(");
	social_list[social_list_top]->to_others_self_msg = NULL;

	social_list_top++;
	MALLOC(social_list[social_list_top], SOCIAL_DATA, 1);
	social_list[social_list_top]->verb = dictionary.add("giggle");
	social_list[social_list_top]->min_vict_pos = DEAD;
	social_list[social_list_top]->can_see_unseen = FALSE;
	STRDUP(social_list[social_list_top]->to_char_msg, "You giggle.");
	STRDUP(social_list[social_list_top]->to_others_msg, "$n giggles.");
	social_list[social_list_top]->to_char_vict_msg = NULL;
	social_list[social_list_top]->to_others_vict_msg = NULL;
	social_list[social_list_top]->to_vict_vict_msg = NULL;
	social_list[social_list_top]->to_char_vict_not_found_msg = NULL;
	social_list[social_list_top]->to_char_self_msg = NULL;
	social_list[social_list_top]->to_others_self_msg = NULL;

	social_list_top++;
	MALLOC(social_list[social_list_top], SOCIAL_DATA, 1);
	social_list[social_list_top]->verb = dictionary.add("smile");
	social_list[social_list_top]->min_vict_pos = DEAD;
	social_list[social_list_top]->can_see_unseen = FALSE;
	STRDUP(social_list[social_list_top]->to_char_msg, "You smile happily.");
	STRDUP(social_list[social_list_top]->to_others_msg, "$n smiles happily.");
	STRDUP(social_list[social_list_top]->to_char_vict_msg, "You smile at $M.");
	STRDUP(social_list[social_list_top]->to_others_vict_msg,
			"$n beams a smile at $N.");
	STRDUP(social_list[social_list_top]->to_vict_vict_msg, "$n smiles at you.");
	STRDUP(social_list[social_list_top]->to_char_vict_not_found_msg,
			"There's no one by that name around.");
	STRDUP(social_list[social_list_top]->to_char_self_msg, "You smile meekly.");
	STRDUP(social_list[social_list_top]->to_others_self_msg,
			"$n arches $s lips into a faint smile.");

	assert(INIT_SOCIAL_COUNT-1 == social_list_top);
}

void destroy_social(const char *verb)
{
	char *dbErrMsg = NULL;

	int bot, top, i, j;

	bot = 0;
	top = social_list_top;

	while (1)
	{
		i = (bot + top) / 2;
		if (strcmp(social_list[i]->verb, verb)==0)
		{
			for (; i <= social_list_top; i++)
			{
				social_list[i] = social_list[i+1];
			}
			social_list_top--;
			return;
		}
		if (bot >= top)
		{
			exitlog("/verb [%s] not found", verb);
		}
		if (strcmp(social_list[i]->verb, verb) > 0)
		{
			social_list_top = i - 1;
		}
		else
		{
			bot = i + 1;
		}
	}

	SQLITE_EXEC_PRINTF(db, "DELETE FROM social_data WHERE verb like %Q",
				NULL,
				NULL,
				dbErrMsg,
				verb);
}

void save_social(SOCIAL_DATA *s)
{
	char *dbErrMsg = NULL;

	SQLITE_EXEC_PRINTF(db, "REPLACE INTO social_data "
				"(verb, min_vict_pos, can_see_unseen,"
				" to_char_msg,"
				" to_others_msg,"
				" to_char_vict_msg,"
				" to_others_vict_msg,"
				" to_vict_vict_msg,"
				" to_char_vict_not_found_msg,"
				" to_char_self_msg,"
				" to_others_self_msg)"
				" VALUES "
				"(%Q, %Q, %u,"
				" %Q, %Q, %Q, %Q, %Q, %Q, %Q, %Q)",
				NULL,
				NULL,
				dbErrMsg,
				s->verb,
					str_position[s->min_vict_pos],
					s->can_see_unseen,
				s->to_char_msg,
				s->to_others_msg,
				s->to_char_vict_msg,
				s->to_others_vict_msg,
				s->to_vict_vict_msg,
				s->to_char_vict_not_found_msg,
				s->to_char_self_msg,
				s->to_others_self_msg);
}

static void save_socials(void)
{
	for (int i = 0; i <= social_list_top; i++)
	{
		save_social(social_list[i]);
	}
}

static void free_socials(void)
{
	for (int i = 0; i <= social_list_top; i++)
	{
		free_social(social_list[i]);
	}
	free(social_list);
	social_list = NULL;
	social_list_top = -1;
}

static int callback_load_socials(void *pArg,
		int argc, char **argv, char **columnName)
{
	social_list_top++;
	REALLOC(social_list, SOCIAL_DATA *, social_list_top+1);
	MALLOC(social_list[social_list_top], SOCIAL_DATA, 1);

	assert(argc == 11);

	for (int i = 0; i < argc; i++)
	{
		if (strcmp(columnName[i], "verb")==0)
		{
			social_list[social_list_top]->verb = dictionary.add(argv[i]);
			continue;
		}
		if (strcmp(columnName[i], "min_vict_pos")==0)
		{
			social_list[social_list_top]->min_vict_pos
				= get_index(argv[i], str_position);
			continue;
		}
		if (strcmp(columnName[i], "can_see_unseen")==0)
		{
			social_list[social_list_top]->can_see_unseen = atoi(argv[i]);
			continue;
		}
		if (strcmp(columnName[i], "to_char_msg")==0)
		{
			if (nostr(argv[i]))
			{
				exitlog("/no character message for verb:%s",
						social_list[social_list_top]->verb);
			}
			STRDUP(social_list[social_list_top]->to_char_msg, argv[i]);
			continue;
		}
		if (strcmp(columnName[i], "to_others_msg")==0)
		{
			STRDUP(social_list[social_list_top]->to_others_msg, argv[i]);
			continue;
		}
		if (strcmp(columnName[i], "to_char_vict_msg")==0)
		{
			STRDUP(social_list[social_list_top]->to_char_vict_msg, argv[i]);
			continue;
		}
		if (strcmp(columnName[i], "to_others_vict_msg")==0)
		{
			STRDUP(social_list[social_list_top]->to_others_vict_msg, argv[i]);
			continue;
		}
		if (strcmp(columnName[i], "to_vict_vict_msg")==0)
		{
			STRDUP(social_list[social_list_top]->to_vict_vict_msg, argv[i]);
			continue;
		}
		if (strcmp(columnName[i], "to_char_vict_not_found_msg")==0)
		{
			STRDUP(social_list[social_list_top]->to_char_vict_not_found_msg,
					argv[i]);
			continue;
		}
		if (strcmp(columnName[i], "to_char_self_msg")==0)
		{
			STRDUP(social_list[social_list_top]->to_char_self_msg, argv[i]);
			continue;
		}
		if (strcmp(columnName[i], "to_others_self_msg")==0)
		{
			STRDUP(social_list[social_list_top]->to_others_self_msg, argv[i]);
			continue;
		}
		exitlog("/column not used/%s", columnName[i]);
	}
	return 0;
}

static int callback_load_social(void *pArg,
		int argc, char **argv, char **columnName)
{
	SOCIAL_DATA *soc = (SOCIAL_DATA *) pArg;
	assert(argc == 11);

	for (int i = 0; i < argc; i++)
	{
		if (strcmp(columnName[i], "verb")==0)
		{
			soc->verb = dictionary.add(argv[i]);
			continue;
		}
		if (strcmp(columnName[i], "min_vict_pos")==0)
		{
			soc->min_vict_pos = get_index(argv[i], str_position);
			continue;
		}
		if (strcmp(columnName[i], "can_see_unseen")==0)
		{
			soc->can_see_unseen = atoi(argv[i]);
			continue;
		}
		if (strcmp(columnName[i], "to_char_msg")==0)
		{
			STRDUP(soc->to_char_msg, argv[i]);
			continue;
		}
		if (strcmp(columnName[i], "to_others_msg")==0)
		{
			STRDUP(soc->to_others_msg, argv[i]);
			continue;
		}
		if (strcmp(columnName[i], "to_char_vict_msg")==0)
		{
			STRDUP(soc->to_char_vict_msg, argv[i]);
			continue;
		}
		if (strcmp(columnName[i], "to_others_vict_msg")==0)
		{
			STRDUP(soc->to_others_vict_msg, argv[i]);
			continue;
		}
		if (strcmp(columnName[i], "to_vict_vict_msg")==0)
		{
			STRDUP(soc->to_vict_vict_msg, argv[i]);
			continue;
		}
		if (strcmp(columnName[i], "to_char_vict_not_found_msg")==0)
		{
			STRDUP(soc->to_char_vict_not_found_msg, argv[i]);
			continue;
		}
		if (strcmp(columnName[i], "to_char_self_msg")==0)
		{
			STRDUP(soc->to_char_self_msg, argv[i]);
			continue;
		}
		if (strcmp(columnName[i], "to_others_self_msg")==0)
		{
			STRDUP(soc->to_others_self_msg, argv[i]);
			continue;
		}
		exitlog("/column not used/%s", columnName[i]);
	}
	return 0;
}

static void load_socials(void)
{
	char *dbErrMsg = NULL;

	SQLITE_EXEC(db, "SELECT * FROM social_data ORDER BY verb",
				callback_load_socials,
				NULL,
				dbErrMsg);
}

void load_social(const char *verb, SOCIAL_DATA *soc)
{
	char *dbErrMsg = NULL;

	SQLITE_EXEC_PRINTF(db, "SELECT * FROM social_data WHERE verb like %Q",
				callback_load_social,
				soc,
				dbErrMsg,
				verb);
}

void init_interpreter_module(void)
{
	create_tinyworld_socials();
	save_socials();
	free_socials();
	load_socials();
}
