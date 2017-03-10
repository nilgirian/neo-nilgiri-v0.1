/* **************************************************************************
    Nilgiri the Forgotten World by:
    (C) 2004
   ************************************************************************** */
/** \file
    \brief Character object source
 */
#include<stdlib.h>
#include<string.h>
#include"db.h"
#include"tinyworld_character.h"
#include"descriptor.h"
#include"combat.h"
#include"err.h"
#include"tinyworld_time.h"
#include"combat.h"
#include"addlog.h"

/** Array of character descriptions */
CHAR_DESCR_DATA **char_descr = NULL;
/** Array of character types */
CHAR_STATS_DATA **char_stats = NULL;
/** Array of character data */
CHAR_DATA **char_data = NULL;

/** Top index of char_descr */
top_t char_descr_top = -1;
/** Top index of char_stats */
top_t char_stats_top = -1;
/** Top index of char_data */
top_t char_data_top = -1;

/** A map from virtual IDs to real IDs for character descriptions created */
VidIndex cdescrIndex;
/** A map from virtual IDs to real IDs for character types created */
VidIndex cstatsIndex;
/** A map from virtual IDs to real IDs for character data created */
VidIndex cdataIndex;

/** A list of characters currently in the game regardless if they are a
    player or not.
 */
Character *character_list = NULL;

/** \return This creates a standard human description.  Everything in
    CHAR_DESCR_DATA is assigned except the virtual_id.
 */
static void create_human_descr(CHAR_DESCR_DATA *cdescr)
{
	cdescr->key->add("human");
	STRDUP(cdescr->proper, "a human");
	STRDUP(cdescr->room, "A human being is here.");
	STRDUP(cdescr->general, "This is a human being.\r\n");
	STRDUP(cdescr->creators_note, "This is the default human being.\r\n");
	return;
}

/** \return This creates a standard elf description.  Everything in
    CHAR_DESCR_DATA is assigned except the virtual_id.
 */
static void create_elf_descr(CHAR_DESCR_DATA *cdescr)
{
	cdescr->key->add("elf");
	STRDUP(cdescr->proper, "an elf");
	STRDUP(cdescr->room, "An elf being is here.");
	STRDUP(cdescr->general, "This is an elf being.\r\n");
	STRDUP(cdescr->creators_note, "This is the default elf being.\r\n");
	return;
}

/** \return This creates a standard high-elf description.  Everything in
    CHAR_DESCR_DATA is assigned except the virtual_id.
 */
static void create_high_elf_descr(CHAR_DESCR_DATA *cdescr)
{
	cdescr->key->add("high-elf");
	STRDUP(cdescr->proper, "a high-elf");
	STRDUP(cdescr->room, "A high-elf being is here.");
	STRDUP(cdescr->general, "This is a high-elf being.\r\n");
	STRDUP(cdescr->creators_note, "This is the default high-elf being.\r\n");
	return;
}

/** \return This creates a standard dark-elf description.  Everything in
    CHAR_DESCR_DATA is assigned except the virtual_id.
 */
static void create_dark_elf_descr(CHAR_DESCR_DATA *cdescr)
{
	cdescr->key->add("dark-elf");
	STRDUP(cdescr->proper, "a dark-elf");
	STRDUP(cdescr->room, "A dark-elf being is here.");
	STRDUP(cdescr->general, "This is a dark-elf being.\r\n");
	STRDUP(cdescr->creators_note, "This is the default dark-elf being.\r\n");
	return;
}

/** \return This creates a standard vori-elf description.  Everything in
    CHAR_DESCR_DATA is assigned except the virtual_id.
 */
static void create_vori_elf_descr(CHAR_DESCR_DATA *cdescr)
{
	cdescr->key->add("vori-elf");
	STRDUP(cdescr->proper, "a vori-elf");
	STRDUP(cdescr->room, "A vori-elf being is here.");
	STRDUP(cdescr->general, "This is a vori-elf being.\r\n");
	STRDUP(cdescr->creators_note, "This is the default vori-elf being.\r\n");
	return;
}

/** \return This creates a standard dwarf description.  Everything in
    CHAR_DESCR_DATA is assigned except the virtual_id.
 */
static void create_dwarf_descr(CHAR_DESCR_DATA *cdescr)
{
	cdescr->key->add("dwarf");
	STRDUP(cdescr->proper, "a dwarf");
	STRDUP(cdescr->room, "A dwarf being is here.");
	STRDUP(cdescr->general, "This is an dwarf being.\r\n");
	STRDUP(cdescr->creators_note, "This is the default dwarf being.\r\n");
	return;
}

/** \return This creates a standard gnome description.  Everything in
    CHAR_DESCR_DATA is assigned except the virtual_id.
 */
static void create_gnome_descr(CHAR_DESCR_DATA *cdescr)
{
	cdescr->key->add("gnome");
	STRDUP(cdescr->proper, "a gnome");
	STRDUP(cdescr->room, "A gnome being is here.");
	STRDUP(cdescr->general, "This is a gnome being.\r\n");
	STRDUP(cdescr->creators_note, "This is the default gnome being.\r\n");
	return;
}

/** \return This creates a standard pixie description.  Everything in
    CHAR_DESCR_DATA is assigned except the virtual_id.
 */
static void create_pixie_descr(CHAR_DESCR_DATA *cdescr)
{
	cdescr->key->add("pixie");
	STRDUP(cdescr->proper, "a pixie");
	STRDUP(cdescr->room, "A pixie being is here.");
	STRDUP(cdescr->general, "This is a pixie being.\r\n");
	STRDUP(cdescr->creators_note, "This is the default pixie being.\r\n");
}

/** Create standard the character descriptions. */
static void create_tinyworld_character_descriptions(void)
{
	vid_t id;
	char_descr_top = 7;

	MALLOC(char_descr, CHAR_DESCR_DATA *, char_descr_top+1);
	id = 0;

	MALLOC(char_descr[id], CHAR_DESCR_DATA, 1);
	char_descr[id]->key = new Words("char_descr");
	create_human_descr(char_descr[id]);
	char_descr[id]->virtual_id = ++id;

	MALLOC(char_descr[id], CHAR_DESCR_DATA, 1);
	char_descr[id]->key = new Words("char_descr");
	create_elf_descr(char_descr[id]);
	char_descr[id]->virtual_id = ++id;

	MALLOC(char_descr[id], CHAR_DESCR_DATA, 1);
	char_descr[id]->key = new Words("char_descr");
	create_high_elf_descr(char_descr[id]);
	char_descr[id]->virtual_id = ++id;

	MALLOC(char_descr[id], CHAR_DESCR_DATA, 1);
	char_descr[id]->key = new Words("char_descr");
	create_dark_elf_descr(char_descr[id]);
	char_descr[id]->virtual_id = ++id;

	MALLOC(char_descr[id], CHAR_DESCR_DATA, 1);
	char_descr[id]->key = new Words("char_descr");
	create_vori_elf_descr(char_descr[id]);
	char_descr[id]->virtual_id = ++id;

	MALLOC(char_descr[id], CHAR_DESCR_DATA, 1);
	char_descr[id]->key = new Words("char_descr");
	create_dwarf_descr(char_descr[id]);
	char_descr[id]->virtual_id = ++id;

	MALLOC(char_descr[id], CHAR_DESCR_DATA, 1);
	char_descr[id]->key = new Words("char_descr");
	create_gnome_descr(char_descr[id]);
	char_descr[id]->virtual_id = ++id;

	MALLOC(char_descr[id], CHAR_DESCR_DATA, 1);
	char_descr[id]->key = new Words("char_descr");
	create_pixie_descr(char_descr[id]);
	char_descr[id]->virtual_id = ++id;

	if((int) id != char_descr_top+1)
	{
		exitlog("/char_descr_top too small");
	}
}

/** Create the character types and data. Changes to how the CHAR_DATA vid is set
    needs to also be reflected in the init_pc_data_idx() function.
 */
static void create_tinyworld_characters(void)
{
	vid_t id;
	int i;
	race_t r;
	gender_t g;
	profession_t p;
	homeland_t h;
	char buf[LINE_LENGTH];

	char_stats_top
		= (PIXIE-HUMAN+1)*(MALE-FEMALE+1)*(DRUID-WARRIOR+1)*(ARGO-JORA+1)-1;

	char_data_top = char_stats_top;

	MALLOC(char_stats, CHAR_STATS_DATA *, char_stats_top+1);
	MALLOC(char_data, CHAR_DATA *, char_data_top+1);

	id = 0;

	for (h = JORA; h <= ARGO; h++)
	{
		for (r = HUMAN; r <= PIXIE; r++)
		{
			for (g = FEMALE; g <= MALE; g++)
			{
				for (p = WARRIOR; p <= DRUID; p++)
				{
					MALLOC(char_stats[id], CHAR_STATS_DATA, 1);
					MALLOC(char_data[id], CHAR_DATA, 1);
					char_stats[id]->race = r;
					char_stats[id]->gender = g;
					char_stats[id]->profession = f_(p);
					char_stats[id]->movement_type = WALKS;
					char_stats[id]->attack_top = MAX_PC_ATTACKS-1;
					MALLOC(char_stats[id]->attack, ATTACK, MAX_PC_ATTACKS);
					for (i = 0; i < MAX_PC_ATTACKS; i++)
					{
						char_stats[id]->attack[i].type = ATTACK_HITS;
						char_stats[id]->attack[i].damage.die = 0;
						char_stats[id]->attack[i].damage.multiplier = 0;
						char_stats[id]->attack[i].damage.modifier = 0;
					}
					char_stats[id]->act_flags = 0;
					char_stats[id]->special.affect_flags = 0;
					char_stats[id]->special.resistance_flags = 0;
					char_stats[id]->special.immunity_flags = 0;
					char_stats[id]->weight_class = MODERATE;
					char_stats[id]->volume_class = MEDIUM;
					char_stats[id]->height_class = AVERAGE;

					char_stats[id]->homeland = h;
					switch (h)
					{
						case JORA:
							char_stats[id]->special.citizenship = f_(MIDGAARD);
							break;
						case ARGO:
							char_stats[id]->special.citizenship = f_(ELTANIN);
							break;
						default:
							exitlog("/bad homeland:%d", h);
					}
					for (stat_t s = 0; s < NO_STAT; s++)
					{
						char_stats[id]->special.stat[s] = 0;
					}
					char_stats[id]->special.position = STANDING;

					char_stats[id]->thac0 = 20;
					char_stats[id]->hitpoints.die = 4;
					char_stats[id]->hitpoints.multiplier = 1;
					char_stats[id]->hitpoints.modifier = 18;
					for (i = 0; i < NO_SAVES; i++)
					{
						char_stats[id]->special.save_vs[i] = 0;
					}

					sprintf(buf, "Default %s %s %s %s character type.\n",
							str_homeland_p[h],
							str_race[r],
							str_gender[g],
							str_profession[p]);
					STRDUP(char_stats[id]->creators_note, buf);

					char_stats[id]->virtual_id = id+1;

					char_data[id]->descr = char_descr[r];
					char_data[id]->stats = char_stats[id];
					char_data[id]->clan = 0;
					char_data[id]->count = 0;
					char_data[id]->limit = (limit_t) -1;
					sprintf(buf, "Default %s %s %s %s character data.\n",
							str_homeland_p[h],
							str_race[r],
							str_gender[g],
							str_profession[p]);
					STRDUP(char_data[id]->creators_note, buf);
					char_data[id]->virtual_id = id+1;
					id++;
				}
			}
		}
	}

	if((int) id != char_stats_top+1)
	{
		exitlog("/char_stats_top too small/%d != %d", char_stats_top, id);
	}
}

/** Save a single character description entry.
	 \param cdescr A newly allocated char_descr item
 */
void save_tinyworld_character_description(CHAR_DESCR_DATA *cdescr)
{
	char *dbErrMsg = NULL;

	SQLITE_EXEC_PRINTF(db, "REPLACE INTO char_descr_data "
			"(char_descr_id, proper, room, general, creators_note)"
			" VALUES (%u, %Q, %Q, %Q, %Q)",
			NULL,
			NULL,
			dbErrMsg,
			cdescr->virtual_id,
			cdescr->proper,
			cdescr->room,
			cdescr->general,
			cdescr->creators_note);
	cdescr->key->save(cdescr->virtual_id);
}

void save_tinyworld_character_descriptions(void)
{
	ubyte4 count = 0;

	for (top_t i = 0; i <= char_descr_top; i++)
	{
		save_tinyworld_character_description(char_descr[i]);
		count++;
	}
}

/** Save the character type dmm data. */
static void save_tinyworld_char_stats_dmm(CHAR_STATS_DATA *cstats)
{
	char *dbErrMsg = NULL;

	SQLITE_EXEC_PRINTF(db, "DELETE FROM char_stats_dmm WHERE char_stats_id = %u",
				NULL,
				NULL,
				dbErrMsg,
				cstats->virtual_id);

	for (top_t i = 0; i <= cstats->attack_top; i++)
	{
		SQLITE_EXEC_PRINTF(db, "INSERT INTO char_stats_dmm"
					"(char_stats_id, attack_id,"
					" attack_type, die, multiplier, modifier)"
					" VALUES "
					"(%u, %u,"
					" %Q, %u, %u, %d)",
					NULL,
					NULL,
					dbErrMsg,
					cstats->virtual_id,
					i,
					str_attack_type[cstats->attack[i].type],
					cstats->attack[i].damage.die,
					cstats->attack[i].damage.multiplier,
					cstats->attack[i].damage.modifier);
	}
}

/** Save the character type flags */
static void save_tinyworld_char_stats_flags(CHAR_STATS_DATA *cstats)
{
	char *dbErrMsg = NULL;

	SQLITE_EXEC_PRINTF(db,
			"DELETE FROM char_stats_flags WHERE char_stats_id = %u",
			NULL,
			NULL,
			dbErrMsg,
			cstats->virtual_id);

	//citizenship flags
	save_flags(cstats->virtual_id, cstats->special.citizenship, str_citizenship,
				"char_stats");

	//profession flags
	save_flags(cstats->virtual_id, cstats->profession, str_profession_f,
				"char_stats");

	//act flags
	save_flags(cstats->virtual_id, cstats->act_flags, str_act_f,
				"char_stats");

	//affects flags
	save_flags(cstats->virtual_id, cstats->special.affect_flags,
			str_affect_f, "char_stats");

	//resisitance flags
	save_flags(cstats->virtual_id, cstats->special.resistance_flags,
			str_resistance_f, "char_stats");

	//immunity flags
	save_flags(cstats->virtual_id, cstats->special.immunity_flags,
			str_immunity_f, "char_stats");
}

/** Save a single character type data. */
void save_tinyworld_character_stat(CHAR_STATS_DATA *cstats)
{
	char *dbErrMsg = NULL;

	SQLITE_EXEC_PRINTF(db, "REPLACE INTO char_stats_data "
				"(char_stats_id, race, gender, movement_type,"
				" weight_class, height_class, volume_class,"
				" homeland,"
				" strength, dexterity, intelligence, wisdom, constitution,"
					"charisma, strength_add,"
				" save_vs_paralyzation, save_vs_rod, save_vs_petrification,"
					" save_vs_breath, save_vs_spell,"
				" hitpoints_die, hitpoints_multiplier, hitpoints_modifier,"
				" thac0, position,"
				" creators_note)"
				" VALUES "
				"(%u, %Q, %Q, %Q,"
				" %Q, %Q, %Q,"
				" %Q,"
				" %u, %u, %u, %u, %u,"
					" %u, %u,"
				" %d, %d, %d,"
					" %d, %d,"
				" %u, %u, %u,"
				" %d, %Q,"
			  	" %Q)",
				NULL,
				NULL,
				dbErrMsg,
				cstats->virtual_id,
					str_race[cstats->race],
					str_gender[cstats->gender],
					str_movement[cstats->movement_type],
				str_weight_type[cstats->weight_class],
					str_height_type[cstats->height_class],
					str_volume_type[cstats->volume_class],
				str_homeland[cstats->homeland],
				cstats->special.stat[STRENGTH],
					cstats->special.stat[DEXTERITY],
					cstats->special.stat[INTELLIGENCE],
					cstats->special.stat[WISDOM],
					cstats->special.stat[CONSTITUTION],
				cstats->special.stat[CHARISMA],
					cstats->special.stat[STRENGTH_ADD],
				cstats->special.save_vs[SAVE_PARALYZATION],
					cstats->special.save_vs[SAVE_STAVES],
					cstats->special.save_vs[SAVE_PETRIFICATION],
				cstats->special.save_vs[SAVE_BREATH],
					cstats->special.save_vs[SAVE_SPELL],
				cstats->hitpoints.die,
					cstats->hitpoints.multiplier,
					cstats->hitpoints.modifier,
				cstats->thac0,
					str_position[cstats->special.position],
				cstats->creators_note);
	save_tinyworld_char_stats_dmm(cstats);
	save_tinyworld_char_stats_flags(cstats);
}

/** Save all the character types data. */
void save_tinyworld_character_stats(void)
{
	ubyte4 count = 0;

	for (top_t i = 0; i <= char_stats_top; i++)
	{
		save_tinyworld_character_stat(char_stats[i]);
		count++;
	}
}

/** Save a single character data entry. */
void save_tinyworld_character(CHAR_DATA *cdata)
{
	char *dbErrMsg = NULL;

	SQLITE_EXEC_PRINTF(db, "REPLACE INTO char_data "
				"(char_data_id, char_descr_id, char_stats_id,"
				" clan, max_existing, creators_note)"
				" VALUES "
				"(%u, %u, %u,"
				" %Q, %d, %Q)",
				NULL,
				NULL,
				dbErrMsg,
				cdata->virtual_id,
					cdata->descr->virtual_id,
					cdata->stats->virtual_id,
				cdata->clan,
					cdata->limit,
					cdata->creators_note);
}

/** Save all the character data. */
void save_tinyworld_characters(void)
{
	char *dbErrMsg = NULL;
	ubyte4 count = 0;

	for (top_t i = 0; i <= char_data_top; i++)
	{
		save_tinyworld_character(char_data[i]);
		count++;
	}
}

int callback_character_description(void *pArg,
		int argc, char **argv, char **columnName)
{
	assert(argc == 5);

	char_descr_top++;
	REALLOC(char_descr, CHAR_DESCR_DATA *, char_descr_top+1);
	MALLOC(char_descr[char_descr_top], CHAR_DESCR_DATA, 1);

	for (int i = 0; i < argc; i++)
	{
		if (strcmp(columnName[i], "char_descr_id")==0)
		{
			char_descr[char_descr_top]->virtual_id = atoi(argv[i]);
			continue;
		}
		if (strcmp(columnName[i], "proper")==0)
		{
			if (nostr(argv[i]))
			{
				exitlog("/no proper name for cdescr:%u",
						char_descr[char_descr_top]->virtual_id);
			}
			STRDUP(char_descr[char_descr_top]->proper, argv[i]);
			continue;
		}
		if (strcmp(columnName[i], "room")==0)
		{
			STRDUP(char_descr[char_descr_top]->room, argv[i]);
			continue;
		}
		if (strcmp(columnName[i], "general")==0)
		{
			STRDUP(char_descr[char_descr_top]->general, argv[i]);
			continue;
		}
		if (strcmp(columnName[i], "creators_note")==0)
		{
			STRDUP(char_descr[char_descr_top]->creators_note, argv[i]);
			continue;
		}
		exitlog("/column not used/%s", columnName[i]);
	}
	char_descr[char_descr_top]->key = new Words("char_descr");
	char_descr[char_descr_top]->key->load(
			char_descr[char_descr_top]->virtual_id);

	cdescrIndex.set(char_descr[char_descr_top]->virtual_id, char_descr_top);
	return 0;
}

/** load the character descriptions. */
void load_tinyworld_character_descriptions(void)
{
	char *dbErrMsg = NULL;

	SQLITE_EXEC(db, "SELECT * FROM char_descr_data ORDER BY char_descr_id",
				callback_character_description,
				NULL,
				dbErrMsg);
}

int callback_char_stats_flags(void *pArg,
		int argc, char **argv, char **columnName)
{
	CHAR_STATS_DATA *cstats = NULL;

	assert(argc == 2);

	for (int i=0; i < argc; i++)
	{
		if (strcmp(columnName[i], "char_stats_id")==0)
		{
			cstats = char_stats[cstatsIndex.get(argv[i])];
			continue;
		}
		if (strcmp(columnName[i], "flag")==0)
		{
	 		//citizenship flags
			assert(cstats);
			if (strncmp("CITIZEN_", argv[i], 8)==0)
			{
				ADD_BIT(argv[i], str_citizenship, cstats->special.citizenship);
				continue;
			}

			//profession flags
			if (strncmp("PRO_", argv[i], 4)==0)
			{
				ADD_BIT(argv[i], str_profession_f, cstats->profession);
				continue;
			}

			//act flags
			if (strncmp("ACT_", argv[i], 4)==0)
			{
				ADD_BIT(argv[i], str_act_f, cstats->act_flags);
				continue;
			}

			//affects flags
			if (strncmp("AFF_", argv[i], 7)==0)
			{
				ADD_BIT(argv[i], str_affect_f, cstats->special.affect_flags);
				continue;
			}

			//resistiance flags
			if (strncmp("RESIST_", argv[i], 7)==0)
			{
				ADD_BIT(argv[i], str_resistance_f,
						cstats->special.resistance_flags);
				continue;
			}

			//immunity flags
			if (strncmp("IMMUNE_", argv[i], 7)==0)
			{
				ADD_BIT(argv[i], str_immunity_f, cstats->special.immunity_flags);
				continue;
			}

			// flag not found
			exitlog("/flag not categorized/%s", argv[i]);
		}
		exitlog("/column not used/%s", columnName[i]);
	}
	return 0;
}

/**  This function assumes that attack_id will be listed prior to all other
     attack information.
 */
int callback_char_stats_dmm(void *pArg,
		int argc, char **argv, char **columnName)
{
	CHAR_STATS_DATA *cstats = NULL;
	top_t top = -1;

	assert(argc == 6);

	for (int i = 0; i < argc; i++)
	{
		if (strcmp(columnName[i], "char_stats_id")==0)
		{
			cstats = char_stats[cstatsIndex.get(argv[i])];
			continue;
		}
		if (strcmp(columnName[i], "attack_id")==0)
		{
			top = atoi(argv[i]);
			assert(cstats);

			REALLOC(cstats->attack, ATTACK, top+1);
			cstats->attack_top = top;
			continue;
		}
		if (strcmp(columnName[i], "attack_type")==0)
		{
			assert(top != -1);
			assert(cstats);
			cstats->attack[top].type = get_index(argv[i], str_attack_type);
			continue;
		}
		if (strcmp(columnName[i], "die")==0)
		{
			assert(top != -1);
			assert(cstats);
			cstats->attack[top].damage.die = atoi(argv[i]);
			continue;
		}
		if (strcmp(columnName[i], "multiplier")==0)
		{
			assert(top != -1);
			assert(cstats);
			cstats->attack[top].damage.multiplier = atoi(argv[i]);
			continue;
		}
		if (strcmp(columnName[i], "modifier")==0)
		{
			assert(top != -1);
			assert(cstats);
			cstats->attack[top].damage.modifier = atoi(argv[i]);
			continue;
		}
		exitlog("/column not used/%s", columnName[i]);
	}
	return 0;
}

int callback_character_stats(void *pArg,
		int argc, char **argv, char **columnName)
{
	char_stats_top++;
	REALLOC(char_stats, CHAR_STATS_DATA *, char_stats_top+1);
	CALLOC(char_stats[char_stats_top], CHAR_STATS_DATA, 1);
	char_stats[char_stats_top]->attack_top = -1;

	assert(argc == 26);

	for (int i = 0; i < argc; i++)
	{
		if (strcmp(columnName[i], "char_stats_id")==0)
		{
			char_stats[char_stats_top]->virtual_id = atoi(argv[i]);
			continue;
		}
		if (strcmp(columnName[i], "race")==0)
		{
			char_stats[char_stats_top]->race = get_index(argv[i], str_race);
			continue;
		}
		if (strcmp(columnName[i], "gender")==0)
		{
			char_stats[char_stats_top]->gender = get_index(argv[i], str_gender);
			continue;
		}
		if (strcmp(columnName[i], "movement_type")==0)
		{
			char_stats[char_stats_top]->movement_type
				= get_index(argv[i], str_movement);
			continue;
		}
		if (strcmp(columnName[i], "attack_top")==0) //MARK get rid of this?
		{
			char_stats[char_stats_top]->attack_top = atoi(argv[i]);
			continue;
		}
		if (strcmp(columnName[i], "weight_class")==0)
		{
			char_stats[char_stats_top]->weight_class
				= get_index(argv[i], str_weight_type);
			continue;
		}
		if (strcmp(columnName[i], "height_class")==0)
		{
			char_stats[char_stats_top]->height_class
				= get_index(argv[i], str_height_type);
			continue;
		}
		if (strcmp(columnName[i], "volume_class")==0)
		{
			char_stats[char_stats_top]->volume_class
				= get_index(argv[i], str_volume_type);
			continue;
		}
		if (strcmp(columnName[i], "homeland")==0)
		{
			char_stats[char_stats_top]->homeland
						= get_index(argv[i], str_homeland);
			continue;
		}
		if (strcmp(columnName[i], "strength")==0)
		{
			char_stats[char_stats_top]->special.stat[STRENGTH] = atoi(argv[i]);
			continue;
		}
		if (strcmp(columnName[i], "dexterity")==0)
		{
			char_stats[char_stats_top]->special.stat[DEXTERITY] = atoi(argv[i]);
			continue;
		}
		if (strcmp(columnName[i], "intelligence")==0)
		{
			char_stats[char_stats_top]->special.stat[INTELLIGENCE] = atoi(argv[i]);
			continue;
		}
		if (strcmp(columnName[i], "wisdom")==0)
		{
			char_stats[char_stats_top]->special.stat[WISDOM] = atoi(argv[i]);
			continue;
		}
		if (strcmp(columnName[i], "constitution")==0)
		{
			char_stats[char_stats_top]->special.stat[CONSTITUTION] = atoi(argv[i]);
			continue;
		}
		if (strcmp(columnName[i], "charisma")==0)
		{
			char_stats[char_stats_top]->special.stat[CHARISMA] = atoi(argv[i]);
			continue;
		}
		if (strcmp(columnName[i], "strength_add")==0)
		{
			char_stats[char_stats_top]->special.stat[STRENGTH_ADD] = atoi(argv[i]);
			continue;
		}
		if (strcmp(columnName[i], "save_vs_paralyzation")==0)
		{
			char_stats[char_stats_top]->special.save_vs[SAVE_PARALYZATION]
				= atoi(argv[i]);
			continue;
		}
		if (strcmp(columnName[i], "save_vs_rod")==0)
		{
			char_stats[char_stats_top]->special.save_vs[SAVE_STAVES]
				= atoi(argv[i]);
			continue;
		}
		if (strcmp(columnName[i], "save_vs_petrification")==0)
		{
			char_stats[char_stats_top]->special.save_vs[SAVE_PETRIFICATION]
				= atoi(argv[i]);
			continue;
		}
		if (strcmp(columnName[i], "save_vs_breath")==0)
		{
			char_stats[char_stats_top]->special.save_vs[SAVE_BREATH] = atoi(argv[i]);
			continue;
		}
		if (strcmp(columnName[i], "save_vs_spell")==0)
		{
			char_stats[char_stats_top]->special.save_vs[SAVE_SPELL]
				= atoi(argv[i]);
			continue;
		}
		if (strcmp(columnName[i], "thac0")==0)
		{
			char_stats[char_stats_top]->thac0 = atoi(argv[i]);
			continue;
		}
		if (strcmp(columnName[i], "hitpoints_die")==0)
		{
			char_stats[char_stats_top]->hitpoints.die = atoi(argv[i]);
			continue;
		}
		if (strcmp(columnName[i], "hitpoints_multiplier")==0)
		{
			char_stats[char_stats_top]->hitpoints.multiplier = atoi(argv[i]);
			continue;
		}
		if (strcmp(columnName[i], "hitpoints_modifier")==0)
		{
			char_stats[char_stats_top]->hitpoints.modifier = atoi(argv[i]);
			continue;
		}
		if (strcmp(columnName[i], "position")==0)
		{
			char_stats[char_stats_top]->special.position
						= get_index(argv[i], str_position);
			continue;
		}
		if (strcmp(columnName[i], "creators_note")==0)
		{
			STRDUP(char_stats[char_stats_top]->creators_note, argv[i]);
			continue;
		}
		exitlog("/column not used/%s", columnName[i]);
	}

	cstatsIndex.set(char_stats[char_stats_top]->virtual_id, char_stats_top);
	return 0;
}

/** load the tinyworld character types. */
static void load_tinyworld_character_stats(void)
{
	char *dbErrMsg = NULL;

	SQLITE_EXEC(db, "SELECT * FROM char_stats_data ORDER BY char_stats_id",
				callback_character_stats,
				NULL,
				dbErrMsg);
	SQLITE_EXEC_PRINTF(db, "SELECT * FROM char_stats_flags",

				callback_char_stats_flags,
				NULL,
				dbErrMsg);
	SQLITE_EXEC_PRINTF(db, "SELECT * FROM char_stats_dmm",
				callback_char_stats_dmm,
				NULL,
				dbErrMsg);
}

/**
  \todo rename char_descr_id to char_descr_id like with char_statss_data_id
  */
int callback_character_data(void *pArg,
		int argc, char **argv, char **columnName)
{
	char_data_top++;
	REALLOC(char_data, CHAR_DATA *, char_data_top+1);
	MALLOC(char_data[char_data_top], CHAR_DATA, 1);
	char_data[char_data_top]->count = 0;

	assert(argc == 6);

	for (int i = 0; i < argc; i++)
	{
		if (strcmp(columnName[i], "char_data_id")==0)
		{
			char_data[char_data_top]->virtual_id = atoi(argv[i]);
			continue;
		}
		if (strcmp(columnName[i], "char_descr_id")==0)
		{
			char_data[char_data_top]->descr
				= char_descr[cdescrIndex.get(atoi(argv[i]))];
			continue;
		}
		if (strcmp(columnName[i], "char_stats_id")==0)
		{
			char_data[char_data_top]->stats
				= char_stats[cstatsIndex.get(atoi(argv[i]))];
			continue;
		}
		if (strcmp(columnName[i], "clan")==0)
		{
			char_data[char_data_top]->clan = nostr(argv[i]) ? 0 : atoi(argv[i]);
			continue;
		}
		if (strcmp(columnName[i], "max_existing")==0)
		{
			char_data[char_data_top]->limit = atoi(argv[i]);
			continue;
		}
		if (strcmp(columnName[i], "creators_note")==0)
		{
			STRDUP(char_data[char_data_top]->creators_note, argv[i]);
			continue;
		}
		exitlog("/column not used/%s", columnName[i]);
	}

	cdataIndex.set(char_data[char_data_top]->virtual_id, char_data_top);
	return 0;
}

void free_tinyworld_character_description(CHAR_DESCR_DATA *cdescr)
{
	delete cdescr->key;
	free(cdescr->proper);
	free(cdescr->room);
	free(cdescr->general);
	free(cdescr->creators_note);
	free(cdescr);
}

static void free_tinyworld_character_descriptions(void)
{
	for (top_t i = 0; i <= char_descr_top; i++)
	{
		free_tinyworld_character_description(char_descr[i]);
	}
	FREE(char_descr);
	char_descr_top = -1;
	cdescrIndex.clear();
}

void free_tinyworld_character_stat(CHAR_STATS_DATA *cstats)
{
	free(cstats->creators_note);
	free(cstats->attack);
	free(cstats);
}

static void free_tinyworld_character_stats(void)
{
	for (top_t i = 0; i <= char_stats_top; i++)
	{
		free_tinyworld_character_stat(char_stats[i]);
	}
	FREE(char_stats);
	char_stats_top = -1;
	cstatsIndex.clear();
}

void free_tinyworld_character(CHAR_DATA *cdata)
{
	free(cdata->creators_note);
	free(cdata);
}

/** Free the tinyworld characters. */
static void free_tinyworld_characters(void)
{
	for (top_t i = 0; i <= char_data_top; i++)
	{
		free_tinyworld_character(char_data[i]);
	}
	FREE(char_data);
	char_data_top = -1;
	cdataIndex.clear();
}

/** Load the tinyworld characters. */
static void load_tinyworld_characters(void)
{
	char *dbErrMsg = NULL;

	SQLITE_EXEC(db, "SELECT * FROM char_data ORDER BY char_data_id",
				callback_character_data,
				NULL,
				dbErrMsg);
}

/** Initialize the tinyworld character module. */
void init_tinyworld_character_module(void)
{
	char *dbErrMsg = NULL;
	create_tinyworld_character_descriptions();
	create_tinyworld_characters();

	BEGIN_TRANSACTION(db, dbErrMsg);
	save_tinyworld_character_descriptions();
	save_tinyworld_character_stats();
	save_tinyworld_characters();
	COMMIT_TRANSACTION(db, dbErrMsg);
	
	free_tinyworld_character_descriptions();
	free_tinyworld_character_stats();
	free_tinyworld_characters();
	load_tinyworld_character_descriptions();
	load_tinyworld_character_stats();
	load_tinyworld_characters();
}

/** This is where the characters get their points updated.
    \param ctx a pointer to the character
 */
void point_update_queue(void *ctx)
{
	CHARACTER_CTX *chCTX = (CHARACTER_CTX *) ctx;

	if (chCTX->character == NULL)
	{
		free(chCTX);
		return;
	}
	chCTX->character->pointUpdate();
	queue_event(POINT_UPDATE_TIMEs, POINT_UPDATE_TIMEus,
			point_update_queue, chCTX);
}

Character::Character(Descriptor *d, cid_t container_id)
: Container(CONTAINER_CHARACTER, container_id)
{
	descriptor = d;
	birth = time0;
	data = NULL;
	Imm = NULL;

	hitpoints_max = hitpoints = 10; //MARK

	MALLOC(pointUpdateCTX, CHARACTER_CTX, 1);
	pointUpdateCTX->character = this;

	queue_event(game_stat.characters_created % POINT_UPDATE_TIMEs,
			point_update_queue, pointUpdateCTX);

	prev_character = NULL;
	if (character_list == NULL)
	{
		next_character = NULL;
	}
	else
	{
		next_character = character_list;
		character_list->prev_character = this;
	}
	character_list = this;

	game_stat.characters_created++;
}

Character::Character(CHAR_DATA *cdata) : Container(CONTAINER_CHARACTER, 0)
{
	descriptor = NULL;
	birth = time0;
	data = cdata;
	Imm = NULL;

	hitpoints_max = hitpoints = 10; //MARK

	MALLOC(pointUpdateCTX, CHARACTER_CTX, 1);
	pointUpdateCTX->character = this;

	point_update_queue(pointUpdateCTX);

	MALLOC(combatCTX, CHARACTER_CTX, 1);
	combatCTX->character = this;

	prev_character = NULL;
	if (character_list == NULL)
	{
		next_character = NULL;
	}
	else
	{
		next_character = character_list;
		character_list->prev_character = this;
	}
	character_list = this;

	game_stat.characters_created++;

	fight = NULL;
}

Character::~Character(void)
{
	if (descriptor)
	{
		descriptor->extract();
	}
	if (Imm)
	{
		delete Imm;
	}
	if (inContainer())
	{
		extract();
	}

	data = NULL;
	pointUpdateCTX->character = NULL;

	game_stat.characters_destroyed++;

	if (prev_character)
	{
		prev_character->next_character = next_character;
	}
	else
	{
		character_list = next_character;
	}
	if (next_character)
	{
		next_character->prev_character = prev_character;
	}
}

/** If the character is fighting, return who the victim is.
    \return a pointer to the victim character
 */
Character *Character::fighting(void)
{
	if (fight && fight->victCTX && fight->victCTX->character)
	{
		return fight->victCTX->character;
	}
	return NULL;
}

/** Sets a victim that this character is fighting
    \param vict a pointer to the victim character
 */
void Character::setFighting(Character *vict)
{
	if (vict == NULL)
	{
		fight = NULL;
		return;
	}
	if (fighting())
	{
		return;
	}
	assert(vict->isDead()==0);
	fight = add_fight(this, vict);
}

/** Removes the character from the game and place him in the post game
     menu.
 */
void Character::extract(void)
{
	assert(inContainer());
	Container::extract();

	combatCTX->character = NULL;
	free_combatCTX(combatCTX);
	combatCTX = NULL;

	if (getDescriptor())
	{
		if (getDescriptor()->envFlag(DENV_SPLITSCREEN))
		{
			getDescriptor()->finitSplitScreen();
		}
		getDescriptor()->handleStateMotd();
	}
}

void Character::insert(vid_t room_vid)
{
	void count_bugs(Character *);
	void count_fixes(Character *);
	addlog_in_player(this);
	assert(inContainer()==NULL);
	put(K_GRN "\r\nWelcome to the Forgotten World. May your visit here be... Interesting.\r\n" K_WHT);
	count_bugs(this);
	count_fixes(this);
	put("\r\n");
	while (getHitpoints() <= 0)
	{
		addHitpoints(dice(1, 3));
	}
	moveTo(room_vid);
	MALLOC(combatCTX, CHARACTER_CTX, 1);
	combatCTX->character = this;
	fight = NULL;
}

/** Handles moving characters from one room to another with messaging.
    \param room_to The destination room, this must exist
	 \param cmd the command type id which the extraction and insertion messages
	 are based upon
 */
void Character::moveTo(Room *room_to, cmd_t cmd)
{
	int path=0;
	assert(room_to);
	if (inContainer())
	{
		if (cmd == CMD_GOTO)
		{
			inContainer()->echo(this, "$n disappears in a puff of smoke.");
		}
		else if (cmd < NO_DIR)
		{
			if ((path=(room_to->getRoomDir(revdir[cmd]) == inContainer())))
			{
				inContainer()->echo(this, "$g$n %s %s.",
						getMovement(),
						str_dir[cmd]);
			}
			else
			{
				inContainer()->echo(this, "$g$n disappears towards the %s.",
						str_dir[cmd]);
			}
		}
		else
		{
			inContainer()->echo(this, "$g$n leaves.");
		}
	}

	room_to->hold(this);

	if (cmd == CMD_GOTO)
	{
		room_to->echo(this, "$n appears with an ear-splitting bang.");
	}
	else if (cmd < NO_DIR)
	{
		if (path)
		{
			inContainer()->echo(this, "$g$n %s in from the %s.",
					getMovement(),
					str_dir[revdir[cmd]]);
		}
		else
		{
			inContainer()->echo(this, "$g$n appears from the %s.",
					str_dir[revdir[cmd]]);
		}
	}
	else
	{
		room_to->echo(this, "$g$n arrives.");
	}
	look();
}

/** Performs a look for the character in the room he is in */
void Character::look(void)
{
	if (descriptor == NULL)
	{
		return;
	}

	assert(inContainer());

	if (Imm)
	{
		Room *room;
	  	if ((room	= inContainer()->objRoom()))
		{
			print(K_BLD_GRN "%s [%lu]\r\n" K_YEL "%s",
					room->getName(),
					room->getVID(),
					room->getDescr());
		}
		else
		{
			print(K_BLD_GRN "%s [C#%08lX]\r\n" K_YEL "%s",
					inContainer()->getName(),
					inContainer()->getCID(),
					inContainer()->getDescr());
		}
	}
	else
	{
		print(K_BLD_GRN "%s\r\n" K_YEL "%s",
				inContainer()->getName(),
				inContainer()->getDescr());
	}
	inContainer()->listItemsTo(this);
	inContainer()->listCharactersTo(this);
}

void Character::look(Container *c)
{
	assert(c);

	echo(c, "$G%s" K_WHT, c->getDescr());
}

/** Echo a message to the character.
    \param ch The subject of the message
	 \param vict The victim of the message
	 \param format the format of the message
	 \param ... variable completion for the format
    \todo Check to see if malloc messages work and apply to descriptor version
 */
void Character::echo(Container *subj, Container *vict, const char *format, ...)
{
	if (descriptor == NULL)
	{
		return;
	}

	int n, size = CHUNK_SIZE_OUT;
	int is_complete = 0;
	char *msg_s;
	char msg[CHUNK_SIZE_OUT];
	va_list vargs;
	char *fmt_s;

	msg_s = NULL;

	while (1)
	{
		va_start(vargs, format);
		n = vsnprintf(msg_s ? msg_s : msg, size, format, vargs);
		va_end(vargs);

		msg_s = va_complete(&is_complete, msg_s, msg, &size, n);
		if (is_complete)
		{
			break;
		}
	}

	comm(this, subj, vict, msg_s ? msg_s : msg, 2);

	if (msg_s)
	{
		free(msg_s);
	}
}

/** Echo a message to the character. Assumes the character is the subject.
	 \param vict The victim of the message
	 \param format the format of the message
	 \param ... variable completion for the format
 */
void Character::echo(Container *vict, const char *format, ...)
{
	if (descriptor == NULL)
	{
		return;
	}

	int n, size = CHUNK_SIZE_OUT;
	int is_complete = 0;
	char *msg_s;
	char msg[CHUNK_SIZE_OUT];
	va_list vargs;
	char *fmt_s;

	msg_s = NULL;

	while (1)
	{
		va_start(vargs, format);
		n = vsnprintf(msg_s ? msg_s : msg, size, format, vargs);
		va_end(vargs);

		msg_s = va_complete(&is_complete, msg_s, msg, &size, n);
		if (is_complete)
		{
			break;
		}
	}

	comm(this, NULL, vict, msg_s ? msg_s : msg, 2);

	if (msg_s)
	{
		free(msg_s);
	}
}

/** Echo a message to the character.  Assumes the character is the subject
    and victim of the message.
	 \param format the format of the message
	 \param ... variable completion for the format
 */
void Character::echo(const char *format, ...)
{
	if (descriptor == NULL)
	{
		return;
	}

	int n, size = CHUNK_SIZE_OUT;
	int is_complete = 0;
	char *msg_s;
	char msg[CHUNK_SIZE_OUT];
	va_list vargs;

	msg_s = NULL;

	while (1)
	{
		va_start(vargs, format);
		n = vsnprintf(msg_s ? msg_s : msg, size, format, vargs);
		va_end(vargs);

		msg_s = va_complete(&is_complete, msg_s, msg, &size, n);

		if (is_complete)
		{
			break;
		}
	}

	comm(this, NULL, NULL, msg_s ? msg_s : msg, 2);

	if (msg_s)
	{
		free(msg_s);
	}
}

/** Echo a message to the target.  Assumes the character is the subject of
    the message.  This differs from message by handling the newline prior to
	 the message and the trailing newline when the messages end.
    \param targ The target of the message
	 \param vict The victim of the message
	 \param format the format of the message
	 \param ... variable completion for the format
 */
void Character::echoTo(Container *targ, Container *vict,
		const char *format, ...)
{
	assert(targ);
	if (targ->getDescriptor() == NULL)
	{
		return;
	}

	int n, size = CHUNK_SIZE_OUT;
	int is_complete = 0;
	char *msg_s;
	char msg[CHUNK_SIZE_OUT];
	va_list vargs;
	char *fmt_s;

	msg_s = NULL;

	while (1)
	{
		va_start(vargs, format);
		n = vsnprintf(msg_s ? msg_s : msg, size, format, vargs);
		va_end(vargs);

		msg_s = va_complete(&is_complete, msg_s, msg, &size, n);
		if (is_complete)
		{
			break;
		}
	}

	comm(targ, this, vict, format, 3);
	if (targ->getDescriptor())
	{
		targ->getDescriptor()->setToGamePrompt();
	}

	if (msg_s)
	{
		free(msg_s);
	}
}

/** Echo a message to the victim.  Assumes the character is the subject of
    the message.  This differs from message by handling the newline prior to
	 the message and the trailing newline when the messages end.
	 \param vict The victim of the message
	 \param format the format of the message
	 \param ... variable completion for the format
 */
void Character::echoTo(Container *vict, const char *format, ...)
{
	assert(vict);
	if (vict->getDescriptor() == NULL)
	{
		return;
	}

	int n, size = CHUNK_SIZE_OUT;
	int is_complete = 0;
	char *msg_s;
	char msg[CHUNK_SIZE_OUT];
	va_list vargs;
	char *fmt_s;

	msg_s = NULL;

	while (1)
	{
		va_start(vargs, format);
		n = vsnprintf(msg_s ? msg_s : msg, size, format, vargs);
		va_end(vargs);

		msg_s = va_complete(&is_complete, msg_s, msg, &size, n);
		if (is_complete)
		{
			break;
		}
	}

	comm(vict, this, NULL, msg_s ? msg_s : msg, 3);
	if (vict->getDescriptor())
	{
		vict->getDescriptor()->setToGamePrompt();
	}

	if (msg_s)
	{
		free(msg_s);
	}
}

/** Echo a message to the victim.  Assumes the character is the subject and
    the victim of the the message.  This differs from message by handling
	 the newline prior to the message and the trailing newline when the messages
	 end.
	 \param format the format of the message
	 \param ... variable completion for the format
 */
void Character::echoTo(const char *format, ...)
{
	int n, size = CHUNK_SIZE_OUT;
	int is_complete = 0;
	char *msg_s;
	char msg[CHUNK_SIZE_OUT];
	va_list vargs;
	char *fmt_s;

	msg_s = NULL;

	while (1)
	{
		va_start(vargs, format);
		n = vsnprintf(msg_s ? msg_s : msg, size, format, vargs);
		va_end(vargs);

		msg_s = va_complete(&is_complete, msg_s, msg, &size, n);
		if (is_complete)
		{
			break;
		}
	}

	comm(this, NULL, NULL, msg_s ? msg_s : msg, 3);
	getDescriptor()->setToGamePrompt();

	if (msg_s)
	{
		free(msg_s);
	}
}

/** This echos unfiltered text to the character.
    \param format The string message format
	 \param ... The variable list of arguments
 */
void Character::print(const char *mesg, ...)
{
	int n, size = CHUNK_SIZE_OUT;
	int is_complete = 0;
	char *msg_s = NULL;
	char msg[CHUNK_SIZE_OUT];
	va_list vargs;

	while (1)
	{
		va_start(vargs, mesg);
		n = vsnprintf(msg_s ? msg_s : msg, size, mesg, vargs);
		va_end(vargs);
		msg_s = va_complete(&is_complete, msg_s, msg, &size, n);
		if (is_complete)
		{
			break;
		}
	}
	put(msg_s ? msg_s : msg);
	if (msg_s);
	{
		free(msg_s);
	}
}

/** This directly calls upons the Descriptor::put() method.  It echos
    unformatted text to the character.
*/
void Character::put(const char *mesg)
{
	if (descriptor == NULL)
	{
		return;
	}
	descriptor->put(mesg);
}

void Character::m_put(char *mesg)
{
	if (descriptor == NULL)
	{
		return;
	}
	descriptor->m_put(mesg);
}

void Character::writer(char **msgptr)
{
	assert(descriptor);
	descriptor->writer(msgptr);
}

void Character::writer(char **msgptr, void (*fn)(void *), void *ctx)
{
	assert(descriptor);
	descriptor->writer(msgptr, fn, ctx);
}

void Character::cr1LWriter(char **msgptr)
{
	assert(descriptor);
	descriptor->setCr1LWriter();
	descriptor->writer(msgptr);
}

/** Sends comm formatted messages to Characters
    \param to The message to this Character, this must always be set
	 \param ch The subject Character, if this is not set, then the subject
	  becomes the to-Character
	 \param vict The victim Character, if this is not set, then the victim
	  becomes the to-Character
	 \param format The encoded string message
    \param flag This describes which type of comm this is:
	 - 0 - echoNo, no returns before or after the message, like put()
	 - 1 - echoFr, returns before and after the message, unsolicited messages
	 - 2 - echo, return only after the message, solicited messages
	 - 3 - echoTo, return before the message, compounded unsolicited messages
 */
void comm(Container *to, Container *subj, Container *vict,
		const char *format, int flag)
{
	assert(to);
	if (to->hasNoListener())
	{
		return;
	}
	if (vict == NULL)
	{
		vict = to;
	}
	if (subj == NULL)
	{
		subj = to;
	}
	char *fmt_s = comm(format, subj, vict);
	if (flag == 1 || flag == 3)
	{
		to->put("\r\n");
	}
	to->put(fmt_s);
	if (flag == 1 || flag == 2)
	{
		to->put("\r\n");
	}
	comm(NULL, NULL, NULL);
}

/** Send the character a prompt */
void Character::prompt(void)
{
	assert(descriptor);
	if (descriptor->envFlag(DENV_COMPACT)==0)
	{
		put("\r\n");
	}
	descriptor->print(K_GRN "%s<%s %uh%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s> %s" K_WHT,
			getDescriptor()->envFlag(DENV_SPLITSCREEN) ? "--- " : "",
			cmclock(),
			getHitpoints(),

			Imm ? Imm->cr_help ? " Hlp:" :"":"",
			Imm ? Imm->cr_help ? "\'" :"":"",
			Imm ? Imm->cr_help ? Imm->cr_help->keyword :"":"",
			Imm ? Imm->cr_help ? "\'" :"":"",

			Imm ? Imm->cr_wizhelp ? " WzHlp:" :"":"",
			Imm ? Imm->cr_wizhelp ? "\'" :"":"",
			Imm ? Imm->cr_wizhelp ? Imm->cr_wizhelp->keyword :"":"",
			Imm ? Imm->cr_wizhelp ? "\'" :"":"",

			Imm ? Imm->cr_social ? " Soc:" :"":"",
			Imm ? Imm->cr_social ? Imm->cr_social->verb :"":"",

			Imm ? Imm->cr_cdata ? " C-V#" :"":"",
			Imm ? Imm->cr_cdata ? itoa(Imm->cr_cdata->virtual_id) :"":"",

			Imm ? Imm->cr_cdescr ? " C-D#" :"":"",
			Imm ? Imm->cr_cdescr ? itoa(Imm->cr_cdescr->virtual_id) :"":"",

			Imm ? Imm->cr_cstats ? " C-S#" :"":"",
			Imm ? Imm->cr_cstats ? itoa(Imm->cr_cstats->virtual_id) :"":"",

			Imm ? Imm->cr_idata ? " I-V#" :"":"",
			Imm ? Imm->cr_idata ? itoa(Imm->cr_idata->virtual_id) :"":"",

			Imm ? Imm->cr_idescr ? " I-D#" :"":"",
			Imm ? Imm->cr_idescr ? itoa(Imm->cr_idescr->virtual_id) :"":"",

			Imm ? Imm->cr_istats ? " I-S#" :"":"",
			Imm ? Imm->cr_istats ? itoa(Imm->cr_istats->virtual_id) :"":"",

			Imm ? Imm->cr_room ? " R#" :"":"",
			Imm ? Imm->cr_room ? itoa(Imm->cr_room->virtual_id) :"":"",

			Imm ? Imm->cr_zone ? " Z#" :"":"",
			Imm ? Imm->cr_zone ? itoa(Imm->cr_zone->virtual_id) :"":"",

			getDescriptor()->envFlag(DENV_SPLITSCREEN) ?
			"---------------------" : "");
}

/** This method is responsible for calculating the points a character has to
    be updated at the point_update time.  If the character receives new
	 points then they should get a prompt informing them.
 */
void Character::pointUpdate(void)
{
	if (hitpoints < hitpoints_max)
	{
		hitpoints++;
		put("\r\n"); //send the character a new prompt
	}
}

ImmortalHelper::ImmortalHelper(immortal_type_t l) : level(l)
{
	if (level == IMPLEMENTOR)
	{
		command_flags = (cmd_flags_t) -1;
	}
	else if (level == DEUCE)
	{
		command_flags = (cmd_flags_t) -1;
		RESET(f_(CMD_GRANT), command_flags);
		RESET(f_(CMD_SHUTDOWN), command_flags);
	}
	else
	{
		command_flags = 0;
	}
	cr_help = NULL;
	cr_wizhelp = NULL;
	cr_social = NULL;
	cr_istats = NULL;
	cr_idescr = NULL;
	cr_idata = NULL;
	cr_cstats = NULL;
	cr_cdescr = NULL;
	cr_cdata = NULL;
	cr_room = NULL;
	cr_zone = NULL;
}

ImmortalHelper::~ImmortalHelper(void)
{
	if (cr_help)
	{
		free(cr_help->entry);
		free(cr_help);
	}
	if (cr_wizhelp)
	{
		free(cr_wizhelp->entry);
		free(cr_wizhelp);
	}
	if (cr_social)
	{
		free_social(cr_social);
	}
	if (cr_cdata)
	{
		free_tinyworld_character(cr_cdata);
	}
	if (cr_idata)
	{
		free_tinyworld_item(cr_idata);
	}
	if (cr_cdescr)
	{
		free_tinyworld_character_description(cr_cdescr);
	}
	if (cr_idescr)
	{
		free_tinyworld_item_description(cr_idescr);
	}
	if (cr_cstats)
	{
		free_tinyworld_character_stat(cr_cstats);
	}
	if (cr_istats)
	{
		free_tinyworld_item_stat(cr_istats);
	}
	if (cr_room)
	{
		free_tinyworld_room(cr_room);
	}
}

Character *create_character(vid_t virtual_id, cid_t loader,
		Container *container)
{
	int rn;
	CHAR_DATA *cdata;
	Character *ch;

		if ((rn = cdataIndex.get(virtual_id)) < 0)
		{
			return NULL;
		}
		cdata = char_data[rn];
		ch = new Character(cdata);
		addlog_new_character(ch, loader);
		container->hold(ch);
		return ch;
}

void add_char_data(CHAR_DATA *cdata)
{
	vid_t i;
	if ((i = cdataIndex.get(cdata->virtual_id)) == (vid_t) -1)
	{
		char_data_top++;
		REALLOC(char_data, CHAR_DATA *, char_data_top+1);
		char_data[char_data_top] = cdata;
		cdataIndex.set(cdata->virtual_id, char_data_top);
		return;
	}
	cdata->count = char_data[i]->count;
	free_tinyworld_character(char_data[i]);
	char_data[i] = cdata;
}

void add_char_descr(CHAR_DESCR_DATA *cdescr)
{
	vid_t i;
	if ((i = cdescrIndex.get(cdescr->virtual_id)) == (vid_t) -1)
	{
		char_descr_top++;
		REALLOC(char_descr, CHAR_DESCR_DATA *, char_descr_top+1);
		char_descr[char_descr_top] = cdescr;
		cdescrIndex.set(cdescr->virtual_id, char_descr_top);
		return;
	}
	free_tinyworld_character_description(char_descr[i]);
	char_descr[i] = cdescr;
}

void add_char_stats(CHAR_STATS_DATA *cstats)
{
	vid_t i;
	if ((i = cstatsIndex.get(cstats->virtual_id)) == (vid_t) -1)
	{
		char_stats_top++;
		REALLOC(char_stats, CHAR_STATS_DATA *, char_stats_top+1);
		char_stats[char_stats_top] = cstats;
		cstatsIndex.set(cstats->virtual_id, char_stats_top);
		return;
	}
	free_tinyworld_character_stat(char_stats[i]);
	char_stats[i] = cstats;
}

void destroy_tinyworld_character(vid_t i)
{
	cdataIndex.reset(i);
	char_data_top--;
	while ((sbyte4) i <= char_data_top)
	{
		char_data[i] = char_data[++i];
	}
}

void destroy_tinyworld_character_stat(vid_t i)
{
	cstatsIndex.reset(i);
	char_stats_top--;
	while ((sbyte4) i <= char_stats_top)
	{
		char_stats[i] = char_stats[++i];
	}
}

void destroy_tinyworld_character_description(vid_t i)
{
	cdescrIndex.reset(i);
	char_descr_top--;
	while ((sbyte4) i <= char_descr_top)
	{
		char_descr[i] = char_descr[++i];
	}
}

void copy_tinyworld_character(CHAR_DATA *cdata, vid_t i)
{
	cdata->virtual_id = char_data[i]->virtual_id;
	cdata->descr = char_data[i]->descr;
	cdata->stats = char_data[i]->stats;
	STRDUP(cdata->creators_note, char_data[i]->creators_note);
	cdata->clan = char_data[i]->clan;
	cdata->limit = char_data[i]->limit;
	cdata->count = 0;
}

void copy_tinyworld_character_description(CHAR_DESCR_DATA *cdescr, vid_t i)
{
	cdescr->virtual_id = char_descr[i]->virtual_id;
	cdescr->key = new Words(char_descr[i]->key);
	STRDUP(cdescr->proper, char_descr[i]->proper);
	STRDUP(cdescr->room, char_descr[i]->room);
	STRDUP(cdescr->general, char_descr[i]->general);
	STRDUP(cdescr->creators_note, char_descr[i]->creators_note);
}

void copy_tinyworld_character_stat(CHAR_STATS_DATA *cstats, vid_t i)
{
	memcpy(cstats, char_stats[i], sizeof(CHAR_STATS_DATA));
	STRDUP(cstats->creators_note, char_stats[i]->creators_note);
	MALLOC(cstats->attack, ATTACK, cstats->attack_top+1);
	memcpy(cstats->attack, char_stats[i]->attack,
			sizeof(ATTACK)*(cstats->attack_top+1));
}

CHAR_DESCR_DATA *find_char_descr(vid_t id)
{
	if ((id = cdescrIndex.get(id)) == (vid_t) -1)
	{
		return NULL;
	}
	return char_descr[id];
}

CHAR_STATS_DATA *find_char_stats(vid_t id)
{
	if ((id = cstatsIndex.get(id)) == (vid_t) -1)
	{
		return NULL;
	}
	return char_stats[id];
}
