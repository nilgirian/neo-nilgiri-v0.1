/* **************************************************************************
    Nilgiri the Forgotten World
    (C) 2004
   ************************************************************************** */
/** \file
    \brief Zone object header
 */

#ifndef _x_TINYWORLD_ZONE_HEADER_x_
#define _x_TINYWORLD_ZONE_HEADER_x_

#include<stdlib.h>
#include"container.h"
#include"constants.h"
#include"utils.h"

#define MAX_ROOM_VID 100000
#define MAX_LIFESPAN 1440

typedef struct _zone_special_data
{
	zone_flags_t flags;
	lifespan_t lifespan;
}
ZONE_SPECIAL_DATA;

/** \brief The zone structure. */
/** The zone container holds a list of rooms.  The zone can affect rooms
    through forces such as weather.
 */
typedef struct _zone_data
{
	char *name;
	char *descr;

	zone_types_t zone_type;
	homeland_t homeland;

	level_t min_lvl;
	level_t max_lvl;
	vid_t top;
	
	ZONE_SPECIAL_DATA special;

	COORD *coord;

	char *creators_note;
	vid_t virtual_id;
}
ZONE_DATA;

class Zone : public Container
{
public:
	Zone(ZONE_DATA *, cid_t container_id = 0);
	~Zone(void);
	inline const char *getName(void);
	inline const char *getDescr(void);
	inline const char *getDescrRoom(void);
	inline int isCalled(const char *);
	inline gender_t getGender(void);
	void save(void);
	void setData(ZONE_DATA *zdata);
	static inline const ZONE_DATA *get_zone_data(vid_t);
	inline vid_t getVID(void);
	static void copy_tinyworld_zone_data(ZONE_DATA *, vid_t);
	inline vid_t getTop(void);
	inline vid_t getBot(void);
private:
	ZONE_DATA *data;
	ZONE_SPECIAL_DATA special;
	ubyte1 players;
	ubyte1 age;
};

inline vid_t Zone::getTop(void)
{
	return data->top;
}

inline vid_t Zone::getVID(void)
{
	return data->virtual_id;
}

inline void Zone::setData(ZONE_DATA *zdata)
{
	data = zdata;
}

inline const char *Zone::getName(void)
{
	return data->name;
}

inline const char *Zone::getDescr(void)
{
	return data->descr;
}

inline const char *Zone::getDescrRoom(void)
{
	abort();
	return "NULL";
}

inline gender_t Zone::getGender(void)
{
	return HERMAPHRODITE;
}

inline int Zone::isCalled(const char *name)
{
	abort();
	return 0;
}

extern Zone *find_zone(vid_t);
inline Zone *get_zone(vid_t vid)
{
	Zone *zone;
	assert((zone = find_zone(vid)));
	return zone;
}

inline const ZONE_DATA *Zone::get_zone_data(vid_t vid)
{
	Zone *zone = get_zone(vid);
	return zone->data;
}

extern void init_tinyworld_zone_module(void);
extern void finit_tinyworld_zone_module(void);
extern void free_tinyworld_zone(ZONE_DATA *);
extern Zone *add_zone(ZONE_DATA *);
extern void destroy_tinyworld_zone(vid_t);
extern Zone *get_next_zone(vid_t);
extern Zone *get_prev_zone(vid_t);
extern Zone *get_room_zone(vid_t);

/** Searches for the previous zone from the current zone's virtual id  and
     returns the top room + 1 as the bottom of the current zone.
	 \return The absolute bottom room virtual id this zone can have
 */
inline vid_t get_zone_bot(vid_t vid)
{
	Zone *prev_z;
	if ((prev_z = get_prev_zone(vid)))
	{
		return prev_z->getTop() + 1;
	}
	else
	{
		return 1;
	}
}

/** Searches for the previous zone from the current zone's virtual id  and
     returns the top room + 1 as the bottom of the current zone.
	 \return The absolute bottom room virtual id this zone can have
 */
inline vid_t Zone::getBot(void)
{
	return get_zone_bot(data->virtual_id);
}

extern VidIndex zoneIndex;
#endif
