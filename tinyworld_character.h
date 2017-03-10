/* **************************************************************************
    Nilgiri the Forgotten World
    (C) 2004
   ************************************************************************** */
/** \file
    \brief Character object header
 */
#ifndef _x_TINYWORLD_CHARACTER_INCLUDE_x_
#define _x_TINYWORLD_CHARACTER_INCLUDE_x_

class Player;
class Room;
class Descriptor;
extern Room **world;

#include<time.h>
#include<errno.h>
#include<crypt.h>
#include"container.h"
#include"constants.h"
#include"utils.h"
extern VidIndex roomIndex;
#include"email_conf.h"
#include"string.h"
#include"time0.h"
#include"tinyworld_zone.h"
#include"tinyworld_room.h"
#include"command.h"
#include"tinyworld_item.h"
#include"tinyworld_time.h"
#include"interpreter.h"

/** The minimum size a player's character name can be. */
#define MIN_NAME_LEN 3

/** The maximum size for a player password. */
#define MAX_PASSWORD_LEN 11

/** The maximum size for a player's encrypted password. */
#define MAX_ENCRYPTED_LEN 15

/** The minimum size for a player password. */
#define MIN_PASSWORD_LEN 3

/** The minimum size for a player email. */
#define MIN_EMAIL_LEN 6

/** The maximum attacks a player character can have.  */
#define MAX_PC_ATTACKS 3

/** The maximum level that players can obtain. */
#define MAX_PLAYER_LEVEL 34

typedef struct _attack
{
	attack_t type;
	DMM damage;
}
ATTACK;

/** \brief For character special information. */
/** The character specials should identify the special qualities that a
    particular <B>CHAR_STATS_DATA</B> values can fluctuate.  This is in a
	 seperate structure within <B>CHAR_STATS_DATA</B> in order that an original
	 record is kept.
 */
typedef struct _char_specials_data
{
	citizenship_flags_t citizenship;
	affect_flags_t affect_flags;
	resistance_flags_t resistance_flags;
	immunity_flags_t immunity_flags;
	stat_t stat[NO_STAT];
	position_t position;
	save_t save_vs[NO_SAVES];
}
CHAR_SPECIALS_DATA;

/** \brief For character type information */
/** The character type can be thought of as the simplest description of a
    character.  For example: <I>a fire-breathing slithering female dragon that
    has mage abilities, is average weight and medium height.</I>  These are
    all componants that this structure can define.<P>
    The basic player race/gender/profession can be defined here.  These should
    be composed and seed the tinyworld.character_type files.
 */
typedef struct _char_stats_data
{
	race_t race;
	gender_t gender;
	homeland_t homeland;
	profession_flags_t profession;
	thac0_t thac0;
	movement_types_t movement_type;
	top_t attack_top;
	ATTACK *attack;
	act_flags_t act_flags;
	height_types_t height_class;
	weight_types_t weight_class;
	volume_types_t volume_class;
	DMM hitpoints;

	CHAR_SPECIALS_DATA special;

	char *creators_note;
	vid_t virtual_id;
}
CHAR_STATS_DATA;

/** \brief For character description informaiton. */
/** The character description describes in english various aspects of a
    particular <B>CHAR_STATS_DATA</B>
 */
typedef struct _char_descr_data
{
	Words *key;
	char *proper;
	char *room;
	char *general;

	vid_t virtual_id;
	char *creators_note;
}
CHAR_DESCR_DATA;

/** \brief For character data */
/** This structure defines a base monster character.  Note the many-to-many
    relationship that can exist between <B>CHAR_STATS_DATA</B> and
    <B>CHAR_DESCR_DATA</B> description.  Character containers derive their
    information from this structure.  Multiple containers can be derived from
    the same <B>CHAR_DATA</B> in which case count should be incremented.
    Should the containers cease to exist count should be decremented.
 */
typedef struct _char_data
{
	vid_t virtual_id;
	CHAR_DESCR_DATA *descr;
	CHAR_STATS_DATA *stats;
	char *creators_note;
	clan_t clan;
	limit_t limit;

	limit_t count;
}
CHAR_DATA;

typedef struct _character_ctx
{
	Character *character;
}
CHARACTER_CTX;

struct _fight;

class ImmortalHelper
{
public:
	ImmortalHelper(immortal_type_t);
	~ImmortalHelper(void);
	inline void newHelpEntry(const char *);
	inline void delHelpEntry(void);
	inline immortal_type_t getLevel(void);
	inline int cannotDo(cmd_t);
	inline int canDo(cmd_t);
	inline cmd_flags_t cmdFlags(void);
	inline void setCmdFlag(cmd_t);
	inline void resetCmdFlag(cmd_t);

	// we'll trust create commands to have full reign of these temp vars
	HELP_ENTRY *cr_help;
	HELP_ENTRY *cr_wizhelp;
	SOCIAL_DATA *cr_social;

	ITEM_STATS_DATA *cr_istats;
	ITEM_DESCR_DATA *cr_idescr;
	ITEM_DATA *cr_idata;

	CHAR_STATS_DATA *cr_cstats;
	CHAR_DESCR_DATA *cr_cdescr;
	CHAR_DATA *cr_cdata;

	ZONE_DATA *cr_zone;
	ROOM_DATA *cr_room;

private:
	immortal_flags_t imm_flags;
	cmd_flags_t command_flags;
	immortal_type_t level;
	level_t imm_message_lvl;
};

inline cmd_flags_t ImmortalHelper::cmdFlags(void)
{
	return command_flags;
}

inline void ImmortalHelper::setCmdFlag(cmd_t cmd)
{
	SET(f_(cmd), command_flags);
}

inline void ImmortalHelper::resetCmdFlag(cmd_t cmd)
{
	RESET(f_(cmd), command_flags);
}

inline immortal_type_t ImmortalHelper::getLevel(void)
{
	return level;
}

inline int ImmortalHelper::cannotDo(cmd_t t)
{
	return canDo(t)==0;
}

inline int ImmortalHelper::canDo(cmd_t t)
{
	return IS_SET(command_flags, f_(t));
}

/** \brief The Character container class. */
/** The character container holds information about a particular character
    derived from <B>CHAR_DATA</B> however keeps seperate record of information
    for which tinyworld may have modified in some way, as well as special
    information that may change interacting within in the tinyworld.
    Creation/deletion of character containers need to be followed by an
    increment/decrement of the count in <B>CHAR_DATA</B>.
 */
class Character : public Container
{
public:
	Character(Descriptor *d, cid_t c);
	Character(CHAR_DATA *);
	virtual ~Character(void);

	vid_t getVID(void);

	virtual inline const char *getName(void);
	virtual inline const char *getDescr(void);
	virtual inline const char *getDescrRoom(void);
	inline void setName(const char *);
	virtual int isCalled(const char *);
	inline const char *getTitle(void);

	virtual inline gender_t getGender(void);

	inline homeland_t getHomeland(void);
	inline race_t getRace(void);

	inline void setHomeland(homeland_t);

	inline profession_flags_t getProfession(void);

	ImmortalHelper *Imm;

	virtual inline level_t getLevel(void);

	inline stat_t getStat(stat_t);
	inline stat_t getNaturalStat(stat_t);
	inline void setStat(stat_t, stat_t);

	inline height_t getHeight(void);
	inline void setHeight(height_t);

	inline time_t getBirth(void);
	inline void setBirth(time_t);

	inline vid_t getCharDataVid(void);

	inline void to(vid_t);
	void moveTo(Room *, cmd_t );
	inline void moveTo(vid_t, cmd_t);
	inline void moveTo(vid_t);
	inline void moveTo(Room *);
	Player *findPlayer(const char *name);

	void look(void);
	void look(Container *);

	inline void setDescriptor(Descriptor *);
	inline Descriptor *getDescriptor(void);
	virtual int hasNoListener(void);
	void unlink(void);
	inline int isUnlinked(void);

	virtual void echo(Container *, Container *, const char *, ...);
	virtual void echo(Container *, const char *, ...);
	virtual void echo(const char *, ...);
	virtual void echoTo(Container *, Container *, const char *, ...);
	virtual void echoTo(Container *, const char *, ...);
	virtual void echoTo(const char *, ...);
	virtual void print(const char *, ...);
	virtual void put(const char *);
	virtual void m_put(char *);
	void writer(char **);
	void writer(char **, void (*fn)(void *), void *);
	void cr1LWriter(char **);

	virtual void pointUpdate(void);
	void prompt(void);
	inline int getPosition(void);

	inline profession_t getPrimaryProfession(void);
	inline int isWarrior(void);
	inline int isThief(void);
	inline int isMonk(void);
	inline int isMage(void);
	inline int isCleric(void);
	inline int isDruid(void);

	void extract(void);
	void insert(vid_t);

	inline hitpoints_t getHitpoints();
	inline hitpoints_t addHitpoints(hitpoints_t);
	inline hitpoints_t getMaxHitpoints();
	inline hitpoints_t addMaxHitpoints(hitpoints_t);

	inline Character *getNextCharacter(void);
	inline CHARACTER_CTX *getCombatCTX(void);

	void setFighting(Character *);
	Character *fighting(void);
	inline int isDead(void);

	virtual inline const char *getMovement(void);

	virtual inline Player *objPlayer(void);
protected:

	CHAR_SPECIALS_DATA special;
	const CHAR_DATA *data;
	time_t birth;
	height_t height;
	hitpoints_t hitpoints;
	hitpoints_t hitpoints_max;

private:

	Descriptor *descriptor;
	CHARACTER_CTX *pointUpdateCTX;
	CHARACTER_CTX *combatCTX;
	static ubyte4 created;
	static ubyte4 destroyed;

	struct _fight *fight;

	Character *next_character;
	Character *prev_character;
};

inline Player *Character::objPlayer(void)
{
	return NULL;
}

inline vid_t Character::getVID(void)
{
	return data->virtual_id;
}

inline const char *Character::getMovement(void)
{
	return str_movement[data->stats->movement_type];
}

inline int Character::hasNoListener(void)
{
	return descriptor == NULL;
}

inline const char *Character::getTitle(void)
{
	return "the Immortal";
}

inline int Character::isDead(void)
{
	return 0 == hitpoints;
}

inline CHARACTER_CTX *Character::getCombatCTX(void)
{
	return combatCTX;
}

inline hitpoints_t Character::getHitpoints(void)
{
	return hitpoints;
}

inline hitpoints_t Character::addHitpoints(hitpoints_t h)
{
	return hitpoints = min(max(hitpoints+h, 0), hitpoints_max);
}

inline hitpoints_t Character::getMaxHitpoints(void)
{
	return hitpoints_max;
}

inline hitpoints_t Character::addMaxHitpoints(hitpoints_t h)
{
	return hitpoints_max+h;
}
	
inline level_t Character::getLevel(void)
{
	return 11;
}

inline int Character::isUnlinked(void)
{
	return descriptor == NULL;
}

inline Character *Character::getNextCharacter(void)
{
	return next_character;
}
inline const char *Character::getName(void)
{
	return data->descr->proper;
}

inline const char *Character::getDescr(void)
{
	return data->descr->general;
}

inline const char *Character::getDescrRoom(void)
{
	return data->descr->room;
}

/** Checks to see if the character can be called by a particular name.
    \param keyword the name to check against the character
 */
inline int Character::isCalled(const char *keyword)
{
	return data->descr->key->lookup(keyword) != NULL;
}


inline void Character::setName(const char *name)
{
	assert(name && strlen(name) < MAX_SHORT_DESCR_SIZE);
}

inline race_t Character::getRace(void)
{
	return data->stats->race;
}

inline gender_t Character::getGender(void)
{
	return data->stats->gender;
}

inline homeland_t Character::getHomeland(void)
{
	return data->stats->homeland;
}

inline profession_flags_t Character::getProfession(void)
{
	return data->stats->profession;
}

inline stat_t Character::getStat(stat_t s)
{
	return special.stat[s];
}

inline stat_t Character::getNaturalStat(stat_t s)
{
	return data->stats->special.stat[s];
}

inline void Character::setStat(stat_t s, stat_t v)
{
	assert(s < NO_STAT);
	special.stat[s] = v;
}

inline height_t Character::getHeight(void)
{
	return height;
}

inline void Character::setHeight(height_t h)
{
	height = h;
}

inline time_t Character::getBirth(void)
{
	return birth;
}

inline void Character::setBirth(time_t t)
{
	birth = t;
}

inline vid_t Character::getCharDataVid(void)
{
	return data->virtual_id;
}

inline void Character::setDescriptor(Descriptor *d)
{
	descriptor = d;
}

inline Descriptor *Character::getDescriptor(void)
{
	return descriptor;
}

inline void Character::to(vid_t vid)
{
	Container::to((Container *)world[roomIndex.get(vid)]);
}

inline void Character::moveTo(vid_t vid)
{
	moveTo(world[roomIndex.get(vid)], NO_COMMAND);
}

inline void Character::moveTo(vid_t vid, cmd_t cmd)
{
	moveTo(world[roomIndex.get(vid)], cmd);
}

inline void Character::moveTo(Room *rm)
{
	moveTo(rm, NO_COMMAND);
}

inline int Character::getPosition(void)
{
	return special.position;
}

inline int Character::isWarrior(void)
{
	 return getProfession() & f_(WARRIOR);
}

inline int Character::isThief(void)
{
	return getProfession() & f_(THIEF);
}

inline int Character::isMonk(void)
{
	return getProfession() & f_(MONK);
}

inline int Character::isMage(void)
{
	return getProfession() & f_(MAGE);
}

inline int Character::isCleric(void)
{
	return getProfession() & f_(CLERIC);
}

inline int Character::isDruid(void)
{
	return getProfession() & f_(DRUID);
}

inline profession_t Character::getPrimaryProfession(void)
{
	if (isWarrior())
	{
		return WARRIOR;
	}
	if (isThief())
	{
		return THIEF;
	}
	if (isMonk())
	{
		return MONK;
	}
	if (isMage())
	{
		return MAGE;
	}
	if (isCleric())
	{
		return CLERIC;
	}
	if (isDruid())
	{
		return DRUID;
	}
	return NO_PROFESSION;
}

extern VidIndex cdataIndex;
extern VidIndex cdescrIndex;
extern VidIndex cstatsIndex;

Character *create_character(vid_t, cid_t loader, Container *);
extern void init_tinyworld_character_module(void);
extern Character *character_list;
extern void comm(Container *, Container *, Container *, const char *, int);
extern CHAR_STATS_DATA *find_char_stats(vid_t);
extern CHAR_DESCR_DATA *find_char_descr(vid_t);
extern void free_tinyworld_character(CHAR_DATA *);
extern void free_tinyworld_character_description(CHAR_DESCR_DATA *);
extern void free_tinyworld_character_stat(CHAR_STATS_DATA *);
extern void add_char_data(CHAR_DATA *);
extern void add_char_descr(CHAR_DESCR_DATA *);
extern void add_char_stats(CHAR_STATS_DATA *);
extern void save_tinyworld_character(CHAR_DATA *);
extern void save_tinyworld_character_stat(CHAR_STATS_DATA *);
extern void save_tinyworld_character_description(CHAR_DESCR_DATA *);
extern void destroy_tinyworld_character(vid_t);
extern void destroy_tinyworld_character_stat(vid_t);
extern void destroy_tinyworld_character_description(vid_t);
extern void copy_tinyworld_character(CHAR_DATA *, vid_t);
extern void copy_tinyworld_character_stat(CHAR_STATS_DATA *, vid_t);
extern void copy_tinyworld_character_description(CHAR_DESCR_DATA *, vid_t);

#endif
