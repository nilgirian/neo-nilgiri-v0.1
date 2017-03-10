/* **************************************************************************
    Nilgiri the Forgotten World
    (C) 2004
   ************************************************************************** */
/** \file
    \brief Basic global utilities source
 */
#include<stdlib.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<assert.h>
#include"utils.h"
#include"err.h"
#include"time0.h"
#include"container.h"
#include"tinyworld_character.h"
#include"db.h"

/** For logging delay between subsequent calls. */
void log_delay(void)
{
	static struct timeval tdelay = { 0, 0 };
	struct timeval ndelay;

	if (tdelay.tv_sec == 0 && tdelay.tv_usec == 0)
	{
		gettimeofday(&tdelay, NULL);
	}
	else
	{
		gettimeofday(&ndelay, NULL);
		fprintf(stderr, "DELAY:%lu.%lu secs",
				ndelay.tv_sec - tdelay.tv_sec,
				ndelay.tv_usec - tdelay.tv_usec);
		tdelay.tv_sec = ndelay.tv_sec;
		tdelay.tv_usec = ndelay.tv_usec;
	}
}

/** Initialize the utils module.  */
void init_utils_module(void)
{
	srand(time0);
};

/** This is intended to keep a list of words so that keywords do not allocate
    unnecessary memory upon the same word.  It can be added upon but never
    subtracted all other Words instantiation should point to a word in
    the dictionary.
 */
Dictionary dictionary;

/** Initialize dictionary word list and list size. */
Dictionary::Dictionary(void)
{
	word = NULL;
	wordTop = -1;
}

/** \todo Need to properly free the words in word[] */
Dictionary::~Dictionary(void)
{
}

/** This adds a word to the word list.  This function creates new memory for
    the word and the increase in the length of the list. This is basically
	 a binary insertion sort. Since this list is expected to be large this
	 might be better implemented with another data structure.
    \param wname The word to add.
    \return a pointer to a new instance of wname is
	 returned
    \todo integrate file_to_str() here, is there a more efficient data struture?
	 \todo use memmove here
 */
char *Dictionary::addword(const char *wname)
{
	assert(wname && *wname && strlen(wname) < LINE_LENGTH);

	int bot, top, i;
	char *w;

	STRDUP(w, wname);
	to_lowerstr(w);

	bot = 0;
	top = wordTop;

	while (1)
	{
		i = (bot + top) / 2;
		if (wordTop >= i && is_equalstr(word[i], w))
		{
			return word[i];
		}
		if (bot >= top)
		{
			game_stat.dictionary_words = wordTop;
			wordTop++;
			REALLOC(word, char *, wordTop+1);
			for (; i <= wordTop-1; i++)
			{
				if (strcmp(word[i], w) > 0)
				{
					memmove(word+i+1, word+i, sizeof(char *) * (wordTop-i));
					break;
				}
			}
			word[i] = w;
			return word[i];
		}
		if (strcmp(word[i], w) > 0)
		{
			top = i - 1;
		}
		else
		{
			bot = i + 1;
		}
	}
}

const char *Dictionary::add(const char *keyword)
{
	return addword(keyword);
}

/** Compares words in the dictionary against a given word.
    \return 1 on a match, 0 on no match.
 */
const char *Dictionary::lookup(const char *w)
{
	assert(w && *w);

	char buf[MAX_SHORT_DESCR_SIZE];
	int bot, top, mid;

	copylowerstr(buf, w);
	w = buf;

	if (wordTop == -1)
	{
		return NULL;
	}

	bot = 0;
	top = wordTop;

	while (1)
	{
		mid = (bot + top) / 2;

		if (is_name(word[mid], w))
		{
			return word[mid];
		}
		if (bot >= top)
		{
			return NULL;
		}
		if (strcasecmp(word[mid], w) > 0)
		{
			top = mid - 1;
		}
		else
		{
			bot = mid + 1;
		}
	}
}

int Dictionary::equals(const char *w)
{
	int bot, top, mid;

	assert(w && *w);

	if (wordTop == -1)
	{
		return 0;
	}

	bot = 0;
	top = wordTop;

	while (1)
	{
		mid = (bot + top) / 2;

		if (word[mid] == w)
		{
			return 1;
		}
		if (bot >= top)
		{
			return 0;
		}
		if (strcmp(word[mid], w) > 0)
		{
			top = mid - 1;
		}
		else
		{
			bot = mid + 1;
		}
	}
}

void Dictionary::listTo(Character *ch)
{
	char sp[20];
	top_t i;
	size_t len, l;

	for (i = 0; i <= wordTop; i++)
	{
		ch->put(word[i]);
		if ((len = 20 - strlen(word[i])) > 0)
		{
			for (l = 0; l < len; l++)
			{
				sp[l] = ' ';
			}
			sp[l] = '\0';
			ch->put(sp);
		}
		if ((i+1) % 4 == 0)
		{
			ch->put("\r\n");
		}
	}
	if (i % 4)
	{
		ch->put("\r\n");
	}
}

void Words::save(vid_t vid)
{
	char *dbErrMsg = NULL;

	SQLITE_EXEC_PRINTF(db, "DELETE FROM %s_keywords WHERE %s_id = %u",
				NULL,
				NULL,
				dbErrMsg,
				table,
				table,
				vid);

	for (top_t i = 0; i <= wordTop; i++)
	{
		SQLITE_EXEC_PRINTF(db, "INSERT INTO %s_keywords VALUES (%u, %Q)",
					NULL,
					NULL,
					dbErrMsg,
					table,
					vid,
					word[i]);
	}
}

void Words::save(vid_t vid, top_t top_id)
{
	char *dbErrMsg = NULL;

	SQLITE_EXEC_PRINTF(db,
			"DELETE FROM %s_%s_keywords WHERE %s_id = %u and %s_id = %u",
			NULL,
			NULL,
			dbErrMsg,
			table,
			column,
			table,
			vid,
			column,
			top_id);

	for (top_t i = 0; i <= wordTop; i++)
	{
		SQLITE_EXEC_PRINTF(db, "INSERT INTO %s_%s_keywords VALUES (%u, %u, %Q)",
					NULL,
					NULL,
					dbErrMsg,
					table,
					column,
					vid,
					top_id);
	}
}

int keywords_callback(void *pArg, int argc, char **argv, char **columnName)
{
	Words *words = (Words *) pArg;
	words->add(argv[1]);
	return 0;
}

void Words::load(vid_t vid)
{
	char *dbErrMsg = NULL;

	SQLITE_EXEC_PRINTF(db, "SELECT * from %s_keywords WHERE %s_id = %u",
				keywords_callback,
				this,
				dbErrMsg,
				table,
				table,
				vid);
}

void Words::load(vid_t vid, top_t id)
{
	char *dbErrMsg = NULL;

	SQLITE_EXEC_PRINTF(db,
			"SELECT * from %s_%s_keywords WHERE %s_id = %u AND %s_id = %u",
			keywords_callback,
			this,
			dbErrMsg,
			table,
			column,
			table,
			vid,
			column,
			id);
}

Words::Words(const char *t, const char *c)
{
	table = t;
	column = c;
	wordTop = -1;
	word = NULL;
}

/** Free the word list but not the words itself. Words are stored in dictionary.
 */
Words::~Words(void)
{
	free(word);
	word = NULL;
}

/** This adds a word to the dictionary if it does not exist, and then points to 
    the dictionary word in the word list.  This is basically a binary insertion
	 sort.  Since this list is to be small for keyword lists this should be the
	 right data structure.
    \param wname The word to add.
    \return 1 - new word added, 0 - no new words added
	 \todo use memmove here
 */
int Words::add(const char *wname)
{
	int bot, top, i, j;
	char *w;

	assert(wname && *wname);

	assert((w = dictionary.addword(wname)));

	if (wordTop == -1)
	{
		MALLOC(word, char *, 1);
		wordTop++;
		word[wordTop] = w;
		return 1;
	}

	bot = 0;
	top = wordTop;

	while (1)
	{
		i = (bot + top) / 2;

		if (strcmp(word[i], w) == 0)
		{
			return 0;
		}
		if (bot >= top)
		{
			wordTop++;
			REALLOC(word, char *, wordTop+1);
			for (; i <= wordTop-1; i++)
			{
				if (strcmp(word[i], w) > 0)
				{
					for (j = wordTop; j > i; j--)
					{
						word[j] = word[j-1];
					}
					break;
				}
			}
			word[i] = w;
			return 1;
		}
		if (strcmp(word[i], w) > 0)
		{
			top = i - 1;
		}
		else
		{
			bot = i + 1;
		}
	}
}

Words::Words(Words *p)
{
	wordTop = p->wordTop;
	MALLOC(word, char *, wordTop+1);
	for (int i = 0; i <= wordTop; i++)
	{
		word[i] = p->word[i];
	}
}


/** The maximum size that a list of words can be displayed on a line.  This is
    used for displaying stats or indexing information.
 */
#define WORDS_LEN 40

/** Displays as many words that will fit in WORDS_LEN number of characters on
    a line.  This is used for displaying stats or indexing information.  The
	 text will end with a '+' character if there are more words in the list than
	 can be displayed.
	 \return The word list
 */
const char *Words::words(void)
{
	static char buf[WORDS_LEN], *p;
	const char *q;

	p = buf;
	for (int i = 0; i <= wordTop; i++)
	{
		q = word[i];
		while (*q)
		{
			*p = *q;
			p++;
			if (p-buf == WORDS_LEN)
			{
				buf[WORDS_LEN-2] = '+';
				buf[WORDS_LEN-1] = '\0';
				return buf;
			}
			q++;
		}
		*p = ' ';
		p++;
		if (p-buf == WORDS_LEN)
		{
			buf[WORDS_LEN-2] = '+';
			buf[WORDS_LEN-1] = '\0';
		}
	}
	*p = '\0';
	return buf;
}

/** This removes the word from the word list.
	 \return 0 - nothing remove, 1 - remove successful
    \param w The word to remove.
 */
int Words::remove(const char *w)
{
	char buf[MAX_SHORT_DESCR_SIZE];
	int bot, top, i;

	copylowerstr(buf, w);
	w = buf;

	bot = 0;
	top = wordTop;

	while (1)
	{
		i = (bot + top) / 2;

		if (is_equalstr(word[i], w))
		{
			if (i < wordTop)
			{
				memmove(word+i, word+i+1, sizeof(char *) * (wordTop - i));
			}
			REALLOC(word, char *, wordTop);
			wordTop--;
			return 1;
		}
		if (bot >= top)
		{
			return 0;
		}
		if (strcmp(word[i], w) > 0)
		{
			top = i - 1;
		}
		else
		{
			bot = i + 1;
		}
	}
}

/** Initialize our vid dictionary. */
VidIndex::VidIndex(void)
{
	vidList = NULL;
	vidTop = -1;
}

/** Free the memory created by the vid dictionary. */
VidIndex::~VidIndex(void)
{
	clear();
}

/** sets a key-value pair in the dictionary.
    \param vnum the key
    \param rnum the value
    \return 0 on success, 1 on failure
 */
void VidIndex::set(const vid_t vnum, const vid_t rnum)
{
	if ((sbyte4) vnum > vidTop)
	{
		REALLOC(vidList, vid_t, vnum+1);
		if (vnum-(vidTop+1))
		{
			memset(vidList+vidTop+1, -1, (vnum-(vidTop+1))<<1);
		}
		vidTop = vnum;
	}
	vidList[vnum] = rnum;
}

/** This function strips the color escape sequence from the given string.  It
    modifies the given parameter.  We assume all colors come in
    <TT>"\x1B[#;##m"</TT> format.
    \param txt The string to strip the color from.
	 \todo This can probably be inline.
 */
void strip_color(char *txt)
{
	char *p, *q;

	q = p = txt;

	while (*q)
	{
		if (*q == '\x1B')
		{
			q += COLORLEN;
			*p = *q;
			if (*q)
			{
				q++;
			}
		}
		else
		{
			*p = *q;
			q++;
		}
		p++;
	}
	*p = '\0';
}

/** Lists an array of string vertically with a couple spaces appended to
    each item.
	 \param array The array of string
	 \param max The last item to list in the string array.  If the ordinal value
	 of the item in the list is 0 then all items are displayed.
 */
char *strarray_to_str(const char **array, size_t max)
{
	static char buf[4096];
	size_t i = 0;

	assert(array);

	buf[0] = '\0';

	while (*array[i] != '\n')
	{
		if (max && i == max)
		{
			break;
		}
		if (*array[i] != '\0')
		{
			strcat(buf, "  ");
			strcat(buf, array[i]);
			strcat(buf, "\n\r");
		}
		i++;
	}
	assert(buf[0]);
	return buf;
}

/** Uppercases an entire given string.
    \param str The string to uppercase.  The string must be less than
    LINE_LENGTH.
    \return A buffer containing a similar string except all in caps.
 */
char *caps(const char *str)
{
	static char buffer[LINE_LENGTH+1];
	char *q;
	int n, len;

	len = strlen(str);

	assert(str && len < LINE_LENGTH);
	q = buffer;

	for (n = 0; n < len; n++)
	{
		if ((str[n] >= 'a' && str[n] <= 'z'))
		{
			*q = str[n] + 'A' - 'a';
		}
		else
		{
			*q = str[n];
		}
		q++;
	}
	*q = '\0';
	return buffer;
}

/** Generate a pseudo-random number.  _Numerical_Recipes_in_C_ states
    "If you want to generate a random integer .. you should always do it
     by using the high-order bits."
    \param max The range of numbers.
    \return A value within the range [0,max).
 */
ubyte2 number(ubyte2 die)
{
	return (int) (((float)die)*((float)rand())/((float)(RAND_MAX+1.0)));
}

/** Simulate rolling multiple dice.
    \param mult The multiplier.
    \param die The number of sides on the die.
    \return The total value of the dice rolled.
 */
ubyte2 dice(ubyte2 mult, ubyte2 die)
{
	ubyte2 result = 0;
	for (ubyte2 m = 0; m < mult; m++)
	{
		result += 1+number(die);
	}
	return result;
}

/** Simulate rolling a die.
    \param die The number of sides on the die.
    \return The side of the die facing up.
 */
ubyte2 dice(ubyte2 die)
{
	return 1 + number(die);
}

/** Checks to see if the file exists and is readable.
    \param filename The file name
	 \return 0 - false, 1 - true
 */
int file_exists(const char *filename)
{
	FILE *fd;
	if ((fd = fopen(filename, "r")))
	{
		fclose(fd);
		return 1;
	}
	if (errno != ENOENT)
	{
		exitlog("/fopen/%s/%s", filename, strerror(errno));
	}
	return 0;
}

/** This function frees an EXTRA_DESCR struct. */
void ExtraDescr::free_exdescr(EXTRA_DESCR *exd)
{
	assert(exd);
	delete exd->key;

	free(exd->proper);
	free(exd->general);
}

/** This function saves an EXTRA_DESCR struct. */
void ExtraDescr::save(vid_t vid)
{
	char *dbErrMsg = NULL;

	SQLITE_EXEC_PRINTF(db, "DELETE FROM %s_extras WHERE %s_id = %u",
				NULL,
				NULL,
				dbErrMsg,
				table,
				table,
				vid);

	for (top_t i = 0; i <= exdescr_top; i++)
	{
		top_t exdescr_id = i+1;
		SQLITE_EXEC_PRINTF(db, "INSERT INTO %s_extras"
				  " (%s_id, extras_id, proper, general)"
				  " VALUES (%u, %Q, %Q)",
					NULL,
					NULL,
					dbErrMsg,
					table,
					table,
					vid,
					exdescr_id,
					exdescr->proper,
					exdescr->general);
		exdescr->key->save(vid, exdescr_id);
	}
}

int callback_extra_descr(void *pArg,
		int argc, char **argv, char **columnName)
{
	assert(pArg);
	assert(argc == 4);
	ExtraDescr *exDescr = (ExtraDescr *) pArg;
	char *proper;
	char *general;
	top_t index=-1;
	top_t vid=-1;

	for (int i = 0; i < argc; i++)
	{
		if (strncmp(columnName[i],
					exDescr->getTable(), strlen(exDescr->getTable()))==0)
		{
			vid = atoi(argv[i]);
			continue;
		}
		if (strcmp(columnName[i], "extras_id")==0)
		{
			index = atoi(argv[i])-1;
			continue;
		}
		if (strcmp(columnName[i], "proper")==0)
		{
			assert(index != -1);
			STRDUP(proper, argv[i]);
			continue;
		}
		if (strcmp(columnName[i], "general")==0)
		{
			assert(index != -1);
			STRDUP(general, argv[i]);
			continue;
		}
		assert(index != -1);
		exDescr->create(vid, index, proper, general);
		exitlog("/column not used/%s", columnName[i]);
	}
	return 0;
}

void ExtraDescr::create(vid_t vid, top_t index, char *proper, char *general)
{
	exdescr_top++;
	assert(index == exdescr_top);

	REALLOC(exdescr, EXTRA_DESCR, index+1);
	exdescr[index].general = general;
	exdescr[index].proper = proper;
	exdescr[index].key = new Words(table);
	exdescr[index].key->load(vid, index);
}

/** For @examine information, this lists the extraDescr information
    \param c The character or container to receive the information
 */
void ExtraDescr::listTo(Container *c)
{
	for (top_t i = 0; i <= exdescr_top; i++)
	{
		c->print(" --- extra description %d ---\r\nKEYWORDS : %s\r\nPROPER   : %s\r\nGENERAL  :\r\n%s",
				i+1,
				exdescr[i].key->words(),
				C(exdescr[i].proper),
				C(exdescr[i].general));
	}
}

int ExtraDescr::checkFor(Container *c)
{
	for (top_t i = 0; i <= exdescr_top; i++)
	{
		if (exdescr[i].key->count() == 0)
		{
			c->print("Extra description %d needs at least one keyword.\r\n", i+1);
			return 1;
		}
		if (exdescr[i].proper == NULL)
		{
			c->print("Extra description %d needs a proper name.\r\n", i+1);
			return 1;
		}
		if (exdescr[i].general == NULL)
		{
			c->print("Extra description %d needs a general description.\r\n", i+1);
			return 1;
		}
	}
	return 0;
}

EXTRA_DESCR *ExtraDescr::get(const char *proper)
{
	for (top_t i = 0; i <= exdescr_top; i++)
	{
		if (is_name(proper, exdescr[i].proper))
		{
			return &exdescr[i];
		}
	}
	return NULL;
}

/**
    \return 1 deleted, 0 not deleted
 */
int ExtraDescr::del(const char *proper)
{
	for (int i = 0; i <= exdescr_top; i++)
	{
		if (is_name(proper, exdescr[i].proper))
		{
			free_exdescr(&exdescr[i]);
			exdescr_top--;
			while (i <= exdescr_top)
			{
				memcpy(&exdescr[i], &exdescr[++i], sizeof(EXTRA_DESCR));
			}
			return 1;
		}
	}
	return 0;
}

int ExtraDescr::add(const char *proper)
{
	assert(proper);
	if (get(proper))
	{
		return 0;
	}
	exdescr_top++;
	REALLOC(exdescr, EXTRA_DESCR, exdescr_top+1);
	exdescr[exdescr_top].general = NULL;
	STRDUP(exdescr[exdescr_top].proper, proper);
	exdescr[exdescr_top].key = new Words(table);
	return 1;
}

void ExtraDescr::load(vid_t vid)
{
	char *dbErrMsg = NULL;

	SQLITE_EXEC_PRINTF(db,
			"SELECT * FROM %s_extras WHERE %s_id = %u ORDER BY %s_id, extras_id",
			callback_extra_descr,
			this,
			dbErrMsg,
			table,
			table,
			vid,
			table);
}

ExtraDescr::ExtraDescr(const char *name, vid_t vid)
{
	assert(name);
	exdescr_top = -1;
	exdescr = NULL;
	table = name;
	load(vid); //MARK maybe remove this from the ctor, it's wasteful when @new
}

ExtraDescr::ExtraDescr(ExtraDescr *p)
{
	table = p->table;
	exdescr_top = p->exdescr_top;
	MALLOC(exdescr, EXTRA_DESCR, exdescr_top+1);
	for (int i = 0; i <= exdescr_top; i++)
	{
		exdescr[i].key = new Words(p->exdescr[i].key);
		STRDUP(exdescr[i].proper, p->exdescr[i].proper);
		STRDUP(exdescr[i].general, p->exdescr[i].general);
	}
}

ExtraDescr::~ExtraDescr(void)
{
	for (int i = 0; i <= exdescr_top; i++)
	{
		free_exdescr(&exdescr[i]);
	}
	free(exdescr);
}

/** List items from a strarry 4 columns across and as many needed rows down.
    \param c The character or container to receive the information
	 \param bitvector The bitvector of what to display, a bitvector of
	 (ubyte8) -1 will return all the flags
	 \param flag The strarray of flags
  */
void list_flags_to(Container *c, ubyte8 bitvector, const char **flag)
{
	size_t len, l;
	int i = 0, nr = 0;
	char sp[20];

	while (bitvector)
	{
		if (*flag[nr] == '\n')
		{
			break;
		}
		if (1ULL & bitvector)
		{
			c->put(flag[nr]);
			if ((len = 20-strlen(flag[nr])) > 0)
			{
				for (l = 0; l < len; l++)
				{
					sp[l] = ' ';
				}
				sp[l] = '\0';
				c->put(sp);
			}
			if ((i+1) % 4 == 0)
			{
				c->put("\r\n");
			}
			i++;
		}
		nr++;
		bitvector >>= 1;
	}
	if (i % 4)
	{
		c->put("\r\n");
	}
}

/** Saves a bitvector of flags into a table.
    \param id Identifies what is carrying the flag
	 \param flag The strarray of flags
	 \param name The name of the table
 */
void save_flags(vid_t id, ubyte8 bitvector, const char **flag, const char *name)
{
	char *dbErrMsg = NULL;

	int nr = 0;
	while (bitvector)
	{
		if (*flag[nr] == '\n')
		{
			break;
		}
		if (1ULL & bitvector)
		{
			assert(flag[nr]);
			SQLITE_EXEC_PRINTF(db, "INSERT INTO %s_flags VALUES (%u, '%s')",
						NULL,
						NULL,
						dbErrMsg,
						name,
						id,
						flag[nr]);
		}
		nr++;
		bitvector >>= 1;
	}
}

void save_flags(vid_t id1, vid_t id2,
		ubyte8 bitvector, const char **flag, const char *name1, const char *name2)
{
	char *dbErrMsg = NULL;

	int nr = 0;
	while (bitvector)
	{
		if (*flag[nr] == '\n')
		{
			break;
		}
		if (1ULL & bitvector)
		{
			assert(flag[nr]);
			SQLITE_EXEC_PRINTF(db, "INSERT INTO %s_%s_flags VALUES (%u, %u, '%s')",
						NULL,
						NULL,
						dbErrMsg,
						name1,
						name2,
						id1,
						id2,
						flag[nr]);
		}
		nr++;
		bitvector >>= 1;
	}
}
/** Get the bit associated with the flag in a strarray.
    \param flag The flag whose bitvector needs identifying
	 \param list The strarray
   \return 0 on failure
 */
ubyte8 get_bit(const char *flag, const char **list)
{
	int n = 0;

	while (*list[n] != '\n')
	{
		if (strcmp(list[n], flag)==0)
		{
			return 1 << n;
		}
		n++;
	}
	return 0;
}

/** \return last index + 1 on failure */
ubyte1 get_index(const char *flag, const char **list)
{
	int n = 0;
	
	while (*list[n] != '\n')
	{
		if (strcmp(list[n], flag)==0)
		{
			return n;
		}
		n++;
	}
	exitlog("/index not found/%s", flag);
	return (ubyte1) -1;
}

/** like get_index except this doesn't exit if fails. */
ubyte1 find_index(const char *flag, const char **list)
{
	int n = 0;
	
	while (*list[n] != '\n')
	{
		if (is_name(list[n], flag))
		{
			return n;
		}
		n++;
	}
	return (ubyte1) -1;
}

/** Reads a file and converts it to a string.  Appends \r to every line.
    \param fname The filename.
    \return Allocated string containing the file text or returns NULL on error.
 */
char *file_to_string(const char *fname)
{
	FILE *fd;
	char buf[1024];
	char *str;
	size_t len, slen;

	if ((fd = fopen(fname, "r"))==NULL)
	{
		exitlog("/fopen/%s/%s", fname, strerror(errno));
	}
	str = NULL;
	slen = 0;
	while (fgets(buf, 1024, fd))
	{
		len = strlen(buf);
		REALLOC(str, char, slen+len+2);
		str[slen] = '\0';
		strcat(str, buf);
		*(str+slen+len)='\r';
		*(str+slen+len+1)='\0';
		slen += len+1;
	}
	if (fclose(fd)==EOF)
	{
		exitlog("/fclose/%s/%s", fname, strerror(errno));
	}
	return str;
}

char *va_complete(int *is_complete, char *msg_s, char *msg, int *size, int n)
{
	assert(msg);
	assert(size);

	if (n > -1 && n < *size)
	{
		*is_complete = 1;
		return msg_s;
	}
	if (msg_s == NULL)
	{
		STRDUP(msg_s, msg);
	}
	if (n > -1)
	{
		*size = n + 1;
	}
	else
	{
		*size <<= 1;
	}
	REALLOC(msg_s, char, *size);
	*is_complete = 0;
	return msg_s;
}

/** This is a fast search to find the fill-words:
     - a
     - an
     - and
     - at
     - from
     - in
     - on
     - the
     - to
     - with

    This is case insensitive.
	 \param *p pointer to the word to analyze
	 \return 0 - false , 1 - true
 */
inline int is_fill_word(const char *p)
{
	int s = 0;
	while (*p)
	{
		if (s == 0)
		{
			if (*p == 'a' || *p == 'A')
			{
				s = 1; // a
			}
			else if (*p == 'f' || *p == 'F')
			{
				s = 2;
			}
			else if (*p == 'i' || *p == 'I')
			{
				s = 3;
			}
			else if (*p == 'o' || *p == 'O')
			{
				s = 4;
			}
			else if (*p == 't' || *p == 'T')
			{
				s = 5;
			}
			else if (*p == 'w' || *p == 'W')
			{
				s = 6;
			}
			else
			{
				return 0;
			}
		}
		else if (s == 1)
		{
			if (*p == 'n' || *p == 'N')
			{
				s = 7; // an
			}
			else if (*p == 't' || *p == 'T')
			{
				s = 8; // at
			}
			else
			{
				return 0;
			}
		}
		else if (s == 7)
		{
			if (*p == 'd' || *p == 'D')
			{
				s = 9; // and
			}
			else
			{
				return 0;
			}
		}
		else if (s == 2)
		{
			if (*p == 'r' || *p == 'R')
			{
				s = 10;
			}
			else
			{
				return 0;
			}
		}
		else if (s == 10)
		{
			if (*p == 'o' || *p == 'O')
			{
				s = 11;
			}
			else
			{
				return 0;
			}
		}
		else if (s == 11)
		{
			if (*p == 'm' || *p == 'M')
			{
				s = 12; // from
			}
			else
			{
				return 0;
			}
		}
		else if (s == 3)
		{
			if (*p == 'n' || *p == 'N')
			{
				s = 13; // in
			}
			else
			{
				return 0;
			}
		}
		else if (s == 4)
		{
			if (*p == 'n' || *p == 'N')
			{
				s = 14; // on
			}
			else
			{
				return 0;
			}
		}
		else if (s == 5)
		{
			if (*p == 'h' || *p == 'H')
			{
				s = 15;
			}
			else if (*p == 'o' || *p == 'O')
			{
				s = 16; // to
			}
			else
			{
				return 0;
			}
		}
		else if (s == 15)
		{
			if (*p == 'e' || *p == 'E')
			{
				s = 17;
			}
			else
			{
				return 0;
			}
		}
		else if (s == 6)
		{
			if (*p == 'i' || *p == 'I')
			{
				s = 18;
			}
			else
			{
				return 0;
			}
		}
		else if (s == 18)
		{
			if (*p == 't' || *p == 'T')
			{
				s = 19;
			}
			else
			{
				return 0;
			}
		}
		else if (s == 19)
		{
			if (*p == 'h' || *p == 'H')
			{
				s = 20; // with
			}
			else
			{
				return 0;
			}
		}
		else if (s == 1 || s == 7 || s == 8 || s == 9 || 12 || s == 13
				|| s == 14 || s == 16 || s == 20)
		{
			return 0;
		}
		p++;
	}
	return 1;
}

/** This function analyzes a string and finds the first non fill-word.  The
    argument is understood to be less than MAX_SHORT_DESCR_SIZE.
    \param *arg This is the on-argument ath will be filled in by the function
	 \warning arg must be a pointer to a char[MAX_SHORT_DESCR_SIZE]
	 \param need to change this to the tokenizer format
 */
void one_argument(char *str, char *arg)
{
	char *p;

	while ((p = strtok(str, " ")))
	{
		str = NULL;
		if (is_fill_word(p))
		{
			continue;
		}
		if (strlen(p) > MAX_SHORT_DESCR_SIZE-1)
		{
			continue;
		}
		strcpy(arg, p);
		return;
	}
}

void tokenizer(char *str, char **arg)
{
	int i = 0;
	int s = 0;
	char *p = str;
	arg[i] = NULL;
	while (*p)
	{
		if (s == 0)
		{
			if (i == MAX_TOKENS)
			{
				return;
			}
			if (*p == ' ')
			{
				s = 0;
			}
			else if (*p == '"')
			{
				if (*(p+1) == '"')
				{
					s = 3;
				}
				else
				{
					s = 1;
				}
			}
			else
			{
				arg[i] = p;
				i++;
				if (i != MAX_TOKENS)
				{
					arg[i] = NULL;
				}
				s = 2;
			}
		}
		else if (s == 1)
		{
			arg[i] = p;
			i++;
			if (i != MAX_TOKENS)
			{
				arg[i] = NULL;
			}
			s = 3;
		}
		else if (s == 2)
		{
			if (*p == ' ')
			{
				*p = '\0';
				s = 0;
			}
			else
			{
				s = 2;
			}
		}
		else if (s == 3)
		{
			if (*p == '"')
			{
				*p = '\0';
				s = 0;
			}
			else
			{
				s = 3;
			}
		}
		p++;
	}
}

const char *time_ago(time_t t)
{
	if (t == 0)
	{
		return "no time";
	}

	static char buf[LINE_LENGTH];
	size_t buflen;

	time_t days = t / SECS_PER_DAY;
	time_t hours = (t % SECS_PER_DAY) / SECS_PER_HOUR;
	time_t minutes = (t % SECS_PER_HOUR) / SECS_PER_MIN;
	time_t seconds = t % SECS_PER_MIN;

	buf[0] = '\0';
	buflen = 0;

	if (days)
	{
		buflen += sprintf(buf+buflen, "%s%lu day%s",
				buflen ? " " : "",
				days,
				days == 1 ? "" : "s");
	}
	if (hours)
	{
		buflen += sprintf(buf+buflen, "%s%lu hour%s",
				buflen ? " " : "",
				hours,
				hours == 1 ? "" : "s");
	}
	if (minutes)
	{
		buflen += sprintf(buf+buflen, "%s%lu minute%s",
				buflen ? " " : "",
				minutes,
				minutes == 1 ? "" : "s");
	}
	if (seconds)
	{
		buflen += sprintf(buf+buflen, "%s%lu second%s",
				buflen ? " " : "",
				seconds,
				seconds == 1 ? "" : "s");
	}
	return buf;
}

const char *time_str(time_t t)
{
	static char buf[LINE_LENGTH+1];
	struct tm *tm0;
	buf[0] = '\0';
	tm0 = localtime(&t);
	strftime(buf, LINE_LENGTH, "%Y-%m-%d %H:%M:%S", tm0);
	return buf;
}


int set_index(ubyte1 *type, const char *index, const char **strarray)
{
	ubyte1 t;
	if (nostr(index))
	{
		return 0;
	}
	if ((t = find_index(index, strarray)) == (ubyte1) -1)
	{
		return 0;
	}
	*type = t;
	return 1;
}

/** Finalize the utils module. */
void finit_utils_module(void)
{
	comm(NULL, NULL, NULL);
}

const int *order_list_index(const char **list, int list_top)
{
	char *tmp;
	int *newlist;
	int j, newlist_top;

	assert(list && list_top+1);

	MALLOC(newlist, int, list_top);
	newlist_top = -1;
	for (int i=0; i < list_top; i++)
	{
		j = newlist_top;
		while (j != -1 && strcmp(list[i], list[newlist[j]]) < 0)
		{
			newlist[j+1] = newlist[j];
			j--;
		}
		newlist[j+1] = i;
		newlist_top++;
	}
	return newlist;
}

void coord_save(const char *name, vid_t vid, COORD *coord)
{
	char *dbErrMsg;
	if (coord == NULL || coord->x == 0 || coord->y == 0 || coord->v == 0)
	{
		return;
	}
	SQLITE_EXEC_PRINTF(db,
			"REPLACE into %s_coord (%s_id, x, y, v) VALUES (%u, %u, %u, %u)",
			NULL,
			NULL,
			dbErrMsg,
			name,
			name,
			vid,
			coord->x,
			coord->y,
			coord->v);
}

int callback_coord_table(void *pArg, int argc, char **argv, char **columnName)
{
	assert(argc == 3);
	COORD *coord = (COORD *) pArg;
	MALLOC(coord, COORD, 1);
	for (int i = 0; i < argc; i++)
	{
		if (strcmp(columnName[i], "x"))
		{
			coord->x = atoi(argv[i]);
			continue;
		}
		if (strcmp(columnName[i], "y"))
		{
			coord->y = atoi(argv[i]);
			continue;
		}
		if (strcmp(columnName[i], "v"))
		{
			coord->v = atoi(argv[i]);
			continue;
		}
		exitlog("/column not used/%s", columnName[i]);
	}
	return 0;
}

void coord_load(const char *name, vid_t vid, COORD *coord)
{
	char *dbErrMsg;
	SQLITE_EXEC_PRINTF(db, "SELECT x, y, v FROM %s_coord WHERE %s_id = %u",
				callback_coord_table,
				coord,
				dbErrMsg,
				name,
				name,
				vid);
}

char *wrap(const char *bufin)
{
	char *bufout;
	const char *p, *lp;
	char *q, *lq;
	int c, s;

	// calculate size of bufout
	c = 0;
	p = bufin;
	while (*p)
	{
		if ((*p == '\r' || *p == '\n') && *(p+1) == '.') //add for newline P
		{
			c += 4;
		}
		c++;
		p++;
	}
	c += 3; //add these three for the first line for free
	c += (c/80)*2; //add for newline
	c++; //add for terminator

	MALLOC(bufout, char, c);

	lp = NULL; //pointer to the last word in bufin
	lq = NULL; //pointer to the last word in bufout
	p = bufin;
	q = bufout;
	c = 0; // count of characters in a line
	s = 0; // state

	while (*p)
	{
		if (s == 0) //beginning of the P
		{
			if (*p == '\r' || *p == '\n' || *p == ' ') //eat spaces
			{
				s = 0;
			}
			else // add the two space and begin the line
			{
				*q = ' ';
				q++;
				*q = ' ';
				q++;
				*q = *p;
				q++;
				c = 3;
				s = 2;
			}
		}
		else if (s == 2) //a line
		{
			if (c == 81) // if we're at the end of the line
			{
				if (lp == NULL) // if we don't have a last word just start new line
				{
					*q = '\r';
					q++;
					*q = '\n';
					q++;
					*q = *p;
					q++;
					c = 1;
					s = 2;
				}
				else // go back to the last words and append return start new line
				{
					q = lq;
					p = lp;
					*q = '\n';
					q++;
					*q = '\r';
					q++;
					lp = NULL;
					lq = NULL;
					c = 1;
					s = 2;
				}
			}
			else if (*p == '\r' || *p == '\n') // if we have a newline
			{
				if (*(p+1) == ' ') // if we lookahead to space start new P
				{
					*q = '\r';
					q++;
					*q = '\n';
					q++;
					c = 0;
					s = 0;
				}
				else if (*(p+1) == '.') // if we lookahead to a dot, start new P
				{
					*q = '\r';
					q++;
					*q = '\n';
					q++;
					*q = '\r';
					q++;
					*q = '\n';
					q++;
					p++;
					c = 0;
					s = 0;
				}
				else if (*(p+1) == '\r' || *(p+1) == '\n') //eat subsequent returns
				{
					s = 2;
				}
				else // turn newlines into space, get last words
				{
					*q = ' ';
					q++;
					lq = q;
					lp = p;
					c++;
					s = 2;
				}
			}
			else if (*p == ' ') // if we see a space
			{
				// eat consecutive spaces
				if (*(p+1) == ' ' || *(p+1) == '\r' || *(p+1) == '\n') 
				{
					s = 2;
				}
				else // since we're at a break we know to assign the last words
				{
					lp = p;
					lq = q;
					*q = *p;
					q++;
					c++;
					s = 2;
				}
			}
			else // standard character assignment
			{
				*q = *p;
				q++;
				c++;
				s = 2;
			}
		}
		p++;
	}
	*q = '\r';
	q++;
	*q = '\n';
	q++;
	*q = '\0';
	return bufout;
}
