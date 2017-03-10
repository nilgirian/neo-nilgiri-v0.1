/* **************************************************************************
    Nilgiri the Forgotten World
    (C) 2004
   ************************************************************************** */
/** \file
    \brief sqlite database header
 */

#ifndef _x_DB_INCLUDE_x_
#define _x_DB_INCLUDE_x_
#include"sqlite.h"


#define TINYWORLD_DB_FILE "lib/tinyworldB"
#define TINYWORLD_SQL_FILE "tinyworld.sql"

#define SQLITE_EXEC_PRINTF(db, format, callback, ctx, dbErrMsg, arg...) \
	if (sqlite_exec_printf(db, \
				format, callback, ctx, &dbErrMsg, ## arg) != SQLITE_OK) \
	{\
		exitlog("/sqlite_exec_printf/%s", dbErrMsg); \
	}

#define SQLITE_EXEC(db, format, callback, ctx, dbErrMsg) \
	if (sqlite_exec(db, format, callback, ctx, &dbErrMsg) != SQLITE_OK) \
	{\
		exitlog("/sqlite_exec/%s", dbErrMsg); \
	}

#define BEGIN_TRANSACTION(db, dbErrMsg) \
	SQLITE_EXEC(db, "BEGIN TRANSACTION", NULL, NULL, dbErrMsg)

#define COMMIT_TRANSACTION(db, dbErrMsg) \
	SQLITE_EXEC(db, "COMMIT TRANSACTION", NULL, NULL, dbErrMsg)

extern sqlite *db;

extern void init_db_module(void);
extern void finit_db_module(void);

#endif
