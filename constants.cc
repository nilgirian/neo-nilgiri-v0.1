/* **************************************************************************
    Nilgiri the Forgotten World
    (C) 2004
   ************************************************************************** */
/** \file
    \brief Game constants and tables source
 */
#include<stdlib.h>
#include<errno.h>
#include"constants.h"
#include"err.h"

/** Does sanity checking on the integrity of the constants. */
void init_constants_module(void)
{
	FLAG_CHECK(	NO_AFFECT_FLAG, 			affect_flags_t				);
	FLAG_CHECK(	NO_ACT_FLAG, 				act_flags_t					);
	FLAG_CHECK(	NO_PROFESSION,				profession_flags_t		);
	FLAG_CHECK(	NO_WEAR_FLAG,				wear_flags_t				);
	FLAG_CHECK(	NO_IMMUNITY_FLAG,			immunity_flags_t			);
	FLAG_CHECK(	NO_MATERIAL_FLAG,			material_flags_t			);
	FLAG_CHECK(	NO_RESISTANCE_FLAG,		resistance_flags_t		);
	FLAG_CHECK(	NO_ROOM_FLAG,				room_flags_t				);
	FLAG_CHECK(	NO_CASTER_FLAG,			spell_caster_flags_t		);
	FLAG_CHECK(	NO_TARGET_FLAG,			spell_target_flags_t		);
	FLAG_CHECK(	NO_DOOR_FLAGS,				door_flags_t				);
	FLAG_CHECK(	NO_CITY,						city_flags_t				);
	FLAG_CHECK(	NO_CRIME,					crime_flags_t				);
	FLAG_CHECK(	NO_ATTRIBUTE,				item_attribute_flags_t	);
	FLAG_CHECK(	NO_PROPERTY_FLAG,			item_property_flags_t	);
	FLAG_CHECK(	NO_CONTAINER_FLAG,		container_flags_t			);
	FLAG_CHECK(	NO_PLAYER_ENVIRONMENT_FLAG, 	player_env_flags_t	);
	FLAG_CHECK(	NO_PLAYER_AFFECT_FLAG,	player_affect_flags_t	);
	FLAG_CHECK(	NO_IMMORTAL_FLAG,			immortal_flags_t			);
	FLAG_CHECK(	NO_ZONE_FLAG,				zone_flags_t				);

	TYPE_CHECK(	NO_STAT,						stat_t						);
	TYPE_CHECK(	NO_ITEM_TYPE,				item_type_t					);
	TYPE_CHECK(	NO_RACE,						race_t						);
	TYPE_CHECK(	NO_GENDER,					gender_t						);
	TYPE_CHECK(	NO_HOMELAND,				homeland_t					);
	TYPE_CHECK(	NO_POSITION,				position_t					);
	TYPE_CHECK(	NO_MOVEMENT,				movement_types_t			);
	TYPE_CHECK(	NO_ATTACK,					attack_t						);
	TYPE_CHECK(	NO_SECTOR,					sector_types_t				);
	TYPE_CHECK(	NO_SPELL,					spell_types_t				);
	TYPE_CHECK(	NO_SAVES,					save_types_t				);
	TYPE_CHECK(	NO_VOLUME,					volume_types_t				);
	TYPE_CHECK(	NO_WEIGHT,					weight_types_t				);
	TYPE_CHECK(	NO_HEIGHT,					height_types_t				);
	TYPE_CHECK(	NO_DIR,						dir_t							);
	TYPE_CHECK(	NO_SKILL,					skill_types_t				);
	TYPE_CHECK(	NO_HERB,						herb_types_t				);
	TYPE_CHECK(	NO_LIQUID,					liquid_types_t				);
	TYPE_CHECK(	NO_WEAR_POSITION,			wear_position_types_t	);
	TYPE_CHECK(	NO_APPLY,					apply_types_t				);
	TYPE_CHECK(	NO_IMMORTAL_TYPE,			immortal_type_t			);
	TYPE_CHECK(	NO_ZONE_TYPE,				zone_types_t				);
	TYPE_CHECK( NO_PRIORITY,				priority_t					);
	TYPE_CHECK(	NO_STATUS,					status_t						);

	// check the stat race modifiers are legitimate
	for (race_t r = 0; r <= PIXIE; r++)
	{
		sbyte1 sum = 0;
		for (stat_t s = 0; s <= CONSTITUTION; s++)
		{
			sum += stat_race_bonus[r][s];
		}
		if (sum)
		{
			exitlog("/stat bonus for %s sums to %d", str_race[r], sum);
		}
	}
}

const char *str_door_type[NO_DOOR_TYPE+1] =
{
	"path",
	"gate",
	"object",
	"window",
	"stadium",
	"\n"
};

const char *str_door_f[NO_DOOR_FLAGS+1] =
{
	"DOOR_PICKPROOF",
	"DOOR_SECRET",
	"DOOR_CLOSED",
	"DOOR_LOCKED",
	"DOOR_WIZARD_LOCK",
	"DOOR_KNOCKPROOF",
	"\n"
};

const char *str_door_flags[NO_DOOR_FLAGS+1] =
{
	"pickproof",
	"secret",
	"closed",
	"locked",
	"wizard-lock",
	"knockproof",
	"\n"
};

const char *str_act_flags[NO_ACT_FLAG+1] =
{
	"sentinal",
	"scavenger",
	"pack",
	"nice-thief",
	"aggressive",
	"stay-zone",
	"wimpy",
	"aggro-evil",
	"aggro-good",
	"aggro-neutral",
	"memory",
	"hunter",
	"rideable",
	"gas-breath",
	"fire-breath",
	"electric-breath",
	"act-drain",
	"act-poison",
	"acid-breath",
	"frost-breath",
	"nocturnal",
	"looter",
	"stone-gaze",
	"fear-aura",
	"aggro-npc",
	"protect",
	"marine",
	"no-charm",
	"no-sleep",
	"tameable",
	"citizen",
	"wimpy-aggro",
	"diurnal",
	"anti-magic",
	"frenzy",
	"\n"
};

const char *str_act_f[NO_ACT_FLAG+1] =
{
	"ACT_SENTINEL",
	"ACT_SCAVENGER",
	"ACT_PACK",
	"ACT_NICE_THIEF",
	"ACT_AGGRESSIVE",
	"ACT_STAY_ZONE",
	"ACT_WIMPY",
	"ACT_AGGRO_EVIL",
	"ACT_AGGRO_GOOD",
	"ACT_AGGRO_NEUTRAL",
	"ACT_MEMORY",
	"ACT_HUNTER",
	"ACT_RIDEABLE",
	"ACT_GAS_BREATH",
	"ACT_FIRE_BREATH",
	"ACT_ELECTRIC_BREATH",
	"ACT_DRAIN",
	"ACT_POISON",
	"ACT_ACID_BREATH",
	"ACT_FROST_BREATH",
	"ACT_NOCTURNAL",
	"ACT_LOOTER",
	"ACT_STONE_GAZE",
	"ACT_FEAR_AURA",
	"ACT_AGGRO_NPC",
	"ACT_PROTECT",
	"ACT_MARINE",
	"ACT_NO_CHARM",
	"ACT_NO_SLEEP",
	"ACT_TAMEABLE",
	"ACT_CITIZEN",
	"ACT_WIMPY_AGGRO",
	"ACT_DIURNAL",
	"ACT_ANTI_MAGIC",
	"ACT_FRENZY",
	"\n"
};

const char *str_immunity_flags[NO_RESISTANCE_FLAG+1] =
{
	"normal",
	"silver",
	"magic",
	"fire",
	"water",
	"air",
	"earth",
	"cold",
	"electric",
	"acid"
	"\n"			
};

const char *str_resistance_f[NO_RESISTANCE_FLAG+1] =
{
	"RESIST_NORM",
	"RESIST_SILVER",
	"RESIST_MAGIC",
	"RESIST_FIRE",
	"RESIST_WATER",
	"RESIST_AIR",
	"RESIST_EARTH",
	"RESIST_COLD",
	"RESIST_ELECTRIC",
	"RESIST_ACID"
	"\n"			
};

const char *str_immunity_f[NO_IMMUNITY_FLAG+1] =
{
	"IMMUNE_NORM",
	"IMMUNE_SILVER",
	"IMMUNE_MAGIC",
	"IMMUNE_FIRE",
	"IMMUNE_WATER",
	"IMMUNE_AIR",
	"IMMUNE_EARTH",
	"IMMUNE_COLD",
	"IMMUNE_ELECTRIC",
	"IMMUNE_ACID",
	"IMMUNE_POISON",
	"\n"
};

const char *str_property_flags[NO_PROPERTY_FLAG+1] =
{
	"magic",
	"evil",
	"anti-good",
	"anti-evil",
	"anti-neutral",
	"nodrop",
	"bless",
	"nolocate",
	"drop-destruct",
	"worthless",
	"rent",
	"hide-eq",
	"noloc-eq",
	"not-seen",
	"searchable",
	"\n"
};

const char *str_property_f[NO_PROPERTY_FLAG+1] =
{
	"PROP_MAGIC",
	"PROP_EVIL",
	"PROP_ANTI_GOOD",
	"PROP_ANTI_EVIL",
	"PROP_ANTI_NEUTRAL",
	"PROP_NODROP",
	"PROP_BLESS",
	"PROP_NOLOCATE",
	"PROP_DROPDESTRUCT",
	"PROP_WORTHLESS",
	"PROP_ANTI_RENT",
	"PROP_HIDE_EQ",
	"PROP_NOLOC_EQ",
	"PROP_NOT_SEEN",
	"PROP_SEARCHABLE",
	"\n"
};

const char *str_race[NO_RACE+1] =
{
	"human",
	"elf",
	"high-elf",
	"vori-elf",
	"dark-elf",
	"dwarf",
	"gnome",
	"pixie",
	"mammal",
	"reptile",
	"bird",
	"insect",
	"fire-elem",
	"water-elem",
	"earth-elem",
	"air-elem",
	"dragon",
	"\n"
};

const char *str_race_header[NO_RACE+1] =
{
	"Hu",
	"El",
	"He",
	"Ve",
	"De",
	"Dw",
	"Gn",
	"Px",
	" -",
	" -",
	" -",
	" -",
	" -",
	" -",
	" -",
	" -",
	" -",
	"\n"
};

const char *str_race_descr[PIXIE+2] =
{

"\
Humans are the common folk in the world, having made city settlements\n\r\
throughout much of Jora and Argoceania. Humans are generally more social\n\r\
and tolerant than most other races, accepting the company of elves, dwarves\n\r\
and any race that they stumble upon if they are not hostile.  Their nature\n\r\
varies widely; some are good, some evil, some friendly, some aggressive.\n\r\
This uncommon feature has made some races slightly distrustful of them,\n\r\
like the elves.\n\r\
Although humans are quick learners of all kinds of arts, there is usually\n\r\
some other race which is even more proficient in that than the humans.\n\r\
Although humans have their greater diversity as an advantage, this still\n\r\
has made many humans feel a need to prove themselves to the other races,\n\r\
giving them a reputation of fearless leaders of men. Humans start in the\n\r\
city of Midgaard.\n\r",

"\
Elves have very pale complexions, which is odd because they spend a great\n\r\
deal of time outdoors. They tend to be lithe and dexterous. The features of\n\r\
an elf are delicate and finely chiseled. The elves count themselves as\n\r\
being the oldest of the races, and can appear somewhat patronizing towards\n\r\
other races. This has caused trouble on several occasions throughout their\n\r\
history. They live in great cities and have a highly organized society.\n\r\
They often find other races to be crude and hasty.\n\r\
There are three subraces of elves: vori elf, high elf and dark elf.\n\r",

"\
High elves are taller than any other race and have a much thinner build\n\r\
than humans. They are known throughout the realm to be very intelligent and\n\r\
extremely wise.  They are fond of beautiful things, and delight in\n\r\
intricate puzzles.  They are also said to be more adept at the magical arts\n\r\
than other races, though that could be nothing but  a rumor.  High elves\n\r\
usually have a cheerful, if somewhat quiet or guarded, disposition.\n\r\
High elves start with a good alignment.\n\r",

"\
There are many rumors about how the Vori came to be. To the far northwest\n\r\
of Midgaard lies a cold forested wasteland where a great mage resides in a\n\r\
formidable tower.  Legend has it that the Vori were enslaved and forced to\n\r\
build this tower. When the tower was completed, the archmage cursed the\n\r\
Vori to walk the realm, only being able to interbreed with humans,\n\r\
diminishing the divine grace bestowed upon them. That is only one tale of\n\r\
many which circulate among the taverns. Many Vori today find themselves as\n\r\
druids or wanderers of the many islands of Nilgiri, some finding it hard to\n\r\
forge steadfast friendships with other races. Unlike their ancestors,\n\r\
modern day Vori elves are half vori and half human. They are slightly\n\r\
taller than the average elf, though much shorter than high elves.\n\r",

"\
Dark elves are the evil cousins of the high elves. The dark elves live deep\n\r\
within the underworld and often in dark and twisted forests where the\n\r\
darkness has tainted the very way things grow.  Dark elves are known all\n\r\
over the world, or rather \"feared\", for they are cruel, wicked, and VERY\n\r\
evil. They find pleasure in death and pain, in torture and the worship of\n\r\
dragons. Dark elves are particularly good at manipulation, and the mind,\n\r\
while they shudder at the thought of good.\n\r\
Dark elves start with an evil alignment.\n\r",

"\
Unlike humans or elves, dwarves tend to be stocky and muscular.  Their skin\n\r\
is typically deep tan or light brown. Their hair is usually black, gray or\n\r\
brown, and worn long, though not long enough to impair vision in any way.\n\r\
They favor long beards and mustaches too, even among females. Dwarves value\n\r\
their beards highly and tend to groom them very carefully. Although they\n\r\
originally lived in their great underground cities, and still feel most at\n\r\
home in caves beneath the earth, they also build and live in cities above\n\r\
ground, for trading purposes. The gems and metals they find and work on are\n\r\
much wanted by others. Like the rock they are so familiar with, dwarves are\n\r\
hard and unyielding: once their mind is set, it is almost impossible to\n\r\
change. This has led to some long and bloody wars in their past. They are\n\r\
born with natural infravision, which allows them to maneuver quite\n\r\
skillfully in the dark.  They are well respected among most of the other\n\r\
races.\n\r",

"\
The gnomes are closely related to the dwarves as a race. Only in this realm\n\r\
will you find that gnomes grow to be somewhat taller than the average dwarf\n\r\
adult.  They are as nimble as dwarves and wear their beards short and\n\r\
stylishly trimmed. They take great pride in their noses, which often are\n\r\
huge. Their skin is of a vibrant blue, red, or purple with as colorful\n\r\
tufts of hair and sparkling eyes. Gnomes do not mine or smith as the\n\r\
dwarves do, but take great interest in gathering information about anything\n\r\
and everything. They are much respected as scholars. Rumors tell of immense\n\r\
libraries that they gather in their treetop villas, small villages or\n\r\
underground in settlements. Gnomes are often interested in all forms of\n\r\
magic. They are a very peaceful race, going to great lengths to avoid\n\r\
fights or wars with other races. It is said, though, that if you push them\n\r\
to their limit of their great patience, they can take quite drastic\n\r\
actions.\n\r",

"\
Pixies are among the most intelligent and mischievous of the faeries. They\n\r\
dwell in idyllic woodlands, taking great delight in harassing travelers\n\r\
with their pranks and tricks. Fun is the goal for nearly all pixies, and\n\r\
they often define fun by the merry pranks they pull. They look like\n\r\
three-foot-tall elves, though their pointed ears are much longer and\n\r\
slender. The silvery wings jutting from their backs are merely for show,\n\r\
being characteristic of ancestral pixies who could fly. They typically\n\r\
have blond hair and blue/green eyes, though exceptions are often seen. They\n\r\
dress in bright colors, often sporting caps and shoes with curled, pointed\n\r\
toes. Pixies inhabit deep forests and sometimes the meadows near a farmers\n\r\
fields. They dance in moonlit glades, moving to the music of crickets,\n\r\
frogs, and other nocturnal creatures. Despite their weaker stature, pixies\n\r\
make excellent mages.\n\r",

"\n"

};

const char *str_gender[NO_GENDER+1] =
{
	"female",
	"male",
	"hermaphrodite",
	"\n"
};

const char *str_gender_u[NO_GENDER+1] =
{
	"FEMALE",
	"MALE",
	"HERMAPHRODITE",
	"\n"
};

const char *str_gender_a[NO_GENDER+1] = 
{
	"M",
	"F",
	"?",
	"\n"
};

const char *str_gender_e[NO_GENDER+1] =
{
	"she",
	"he",
	"it",
	"\n"
};

const char *str_gender_m[NO_GENDER+1] =
{
	"her",
	"him",
	"it",
	"\n"
};

const char *str_gender_s[NO_GENDER+1] =
{
	"her",
	"his",
	"its",
	"\n"
};

const char *str_hair_length[NO_HAIR_LENGTH+1] =
{
	"bald",
	"cropped",
	"short",
	"medium length",
	"long",
	"very long",
	"\n"
};

const char *str_race_hair_length[PIXIE+1][NO_HAIR_LENGTH+1] =
{
	{
		// human
		str_hair_length[HAIR_BALD],
		str_hair_length[HAIR_CROPPED],
		str_hair_length[HAIR_SHORT],
		str_hair_length[HAIR_MEDIUM],
		str_hair_length[HAIR_LONG],
		str_hair_length[HAIR_VERY_LONG],
		str_hair_length[NO_HAIR_LENGTH]
	},
	{
		// elf
		"" /*HAIR_BALD*/,
		"" /*HAIR_CROPPED*/,
		str_hair_length[HAIR_SHORT],
		str_hair_length[HAIR_MEDIUM],
		str_hair_length[HAIR_LONG],
		str_hair_length[HAIR_VERY_LONG],
		str_hair_length[NO_HAIR_LENGTH]
	},
	{
		// high-elf
		"" /*HAIR_BALD*/,
		"" /*HAIR_CROPPED*/,
		"" /*HAIR_SHORT*/,
		str_hair_length[HAIR_MEDIUM],
		str_hair_length[HAIR_LONG],
		str_hair_length[HAIR_VERY_LONG],
		str_hair_length[NO_HAIR_LENGTH]
	},
	{
		// vori-elf
		"" /*HAIR_BALD*/,
		str_hair_length[HAIR_CROPPED],
		str_hair_length[HAIR_SHORT],
		str_hair_length[HAIR_MEDIUM],
		str_hair_length[HAIR_LONG],
		str_hair_length[HAIR_VERY_LONG],
		str_hair_length[NO_HAIR_LENGTH]
	},
	{
		// dark-elf
		"" /*HAIR_BALD*/,
		"" /*HAIR_CROPPED*/,
		"" /*HAIR_SHORT*/,
		str_hair_length[HAIR_MEDIUM],
		str_hair_length[HAIR_LONG],
		str_hair_length[HAIR_VERY_LONG],
		str_hair_length[NO_HAIR_LENGTH]
	},
	{
		// dwarf
		str_hair_length[HAIR_BALD], 
		"" /*HAIR_CROPPED*/,
		"" /*HAIR_SHORT*/,
		str_hair_length[HAIR_MEDIUM],
		str_hair_length[HAIR_LONG],
		str_hair_length[HAIR_VERY_LONG],
		str_hair_length[NO_HAIR_LENGTH]
	},
	{
		// gnome
		"" /*HAIR_BALD*/,
		"" /*HAIR_CROPPED*/,
		str_hair_length[HAIR_SHORT],
		str_hair_length[HAIR_MEDIUM],
		str_hair_length[HAIR_LONG],
		str_hair_length[HAIR_VERY_LONG],
		str_hair_length[NO_HAIR_LENGTH]
	},
	{
		// pixie
		"" /*HAIR_BALD*/,
		str_hair_length[HAIR_CROPPED],
		str_hair_length[HAIR_SHORT],
		str_hair_length[HAIR_MEDIUM],
		str_hair_length[HAIR_LONG],
		str_hair_length[HAIR_VERY_LONG],
		str_hair_length[NO_HAIR_LENGTH]
	}
};

const char *str_hair_color[NO_HAIR_COLOR+1] =
{
	"black",
	"brown",
	"red",
	"blond",
	"platinum",
	"gray",
	"blue",
	"green",
	"purple",
	"\n"
};

const char *str_race_hair_color[PIXIE+1][NO_HAIR_COLOR+1] =
{
	{
		// human
		str_hair_color[HAIR_BLACK],
		str_hair_color[HAIR_BROWN],
		str_hair_color[HAIR_RED],
		str_hair_color[HAIR_BLOND],
		str_hair_color[HAIR_PLATINUM],
		str_hair_color[HAIR_GRAY],
 		"" /*HAIR_BLUE*/,
		"" /*HAIR_GREEN*/,
		"" /*HAIR_PURPLE*/,
		str_hair_color[NO_HAIR_COLOR]
	},
	{
		// Elf
		str_hair_color[HAIR_BLACK],
		str_hair_color[HAIR_BROWN],
		"" /*HAIR_RED*/,
		str_hair_color[HAIR_BLOND],
		"" /*HAIR_PLATINUM*/,
		"" /*HAIR_GRAY*/,
		str_hair_color[HAIR_BLUE],
		str_hair_color[HAIR_GREEN],
		str_hair_color[HAIR_PURPLE],
		str_hair_color[NO_HAIR_COLOR]
	},
	{
		// high-elf
		"" /*HAIR_BLACK*/,
		"" /*HAIR_BROWN*/,
		"" /*HAIR_RED*/,
		str_hair_color[HAIR_BLOND],
		str_hair_color[HAIR_PLATINUM],
		str_hair_color[HAIR_GRAY],
		str_hair_color[HAIR_BLUE],
		str_hair_color[HAIR_GREEN],
		"" /*HAIR_PURPLE*/,
		str_hair_color[NO_HAIR_COLOR]
	},
	{
		// vori-elf
		str_hair_color[HAIR_BLACK],
		str_hair_color[HAIR_BROWN],
		str_hair_color[HAIR_RED],
		str_hair_color[HAIR_BLOND],
		"" /*HAIR_PLATINUM*/,
		"" /*HAIR_GRAY*/,
		"" /*HAIR_BLUE*/,
		"" /*HAIR_GREEN*/,
		"" /*HAIR_PURPLE*/,
		str_hair_color[NO_HAIR_COLOR]
	},
	{
		// dark-elf
		str_hair_color[HAIR_BLACK],
		"" /*HAIR_BROWN*/,
		str_hair_color[HAIR_RED],
		"" /*HAIR_BLOND*/,
		"" /*HAIR_PLATINUM*/,
		"" /*HAIR_GRAY*/,
		"" /*HAIR_BLUE*/,
		"" /*HAIR_GREEN*/,
		str_hair_color[HAIR_PURPLE],
		str_hair_color[NO_HAIR_COLOR]
	},
	{
		// gnome
		"" /*HAIR_BLACK*/,
		"" /*HAIR_BROWN*/,
		"" /*HAIR_RED*/,
		"" /*HAIR_BLOND*/,
		"" /*HAIR_PLATINUM*/,
		"" /*HAIR_GRAY*/,
		str_hair_color[HAIR_BLUE],
		str_hair_color[HAIR_GREEN],
		str_hair_color[HAIR_PURPLE],
		str_hair_color[NO_HAIR_COLOR]
	},
	{
		// dwarf
		str_hair_color[HAIR_BLACK],
		str_hair_color[HAIR_BROWN],
		"" /*HAIR_RED*/,
		"" /*HAIR_BLOND*/,
		str_hair_color[HAIR_PLATINUM],
		str_hair_color[HAIR_GRAY],
		"" /*HAIR_BLUE*/,
		"" /*HAIR_GREEN*/,
		"" /*HAIR_PURPLE*/,
		str_hair_color[NO_HAIR_COLOR]
	},
	{
		// pixie
		"" /*HAIR_BLACK*/,
		str_hair_color[HAIR_BROWN],
		"" /*HAIR_RED*/,
		str_hair_color[HAIR_BLOND],
		str_hair_color[HAIR_PLATINUM],
		"" /*HAIR_GRAY*/,
		"" /*HAIR_BLUE*/,
		"" /*HAIR_PURPLE*/,
		str_hair_color[HAIR_PURPLE],
		str_hair_color[NO_HAIR_COLOR]
	}
};

const char *str_hair_shape[NO_HAIR_SHAPE+1] =
{
	"straight",
	"wavy",
	"curly",
	"spiked",
	"mohawked",
	"braided",
	"dreadlocked",
	"\n"
};

const char *str_race_hair_shape[PIXIE+1][NO_HAIR_SHAPE+1] =
{
	{
		// human
		str_hair_shape[HAIR_STRAIGHT],
		str_hair_shape[HAIR_WAVY],
		str_hair_shape[HAIR_CURLY],
		str_hair_shape[HAIR_SPIKED],
		str_hair_shape[HAIR_MOHAWKED],
		str_hair_shape[HAIR_BRAIDED],
		str_hair_shape[HAIR_DREADLOCKED],
		str_hair_shape[NO_HAIR_SHAPE]
	},
	{
		// elf
		str_hair_shape[HAIR_STRAIGHT],
		str_hair_shape[HAIR_WAVY],
		"" /*HAIR_CURLY*/,
		"" /*HAIR_SPIKED*/,
		"" /*HAIR_MOHAWKED*/,
		str_hair_shape[HAIR_BRAIDED],
		str_hair_shape[HAIR_DREADLOCKED],
		str_hair_shape[NO_HAIR_SHAPE]
	},
	{
		// high-elf
		str_hair_shape[HAIR_STRAIGHT],
		"" /*HAIR_WAVY*/,
		"" /*HAIR_CURLY*/,
		"" /*HAIR_SPIKED*/,
		"" /*HAIR_MOHAWKED*/,
		str_hair_shape[HAIR_BRAIDED],
		str_hair_shape[HAIR_DREADLOCKED],
		str_hair_shape[NO_HAIR_SHAPE]
	},
	{
		// vori-elf
		str_hair_shape[HAIR_STRAIGHT],
		str_hair_shape[HAIR_WAVY],
		"" /*HAIR_CURLY*/,
		str_hair_shape[HAIR_BRAIDED],
		str_hair_shape[HAIR_SPIKED],
		str_hair_shape[HAIR_MOHAWKED],
		str_hair_shape[HAIR_DREADLOCKED],
		str_hair_shape[NO_HAIR_SHAPE]
	},
	{
		// dark-elf
		str_hair_shape[HAIR_STRAIGHT],
		"" /*HAIR_WAVY*/,
		"" /*HAIR_CURLY*/,
		"" /*HAIR_SPIKED*/,
		"" /*HAIR_MOHAWKED*/,
		str_hair_shape[HAIR_BRAIDED],
		str_hair_shape[HAIR_DREADLOCKED],
		str_hair_shape[NO_HAIR_SHAPE]
	},
	{
		// dwarf
		str_hair_shape[HAIR_STRAIGHT],
		"" /*HAIR_WAVY*/,
		"" /*HAIR_CURLY*/,
		"" /*HAIR_SPIKED*/,
		"" /*HAIR_MOHAWKED*/,
		str_hair_shape[HAIR_BRAIDED],
		str_hair_shape[HAIR_DREADLOCKED],
		str_hair_shape[NO_HAIR_SHAPE]
	},
	{
		// gnome
		str_hair_shape[HAIR_STRAIGHT],
		str_hair_shape[HAIR_WAVY],
		str_hair_shape[HAIR_CURLY],
		"" /*HAIR_SPIKED*/,
		"" /*HAIR_MOHAWKED*/,
		str_hair_shape[HAIR_BRAIDED],
		str_hair_shape[HAIR_DREADLOCKED],
		str_hair_shape[NO_HAIR_SHAPE]
	},
	{
		// pixie
		str_hair_shape[HAIR_STRAIGHT],
		"" /*HAIR_WAVY*/,
		"" /*HAIR_CURLY*/,
		str_hair_shape[HAIR_SPIKED],
		"" /*HAIR_MOHAWKED*/,
		str_hair_shape[HAIR_BRAIDED],
		str_hair_shape[HAIR_DREADLOCKED],
		str_hair_shape[NO_HAIR_SHAPE]
	}
};

const char *str_eye_color[NO_EYE_COLOR+1] =
{
	"black",
	"brown",
	"blue",
	"green",
	"hazel",
	"red",
	"yellow",
	"purple",
	"\n"
};

const char *str_race_eye_color[PIXIE+1][NO_EYE_COLOR+1] =
{
	{
		// human
		str_eye_color[EYES_BLACK],
		str_eye_color[EYES_BROWN],
		str_eye_color[EYES_BLUE],
		str_eye_color[EYES_GREEN],
		str_eye_color[EYES_HAZEL],
		"" /*EYES_RED*/,
		"" /*EYES_YELLOW*/,
		"" /*EYES_PURPLE*/,
		str_eye_color[NO_EYE_COLOR]
	},
	{
		// elf
		"" /*EYES_BLACK*/,
		"" /*EYES_BROWN*/,
		str_eye_color[EYES_BLUE],
		str_eye_color[EYES_GREEN],
		str_eye_color[EYES_HAZEL],
		"" /*EYES_RED*/,
		"" /*EYES_YELLOW*/,
		str_eye_color[EYES_PURPLE],
		str_eye_color[NO_EYE_COLOR]
	},
	{
		// high-elf
		"" /*EYES_BLACK*/,
		"" /*EYES_BROWN*/,
		str_eye_color[EYES_BLUE],
		str_eye_color[EYES_GREEN],
		str_eye_color[EYES_HAZEL],
		"" /*EYES_RED*/,
		"" /*EYES_YELLOW*/,
		str_eye_color[EYES_PURPLE],
		str_eye_color[NO_EYE_COLOR]
	},
	{
		// vori-elf
		"" /*EYES_BLACK*/,
		"" /*EYES_BROWN*/,
		str_eye_color[EYES_BLUE],
		str_eye_color[EYES_GREEN],
		"" /*EYES_HAZEL*/,
		"" /*EYES_RED*/,
		"" /*EYES_YELLOW*/,
		str_eye_color[EYES_PURPLE],
		str_eye_color[NO_EYE_COLOR]
	},
	{
		// dark-elf
		str_eye_color[EYES_BLACK],
		"" /*EYES_BROWN*/,
		"" /*EYES_BLUE*/,
		"" /*EYES_GREEN*/,
		"" /*EYES_HAZEL*/,
		str_eye_color[EYES_RED],
		str_eye_color[EYES_YELLOW],
		str_eye_color[EYES_PURPLE],
		str_eye_color[NO_EYE_COLOR]
	},
	{
		// dawrf
		str_eye_color[EYES_BLACK],
		str_eye_color[EYES_BROWN],
		"" /*EYES_BLUE*/,
		"" /*EYES_GREEN*/,
		str_eye_color[EYES_HAZEL],
		"" /*EYES_RED*/,
		"" /*EYES_YELLOW*/,
		"" /*EYES_PURPLE*/,
		str_eye_color[NO_EYE_COLOR]
	},
	{
		// gnome
		"" /*EYES_BLACK*/,
		"" /*EYES_BROWN*/,
		str_eye_color[EYES_BLUE],
		str_eye_color[EYES_GREEN],
		"" /*EYES_HAZEL*/,
		"" /*EYES_RED*/,
		str_eye_color[EYES_YELLOW],
		str_eye_color[EYES_PURPLE],
		str_eye_color[NO_EYE_COLOR]
	},
	{
		// pixie
		"" /*EYES_BLACK*/,
		str_eye_color[EYES_BROWN],
		"" /*EYES_BLUE*/,
		str_eye_color[EYES_GREEN],
		str_eye_color[EYES_HAZEL],
		"" /*EYES_RED*/,
		"" /*EYES_YELLOW*/,
		"" /*EYES_PURPLE*/,
		str_eye_color[NO_EYE_COLOR]
	}
};

const char *str_eye_shape[NO_EYE_SHAPE+1] =
{
	"almond shaped",
	"large round",
	"squinty",
	"small beady",
	"\n"
};

const char *str_race_eye_shape[PIXIE+1][NO_EYE_SHAPE+1] =
{
	{
		// human
		str_eye_shape[EYES_ALMOND],
		str_eye_shape[EYES_ROUND],
		str_eye_shape[EYES_SQUINTY],
		str_eye_shape[EYES_BEADY],
		str_eye_shape[NO_EYE_SHAPE]
	},
	{
		// elf
		str_eye_shape[EYES_ALMOND],
		"" /*EYES_ROUND*/,
		"" /*EYES_SQUINTY*/,
		"" /*EYES_BEADY*/,
		str_eye_shape[NO_EYE_SHAPE]
	},
	{
		// high-elf
		str_eye_shape[EYES_ALMOND],
		str_eye_shape[EYES_ROUND],
		"" /*EYES_SQUINTY*/,
		"" /*EYES_BEADY*/,
		str_eye_shape[NO_EYE_SHAPE]
	},
	{
		// vori-elf
		str_eye_shape[EYES_ALMOND],
		str_eye_shape[EYES_ROUND],
		str_eye_shape[EYES_SQUINTY],
		str_eye_shape[EYES_BEADY],
		str_eye_shape[NO_EYE_SHAPE]
	},
	{
		// dark-elf
		"" /*EYES_ALMOND*/,
		"" /*EYES_ROUND*/,
		str_eye_shape[EYES_SQUINTY],
		str_eye_shape[EYES_BEADY],
		str_eye_shape[NO_EYE_SHAPE]
	},
	{
		// dwarf
 		"" /*EYES_ALMOND*/,
 		str_eye_shape[EYES_ROUND],
		"" /*EYES_SQUINTY*/,
		"" /*EYES_BEADY*/,
		str_eye_shape[NO_EYE_SHAPE]
	},
	{
		// gnome
		str_eye_shape[EYES_ALMOND],
		str_eye_shape[EYES_ROUND],
		str_eye_shape[EYES_SQUINTY],
		str_eye_shape[EYES_BEADY],
		str_eye_shape[NO_EYE_SHAPE]
	},
	{
		// pixie
		"" /*EYES_ALMOND*/,
		str_eye_shape[EYES_ROUND],
		str_eye_shape[EYES_SQUINTY],
		str_eye_shape[EYES_BEADY],
		str_eye_shape[NO_EYE_SHAPE]
	}
};
  
const char *str_skin_complexion[NO_SKIN_COMPLEXION+1] =
{
	"pale",
	"light tan",
	"tan",
	"dark tan",
	"dark",
	"white",
	"yellow",
	"red",
	"brown",
	"black",
	"\n"
};

const char *str_race_skin_complexion[PIXIE+1][NO_SKIN_COMPLEXION+1] =
{
	{
		// human
		str_skin_complexion[SKIN_PALE],
		str_skin_complexion[SKIN_LIGHT_TAN],
		str_skin_complexion[SKIN_TAN],
		str_skin_complexion[SKIN_DARK_TAN],
		str_skin_complexion[SKIN_DARK],
		"" /*SKIN_WHITE*/,
		"" /*SKIN_YELLOW*/,
		"" /*SKIN_RED*/,
		"" /*SKIN_BROWN*/,
		"" /*SKIN_BLACK*/,
		str_skin_complexion[NO_SKIN_COMPLEXION]
	},
	{
		// elf
		str_skin_complexion[SKIN_PALE],
		str_skin_complexion[SKIN_LIGHT_TAN],
		"" /*SKIN_TAN*/,
		"" /*SKIN_DARK_TAN*/,
		"" /*SKIN_DARK*/,
		"" /*SKIN_WHITE*/,
		"" /*SKIN_YELLOW*/,
		"" /*SKIN_RED*/,
		"" /*SKIN_BROWN*/,
		"" /*SKIN_BLACK*/,
		str_skin_complexion[NO_SKIN_COMPLEXION]
	},
	{
		// high-elf
		str_skin_complexion[SKIN_PALE],
		"" /*SKIN_LIGHT_TAN*/,
		"" /*SKIN_TAN*/,
		"" /*SKIN_DARK_TAN*/,
		"" /*SKIN_DARK*/,
		"" /*SKIN_WHITE*/,
		"" /*SKIN_YELLOW*/,
		"" /*SKIN_RED*/,
		"" /*SKIN_BROWN*/,
		"" /*SKIN_BLACK*/,
		str_skin_complexion[NO_SKIN_COMPLEXION]
	},
	{
		// vori-elf
		str_skin_complexion[SKIN_PALE],
		str_skin_complexion[SKIN_LIGHT_TAN],
		str_skin_complexion[SKIN_TAN],
		str_skin_complexion[SKIN_DARK_TAN],
		"" /*SKIN_DARK*/,
		"" /*SKIN_WHITE*/,
		"" /*SKIN_YELLOW*/,
		"" /*SKIN_RED*/,
		"" /*SKIN_BROWN*/,
		"" /*SKIN_BLACK*/,
		str_skin_complexion[NO_SKIN_COMPLEXION]
	},
	{
		// dark-elf
		"" /*SKIN_PALE*/,
		"" /*SKIN_LIGHT_TAN*/,
		"" /*SKIN_TAN*/,
		str_skin_complexion[SKIN_DARK_TAN],
		str_skin_complexion[SKIN_DARK],
		"" /*SKIN_WHITE*/,
		"" /*SKIN_YELLOW*/,
		"" /*SKIN_RED*/,
		"" /*SKIN_BROWN*/,
		"" /*SKIN_BLACK*/,
		str_skin_complexion[NO_SKIN_COMPLEXION]
	},
	{
		// dwarf
		"" /*SKIN_PALE*/,
		str_skin_complexion[SKIN_LIGHT_TAN],
		str_skin_complexion[SKIN_TAN],
		str_skin_complexion[SKIN_DARK_TAN],
		"" /*SKIN_DARK*/,
		"" /*SKIN_WHITE*/,
		"" /*SKIN_YELLOW*/,
		"" /*SKIN_RED*/,
		"" /*SKIN_BROWN*/,
		"" /*SKIN_BLACK*/,
		str_skin_complexion[NO_SKIN_COMPLEXION]
	},
	{
		// gnome
		"" /*SKIN_PALE*/,
		"" /*SKIN_LIGHT_TAN*/,
		"" /*SKIN_TAN*/,
		"" /*SKIN_DARK_TAN*/,
		"" /*SKIN_DARK*/,
		str_skin_complexion[SKIN_WHITE],
		str_skin_complexion[SKIN_YELLOW],
		str_skin_complexion[SKIN_RED],
		str_skin_complexion[SKIN_BROWN],
		str_skin_complexion[SKIN_BLACK],
		str_skin_complexion[NO_SKIN_COMPLEXION]
	},
	{
		// pixie
		str_skin_complexion[SKIN_PALE],
		str_skin_complexion[SKIN_LIGHT_TAN],
		str_skin_complexion[SKIN_TAN],
		"" /*SKIN_DARK_TAN*/,
		"" /*SKIN_DARK*/,
		"" /*SKIN_WHITE*/,
		"" /*SKIN_YELLOW*/,
		"" /*SKIN_RED*/,
		"" /*SKIN_BROWN*/,
		"" /*SKIN_BLACK*/,
		str_skin_complexion[NO_SKIN_COMPLEXION]
	}
};

const char *str_height_type[NO_HEIGHT+1] =
{
	"very short",
	"short",
	"average",
	"tall",
	"very tall",
	"\n"
};

const char *str_weight_type[NO_WEIGHT+1] =
{
	"very light",
	"light",
	"moderate",
	"heavy",
	"very heavy",
	"\n"
};

const char *str_volume_type[NO_VOLUME+1] =
{
	"tiny",
	"very small",
	"medium",
	"large",
	"very large",
	"humungous",
	"\n"
};

const char *str_demeanor[NO_DEMEANOR+1] =
{
	"very young",
	"young",
	"adolescent",
	"mature",
	"elderly",
	"old",
	"ancient",
	"\n"
};

const char *str_position[NO_POSITION+1] =
{
	"dead",
	"mortally-wounded",
	"incapcitated",
	"stunned",
	"sleeping",
	"resting",
	"sitting",
	"fighting",
	"standing",
	"\n"
};

const char *str_position_t[NO_POSITION+1] =
{
	"dead",
	"mortal-wounds",
	"incapacitated",
	"stunned",
	"sleeping",
	"resting",
	"sitting",
	"fighting",
	"standing",
	"\n"
};

const char *str_movement[NO_MOVEMENT+1] =
{
	"walks",
	"flies",
	"swims",
	"slithers",
	"oozes",
	"hops",
	"crawls",
	"marches",
	"creeps",
	"floats",
	"gallops",
	"limps",
	"prances",
	"scurries",
	"lumbers",
	"staggers",
	"runs",
	"rolls",
	"waddles",
	"digs",
	"darts",
	"skitters",
	"scampers",
	"strolls",
	"moseyes",
	"slides",
	"spins",
	"wanders",
	"charges",
	"struts",
	"pounces",
	"hobbles",
	"climbs",
	"leaps",
	"jumps",
	"glides",
	"\n"
};

const char *str_attack_type[NO_ATTACK+1] =
{
	"hits",
	"pounds",
	"pierces",
	"slashes",
	"whips",
	"claws",
	"bites",
	"stings",
	"crushes",
	"mauls",
	"\n"
};

/** \todo beards 
{
	"no facial hair",
	"moustache",
	"long beard",
	"short beard",
	"scruffy",
	"fu manchu",
	"goatee",
	"\n"
};
*/

const char *str_sector_type[NO_SECTOR+1] =
{
	"city",
	"plain",
	"field",
	"forest",
	"desert",
	"water",
	"deep_water",
	"underwater",
	"air",
	"snow",
	"ice",
	"fire",
	"\n"
};

/** \todo some of these flags should be sectors */
const char *str_room_f[NO_ROOM_FLAG+1] =
{
	"ROOM_DARK",
	"ROOM_DEATH_MIDAIR",
	"ROOM_NO_NPC",
	"ROOM_NO_WEATHER",
	"ROOM_NO_MAGIC",
	"ROOM_TUNNEL",
	"ROOM_PRIVATE",
	"ROOM_HEALING",
	"ROOM_UNDERWATER",
	"ROOM_AIR",
	"ROOM_SILENCE",
	"ROOM_NO_TRACK",
	"ROOM_ARENA",
	"ROOM_DUMP",
	"ROOM_GOD",
	"ROOM_SAVE",
	"ROOM_RECALL",
	"ROOM_NO_TELEPORT_IN",
	"ROOM_NO_TELEPORT_OUT",
	"ROOM_RESTORE",
	"ROOM_NO_CAMP",
	"ROOM_SHIP_COMMAND",
	"ROOM_SHIP_FORE",
	"ROOM_SHIP_AFT",
	"ROOM_SHIP_STARBOARD",
	"ROOM_SHIP_PORT",
	"\n"
};

const char *str_room_flags[NO_ROOM_FLAG+1] =
{
	"dark",
	"death-midair",
	"no-npc",
	"no-weather",
	"no-magic",
	"tunnel",
	"private",
	"healing",
	"underwater",
	"air",
	"silence",
	"no-track",
	"arena",
	"dump",
	"god",
	"save",
	"recall",
	"no-tele-in",
	"no-tele-out",
	"restore",
	"no-camp",
	"ship-comm",
	"ship-fore",
	"ship-aft",
	"ship-starboard",
	"ship-port",
	"\n"
};

const char *str_handed[NO_HANDED+1] =
{
	"right",
	"left",
	"ambidexterous",
	"\n"
};

const char *str_profession[NO_PROFESSION+1] =
{
	"warrior",
	"thief",
	"monk",
	"mage",
	"cleric",
	"druid",
	"\n"
};

const char *str_profession_f[NO_PROFESSION+1] =
{
	"PRO_WARRIOR",
	"PRO_THIEF",
	"PRO_MONK",
	"PRO_MAGE",
	"PRO_CLERIC",
	"PRO_DRUID",
	"\n"
};

const char *str_profession_header[NO_PROFESSION+1] =
{
	"Wa",
	"Th",
	"Mo",
	"Ma",
	"Cl",
	"Dr",
	"\n"
};

const char *str_profession_index(profession_flags_t pro)
{
	static char buf[13];
	char *p;
	p = buf;
	if (IS_SET(pro, f_(WARRIOR)))
	{
		*p++ = 'W';
		*p++ = 'a';
	}
	if (IS_SET(pro, f_(THIEF)))
	{
		*p++ = 'T';
		*p++ = 'h';
	}
	if (IS_SET(pro, f_(MONK)))
	{
		*p++ = 'M';
		*p++ = 'o';
	}
	if (IS_SET(pro, f_(MAGE)))
	{
		*p++ = 'M';
		*p++ = 'a';
	}
	if (IS_SET(pro, f_(CLERIC)))
	{
		*p++ = 'C';
		*p++ = 'l';
	}
	if (IS_SET(pro, f_(DRUID)))
	{
		*p++ = 'D';
		*p++ = 'r';
	}
	*p = '\0';
	return buf;
}

const char *str_homeland[NO_HOMELAND+1] =
{
	"Jora",
	"Argoceania",
	"Immortal",
	"\n"
};

const char *str_homeland_p[NO_HOMELAND+1] =
{
	"Joran",
	"Argocean",
	"Immortal",
	"\n"
};

const char *str_homeland_a[NO_HOMELAND+1] =
{
	"Jor",
	"Arg",
	"Imm",
	"\n"
};

const char *str_homeland_d[NO_HOMELAND+1] =
{
"\
Jora is the cradle of Nilgiri civilization. It is said all things began in\n\r\
Jora and the ancients took to the seas and brought civilization to the\n\r\
other islands and the other great continents.\n\r",

"\
It is said the land of Argoceania was built by a dying great bird of fire.\n\r\
The Great Phoenix caused the ocean waters to boil and the land to rise\n\r\
underneath it. At the moment of death, she cast herself down at the center\n\r\
splitting the great contient in two: Argosiae and Eltanin.\n\r",

"\n"
};

const char *str_city[NO_CITY+1] =
{
	"Midgaard",
	"Subterfuge",
	"Catharsis",
	"Tokugawa",
	"Argoceania",
	"Eltanin",
	"\n"
};

const char *str_citizenship[NO_CITY+1] =
{
	"CITIZEN_MIDGAARD",
	"CITIZEN_SUBTERFUGE",
	"CITIZEN_CATHARSIS",
	"CITIZEN_TOKUGAWA",
	"CITIZEN_ARGOCEANIA",
	"CITIZEN_ELTANIN",
	"\n"
};

const char *str_stat[NO_STAT+1] =
{
	"strength",
	"dexterity",
	"intelligence",
	"wisdom",
	"constitution",
	"charisma",
	"srength-bonus",
	"\n"
};

const char *str_stat_brev[NO_STAT+1] =
{
	"str",
	"dex",
	"int",
	"wis",
	"con",
	"chr",
	"stradd",
	"\n"
};

const stat_t rank_profession_stats[NO_PROFESSION][CONSTITUTION+1] =
{
	{
		// warrior
		STRENGTH,
		DEXTERITY,
		CONSTITUTION,
		WISDOM,
		INTELLIGENCE
	},
	{
		// thief
		DEXTERITY,
 		STRENGTH,
		CONSTITUTION,
		INTELLIGENCE,
		WISDOM
	},
	{
		// monk
		WISDOM,
 		DEXTERITY,
		STRENGTH,
		CONSTITUTION,
		INTELLIGENCE
	},
	{
		// mage
		INTELLIGENCE,
		WISDOM,
		DEXTERITY,
		STRENGTH,
		CONSTITUTION
	},
	{
		// cleric
		WISDOM,
		INTELLIGENCE,
		STRENGTH,
		DEXTERITY,
		CONSTITUTION
	},
	{
		// druid
		WISDOM,
		DEXTERITY,
		STRENGTH,
		CONSTITUTION,
		INTELLIGENCE
	}
};

sbyte1 stat_race_bonus[PIXIE+1][CHARISMA+1] =
{
	// "strength", "dexterity", "intelligence", "wisdom", "constitution"
	{
		// human,
		0, 0, 0, 0, 0, 0
	},
	{
		// elf
		-2, 1, 1, 1, -1, 0
	},
	{
		// high-elf
		-3, 1, 2, 2, -2, 0
	},
	{
		// vori-elf
		-1, 0, 1, 1, -1, 2
	},
	{
		// dark-elf
		-2, 1, 1, 1, -1, 0
	},
	{
		// dwarf
		1, -1, -1, -1, 2, 0
	},
	{
		// gnome
		-1, -1, -1, 1, 2, 0
	},
	{
		// pixie
		-3, 3, 2, -1, -1, 1
	}
};

const char *str_material_f[NO_MATERIAL_FLAG+1] =
{
	"MTRL_ADAMANTIUM",
	"MTRL_ALUMINUM",
	"MTRL_BARK",
	"MTRL_BONE",
	"MTRL_BRASS",
	"MTRL_BRONZE",
	"MTRL_CANVAS",
	"MTRL_CHITIN",
	"MTRL_CLAY",
	"MTRL_CLOTH",
	"MTRL_COPPER",
	"MTRL_COTTON",
	"MTRL_CRYSTAL",
	"MTRL_DEMONHIDE",
	"MTRL_DIAMOND",
	"MTRL_DRAGONSCALE",
	"MTRL_EMERALD",
	"MTRL_FLESH",
	"MTRL_FUR",
	"MTRL_GEMSTONE",
	"MTRL_GLASS",
	"MTRL_GOLD",
	"MTRL_GRANITE",
	"MTRL_HAIR",
	"MTRL_HARD_LEATHER",
	"MTRL_HIDE",
	"MTRL_IRON",
	"MTRL_LEAD",
	"MTRL_LINEN",
	"MTRL_MYTHRIL",
	"MTRL_PLANT",
	"MTRL_PLATINUM",
	"MTRL_QUARTZ",
	"MTRL_RUBY",
	"MTRL_SAPPHIRE",
	"MTRL_SILVER",
	"MTRL_SILK",
	"MTRL_SOFT_LEATHER",
	"MTRL_STEEL",
	"MTRL_STONE",
	"MTRL_SUEDE",
	"MTRL_TIN",
	"MTRL_TITANIUM",
	"MTRL_WOOD",
	"\n"
};

const char *str_material[NO_MATERIAL_FLAG+1] =
{
	"adamantium",
	"aluminum",
	"bark",
	"bone",
	"brass",
	"bronze",
	"canvas",
	"chitin",
	"clay",
	"cloth",
	"copper",
	"cotton",
	"crystal",
	"demonhide",
	"diamond",
	"dragonscale",
	"emerald",
	"flesh",
	"fur",
	"gemstone",
	"glass",
	"gold",
	"granite",
	"hair",
	"hair_leather",
	"hide",
	"iron",
	"lead",
	"linen",
	"mythril",
	"plant",
	"platinum",
	"quartz",
	"ruby",
	"sapphire",
	"silver",
	"silk",
	"soft leather",
	"steel",
	"stone",
	"suede",
	"tin",
	"titanium",
	"wood",
	"\n"
};

const char *str_item_type[NO_ITEM_TYPE+1] =
{
	"other",
	"light",
	"scroll",
	"wand",
	"staff",
	"weapon",
	"fire_weapon",
	"missile",
	"treasure",
	"armor",
	"potion",
	"worn",
	"trash",
	"trap",
	"container",
	"note",
	"liquid",
	"key",
	"food",
	"money",
	"pen",
	"boat",
	"herb",
	"plant",
	"spell_object",
	"statue",
	"teleport",
	"lock_pick",
	"ship",
	"war ship",
	"chart",
	"furniture",
	"\n"
};

const char *str_wear_pos_f[NO_WEAR_FLAG+1] =
{
	"WEAR_TAKE",
	"WEAR_FINGER",
	"WEAR_NECK",
	"WEAR_BODY",
	"WEAR_HEAD",
	"WEAR_LEGS",
	"WEAR_FEET",
	"WEAR_HANDS",
	"WEAR_ARMS",
	"WEAR_SHIELD",
	"WEAR_BUCKLER",
	"WEAR_ABOUT",
	"WEAR_WAIST",
	"WEAR_WRIST",
	"WEAR_WIELD",
	"WEAR_HOLD",
	"WEAR_FLOATING",
	"WEAR_EARRING",
	"WEAR_FACE",
	"\n"
};

const char *str_wear_pos_flags[NO_WEAR_FLAG+1] =
{
	"take",
	"finger",
	"neck",
	"body",
	"head",
	"legs",
	"feet",
	"hands",
	"arms",
	"shield",
	"buckler",
	"about",
	"waist",
	"wrist",
	"wield",
	"hold",
	"floating",
	"earring",
	"face",
	"\n"
};


const char *str_attribute_flags[NO_ATTRIBUTE+1] =
{
	"glow",
	"hum",
	"mirrored",
	"invisible",
	"lit",
	"\n"
};

const char *str_attribute_f[NO_ATTRIBUTE+1] =
{
	"ATTR_GLOW",
	"ATTR_HUM",
	"ATTR_MIRRORED",
	"ATTR_INVISIBLE",
	"ATTR_LIT",
	"\n"
};

const char *str_affect_flags[NO_AFFECT_FLAG+1] =
{
	"undead",
	"invisible",
	"detect-evil",
	"detect-invis",
	"detect-magic",
	"sense-life",
	"sanctuary",
	"faerie-fire",
	"water-breath",
	"blind",
	"poison",
	"protect-evil",
	"petrify",
	"detect-good",
	"water-walking",
	"sleep",
	"evade",
	"sneak",
	"hide",
	"protect-good",
	"charm",
	"haste",
	"light",
	"anti-summon",
	"infravision",
	"invulnerability",
	"non-detection",
	"allsight",
	"farsee",
	"\n"
};

const char *str_affect_f[NO_AFFECT_FLAG+1] =
{
	"AFF_UNDEAD",
	"AFF_INVISIBLE",
	"AFF_DETECT_EVIL",
	"AFF_DETECT_INVISIBLE",
	"AFF_DETECT_MAGIC",
	"AFF_SENSE_LIFE",
	"AFF_SANCTUARY",
	"AFF_FAERIE_FIRE",
	"AFF_WATER_BREATHING",
	"AFF_BLIND",
	"AFF_POISON",
	"AFF_PROTECT_EVIL",
	"AFF_PETRIFY",
	"AFF_DETECT_GOOD",
	"AFF_WATER_WALKING",
	"AFF_SLEEP",
	"AFF_EVADE",
	"AFF_SNEAK",
	"AFF_HIDE",
	"AFF_PROTECT_GOOD",
	"AFF_CHARM",
	"AFF_HASTE",
	"AFF_LIGHT",
	"AFF_ANTI_SUMMON",
	"AFF_INFRAVISION",
	"AFF_INVULNERABILITY",
	"AFF_NON_DETECTION",
	"AFF_ALLSIGHT",
	"AFF_FARSEE",
	"\n"
};

const char *str_player_env[NO_PLAYER_ENVIRONMENT_FLAG+1] =
{
	"sneak",
	"evade",
	"nokill",
	"notake",
	"nomercy",
	"noflee",
	"guard",
	"recall",
	"consider",
	"\n"
};

const char *str_player_env_f[NO_PLAYER_ENVIRONMENT_FLAG+1] =
{
	"ENV_SNEAK",
	"ENV_EVADE",
	"ENV_KILL",
	"ENV_TAKE",
	"ENV_MERCY",
	"ENV_FLEE",
	"ENV_GUARD",
	"ENV_RECALL",
	"ENV_CONSIDER",
	"\n"
};

const char *str_saves[NO_SAVES+1] =
{
	"paralyzations",
	"staves",
	"petrifications",
	"breaths",
	"spells",
	"\n"
};

const char *str_desc_env[NO_DESC_ENVIRONMENT_FLAG+1] =
{
	"brief",
	"compact",
	"abbrev",
	"exits",
	"exitslong",
	"combatbrief",
	"echo",
	"weather",
	"display_h",
	"display_m",
	"display_v",
	"tell",
	"shout",
	"auction",
	"gossip",
	"wrap",
	"splitscreen",
	"color",
	"\n"
};

const char *str_desc_env_f[NO_DESC_ENVIRONMENT_FLAG+1] =
{
	"DENV_BRIEF",
	"DENV_COMPACT",
	"DENV_ABBREV",
	"DENV_EXITS",
	"DENV_EXITS_L",
	"DENV_CBRIEF",
	"DENV_ECHO",
	"DENV_WEATHER",
	"DENV_DIS_H",
	"DENV_DIS_M",
	"DENV_DIS_V",
	"DENV_TELL",
	"DENV_SHOUT",
	"DENV_AUCTION",
	"DENV_GOSSIP",
	"DENV_WRAP",
	"DENV_SPLITSCREEN",
	"DENV_COLOR",
	"\n"
};

const char *str_immortal_types_u[NO_IMMORTAL_TYPE+1] =
{
	"IMMORTAL",
	"DEITY",
	"GOD",
	"DEUCE",
	"IMPLEMENTOR",
	"\n"
};

const char *str_immortal_types[NO_IMMORTAL_TYPE+1] =
{
	"immortal",
	"deity",
	"god",
	"deuce",
	"implementor",
	"\n"
};

const char *str_immortal_types_abbrev[NO_IMMORTAL_TYPE+1] =
{
	"IMM",
	"DEI",
	"GOD",
	"IMP",
	"\n"
};

const char *str_boolean[NO_BOOLEAN_TYPE+1] =
{
	"TRUE",
	"FALSE",
	"\n"
};

const char *str_Dir[NO_DIR+1] =
{
	"North",
	"East",
	"West",
	"South",
	"Up",
	"Down",
	"\n"
};

const char *str_dir[NO_DIR+1] =
{
	"north",
	"east",
	"west",
	"south",
	"up",
	"down",
	"\n"
};

const dir_t revdir[NO_DIR+1] =
{
	SOUTH,
	WEST,
	EAST,
	NORTH,
	DOWN,
	UP,
	NO_DIR
};

const char *str_zone_type[NO_ZONE_TYPE+1] =
{
	"reset-always",
	"reset-empty",
	"\n"
};

const char *str_zone_f[NO_ZONE_FLAG+1] =
{
	"ZONE_NEW",
	"ZONE_CLEARED", 
	"ZONE_RESTRICTED",
	"ZONE_TBA",
	"ZONE_NOWEATHER",
	"ZONE_RAIN",
	"ZONE_CLEAR",
	"ZONE_LIGHTNING",
	"ZONE_HOT",
	"ZONE_SNOW",
	"\n"
};


const char *str_zone_flags[NO_ZONE_FLAG+1] =
{
	"new",
	"clearec", 
	"restricted",
	"tba",
	"noweather",
	"rain",
	"clear",
	"lightning",
	"hot",
	"snow",
	"\n"
};

const char *str_status[NO_STATUS+1] =
{
	"open",
	"fixed",
	"closed",
	"\n"
};

const char *str_priority[NO_PRIORITY+1] =
{
	"low",
	"high",
	"\n"
};

