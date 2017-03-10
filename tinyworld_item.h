/* **************************************************************************
    Nilgiri the Forgotten World
    (C) 2004
    tinyworld_item.h: object data
   ************************************************************************** */
/** \file
    \brief Item object header
 */
#ifndef _x_TINYWORLD_ITEM_INCLUDE_x_
#define _x_TINYWORLD_ITEM_INCLUDE_x_

#include"container.h"
#include"constants.h"
#include"utils.h"

/// The maximum set of values that a particular item type may have
#define MAX_VALUES 4

#define ITEM_CORPSE_VID 1
#define ITEM_NOTE_VID 2

class ItemNote;
class ItemContainer;

/** The raw specials data with the creation information. */
typedef struct _item_special_data
{
	sbyte4 value[MAX_VALUES];
	affect_flags_t affect_flags;
	resistance_flags_t resistance_flags;
	immunity_flags_t immunity_flags;
}
ITEM_SPECIAL_DATA;

/** \brief For item type information. */
/** The item type can be thought of as the simplest description of an item. For
    example: <I>a light but large wooden sword that a character can wield and
    hold</I> are componants that this structure can define.
 */
typedef struct _item_stats_data
{
	item_type_t item_type;
	item_attribute_flags_t attribute_flags;
	item_property_flags_t property_flags;
	material_flags_t material_flags;
	wear_flags_t wear_flags;
	weight_t weight;
	volume_t volume;
	ITEM_SPECIAL_DATA special;

	char *creators_note;
	vid_t virtual_id;
}
ITEM_STATS_DATA;

/** \brief For item description information. */
/** The item description describes in english various aspects of a particular
    <B>ITEM_STATS_DATA</B>.
 */
typedef struct _item_descr_data
{
	Words *key;
	char *proper;
	char *room;
	char *general;
	ExtraDescr *extra;

	char *creators_note;
	vid_t virtual_id;
}
ITEM_DESCR_DATA;

/** \brief Basic item information. */
/** This structure defines a base item.  Note the many-to-many relationship
    that can exist between <B>ITEM_STATS_DATA</B> and <B>ITEM_DESCR_DATA</B>
    description.  However, no two <B>ITEM_DATA</B> structures should ever
    point to the same <B>ITEM_STATS_DATA</B> and <B>ITEM_STATS_DATA</B>
    structures pair ensuring a unique piece of <B>ITEM_DATA</B>.  
    Item containers derive their information from this structure.  Multiple
	 containers can be derived from the same <B>ITEM_DATA</B> in which case
	 count should be incremented.  Should the container cease to exist count
	 should be decremented.
 */
typedef struct _item_data
{
	vid_t virtual_id;
	const ITEM_DESCR_DATA *descr;
	const ITEM_STATS_DATA *stats;
	char *creators_note;
	clan_t clan;
	limit_t limit;

	limit_t count;
}
ITEM_DATA;

inline item_type_t find_item_type(const ITEM_DATA *idata)
{
	assert(idata);
	assert(idata->stats);
	return idata->stats->item_type;
}

/** \brief The Item container class. */
/** The item container holds information about a particular item derived from
    <B>ITEM_DATA</B> however keeps a seperate record of information for which
    tinyworld may have modified in some way, as well as special information
    that may change interacting within the tinyworld.  Creation/deletion of
    item containers needs to be followed by an increment/decrement of the
    count in <B>ITEM_DATA</B>.
 */
class Item : public Container
{
public:
	Item(ITEM_DATA *idata, cid_t loader_id=0, cid_t container_id=0);
	virtual ~Item(void);
	inline item_type_t getType(void);
	inline const char *getName(void);
	inline const char *getDescr(void);
	inline const char *getDescrRoom(void);
	inline int isCalled(const char *);
	inline gender_t getGender(void);
	inline vid_t getVID(void);

protected:
	const ITEM_DATA *item_data;
	ITEM_SPECIAL_DATA special;

private:
	Item *prev_item;
	Item *next_item;
	cid_t loaderId;
};

inline vid_t Item::getVID(void)
{
	return item_data->virtual_id;
}

inline item_type_t Item::getType(void)
{
	return find_item_type(item_data);
}

inline const char *Item::getName(void)
{
	return item_data->descr->proper;
}

inline const char *Item::getDescr(void)
{
	return item_data->descr->general;
}

inline const char *Item::getDescrRoom(void)
{
	return item_data->descr->room;
}

inline int Item::isCalled(const char *keyword)
{
	return item_data->descr->key->lookup(keyword) != NULL;
}

inline gender_t Item::getGender(void)
{
	return HERMAPHRODITE;
}

Item *create_item(vid_t, cid_t, Container *);

inline Item *create_item(vid_t vid, Container *c)
{
	return create_item(vid, c->getCID(), c);
}

extern VidIndex idataIndex;
extern VidIndex idescrIndex;
extern VidIndex istatsIndex;

extern void init_tinyworld_item_module(void);
extern void finit_tinyworld_item_module(void);
extern const ITEM_DATA *find_item_data(vid_t);
extern const ITEM_DESCR_DATA *find_item_descr(vid_t);
extern const ITEM_STATS_DATA *find_item_stats(vid_t);
extern void free_tinyworld_item(ITEM_DATA *);
extern void free_tinyworld_item_description(ITEM_DESCR_DATA *);
extern void free_tinyworld_item_stat(ITEM_STATS_DATA *);
extern void add_item_data(ITEM_DATA *);
extern void add_item_descr(ITEM_DESCR_DATA *);
extern void add_item_stats(ITEM_STATS_DATA *);
extern void save_tinyworld_item(ITEM_DATA *);
extern void save_tinyworld_item_stat(ITEM_STATS_DATA *);
extern void save_tinyworld_item_description(ITEM_DESCR_DATA *);
extern void destroy_tinyworld_item(vid_t);
extern void destroy_tinyworld_item_stat(vid_t);
extern void destroy_tinyworld_item_description(vid_t);
extern void copy_tinyworld_item(ITEM_DATA *, vid_t);
extern void copy_tinyworld_item_stat(ITEM_STATS_DATA *, vid_t);
extern void copy_tinyworld_item_description(ITEM_DESCR_DATA *, vid_t);

inline const ITEM_DATA *get_item_data(vid_t vid)
{
	const ITEM_DATA *idata;
	assert((idata = find_item_data(vid)));
	return idata;
}

inline const ITEM_DATA *get_item_data(const char *vid)
{
	return get_item_data(atoi(vid));
}

#endif
