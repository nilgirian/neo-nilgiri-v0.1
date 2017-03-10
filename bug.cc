/* **************************************************************************
    Nilgiri the Forgotten World
    (C) 2004
    err.h: Logging
   ************************************************************************** */
/** \file
    \brief bug reporting and tracking source
 */

#include<stdio.h>
#include"sqlite.h"
#include"tinyworld_character.h"
#include"constants.h"
#include"bug.h"
#include"err.h"
#include"string.h"

#define BUGDB_FILE "bugdB"

static sqlite *bugdb = NULL;

BugPermit **BugPermit::list = NULL;
top_t BugPermit::list_top = -1;

BugPermit::BugPermit(const char *name, const char *submitter)
{
	list_top++;
	STRDUP(owner, name);
	REALLOC(list, BugPermit *, list_top+1);
	list[list_top] = this;
	submitters = new Words("bug_permit");
	submitters->add(submitter);
}

BugPermit::~BugPermit(void)
{
	top_t i;
	for (i = 0; i <= list_top; i++)
	{
		if (list[i] == this)
		{
			break;
		}
	}
	assert(i != list_top+1);
	memmove(list+i, list+i+1, sizeof(BugPermit *)*(list_top - i));
	REALLOC(list, BugPermit *, list_top);
	list_top--;

	delete submitters;
}

const char *BugPermit::allowed(const char *owner, const char *submitter)
{
	for (top_t i = 0; i <= list_top; i++)
	{
		if (list[i]->isOwner(owner) && list[i]->canSubmit(submitter))
		{
			return list[i]->getOwner();
		}
	}
	return NULL;
}

void BugPermit::add(const char *owner, const char *submitter)
{
	for (top_t i = 0; i <= list_top; i++)
	{
		if (is_name(list[i]->owner, owner))
		{
			list[i]->submitters->add(submitter);
			return;
		}
	}
	new BugPermit(owner, submitter);
}

void BugPermit::del(const char *owner, const char *submitter)
{
	for (top_t i = 0; i <= list_top; i++)
	{
		if (is_name(list[i]->owner, owner))
		{
			fprintf(stderr, "name %s count %d\n",
					list[i]->owner,
					list[i]->submitters->count());
			int s = list[i]->submitters->remove(submitter);
			fprintf(stderr, "s:%d count %d\n", s, list[i]->submitters->count());
			if (list[i]->submitters->count() == 0)
			{
				delete list[i];
			}
			return;
		}
	}
	exit(0);
}

void BugPermit::indexTo(Character *ch)
{
	size_t count = 0;
	ch->put("Bug Permits:\r\n");
	for (top_t i = 0; i <= list_top; i++)
	{
		ch->put("-----------\r\n");
		ch->print("Owner [%s]\r\n", list[i]->owner);
		list[i]->submitters->listTo(ch);
		count++;
	}
	if (count == 0)
	{
		ch->put("none.\r\n");
	}
}

int bug_permits_callback(void *pArg, int argc, char **argv, char **columnName)
{
	assert(argc == 2);
	const char  *owner;
	const char *submitter;
	for (int i = 0; i < argc; i++)
	{
		if (is_equalstr(columnName[i], "owner"))
		{
			owner = argv[i];
			continue;
		}
		if (is_equalstr(columnName[i], "submitter"))
		{
			submitter = argv[i];
			continue;
		}
		exitlog("/column not used/%s", columnName[i]);
	}
	BugPermit::add(owner, submitter);
	return 0;
}

void BugPermit::load(void)
{
	char *dbErrMsg = NULL;

	SQLITE_EXEC_PRINTF(bugdb, "SELECT * FROM bug_permits",
				bug_permits_callback,
				NULL,
				dbErrMsg);
}

void BugPermit::save(const char *owner, const char *submitter)
{
	char *dbErrMsg = NULL;

	SQLITE_EXEC_PRINTF(bugdb, "REPLACE INTO bug_permits VALUES (%Q, %Q)",
			NULL,
			NULL,
			dbErrMsg,
			owner,
			submitter);
}

void BugPermit::delDB(const char *owner, const char *submitter)
{
	char *dbErrMsg = NULL;

	SQLITE_EXEC_PRINTF(bugdb, "DELETE FROM bug_permits "
			"WHERE owner like %Q and submitter like %Q",
			NULL,
			NULL,
			dbErrMsg,
			owner,
			submitter);
}

static int callback_bug_exists(void *pArg, int argc, char **argv,
		char **columnName)
{
	int *count = (int *) pArg;
	*count = 1;
	return 0;
}

int bug_exists(int id)
{
	char *dbErrMsg = NULL;
	int count = 0;

	SQLITE_EXEC_PRINTF(bugdb, "SELECT bug_id FROM bug WHERE bug_id = %d",
			callback_bug_exists,
			&count,
			dbErrMsg,
			id);
	return count;
}

static int callback_count_bugs(void *pArg, int argc, char **argv,
		char **columnName)
{
	assert(argc == 1);

	int *count  = (int *) pArg;
	*count = atoi(argv[0]);
	return 0;
}

void count_bugs(Character *ch)
{
	char *dbErrMsg = NULL;
	int count;

	SQLITE_EXEC_PRINTF(bugdb,
			"SELECT count(*) FROM bug WHERE owner like %Q AND status like 'open'",
			callback_count_bugs,
			&count,
			dbErrMsg,
			ch->getName());
	if (count > 0)
	{
		ch->print(K_BLD_YEL "** You have %d bug%s assigned to you that need to be resolved. ***\r\n" K_WHT,
				count,
				count == 1 ? "" : "s");
	}
}

void count_fixes(Character *ch)
{
	char *dbErrMsg = NULL;
	int count;

	SQLITE_EXEC_PRINTF(bugdb, "SELECT count(*) FROM bug WHERE submitter like %Q AND status like 'fixed'",
			callback_count_bugs,
			&count,
			dbErrMsg,
			ch->getName());
	if (count > 0)
	{
		ch->print(K_BLD_YEL "** You have %d fix%s that you need to check. ***\r\n" K_WHT,
				count,
				count == 1 ? "" : "es");
	}
}

static int callback_is_bug_owner(void *pArg, int argc, char **argv,
		char **columnName)
{
	assert(argc == 1);
	char *owner = (char *) pArg;
	owner = argv[0];
	return 0;
}

int is_bug_owner(int id, const char *name)
{
	char *dbErrMsg = NULL;
	char *owner;
	SQLITE_EXEC_PRINTF(bugdb, "SELECT owner FROM bug WHERE bug_id = %u",
			callback_is_bug_owner,
			owner,
			dbErrMsg,
			id);
	return is_name(owner, name);
}

static int callback_is_bug_submitter(void *pArg, int argc, char **argv,
		char **columnName)
{
	assert(argc == 1);
	char *submitter = (char *) pArg;
	submitter = argv[0];
	return 0;
}

int is_bug_submitter(int id, const char *name)
{
	char *dbErrMsg = NULL;
	char *submitter;
	SQLITE_EXEC_PRINTF(bugdb, "SELECT submitter FROM bug WHERE bug_id = %u",
			callback_is_bug_submitter,
			submitter,
			dbErrMsg,
			id);
	return is_name(name, submitter);
}

int can_reopen_bug(int id, const char *name)
{
	char *dbErrMsg = NULL;
	char *owner;
	SQLITE_EXEC_PRINTF(bugdb, "SELECT owner FROM bug WHERE bug_id = %u",
			callback_is_bug_owner,
			owner,
			dbErrMsg,
			id);
	return BugPermit::allowed(owner, name) != NULL;
}

static int callback_get_bug_status(void *pArg, int argc, char **argv,
		char **columnName)
{
	assert(argc == 1);

	status_t *status = (status_t *) pArg;
	*status = find_index(argv[0], str_status);
	return 0;
}

status_t get_bug_status(int id)
{
	char *dbErrMsg = NULL;
	status_t status;

	SQLITE_EXEC_PRINTF(bugdb, "SELECT status FROM bug WHERE bug_id = %u",
			callback_get_bug_status,
			&status,
			dbErrMsg,
			id);
	return status;
}

static int callback_list_bugs_count = 0;
static int callback_list_bugs(void *pArg, int argc, char **argv,
		char **columnName)
{
	assert(argc == 6);
	Character *ch = (Character *) pArg;

	char *bug_id;
	char *owner;
	char *submitter;
	char *priority;
	char *status;
	char *title;

	for (int i = 0; i < argc; i++)
	{
		if (strcmp(columnName[i], "bug_id") == 0)
		{
			bug_id = argv[i];
			continue;
		}
		if (strcmp(columnName[i], "owner") == 0)
		{
			owner = argv[i];
			continue;
		}
		if (strcmp(columnName[i], "submitter") == 0)
		{
			submitter = argv[i];
			continue;
		}
		if (strcmp(columnName[i], "priority") == 0)
		{
			priority = argv[i];
			continue;
		}
		if (strcmp(columnName[i], "status") == 0)
		{
			status = argv[i];
			continue;
		}
		if (strcmp(columnName[i], "title") == 0)
		{
			title = argv[i];
			continue;
		}
		exitlog("/column not used/%s", columnName[i]);
	}
	callback_list_bugs_count++;
	ch->print("%4s] Owner[%s] Submitter[%s] Status[%s] Priority[%s]\r\n        %s\r\n",
			bug_id,
			owner,
			submitter,
			status,
			priority,
			title);
	return 0;
}

static int callback_list_bugs_owner_count = 0;
static int callback_list_bugs_owner(void *pArg, int argc, char **argv,
		char **columnName)
{
	assert(argc == 4);
	Character *ch = (Character *) pArg;

	char *bug_id;
	char *owner;
	char *submitter;
	char *priority;
	char *status;
	char *title;

	for (int i = 0; i < argc; i++)
	{
		if (strcmp(columnName[i], "bug_id") == 0)
		{
			bug_id = argv[i];
			continue;
		}
		if (strcmp(columnName[i], "submitter") == 0)
		{
			submitter = argv[i];
			continue;
		}
		if (strcmp(columnName[i], "priority") == 0)
		{
			priority = argv[i];
			continue;
		}
		if (strcmp(columnName[i], "title") == 0)
		{
			title = argv[i];
			continue;
		}
		exitlog("/column not used/%s", columnName[i]);
	}
	callback_list_bugs_owner_count++;
	ch->print("%4s] Submitter[%s] Priority[%s]\r\n        %s\r\n",
			bug_id,
			submitter,
			priority,
			title);
	return 0;
}

void list_bugs_owner(Character *ch, const char *owner)
{
	char *dbErrMsg;
	callback_list_bugs_owner_count = 0;
	SQLITE_EXEC_PRINTF(bugdb,
				"SELECT bug_id, submitter, priority, title "
				"FROM bug "
				"WHERE status LIKE 'open' AND owner LIKE %Q",
				callback_list_bugs_owner,
				ch,
				dbErrMsg,
				owner);
	if (callback_list_bugs_owner_count == 0)
	{
		ch->put("No bugs found.\r\n");
	}
}

void list_bugs(Character *ch,
		const char *lim_owner, const char *lim_submitter ,
		const char *lim_status, const char *lim_priority, const char *lim_title)
{
	char *dbErrMsg;
	callback_list_bugs_count = 0;
	SQLITE_EXEC_PRINTF(bugdb,
				"SELECT bug_id, owner, submitter, status, priority, title "
				"FROM bug "
				"WHERE owner LIKE %Q "
				"AND submitter LIKE %Q "
				"AND status LIKE %Q "
				"AND priority LIKE %Q "
				"AND title LIKE '%%%q%%'",
				callback_list_bugs,
				ch,
				dbErrMsg,
				lim_owner ? lim_owner : "%",
				lim_submitter ? lim_submitter : "%",
				lim_status ? lim_status : "%",
				lim_priority ? lim_priority : "%",
				lim_title ? lim_title : "%");
	if (callback_list_bugs_count == 0)
	{
		ch->put("No bugs found.\r\n");
	}
}

static int callback_bug_detail_1(void *pArg, int argc, char **argv,
		char **columnName)
{
	assert(argc == 7);
	Character *ch = (Character *) pArg;
	char *bug_id;
	char *owner;
	char *submitter;
	char *priority;
	char *status;
	char *title;
	char *description;

	for (int i = 0; i < argc; i++)
	{
		if (is_equalstr(columnName[i], "bug_id"))
		{
			bug_id = argv[i];
			continue;
		}
		if (is_equalstr(columnName[i], "owner"))
		{
			owner = argv[i];
			continue;
		}
		if (is_equalstr(columnName[i], "submitter"))
		{
			submitter = argv[i];
			continue;
		}
		if (is_equalstr(columnName[i], "priority"))
		{
			priority = argv[i];
			continue;
		}
		if (is_equalstr(columnName[i], "status"))
		{
			status = argv[i];
			continue;
		}
		if (is_equalstr(columnName[i], "title"))
		{
			title = argv[i];
			continue;
		}
		if (is_equalstr(columnName[i], "description"))
		{
			description = argv[i];
			continue;
		}
		exitlog("/column not used/%s", columnName[i]);
	}
	ch->print("%4s] Owner[%s] Submitter[%s] Status[%s] Priority[%s]\r\n        %s\r\n--- bug description ---\r\n%s",
			bug_id,
			owner,
			submitter,
			status,
			priority,
			title,
			description);
	return 0;
}

static int callback_bug_detail_2(void *pArg, int argc, char **argv,
		char **columnName)
{
	assert(argc == 2);
	Character *ch = (Character *) pArg;
	char *submitter;
	char *description;

	for (int i = 0; i < argc; i++)
	{
		if (is_equalstr(columnName[i], "submitter"))
		{
			submitter = argv[i];
			continue;
		}
		if (is_equalstr(columnName[i], "description"))
		{
			description = argv[i];
			continue;
		}
		exitlog("/column not used/%s", columnName[i]);
	}
	ch->print("--- followup by %s ---\r\n%s",
			submitter,
			description);
	return 0;
}

void bug_detail_to(Character *ch, int bug_id)
{
	char *dbErrMsg;

	SQLITE_EXEC_PRINTF(bugdb, "SELECT * FROM bug WHERE bug_id = %u",
			callback_bug_detail_1,
			ch,
			dbErrMsg,
			bug_id);

	SQLITE_EXEC_PRINTF(bugdb, "SELECT submitter, description FROM bug_followup WHERE bug_id = %u ORDER BY followup_id",
			callback_bug_detail_2,
			ch,
			dbErrMsg,
			bug_id);
	ch->put("-\r\n");
}

typedef struct _bug_data
{
	const char *owner;
	const char *submitter;
	priority_t priority;
	char *title;
	char *notepad; // this is freed by notepad code
}
BUG_DATA;

static void free_bug_data(BUG_DATA *bdata)
{
	free(bdata->title);
	free(bdata);
}

void insert_bug(void *ctx)
{
	assert(ctx);
	BUG_DATA *bdata = (BUG_DATA *) ctx;
	if (bdata->notepad)
	{
		char *dbErrMsg;
		SQLITE_EXEC_PRINTF(bugdb, "INSERT INTO bug "
				"VALUES (NULL, %Q, %Q, %Q, %Q, %Q, %Q)",
				NULL,
				NULL,
				dbErrMsg,
				bdata->owner,
				bdata->submitter,
				str_status[OPEN],
				str_priority[bdata->priority],
				bdata->title,
				bdata->notepad);
	}
	free_bug_data(bdata);
}

void bug_writer(Character *ch, const char *owner, priority_t p,
		const char *title)
{
	BUG_DATA *bdata;
	MALLOC(bdata, BUG_DATA, 1);
	bdata->owner = owner;
	bdata->submitter = ch->getName();
	bdata->priority = p;
	STRDUP(bdata->title, title);
	bdata->notepad = NULL;
	ch->writer(&bdata->notepad, insert_bug, bdata);
}

typedef struct _followup_data
{
	const char *submitter;
	int bug_id;
	char *notepad; // this is freed by notepad code
}
FOLLOWUP_DATA;

void insert_followup(void *ctx)
{
	assert(ctx);
	FOLLOWUP_DATA *fdata = (FOLLOWUP_DATA *) ctx;
	if (fdata->notepad)
	{
		char *dbErrMsg;
		SQLITE_EXEC_PRINTF(bugdb, "INSERT INTO bug_followup "
				"VALUES (NULL, %u, %Q, %Q)",
				NULL,
				NULL,
				dbErrMsg,
				fdata->bug_id,
				fdata->submitter,
				fdata->notepad);
	}
	free(fdata);
}

void followup_writer(Character *ch, int bug_id)
{
	FOLLOWUP_DATA *fdata;
	MALLOC(fdata, FOLLOWUP_DATA, 1);
	fdata->submitter = ch->getName();
	fdata->bug_id = bug_id;
	fdata->notepad = NULL;
	ch->writer(&fdata->notepad, insert_followup, fdata);
}

typedef struct _new_status_data
{
	const char *submitter;
	int bug_id;
	status_t status;
	char *notepad; // this is freed by notepad code
}
NEW_STATUS_DATA;

void update_status(void *ctx)
{
	assert(ctx);
	NEW_STATUS_DATA *sdata = (NEW_STATUS_DATA *) ctx;
	if (sdata->notepad)
	{
		char *dbErrMsg;
		SQLITE_EXEC_PRINTF(bugdb, "UPDATE bug SET status = %Q WHERE bug_id = %u",
				NULL,
				NULL,
				dbErrMsg,
				str_status[sdata->status],
				sdata->bug_id);
		SQLITE_EXEC_PRINTF(bugdb, "INSERT INTO bug_followup "
				"VALUES (NULL, %u, %Q, '** NEW STATUS [%q] **\r\n%q')",
				NULL,
				NULL,
				dbErrMsg,
				sdata->bug_id,
				sdata->submitter,
				str_status[sdata->status],
				sdata->notepad);
	}
	free(sdata);
}

void new_status_writer(Character *ch, int bug_id, status_t status)
{
	NEW_STATUS_DATA *sdata;
	MALLOC(sdata, NEW_STATUS_DATA, 1);
	sdata->submitter = ch->getName();
	sdata->bug_id = bug_id;
	sdata->status = status;
	sdata->notepad = NULL;
	ch->writer(&sdata->notepad, update_status, sdata);
}

void init_bug_module(void)
{
	char *dbErrMsg = NULL;
	extern int file_exists(const char *);

	if (file_exists(BUGDB_FILE) == 0)
	{
		if ((bugdb = sqlite_open(BUGDB_FILE, 0, &dbErrMsg)) == NULL)
		{
			exitlog("/sqlite_open/%s", dbErrMsg);
		}
		SQLITE_EXEC(bugdb, "CREATE TABLE bug (bug_id integer primary key, "
					"owner text, submitter text, status text, priority text, "
					"title text, description text)",
					NULL,
					NULL,
					dbErrMsg);
		SQLITE_EXEC(bugdb,
			 		"CREATE TABLE bug_followup (followup_id integer primary key, "
					"bug_id integer, submitter text, description text)",
					NULL,
					NULL,
					dbErrMsg);
		SQLITE_EXEC(bugdb, "CREATE TABLE bug_permits "
				"(owner text not null, submitter text not null)",
				NULL,
				NULL,
				dbErrMsg);
	}
	else
	{
		if ((bugdb = sqlite_open(BUGDB_FILE, 0, &dbErrMsg)) == NULL)
		{
			exitlog("/sqlite_open/%s", dbErrMsg);
		}
	}
	//MARK maybe rename these to from load/save to save/load
	BugPermit::load();
}

void finit_bug_module(void)
{
	sqlite_close(bugdb);
}
