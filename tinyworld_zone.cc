/* **************************************************************************
    Nilgiri the Forgotten World
    (C) 2004
   ************************************************************************** */
/** \file
    \brief Zone object header
 */
#include"db.h"
#include"tinyworld_zone.h"

Zone **zone_table = NULL;
top_t zone_table_top = -1;

VidIndex zoneIndex;

Zone::Zone(ZONE_DATA *zdata, cid_t container_id)
	: Container(CONTAINER_ZONE, container_id)
{
	data = zdata;
}

void free_tinyworld_zone(ZONE_DATA *zdata)
{
	free(zdata->name);
	free(zdata->descr);
	free(zdata->creators_note);
	free(zdata->coord);
	free(zdata);
}

Zone::~Zone(void)
{
	free_tinyworld_zone(data);
}

void Zone::save(void)
{
	char *dbErrMsg;

	SQLITE_EXEC_PRINTF(db, "REPLACE INTO zone "
				"(zone_id, container_id, name, descr, zone_type, homeland, "
				"min_lvl, max_lvl, lifespan, top, creators_note) "
				"VALUES"
				"(%u, %u, %Q, %Q, %Q, %Q, %u, %u, %u, %u, %Q)",
				NULL,
				NULL,
				dbErrMsg,
				data->virtual_id,
				getCID(),
				data->name,
				data->descr,
				str_zone_type[data->zone_type],
				str_homeland[data->homeland],
				data->min_lvl,
				data->max_lvl,
				data->special.lifespan,
				data->top,
				data->creators_note);
	coord_save("zone", data->virtual_id, data->coord);
	SQLITE_EXEC_PRINTF(db, "DELETE FROM zone_flags WHERE zone_id = %u",
				NULL,
				NULL,
				dbErrMsg,
				data->virtual_id);
	save_flags(data->virtual_id, data->special.flags, str_zone_f, "zone");
}

/** This is a static object method of Zone that does a deep copy of
     ZONE_DATA into data of the Zone object.
    \param zdata The ZONE_DATA destination of the deep copy
	 \param i The VID of the zone where the ZONE_DATA is copied from
 */
void Zone::copy_tinyworld_zone_data(ZONE_DATA *zdata, vid_t i)
{
	memcpy(zdata, zone_table[i]->data, sizeof(ZONE_DATA));
	STRDUP(zdata->name, zone_table[i]->data->name);
	STRDUP(zdata->descr, zone_table[i]->data->descr);
	if (zone_table[i]->data->coord)
	{
		MALLOC(zdata->coord, COORD, 1);
		memcpy(zdata->coord, zone_table[i]->data->coord, sizeof(COORD));
	}
	else
	{
		zdata->coord = NULL;
	}
	STRDUP(zdata->creators_note, zone_table[i]->data->creators_note);
}

/** Destroys the zone information in the database and in memory.
    \param i The VID of the zone to destroy
 */
void destroy_tinyworld_zone(vid_t i)
{
	char *dbErrMsg;
	BEGIN_TRANSACTION(db, dbErrMsg);
	SQLITE_EXEC_PRINTF(db, "DELETE FROM zone WHERE zone_id = %u",
				NULL,
				NULL,
				dbErrMsg,
				zone_table[i]->getVID());
	SQLITE_EXEC_PRINTF(db, "DELETE FROM zone_flags WHERE zone_id = %u",
				NULL,
				NULL,
				dbErrMsg,
				zone_table[i]->getVID());
	SQLITE_EXEC_PRINTF(db, "DELETE FROM zone_coord WHERE zone_id = %u",
				NULL,
				NULL,
				dbErrMsg,
				zone_table[i]->getVID());
	COMMIT_TRANSACTION(db, dbErrMsg);
	delete zone_table[i];
	if (zone_table_top > (sbyte4) i)
	{
		memmove(zone_table + i, zone_table + i + 1,
				sizeof(char *) * zone_table_top - i);
	}
	REALLOC(zone_table, Zone *, zone_table_top);
	zone_table_top--;
}

/** Initialize the initial zones on boot. */
static void create_tinyworld_zones(void)
{
	vid_t vid = 0;
	cid_t cid = CID_ZONE_BOT;
	ZONE_DATA *zdata;

	MALLOC(zone_table, Zone *, NUM_START_ZONES);

	CALLOC(zdata, ZONE_DATA, 1);
	STRDUP(zdata->name, "Tinyworld");
	zdata->descr
		= wrap("Various world administration happens in these zones.\r\n");
	zdata->top = 8;
	zdata->homeland = IMMORTAL;
	STRDUP(zdata->creators_note, "This is a game generated tinyworld zone.");
	zdata->virtual_id = ++vid;
	zone_table_top++;
	zone_table[zone_table_top] = new Zone(zdata, cid++);
	zoneIndex.set(vid, zone_table_top);

	CALLOC(zdata, ZONE_DATA, 1);
	STRDUP(zdata->name, "End of the Tinyworld");
	zdata->descr = wrap("This is the absolute end of the tinyworld.\r\n");
	zdata->top = MAX_ROOM_VID;
	zdata->homeland = IMMORTAL;
	STRDUP(zdata->creators_note, "This is a game generated tinyworld zone.");
	zdata->virtual_id = ++vid;
	zone_table_top++;
	zone_table[zone_table_top] = new Zone(zdata, cid++);
	zoneIndex.set(vid, zone_table_top);

	assert(vid == NUM_START_ZONES);
	assert(cid == CID_ZONE_TOP + 1);
}

/** Free all the zones. */
static void free_tinyworld_zones(void)
{
	for (top_t i = 0; i <= zone_table_top; i++)
	{
		delete zone_table[i];
	}
	zone_table = NULL;
	zone_table_top = -1;
}


int callback_zone_flags(void *pArg, int argc, char **argv, char **columnName)
{
	ZONE_DATA *zdata = (ZONE_DATA *) pArg;
	assert(argc == 1);
	assert(strcmp(columnName[0], "flag") == 0);

	if (strncmp("ZONE_", argv[0], 5) == 0)
	{
		ADD_BIT(argv[0], str_zone_flags, zdata->special.flags);
	}
	else
	{
		exitlog("/flag not categorized/%s", argv[0]);
	}
	return 0;
}

int callback_zone(void *pArg, int argc, char **argv, char **columnName)
{
	char *dbErrMsg;
	cid_t cid = 0;
	assert(argc == 11);
	ZONE_DATA *zdata;
	CALLOC(zdata, ZONE_DATA, 1);
	for (int i = 0; i < argc; i++)
	{
		if (strcmp(columnName[i], "zone_id") == 0)
		{
			zdata->virtual_id = atoi(argv[i]);
			continue;
		}
		if (strcmp(columnName[i], "container_id") == 0)
		{
			cid = atoi(argv[i]);
			continue;
		}
		if (strcmp(columnName[i], "name") == 0)
		{
			STRDUP(zdata->name, argv[i]);
			continue;
		}
		if (strcmp(columnName[i], "descr") == 0)
		{
			STRDUP(zdata->descr, argv[i]);
			continue;
		}
		if (strcmp(columnName[i], "zone_type") == 0)
		{
			zdata->zone_type = atoi(argv[i]);
			continue;
		}
		if (strcmp(columnName[i], "homeland") == 0)
		{
			zdata->homeland = get_index(argv[i], str_homeland);
			continue;
		}
		if (strcmp(columnName[i], "min_lvl") == 0)
		{
			zdata->min_lvl = atoi(argv[i]);
			continue;
		}
		if (strcmp(columnName[i], "max_lvl") == 0)
		{
			zdata->max_lvl = atoi(argv[i]);
			continue;
		}
		if (strcmp(columnName[i], "top") == 0)
		{
			zdata->top = atoi(argv[i]);
			continue;
		}
		if (strcmp(columnName[i], "lifespan") == 0)
		{
			zdata->special.lifespan = atoi(argv[i]);
			continue;
		}
		if (strcmp(columnName[i], "creators_note") == 0)
		{
			STRDUP(zdata->creators_note, argv[i]);
			continue;
		}
		exitlog("/column not used/%s", columnName[i]);
	}
	zone_table_top++;
	REALLOC(zone_table, Zone *, zone_table_top+1);
	zone_table[zone_table_top] = new Zone(zdata, cid);
	zoneIndex.set(zdata->virtual_id, zone_table_top);
	coord_load("zone", zdata->virtual_id, zdata->coord);
	
	SQLITE_EXEC_PRINTF(db, "SELECT flag FROM zone_flags WHERE zone_id = %u",
				callback_zone_flags,
				zdata,
				dbErrMsg,
				zdata->virtual_id);
	return 0;
}

void load_tinyworld_zones(void)
{
	char *dbErrMsg;

	SQLITE_EXEC_PRINTF(db, "SELECT * FROM zone ORDER BY zone_id",
				callback_zone,
				NULL,
				dbErrMsg);
}

void save_tinyworld_zones(void)
{
	for (top_t i = 0; i <= zone_table_top; i++)
	{
		zone_table[i]->save();
	}
}

/** \todo binary search */
Zone *find_zone(vid_t id)
{
	if ((id = zoneIndex.get(id)) == (vid_t) -1)
	{
		return NULL;
	}
	return zone_table[id];
}

Zone *add_zone(ZONE_DATA *zdata)
{
	Zone *zone;
	top_t bot, top, i;

	bot = 0;
	top = zone_table_top;

	while (1)
	{
		i = (bot + top) / 2;
		if (zone_table_top >= i && zone_table[i]->getVID() == zdata->virtual_id)
		{
			zone = zone_table[i];
			zone->setData(zdata);
			break;
		}
		if (bot >= top)
		{
			zone_table_top++;
			REALLOC(zone_table, Zone *, zone_table_top+1);
			for (; i <= zone_table_top - 1; i++)
			{
				if (zone_table[i]->getVID() > zdata->virtual_id)
				{
					memmove(zone_table+i+1, zone_table+i,
						sizeof(Zone *)*(zone_table_top - i));
					break;
				}
			}
			zone = zone_table[i] = new Zone(zdata);
			zoneIndex.set(zdata->virtual_id, zone_table_top);
			break;
		}
		if (zone_table[i]->getVID() > zdata->virtual_id)
		{
			top = i - 1;
		}
		else
		{
			bot = i + 1;
		}
	}
	return zone;
}

/** Returns the next zone after the the current one identified by Zone's
    virtual id.  First it checks if the zone exists, if not then does a binary
	 search to see where the current zone's virtual id would go in order to
	 find its next zone.
	 \param vid The virtual id of the current zone
	 \return Zone The next zone in order
 */
Zone *get_next_zone(vid_t vid)
{
	top_t i;

	if ((vid_t) (i = zoneIndex.get(vid)) != (vid_t) -1)
	{
		i++;
		if (i > zone_table_top)
		{
			return NULL;
		}
		return zone_table[i];
	}

	top_t bot, top;

	bot = 0;
	top = zone_table_top;

	while (1)
	{
		i = (bot + top) / 2;
		if (bot >= top)
		{
			for (; i <= zone_table_top; i++)
			{
				if (zone_table[i]->getVID() > vid)
				{
					break;
				}
			}
			if (i > zone_table_top)
			{
				return NULL;
			}
			return zone_table[i];
		}
		if (zone_table[i]->getVID() > vid)
		{
			top = i - 1;
		}
		else
		{
			bot = i + 1;
		}
	}
}

Zone *get_room_zone(vid_t vid)
{
	top_t bot, top, i;
	
	bot = 0;
	top = zone_table_top;

	while (1)
	{
		i = (bot + top) / 2;
		if (zone_table[i]->getVID() == vid)
		{
			return zone_table[i];
		}
		if (bot >= top)
		{
			for (; i <= zone_table_top; i++)
			{
				if (zone_table[i]->getVID() > vid)
				{
					break;
				}
			}
			while (--i > -1)
			{
				if (zone_table[i]->getBot() <= vid
						&& vid <= zone_table[i]->getTop())
				{
					return zone_table[i];
				}
			}
			return NULL;
		}
		if (zone_table[i]->getTop() > vid)
		{
			top = i - 1;
		}
		else
		{
			bot = i + 1;
		}
	}
}

/** Returns the previous zone after the the current one identified by Zone's
    virtual id.  First it checks if the zone exists, if not then does a binary
	 search to see where the current zone's virtual id would go in order to
	 find its previous zone.
	 \param vid The virtual id of the current zone
	 \return Zone The next zone in order
 */
Zone *get_prev_zone(vid_t vid)
{
	top_t i;

	if ((vid_t) (i = zoneIndex.get(vid)) != (vid_t) -1)
	{
		i--;
		if (i == -1)
		{
			return NULL;
		}
		return zone_table[i];
	}

	top_t bot, top;

	bot = 0;
	top = zone_table_top;

	while (1)
	{
		i = (bot + top) / 2;
		if (bot >= top)
		{
			for (; i <= zone_table_top; i++)
			{
				if (zone_table[i]->getVID() > vid)
				{
					break;
				}
			}
			i--;
			if (i == -1)
			{
				return NULL;
			}
			return zone_table[i];
		}
		if (zone_table[i]->getVID() > vid)
		{
			top = i - 1;
		}
		else
		{
			bot = i + 1;
		}
	}
}

void init_tinyworld_zone_module(void)
{
	char *dbErrMsg;
	create_tinyworld_zones();
	BEGIN_TRANSACTION(db, dbErrMsg);
	save_tinyworld_zones();
	COMMIT_TRANSACTION(db, dbErrMsg);
	free_tinyworld_zones();

	load_tinyworld_zones();
	assert(zone_table_top > -1);
}

void finit_tinyworld_zone_module(void)
{
	free_tinyworld_zones();
}
