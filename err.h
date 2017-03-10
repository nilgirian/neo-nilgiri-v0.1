/* **************************************************************************
    Nilgiri the Forgotten World
    (C) 2004
    err.h: Logging
   ************************************************************************** */
/** \file
    \brief logging events and errors header
 */

#ifndef _x_ERR_INCLUDE_x_
#define _x_ERR_INCLUDE_x_

/** This should be the maximum size a log entry is expected to ever be */
#define ERRSIZE 1024

/** The session resource filename. */
#define SESSION_FILE "lib/session.rc"

extern void _x_errlog_x_(const char *, const char *, const unsigned int,
		const char *,  ...);
extern void _x_exitlog_x_(const char *, const char *, const unsigned int,
		const char *, ...);

/** This uses the __FILE__, __FUNCTION__, and __LINE__ defines to plug values
    into the _x_errlog_x_() function.
    \param A format for a variable list of arguments that will form the error
    message
    \param B variable list of arguments
 */
#define errlog(A, B...) _x_errlog_x_(__FILE__, __FUNCTION__, __LINE__, A, ## B)

/** This uses the __FILE__, __FUNCTION__, and __LINE__ defines to plug values
    into the _x_exitlog_x_() function.
    \param A format for a variable list of arguments that will form the error
    message
    \param B variable list of arguments
 */
#define exitlog(A, B...) _x_exitlog_x_(__FILE__, __FUNCTION__, __LINE__, A, ## B);

typedef struct _game_stats
{
	unsigned long int last_cid;
	unsigned long int boot_time;
	unsigned int items_created;
	unsigned int items_destroyed;
	unsigned int characters_created;
	unsigned int characters_destroyed;
	unsigned int old_player_characters_created;
	unsigned int new_player_characters_created;
	unsigned int player_characters_destroyed;
	unsigned int descriptors_created;
	unsigned int descriptors_destroyed;
	unsigned short int session_id;
	unsigned long int time_zero;
	unsigned int load_average[15];
	unsigned int rooms_created;
	unsigned int rooms_destroyed;
	unsigned int dictionary_words;
}
GAME_STATS;

extern GAME_STATS game_stat;

extern void init_err_module(void);
extern void finit_err_module(void);

#endif
