/* **************************************************************************
    Nilgiri the Forgotten World
    (C) 2004
   ************************************************************************** */
/** \file
    \brief Room object header
 */

#ifndef _x_TINYWORLD_ROOM_HEADER_x_
#define _x_TINYWORLD_ROOM_HEADER_x_

class Character;
class Zone;

#include"container.h"
#include"constants.h"
#include"utils.h"
#include"tinyworld_item.h"
#include<assert.h>

#define MAX_ROOM_NAME_LEN 64
#define MAX_ROOM_DESCR_LEN 2048

#define RM_VOID 1
#define RM_FLOATING 2
#define RM_PURGATORY 3
#define RM_LIMBO 4
#define RM_CHAT 5
#define RM_ENEX 6
#define RM_ENSIGN 7
#define RM_SHIZUKA 8

extern VidIndex roomIndex;

/** \brief For door special information. */
/** An original record must be kept of this information because it may change
    in tinyworld.
 */
typedef struct _door_special_data
{
	door_flags_t flags;
}
DOOR_SPECIAL_DATA;

class Room;

/** \brief The room exit information. */
/** Doors indicate an exit and not necessarily the presence of an actual
    gate.
 */
typedef struct _door
{
	Words *key;
	char *proper;
	char *descr;
	DOOR_SPECIAL_DATA special;
	door_t door_type;
	ubyte1 weight;
	Room *room;
	const ITEM_DATA *room_key;
}
DOOR;

typedef ubyte1 tread_t;

/** \brief For room special information. */
/** An original record must be kept of this information because it may change
    in tinyworld.
 */
typedef struct _room_special_data
{
	room_flags_t flags;
}
ROOM_SPECIAL_DATA;

typedef struct _room_data
{
	// description
	char *name;
	char *descr;
	char *descr_dark;
	char *descr_storm;
	ExtraDescr *extra;

	// type
	sector_types_t sector;
	/** tread_factor needs to be between 0 and 99 where a higher tread_factor
	    inhibits movement more than a lower tread_factor.
	 */
	tread_t tread;

	// special
	ROOM_SPECIAL_DATA special;

	DOOR *door[NO_DIR];

	ubyte1 clan;

	COORD *coord;

	char *creators_note;
	vid_t virtual_id; 
}
ROOM_DATA;

/** \brief The room container class. */
/** The room container holds information about a room.  Seperate special
    information is instantiated for the room and doors for changes
    due to interactions within the tinyworld.
 */
class Room : public Container
{
public:
	Room(vid_t, const char *, const char *, cid_t c = 0);
	Room(ROOM_DATA *rdata, cid_t c = 0);

	~Room(void);

	inline vid_t getVID(void);
	inline void setData(ROOM_DATA *);

	inline const char *getName(void);
	inline void setName(const char *);
	inline int isCalled(const char *);

	inline gender_t getGender(void);

	inline const char *getDescr(void);
	inline void setDescr(const char *);
	inline const char *getDescrRoom(void) { return NULL; }

	inline const char *getDescrDark(void);
	inline void setDescrDark(const char *);

	inline const char *getDescrStorm(void);
	inline void setDescrStorm(const char *);

	inline sector_types_t getSector(void);
	inline void setSector(sector_types_t);

	inline tread_t getOriginalTreadFactor(void);
	inline void setOriginalTreadFactor(tread_t);

	inline coord_t getCoordX(void);
	inline coord_t getCoordY(void);
	inline vid_t getCoordV(void);

	inline clan_t getClan(void);
	inline void setClan(clan_t);

	inline const char *getCreatorsNote(void);
	inline void setCreatorsNote(const char *);

	void save(void);

	void createDir(dir_t, vid_t);
	inline void setDir(dir_t, vid_t);
	inline Room *getRoomDir(dir_t);
	inline Zone *getZone(void);

	static void copy_tinyworld_room(ROOM_DATA *, vid_t);
	static inline ROOM_DATA *get_room_data(vid_t);

private:
	ROOM_DATA *data;
	ROOM_SPECIAL_DATA special;
	DOOR_SPECIAL_DATA *door_special[NO_DIR];
	CidList *tracks_ex[NO_DIR];
	CidList *tracks_en[NO_DIR];
};

inline Zone *Room::getZone(void)
{
	extern Zone *get_room_zone(vid_t);
	return get_room_zone(data->virtual_id);
}

inline void Room::setData(ROOM_DATA *rdata)
{
	data = rdata;
}

inline Room *Room::getRoomDir(dir_t dir)
{
	if (data->door[dir] == NULL)
	{
		return NULL;
	}
	return data->door[dir]->room;
}

inline int Room::isCalled(const char *keyword)
{
	if (is_name(keyword, "room")
			|| is_name(keyword, "here")
			|| (atoi(keyword) && is_name(keyword, itoa(data->virtual_id))))
	{
		return 1;
	}
	return 0;
}

inline vid_t Room::getVID(void)
{
	return data->virtual_id;
}

inline const char *Room::getName(void)
{
	return data->name;
}

inline gender_t Room::getGender(void)
{
	return NO_GENDER;
}

inline void Room::setName(const char *arg)
{
	assert(arg);
	STRDUP(data->name, arg);
}

inline const char *Room::getDescr(void)
{
	return data->descr;
}

inline void Room::setDescr(const char *arg)
{
	assert(arg);
	STRDUP(data->descr, arg);
}

inline const char *Room::getDescrDark(void)
{
	return data->descr_dark;
}

inline void Room::setDescrDark(const char *arg)
{
	STRDUP(data->descr_dark, arg);
}

inline const char *Room::getDescrStorm(void)
{
	return data->descr_storm;
}

inline void Room::setDescrStorm(const char *arg)
{
	STRDUP(data->descr_storm, arg);
}

inline sector_types_t Room::getSector(void)
{
	return data->sector;
}

inline void Room::setSector(sector_types_t s)
{
	assert(s <= NO_SECTOR);
	data->sector = s;
}

inline tread_t Room::getOriginalTreadFactor(void)
{
	return data->tread;
}

inline void Room::setOriginalTreadFactor(tread_t t)
{
	assert(t < 100);
	data->tread = 0;
}

inline coord_t Room::getCoordX(void)
{
	if (data->coord == NULL)
	{
		return 0;
	}
	return data->coord->x;
}

inline coord_t Room::getCoordY(void)
{
	if (data->coord == NULL)
	{
		return 0;
	}
	return data->coord->y;
}

inline vid_t Room::getCoordV(void)
{
	if (data->coord == NULL)
	{
		return 0;
	}
	return data->coord->v;
}

inline clan_t Room::getClan(void)
{
	return data->clan;
}

inline void Room::setClan(clan_t c)
{
	data->clan = c;
}

inline const char *Room::getCreatorsNote(void)
{
	return data->creators_note;
}

inline void Room::setCreatorsNote(const char *arg)
{
	if (data->creators_note)
	{
		FREE(data->creators_note);
	}
	STRDUP(data->creators_note, arg);
}

extern void init_tinyworld_room_module(void);
extern void finit_tinyworld_room_module(void);
extern void free_door(DOOR *);
extern void free_tinyworld_room(ROOM_DATA *);
extern Room *find_room(vid_t);
extern Room *add_room(ROOM_DATA *);
extern void destroy_tinyworld_room(vid_t);

inline Room *find_room(const char *id)
{
	return find_room(atoi(id));
}

inline Room *get_room(vid_t vid)
{
	Room *room;
	assert((room = find_room(vid)));
	return room;
}

inline Room *get_room(const char *vid)
{
	return get_room(atoi(vid));
}

extern Room **world;
extern top_t world_top;

#endif
