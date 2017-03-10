/* **************************************************************************
    Nilgiri the Forgotten World
    (C) 2004
   ************************************************************************** */
/** \file
    \brief sqlite database source
 */
#include<stdio.h>
#include"err.h"
#include"db.h"
#include"utils.h"
#include"addlog.h"

sqlite *db = NULL;

/** Initialize the db module. This must always go before the first db. */
void init_db_module(void)
{
	char *dbErrMsg = NULL;

	if (file_exists(TINYWORLD_DB_FILE)==0)
	{
		char *sqlText = NULL;

		addlog_new_db();
		sqlText = file_to_string(TINYWORLD_SQL_FILE);

		if ((db = sqlite_open(TINYWORLD_DB_FILE, 0, &dbErrMsg))==NULL)
		{

			exitlog("/sqlite_open/%s", dbErrMsg);
		}
		SQLITE_EXEC(db, sqlText, NULL, NULL, dbErrMsg);
		free(sqlText);
	}
	else
	{
		if ((db = sqlite_open(TINYWORLD_DB_FILE, 0, &dbErrMsg))==NULL)
		{
			exitlog("/sqlite_open/%s", dbErrMsg);
		}
	}
}

/** Finilize the sqlite db module. */
void finit_db_module(void)
{
	sqlite_close(db);
}
