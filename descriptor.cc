/* **************************************************************************
    Nilgiri the Forgotten World
    (C) 2004
   ************************************************************************** */
/** \file
    \brief Socket descriptor class handling source
 */

#include<unistd.h>
#include<stdio.h>
#include<stdarg.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#include<pthread.h>
#include"descriptor.h"
#include"tcphost.h"
#include"err.h"
#include"main.h"
#include"library.h"
#include"tinyworld_room.h"
#include"interpreter.h"
#include"addlog.h"

/** This is a null terminated list of descriptors.  Only the functions
    new_descriptor() and free_descriptor() should ever add or remove from this
    list.
 */
Descriptor *descriptor_list = NULL;
static pthread_mutex_t mtx_gethostbyaddr;

/** Initialize the descriptor module. */
void init_descriptor_module(void)
{
	if (MAX_INP_LEN < CHUNK_SIZE)
	{
		exitlog("MAX_INP_LEN < CHUNK_SIZE");
	}
	pthread_mutex_init(&mtx_gethostbyaddr, NULL);

	if (NO_CLIENT >= (1<<sizeof(client_types_t)*CHARBITS)-1)
	{
		exitlog("/max client_types");
	}
}

/** A thread function used to lookup the hostname of a given IP in a
    descriptor. This is threaded because gethostbyaddr() can block.
    \param ctx This is a pointer to a descriptor.
	 \todo check that ctx is a descriptor?
 */
void *lookup_hostname(void *ctx)
{
	Descriptor *d;
	const char *hname;

	d = (Descriptor *) ctx;

	pthread_mutex_lock(&mtx_gethostbyaddr);
	if ((hname = get_hostname(d->getIP())) != NULL)
	{
		d->setHostname(hname);
	}
	pthread_mutex_unlock(&mtx_gethostbyaddr);
	d->resetThrID();
	pthread_exit(NULL);
	return NULL;
}

/** This is the interpreter queue event.  All input/output for individual
    descriptors happens here.
	 \param ctx This is a pointer to a descriptor.
	 \todo check that ctx is a descriptor?
 */
void interpreter_queue(void *ctx)
{
	DESCRIPTOR_CTX *descCTX = (DESCRIPTOR_CTX *) ctx;
	if (descCTX->descriptor == NULL)
	{
		free(descCTX);
		return;
	}
	descCTX->descriptor->handleInput();
	queue_event(100000, interpreter_queue, descCTX);
}

/** Creates memory for a new descriptor and adds it to descriptor_list.  Should
    the descriptor not properly instantiate, the desc will be set to -1.
 */
Descriptor::Descriptor(void)
{
	int sdesc;
	unsigned long int ipr=0;
	char *hname = NULL;

	extraction=0;
	flags = 0;

	environment = 0;
	setEnvFlag(DENV_COMPACT);
	setEnvFlag(DENV_WRAP);
	setEnvFlag(DENV_WEATHER);

	notepad.text = NULL;
	notepad.len = 0;
	notepad.fn = NULL;
	notepad.ctx = NULL;

	if ((sdesc = new_connection(&ipr)) == -1)
	{
		return;
	}

	addlog_connect(ipr);
			/*
		*((unsigned char *) &ipr),
		*((unsigned char *) &ipr+1),
		*((unsigned char *) &ipr+2),
		*((unsigned char *) &ipr+3));
		*/

	this->desc = sdesc;
	this->ip = ipr;
	this->hostname[0] = '\0';
	if (pthread_create(&this->thrID, &thr_attr, lookup_hostname, this))
	{
		exitlog("/pthread_create/%s", strerror(errno));
	}
	this->client = NO_CLIENT;
	this->interpreter = &Descriptor::handleStateConnect;
	this->msg.txt[0] = '\0';
	this->msg.len = 0;
	this->buf.txt[0] = '\0';
	this->buf.len = 0;
	this->thrID = 0;
	this->pc.temp = NULL;
	pthread_mutex_init(&this->mtxHostname, NULL);

	prev = NULL;
	if (descriptor_list==NULL)
	{
		next = NULL;
	}
	else
	{
		next = descriptor_list;
		descriptor_list->prev = this;
	}
	descriptor_list = this;

	MALLOC(interpreterCTX, DESCRIPTOR_CTX, 1);
	interpreterCTX->descriptor = this;

	next_prompt = NULL;
	prev_prompt = NULL;

	lastActiveTime = time0;
	game_stat.descriptors_created++;
}

/** Free unessential memory associated to a particular descriptor.
 */
Descriptor::~Descriptor(void)
{
	destroy_connection(this->desc);
	free(notepad.text);

	if (this->isPCDataClass())
	{
		if (this->pc.data)
		{
			if (this->isPlayingGame())
			{
				this->pc.data->setDescriptor(NULL);
			}
			else
			{
				delete pc.data;
			}
		}
	}
	else if (this->isPCDataStruct())
	{
		free(this->pc.temp);
	}
	this->pc.data = NULL;

	if (prev)
	{
		prev->next = next;
	}
	else
	{
		descriptor_list = next;
	}
	if (next)
	{
		next->prev = prev;
	}

	if (this->thrID)
	{
		pthread_cancel(this->thrID);
	}
	interpreterCTX->descriptor = NULL;
	game_stat.descriptors_destroyed++;
}

TxtQ::TxtQ(void)
{
	head = NULL;
	tail = NULL;
}

TxtQ::~TxtQ(void)
{
	TXT_BLOCK *block, *block_next;

	for (block = this->head; block; block = block_next)
	{
		block_next = block->next;
		free(block->txt);
		free(block);
	}
}

/** Creates a new text block and pushes it onto the tail of the text queue.
    \param txt This must be a string message and point to allocated memory.
 */
void TxtQ::push(char *txt)
{
	TXT_BLOCK *new_block;

	MALLOC(new_block, TXT_BLOCK, 1);
	new_block->txt = txt;
	new_block->next = NULL;
	if (this->tail==NULL)
	{
		this->head = new_block;
	}
	else
	{
		this->tail->next = new_block;
	}
	this->tail = new_block;
}

/** This allocates memory for string to be put onto the text queue output.
    We assume all text to be sent may end in a \\n\\r order (and never in a
    \\r\\n order).
    \param format The string message format
    \param ... The variable list of arguments
    \todo Check the LINE_LENGTH here to be optimal so we are not always
    realloc'ing text.
	 \todo Use the Character::message() method of va_complete.
	 \todo removed returnSentencClient() from this function
 */
void Descriptor::print(const char *format, ...)
{
	int n, size = LINE_LENGTH+3; /* Line length plus \n, \r and \0 */
	char *mesg;

	va_list vargs;
	MALLOC(mesg, char, size+2); // +2 for additional \r\n
	while (1)
	{
		va_start(vargs, format);
		n = vsnprintf(mesg, (size_t) size, format, vargs);
		va_end(vargs);
		if (n > -1 && n < size)
		{
			if (envFlag(DENV_COLOR)==0)
			{
				strip_color(mesg);
			}
			this->out.push(mesg);
			return;
		}
		if (n > -1)
		{
			size = n+3; // add two more just in case for addition \r\n
		}
		else
		{
			errlog("/vsnprintf/precise char count not returned");
			return;
		}
		if ((mesg = (char *) realloc(mesg, (size_t) size))==NULL)
		{
			errlog("/realloc/%s", strerror(errno));
			return;
		}
	}
}

/** This allocates memory for string to be put onto the text queue output.
    We assume all text to be sent may end in a \\r\\n order (and never in a
    \\n\\r order).
    \param mesg The string message.
	 \todo removed returnSentencClient() from this function
 */
void Descriptor::put(const char *mesg)
{
	char *s, *q;
	size_t size;

	//MARK this works temporarily prompt needs its own out.push function
	if (envFlag(DENV_SPLITSCREEN))
	{
		moveCursor(SCROLL, 1);
	}
	if (startTextRound())
	{
		resetStartTextRound();
	}
	size = strlen(mesg);

	//MARK I had to change this to size+6 from size+2 for valgrind, why???
	MALLOC(s, char, size+6);
	q = s;
	while (*mesg)
	{
		*q = *mesg;
		mesg++;
		q++;
	}
	*q = '\0';
	if (envFlag(DENV_COLOR)==0)
	{
		strip_color(s);
	}
	out.push(s);
}

void Descriptor::clearScreen(void)
{
	out.push(strdup("\x1B[2J"));
}

void Descriptor::scroll(int sta, int fin)
{
	char esc[16];
	sprintf(esc, "\x1B[%d;%dr", sta, fin);
	out.push(strdup(esc));
}

void Descriptor::scrollAll(void)
{
	out.push(strdup("\x1B[r"));
}

void Descriptor::cursorHome(int sta, int fin)
{
	char esc[16];
	sprintf(esc, "\x1B[%d;%dH", sta, fin);
	out.push(strdup(esc));
}

void Descriptor::moveCursor(int sta, int fin)
{
	char esc[16];
	sprintf(esc, "\x1B[%d;%df", sta, fin);
	out.push(strdup(esc));
}

void Descriptor::eraseDown(void)
{
	out.push(strdup("\x1B[J"));
}

/** This processes the socket input and allocates new memory for string text
    to be queued in the descriptor input queue.
    \param d The descriptor 
    \return 0 on success, -1 on failure
 */
int Descriptor::processInp(void)
{
	char *txt;
	lastActiveTime = time0;
	switch(tcpread(this->desc, this->msg.txt, &this->msg.len, MAX_INP_LEN))
	{
		case 0:
			STRDUP(txt, this->msg.txt);
			this->inp.push(txt);
			this->msg.txt[0] = '\0';
			this->msg.len = 0;
			return 0;
		case 1:
			return 0;
		case 2:
			put("\r\n\r\nYou are being disconnected because your message is too long..\r\n");
			extract();
			return -1;
		default:
			if (isPlayingGame() && pc.data->inContainer())
			{
				addlog_link_loss(pc.data);
				pc.data->inContainer()->echo(pc.data, "$g$n has lost $s link.");
			}
			extract();
	}
	return -1;
}

/** Processes the output queue of the descriptor to send messages back to the
    client
    \return 0 on success, 1 on failure
 */
int Descriptor::processOut(void)
{
	if ((isPlayingGame()==0 || pc.data==NULL || out.isEmpty()
				|| pc.data->isDead() || shutting_down())==0)
	{
		if (toGamePrompt())
		{
			resetToGamePrompt();
			put("\r\n");
		}
		else
		{
			if (envFlag(DENV_SPLITSCREEN))
			{
				moveCursor(SCROLL+1, 1);
				eraseDown();
			}
		}
		if (envFlag(DENV_SPLITSCREEN))
		{
			moveCursor(SCROLL+1, 1);
		}
		if (interpreter == &Descriptor::handleStateGameInterpreter)
		{
			pc.data->prompt();
		}
		if (envFlag(DENV_SPLITSCREEN))
		{
			moveCursor(SCROLL+2, 1);
		}
		setStartTextRound();
	}
	while (this->out.isEmpty() == 0)
	{
		switch(tcpwrite(this->desc, this->out.getTxt()))
		{
			case 0: 
				this->out.pop();
			case 1:
				break;
			default:
				extract();
				return 1;
		}
	}
	return 0;
}

/** This function does not create any new memory.
    \return The hostname of the descriptor.
 */
char *Descriptor::getHostname(void)
{
	static char hname[MAXHOSTNAMELEN];
	pthread_mutex_lock(&this->mtxHostname);
	strcpy(hname, this->hostname);
	pthread_mutex_unlock(&this->mtxHostname);
	return hname;
}

/** Set the hostname of the descriptor. */
inline void Descriptor::setHostname(const char *hname)
{
	pthread_mutex_lock(&this->mtxHostname);
	strcpy(this->hostname, hname);
	pthread_mutex_unlock(&this->mtxHostname);
}

/** This function strips the unsavory characters, the newline, the leading and
    trailing whitespace and finally stores it in the buffer.
    \todo This function maybe should assume that sentence input can come in
    several packets and it is up to this function to piece whole sentences
    together and store that in the buffer.  For now we'll just assume
    we get nice clean packets of string.
 */
void Descriptor::handleInput(void)
{
	char *p, *q;
	if (inp.isEmpty())
	{
		return;
	}

	buf.len = 0;
	buf.txt[0] = '\0';

	p = buf.txt;
	q = inp.getTxt();

	//We want our intro spaces in text editor
	if (characterWriting()==0)
	{
		while (*q != '\n' && (*q == ' ' || is_print(*q) == 0))
		{
			q++;
		}
	}

	while (*q)
	{
		if (is_print(*q))
		{
			*p = *q;
			buf.len++;
			p++;
		}
		if (*q == '\n')
		{
			break;
		}
		if (*q == '\b')
		{
			if (buf.len)
			{
				p--;
				buf.len--;
			}
		}
		q++;
	}
	*p = '0';

	while (buf.len > 0)
	{
		p--;
		buf.len--;
		if (*p != ' ')
		{
			buf.len++;
			p++;
			*p = '\0';
			break;
		}
	}
	inp.pop();
	interpret();
}

/** This makes an english description of the player.
    \param pc The BASIC_PC structure of the player
    \return The description.
 */
const char *basic_description(const BASIC_PC *pc)
{
	static char buffer[4096];

	buffer[0] = '\0';

	strcat(buffer, "Before you is a ");
	strcat(buffer, str_demeanor[pc->demeanor]);
	strcat(buffer, " looking ");
	strcat(buffer, str_gender[pc->gender]);
	strcat(buffer, " ");
	strcat(buffer, str_race[pc->race]);
	strcat(buffer, ". ");
	strcat(buffer, str_gender_e[pc->gender]);
	strcat(buffer, " has ");
	if (pc->hair_length == HAIR_BALD)
	{
		strcat(buffer, "a bald head");
	}
	else
	{
		strcat(buffer, str_hair_length[pc->hair_length]);
		strcat(buffer, " ");
		strcat(buffer, str_hair_color[pc->hair_color]);
		strcat(buffer, " ");
		strcat(buffer, str_hair_shape[pc->hair_shape]);
		strcat(buffer, " hair");
	}
	strcat(buffer, ", ");
	strcat(buffer, str_eye_shape[pc->eye_shape]);
	strcat(buffer, " ");
	strcat(buffer, str_eye_color[pc->eye_color]);
	strcat(buffer, " eyes, and a ");
	strcat(buffer, str_skin_complexion[pc->skin_complexion]);
	strcat(buffer, " complexion. ");
	strcat(buffer, str_gender_e[pc->gender]);
	strcat(buffer, " is of ");
	strcat(buffer, str_height_type[pc->height]);
	strcat(buffer, " height and ");
	strcat(buffer, str_weight_type[pc->weight]);
	strcat(buffer, " weight for ");
	strcat(buffer, str_gender_s[pc->gender]);
	strcat(buffer, " kind.\r\n");
	return buffer;
}

/** Rolls four six sided dice and discard the lowest roll.
    \return a stat.
 */
stat_t roll_stat(void)
{
	stat_t roll[4];

	roll[0] = dice(6);
	roll[1] = dice(6);
	roll[2] = dice(6);
	roll[3] = dice(6);

	return roll[0] + roll[1] + roll[2] + roll[3]
								- min(roll[3], min(roll[2], min(roll[1], roll[0])));
}

/** Given a set of particular stats, apply the race bonus modifiers to the
    set of stats.
    \param stat The stats.
    \param race The race.
 */
static inline void apply_race_bonus(stat_t *stat, race_t race)
{
	for (stat_t s = 0; s <= CHARISMA; s++)
	{
		stat[s] += stat_race_bonus[race][s];
	}
}

/** Randomly roll stats.
    \param stat The structure to hold the stats.
 */
void roll_random_stats(stat_t *stat)
{
	stat_t s, sum, roll;
	do
	{
		sum = 0;
		for (s = 0; s <= CONSTITUTION; s++)
		{
			roll = roll_stat();
			stat[s] = roll;
			sum += roll;
		}
	}
	while (sum < MIN_TOTAL_STATS || sum > MAX_TOTAL_STATS);

	stat[CHARISMA] = dice(3,6);
	stat[STRENGTH_ADD] = stat[STRENGTH] == 18 ? dice(1, 100) : 0;
}

/** Roll a set of stats randomly and apply the race bonuses.
    \param stats The set of stats.
    \param race The race.
 */
void roll_random_stats(stat_t *stat, race_t race)
{
	roll_random_stats(stat);
	apply_race_bonus(stat, race);
}

/** Roll a set of stats, order them according to profession and apply the
    race bonuses.
    \param stat The set of stats.
    \param profession The profession.
    \param race The race.
 */
void roll_stats(stat_t *stat, profession_t profession, race_t race)
{
	stat_t tmp_stat[NO_STAT], index, i, j;

	roll_random_stats(tmp_stat);

	for (i = 1; i <= CONSTITUTION; i++)
	{
		index = tmp_stat[i];
		j = i;
		while ((j > 0) && (tmp_stat[j-1] > index))
		{
			tmp_stat[j] = tmp_stat[j-1];
			j = j - 1;
		}
		tmp_stat[j] = index;
	}
	for (i = 0; i <= CONSTITUTION; i++)
	{
		stat[rank_profession_stats[profession][i]] = tmp_stat[CONSTITUTION-i];
	}

	stat[CHARISMA] = tmp_stat[CHARISMA];
	stat[STRENGTH_ADD] = stat[STRENGTH] == 18 ? dice(1, 100) : 0;
	apply_race_bonus(stat, race);
}

/** Makes a text representation given a set of stats.
    \param stat The set of stats.
    \param race Race is needed to determine the bonuses for the text.
    \return A text representation of the stats.
 */
static char *list_stats(stat_t *stat, race_t race)
{
	static char buf[1024];
	sbyte1 bonus;

	buf[0] = '\0';
	for (stat_t s = 0; s <= CHARISMA; s++)
	{
		if ((bonus = stat_race_bonus[race][s]))
		{
			sprintf(buf+strlen(buf), "  %-13s: %2d %c %d = %2d\r\n",
					str_stat[s],
					stat[s] - bonus,
					bonus < 0 ? '-' : '+',
					bonus < 0 ? -bonus : bonus,
					stat[s]);
		}
		else
		{
			sprintf(buf+strlen(buf), "  %-13s: %2d\r\n", str_stat[s], stat[s]);
		}
		if (s == CONSTITUTION)
		{
			strcat(buf, "  ---\r\n");
		}
	}
	if (stat[STRENGTH_ADD])
	{
		sprintf(buf+strlen(buf), "  %-13s: %2d\r\n",
		str_stat[STRENGTH_ADD], stat[STRENGTH_ADD]);
	}
	return buf;
}

#define STR_QCLIENT K_YEL "Do you want text color? " K_WHT

#define STR_QNAME K_YEL "By what name do you wish to be known? " K_WHT

#define STR_QPASSWORD K_YEL "What is the password? " K_WHT

#define STR_BADPASSWORD K_YEL "That password is incorrect.\r\r\n"

#define STR_QNEWPASSWORD K_YEL "Give me a new password for %s: "

#define STR_QNEWPASSWORDOLD K_YEL "Give me the current password for %s: "

#define STR_YES_NO K_YEL "Type 'yes' or 'no': " K_WHT

#define STR_QMARK K_YEL "? " K_WHT

#define STR_QEMAIL_FULL K_YEL \
"An e-mail address is used by the game to identify multiple characters\r\n"\
"that belong to a single person.  Sharing characters is also allowed, but\r\n"\
"each character must have a primary owner who shall be identified here.\r\n"\
"Upon completing the creation of your character\r\n\r\n"\
K_BLD_YEL\
"                   A TEMPORARY PASSWORD WILL BE SENT TO YOU!\r\n\r\n"\
K_YEL\
"Your e-mail address is private and you will not receive any further\r\n"\
"e-mail messages from Nilgiri nor from its administrators.\r\n\r\n"

#define STR_QEMAIL K_YEL "Please enter your e-mail address: " K_WHT

#define STR_NOMATCHEMAIL K_YEL "\r\nYour e-mail address does not match.\r\n"

#define STR_QEMAILC K_YEL "Confirm by retyping your e-mail adddress: " K_WHT

#define STR_QGENDER K_YEL "Are you female or male? " K_WHT

#define STR_QRACE K_YEL "\r\nSelect a race:\r\n"

#define STR_QHAIRLENGTH K_YEL "\r\nSelect a hair length:\r\n"

#define STR_QHAIRCOLOR K_YEL "\r\nSelect a hair color:\r\n"

#define STR_QHAIRSHAPE K_YEL "\r\nSelect a hair style:\r\n"

#define STR_QEYECOLOR K_YEL "\r\nSelect an eye color:\r\n"

#define STR_QEYESHAPE K_YEL "\r\nSelect an eye shape:\r\n"

#define STR_QSKINCOMPLEXION K_YEL "\r\nSelect a skin tone:\r\n"

#define STR_QHEIGHT K_YEL "\r\nSelect a height type:\r\n"

#define STR_QWEIGHT K_YEL "\r\nSelect a weight type:\r\n"

#define STR_QDEMEANOR K_YEL "\r\nSelect a demeanor:\r\n"

#define STR_QSELF_INTRO K_YEL "\r\nThis is your description:\r\n\r\n"
#define STR_QSELF K_YEL "\r\nDoes this description suite you (yes/no) ? " K_WHT

#define STR_QHOMELAND K_YEL "\r\nSelect your homeland:\r\n  Jora\r\n  Argoceania\r\n"

#define STR_QORIENTSTATS_FULL K_YEL "\r\n\
If you choose profession oriented stats, you will choose your profession\r\n\
first, and then your stats will be ordered such that they are most\r\n\
appropriate for for your profession. If you choose random stats, your stats\r\n\
will be in no particular order and you can choose your class according to\r\n\
your stats.\r\n\r\n"

#define STR_QORIENTSTATS K_YEL \
"Do you want 'profession' or 'random' oriented stats? " K_WHT

#define STR_QPROFESSION K_YEL "\r\nSelect a profession:\r\n"

#define STR_QSTATS K_YEL "\r\nYour stats are now-\r\n"

#define STR_QSTATS_REROLL K_YEL "Do you wish to reroll? " K_WHT

#define STR_QHANDED K_YEL "\r\nSelect your handedness:\r\n"

#define STR_PRESSRETURN "\r\n*** PRESS RETURN:" K_WHT

#define STR_QMOTDPASSWORD "\r\r\n\
" K_GRN "***  " K_BLD_YEL " A password has been sent to you at:\r\r\n\
" K_GRN "***  " K_BLD_YEL "    %s\r\r\n\
" K_GRN "***  " K_BLD_YEL " Please enter in this password now.\r\r\n"

#define STR_NOSAVEBASICPC "\r\n" K_BLD_YEL "\
Some technical problems have occured.\r\n\
Sorry, but You will to recreate your character again.\r\n\
Goodbye!\r\n" K_WHT

#define STR_OBQUOTE "\"Live life to the fulliest.\"\r\n-- Russ\r\n"

#define STR_QMENU K_GRN\
"\r\n\
%s\r\n\
ObQuote-\
\r\n\
\r\n\
" K_BLD_YEL "%s" K_GRN "\
\r\n\
Welcome to Nilgiri the Forgotten World\r\n\
\r\n\
0) Exit the Forgotten World.\r\n\
1) Enter the game.\r\n\
2) Update your e-mail address.\r\n\
3) Change your password.\r\n\
\r\n\
   Make your choice: " K_WHT

#define STR_NEWEMAIL K_YEL "\r\r\n\
A new password has been generated and sent to your new e-mail address.\r\r\n"

#define STR_GOODBYE K_BLD_YEL "\r\nGoodbye!\r\n\r\n" K_WHT

/** Handles what the user sees upon connecting to Nilgiri.
 */
void Descriptor::handleStateConnect(void)
{
	this->put(Credits->next());
	this->put(STR_QCLIENT);
	this->interpreter = &Descriptor::handleStateQClient;
	interpreter_queue(interpreterCTX);
}

void Descriptor::handleStateQClient(void)
{
	struct client_key_t
	{
		size_t size;
		char *txt;
	};

	/** The keyword to identifying the type of client the user has.
	*/
	const struct client_key_t client_key[NO_CLIENT] =
	{
		{ 24, "UsingNilgiriAppletClient" },
		{ 22, "UsingNilgiriJ2MEClient" }, 
		{ 25, "UsingNilgiriSymbianClient" },
		{ 21, "UsingNilgiriAimClient" },
		{ 24, "UsingNilgiriPuebloClient" }
	};

	client = NO_CLIENT;
	if (buf.len && strncasecmp("yes", buf.txt, buf.len)==0)
	{
		setEnvFlag(DENV_COLOR);
	}
	else
	{
		for (int i = 1; i < NO_CLIENT; i++)
		{
			if (buf.len == client_key[i].size
			&& strcmp(buf.txt, client_key[i].txt)==0)
			{
				client = i;
				break;
			}
		}
	}
	put(Greeting->next());
	put(STR_QNAME);
	interpreter = &Descriptor::handleStateQName;
}

/** 
    \todo Badname check
    \todo relogin
 */
void Descriptor::handleStateQName(void)
{
	char *p;

	if (buf.len==0)
	{
		put(STR_QNAME);
		return;
	}
	for (p = buf.txt; *p; p++)
	{
		if (*p == ' ')
		{
			put("Your name may not contain spaces.\r\n" STR_QNAME);
			return;
		}
		else if (is_not_alpha(*p))
		{
			put("Your name must only contain alphabetical characters.\r\n" STR_QNAME);
			return;
		}
	}
	if (buf.len > MAX_NAME_LEN)
	{
		print("Your name must be at most %d characters long.\r\n" STR_QNAME,
		MAX_NAME_LEN);
		return;
	}
	else if (buf.len < MIN_NAME_LEN)
	{
		print("Your name must be at least %d characters long.\r\n" STR_QNAME,
		MIN_NAME_LEN);
		return;
	}
	to_upper(buf.txt);
	MALLOC(pc.temp, BASIC_PC, 1);
	strcpy(pc.temp->name, buf.txt);
	pc.temp->birth_host[0] = '\0';
fprintf(stderr, "A birth_host: [%s]\n", pc.temp->birth_host);

	LOAD_PC_DATA *load_pc_data;
	if ((load_pc_data = load_player(pc.temp->name)))
	{
		free(pc.temp);
		pc.data = new Player(load_pc_data, this);
		put(STR_QPASSWORD);
		interpreter = &Descriptor::handleStateQPassword;
		put(echo_off);
		return;
	}
	print(K_YEL "Did I get that right, %s (yes/no) ? " K_WHT, pc.temp->name);
	interpreter = &Descriptor::handleStateQNameC;
}

void Descriptor::handleStateQNameC(void)
{
	if (buf.len != 0)
	{
		if (strcasecmp(buf.txt, "yes")==0)
		{
			put(K_BLD_GRN "\r\nWelcome to Nilgiri the Forgotten World!\r\n\r\n"
				STR_QEMAIL_FULL STR_QEMAIL);
			interpreter = &Descriptor::handleStateQEmail;
			return;
	 	}
		if (strcasecmp(buf.txt, "no")==0)
		{
			put(K_YEL "Okay, what is it then? ");
			interpreter = &Descriptor::handleStateQName;
			return;
		}
	}
	put(STR_YES_NO);
}

void Descriptor::handleStateQPassword(void)
{
	put(echo_on);
	Player *player;

	if (buf.len != 0)
	{
		if (pc.data->isPassword(buf.txt))
		{
			if ((player = getPlaying(pc.data)))
			{
				if (player->getDescriptor() == NULL || player->inContainer())
				{
					void count_bugs(Character *);
					void count_fixes(Character *);
					pc.data->setDescriptor(NULL);
					delete pc.data; //MARK is this ugly?
					player->reconnect(this);
					interpreter = &Descriptor::handleStateGameInterpreter;
					addlog_reconnect_player(player);
					put("\r\nWelcome back! You are being reconnected.\r\n");
					count_bugs(player);
					count_fixes(player);
					player->inContainer()->echo(player, "$g$n has reconnected.");
					return;
				}
				else
				{
					player->put("\r\n\r\nYou are being reconnected. Goodbye!\r\n");
					player->getDescriptor()->extract();
				}
			}
			put("\r\n" K_GRN);
			put(Motd);
			put(STR_PRESSRETURN);
			pc.data->setLastLogon(time0);
			interpreter = &Descriptor::handleStateMotd;
			return;
		}
	}
	//print("What is the password for %s: ", pc.temp->name); //MARK
	put(STR_BADPASSWORD);
	put(STR_QPASSWORD);
	put(echo_off);
}

void Descriptor::handleStateQEmail(void)
{
	if (buf.len == 0)
	{
		put(STR_QEMAIL);
		return;
	}
	if (is_invalid_email(buf.txt))
	{
		put(K_YEL "That is an invalid e-mail address.\r\n");
		put(STR_QEMAIL);
		return;
	}
	strcpy(pc.temp->email, buf.txt);
	put(STR_QEMAILC);
	interpreter = &Descriptor::handleStateQEmailC;
}

void Descriptor::handleStateQEmailC(void)
{
	if (buf.len == 0)
	{
		put(STR_QEMAILC);
		return;
	}
	if (strcmp(pc.temp->email, buf.txt))
	{
		put(STR_NOMATCHEMAIL STR_QEMAIL);
		interpreter = &Descriptor::handleStateQEmail;
		return;
	}
	put("\r\n" STR_QGENDER);
	interpreter = &Descriptor::handleStateQGender;
}

void Descriptor::handleStateQGender(void)
{
	if (buf.len != 0)
	{
		for (gender_t g = 0; g <= MALE; g++)
		{
			if (strcasecmp(buf.txt, str_gender[g])==0)
			{
				pc.temp->gender = g;
				put(STR_QRACE);
				put(strarray_to_str(str_race, PIXIE+1));
				put(STR_QMARK);
				interpreter = &Descriptor::handleStateQRace;
				return;
			}
		}
	}
	put(STR_QGENDER);
}

void Descriptor::handleStateQRace(void)
{
	if (buf.len != 0)
	{
		for (race_t r = 0; r <= PIXIE; r++)
		{
			if (strcasecmp(buf.txt, str_race[r])==0)
			{
				pc.temp->race = r;
				put(K_YEL "\r\n");
				put(str_race_descr[pc.temp->race]);
				print("\r\nDo you wish to be %s (yes/no) ? " K_WHT,
					str_race[pc.temp->race]);
				interpreter = &Descriptor::handleStateQRaceC;
				return;
			}
		}
	}
	put(STR_QRACE);
	put(strarray_to_str(str_race, PIXIE));
	put(STR_QMARK);
}

void Descriptor::handleStateQRaceC(void)
{
	if (buf.len != 0)
	{
		if (strcasecmp(buf.txt, "yes")==0)
		{
			put(STR_QHAIRLENGTH);
			put(strarray_to_str(str_race_hair_length[pc.temp->race]));
			put(STR_QMARK);
			interpreter = &Descriptor::handleStateQHairLength;
			return;
		}
		if (strcasecmp(buf.txt, "no")==0)
		{
			put(STR_QRACE);
			put(strarray_to_str(str_race, PIXIE));
			interpreter = &Descriptor::handleStateQRace;
			return;
		}
	}
	put(STR_YES_NO);
}

void Descriptor::handleStateQHairLength(void)
{
	if (buf.len != 0)
	{
		for (hair_length_t h = 0; h < NO_HAIR_LENGTH; h++)
		{
			if (strcasecmp(buf.txt, str_race_hair_length[pc.temp->race][h])==0)
			{
				if ((pc.temp->hair_length = h)==HAIR_BALD)
				{
					pc.temp->hair_color = NO_HAIR_COLOR;
					pc.temp->hair_shape = NO_HAIR_SHAPE;
					put(STR_QEYECOLOR);
					put(strarray_to_str(str_race_eye_color[pc.temp->race]));
					put(STR_QMARK);
					interpreter = &Descriptor::handleStateQEyeColor;
					return;
				}
				else
				{
					put(STR_QHAIRCOLOR);
					put(strarray_to_str(str_race_hair_color[pc.temp->race]));
					put(STR_QMARK);
					interpreter = &Descriptor::handleStateQHairColor;
					return;
				}
			}
		}
	}
	put(STR_QHAIRLENGTH);
	put(strarray_to_str(str_race_hair_length[pc.temp->race]));
	put(STR_QMARK);
}

void Descriptor::handleStateQHairColor(void)
{
	if (buf.len != 0)
	{
		for (hair_color_t h = 0; h < NO_HAIR_COLOR; h++)
		{
			if (strcasecmp(buf.txt, str_race_hair_color[pc.temp->race][h])==0)
			{
				pc.temp->hair_color = h;
				put(STR_QHAIRSHAPE);
				put(strarray_to_str(str_race_hair_shape[pc.temp->race]));
				put(STR_QMARK);
				interpreter = &Descriptor::handleStateQHairShape;
				return;
			}
		}
	}
	put(STR_QHAIRCOLOR);
	put(strarray_to_str(str_race_hair_color[pc.temp->race]));
	put(STR_QMARK);
}

void Descriptor::handleStateQHairShape(void)
{
	if (buf.len != 0)
	{
		for (hair_shape_t h = 0; h < NO_HAIR_SHAPE; h++)
		{
			if (strcasecmp(buf.txt, str_race_hair_shape[pc.temp->race][h])==0)
			{
				pc.temp->hair_shape = h;
				put(STR_QEYECOLOR);
				put(strarray_to_str(str_race_eye_color[pc.temp->race]));
				put(STR_QMARK);
				interpreter = &Descriptor::handleStateQEyeColor;
				return;
			}
		}
	}
	put(STR_QHAIRSHAPE);
	put(strarray_to_str(str_race_hair_shape[pc.temp->race]));
	put(STR_QMARK);
}

void Descriptor::handleStateQEyeColor(void)
{
	if (buf.len != 0)
	{
		for (eye_color_t e = 0; e < NO_EYE_COLOR; e++)
		{
			if (strcasecmp(buf.txt, str_race_eye_color[pc.temp->race][e])==0)
			{
				pc.temp->eye_color = e;
				put(STR_QEYESHAPE);
				put(strarray_to_str(str_race_eye_shape[pc.temp->race]));
				put(STR_QMARK);
				interpreter = &Descriptor::handleStateQEyeShape;
				return;
			}
		}
	}
	put(STR_QEYECOLOR);
	put(strarray_to_str(str_race_eye_color[pc.temp->race]));
	put(STR_QMARK);
};

void Descriptor::handleStateQEyeShape(void)
{
	if (buf.len != 0)
	{
		for (eye_shape_t e = 0; e < NO_EYE_SHAPE; e++)
		{
			if (strcasecmp(buf.txt, str_race_eye_shape[pc.temp->race][e])==0)
			{
				pc.temp->eye_shape = e;
				put(STR_QSKINCOMPLEXION);
				put(strarray_to_str(str_race_skin_complexion[pc.temp->race]));
				put(STR_QMARK);
				interpreter = &Descriptor::handleStateQSkinComplexion;
				return;
			}
		}
	}
	put(STR_QEYESHAPE);
	put(strarray_to_str(str_race_eye_shape[pc.temp->race]));
	put(STR_QMARK);
}

void Descriptor::handleStateQSkinComplexion(void)
{
	if (buf.len != 0)
	{
		for (skin_complexion_t s = 0; s < NO_SKIN_COMPLEXION; s++)
		{
			if (strcasecmp(buf.txt, str_race_skin_complexion[pc.temp->race][s])==0)
			{
				pc.temp->skin_complexion= s;
				put(STR_QHEIGHT);
				put(strarray_to_str(str_height_type));
				put(STR_QMARK);
				interpreter = &Descriptor::handleStateQHeight;
				return;
			}
		}
	}
	put(STR_QSKINCOMPLEXION);
	put(strarray_to_str(str_race_skin_complexion[pc.temp->race]));
	put(STR_QMARK);
}

void Descriptor::handleStateQHeight(void)
{
	if (buf.len != 0)
	{
		for (height_types_t h = 0; h < NO_HEIGHT; h++)
		{
			if (strcasecmp(buf.txt, str_height_type[h])==0)
			{
				pc.temp->height = h; //MARK this is wrong
				put(STR_QWEIGHT);
				put(strarray_to_str(str_weight_type));
				put(STR_QMARK);
				interpreter = &Descriptor::handleStateQWeight;
				return;
			}
		}
	}
	put(STR_QHEIGHT);
	put(strarray_to_str(str_height_type));
	put(STR_QMARK);
}

void Descriptor::handleStateQWeight(void)
{
	if (buf.len != 0)
	{
		for(weight_types_t w = 0; w < NO_WEIGHT; w++)
		{
			if (strcasecmp(buf.txt, str_weight_type[w])==0)
			{
				pc.temp->weight = w; //MARK this is wrong
				put(STR_QDEMEANOR);
				put(strarray_to_str(str_demeanor));
				put(STR_QMARK);
				interpreter = &Descriptor::handleStateQDemeanor;
				return;
			}
		}
	}
	put(STR_QWEIGHT);
	put(strarray_to_str(str_weight_type));
	put(STR_QMARK);
}

void Descriptor::handleStateQDemeanor(void)
{
	if (buf.len != 0)
	{
		for (demeanor_t d = 0; d < NO_DEMEANOR; d++)
		{
			if (strcasecmp(buf.txt, str_demeanor[d])==0)
			{
				pc.temp->demeanor = d;
				put(STR_QSELF_INTRO);
				put(basic_description(pc.temp));
				put(STR_QSELF);
				interpreter = &Descriptor::handleStateQSelf;
				return;
			}
		}
	}
	put(STR_QDEMEANOR);
	put(strarray_to_str(str_demeanor));
	put(STR_QMARK);
}

void Descriptor::handleStateQSelf(void)
{
	if (buf.len != 0)
	{
		if (strcasecmp(buf.txt, "yes")==0)
		{
			put("\r\n");
			for (homeland_t l = 0; l <= ARGO; l++)
			{
				print(K_YEL "%s\r\n%s\r\n", caps(str_homeland[l]), str_homeland_d[l]);
			}
			put(STR_QHOMELAND);
			put(STR_QMARK);
			interpreter = &Descriptor::handleStateQHomeland;
			return;
		}
		if (strcasecmp(buf.txt, "no")==0)
		{
			put(STR_QGENDER);
			put(STR_QMARK);
			interpreter = &Descriptor::handleStateQGender;
			return;
		}
	}
	put(STR_QSELF_INTRO);
	put(basic_description(pc.temp));
	put(STR_QSELF);
}

void Descriptor::handleStateQHomeland(void)
{
	if (buf.len != 0)
	{
		for (homeland_t l = 0; l <= ARGO; l++)
		{
			if (strcasecmp(buf.txt, str_homeland[l])==0)
			{
				pc.temp->homeland = l;
				put(STR_QORIENTSTATS_FULL);
				put(STR_QORIENTSTATS);
				interpreter = &Descriptor::handleStateQOrientStats;
				return;
			}
		}
	}
	put(STR_QHOMELAND);
	put(STR_QMARK);
}

void Descriptor::handleStateQOrientStats(void)
{
	if (buf.len != 0)
	{
		if (strcasecmp(buf.txt, "profession")==0)
		{
			put(STR_QPROFESSION);
			put(strarray_to_str(str_profession));
			put(STR_QMARK);
			interpreter = &Descriptor::handleStateQProfession;
			return;
		}
		if (strcasecmp(buf.txt, "random")==0)
		{
			put(STR_QSTATS);
			roll_random_stats(pc.temp->stat, pc.temp->race);
			put(list_stats(pc.temp->stat, pc.temp->race));
			put(STR_QSTATS_REROLL);
			interpreter = &Descriptor::handleStateQRandStats;
			return;
		}
	}
	put(STR_QORIENTSTATS);
}

void Descriptor::handleStateQProfession(void)
{
	if (buf.len != 0)
	{
		for (profession_t p = 0; p < NO_PROFESSION; p++)
		{
			if (strcasecmp(buf.txt, str_profession[p])==0)
			{
				pc.temp->profession = p;
				roll_stats(pc.temp->stat, pc.temp->profession, pc.temp->race);
				put(STR_QSTATS);
				put(list_stats(pc.temp->stat, pc.temp->race));
				put(STR_QSTATS_REROLL);
				interpreter = &Descriptor::handleStateQStats;
				return;
			}
		}
	}
	put(STR_QPROFESSION);
	put(strarray_to_str(str_profession));
	put(STR_QMARK);
}

void Descriptor::handleStateQStats(void)
{
	if (buf.len != 0)
	{
		if (strcasecmp(buf.txt, "yes")==0)
		{
			put(STR_QSTATS);
			roll_stats(pc.temp->stat, pc.temp->profession, pc.temp->race);
			put(list_stats(pc.temp->stat, pc.temp->race));
			put(STR_QSTATS_REROLL);
			return;
		}
		if (strcasecmp(buf.txt, "no")==0)
		{
			put(STR_QHANDED);
			put(strarray_to_str(str_handed, LEFT+1));
			put(STR_QMARK);
			interpreter = &Descriptor::handleStateQHanded;
			return;
		}
	}
	put(STR_QSTATS_REROLL);
}

void Descriptor::handleStateQRandStats(void)
{
	if (buf.len != 0)
	{
		if (strcasecmp(buf.txt, "yes")==0)
		{
			put(STR_QSTATS);
			roll_random_stats(pc.temp->stat, pc.temp->race);
			put(list_stats(pc.temp->stat, pc.temp->race));
			put(STR_QSTATS_REROLL);
			return;
		}
		if (strcasecmp(buf.txt, "no")==0)
		{
			put(STR_QPROFESSION);
			put(strarray_to_str(str_profession));
			put(STR_QMARK);
			interpreter = &Descriptor::handleStateQRandProfession;
			return;
		}
	}
	put(STR_QSTATS_REROLL);
}

void Descriptor::handleStateQRandProfession(void)
{
	if (buf.len != 0)
	{
		for (profession_t p = 0; p < NO_PROFESSION; p++)
		{
			if (strcasecmp(buf.txt, str_profession[p])==0)
			{
				pc.temp->profession = p;
				put(STR_QHANDED);
				put(strarray_to_str(str_handed, LEFT+1));
				put(STR_QMARK);
				interpreter = &Descriptor::handleStateQHanded;
				return;
			}
		}
	}
	put(STR_QPROFESSION);
	put(strarray_to_str(str_profession));
	put(STR_QMARK);
}

void Descriptor::handleStateQHanded(void)
{
	if (buf.len != 0)
	{
		for (handed_t h = 0; h < NO_HANDED; h++)
		{
			if (strcasecmp(buf.txt, str_handed[h])==0)
			{
				//MARK check for ambidexterousness
				pc.temp->handed = h;
				//MARK delete this sometime after testing
				char *pass = generate_password();
				char *cryp = crypt(pass, pc.temp->name);
				strncpy(pc.temp->password, cryp, MAX_ENCRYPTED_LEN);
				pc.temp->password[MAX_ENCRYPTED_LEN] = '\0';
				write_email_confirm(pc.temp, pass);
				put("\r\n" K_GRN);
				put(Motd);
				print(STR_QMOTDPASSWORD, pc.temp->email);
				print(K_YEL "      (Psst.. Your password is: %s)\r\r\n", pass);
				put(STR_QPASSWORD);
				put(echo_off);
				interpreter = &Descriptor::handleStateQMotdPassword;
				return;
			}
		}
	}
	put(STR_QHANDED);
	put(strarray_to_str(str_handed, LEFT));
	put(STR_QMARK);
}

/**
   \todo Put in rank listings
 */
void Descriptor::handleStateQMotdPassword(void)
{
	put(echo_on);
	if (buf.len != 0)
	{
		if (strncmp(pc.temp->password, crypt(buf.txt, pc.temp->password),
					MAX_PASSWORD_LEN)==0)
		{
			strcpy(pc.temp->birth_host, getHostname());
fprintf(stderr, "B birth_host: [%s]\n", pc.temp->birth_host);
			pc.data = new Player((BASIC_PC *) pc.temp, this);
			pc.data->save();
			print(STR_QMENU,
			"(ranks)\r\n",
			ObQuote ? ObQuote->next() : STR_OBQUOTE);
			interpreter = &Descriptor::handleStateQMenu;
			return;
		}
		put(STR_BADPASSWORD);
	}
	put(STR_QPASSWORD);
	put(echo_off);
}

/**
   \todo Put in rank listings
 */
void Descriptor::handleStateMotd(void)
{
	if (toGamePrompt())
	{
		resetToGamePrompt();
	}
	pc.data->save();
	print(STR_QMENU,
		"(ranks)\r\n",
		ObQuote ? ObQuote->next() : STR_OBQUOTE);
		interpreter = &Descriptor::handleStateQMenu;
}

void Descriptor::handleStateQMenu(void)
{
	if (buf.len == 1)
	{
		switch(buf.txt[0])
		{
			case '0':
				put(STR_GOODBYE);
				extract();
				return;
			case '1':
				if (envFlag(DENV_SPLITSCREEN))
				{
					initSplitScreen();
				}
				interpreter = &Descriptor::handleStateGameInterpreter;
				pc.data->insert(RM_CHAT);
				return;
			case '2':
				put(STR_QEMAIL_FULL STR_QEMAIL);
				interpreter = &Descriptor::handleStateQNewEmail;
				return;
			case '3':
				print(STR_QNEWPASSWORDOLD, pc.data->getName());
				interpreter = &Descriptor::handleStateQNewPasswordOld;
				put(echo_off);
				return;
			default:
				break;
		}
	}
	print(STR_QMENU,
	"(ranks)\r\n",
	ObQuote ? ObQuote->next() : STR_OBQUOTE);
	interpreter = &Descriptor::handleStateQMenu;
}

void Descriptor::handleStateQNewPasswordOld(void)
{
	put(echo_on);
	if (buf.len == 0)
	{
		print(STR_QNEWPASSWORDOLD, pc.data->getName());
		put(echo_off);
		return;
	}
	if (pc.data->isPassword(buf.txt)==0)
	{
		print(STR_BADPASSWORD);
		print(STR_PRESSRETURN);
		interpreter = &Descriptor::handleStateQMenuPause;
		return;
	}
	print(STR_QNEWPASSWORD, pc.data->getName());
	interpreter = &Descriptor::handleStateQNewPassword;
	put(echo_off);
}

void Descriptor::handleStateQNewPassword(void)
{
	put(echo_on);
	char *p;

	if (buf.len == 0)
	{
		print(STR_QNEWPASSWORD, pc.data->getName());
		put(echo_off);
		return;
	}
	for (p = buf.txt; *p; p++)
	{
		if (*p == ' ')
		{
			print("Your password may not contain spaces.\r\n" STR_QNEWPASSWORD,
				pc.data->getName());
			put(echo_off);
			return;
		}
	}
	if (buf.len > MAX_PASSWORD_LEN)
	{
		print("Your password must be at most %d characters long.\r\n"
			STR_QNEWPASSWORD,
			MAX_PASSWORD_LEN,
			pc.data->getName());
		put(echo_off);
		return;
	}
	else if (buf.len < MIN_PASSWORD_LEN)
	{
		print("Your password must be at least %d characters long.\r\n"
			STR_QNEWPASSWORD,
		MIN_PASSWORD_LEN,
		pc.data->getName());
		put(echo_off);
		return;
	}
	pc.data->setPassword(buf.txt); //MARK we shouldn't set this just yet
	put("Retype your password: ");
	interpreter = &Descriptor::handleStateQNewPasswordC;
	put(echo_off);
}

void Descriptor::handleStateQNewPasswordC(void)
{
	put(echo_on);
	if (buf.len == 0)
	{
		put("Retype your password: ");
		put(echo_off);
		return;
	}
	if (pc.data->isPassword(buf.txt)==0)
	{
		print("Your password does not match.\r\r\n"
				"Please give me a new password for %s: ",
				pc.data->getName());
		interpreter = &Descriptor::handleStateQNewPassword;
		put(echo_off);
		return;
	}
	pc.data->save();
	put(K_YEL "Your password has been changed.\r\n");
	put(STR_PRESSRETURN);
	interpreter = &Descriptor::handleStateQMenuPause;
}

void Descriptor::handleStateQNewEmail(void)
{
	if (buf.len == 0)
	{
		put(STR_QEMAIL);
		return;
	}
	if (is_invalid_email(buf.txt))
	{
		put(K_YEL "That is an invalid e-mail address.\r\n");
		put(STR_QEMAIL);
		return;
	}
	pc.data->setEmail(buf.txt);
	put(STR_QEMAILC);
	interpreter = &Descriptor::handleStateQNewEmailC;
}

void Descriptor::handleStateQNewEmailC(void)
{
	if (buf.len == 0)
	{
		put(STR_QEMAILC);
		return;
	}
	if (strcmp(pc.data->getEmail(), buf.txt))
	{
		put(STR_NOMATCHEMAIL STR_QEMAIL);
		interpreter = &Descriptor::handleStateQNewEmail;
		return;
	}
	char *pass = generate_password();
	pc.data->setPassword(pass);
	pc.data->save();
	write_email_confirm(pc.data, pass);
	print(STR_NEWEMAIL);
	print(K_YEL "(Psst.. Your password is: %s)\r\r\n", pass);
	print(STR_QPASSWORD);
	interpreter = &Descriptor::handleStateQMenuPassword;
	put(echo_off);
}

/**
   \todo Put in rank listings
 */
void Descriptor::handleStateQMenuPassword(void)
{
	put(echo_on);
	if (buf.len != 0)
	{
		if (pc.data->isPassword(buf.txt))
		{
			print(STR_QMENU, "(ranks)\r\n",
				ObQuote ? ObQuote->next() : STR_OBQUOTE);
			interpreter = &Descriptor::handleStateQMenu;
			return;
		}
		put(STR_BADPASSWORD);
	}
	put(STR_QPASSWORD);
	put(echo_off);
}

/**
   \todo Put in rank listings
 */
void Descriptor::handleStateQMenuPause(void)
{
	print(STR_QMENU, "(ranks)\r\n",
		ObQuote ? ObQuote->next() : STR_OBQUOTE);
	interpreter = &Descriptor::handleStateQMenu;
}

void Descriptor::handleStateGameInterpreter(void)
{
	if (buf.len == 0)
	{
		put(""); //prompt
	}
	else
	{
		if (do_command(pc.data, buf.txt)==0)
		{
			put("Arglebargle, glop-glypf!?!\r\n");
		}
	}
}

void Descriptor::handleStateWritingInterpreter(void)
{
	if (buf.len == 1 && buf.txt[0] == '@')
	{
		if (notepad.len > 0)
		{
			notepad.text[notepad.len] = '\0';
			if (cr1LWriter())
			{
				cleanl(notepad.text);
				STRDUP(*notepad.target, notepad.text);
				resetCr1LWriter();
			}
			else if (envFlag(DENV_WRAP))
			{
				*notepad.target = wrap(notepad.text);
			}
			else
			{
				STRDUP(*notepad.target, notepad.text);
			}
			FREE(notepad.text);
		}
		if (notepad.fn)
		{
			notepad.fn(notepad.ctx);
			notepad.fn = NULL;
			notepad.ctx = NULL;
		}
		interpreter = &Descriptor::handleStateGameInterpreter;
		put("Ok.\r\n");
		return;
	}
	if (notepad.len > MAX_NOTE_LEN)
	{
		put("Your note is too long and your last line was not written.\r\n");
		interpreter = &Descriptor::handleStateGameInterpreter;
		return;
	}
	put("] ");
	REALLOC(notepad.text, char, notepad.len+buf.len+3);
	strncpy(notepad.text+notepad.len, buf.txt, buf.len);
	*(notepad.text+notepad.len+buf.len) = '\r';
	*(notepad.text+notepad.len+buf.len+1) = '\n';
	notepad.len += buf.len+2;
}

void send_to_descriptors(const char *mesg, ...)
{
	int n, size = CHUNK_SIZE_OUT;
	int is_complete = 0;
	char *msg_s = NULL;
	char msg[CHUNK_SIZE_OUT];
	va_list vargs;

	while (1)
	{
		va_start(vargs, mesg);
		n = vsnprintf(msg_s ? msg_s : msg, size, mesg, vargs);
		va_end(vargs);
		msg_s = va_complete(&is_complete, msg_s, msg, &size, n);
		if (is_complete)
		{
			break;
		}
	}
	for (Descriptor *d = descriptor_list; d; d = d->getNext())
	{
		d->put("\r\n");
		d->put(msg_s ? msg_s : msg);
	}
	if (msg_s);
	{
		free(msg_s);
	}
}
