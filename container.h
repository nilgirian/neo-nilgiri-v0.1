/* **************************************************************************
    Nilgiri the Forgotten World
    (C) 2004
   ************************************************************************** */
/** \file
    \brief Container object header
*/

#ifndef _x_CONTAINER_INCLUDE_x_
#define _x_CONTAINER_INCLUDE_x_

#include"utils.h"
#include"constants.h"

#define CID_FILE "cid.rc"

class Descriptor;
class Character;
class Player;
class Room;
class Item;

#define CID_ZONE_BOT 1
#define NUM_START_ZONES 2
#define CID_ZONE_TOP (CID_ZONE_BOT + NUM_START_ZONES - 1)

#define CID_ROOM_BOT CID_ZONE_TOP + 1
#define NUM_START_ROOMS 8
#define CID_ROOM_TOP (CID_ROOM_BOT + NUM_START_ROOMS - 1)

/** Since zones and rooms are the first containers to be created those
 	 containers need to remain constant.  This constant reserves that space.
 */
#define CID_START (CID_ROOM_TOP + 1)

/** The maximum output length from a Character. */
#define CHUNK_SIZE_OUT 1024

/** The height type should be in centimeters. */
typedef ubyte2 height_t;

/** The weight type should be in kilograms. */
typedef ubyte2 weight_t;

/** The volume type should be in decimters cubed. */
typedef ubyte2 volume_t;

/** A count of the items. */
typedef ubyte2 contents_t;

/** The container id type. CID cannot be 0. */
typedef ubyte4 cid_t;

/** A managed list of container ID's.
 */
typedef IntList<cid_t> CidList;

/** The container type type. */
typedef ubyte1 container_type_t;
/** For the different container types. */
enum CONTAINER_TYPES
{
	CONTAINER_ITEM,
	CONTAINER_CHARACTER,
	CONTAINER_ROOM,
	CONTAINER_ZONE,
	NO_CONTAINER
};

/** \brief the base atomic object in tinyworld */
/** A Container is the basic atomic object with a single defining property that
    it can contain other containers.  Your basic container is an item such as a
    sword.  A sword would not normally be a container that contains other
    containers, however a bag would be a such a container.  A character also
    could be thought of a container that contains items such as swords and
    bags.  Conversly, since a bag is a container and a character is also a
    container, a bag can contain a character.  Ultimately, a room is also a
    container that can contain several characters or items. Containers have a
	 name and gender.
 */
class Container
{
public:

	Container(container_type_t, cid_t);
	virtual ~Container(void);
	inline cid_t getCID(void);

	inline Character *objCharacter(void);
	inline Item *objItem(void);
	inline Room *objRoom(void);

	virtual int isCalled(const char *)=0;
	virtual const char *getName(void)=0;
	virtual const char *getDescr(void)=0;
	virtual const char *getDescrRoom(void)=0;
	virtual gender_t getGender(void)=0;
	inline int isMale();
	inline int isFemale();

	virtual Descriptor *getDescriptor(void) { return NULL; }
	virtual int hasNoListener(void) { return 0; }
	virtual void echo(const char *, ...);
	virtual void echo(Container *, const char *, ...);
	virtual void echo(Container *, Container *, const char *, ...);
	inline virtual void echoTo(const char *, ...){}
	inline virtual void echoTo(Container *, const char *, ...){}
	inline virtual void echoTo(Container *, Container *, const char *, ...){}
	inline virtual void put(const char *){}
	inline virtual void print(const char *, ...){}
	inline virtual void m_put(char *){}
	void listItemsTo(Container *vict=NULL);
	void listCharactersTo(Container *vict);
	void printStats(Container *vict);
	virtual int isInvisTo(Container *) { return 0; }

	void hold(Container *);
	void drop(Container *);
	void to(Container *);
	void extract(void);
	void empty(void);
	int contains(Container *);
	int canHold(Container *);

	Container *findContent(const char *name, Container *searcher=NULL);
	Character *findCharacter(const char *name, Container *searcher=NULL);
	Item *findItem(const char *name, Container *searcher=NULL);

	inline weight_t getCarryWeight(void);
	inline volume_t getCarryVolume(void);
	inline contents_t getCarryContents(void);
	virtual inline weight_t maxCarryWeight(void) { return (weight_t) -1; }
	virtual inline volume_t maxCarryVolume(void) { return (volume_t) -1; }
	inline weight_t getWeight(void);
	inline volume_t getVolume(void);
	inline void setWeight(weight_t);
	inline void setVolume(volume_t);

	inline Container *getHeadContent(void);
	inline Container *getNextContent(void);
	inline Container *inContainer(void);

private:

	container_type_t type;
	cid_t cid;
	contents_t carry_contents;
	volume_t carry_volume;
	weight_t carry_weight;
	weight_t weight;
	volume_t volume;

	Container *container_was_in; // previous mother
	Container *container_in; // mother

	Container *head_content; // daughter
	Container *tail_content; // daughter

	Container *next_content; // sister
	Container *prev_content; // sister
};

/** Checks the contents for a specific container.
    \param c The content to match for.
    \return 1 - success, 0 - failure
 */
inline int Container::contains(Container *c)
{
	assert(c);
	return c->container_in && c->container_in == this;
}

inline Container *Container::getNextContent(void)
{
	return next_content;
}

inline Container *Container::getHeadContent(void)
{
	return head_content;
}

inline Container *Container::inContainer(void)
{
	return container_in;
}
/** \return The container id. */
inline cid_t Container::getCID(void)
{
	return cid;
}

/** \return The current weight of all the contents in the container. */
inline weight_t Container::getCarryWeight()
{
	return carry_weight;
}

/** \return The current volume of all the contents in the container. */
inline volume_t Container::getCarryVolume()
{
	return carry_volume;
}

/** \return The current number of contents in the container. */
inline contents_t Container::getCarryContents()
{
	return carry_contents;
}

inline int Container::isMale(void)
{
	return getGender() == MALE;
}

inline int Container::isFemale(void)
{
	return getGender() == FEMALE;
}

inline Character *Container::objCharacter(void)
{
	if (type == CONTAINER_CHARACTER)
	{
		return (Character *) this;
	}
	return NULL;
}

inline Item *Container::objItem(void)
{
	if (type == CONTAINER_ITEM)
	{
		return (Item *) this;
	}
	return NULL;
}

inline Room *Container::objRoom(void)
{
	if (type == CONTAINER_ROOM)
	{
		return (Room *) this;
	}
	return NULL;
}

inline weight_t Container::getWeight(void)
{
	return weight;
}

inline void Container::setWeight(weight_t w)
{
	weight = w;
}

inline volume_t Container::getVolume(void)
{
	return volume;
}

inline void Container::setVolume(volume_t v)
{
	volume = v;
}

/** Return the ANSI color code associated with a character's gender.
    \param *ch The character being analyzed
 */
inline const char *c_sex(Container *c)
{
	if (c->isMale())
	{
		return K_CYN;
	}
	if (c->isFemale())
	{
		return K_MAG;
	}
	return K_YEL;
}

inline const char *c_bld_sex(Container *c)
{
	if (c->isMale())
	{
		return K_BLD_CYN;
	}
	if (c->isFemale())
	{
		return K_BLD_MAG;
	}
	return K_BLD_YEL;
}

inline Item *searchItem(const char *name, Container *c)
{
	Item *item;

	if ((item = c->findItem(name, c))==NULL)
	{
		item = c->inContainer()->findItem(name, c);
	}
	return item;
}

inline Container *searchContent(const char *name, Container *c)
{
	Container *container;

	if ((container = c->findContent(name, c))==NULL)
	{
		container = c->inContainer()->findContent(name, c);
	}
	return container;
}

extern char *comm(const char *, Container *, Container *);
extern void init_container_module(void);
#endif
