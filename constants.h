/* **************************************************************************
    Nilgiri the Forgotten World
    (C) 2004
   ************************************************************************** */
/** \file
    \brief Game constants and tables header
   
    Enumerations are either <B>FLAGS</B> or <B>TYPES</B>.  Types are just
    enumerations to give unique value for different items in its class.  Flags
    are types with the additional quality that a corresponding bitvector will
    be assigned.<P>
    This means for the world builder that for every value that requires an
    assignment from the enumeration <B>TYPES</B> there must exist one and only
    one value.  For enumeration <B>FLAGS</B> you may assign none or all.<P>
    After the end of each enumeration there is a <B>NO_</B> value.  For
    enumerations <B>TYPES</B> it may make sense to use the <B>NO_</B> type as
    a value of none of the listed types,  however for <B>FLAGS</B> this must
    never be set.  Enumerations <B>TYPES</B> can number in the thousands.
    However, enumerations <B>FLAGS</B> can only have up to a max of 63 not
    including the <B>NO_</B> flag.  When considering the number of items
    each enumeration have remember to also keep in mind the type for the
    values which they are stored in.  If you are not the implementor, you will
	 probably want to skip most of the beginning of this document down to the
	 section that reads <B>Enumeration Type Document</B>.
 */
    
#ifndef _x_CONSTANTS_HEADER_x_
#define _x_CONSTANTS_HEADER_x_

#include"utils.h"

// VARIOUS ENUMERATIONS AND FLAGS FOR TINYWORLD_ITEM

/** \sa MATERIAL_FLAGS */
typedef ubyte8 material_flags_t;
/** This should be an 8-byte integer and no more than 63 of these. */
enum MATERIAL_FLAGS
{
	ADAMANTIUM, ///<-
	ALUMINUM, ///<-
	BARK, ///<-
	BONE, ///<-
	BRASS, ///<-
	BRONZE, ///<-
	CANVAS, ///<-
	CHITIN, ///<-
	CLAY, ///<-
	CLOTH, ///<-
	COPPER, ///<-
	COTTON, ///<-
	CRYSTAL, ///<-
	DEMONHIDE, ///<-
	DIAMOND, ///<-
	DRAGONSCALE, ///<-
	EMERALD, ///<-
	FLESH, ///<-
	FUR, ///<-
	GEMSTONE, ///<-
	GLASS, ///<-
	GOLD, ///<-
	GRANITE, ///<-
	HAIR, ///<-
	HARD_LEATHER, ///<-
	HIDE, ///<-
	IRON, ///<-
	LEAD, ///<-
	LINEN, ///<-
	MYTHRIL, ///<-
	PLANT, ///<-
	PLATINUM, ///<-
	QUARTZ, ///<-
	RUBY, ///<-
	SAPPHIRE, ///<-
	SILK, ///<-
	SILVER, ///<-
	SOFT_LEATHER, ///<-
	STEEL, ///<-
	STONE, ///<-
	SUEDE, ///<-
	TIN, ///<-
	TITANIUM, ///<-
	WOOD, ///<-
	NO_MATERIAL_FLAG
};

extern const char *str_material[];
extern const char *str_material_f[];

/** \sa ITEM_TYPE_TYPES */
typedef ubyte1 item_type_t;
/** This should be a 1-byte integer and no more than 254 of these. */
enum ITEM_TYPES
{
	ITEM_OTHER, ///<-
	ITEM_LIGHT, ///<-
	ITEM_SCROLL, ///<-
	ITEM_WAND, ///<-
	ITEM_STAFF, ///<-
	ITEM_WEAPON, ///<-
	ITEM_FIRE_WEAPON, ///<-
	ITEM_MISSILE, ///<-
	ITEM_TREASURE, ///<-
	ITEM_ARMOR, ///<-
	ITEM_POTION, ///<-
	ITEM_WORN, ///<-
	ITEM_TRASH, ///<-
	ITEM_TRAP, ///<-
	ITEM_CONTAINER, ///<-
	ITEM_NOTE, ///<-
	ITEM_LIQUID_CONTAINER, ///<-
	ITEM_KEY, ///<-
	ITEM_FOOD, ///<-
	ITEM_MONEY, ///<-
	ITEM_PEN, ///<-
	ITEM_BOAT, ///<-
	ITEM_HERB, ///<-
	ITEM_PLANT, ///<-
	ITEM_SPELL_OBJECT, ///<- maybe should be handled differently
	ITEM_STATUE, ///<-
	ITEM_TELEPORT, ///<-
	ITEM_LOCK_PICK, ///<-
	ITEM_SHIP, ///<-
	ITEM_WAR_SHIP, ///<-
	ITEM_CHART, ///<-
	ITEM_FURNITURE, ///<-
	NO_ITEM_TYPE
};

extern const char *str_item_type[];

/** \sa ITEM_TYPE_FLAGS */
typedef ubyte4 item_attribute_flags_t;
/** This should be a 2-byte integer and no more than 15 of these. */
enum ITEM_ATTRIBUTE_FLAGS
{
	ATTR_GLOW, ///<-
	ATTR_HUM, ///<-
	ATTR_MIRRORED, ///<-
	ATTR_INVISIBLE, ///<-
	ATTR_LIT, ///<-
	NO_ATTRIBUTE
};

extern const char *str_attribute_flags[];
extern const char *str_attribute_f[];

/** \sa ITEM_PROPERTY_FLAGS */
typedef ubyte4 item_property_flags_t;
/** This should be a 4-byte integer and no more than 31 of these. */
enum ITEM_PROPERTY_FLAGS
{
	PROP_MAGIC, ///<-
	PROP_EVIL, ///<-
	PROP_ANTI_GOOD, ///<-
	PROP_ANTI_EVIL, ///<-
	PROP_ANTI_NEUTRAL, ///<-
	PROP_NODROP, ///<-
	PROP_BLESS, ///<-
	PROP_NOLOCATE, ///<-
	PROP_DROPDESTRUCT, ///<-
	PROP_WORTHLESS, ///<-
	PROP_ANTI_RENT, ///<-
	PROP_HIDE_EQ, ///<-
	PROP_NOLOC_EQ, ///<-
	PROP_NOT_SEEN, ///<-
	PROP_SEARCHABLE, ///<-
	NO_PROPERTY_FLAG
};

extern const char *str_property_flags[];
extern const char *str_property_f[];

/** \sa WEAR_FLAGS */
typedef ubyte4 wear_flags_t;
/** This should be a 4-byte integer and no more than 31 of these. */
enum WEAR_FLAGS
{
	TAKE, ///<-
	FINGER, ///<-
	NECK, ///<-
	BODY, ///<-
	HEAD, ///<-
	LEGS, ///<-
	FEET, ///<-
	HANDS, ///<-
	ARMS, ///<-
	SHIELD, ///<-
	BUCKLER, ///<- new
	ABOUT, ///<-
	WAIST, ///<-
	WRIST, ///<-
	WIELD, ///<-
	HOLD, ///<-
	FLOATING, ///<-
	EARRING, ///<-
	FACE, ///<-
	NO_WEAR_FLAG
};

extern const char *str_wear_pos_flags[];
extern const char *str_wear_pos_f[];

/** \sa CONTAINER_FLAGS */
typedef ubyte4 container_flags_t;
/** This should be a 4-byte integer and no more than 31 of these. */
enum CONTAINER_FLAGS
{
	CONT_CLOSEABLE, ///<-
	CONT_PICKPROOF, ///<-
	CONT_CLOSED, ///<-
	CONT_LOCKED, ///<-
	CONT_WIZARD_LOCK, ///<-
	CONT_KNOCKPROOF, ///<-
	NO_CONTAINER_FLAG
};

/** \sa LIQUID_TYPES */
typedef ubyte1 liquid_types_t;
/** This should be a 1-byte integer and no more than 254 of these. */
enum LIQUID_TYPES
{
	LIQ_WATER, ///<-
	LIQ_BEER, ///<-
	LIQ_WHITE_WINE, ///<-
	LIQ_ALE, ///<-
	LIQ_DARKALE, ///<-
	LIQ_WHISKY, ///<-
	LIQ_LEMONADE, ///<-
	LIQ_FIREBRT, ///<-
	LIQ_LOCALSPC, ///<-
	LIQ_SLIME, ///<-
	LIQ_MILK, ///<-
	LIQ_TEA, ///<-
	LIQ_COFFEE, ///<-
	LIQ_BLOOD, ///<-
	LIQ_SALTWATER, ///<-
	LIQ_VODKA, ///<-
	LIQ_RED_WINE, ///<-
	LIQ_GREEN_TEA, ///<-
	NO_LIQUID
};

// VARIOUS ENUMERATIONS AND FLAGS FOR TINYWORLD_CHARACTER

/** \sa PLAYER_ATTRIBUTE_FLAGS */
typedef ubyte2 player_env_flags_t;
/** This should be a 4-byte integer and no more than 31 of these. */
enum PLAYER_ENVIRONMENT_FLAGS
{
	ENV_SNEAK, ///<-
	ENV_EVADE, ///<-
	ENV_KILL, ///<-
	ENV_TAKE, ///<-
	ENV_MERCY, ///<-
	ENV_FLEE, ///<-
	ENV_GUARD, ///<-
	ENV_RECALL, ///<-
	ENV_CONSIDER, ///<-
	NO_PLAYER_ENVIRONMENT_FLAG
};

extern const char *str_player_env[];
extern const char *str_player_env_f[];

typedef ubyte4 desc_env_flags_t;

enum DESC_ENVIRONMENT_FLAGS
{
	DENV_BRIEF, ///<-
	DENV_COMPACT, ///<-
	DENV_ABBREV, ///<-
	DENV_EXITS, ///<-
	DENV_EXITS_L, ///<-
	DENV_CBRIEF, ///<-
	DENV_ECHO, ///<-
	DENV_WEATHER, ///<-
	DENV_DIS_H, ///<-
	DENV_DIS_M, ///<-
	DENV_DIS_V, ///<-
	DENV_TELL, ///<-
	DENV_SHOUT, ///<-
	DENV_AUCTION, ///<-
	DENV_GOSSIP, ///<-
	DENV_WRAP, ///<-
	DENV_SPLITSCREEN, ///<-
	DENV_COLOR, ///<-
	NO_DESC_ENVIRONMENT_FLAG
};

extern const char *str_desc_env[];
extern const char *str_desc_env_f[];

/** \sa PLAYER_AFFECT_FLAGS */
typedef ubyte2 player_affect_flags_t;
/** This should be a 2-byte integer and no more than 15 of these. */
enum PLAYER_AFFECT_FLAGS
{
	PLR_IMMORTAL, ///<-
	PLR_NOSHOUT, ///<-
	PLR_SILENCE, ///<-
	PLR_DAMNED, ///<-
	PLR_LORD, ///<-
	PLR_DUKE, ///<-
	PLR_MYTHS, ///<-
	PLR_VAMPIRE, ///<-
	PLR_FROZEN, ///<-
	PLR_BADNAME, ///<-
	NO_PLAYER_AFFECT_FLAG
};

/** \sa IMMORTAL_FLAGS */
typedef ubyte2 immortal_flags_t;
/** This should be a 2-byte integer and no more than 15 of these. */
enum IMMORTAL_FLAGS
{
	IMM_WIZINVIS, ///<-
	IMM_IMPERCEPT, ///<-
	NO_IMMORTAL_FLAG
};

/** \sa WIZARD_TYPES */
typedef ubyte1 immortal_type_t;
/** This should be a 1-byte integer and no more than 254 of these. */
enum IMMORTAL_TYPES
{
	IMMORTAL, ///<-
	DEITY, ///<-
	GOD, ///<-
	DEUCE, ///<-
	IMPLEMENTOR, ///<-
	NO_IMMORTAL_TYPE
};

extern const char *str_immortal_types[];
extern const char *str_immortal_types_u[];
extern const char *str_immortal_types_abbrev[];

/** \sa WEAR_POSITION_TYPES */
typedef ubyte1 wear_position_types_t;
/** This should be a 1-byte integer and no more than 254 of these. */
enum WEAR_POSITION_TYPES
{
	POS_FLOAT, ///<-
	POS_FINGER_R, ///<-
	POS_FINGER_L, ///<-
	POS_NECK_1, ///<-
	POS_NECK_2, ///<-
	POS_BODY, ///<-
	POS_HEAD, ///<-
	POS_LEGS, ///<-
	POS_FEET, ///<-
	POS_HANDS, ///<-
	POS_ARMS, ///<-
	POS_SHIELD, ///<-
	POS_ABOUT, ///<-
	POS_WAIST, ///<-
	POS_WRIST_R, ///<-
	POS_WRIST_L, ///<-
	POS_WIELD, ///<-
	POS_HOLD_R, ///<-
	POS_HOLD_L, ///<-
	POS_EAR_R, ///<-
	POS_EAR_L, ///<-
	POS_FACE, ///<-
	NO_WEAR_POSITION
};

/** \sa RACE_TYPES */
typedef ubyte1 race_t;
/** This should be a 1-byte integer and no more than 254 of these. */
enum RACE_TYPES
{
	HUMAN, ///<-
	ELF, ///<-
	HIGH_ELF, ///<-
	VORI_ELF, ///<-
	DARK_ELF, ///<-
	DWARF, ///<-
	GNOME, ///<-
	PIXIE, ///<-
	MAMMAL, ///<-
	REPTILE, ///<-
	BIRD, ///<-
	INSECT, ///<-
	ELEMENTAL_FIRE, ///<-
	ELEMENTAL_WATER, ///<-
	ELEMENTAL_EARTH, ///<-
	ELEMENTAL_AIR, ///<-
	DRAGON, ///<-
	NO_RACE
};

extern const char *str_race[];
extern const char *str_race_descr[];
extern const char *str_race_header[];

/** \sa HAIR_LENGTH_TYPES */
typedef ubyte1 hair_length_t;
/** This should be a 1-byte integer and no more than 254 of these. */
enum HAIR_LENGTH_TYPES
{
	HAIR_BALD, ///<-
	HAIR_CROPPED, ///<-
	HAIR_SHORT, ///<-
	HAIR_MEDIUM, ///<-
	HAIR_LONG, ///<-
	HAIR_VERY_LONG, ///<-
	NO_HAIR_LENGTH
};

extern const char *str_hair_length[];
extern const char *str_race_hair_length[][NO_HAIR_LENGTH+1];

/** \sa HAIR_COLOR_TYPES */
typedef ubyte1 hair_color_t;
/** This should be a 1-byte integer and no more than 254 of these. */
enum HAIR_COLOR_TYPES
{
	HAIR_BLACK, ///<-
	HAIR_BROWN, ///<-
	HAIR_RED, ///<-
	HAIR_BLOND, ///<-
	HAIR_PLATINUM, ///<-
	HAIR_GRAY, ///<-
	HAIR_BLUE, ///<-
	HAIR_GREEN, ///<-
	HAIR_PURPLE, ///<-
	NO_HAIR_COLOR
};

extern const char *str_hair_color[];
extern const char *str_race_hair_color[][NO_HAIR_COLOR+1];

/** \sa HAIR_SHAPE_TYPES */
typedef ubyte1 hair_shape_t;
/** This should be a 1-byte integer and no more than 254 of these. */
enum HAIR_SHAPE_TYPES
{
	HAIR_STRAIGHT, ///<-
	HAIR_WAVY, ///<-
	HAIR_CURLY, ///<-
	HAIR_SPIKED, ///<-
	HAIR_MOHAWKED, ///<-
	HAIR_BRAIDED, ///<-
	HAIR_DREADLOCKED, ///<-
	NO_HAIR_SHAPE
};

extern const char *str_hair_shape[];
extern const char *str_race_hair_shape[][NO_HAIR_SHAPE+1];

/** \sa EYE_COLOR_TYPES */
typedef ubyte1 eye_color_t;
/** This should be a 1-byte integer and no more than 254 of these. */
enum EYE_COLOR_TYPES
{
	EYES_BLACK, ///<-
	EYES_BROWN, ///<-
	EYES_BLUE, ///<-
	EYES_GREEN, ///<-
	EYES_HAZEL, ///<-
	EYES_RED, ///<-
	EYES_YELLOW, ///<-
	EYES_PURPLE, ///<-
	NO_EYE_COLOR
};

extern const char *str_eye_color[];
extern const char *str_race_eye_color[][NO_EYE_COLOR+1];

/** \sa EYE_SHAPE_TYPES */
typedef ubyte1 eye_shape_t;
/** This should be a 1-byte integer and no more than 254 of these. */
enum EYE_SHAPE_TYPES
{
	EYES_ALMOND, ///<-
	EYES_ROUND, ///<-
	EYES_SQUINTY, ///<-
	EYES_BEADY, ///<-
	NO_EYE_SHAPE
};

extern const char *str_eye_shape[];
extern const char *str_race_eye_shape[][NO_EYE_SHAPE+1];

/** \sa SKIN_COMPLEXION_TYPES */
typedef ubyte1 skin_complexion_t;
/** This should be a 1-byte integer and no more than 254 of these. */
enum SKIN_COMPLEXION_TYPES
{
	SKIN_PALE, ///<-
	SKIN_LIGHT_TAN, ///<-
	SKIN_TAN, ///<-
	SKIN_DARK_TAN, ///<-
	SKIN_DARK, ///<-
	SKIN_WHITE, ///<-
	SKIN_YELLOW, ///<-
	SKIN_RED, ///<-
	SKIN_BROWN, ///<-
	SKIN_BLACK, ///<-
	NO_SKIN_COMPLEXION
};

extern const char *str_skin_complexion[];
extern const char *str_race_skin_complexion[][NO_SKIN_COMPLEXION+1];

/** \sa DEMEANOR_TYPES */
typedef ubyte1 demeanor_t;
/** This should be a 1-byte integer and no more than 254 of these. */
enum DEMEANOR_TYPES
{
	DEMEANOR_VERY_YOUNG, ///<-
	DEMEANOR_YOUNG, ///<-
	DEMEANOR_ADOLESCENT, ///<-
	DEMEANOR_MATURE, ///<-
	DEMEANOR_ELDERLY, ///<-
	DEMEANOR_OLD, ///<-
	DEMEANOR_ANCIENT, ///<-
	NO_DEMEANOR
};

extern const char *str_demeanor[];

/** \sa HANDED_TYPES */
typedef ubyte1 handed_t;
/** This should be a a 1-byte integer and no more than 254 of these. */
enum HANDED_TYPES
{
	RIGHT, ///<-
	LEFT, ///<-
	AMBIDEXTEROUS, ///<-
	NO_HANDED
};

extern const char *str_handed[];

/** \sa GENDER_TYPES */
typedef ubyte1 gender_t;
/** This should be a 1-byte integer and no more than 254 of these. */
enum GENDER_TYPES
{
	FEMALE, ///<-
	MALE, ///<-
	HERMAPHRODITE, ///<-
	NO_GENDER
};

extern const char *str_gender[];
extern const char *str_gender_u[];
extern const char *str_gender_e[];
extern const char *str_gender_m[];
extern const char *str_gender_s[];
extern const char *str_gender_a[];

/** \sa PROFESSION_FLAGS */
typedef ubyte1 profession_flags_t;
typedef ubyte1 profession_t;
/** This should be a 1-byte integer and no more than 7 of these. */
enum PROFESSION_FLAGS
{
	WARRIOR, ///<-
	THIEF, ///<-
	MONK, ///<-
	MAGE, ///<-
	CLERIC, ///<-
	DRUID, ///<-
	NO_PROFESSION
};

extern const char *str_profession[];
extern const char *str_profession_f[];
extern const char *str_profession_header[];
extern const char *str_profession_index(profession_flags_t);

/** \sa HOMELAND_TYPES */
typedef ubyte1 homeland_t;
/** This should be a 1-byte integer and no more than 254 of these. */
enum HOMELAND_TYPES
{
	JORA, ///<-
	ARGO, ///<-
	IMMO, ///<-
	NO_HOMELAND
};

extern const char *str_homeland_a[];
extern const char *str_homeland_p[];
extern const char *str_homeland_d[];
extern const char *str_homeland[];

/** \sa CITY_FLAGS */
typedef ubyte4 citizenship_flags_t;

/** \sa CITY_FLAGS */
typedef ubyte1 city_flags_t;
/** This should be a 2-byte integer and no more than 15 of these */
enum CITY_FLAGS
{
	MIDGAARD, ///<-
	SUBTERFUGE, ///<-
	CATHARSIS, ///<-
	TOKUGAWA, ///<-
	ARGOCEANIA, ///<-
	ELTANIN, ///<-
	NO_CITY
};

extern const char *str_city[];
extern const char *str_citizenship[];

/** \sa CRIMINAL_FLAGS */
typedef ubyte1 crime_flags_t;
/** This should be a 1-byte integer and no more than 7 of these */
enum CRIME_FLAGS
{
	CRIME_KILLER, ///<-
	CRIME_THEFT, ///<-
	CRIME_LOOTER, ///<-
	CRIME_DEAD, ///<-
	NO_CRIME
};

/** \sa POSITION_TYPES */
typedef ubyte1 position_t;
/** This should be a 1-byte integer and no more than 254 of these. */
enum POSITION_TYPES
{
	DEAD, ///<-
	MORTALLY_WOUNDED, ///<-
	INCAPACITATED, ///<-
	STUNNED, ///<-
	SLEEPING, ///<-
	RESTING, ///<-
	SITTING, ///<-
	FIGHTING, ///<-
	STANDING, ///<-
	NO_POSITION
};

extern const char *str_position[];
extern const char *str_position_t[];

/** \sa MOVEMENT_TYPES */
typedef ubyte1 movement_types_t;
/** This should be a 1-byte integer and no more than 254 of these. */
enum MOVEMENT_TYPES
{
	WALKS, ///<-
	FLIES, ///<-
	SWIMS, ///<-
	SLITHERS, ///<-
	OOZES, ///<-
	HOPS, ///<-
	CRAWLS, ///<-
	MARCHES, ///<-
	CREEPS, ///<-
	FLOATS, ///<-
	GALLOPS, ///<-
	LIMPS, ///<-
	PRANCES, ///<-
	SCURRIES, ///<-
	LUMBERS, ///<-
	STAGGERS, ///<-
	RUNS, ///<-
	ROLLS, ///<-
	WADDLES, ///<-
	DIGS, ///<-
	DARTS, ///<-
	SKITTERS, ///<-
	SCAMPERS, ///<-
	STROLLS, ///<-
	MOSEYS, ///<-
	SLIDES, ///<-
	SPINS, ///<-
	WANDERS, ///<-
	CHARGES, ///<-
	STRUTS, ///<-
	POUNCES, ///<-
	HOBBLES, ///<-
	CLIMBS, ///<-
	LEAPS, ///<-
	JUMPS, ///<-
	GLIDES, ///<-
	NO_MOVEMENT
};

extern const char *str_movement[];

/** \sa ATTACK_TYPES */
typedef ubyte1 attack_t;
/** This should be a 1-byte integer and no more than 254 of these. */
enum ATTACK_TYPES
{
	ATTACK_HITS, ///<-
	ATTACK_POUNDS, ///<-
	ATTACK_PIERCES, ///<-
	ATTACK_SLASHES, ///<-
	ATTACK_WHIPS, ///<-
	ATTACK_CLAWS, ///<-
	ATTACK_BITES, ///<-
	ATTACK_STINGS, ///<-
	ATTACK_CRUSHS, ///<-
	ATTACK_MAULS, ///<-
	NO_ATTACK
};

extern const char *str_attack_type[];

// VARIOUS ENUMERATIONS AND FLAGS FOR TINYWORLD_ROOM

/** \sa SECTOR_TYPES */
typedef ubyte1 sector_types_t;
/** This should be a 1-byte integer and no more than 254 of these. */
enum SECTOR_TYPES
{
	CITY, ///<-
	PLAIN, ///<-
	FIELD, ///<-
	FOREST, ///<-
	DESERT, ///<-
	WATER, ///<-
	DEEP_WATER, ///<-
	UNDERWATER, ///<-
	AIR, ///<-
	SNOW, ///<-
	ICE, ///<-
	FIRE, ///<-
	NO_SECTOR
};
  
extern const char *str_sector_type[];

/** \sa ROOM_FLAGS */
typedef ubyte8 room_flags_t;
/** This should be an 8-byte integer and no more than 63 of these. */
enum ROOM_FLAGS
{
	ROOM_DARK, ///<- maybe should be light
	ROOM_DEATH_MIDAIR, ///<- need to iterate the DEATH room types
	ROOM_NO_NPC, ///<- 
	ROOM_NO_WEATHER, ///<-
	ROOM_NO_MAGIC, ///<-
	ROOM_TUNNEL, ///<-
	ROOM_PRIVATE, ///<-
	ROOM_HEALING, ///<-
	ROOM_UNDERWATER, ///<-
	ROOM_AIR, ///<-
	ROOM_SILENCE, ///<-
	ROOM_NO_TRACK, ///<-
	ROOM_ARENA, ///<-
	ROOM_DUMP, ///<-
	ROOM_GOD, ///<-
	ROOM_SAVE, ///<-
	ROOM_RECALL, ///<-
	ROOM_NO_TELEPORT_IN, ///<-
	ROOM_NO_TELEPORT_OUT, ///<-
	ROOM_RESTORE, ///<-
	ROOM_NO_CAMP, ///<-
	ROOM_SHIP_COMMAND, ///<- maybe rethink this
	ROOM_SHIP_FORE, ///<- maybe rethink this
	ROOM_SHIP_AFT, ///<- maybe rethink this
	ROOM_SHIP_STARBOARD, ///<- maybe rethink this
	ROOM_SHIP_PORT, ///<- maybe rethink this
	NO_ROOM_FLAG
};

extern const char *str_room_f[];
extern const char *str_room_flags[];

typedef ubyte1 door_t;

enum DOOR_TYPES
{
	DOOR_PATH,
	DOOR_GATE,
	DOOR_OBJECT,
	DOOR_WINDOW,
	DOOR_STADIUM,
	NO_DOOR_TYPE
};

extern const char *str_door_type[];

/** \sa DOOR_FLAGS */
typedef ubyte2 door_flags_t;
/** This should be a 2-byte integer and no more than 15 of these. */
enum DOOR_FLAGS
{
	DOOR_PICKPROOF, ///<-
	DOOR_SECRET, ///<-
	DOOR_CLOSED, ///<-
	DOOR_LOCKED, ///<-
	DOOR_WIZARD_LOCK, ///<-
	DOOR_KNOCKPROOF, ///<-
	NO_DOOR_FLAGS
};

extern const char *str_door_f[];
extern const char *str_door_flags[];

/** \sa ACT_FLAGS */
typedef ubyte8 act_flags_t;
/** This should be a 8-byte integer and no more than 63 of these. */
enum ACT_FLAGS
{
	ACT_SENTINEL, ///<-
	ACT_SCAVENGER, ///<-
	ACT_PACK, ///<-
	ACT_NICE_THIEF, ///<-
	ACT_AGGRESSIVE, ///<-
	ACT_STAY_ZONE, ///<-
	ACT_WIMPY, ///<-
	ACT_AGGRO_EVIL, ///<-
	ACT_AGGRO_GOOD, ///<-
	ACT_AGGRO_NEUTRAL, ///<-
	ACT_MEMORY, ///<-
	ACT_HUNTER, ///<-
	ACT_RIDEABLE, ///<-
	ACT_GAS_BREATH, ///<-
	ACT_FIRE_BREATH, ///<-
	ACT_LIGHTNING_BREATH, ///<-
	ACT_DRAIN, ///<-
	ACT_POISON, ///<-
	ACT_ACID_BREATH, ///<-
	ACT_FROST_BREATH, ///<-
	ACT_NOCTURNAL, ///<-
	ACT_LOOTER, ///<-
	ACT_STONE_GAZE, ///<-
	ACT_FEAR_AURA, ///<-
	ACT_AGGRO_NPC, ///<-
	ACT_PROTECT, ///<-
	ACT_MARINE, ///<-
	ACT_NO_CHARM, ///<-
	ACT_NO_SLEEP, ///<-
	ACT_TAMEABLE, ///<-
	ACT_CITIZEN, ///<-
	ACT_WIMPY_AGGRO, ///<-
	ACT_DIURNAL, ///<-
	ACT_ANTI_MAGIC, ///<-
	ACT_FRENZY, ///<-
	NO_ACT_FLAG
};

extern const char *str_act_f[];
extern const char *str_act_flags[];

/** \sa AFFECT_FLAGS */
typedef ubyte8 affect_flags_t;
/** This should be a 8-byte integer and no more than 63 of these. */
enum AFFECT_FLAGS
{
	AFF_UNDEAD, ///<-
	AFF_INVISIBLE, ///<-
	AFF_DETECT_EVIL, ///<-
	AFF_DETECT_INVISIBLE, ///<-
	AFF_DETECT_MAGIC, ///<-
	AFF_SENSE_LIFE, ///<-
	AFF_SANCTUARY, ///<-
	AFF_FAERIE_FIRE, ///<-
	AFF_WATER_BREATHING, ///<-
	AFF_BLIND, ///<-
	AFF_POISON, ///<-
	AFF_PROTECT_EVIL, ///<-
	AFF_PETRIFY, ///<-
	AFF_DETECT_GOOD, ///<-
	AFF_WATER_WALKING, ///<-
	AFF_SLEEP, ///<-
	AFF_EVADE, ///<-
	AFF_SNEAK, ///<-
	AFF_HIDE, ///<-
	AFF_PROTECT_GOOD, ///<-
	AFF_CHARM, ///<-
	AFF_HASTE, ///<-
	AFF_LIGHT, ///<-
	AFF_ANTI_SUMMON, ///<-
	AFF_INFRAVISION, ///<-
	AFF_INVULNERABILITY, ///<-
	AFF_NON_DETECTION, ///<-
	AFF_ALLSIGHT, ///<-
	AFF_FARSEE, ///<-
	NO_AFFECT_FLAG
};

extern const char *str_affect_flags[];
extern const char *str_affect_f[];

// ##################
// FOR TINYWORLD_ZONE
// ##################

/** \sa ZONE_TYPES */
typedef ubyte1 zone_types_t;
/** This should be a 1-byte integer and no more than 254 of these. */
enum ZONE_TYPES
{
	RESET_ALWAYS, ///<-
	RESET_EMPTY, ///<-
	NO_ZONE_TYPE
};

extern const char *str_zone_type[];

/** \sa ZONE_FLAGS */
typedef ubyte2 zone_flags_t;
/** This should be a 2-byte integer and no more than 15 of these. */
enum ZONE_FLAGS
{
	ZONE_NEW, ///<-
	ZONE_CLEARED, ///<-
	ZONE_RESTRICTED, ///<-
	ZONE_TO_BE_APPROVED, ///<-
	ZONE_NOWEATHER, ///<-
	ZONE_RAIN, ///<-
	ZONE_CLEAR, ///<-
	ZONE_LIGHTNING, ///<-
	ZONE_HOT, ///<-
	ZONE_SNOW, ///<-
	NO_ZONE_FLAG
};

extern const char *str_zone_f[];
extern const char *str_zone_flags[];

// #############################
// COMMON ENUMERATIONS AND FLAGS
// #############################

/** \sa APPLY_TYPES */
typedef ubyte1 apply_types_t;
/** This should be a 1-byte integer and no more than 254 of these. */
enum APPLY_TYPES
{
	APPLY_STRENGTH, ///<-
	APPLY_DEXTERITY, ///<-
	APPLY_INTELLIGENCE, ///<-
	APPLY_WISDOM, ///<-
	APPLY_CONSTITUTION, ///<-
	APPLY_MAX_MANA, ///<-
	APPLY_MAX_HIT, ///<-
	APPLY_MAX_MOVE, ///<-
	APPLY_ARMOR, ///<-
	APPLY_HITROLL, ///<-
	APPLY_DAMROLL, ///<-
	APPLY_SAVING_PARA, ///<-
	APPLY_SAVING_STAVES, ///<-
	APPLY_SAVING_PETRI, ///<-
	APPLY_SAVING_BREATH, ///<-
	APPLY_SAVING_SPELL, ///<-
	NO_APPLY
};

/** \sa HERB_TYPES */
typedef ubyte1 herb_types_t;
/** This should be a 1-byte integer and no more than 254 of these. */
enum HERB_TYPES
{
	HERB_CURE_LIGHT, ///<-
	HERB_CURE_CRITIC, ///<-
	HERB_HEAL, ///<-
	HERB_MIRACLE, ///<-
	HERB_ARMOR, ///<-
	HERB_BLESS, ///<-
	HERB_SAVAKIS_GLOBE, ///<-
	HERB_HASTE, ///<-
	HERB_INVISIBLE, ///<-
	HERB_SANCTUARY, ///<-
	HERB_DETECT_INVISIBLE, ///<-
	HERB_INFRAVISION, ///<-
	HERB_CURE_BLIND, ///<-
	HERB_REMOVE_POISON, ///<-
	HERB_FOOD, ///<-
	HERB_BLINDNESS, ///<-
	HERB_POISON, ///<-
	HERB_CURE_SERIOUS, ///<-
	HERB_CAUSE_LIGHT, ///<-
	HERB_CAUSE_CRITIC, ///<-
	HERB_CAUSE_SERIOUS, ///<-
	HERB_SLEEP, ///<-
	HERB_VITALITY, ///<-
	HERB_FARSEE, ///<-
	HERB_STRENGTH, ///<-
	HERB_IMPROVE_INVIS, ///<-
	HERB_STONE_TO_FLESH, ///<-
	HERB_HARM, ///<-
	HERB_QUARTER_MANA, ///<-
	HERB_HALF_MANA, ///<-
	HERB_FULL_MANA, ///<-
	NO_HERB
};

/** \sa SPELL_TYPES */
typedef ubyte2 spell_types_t;
/** This should be a 2-byte integer and no more than 65534 of these. */
enum SPELL_TYPES
{
	SPELL_MAGIC_MISSILE, ///<-
	SPELL_VENTRILOQUATE, ///<-
	SPELL_TENSERS_FLOATING_DISC, ///<- who is Tenser
	SPELL_DETECT_MAGIC, ///<-
	SPELL_DETECT_INVISIBLE, ///<-
	SPELL_REMOVE_DARKNESS, ///<-
	SPELL_CHILL_TOUCH, ///<-
	SPELL_INFRAVISION, ///<-
	SPELL_RATHNORS_BARRIER, ///<-
	SPELL_INVISIBILITY, ///<-
	SPELL_FORGET, ///<-
	SPELL_LEARN_ROOM, ///<-
	SPELL_IDENTIFY, ///<-
	SPELL_BURNING_HANDS, ///<-
	SPELL_ARMOR, ///<-
	SPELL_LOCATE_OBJECT, ///<-
	SPELL_FEAR, ///<-
	SPELL_SHOCKING_GRASP, ///<-
	SPELL_STRENGTH, ///<-
	SPELL_KNOCK, ///<-
	SPELL_BLINDNESS, ///<-
	SPELL_TELEPORT_WITH_ERROR, ///<-
	SPELL_FARSEE, ///<-
	SPELL_LIGHTNING_BOLT, ///<-
	SPELL_VITALITY, ///<-
	SPELL_MASS_INVISIBILIT, ///<-
	SPELL_CONTROL_WEATHER, ///<-
	SPELL_TELEPORT_WITHOUT_ERROR, ///<-
	SPELL_WIZARD_EYE, ///<-
	SPELL_COLOR_SPRAY, ///<-
	SPELL_SAVAKIS_INSTANT_RECHARGER, ///<-
	SPELL_IMPROVED_INVISIBILITY, ///<-
	SPELL_ENCHANT_EWAPON, ///<-
	SPELL_CURSE, ///<-
	SPELL_ANIMATE_DEAD, ///<-
	SPELL_ENERGY_DRAIN, ///<-
	SPELL_STONE_TO_FLESH, ///<-
	SPELL_SLEEP, ///<-
	SPELL_CHARM_PERSON, ///<-
	SPELL_GLOBE_OF_INVULNERABILITY, ///<-
	SPELL_FIREBALL, ///<-
	SPELL_WIZARD_LOCK, ///<-
	SPELL_NON_DETECTION, ///<-
	SPELL_HASTE, ///<-
	SPELL_METEOR_SWARM, ///<-
	SPELL_SUMMON_ELEMENTAL, ///<-
	SPELL_ACID_BLAST, ///<-
	SPELL_PRISMATIC_SPRAY, ///<-
	SPELL_DELAYED_BLAST_FIREBALL, ///<-
	SPELL_ICE_STORM, ///<-
	SPELL_CONE_OF_COLD, ///<-
	SPELL_GATE_DAEMON, ///<-
	SPELL_DEATH_HAND, ///<-
	SPELL_CURE_LIGHT_WOUNDS, ///<-
	SPELL_CREATE_WATER, ///<-
	SPELL_DETECT_POISON, ///<-
	SPELL_FAERIE_FIRE, ///<-
	SPELL_CAUSE_LIGHT_WOUNDS, ///<-
	SPELL_CREATE_FOOD, ///<-
	SPELL_WATER_WALKING, ///<-
	SPELL_PASS_WITHOUT_TRACE, ///<-
	SPELL_CURE_BLIND, ///<-
	SPELL_DETECT_EVIL, ///<-
	SPELL_DETECT_GOOD, ///<-
	SPELL_CURE_SERIOUS_WOUNDS, ///<-
	SPELL_CAUSE_SERIOUS_WOUNDS, ///<-
	SPELL_BLESS, ///<-
	SPELL_PROTECTION_FROM_EVIL, ///<-
	SPELL_PROTECTION_FROM_GOOD, ///<-
	SPELL_CAUSE_CRITICAL_WOUNDS, ///<-
	SPELL_EARTHQUAKE, ///<-
	SPELL_SENSE_LIFE, ///<-
	SPELL_ORB_OF_DRAINING, ///<-
	SPELL_SUMMON, ///<-
	SPELL_POISON, ///<-
	SPELL_DISPEL_MAGIC, ///<-
	SPELL_SECOND_WIND, ///<-
	SPELL_CURE_CRITICAL_WOUNDS, ///<-
	SPELL_REMOVE_POISON, ///<-
	SPELL_MASS_BLESS, ///<-
	SPELL_DISPEL_EVIL, ///<-
	SPELL_DISPEL_GOOD, ///<-
	SPELL_WATER_BREATHING, ///<-
	SPELL_WORD_OF_RECALL, ///<-
	SPELL_CALL_LIGHTNING, ///<-
	SPELL_REMOVE_CURSE, ///<-
	SPELL_METASTATSIS, ///<-
	SPELL_FLAMESTRIKE, ///<-
	SPELL_SANCTUARY, ///<-
	SPELL_HEAL, ///<-
	SPELL_HARM, ///<-
	SPELL_FIRESTORM, ///<-
	SPELL_MOONBEAM, ///<-
	SPELL_HOLY_WORD, ///<-
	SPELL_GREATE_HEAL, ///<-
	SPELL_MIRACLE, ///<-
	SPELL_GREATER_MIRACLE, ///<-
	SPELL_HERB_LORE, ///<-
	SPELL_FORAGE, ///<-
	SPELL_AURIC_SNSE, ///<-
	SPELL_SPORE_CLOUD, ///<-
	SPELL_SNAPDRAGON, ///<-
	SPELL_WALL_OF_THORNS, ///<-
	SPELL_BARKSKIN, ///<-
	SPELL_HEART_OF_OAK, ///<-
	SPELL_VINE_SHIELD, ///<-
	SPELL_SUNBEAM, ///<-
	SPELL_GUST_OF_WIND, ///<-
	SPELL_INVIGORATE, ///<-
	SPELL_REJUVENATE, ///<-
	SPELL_FORESTCAPE, ///<-
	SPELL_MALEVOLENT_STORM, ///<-
	SPELL_SUMMON_TREANT, ///<-
	SPELL_EXEMPTION, ///<-
	SPELL_RESTITUTE_BOJECT, ///<-
	SPELL_CATACLYSM, ///<-
	NO_SPELL
};

/** \sa SKILL_TYPES */
typedef ubyte2 skill_types_t;
/** This should be a 2-byte integer and no more than 65534 of these. */
enum SKILL_TYPES
{
	SKILL_BLOCK, ///<-
	SKILL_DODGE, ///<-
	SKILL_CRIT_HIT, ///<-
	SKILL_MEDITATE, ///<-
	SKILL_KILL_HANDS, ///<-
	SKILL_WATER_WALK, ///<-
	SKILL_BREATH, ///<-
	SKILL_ENDURE, ///<-
	SKILL_PREPARE, ///<-
	SKILL_APPLY, ///<-
	SKILL_DRUID_TRACK, ///<-
	SKILL_ENVISAGE, ///<-
	SKILL_FOCUS, ///<-
	SKILL_REFRESH, ///<-
	SKILL_TAME_ANIMAL, ///<-
	SKILL_SWEEP, ///<-
	SKILL_SPIN, ///<-
	SKILL_SLIP, ///<-
	SKILL_SNEAK, ///<-
	SKILL_HIDE, ///<-
	SKILL_STEAL, ///<-
	SKILL_BACKSTAB, ///<-
	SKILL_PICK_LOCK, ///<-
	SKILL_TRACK, ///<-
	SKILL_EVASION, ///<-
	SKILL_PARRY, ///<-
	SKILL_SPY, ///<-
	SKILL_PALM, ///<-
	SKILL_KICK, ///<-
	SKILL_BASH, ///<-
	SKILL_RESCUE, ///<-
	SKILL_MOUNTED_COMBAT, ///<-
	SKILL_BATTLECRY, ///<-
	SKILL_GUARD, ///<-
	SKILL_CHARGE, ///<-
	SKILL_SEARCH, ///<-
	NO_SKILL
};

typedef sbyte1 save_t;

/** \sa SAVING_THROW_TYPES */
typedef ubyte1 save_types_t;
/** Saving throws are what protect you from special forms of attack, such as
    poison or magic.  They are analagous to armor class for normal attacks.
    Your saving throws improve as you gain levels, and some items can also
    improve them.  Similar to armor class, lower saving throw numbers are
    better.<P>
    This should be a 1-byte integer and no more than 254 of these.
 */
enum SAVING_THROW_TYPES
{
	SAVE_PARALYZATION, ///<- paralyzing magic, poison and death magic
	SAVE_STAVES, ///<- staves and wands
	SAVE_PETRIFICATION, ///<-  attacks that could turn you to stone
	SAVE_BREATH, ///<- breath attacks, e.g. from a dragon
	SAVE_SPELL,  ///<- any magical attacks that aren't listed above
	NO_SAVES
};

extern const char *str_saves[];

/** \sa SPELL_TARGET_FLAGS */
typedef ubyte2 spell_target_flags_t;
/** This should be a 2-byte integer and no more than 15 of these. */
enum SPELL_TARGET_FLAGS
{
	TAR_IGNORE, ///<- Target is ignored
	TAR_CHAR_ROOM, ///<- Character in room
	TAR_CHAR_WORLD, ///<- Character in world
	TAR_FIGHT_SELF, ///<- Self if fighting
	TAR_FIGHT_VICT, ///<- Fighting opponant
	TAR_CHAR_AREA, ///<- Character in area
	TAR_NOT_GROUP, ///<- Character not in group
	TAR_SELF_ONLY, ///<- Self not fighting
	TAR_SELF_NONO, ///<- Other character except self
	TAR_ITEM_INVIS, ///<- Unseen item
	TAR_ITEM_ROOM, ///<- Item in room
	TAR_ITEM_WORLD, ///<- Item in world
	TAR_ITEM_EQUIP, ///<- Item in equipment
	NO_TARGET_FLAG
};

/** \sa SPELL_CASTER_FLAGS */
typedef ubyte2 spell_caster_flags_t;
/** This should be a 2-byte integer and no more than 15 of these. */
enum SPELL_CASTER_FLAGS
{
	CAST_CHAR, ///<- from character
	CAST_POTION, ///<- from potion
	CAST_WAND, ///<- from wand
	CAST_STAFF, ///<- from staff
	CAST_SCROLL, ///<- from scroll
	CAST_HERB, ///<- from herb
	NO_CASTER_FLAG
};

/** \sa RESISTANCE_FLAGS */
typedef ubyte2 resistance_flags_t;
/** This should be a 2-byte integer and no more than 15 of these. */
enum RESISTANCE_FLAGS
{
	RESIST_NORM, ///<-
	RESIST_SILVER, ///<-
	RESIST_MAGIC, ///<-
	RESIST_FIRE, ///<-
	RESIST_WATER, ///<-
	RESIST_AIR, ///<-
	RESIST_EARTH, ///<-
	RESIST_COLD, ///<-
	RESIST_ELECTRIC, ///<-
	RESIST_ACID, ///<-
	NO_RESISTANCE_FLAG
};

extern const char *str_resistance_f[];

/** \sa IMMUNITY_FLAGS */
typedef ubyte2 immunity_flags_t;
/** This should be a 2-byte integer and no more than 15 of these. */
enum IMMUNITY_FLAGS
{
	IMMUNE_NORM, ///<-
	IMMUNE_SILVER, ///<-
	IMMUNE_MAGIC, ///<-
	IMMUNE_FIRE, ///<-
	IMMUNE_WATER, ///<-
	IMMUNE_AIR, ///<-
	IMMUNE_EARTH, ///<-
	IMMUNE_COLD, ///<-
	IMMUNE_ELECTRIC, ///<-
	IMMUNE_ACID, ///<-
	IMMUNE_POISON, ///<-
	NO_IMMUNITY_FLAG
};

extern const char *str_immunity_f[];
extern const char *str_immunity_flags[];

/** \sa STAT_TYPES */
typedef ubyte1 stat_t;
/** This should be a 1-byte integer and no more than 254 of these. */
enum STAT_TYPES
{
	STRENGTH, ///<-
	DEXTERITY, ///<-
	INTELLIGENCE, ///<-
	WISDOM, ///<-
	CONSTITUTION, ///<-
	CHARISMA, ///<-
	STRENGTH_ADD, ///<-
	NO_STAT
};

extern const char *str_stat[];
extern const char *str_stat_brev[];
extern const stat_t rank_profession_stats[][CONSTITUTION+1];
extern sbyte1 stat_race_bonus[][CHARISMA+1];


/** \sa VOLUME_TYPES */
typedef ubyte1 volume_types_t;
/** This should be a 1-byte integer and no more than 254 of these. */
enum VOLUME_TYPES
{
	TINY, ///<-
	VERY_SMALL, ///<-
	SMALL, ///<-
	MEDIUM, ///<-
	LARGE, ///<-
	VERY_LARGE, ///<-
	HUMUNGOUS, ///<-
	NO_VOLUME
};

extern const char *str_volume_type[];

/** \sa WEIGHT_TYPES */
typedef ubyte1 weight_types_t;
/** This should be a 1-byte integer and no more than 254 of these. */
enum WEIGHT_TYPES
{
	VERY_LIGHT, ///<-
	LIGHT, ///<-
	MODERATE, ///<-
	HEAVY, ///<-
	VERY_HEAVY, ///<-
	NO_WEIGHT
};

extern const char *str_weight_type[];

/** \sa HEIGHT_TYPES */
typedef ubyte1 height_types_t;
/** This should be a 1-byte integer and no more than 254 of these. */
enum HEIGHT_TYPES
{
	VERY_SHORT, ///<-
	SHORT, ///<-
	AVERAGE, ///<-
	TALL, ///<-
	VERY_TALL, ///<-
	NO_HEIGHT
};

extern const char *str_height_type[];

/** \sa DIR_TYPES */
typedef ubyte1 dir_t;
/** This should be a 1-byte integer and no more than 254 of these. */
enum DIR_TYPES
{
	NORTH, ///<-
	EAST, ///<-
	WEST, ///<-
	SOUTH, ///<-
	UP, ///<-
	DOWN, ///<-
	NO_DIR
};

extern const char *str_Dir[];
extern const char *str_dir[];
extern const dir_t revdir[];

typedef ubyte1 boolean_t;

enum BOOLEAN
{
	FALSE,
	TRUE,
	NO_BOOLEAN_TYPE
};

extern const char *str_boolean[];

typedef ubyte1 status_t;

enum STATUS
{
	OPEN,
	FIXED,
	CLOSED,
	NO_STATUS
};

extern const char *str_status[];

typedef ubyte1 priority_t;

enum PRIORITY
{
	LOW,
	HIGH,
	NO_PRIORITY
};

extern const char *str_priority[];

typedef ubyte1 clan_t;
typedef ubyte2 limit_t;
typedef ubyte1 level_t;
typedef sbyte2 hitpoints_t;
typedef ubyte1 lifespan_t;

extern void init_constants_module(void);

#endif
