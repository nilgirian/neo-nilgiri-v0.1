/* **************************************************************************
    Nilgiri the Forgotten World
    (C) 2004
   ************************************************************************** */
/** \file
    \brief Item object source
 */

#include"tinyworld_character.h"
#include"tinyworld_item.h"
#include"descriptor.h"
#include"utils.h"
#include"db.h"
#include"addlog.h"

#include"item_container.h"
#include"item_note.h"

/** Array of item descriptions */
ITEM_DESCR_DATA **item_descr = NULL;
/** Array of item stats */
ITEM_STATS_DATA **item_stats = NULL;
/** Array of item data */
ITEM_DATA **item_data = NULL;

/** Top index of item_descr */
top_t item_descr_top = -1;
/** Top index of item_stats */
top_t item_stats_top = -1;
/** Top index of item_data */
top_t item_data_top = -1;

/** A map from virtual IDs to real IDs for item descriptions created */
VidIndex idescrIndex;
/** A map from virtual IDs to real IDs for item types created */
VidIndex istatsIndex;
/** A map from virtual IDs to real IDs for item data created */
VidIndex idataIndex;

Item *item_list = NULL;

#define INIT_ITEM_TOP 1

static void create_tinyworld_item_descriptions(void)
{
	vid_t id;
	item_descr_top = INIT_ITEM_TOP;

	MALLOC(item_descr, ITEM_DESCR_DATA *, item_descr_top+1);
	id = 0;

	MALLOC(item_descr[id], ITEM_DESCR_DATA, 1);
	item_descr[id]->key = new Words("item_descr");
	item_descr[id]->key->add("corpse");
	STRDUP(item_descr[id]->proper, "a corpse");
	STRDUP(item_descr[id]->room,
			"A corpse so decayed you cannot identify who or what it was.\r\n");
	STRDUP(item_descr[id]->general, "It is a corpse. It is a little smelly.");
	item_descr[id]->extra = new ExtraDescr("item_descr", id+1);
	STRDUP(item_descr[id]->creators_note,
			"standard item corpse description\r\n");
	item_descr[id]->virtual_id = ++id;

	MALLOC(item_descr[id], ITEM_DESCR_DATA, 1);
	item_descr[id]->key = new Words("item_descr");
	item_descr[id]->key->add("parchment");
	item_descr[id]->key->add("note");
	item_descr[id]->key->add("paper");
	STRDUP(item_descr[id]->proper, "a piece of parchment");
	STRDUP(item_descr[id]->room, "A piece of parchment lies here.");
	STRDUP(item_descr[id]->general,
			"This is a virtual two dimensional object.\r\n");
	item_descr[id]->extra = new ExtraDescr("item_descr", id+1);
	STRDUP(item_descr[id]->creators_note,
			"standard item parchment description\r\n");
	item_descr[id]->virtual_id = ++id;

	assert(id == INIT_ITEM_TOP+1);
}

static void create_tinyworld_item_stats(void)
{
	vid_t id;
	item_stats_top = INIT_ITEM_TOP;

	MALLOC(item_stats, ITEM_STATS_DATA *, item_stats_top+1);
	id = 0;

	MALLOC(item_stats[id], ITEM_STATS_DATA, 1);
	item_stats[id]->material_flags = 0;
	item_stats[id]->item_type = ITEM_CONTAINER;
	item_stats[id]->wear_flags = 0;
	item_stats[id]->attribute_flags = 0;
	item_stats[id]->special.value[0] = 0;
	item_stats[id]->special.value[1] = 0;
	item_stats[id]->special.value[2] = 0;
	item_stats[id]->special.value[3] = 0;
	item_stats[id]->special.affect_flags = 0;
	item_stats[id]->special.resistance_flags = 0;
	item_stats[id]->special.immunity_flags = 0;
	item_stats[id]->property_flags = 0;
	item_stats[id]->weight = 0;
	item_stats[id]->volume = 0;
	STRDUP(item_stats[id]->creators_note, "standard item corpse stats");
	item_stats[id]->virtual_id = ++id;

	MALLOC(item_stats[id], ITEM_STATS_DATA, 1);
	item_stats[id]->material_flags = 0;
	item_stats[id]->item_type = ITEM_NOTE;
	item_stats[id]->wear_flags = 0;
	item_stats[id]->attribute_flags = 0;
	item_stats[id]->special.value[0] = 0;
	item_stats[id]->special.value[1] = 0;
	item_stats[id]->special.value[2] = 0;
	item_stats[id]->special.value[3] = 0;
	item_stats[id]->special.affect_flags = 0;
	item_stats[id]->special.resistance_flags = 0;
	item_stats[id]->special.immunity_flags = 0;
	item_stats[id]->property_flags = 0;
	item_stats[id]->weight = 0;
	item_stats[id]->volume = 0;
	STRDUP(item_stats[id]->creators_note, "standard item parchment stats");
	item_stats[id]->virtual_id = ++id;

	assert(id == INIT_ITEM_TOP+1);
}

void create_tinyworld_items(void)
{
	vid_t id;

	create_tinyworld_item_descriptions();
	create_tinyworld_item_stats();

	item_data_top = INIT_ITEM_TOP;

	MALLOC(item_data, ITEM_DATA *, item_data_top+1);
	id = 0;

	MALLOC(item_data[id], ITEM_DATA, 1);
	item_data[id]->descr = item_descr[id];
	item_data[id]->stats = item_stats[id];
	item_data[id]->clan = 0;
	item_data[id]->limit = (limit_t) -1;
	item_data[id]->count = 0;
	STRDUP(item_data[id]->creators_note, "standard item corpse");
	item_data[id]->virtual_id = ++id;

	MALLOC(item_data[id], ITEM_DATA, 1);
	item_data[id]->descr = item_descr[id];
	item_data[id]->stats = item_stats[id];
	item_data[id]->clan = 0;
	item_data[id]->limit = (limit_t) -1;
	item_data[id]->count = 0;
	STRDUP(item_data[id]->creators_note, "standard item note");
	item_data[id]->virtual_id = ++id;

	assert(id == INIT_ITEM_TOP+1);
}

void save_tinyworld_item_description(ITEM_DESCR_DATA *idescr)
{
	char *dbErrMsg = NULL;

	SQLITE_EXEC_PRINTF(db, "REPLACE INTO item_descr_data "
				"(item_descr_id, proper, room, general, creators_note)"
				" VALUES (%u, %Q, %Q, %Q, %Q)",
				NULL,
				NULL,
				dbErrMsg,
				idescr->virtual_id,
				idescr->proper,
				idescr->room,
				idescr->general,
				idescr->creators_note);
	idescr->key->save(idescr->virtual_id);
	idescr->extra->save(idescr->virtual_id);
}

static void save_tinyworld_item_descriptions(void)
{
	for (top_t i = 0; i <= item_descr_top; i++)
	{
		save_tinyworld_item_description(item_descr[i]);
	}
}

static void save_tinyworld_item_stat_flags(ITEM_STATS_DATA *d)
{
	char *dbErrMsg = NULL;

	SQLITE_EXEC_PRINTF(db,
			"DELETE FROM item_stats_flags WHERE item_stats_id = %u",
			NULL,
			NULL,
			dbErrMsg,
			d->virtual_id);

	save_flags(d->virtual_id, (ubyte8) d->material_flags, str_material_f,
			"item_stats");

	save_flags(d->virtual_id, d->wear_flags, str_wear_pos_f,
			"item_stats");

	save_flags(d->virtual_id, d->attribute_flags, str_attribute_f,
			"item_stats");

	save_flags(d->virtual_id, d->special.affect_flags, str_affect_f,
			"item_stats");

	save_flags(d->virtual_id,
			d->special.resistance_flags, str_resistance_f, "item_stats");
	
	save_flags(d->virtual_id, d->special.immunity_flags, str_immunity_f,
			"item_stats");

	save_flags(d->virtual_id, d->property_flags, str_property_f, "item_stats");
}

void save_tinyworld_item_stat(ITEM_STATS_DATA *istats)
{
	char *dbErrMsg = NULL;

	SQLITE_EXEC_PRINTF(db, "REPLACE INTO item_stats_data "
				"(item_stats_id, item_type,"
				" value0, value1, value2, value3, weight, volume, creators_note)"
				" VALUES "
				"(%u, %Q,"
				" %d, %d, %d, %d, %u, %u, %Q)",
				NULL,
				NULL,
				dbErrMsg,
				istats->virtual_id,
					str_item_type[istats->item_type],
				istats->special.value[0],
					istats->special.value[1],
					istats->special.value[2],
					istats->special.value[3],
					istats->weight,
					istats->volume,
					istats->creators_note);
	save_tinyworld_item_stat_flags(istats);
}

static void save_tinyworld_item_stats(void)
{
	for (top_t i = 0; i <= item_stats_top; i++)
	{
		save_tinyworld_item_stat(item_stats[i]);
	}
}

void save_tinyworld_item(ITEM_DATA *idata)
{
	char *dbErrMsg = NULL;

	SQLITE_EXEC_PRINTF(db, "REPLACE INTO item_data "
				"(item_data_id, item_descr_id, item_stats_id,"
				" clan, max_existing, creators_note)"
				" VALUES (%u, %u, %u, %u, %d, %Q)",
				NULL,
				NULL,
				dbErrMsg,
				idata->virtual_id,
				idata->descr->virtual_id,
				idata->stats->virtual_id,
				idata->clan,
				idata->limit,
				idata->creators_note);
}

static void save_tinyworld_items(void)
{
	for (top_t i = 0; i <= item_data_top; i++)
	{
		save_tinyworld_item(item_data[i]);
	}
}

void free_tinyworld_item_description(ITEM_DESCR_DATA *idescr)
{
	delete idescr->key;
	free(idescr->proper);
	free(idescr->room);
	free(idescr->general);
	delete idescr->extra;
	free(idescr->creators_note);
	free(idescr);
}

static void free_tinyworld_item_descriptions(void)
{
	for (top_t i = 0; i <= item_descr_top; i++)
	{
		free_tinyworld_item_description(item_descr[i]);
	}
	FREE(item_descr);	
	item_descr_top = -1;
	idescrIndex.clear();
}

void free_tinyworld_item_stat(ITEM_STATS_DATA *istats)
{
	free(istats->creators_note);
	free(istats);
}

static void free_tinyworld_item_stats(void)
{
	for (top_t i = 0; i <= item_stats_top; i++)
	{
		free_tinyworld_item_stat(item_stats[i]);
	}
	FREE(item_stats);
	item_stats_top = -1;
	istatsIndex.clear();
}

void free_tinyworld_item(ITEM_DATA *idata)
{
	free(idata->creators_note);
	free(idata);
}

static void free_tinyworld_items(void)
{
	for (top_t i = 0; i <= item_data_top; i++)
	{
		free_tinyworld_item(item_data[i]);
	}
	FREE(item_data);
	item_data_top = -1;
	idataIndex.clear();
}

int callback_item_descr(void *pArg,
		int argc, char **argv, char **columnName)
{
	item_descr_top++;
	REALLOC(item_descr, ITEM_DESCR_DATA *, item_descr_top+1);
	MALLOC(item_descr[item_descr_top], ITEM_DESCR_DATA, 1);

	assert(argc == 5);

	for (int i = 0; i < argc; i++)
	{
		if (strcmp(columnName[i], "item_descr_id")==0)
		{
			item_descr[item_descr_top]->virtual_id = atoi(argv[i]);
			continue;
		}
		if (strcmp(columnName[i], "proper")==0)
		{
			STRDUP(item_descr[item_descr_top]->proper, argv[i]);
			continue;
		}
		if (strcmp(columnName[i], "room")==0)
		{
			STRDUP(item_descr[item_descr_top]->room, argv[i]);
			continue;
		}
		if (strcmp(columnName[i], "general")==0)
		{
			STRDUP(item_descr[item_descr_top]->general, argv[i]);
			continue;
		}
		if (strcmp(columnName[i], "creators_note")==0)
		{
			STRDUP(item_descr[item_descr_top]->creators_note, argv[i]);
			continue;
		}
		exitlog("/column not used/%s", columnName[i]);
	}
	item_descr[item_descr_top]->key = new Words("item_descr");
	item_descr[item_descr_top]->key->load(
			item_descr[item_descr_top]->virtual_id);

	item_descr[item_descr_top]->extra = new ExtraDescr("item_descr",
			item_descr[item_descr_top]->virtual_id);
	idescrIndex.set(item_descr[item_descr_top]->virtual_id, item_descr_top);
	return 0;
}

static void load_tinyworld_item_descriptions(void)
{
	char *dbErrMsg = NULL;

	SQLITE_EXEC(db, "SELECT * FROM item_descr_data ORDER BY item_descr_id",
				callback_item_descr,
				NULL,
				dbErrMsg);
}

int callback_item_stats_flags(void *pArg,
		int argc, char **argv, char **columnName)
{
	ITEM_STATS_DATA *istats = NULL;

	assert(argc == 2);

	for (int i = 0; i < argc; i++)
	{
		if (strcmp(columnName[i], "item_stats_id")==0)
		{
			istats = item_stats[istatsIndex.get(argv[i])];
			continue;
		}
		if (strcmp(columnName[i], "flag")==0)
		{
continue;
			if (strncmp("MTRL_", argv[i], 5)==0)
			{
				continue;
			}
			if (strncmp("WEAR_", argv[i], 5)==0)
			{
				continue;
			}
			if (strncmp("ATTR_", argv[i], 5)==0)
			{
				continue;
			}
			if (strncmp("AFF_", argv[i], 4)==0)
			{
				continue;
			}
			if (strncmp("RESIST_", argv[i], 7)==0)
			{
				continue;
			}
			if (strncmp("IMMUNE_", argv[i], 7)==0)
			{
				continue;
			}
			if (strncmp("PROP_", argv[i], 5)==0)
			{
				continue;
			}
			exitlog("/flag not categorized/%s", argv[i]);
		}
		exitlog("/column not used/%s", columnName[i]);
	}
	return 0;
}

int callback_item_stats(void *pArg,
		int argc, char **argv, char **columnName)
{
	item_stats_top++;
	REALLOC(item_stats, ITEM_STATS_DATA *, item_stats_top+1);
	MALLOC(item_stats[item_stats_top], ITEM_STATS_DATA, 1);

	item_stats[item_stats_top]->item_type = NO_ITEM_TYPE;
	item_stats[item_stats_top]->material_flags = 0;
	item_stats[item_stats_top]->wear_flags = 0;
	item_stats[item_stats_top]->attribute_flags = 0;
	item_stats[item_stats_top]->special.affect_flags = 0;
	item_stats[item_stats_top]->special.resistance_flags = 0;
	item_stats[item_stats_top]->special.immunity_flags = 0;
	item_stats[item_stats_top]->property_flags = 0;

	assert(argc == 9);

	for (int i = 0; i < argc; i++)
	{
		if (strcmp(columnName[i], "item_stats_id")==0)
		{
			item_stats[item_stats_top]->virtual_id = atoi(argv[i]);
			continue;
		}
		if (strcmp(columnName[i], "item_type")==0)
		{
			item_stats[item_stats_top]->item_type
				= get_index(argv[i], str_item_type);
			continue;
		}
		if (strcmp(columnName[i], "value0")==0)
		{
			item_stats[item_stats_top]->special.value[0] = atoi(argv[i]);
			continue;
		}
		if (strcmp(columnName[i], "value1")==0)
		{
			item_stats[item_stats_top]->special.value[1] = atoi(argv[i]);
			continue;
		}
		if (strcmp(columnName[i], "value2")==0)
		{
			item_stats[item_stats_top]->special.value[2] = atoi(argv[i]);
			continue;
		}
		if (strcmp(columnName[i], "value3")==0)
		{
			item_stats[item_stats_top]->special.value[3] = atoi(argv[i]);
			continue;
		}
		if (strcmp(columnName[i], "weight")==0)
		{
			item_stats[item_stats_top]->weight = atoi(argv[i]);
			continue;
		}
		if (strcmp(columnName[i], "volume")==0)
		{
			item_stats[item_stats_top]->volume = atoi(argv[i]);
			continue;
		}
		if (strcmp(columnName[i], "creators_note")==0)
		{
			STRDUP(item_stats[item_stats_top]->creators_note, argv[i]);
			continue;
		}
		exitlog("/column not used/%s", columnName[i]);
	}
	istatsIndex.set(item_stats[item_stats_top]->virtual_id, item_stats_top);
	return 0;
}

static void load_tinyworld_item_stats(void)
{
	char *dbErrMsg = NULL;

	SQLITE_EXEC(db, "SELECT * FROM item_stats_data ORDER BY item_stats_id",
				callback_item_stats,
				NULL,
				dbErrMsg);

	SQLITE_EXEC(db, "SELECT * FROM item_stats_flags",
				callback_item_stats_flags,
				NULL,
				dbErrMsg);
}

int callback_item_data(void *pArg,
		int argc, char **argv, char **columnName)
{
	item_data_top++;
	REALLOC(item_data, ITEM_DATA *, item_data_top+1);
	MALLOC(item_data[item_data_top], ITEM_DATA, 1);
	item_data[item_data_top]->count = 0;

	assert(argc == 6);

	for (int i = 0; i < argc; i++)
	{
		if (strcmp(columnName[i], "item_data_id")==0)
		{
			item_data[item_data_top]->virtual_id = atoi(argv[i]);
			continue;
		}
		if (strcmp(columnName[i], "item_descr_id")==0)
		{
			item_data[item_data_top]->descr =
				item_descr[idescrIndex.get(atoi(argv[i]))];
			continue;
		}
		if (strcmp(columnName[i], "item_stats_id")==0)
		{
			item_data[item_data_top]->stats =
				item_stats[istatsIndex.get(atoi(argv[i]))];
			continue;
		}
		if (strcmp(columnName[i], "clan")==0)
		{
			item_data[item_data_top]->clan = atoi(argv[i]);
			continue;
		}
		if (strcmp(columnName[i], "max_existing")==0)
		{
			item_data[item_data_top]->limit = atoi(argv[i]);
			continue;
		}
		if (strcmp(columnName[i], "creators_note")==0)
		{
			STRDUP(item_data[item_data_top]->creators_note, argv[i]);
			continue;
		}
		exitlog("/column not used/%s", columnName[i]);
	}
	idataIndex.set(item_data[item_data_top]->virtual_id, item_data_top);
	return 0;
}

static void load_tinyworld_items(void)
{
	char *dbErrMsg = NULL;

	SQLITE_EXEC(db, "SELECT * FROM item_data ORDER BY item_data_id",
				callback_item_data,
				NULL,
				dbErrMsg);
}

/** Initialize the tinyworld item module. */
void init_tinyworld_item_module(void)
{
	char *dbErrMsg = NULL;

	create_tinyworld_items();

	BEGIN_TRANSACTION(db, dbErrMsg);
	save_tinyworld_item_descriptions();
	save_tinyworld_item_stats();
	save_tinyworld_items();
	COMMIT_TRANSACTION(db, dbErrMsg);

	free_tinyworld_item_descriptions();
	free_tinyworld_item_stats();
	free_tinyworld_items();
	load_tinyworld_item_descriptions();
	load_tinyworld_item_stats();
	load_tinyworld_items();
}

/** Finalize the tinyworld room module. */
void finit_tinyworld_item_module(void)
{
	free_tinyworld_item_descriptions();
	free_tinyworld_item_stats();
	free_tinyworld_items();
}

Item::Item(ITEM_DATA *idata, cid_t loader_id, cid_t container_id)
: Container(CONTAINER_ITEM, container_id)
{
	item_data = idata;
	loaderId = loader_id;
	setWeight(5);
	setVolume(6);

	prev_item = NULL;
	if (item_list == NULL)
	{
		next_item = NULL;
	}
	else
	{
		next_item = item_list;
		item_list->prev_item = this;
	}
	item_list = this;
	loaderId = loader_id;
	game_stat.items_created++;
}

Item::~Item(void)
{
	if (prev_item)
	{
		prev_item->next_item = next_item;
	}
	else
	{
		item_list = next_item;
	}
	if (next_item)
	{
		next_item->prev_item = prev_item;
	}
	game_stat.items_destroyed++;
}

Item *create_item(vid_t virtual_id, cid_t loader, Container *container)
{
	ITEM_DATA *idata;
	Item *item;
	int rn;
  
	if ((rn = idataIndex.get(virtual_id)) < 0)
	{
		return NULL;
	}
	idata = item_data[rn];

	if (find_item_type(idata)==ITEM_CONTAINER)
	{
		item = new ItemContainer(idata, loader);
	}
	else if (find_item_type(idata)==ITEM_NOTE)
	{
		item = new ItemNote(idata, loader);
	}
	else
	{
		exitlog("/no Item sub-class for %s",
				str_item_type[find_item_type(idata)]);
		return NULL;
	}
	addlog_new_item(item, loader);
	container->hold(item);
	return item;
}

void add_item_data(ITEM_DATA *idata)
{
	vid_t i;
	if ((i = idataIndex.get(idata->virtual_id)) == (vid_t) -1)
	{
		item_data_top++;
		REALLOC(item_data, ITEM_DATA *, item_data_top+1);
		item_data[item_data_top] = idata;
		idataIndex.set(idata->virtual_id, item_data_top);
		return;
	}
	idata->count = item_data[i]->count;
	free_tinyworld_item(item_data[i]);
	item_data[i] = idata;
}

void add_item_descr(ITEM_DESCR_DATA *idescr)
{
	vid_t i;
	if ((i = idescrIndex.get(idescr->virtual_id)) == (vid_t) -1)
	{
		item_descr_top++;
		REALLOC(item_descr, ITEM_DESCR_DATA *, item_descr_top+1);
		item_descr[item_descr_top] = idescr;
		return;
	}
	free_tinyworld_item_description(item_descr[i]);
	item_descr[i] = idescr;
}

void add_item_stats(ITEM_STATS_DATA *istats)
{
	vid_t i;
	if ((i = istatsIndex.get(istats->virtual_id)) == (vid_t) -1)
	{
		item_stats_top++;
		REALLOC(item_stats, ITEM_STATS_DATA *, item_stats_top+1);
		item_stats[item_stats_top] = istats;
		return;
	}
	free_tinyworld_item_stat(item_stats[i]);
	item_stats[i] = istats;
}

void destroy_tinyworld_item(vid_t i)
{
	idataIndex.reset(i);
	while ((sbyte4) i <= item_data_top)
	{
		item_data[i] = item_data[++i];
	}
	item_data_top--;
}

void destroy_tinyworld_item_stat(vid_t i)
{
	istatsIndex.reset(i);
	while ((sbyte4) i <= item_stats_top)
	{
		item_stats[i] = item_stats[++i];
	}
	item_stats_top--;
}

void destroy_tinyworld_item_description(vid_t i)
{
	idescrIndex.reset(i);
	while ((sbyte4) i <= item_descr_top)
	{
		item_descr[i] = item_descr[++i];
	}
	item_descr_top--;
}

/** \todo what happens when we've got this in the buffer and someone else
    decides to destroy descr and/or stats? take care of these pointers.
	 */
void copy_tinyworld_item(ITEM_DATA *idata, vid_t i)
{
	idata->virtual_id = item_data[i]->virtual_id;
	idata->descr = item_data[i]->descr;
	idata->stats = item_data[i]->stats;
	STRDUP(idata->creators_note, item_data[i]->creators_note);
	idata->clan = item_data[i]->clan;
	idata->limit = item_data[i]->limit;
	idata->count = 0;
}

void copy_tinyworld_item_description(ITEM_DESCR_DATA *idescr, vid_t i)
{
	idescr->virtual_id = item_descr[i]->virtual_id;
	idescr->key = new Words(item_descr[i]->key);
	STRDUP(idescr->proper, item_descr[i]->proper);
	STRDUP(idescr->room, item_descr[i]->room);
	STRDUP(idescr->general, item_descr[i]->general);
	idescr->extra = new ExtraDescr(item_descr[i]->extra);
	STRDUP(idescr->creators_note, item_descr[i]->creators_note);
}

void copy_tinyworld_item_stat(ITEM_STATS_DATA *istats, vid_t i)
{
	memcpy(istats, item_stats[i], sizeof(ITEM_STATS_DATA));
	STRDUP(istats->creators_note, item_stats[i]->creators_note);
}

const ITEM_DESCR_DATA *find_item_descr(vid_t id)
{
	if ((id = idescrIndex.get(id)) == (vid_t) -1)
	{
		return NULL;
	}
	return item_descr[id];
}

const ITEM_STATS_DATA *find_item_stats(vid_t id)
{
	if ((id = istatsIndex.get(id)) == (vid_t) -1)
	{
		return NULL;
	}
	return item_stats[id];
}

const ITEM_DATA *find_item_data(vid_t id)
{
	if ((id = idataIndex.get(id)) == (vid_t) -1)
	{
		return NULL;
	}
	return item_data[id];
}

