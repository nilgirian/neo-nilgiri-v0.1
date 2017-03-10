/* **************************************************************************
    Nilgiri the Forgotten World
    (C) 2004
   ************************************************************************** */
/** \file
    \brief Load the library files source
 */

#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<dirent.h>
#include<assert.h>
#include"library.h"
#include"err.h"
#include"utils.h"
#include"descriptor.h"
#include"db.h"

RevolvingText *Credits = NULL;
RevolvingText *Greeting = NULL;
RevolvingText *ObQuote = NULL;
HelpTable *helpTable = NULL;
HelpTable *immhelpTable = NULL;

char *Motd = NULL;

/** Frees memory created by this module and then reassigns it. */
void reinitialize_library_module(void)
{
	if (Credits)
	{
		delete Credits;
	}
	Credits = new RevolvingText(CREDITS_FILE);

	if (Greeting)
	{
		delete Greeting;
	}
	Greeting = new RevolvingText(GREETING_FILE);

	if (Motd)
	{
		free(Motd);
	}
	Motd = file_to_string(MOTD_FILE);

	if (ObQuote)
	{
		delete ObQuote;
	}
	ObQuote = new RevolvingText(OBQUOTES_FILE, 0);
	if (helpTable)
	{
		delete helpTable;
	}
	helpTable = new HelpTable(HELPTABLE_MORTAL);
	if (immhelpTable)
	{
		delete immhelpTable;
	}
	immhelpTable = new HelpTable(HELPTABLE_IMMORTAL);
}

/** Initialize the library module. */
void init_library_module(void)
{
	if (chdir("lib")==-1)
	{
		exitlog("/chdir/%s", strerror(errno));
	}
	reinitialize_library_module();
}

/** Looks into the library directory and picks out files that start with the
    same name and throws it into a RevolvingText.
    \param fname The name to match to.  This is case sensitive.
	 \todo might better to do this the old way of loading only pointers depends
	 how big the revolving text files get
 */
RevolvingText::RevolvingText(const char *fname)
{
	DIR *dr;
	FILE *fd;
	struct dirent *dp;
	size_t fnamelen;

	size_t dsize;
	char data[MAX_DATA_LEN];

	index = 0;
	listTop = -1;
	list = NULL;

	assert(fname && *fname);
	fnamelen = strlen(fname);

	if ((dr = opendir("."))==NULL)
	{
		exitlog("/opendir/%s", strerror(errno));
	}
	while ((dp = readdir(dr)) != NULL)
	{
		if (strncmp(fname, dp->d_name, fnamelen)==0)
		{
			if ((fd = fopen(dp->d_name, "r"))==NULL)
			{
				exitlog("/fopen/%s/%s", dp->d_name, strerror(errno));
			}
			if ((dsize = fread(data, sizeof(char), MAX_DATA_LEN-1, fd))==0)
			{
				exitlog("/fread/%s/%s", dp->d_name, strerror(errno));
			}
			if (ferror(fd))
			{
				exitlog("/fread/%s/%s", dp->d_name, strerror(errno));
			}
			data[dsize-1]='\0';
			listTop++;
			REALLOC(list, char *, listTop+1);
			STRDUP(list[listTop], data);
			if (fclose(fd)==EOF)
			{
				exitlog("/fclose/%s/%s", dp->d_name, strerror(errno));
			}
		}
	}
	if (closedir(dr)==-1)
	{
		exitlog("/closedir/%s", strerror(errno));
	}
	if (listTop==-1)
	{
		exitlog("/found no files/%s", fname);
	}
}

/** Looks into the library directory and reads a file and inserts text into a
    revolving text class with text deliminated by a hash (#).  Every newline
    is appended by a '\r'.
    \param name The name to match to.  This is case sensitive.
	 \todo probably better to do this the old way of loading only pointers
 */
RevolvingText::RevolvingText(const char *fname, int x)
{
	FILE *fd;
	char buf[MAX_DATA_LEN];
	char *text;
	size_t len, textlen;

	index = 0;
	listTop = -1;
	list = NULL;
	assert(fname && *fname);

	if ((fd = fopen(fname, "r"))==NULL)
	{
		exitlog("/fopen/%s/%s", fname, strerror(errno));
	}

	text = NULL;
	textlen = 0;

	while (fgets(buf, MAX_DATA_LEN, fd))
	{
		if (buf[0] == '#')
		{
			listTop++;
			REALLOC(list, char *, listTop+1);
			list[listTop] = text;
			text = NULL;
			textlen = 0;
		}
		else
		{
			len = strlen(buf);
			REALLOC(text, char, textlen+len+2);
			text[textlen] = '\0';
			strcat(text, buf);
			*(text+textlen+len)='\r';
			*(text+textlen+len+1)='\0';
			textlen += len+1;
		}
	}
	free(text);

	if (fclose(fd)==EOF)
	{
		exitlog("/fclose/%s/%s", fname, strerror(errno));
	}
}

/** Free memory */
RevolvingText::~RevolvingText(void)
{
	int i;

	for (i = 0; i < listTop; i++)
	{
		free(list[i]);
	}
	free(list);
}

/** \return The next text on the list. */
char *RevolvingText::next(void)
{
	assert(index > -1 && index <= listTop+1);
	if (index == listTop+1)
	{
		index = 0;
	}
	return list[index++];
}

/** Initialize the helpTable. */
HelpTable::HelpTable(const char *tname)
{
	table_name = tname;
}

/** De-initialize the HelpTable. */
HelpTable::~HelpTable(void)
{
}

int callback_helptable_list_size(void *pArg,
		int argc, char **argv, char **columnName)
{
	int *list_size = (int *) pArg;

	assert(argc==1);
	*list_size = atoi(argv[0]);
	return 0;
}

typedef struct _helptable_list_data
{
	char *keywords;
	size_t len;
	size_t maxlen;
	size_t linelen;
	int top;
}
HLIST_DATA;

int callback_helptable_list(void *pArg,
		int argc, char **argv, char **columnName)
{
	HLIST_DATA *hldata = (HLIST_DATA *) pArg;

	assert(argc==1);

	size_t arglen, ilen;
	char *p, *q;

	hldata->top++;

	arglen = strlen(argv[0]);
	if (arglen < 15)
	{
		ilen = 15;
	}
	else if (arglen < 30)
	{
		ilen = 30;
	}
	else if (arglen < 45)
	{
		ilen = 45;
	}
	else if (arglen < 60)
	{
		ilen = 60;
	}
	else
	{
		exitlog("/keyword too long/%s", argv[0]);
		return 0;
	}

	if (hldata->len + ilen + 3 > hldata->maxlen)
	{
		hldata->maxlen <<= 1;
		REALLOC(hldata->keywords, char, hldata->maxlen);
	}

	if (hldata->linelen + ilen > 80)
	{
		*(hldata->keywords+hldata->len) = '\r';
		hldata->len++;
		*(hldata->keywords+hldata->len) = '\n';
		hldata->len++;
		hldata->linelen = 0;
	}

	p = hldata->keywords + hldata->len;
	q = argv[0];
	for (size_t i = 0; i < ilen; i++)
	{
		if (*q)
		{
			*p = lower(*q);
			q++;
		}
		else
		{
			*p = ' ';
		}
		p++;
	}
	*p = '\0';
	hldata->len += ilen;
	hldata->linelen += ilen;
	return 0;
}

char *HelpTable::list(void)
{
	HLIST_DATA hldata;
	char *dbErrMsg = NULL;
	list_top = -1;

	hldata.top = 0;
	hldata.maxlen = 4096;
	hldata.linelen = 0;
	hldata.len = 0;
	MALLOC(hldata.keywords, char, hldata.maxlen);
	strcpy(hldata.keywords, HELP_HEADER);
	hldata.len += strlen(HELP_HEADER);
	SQLITE_EXEC_PRINTF(db, "SELECT keyword FROM %s_keywords ORDER BY keyword",
				callback_helptable_list,
				&hldata,
				dbErrMsg,
				table_name);
	strcat(hldata.keywords, "\r\n");
	return hldata.keywords;
}

int callback_helptable_load(void *pArg,
		int argc, char **argv, char **columnName)
{
	HELP_ENTRY *hentry = (HELP_ENTRY *) pArg;

	assert(argc==2);

	STRDUP(hentry->keyword, argv[0]);
	STRDUP(hentry->entry, argv[1]);
	return 0;
}

void HelpTable::load(const char *keyword, HELP_ENTRY *hentry)
{
	char *dbErrMsg = NULL;

	SQLITE_EXEC_PRINTF(db, "SELECT %s_keywords.keyword, %s.entry "
				"FROM %s, %s_keywords "
				"WHERE %s_keywords.keyword LIKE '%q%%' "
				"AND %s_keywords.%s_id = %s.id "
				"ORDER BY %s_keywords.keyword "
				"LIMIT 1",
				callback_helptable_load,
				hentry,
				dbErrMsg,
				table_name,
				table_name,
				table_name,
				table_name,
				table_name,
				keyword,
				table_name,
				table_name,
				table_name,
				table_name);
}

int callback_get_entry_id(void *pArg, int argc, char **argv, char **columnName)
{
	int *entry_id = (int *) pArg;

	assert (argc == 1);

	*entry_id = atoi(argv[0]);

	return 0;
}

int HelpTable::getEntryID(const char *keyword)
{
	char *dbErrMsg = NULL;
	int entry_id = -1;

	SQLITE_EXEC_PRINTF(db, "SELECT %s_id FROM %s_keywords WHERE keyword like %Q",
				callback_get_entry_id,
				&entry_id,
				dbErrMsg,
				table_name,
				table_name,
				keyword);
	return entry_id;
}

int HelpTable::add(HELP_ENTRY *hentry)
{
	char *dbErrMsg = NULL;
	int entry_id;

	if ((entry_id = getEntryID(hentry->keyword))==-1)
	{
		BEGIN_TRANSACTION(db, dbErrMsg);
		SQLITE_EXEC_PRINTF(db, "INSERT INTO %s VALUES (NULL, %Q)",
					NULL,
					NULL,
					dbErrMsg,
					table_name,
					hentry->entry);
		SQLITE_EXEC_PRINTF(db, "INSERT INTO %s_keywords VALUES (%Q, %u)",
					NULL,
					NULL,
					dbErrMsg,
					table_name,
					hentry->keyword,
					sqlite_last_insert_rowid(db));
		COMMIT_TRANSACTION(db, dbErrMsg);
		return 0;
	}
	SQLITE_EXEC_PRINTF(db, "UPDATE %s SET entry = %Q WHERE id = %u",
				NULL,
				NULL,
				dbErrMsg,
				table_name,
				hentry->entry,
				entry_id);
	return 0;
}

int HelpTable::add(const char *keyword1, const char *keyword2)
{
	char *dbErrMsg = NULL;
	int entry_id = -1;

	if ((entry_id = getEntryID(keyword2))==-1)
	{
		return -1;
	}
	SQLITE_EXEC_PRINTF(db, "REPLACE INTO %s_keywords VALUES (%Q, %u)",
				NULL,
				NULL,
				dbErrMsg,
				table_name,
				keyword1,
				entry_id);
	return 0;
}

int HelpTable::update(const char *keyword1, const char *keyword2)
{
	char *dbErrMsg = NULL;

	if ((getEntryID(keyword2))==-1)
	{
		return -1;
	}
	SQLITE_EXEC_PRINTF(db,
			"UPDATE OR REPLACE %s_keywords SET keyword = %Q WHERE keyword like %Q",
			NULL,
			NULL,
			dbErrMsg,
			table_name,
			keyword2,
			keyword1);
	return 0;
}

void HelpTable::del(const char *keyword)
{
	char *dbErrMsg = NULL;

	SQLITE_EXEC_PRINTF(db, "DELETE FROM %s_keywords WHERE keyword like %Q",
				NULL,
				NULL,
				dbErrMsg,
				table_name,
				keyword);
}
