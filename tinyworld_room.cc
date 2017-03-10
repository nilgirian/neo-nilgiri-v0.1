/* **************************************************************************
    Nilgiri the Forgotten World
    (C) 2004
   ************************************************************************** */
/** \file
    \brief Room object source
 */
#include<string.h>
#include<errno.h>
#include"err.h"
#include"tinyworld_room.h"

Room **world = NULL;
top_t world_top = -1;

VidIndex roomIndex;

static void create_tinyworld_rooms(void)
{
	vid_t vid = 0;
	vid_t cid = CID_ROOM_BOT;
	ROOM_DATA *rdata;

	MALLOC(world, Room *, NUM_START_ROOMS);

	CALLOC(rdata, ROOM_DATA, 1);
	STRDUP(rdata->creators_note, "Lost objects go here.\r\n");
	STRDUP(rdata->name, "The void");
	STRDUP(rdata->descr, "Endless nothingness.\r\n");
	rdata->virtual_id= ++vid;
	world[++world_top] = new Room(rdata, cid++);
	roomIndex.set(vid, world_top);

	CALLOC(rdata, ROOM_DATA, 1);
	STRDUP(rdata->creators_note, "Link-dead players go here\r\n");
	STRDUP(rdata->name, "A place between sight and sound");
	rdata->descr = wrap("\
   This is a floating formless void, detached from all sensation of physical \
matter, surrounded by a swirling glowing light, which fades into relative \
darkness around you without any trace of edge or shadow.");
	rdata->virtual_id = ++vid;
	world[++world_top] = new Room(rdata, cid++);
	roomIndex.set(vid, world_top);

	CALLOC(rdata, ROOM_DATA, 1);
	STRDUP(rdata->creators_note, "Bad players go here\r\n");
	STRDUP(rdata->name, "Purgatory");
	rdata->descr = wrap("\
   Swirling glowing light fades into the relative darkness of this formless \
void.  Detached from all sensation of physical matter, the traces of edges or \
shadow are nowhere to be found.");
	rdata->virtual_id = ++vid;
	world[++world_top] = new Room(rdata, cid++);
	roomIndex.set(vid, world_top);

	CALLOC(rdata, ROOM_DATA, 1);
	STRDUP(rdata->creators_note, "Link-dead players go here\r\n");
	STRDUP(rdata->name, "Limbo");
	STRDUP(rdata->descr, "Only bimbos go to limbos.\r\n");
	rdata->virtual_id = ++vid;
	world[++world_top] = new Room(rdata, cid++);
	roomIndex.set(vid, world_top);

	CALLOC(rdata, ROOM_DATA, 1);
	STRDUP(rdata->creators_note, "Immortals congregate here\r\n");
	STRDUP(rdata->name, "The chat room");
	rdata->descr = wrap("\
  This quiet, cozy parlor is warmed by a gentle magical fire which twinkles \
happily in a warm fireplace. There are several exits out of this room, \
clearly the owner needs them.");
	rdata->virtual_id = ++vid;
	world[++world_top] = new Room(rdata, cid++);
	roomIndex.set(vid, world_top);

	CALLOC(rdata, ROOM_DATA, 1);
	STRDUP(rdata->creators_note, "The code board is here\r\n");
	STRDUP(rdata->name, "Enex's room");
	rdata->descr = wrap("\
   This is Enex's room. It looks comfy but it looks like that Enex is almost \
never here since he always hangs out in either Limbo or Cory. You notice a \
huge fully integrated stereo system in one of the corners of the room.");
	rdata->virtual_id = ++vid;
	world[++world_top] = new Room(rdata, cid++);
	roomIndex.set(vid, world_top);

	CALLOC(rdata, ROOM_DATA, 1);
	STRDUP(rdata->creators_note, "The implementor board is here\r\n");
	STRDUP(rdata->name, "David's grungy apartment");
	rdata->descr = wrap("\
   You are in the playhouse of the Friendly One, Ensign. You hear music \
blasting out from four 7 feet tall speakers which made you feel like you're \
in another dimension. On the facing wall sits a fully integrated stereo \
system with all the works. In one corner you see a Sun Sparc workstation. \
(Xtrek perhaps? *wink*) In another corner sits a 30-inch TV with a Super NES \
and a whole library of cartridges. This place looks really comfortable, and \
you get the feeling that Ensign likes to have fun. :)");
	rdata->virtual_id = ++vid;
	world[++world_top] = new Room(rdata, cid++);
	roomIndex.set(vid, world_top);

	CALLOC(rdata, ROOM_DATA, 1);
	STRDUP(rdata->creators_note, "The god board is here\r\n");
	STRDUP(rdata->name, "Shizuka's room");
	rdata->descr = wrap("\
   This is Shizuka's room. On the walls are hundreds of her idol posters from \
her previous albums and advertisements. You notice that there is a mountain \
of stuffed animals in one corner. In the other corner there sits a nice stereo \
system. You find this room to be very comfortable and the carpeting beneath \
your feet makes you just want to lie down and take a short nap.");
	rdata->virtual_id = ++vid;
	world[++world_top] = new Room(rdata, cid++);
	roomIndex.set(vid, world_top);

	assert(vid == NUM_START_ROOMS);
	assert(cid == CID_ROOM_TOP + 1);
}

static void create_tinyworld_room_doors(void)
{
	world[roomIndex.get(5)]->createDir(NORTH, 7);
	world[roomIndex.get(5)]->createDir(EAST, 8);
	world[roomIndex.get(5)]->createDir(WEST, 6);

	world[roomIndex.get(6)]->createDir(EAST, 5);

	world[roomIndex.get(7)]->createDir(SOUTH, 5);
	
	world[roomIndex.get(8)]->createDir(WEST, 5);
}

Room::Room(ROOM_DATA *rdata, cid_t container_id)
	: Container(CONTAINER_ROOM, container_id)
{
	data = rdata;
	game_stat.rooms_created++;
}

void free_door(DOOR *door)
{
	delete door->key;
	free(door->descr);

	door->room = NULL;
	door->room_key = NULL;
}

Room::~Room(void)
{
	free_tinyworld_room(data);
	game_stat.rooms_destroyed++;
}

void Room::save(void)
{
	char *dbErrMsg = NULL;

#ifdef NEVER_DEFINED
fprintf(stderr, "vid:%u\n", getVID());
fprintf(stderr, "cid:%08lX\n", getCID());
fprintf(stderr, "name:%s\n", getName());
fprintf(stderr, "descr:%s\n", getDescr());
fprintf(stderr, "descrDark:%s\n", getDescrDark()==NULL?"":getDescrDark());
fprintf(stderr, "descrStorm:%s\n", getDescrStorm()==NULL?"":getDescrStorm());
fprintf(stderr, "sector:%s\n", str_sector_type[getSector()]);
fprintf(stderr, "treadFactor:%u\n", getOriginalTreadFactor());
fprintf(stderr, "clan:%u\n", getClan());
fprintf(stderr, "creators_note:%s\n", getCreatorsNote());
#endif

	SQLITE_EXEC_PRINTF(db, "REPLACE INTO room "
				"(room_id, container_id, name,"
				" descr, descr_dark, descr_storm,"
				" sector, tread,"
				" clan, creators_note)"
				" VALUES "
				"(%u, %u, %Q,"
				" %Q, %Q, %Q,"
				" %Q, %u,"
				" %u, %Q)",
				NULL,
				NULL,
				dbErrMsg,
				getVID(),
					getCID(),
					getName(),
				getDescr(),
					getDescrDark(),
					getDescrStorm(),
				str_sector_type[getSector()],
					getOriginalTreadFactor(),
				getClan(),
					getCreatorsNote());

	if (data->extra)
	{
		data->extra->save(data->virtual_id);
	}

	SQLITE_EXEC_PRINTF(db, "DELETE FROM room_flags WHERE room_id = %u",
				NULL,
				NULL,
				dbErrMsg,
				data->virtual_id);
	save_flags(data->virtual_id, data->special.flags, str_room_f, "room");

	for (dir_t dir = 0; dir < NO_DIR; dir++)
	{
		if (data->door[dir] == NULL)
		{
			continue;
		}
		SQLITE_EXEC_PRINTF(db, "REPLACE INTO room_door VALUES "
					"(%u, %u, %Q, %Q, %Q, %u, %u, %u)",
					NULL,
					NULL,
					dbErrMsg,
					data->virtual_id,
					dir,
					data->door[dir]->proper,
					data->door[dir]->descr,
					str_door_type[data->door[dir]->door_type],
					data->door[dir]->weight,
					data->door[dir]->room ? data->door[dir]->room->getVID() : 0,
					data->door[dir]->room_key
						?  data->door[dir]->room_key->virtual_id : 0);
		if (data->door[dir]->key)
		{
			data->door[dir]->key->save(data->virtual_id, dir);
		}
		SQLITE_EXEC_PRINTF(db, "DELETE FROM room_door_flags WHERE "
					"room_id = %u AND door_id = %u",
					NULL,
					NULL,
					dbErrMsg,
					data->virtual_id,
					dir);
		save_flags(data->virtual_id, dir,
				data->door[dir]->special.flags, str_door_f, "room", "door");
	}
}

void Room::createDir(dir_t dir, vid_t room_vid)
{
	CALLOC(data->door[dir], DOOR, 1);
	data->door[dir]->room = world[roomIndex.get(room_vid)];
}

void save_tinyworld_rooms()
{
	ubyte4 count = 0;

	for (top_t i = 0; i <= world_top; i++)
	{
		world[i]->save();
		count++;
	}
}

static void free_tinyworld_rooms(void)
{
	for (top_t i = 0; i <= world_top; i++)
	{
		delete world[i];
		world[i] = NULL;
	}
	free(world);
	world = NULL;
	world_top = -1;
}

int callback_room_flags(void *pArg, int argc, char **argv, char **columnName)
{
	ROOM_DATA *rdata = (ROOM_DATA *) pArg;
	assert(argc == 1);
	assert(strcmp(columnName[0], "flag")==0);

	if (strncmp("ROOM_", argv[0], 5) == 0)
	{
		//room flags
		ADD_BIT(argv[0], str_room_flags, rdata->special.flags);
	}
	else
	{
		exitlog("/flag not categorized/%s", argv[0]);
	}
	return 0;
}

int callback_room_door_flags(void *pArg,
		int argc, char **argv, char **columnName)
{
	DOOR *door = (DOOR *) pArg;
	assert(argc == 1);
	assert(strcmp(columnName[0], "flag")==0);

	if (strncmp("DOOR_", argv[0], 5) == 0)
	{
		//room door flags
		ADD_BIT(argv[0], str_door_flags, door->special.flags);
	}
	else
	{
		exitlog("/flag not categorized/%s", argv[0]);
	}
	return 0;
}

inline ROOM_DATA *Room::get_room_data(vid_t vid)
{
	Room *room = get_room(vid);
	return room->data;
}

int callback_room_door(void *pArg, int argc, char **argv, char **columnName)
{
	vid_t room_id;
	dir_t dir;
	char *dbErrMsg;
	ROOM_DATA *rdata;
	assert(argc == 8);

	for (int i = 0; i < argc; i++)
	{
		if (strcmp(columnName[i], "room_id") == 0)
		{
			room_id = atoi(argv[i]);
			rdata = Room::get_room_data(room_id);
			continue;
		}
		if (strcmp(columnName[i], "door_id") == 0)
		{
			dir = atoi(argv[i]);
			CALLOC(rdata->door[dir], DOOR, 1);
			continue;
		}
		if (strcmp(columnName[i], "proper") == 0)
		{
			STRDUP(rdata->door[dir]->proper, argv[i]);
			continue;
		}
		if (strcmp(columnName[i], "descr") == 0)
		{
			STRDUP(rdata->door[dir]->descr, argv[i]);
			continue;
		}
		if (strcmp(columnName[i], "weight") == 0)
		{
			rdata->door[dir]->weight = atoi(argv[i]);
			continue;
		}
		if (strcmp(columnName[i], "door_type") == 0)
		{
			rdata->door[dir]->door_type = get_index(argv[i], str_door_type);
			continue;
		}
		if (strcmp(columnName[i], "to_room_id") == 0)
		{
			vid_t to_room_vid = atoi(argv[i]);
			if (to_room_vid)
			{
				rdata->door[dir]->room = get_room(argv[i]);
			}
			continue;
		}
		if (strcmp(columnName[i], "room_key") == 0)
		{
			vid_t item_vid = atoi(argv[i]);
			if (item_vid)
			{
				rdata->door[dir]->room_key = get_item_data(item_vid);
			}
			continue;
		}
		exitlog("/column not used/%s", columnName[i]);
	}
	rdata->door[dir]->key = new Words("room", "door");
	rdata->door[dir]->key->load(rdata->virtual_id, dir);
	SQLITE_EXEC_PRINTF(db, "SELECT flag FROM room_door_flags "
			  "WHERE room_id = %u AND door_id = %u",
				callback_room_door_flags,
				rdata->door[dir],
				dbErrMsg,
				rdata->virtual_id,
				dir);
	return 0;
}

int callback_room(void *pArg, int argc, char **argv, char **columnName)
{
	char *name;
	cid_t cid;
	char *dbErrMsg;

	assert(argc == 10);

	ROOM_DATA *rdata;
	CALLOC(rdata, ROOM_DATA, 1);
	for (int i = 0; i < argc; i++)
	{
		if (strcmp(columnName[i], "room_id")==0)
		{
			rdata->virtual_id = atoi(argv[i]);
			continue;
		}
		if (strcmp(columnName[i], "container_id")==0)
		{
			cid = atoi(argv[i]);
			continue;
		}
		if (strcmp(columnName[i], "name")==0)
		{
			STRDUP(rdata->name, argv[i]);
			continue;
		}
		if (strcmp(columnName[i], "descr")==0)
		{
			STRDUP(rdata->descr, argv[i]);
			continue;
		}
		if (strcmp(columnName[i], "descr_dark")==0)
		{
			STRDUP(rdata->descr_dark, argv[i]);
			continue;
		}
		if (strcmp(columnName[i], "descr_storm")==0)
		{
			STRDUP(rdata->descr_storm, argv[i]);
			continue;
		}
		if (strcmp(columnName[i], "sector")==0)
		{
			rdata->sector = get_index(argv[i], str_sector_type);
			continue;
		}
		if (strcmp(columnName[i], "tread")==0)
		{
			rdata->tread = atoi(argv[i]);
			continue;
		}
		if (strcmp(columnName[i], "clan")==0)
		{
			rdata->clan = atoi(argv[i]);
			continue;
		}
		if (strcmp(columnName[i], "creators_note")==0)
		{
			STRDUP(rdata->creators_note, argv[i]);
			continue;
		}
		exitlog("/column not used/%s", columnName[i]);
	}
	rdata->extra = new ExtraDescr("room", rdata->virtual_id);
	SQLITE_EXEC_PRINTF(db, "SELECT flag FROM room_flags WHERE room_id = %u",
				callback_room_flags,
				rdata,
				dbErrMsg,
				rdata->virtual_id);

	world_top++;
	REALLOC(world, Room *, world_top+1);
	world[world_top] = new Room(rdata, cid);
	roomIndex.set(rdata->virtual_id, world_top);

	return 0;
}

static void load_tinyworld_rooms(void)
{
	char *dbErrMsg = NULL;

	SQLITE_EXEC(db, "SELECT * FROM room ORDER BY room_id",
				callback_room,
				NULL,
				dbErrMsg);
}

static void load_tinyworld_room_doors(void)
{
	char *dbErrMsg = NULL;
	SQLITE_EXEC(db, "SELECT * FROM room_door",
				callback_room_door,
				NULL,
				dbErrMsg);
}

Room *find_room(vid_t id)
{
	if ((id = roomIndex.get(id)) == (vid_t) -1)
	{
		return NULL;
	}
	return world[id];
}

DOOR *copy_door(DOOR *d)
{
	DOOR *door;

	MALLOC(door, DOOR, 1);
	memcpy(door, d, sizeof(DOOR));

	if (d->key)
	{
		door->key= new Words(d->key);
	}
	else
	{
		door->key = NULL;
	}
	STRDUP(door->descr, d->descr);

	return door;
}

void Room::copy_tinyworld_room(ROOM_DATA *rdata, vid_t i)
{
	memcpy(rdata, world[i]->data, sizeof(ROOM_DATA));
	STRDUP(rdata->name, world[i]->data->name);
	STRDUP(rdata->descr, world[i]->data->descr);
	STRDUP(rdata->descr_dark, world[i]->data->descr_dark);
	STRDUP(rdata->descr_storm, world[i]->data->descr_storm);
	if (world[i]->data->extra)
	{
		rdata->extra = new ExtraDescr(world[i]->data->extra);
	}
	else
	{
		rdata->extra = NULL;
	}
	for (dir_t dir = 0; dir < NO_DIR; dir++)
	{
		if (world[i]->data->door[dir])
		{
			rdata->door[dir] = copy_door(world[i]->data->door[dir]);
		}
		else
		{
			rdata->door[dir] = NULL;
		}
	}
	if (world[i]->data->coord)
	{
		MALLOC(rdata->coord, COORD, 1);
		memcpy(rdata->coord, world[i]->data->coord, sizeof(COORD));
	}
	else
	{
		rdata->coord = NULL;
	}
	STRDUP(rdata->creators_note, world[i]->data->creators_note);
}

Room *add_room(ROOM_DATA *rdata)
{
	Room *room;
	if ((room = find_room(rdata->virtual_id)))
	{
		room->setData(rdata);
	}
	else
	{
		world_top++;
		REALLOC(world, Room *, world_top+1);
		world[world_top] = new Room(rdata);
		roomIndex.set(rdata->virtual_id, world_top);
		room = world[world_top];
	}
	return room;
}

void destroy_tinyworld_room(vid_t i)
{
	char *dbErrMsg;

	BEGIN_TRANSACTION(db, dbErrMsg);
	SQLITE_EXEC_PRINTF(db, "DELETE FROM room WHERE room_id = %u",
				NULL,
				NULL,
				dbErrMsg,
				world[i]->getVID());
	SQLITE_EXEC_PRINTF(db, "DELETE FROM room_coord WHERE room_id = %u",
				NULL,
				NULL,
				dbErrMsg,
				world[i]->getVID());
	SQLITE_EXEC_PRINTF(db, "DELETE FROM room_extras WHERE room_id = %u",
				NULL,
				NULL,
				dbErrMsg,
				world[i]->getVID());
	SQLITE_EXEC_PRINTF(db, "DELETE FROM room_extras_keywords WHERE room_id = %u",
				NULL,
				NULL,
				dbErrMsg,
				world[i]->getVID());
	SQLITE_EXEC_PRINTF(db, "DELETE FROM room_flags WHERE room_id = %u",
				NULL,
				NULL,
				dbErrMsg,
				world[i]->getVID());
	SQLITE_EXEC_PRINTF(db, "DELETE FROM room_door WHERE room_id = %u",
				NULL,
				NULL,
				dbErrMsg,
				world[i]->getVID());
	SQLITE_EXEC_PRINTF(db, "DELETE FROM room_door_keywords WHERE room_id = %u",
				NULL,
				NULL,
				dbErrMsg,
				world[i]->getVID());
	SQLITE_EXEC_PRINTF(db, "DELETE FROM room_door_flags WHERE room_id = %u",
				NULL,
				NULL,
				dbErrMsg,
				world[i]->getVID());
	SQLITE_EXEC_PRINTF(db, "DELETE FROM room_coord WHERE room_id = %u",
				NULL,
				NULL,
				dbErrMsg,
				world[i]->getVID());
	COMMIT_TRANSACTION(db, dbErrMsg);
	delete world[i];
	world[i] = NULL;
}

void free_tinyworld_room_door(DOOR *door)
{
	delete door->key;
	free(door->proper);
	free(door->descr);
}

void free_tinyworld_room(ROOM_DATA *rdata)
{
	free(rdata->name);
	free(rdata->descr);
	free(rdata->descr_dark);
	free(rdata->descr_storm);
	if (rdata->extra)
	{
		delete rdata->extra;
	}
	for (dir_t dir = 0; dir < NO_DIR; dir++)
	{
		free(rdata->door[dir]);
	}
	if (rdata->coord)
	{
		free(rdata->coord);
	}
	free(rdata->creators_note);
	free(rdata);
}

/** Initialize the tinyworld room module. */
void init_tinyworld_room_module(void)
{
	char *dbErrMsg = NULL;

	create_tinyworld_rooms();
	create_tinyworld_room_doors();
	BEGIN_TRANSACTION(db, dbErrMsg);
	save_tinyworld_rooms();
	COMMIT_TRANSACTION(db, dbErrMsg);
	free_tinyworld_rooms();

	load_tinyworld_rooms();
	load_tinyworld_room_doors();
	assert(world_top > -1);
}

/** Finalize the tinyworld room module. */
void finit_tinyworld_room_module(void)
{
	free_tinyworld_rooms();
}
