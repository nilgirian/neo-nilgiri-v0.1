/* **************************************************************************
    Nilgiri the Forgotten World
    (C) 2004
   ************************************************************************** */
/** \file
    \brief Load the library files header
 */
#ifndef _x_LIBRARY_HEADER_x_
#define _x_LIBRARY_HEADER_x_

#define MAX_DATA_LEN 4096

#define CREDITS_FILE "credits"

#define GREETING_FILE "greeting"

#define MOTD_FILE "motd"

#define OBQUOTES_FILE "obQuotes"

/** \brief For continually changing text. */
/** This is used for changing text in the same space where no consequence would
    occur should the text be different no matter what un/skewed order they may
    appear in.  For example: the splash screen, daily obligatory quote, etc.
    The text here is to be persistent throughout the entire game.  Notice: text
    can be added to but never removed without deleting the entire instance.
 */
class RevolvingText
{
public:
	RevolvingText(const char *);
	RevolvingText(const char *, int);
	~RevolvingText();
	char *next(void);
private:
	char **list;
	int listTop;
	int index;
};

extern void init_library_module(void);

extern RevolvingText *Credits;
extern RevolvingText *Greeting;
extern RevolvingText *ObQuote;
extern char *Motd;

#define HELPTABLE_MORTAL "help_mortal_table"
#define HELPTABLE_IMMORTAL "help_immortal_table"

typedef struct _help_entry
{
	const char *keyword;
	char *entry;
}
HELP_ENTRY;

class HelpTable
{
public:
	HelpTable(const char *);
	~HelpTable(void);
	char *list(void);
	void load(const char *, HELP_ENTRY *);
	int add(HELP_ENTRY *);
	int add(const char *, const char *);
	int update(HELP_ENTRY *);
	int update(const char *, const char *);
	void del(const char *);
	inline int exists(const char *);
private:
	int getEntryID(const char *);
	const char *table_name;
	int list_top;
};

inline int HelpTable::exists(const char *keyword)
{
	return getEntryID(keyword) != -1;
}

#define HELP_HEADER \
"******************************************************************************\r\n HELP TOPICS HELP TOPICS HELP TOPICS HELP TOPICS HELP TOPICS HELP TOPICS HELP\r\n******************************************************************************\r\n"

extern HelpTable *helpTable;
extern HelpTable *immhelpTable;
#endif
