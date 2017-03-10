/* **************************************************************************
    Nilgiri the Forgotten World
    (C) 2004
   ************************************************************************** */
/** \file
    \brief logging events and errors source
 */

#include<stdio.h>
#include<stdlib.h>
#include<stdarg.h>
#include<string.h>
#include<errno.h>
#include"sqlite.h"
#include"err.h"
#include"time0.h"
#include"addlog.h"

#define DEBUG(x) x

#define LOGDB_FILE "logdB"

/** Data structure for holding random game information */
GAME_STATS game_stat;

/** For logging information. */
static sqlite *logdb = NULL;

/** This takes in a log line and changes $ variables into proper values.
    - $T the current time
	 - $S the session ID
	 \param session The session ID
	 \param time The current time
	 \param line The log line to format
 */
const char *format_line(unsigned short int session, time_t time,
		const char *line)
{
	extern const char *time_str(time_t);
	static char buf[ERRSIZE+1];
	const char *p;
	char *q;
	int s;
	
	p = line;
	q = buf;
	s = 0;

	while (*p)
	{
		if (*p == '$')
		{
			if (*(p+1) == 'S')
			{
				sprintf(q, "%04X", session);
				q += 4;
				p++;
			}
			if (*(p+1) == 's')
			{
				sprintf(q, "%04X", (unsigned short int) ~session);
				q += 4;
				p++;
			}
			else if (*(p+1) == 'T')
			{
				strcat(buf, time_str(time));
				q += 19;
				p++;
			}
			else if (*(p+1) == 'U')
			{
				sprintf(q, "%08X", (unsigned int) time0);
				q += 8;
			}
			else
			{
				*q = *p;
				q++;
			}
		}
		else
		{
			*q = *p;
			q++;
		}
		p++;
		*q = '\0';
	}
	return buf;
}

/** This writes out a log entry to the logdb database in the SESSION tree in
    a particular errlog format.  This is not to be used directly but through the
	 errlog() macro.
    \param fi filename
    \param fn function
    \param ln line number
    \param format format for a variable list of arguments that would form the
    error log message
    \param ... a variable list of arguments
	 \sa errlog()
 */
void _x_errlog_x_(const char *fi, const char *fn, const unsigned int ln,
  const char *format, ...)
{
	char errbuf[ERRSIZE+1];
	char errbuf2[ERRSIZE+1];
	va_list varglist;
	char *dbErrMsg;

	va_start(varglist, format);
	vsnprintf(errbuf2, ERRSIZE, format, varglist);
	va_end(varglist);
	errbuf2[ERRSIZE] = '\0';
	snprintf(errbuf, ERRSIZE, "/SESSIONS/%04hX/::%s:WARNINGS/%s:%d/%s%s",
			game_stat.session_id,
			time_str(time0),
			fi,
			ln,
			fn,
			errbuf2);
	errbuf[ERRSIZE] = '\0';
	fprintf(stderr, "%s\n", errbuf);
	if (sqlite_exec_printf(logdb, "INSERT INTO log VALUES (%u, %u, %Q)",
				NULL,
				NULL,
				&dbErrMsg, 
				errbuf) != SQLITE_OK)
	{
		perror("_x_errlog_x_:");
		abort();
	}
}

/** This writes out a log entry to the logdb database in the SESSION tree in
    a particular exitlog format.  This is not to be used directly but through
	 the exitlog() macro.
    \param fi filename
    \param fn function
    \param ln line number
    \param format format for a variable list of arguments that would form the
    exit log message
    \param ... a variable list of arguments
	 \sa exitlog()
 */
void _x_exitlog_x_(const char *fi, const char *fn, const unsigned int ln,
		const char *format, ...)
{
	char errbuf[ERRSIZE+1];
	char errbuf2[ERRSIZE+1];
	va_list varglist;
	char *dbErrMsg;

	va_start(varglist, format);
	vsnprintf(errbuf2, ERRSIZE, format, varglist);
	va_end(varglist);
	errbuf2[ERRSIZE] = '\0';
	snprintf(errbuf, ERRSIZE, "/SESSIONS/%04hX/::%s:ABORT/%s:%d/%s%s",
			game_stat.session_id, time_str(time0), fi, ln, fn, errbuf2);
	errbuf[ERRSIZE] = '\0';
	fprintf(stderr, "%s\n", errbuf);
	if (sqlite_exec_printf(logdb, "INSERT INTO log VALUES (%u, %u, %Q)",
				NULL,
				NULL,
				&dbErrMsg,
				game_stat.session_id,
				time0,
				errbuf) != SQLITE_OK)
	{
		perror("_x_exitlog_x_:");
		abort();
	}
	sqlite_close(logdb);
	abort();
	exit(EXIT_FAILURE);
}

/** This writes out a log entry to stdout using the format_line() function.
    \param format format for a variable list of arguments that would form the
    log message
    \param ... variable list of arguments
 */
void addlog(const char *format, ...)
{
	char errbuf[ERRSIZE+1];
	const char *reformat;
	va_list varglist;
	char *dbErrMsg;

	va_start(varglist, format);
	vsnprintf(errbuf, ERRSIZE, format, varglist);
	va_end(varglist);
	errbuf[ERRSIZE-4-8] = '\0'; // -4 for SessionId -8 for CID

	reformat = format_line(game_stat.session_id, time0, errbuf);

	DEBUG(fprintf(stderr, "%s\n", reformat));
	if (sqlite_exec_printf(logdb, "INSERT INTO log VALUES (%u, %u, %Q)",
				NULL,
				NULL,
				&dbErrMsg,
				game_stat.session_id,
				time0,
				reformat) != SQLITE_OK)
	{
		perror("addlog:");
		abort();
	}
}

/** Initialize the err module. This must always go after the time module. */
void init_err_module(void)
{
	char *dbErrMsg = NULL;
	extern int file_exists(const char *);

	if (file_exists(LOGDB_FILE)==0)
	{
		if ((logdb = sqlite_open(LOGDB_FILE, 0, &dbErrMsg))==NULL)
		{
			exitlog("/sqlite_open/%s", dbErrMsg);
		}
		if (sqlite_exec(logdb,
					"CREATE TABLE log (session int, time int, line text)",
					NULL,
					NULL,
					&dbErrMsg) != SQLITE_OK)
		{
			perror(dbErrMsg);
			abort();
		}
	}
	else
	{
		if ((logdb = sqlite_open(LOGDB_FILE, 0, &dbErrMsg))==NULL)
		{
			perror(dbErrMsg);
			abort();
		}
	}

	FILE *fd;

	if ((fd = fopen(SESSION_FILE, "r"))==NULL)
	{
		if (errno != ENOENT)
		{
			exitlog("/fopen/%s/%s", SESSION_FILE, strerror(errno));
		}
	}

	if (fd == NULL)
	{
		game_stat.session_id = 1;
		game_stat.time_zero = time0;
	}
	else
	{
		if (1 != fread(&game_stat.session_id, sizeof(unsigned short int), 1, fd))
		{
			exitlog("/fread/%s", SESSION_FILE);
		}
		if (1 != fread(&game_stat.time_zero, sizeof(unsigned long int), 1, fd))
		{
			exitlog("/fread/%s", SESSION_FILE);
		}
		if (fclose(fd)==EOF)
		{
			exitlog("/fclose/%s/%s", SESSION_FILE, strerror(errno));
		}
	}

	if ((fd = fopen(SESSION_FILE, "w+"))==NULL)
	{
		exitlog("/fopen/%s/%s", SESSION_FILE, strerror(errno));
	}
	game_stat.session_id++;
	if (1 != fwrite(&game_stat.session_id, sizeof(unsigned short int), 1, fd))
	{
		exitlog("/fwrite/%s/%s", SESSION_FILE, strerror(errno));
	}
	if (1 != fwrite(&game_stat.time_zero, sizeof(unsigned long int), 1, fd))
	{
		exitlog("/fwrite/%s/%s", SESSION_FILE, strerror(errno));
	}
	game_stat.session_id--;
	if (fclose(fd)==EOF)
	{
		exitlog("/fclose/%s/%s", SESSION_FILE, strerror(errno));
	}

	game_stat.boot_time = time0;
	game_stat.items_created = 0;
	game_stat.items_destroyed = 0;
	game_stat.characters_created = 0;
	game_stat.characters_destroyed = 0;
	game_stat.old_player_characters_created = 0;
	game_stat.new_player_characters_created = 0;
	game_stat.player_characters_destroyed = 0;
	game_stat.descriptors_created = 0;
	game_stat.descriptors_destroyed = 0;
	game_stat.rooms_created = 0;
	game_stat.rooms_destroyed = 0;
	game_stat.load_average[0] = 0;
	game_stat.load_average[1] = 0;
	game_stat.load_average[2] = 0;
	game_stat.load_average[3] = 0;
	game_stat.load_average[4] = 0;
	game_stat.load_average[5] = 0;
	game_stat.load_average[6] = 0;
	game_stat.load_average[7] = 0;
	game_stat.load_average[8] = 0;
	game_stat.load_average[9] = 0;
	game_stat.load_average[10] = 0;
	game_stat.load_average[11] = 0;
	game_stat.load_average[12] = 0;
	game_stat.load_average[13] = 0;
	game_stat.load_average[14] = 0;

	addlog_start();
}

/** Finalize the err module. */
void finit_err_module(void)
{
	sqlite_close(logdb);
}
