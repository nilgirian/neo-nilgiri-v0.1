/* **************************************************************************
    Nilgiri the Forgotten World
    (C) 2004
   ************************************************************************** */
/** \file
    \brief general modify game commands module
 */
#include<stdio.h>
#include"utils.h"
#include"tinyworld_character.h"
#include"library.h"
#include"addlog.h"

void cr_new_help(Character *ch, char *keyword) {
	if (ch->Imm->cr_help)
	{
		ch->put("You have a help entry in your buffer already.\r\n");
		return;
	}
	if (keyword)
	{
		to_upperstr(keyword);
		if (helpTable->exists(keyword))
		{
			ch->print("Keyword '%s' already exists in the help.\r\n", keyword);
			return;
		}
		CALLOC(ch->Imm->cr_help, HELP_ENTRY, 1);
		STRDUP(ch->Imm->cr_help->keyword, keyword);
		ch->put("Ok\r\n");
		return;
	}
	ch->put("@new help <keyword>\r\n");
}

void cr_new_wizhelp(Character *ch, char *keyword)
{
	if (ch->Imm->cr_wizhelp)
	{
		ch->put("You have a wizhelp entry in your buffer already.\r\n");
		return;
	}
	if (keyword)
	{
		to_upperstr(keyword);
		if (immhelpTable->exists(keyword))
		{
			ch->print("Keyword '%s' already exists in the help.\r\n", keyword);
			return;
		}
		CALLOC(ch->Imm->cr_wizhelp, HELP_ENTRY, 1);
		STRDUP(ch->Imm->cr_wizhelp->keyword, keyword);
		ch->put("Ok\r\n");
		return;
	}
	ch->put("@new wizhelp <keyword>\r\n");
}

void cr_new_social(Character *ch, char *verb)
{
	if (ch->Imm->cr_social)
	{
		ch->put("You have a social in your buffer already.\r\n");
		return;
	}
	if (verb)
	{
		to_lowerstr(verb);
		if (social_exists(verb))
		{
			ch->print("Verb '%s' already exists in the socials.\r\n", verb);
			return;
		}
		CALLOC(ch->Imm->cr_social, SOCIAL_DATA, 1);
		ch->Imm->cr_social->verb = dictionary.add(verb);
		ch->put("Ok\r\n");
		return;
	}
	ch->put("@new social <action>\r\n");
}

void cr_new_cdata(Character *ch, char *id)
{
	vid_t vid;
	if (ch->Imm->cr_cdata)
	{
		ch->put("You have character data in your buffer already.\r\n");
		return;
	}
	if (id)
	{
		if (is_not_number(id) || (vid = atoi(id)) == 0)
		{
			ch->put("The V# must be a non-zero number.\r\n");
			return;
		}
		if (cdataIndex.get(vid) != (vid_t) -1)
		{
			ch->print("The V#%u already exists in the character index.\r\n", vid);
			return;
		}
		CALLOC(ch->Imm->cr_cdata, CHAR_DATA, 1);
		ch->Imm->cr_cdata->virtual_id = vid;
		ch->Imm->cr_cdata->limit = (limit_t) -1;
		ch->put("Ok\r\n");
		return;
	}
	ch->put("@new cdata <virtual-id>\r\n");
}

void cr_new_idata(Character *ch, char *id)
{
	vid_t vid;
	if (ch->Imm->cr_idata)
	{
		ch->put("You have item data in your buffer already.\r\n");
		return;
	}
	if (id)
	{
		if (is_not_number(id) || (vid = atoi(id)) == 0)
		{
			ch->put("The V# must be a non-zero number.\r\n");
			return;
		}
		if (idataIndex.get(vid) != (vid_t) -1)
		{
			ch->print("The V#%u already exists in the item index.\r\n", vid);
			return;
		}
		CALLOC(ch->Imm->cr_idata, ITEM_DATA, 1);
		ch->Imm->cr_idata->virtual_id = vid;
		ch->Imm->cr_idata->limit = (limit_t) -1;
		ch->put("Ok.\r\n");
		return;
	}
	ch->put("@new idata <virtual-id>\r\n");
}

void cr_new_cdescr(Character *ch, char *id)
{
	vid_t vid;
	if (ch->Imm->cr_cdescr)
	{
		ch->put("You have character descr data in your buffer already.\r\n");
		return;
	}
	if (id)
	{
		if (is_not_number(id) || (vid = atoi(id)) == 0)
		{
			ch->put("The D# must be a non-zero number.\r\n");
			return;
		}
		if (cdescrIndex.get(vid) != (vid_t) -1)
		{
			ch->print("The D#%u already exists in the descr index.\r\n", vid);
			return;
		}
		CALLOC(ch->Imm->cr_cdescr, CHAR_DESCR_DATA, 1);
		ch->Imm->cr_cdescr->virtual_id = vid;
		ch->Imm->cr_cdescr->key = new Words("char_descr");
		ch->put("Ok.\r\n");
		return;
	}
	ch->put("@new cdescr <description-id>\r\n");
}

void cr_new_idescr(Character *ch, char *id)
{
	vid_t vid;
	if (ch->Imm->cr_idescr)
	{
		ch->put("You have item descr data in your buffer already.\r\n");
		return;
	}
	if (id)
	{
		if (is_not_number(id) || (vid = atoi(id)) == 0)
		{
			ch->put("The D# must be a non-zero number.\r\n");
			return;
		}
		if (idescrIndex.get(vid) != (vid_t) -1)
		{
			ch->print("The D#%u already exists in the descr index.\r\n", vid);
			return;
		}
		CALLOC(ch->Imm->cr_idescr, ITEM_DESCR_DATA, 1);
		ch->Imm->cr_idescr->virtual_id = vid;
		ch->Imm->cr_idescr->key = new Words("item_descr");
		ch->Imm->cr_idescr->extra = new ExtraDescr("item_descr", vid);
		ch->put("Ok.\r\n");
		return;
	}
	ch->put("@new idescr <description-id>\r\n");
}

void cr_new_cstats(Character *ch, char *id)
{
	vid_t vid;
	if (ch->Imm->cr_cstats)
	{
		ch->put("You have character stats data in your buffer already.\r\n");
		return;
	}
	if (id)
	{
		if (is_not_number(id) || (vid = atoi(id)) == 0)
		{
			ch->put("The S# must be a non-zero number.\r\n");
			return;
		}
		if (cstatsIndex.get(vid) != (vid_t) -1)
		{
			ch->print("The S#%u already exists in the stats index.\r\n", vid);
			return;
		}
		CALLOC(ch->Imm->cr_cstats, CHAR_STATS_DATA, 1);
		ch->Imm->cr_cstats->virtual_id = vid;
		ch->Imm->cr_cstats->profession |= f_(WARRIOR);
		MALLOC(ch->Imm->cr_cstats->attack, ATTACK, 1);
		ch->Imm->cr_cstats->attack[0].type = ATTACK_HITS;
		ch->Imm->cr_cstats->attack[0].damage.die = 4;
		ch->Imm->cr_cstats->attack[0].damage.multiplier = 3;
		ch->Imm->cr_cstats->attack[0].damage.modifier = 0;
		ch->Imm->cr_cstats->hitpoints.die = 4;
		ch->Imm->cr_cstats->hitpoints.multiplier = 1;
		ch->Imm->cr_cstats->hitpoints.modifier = 16;
		ch->Imm->cr_cstats->weight_class = MODERATE;
		ch->Imm->cr_cstats->volume_class = MEDIUM;
		ch->Imm->cr_cstats->height_class = AVERAGE;
		memset(ch->Imm->cr_cstats->special.stat, 16, sizeof(stat_t)*STRENGTH_ADD);
		ch->Imm->cr_cstats->special.position = STANDING;
		ch->Imm->cr_cstats->thac0 = 20;
		ch->put("Ok.\r\n");
		return;
	}
	ch->put("@new cstats <stats-id>\r\n");
}

void cr_new_istats(Character *ch, char *id)
{
	vid_t vid;
	if (ch->Imm->cr_istats)
	{
		ch->put("You have item stats data in your buffer already.\r\n");
		return;
	}
	if (id)
	{
		if (is_not_number(id) || (vid = atoi(id)) == 0)
		{
			ch->put("The S# must be a non-zero number.\r\n");
			return;
		}
		if (istatsIndex.get(vid) != (vid_t) -1)
		{
			ch->print("The S#%u already exists in the stats index.\r\n", vid);
			return;
		}
		CALLOC(ch->Imm->cr_istats, ITEM_STATS_DATA, 1);
		ch->Imm->cr_istats->virtual_id = vid;
		ch->put("Ok.\r\n");
		return;
	}
	ch->put("@new istats <stats-id>\r\n");
}

void cr_new_room(Character *ch, char *id)
{
	vid_t vid;
	if (ch->Imm->cr_room)
	{
		ch->put("You have room data in your buffer already.\r\n");
		return;
	}
	if (id)
	{
		if (is_not_number(id) || (vid = atoi(id)) == 0)
		{
			ch->put("The room number must be a non-zero number.\r\n");
			return;
		}
		if (roomIndex.get(vid) != (vid_t) -1)
		{
			ch->print("The room [%u] already exists in the world.\r\n", vid);
			return;
		}
		CALLOC(ch->Imm->cr_room, ROOM_DATA, 1);
		ch->Imm->cr_room->virtual_id = vid;
		ch->Imm->cr_room->tread = 1;
		ch->Imm->cr_room->extra = new ExtraDescr("room", vid);
		ch->put("Ok.\r\n");
		return;
	}
	ch->put("@new room <room-number>\r\n");
}

void cr_new_zone(Character *ch, char *id)
{
	vid_t vid;
	if (ch->Imm->cr_zone)
	{
		ch->put("You have zone data in your buffer already.\r\n");
		return;
	}
	if (id)
	{
		if (is_not_number(id) || (vid = atoi(id)) == 0)
		{
			ch->put("The zone id must be a non-zero number.\r\n");
			return;
		}
		if(zoneIndex.get(vid) != (vid_t) -1)
		{
			ch->print("Zone %u already exists.\r\n", vid);
			return;
		}
		CALLOC(ch->Imm->cr_zone, ZONE_DATA, 1);
		ch->Imm->cr_zone->virtual_id = vid;
		ch->Imm->cr_zone->top = vid * 10;
		ch->put("Ok.\r\n");
		return;
	}
	ch->put("@new zone <zone-id>\r\n");
}

// STEP 1   //////////////////////////////////////////////////////////////////
 
void do_cr_new(Character *ch, char *arg, cmd_t cmd)
{
	TOKENIZER(arg, argv);

	if (argv[0])
	{
		if (is_name(argv[0], "help"))
		{
			cr_new_help(ch, argv[1]);
			return;
		}
		if (is_name(argv[0], "wizhelp"))
		{
			cr_new_wizhelp(ch, argv[1]);
			return;
		}
		if (is_name(argv[0], "social"))
		{
			cr_new_social(ch, argv[1]);
			return;
		}
		if (is_name(argv[0], "cdata"))
		{
			cr_new_cdata(ch, argv[1]);
			return;
		}
		if (is_name(argv[0], "idata"))
		{
			cr_new_idata(ch, argv[1]);
			return;
		}
		if (is_name(argv[0], "cdescr"))
		{
			cr_new_cdescr(ch, argv[1]);
			return;
		}
		if (is_name(argv[0], "cstats"))
		{
			cr_new_cstats(ch, argv[1]);
			return;
		}
		if (is_name(argv[0], "idescr"))
		{
			cr_new_idescr(ch, argv[1]);
			return;
		}
		if (is_name(argv[0], "istats"))
		{
			cr_new_istats(ch, argv[1]);
			return;
		}
		if (is_name(argv[0], "room"))
		{
			cr_new_room(ch, argv[1]);
			return;
		}
		if (is_name(argv[0], "zone"))
		{
			cr_new_zone(ch, argv[1]);
			return;
		}
	}
	ch->put("@new [help | wizhelp | social | room | zone |\r\n"
		     "      cdata | cdescr | cstats | idata | idescr | istats]\r\n");
}

void cr_examine_help(Character *ch)
{
	if (ch->Imm->cr_help==NULL)
	{
		ch->put("You need a help entry in your buffer first.\r\n");
		return;
	}
	ch->print(" --- help ---\r\nKEYWORD: %s\r\n\r\n --- entry ---\r\n%s~\r\n",
		ch->Imm->cr_help->keyword,
		C(ch->Imm->cr_help->entry));
}

void cr_examine_wizhelp(Character *ch)
{
	if (ch->Imm->cr_wizhelp==NULL)
	{
		ch->put("You need a wizhelp entry in your buffer first.\r\n");
		return;
	}
	ch->print(" --- wizhelp ---\r\nKEYWORD: %s\r\n\r\n --- entry ---\r\n%s~\r\n",
		ch->Imm->cr_wizhelp->keyword,
		C(ch->Imm->cr_wizhelp->entry));
}

void cr_examine_social(Character *ch)
{
	if (ch->Imm->cr_social==NULL)
	{
		ch->put("You need social in your buffer first.\r\n");
		return;
	}
	ch->print(" --- social ---\r\nVERB: %s\r\nMIN VICT POS: %s\r\nCAN SEE UNSEEN: %s\r\n\r\n --- 'to character' message ---\r\n%s\r\n~\r\n --- 'to others' message ---\r\n%s\r\n~\r\n --- 'to character' victim message ---\r\n%s\r\n~\r\n --- 'to others' victim message ---\r\n%s\r\n~\r\n --- 'to victim' victim message ---\r\n%s\r\n~\r\n --- 'to character' victim not found message ---\r\n%s\r\n~\r\n --- 'to character' self message ---\r\n%s\r\n~\r\n --- 'to others' self message ---\r\n%s\r\n~\r\n",
			ch->Imm->cr_social->verb,
			str_position[ch->Imm->cr_social->min_vict_pos],
			str_boolean[ch->Imm->cr_social->can_see_unseen],
			C(ch->Imm->cr_social->to_char_msg),
			C(ch->Imm->cr_social->to_others_msg),
			C(ch->Imm->cr_social->to_char_vict_msg),
			C(ch->Imm->cr_social->to_others_vict_msg),
			C(ch->Imm->cr_social->to_vict_vict_msg),
			C(ch->Imm->cr_social->to_char_vict_not_found_msg),
			C(ch->Imm->cr_social->to_char_self_msg),
			C(ch->Imm->cr_social->to_others_self_msg));
}

void cr_examine_cdata(Character *ch)
{
	if (ch->Imm->cr_cdata == NULL)
	{
		ch->put("You need character data in your buffer first.\r\n");
		return;
	}
	ch->print(" --- character data ---\r\nID    : V#%u\r\nCLAN  : %u\r\nLIMIT : %d\r\nNOTE  :\r\n%s\r\n --- values ---\r\nCharacter Description : D#%-5d '%s'\r\nCharacter Stats       : S#%-5d [%s][%s][%s][%s] [%d]\r\n",
			ch->Imm->cr_cdata->virtual_id,
			ch->Imm->cr_cdata->clan,
			ch->Imm->cr_cdata->limit == (limit_t) -1
				? -1 : ch->Imm->cr_cdata->limit,
			C(ch->Imm->cr_cdata->creators_note),
			ch->Imm->cr_cdata->descr ?
				(sbyte4) ch->Imm->cr_cdata->descr->virtual_id : -1,
			ch->Imm->cr_cdata->descr ?
				ch->Imm->cr_cdata->descr->proper : "",
			ch->Imm->cr_cdata->stats ?
				(sbyte4) ch->Imm->cr_cdata->stats->virtual_id : -1,
			ch->Imm->cr_cdata->stats ?
				str_homeland_a[ch->Imm->cr_cdata->stats->homeland] : "",
			ch->Imm->cr_cdata->stats ?
				str_race[ch->Imm->cr_cdata->stats->race] : "",
			ch->Imm->cr_cdata->stats ?
				str_gender_a[ch->Imm->cr_cdata->stats->gender] : "",
			ch->Imm->cr_cdata->stats ?
				str_profession_index(ch->Imm->cr_cdata->stats->profession) : "",
			ch->Imm->cr_cdata->stats ?  0 : -1); //MARK calculate value
}

void cr_examine_idata(Character *ch)
{
	if (ch->Imm->cr_idata == NULL)
	{
		ch->put("You need item data in your buffer first.\r\n");
		return;
	}
	ch->print(" --- item data ---\r\nID    : V#%u\r\nCLAN  : %u\r\nLIMIT : %d\r\nNOTE  :\r\n%s\r\n --- values ---\r\nItem Description : D#%-5d [%s]\r\nItem Stats       : S#%-5d [%s] [%d]\r\n",
			ch->Imm->cr_idata->virtual_id,
			ch->Imm->cr_idata->clan,
			ch->Imm->cr_idata->limit == (limit_t) -1
				? -1 : ch->Imm->cr_idata->limit,
			C(ch->Imm->cr_idata->creators_note),
			ch->Imm->cr_idata->descr ?
				(sbyte4) ch->Imm->cr_idata->descr->virtual_id : -1,
			ch->Imm->cr_idata->descr ?
				ch->Imm->cr_idata->descr->proper : "",
			ch->Imm->cr_idata->stats ?
				(sbyte4) ch->Imm->cr_idata->stats->virtual_id : -1,
			ch->Imm->cr_idata->stats ?
				str_item_type[ch->Imm->cr_idata->stats->item_type] : "",
			ch->Imm->cr_idata->stats ? 0 : -1); //MARK calculate value
}

void cr_examine_cdescr(Character *ch)
{
	if (ch->Imm->cr_cdescr == NULL)
	{
		ch->put("You need character descr data in your buffer first.\r\n");
		return;
	}
	ch->print(" --- char descr data ---\r\nID           : D#%u\r\nCREATORS NOTE :\r\n%s\r\n --- descriptions ---\r\nKEYWORDS : %s\r\nPROPER   : %s\r\nGENERAL  :\r\n%s",
			ch->Imm->cr_cdescr->virtual_id,
			C(ch->Imm->cr_cdescr->creators_note),
			ch->Imm->cr_cdescr->key->words(),
			C(ch->Imm->cr_cdescr->proper),
			C(ch->Imm->cr_cdescr->general));
}

void cr_examine_idescr(Character *ch)
{
	if (ch->Imm->cr_idescr == NULL)
	{
		ch->put("You need item descr data in your buffer first.\r\n");
		return;
	}
	ch->print(" --- item descr data ---\r\nID           : D#%u\r\nCREATORS NOTE :\r\n%s\r\n --- descriptions ---\r\nKEYWORDS : %s\r\nPROPER   : %s\r\nGENERAL  :\r\n%s",
			ch->Imm->cr_idescr->virtual_id,
			C(ch->Imm->cr_idescr->creators_note),
			ch->Imm->cr_idescr->key->words(),
			C(ch->Imm->cr_idescr->proper),
			C(ch->Imm->cr_idescr->general));
	ch->Imm->cr_idescr->extra->listTo(ch);
}

void cr_examine_cstats(Character *ch)
{
	if (ch->Imm->cr_cstats == NULL)
	{
		ch->put("You need character stats data in your buffer first.\r\n");
		return;
	}
	ch->print(" --- char stats data ---\r\nID          : S#%u\r\nRACE        : %s\r\nGENDER      : %s\r\nHOMELAND    : %s\r\nPROFESSION  : %s\r\nHITPOINTS   : %ud%u+%u\r\nTHAC0       : %d\r\nMOVE TYPE   : %s\r\nHEIGHT TYPE : %s\r\nWEIGHT TYPE : %s\r\nVOLUME TYPE : %s\r\n --- specials ---\r\nSTATS       : STR[%u/%u] DEX[%u] INT[%u] WIS[%u] CON[%u] CHA[%u]\r\nDEFAULT POS : %s\r\nSAVES       : PARAL[%d] STAVE[%d] PETRI[%d] BREATH[%d] SPELL[%d]\r\n",
			ch->Imm->cr_cstats->virtual_id,
			str_race[ch->Imm->cr_cstats->race],
			str_gender[ch->Imm->cr_cstats->gender],
			str_homeland[ch->Imm->cr_cstats->homeland],
			str_profession_index(ch->Imm->cr_cstats->profession),
			ch->Imm->cr_cstats->hitpoints.multiplier,
			ch->Imm->cr_cstats->hitpoints.die,
			ch->Imm->cr_cstats->hitpoints.modifier,
			ch->Imm->cr_cstats->thac0,
			str_movement[ch->Imm->cr_cstats->movement_type],
			str_height_type[ch->Imm->cr_cstats->height_class],
			str_weight_type[ch->Imm->cr_cstats->weight_class],
			str_volume_type[ch->Imm->cr_cstats->volume_class],
			ch->Imm->cr_cstats->special.stat[STRENGTH],
			ch->Imm->cr_cstats->special.stat[STRENGTH_ADD],
			ch->Imm->cr_cstats->special.stat[DEXTERITY],
			ch->Imm->cr_cstats->special.stat[INTELLIGENCE],
			ch->Imm->cr_cstats->special.stat[WISDOM],
			ch->Imm->cr_cstats->special.stat[CONSTITUTION],
			ch->Imm->cr_cstats->special.stat[CHARISMA],
			str_position[ch->Imm->cr_cstats->special.position],
			ch->Imm->cr_cstats->special.save_vs[SAVE_PARALYZATION],
			ch->Imm->cr_cstats->special.save_vs[SAVE_STAVES],
			ch->Imm->cr_cstats->special.save_vs[SAVE_PETRIFICATION],
			ch->Imm->cr_cstats->special.save_vs[SAVE_BREATH],
			ch->Imm->cr_cstats->special.save_vs[SAVE_SPELL]);
	ch->put(" --- acts ---\r\n");
	list_flags_to(ch, ch->Imm->cr_cstats->act_flags, str_act_f);
	ch->put(" --- affects ---\r\n");
	list_flags_to(ch, ch->Imm->cr_cstats->special.affect_flags, str_affect_f);
	ch->put(" --- immunities & resistances ---\r\n");
	list_flags_to(ch, ch->Imm->cr_cstats->special.immunity_flags,
			str_immunity_f);
	list_flags_to(ch, ch->Imm->cr_cstats->special.resistance_flags,
			str_resistance_f);
	ch->put(" --- citizenship ---\r\n");
	list_flags_to(ch, ch->Imm->cr_cstats->special.citizenship, str_citizenship);
	ch->put(" --- attacks ---\r\n");
	for (int i = 0; i <= ch->Imm->cr_cstats->attack_top; i++)
	{
		ch->print("  [%d] %s - %dd%d+%d\r\n",
				i,
				str_attack_type[ch->Imm->cr_cstats->attack[i].type],
				ch->Imm->cr_cstats->attack[i].damage.multiplier,
				ch->Imm->cr_cstats->attack[i].damage.die,
				ch->Imm->cr_cstats->attack[i].damage.modifier);
	}
}

void cr_examine_istats(Character *ch)
{
	if (ch->Imm->cr_istats == NULL)
	{
		ch->put("You need item stats data in your buffer first.\r\n");
		return;
	}
	ch->print(" --- item stats data ---\r\nID              : S#%u\r\nCLASS           : %s\r\nWEIGHT          : %d\r\nVOLUME          : %d\r\n",
			ch->Imm->cr_istats->virtual_id,
			str_item_type[ch->Imm->cr_istats->item_type],
			ch->Imm->cr_istats->weight,
			ch->Imm->cr_istats->volume);
	ch->put(" --- values ---\r\n");
	switch(ch->Imm->cr_istats->item_type)
	{
		ITEM_CONTAINER:
			ch->print("0-SIZE          : %d\r\n"
					    "1-FLAGS         : %u\r\n"
						 "2-KEY V#        : %u\r\n"
						 "3-CORPSE C#     : %u\r\n",
						 ch->Imm->cr_istats->special.value[0],
						 ch->Imm->cr_istats->special.value[1],
						 ch->Imm->cr_istats->special.value[2],
						 ch->Imm->cr_istats->special.value[3]);
			break;
		ITEM_NOTE:
			ch->print("0-LANGUAGE      : %d\r\n"
					    "VALUE-1         : %d\r\n"
						 "VALUE-2         : %d\r\n"
						 "VALUE-3         : %d\r\n",
						 ch->Imm->cr_istats->special.value[0],
						 ch->Imm->cr_istats->special.value[1],
						 ch->Imm->cr_istats->special.value[2],
						 ch->Imm->cr_istats->special.value[3]);
			break;
		default:
			ch->print("VALUE 0         : %d\r\n"
					    "VALUE 1         : %d\r\n"
						 "VALUE 2         : %d\r\n"
						 "VALUE 3         : %d\r\n",
						 ch->Imm->cr_istats->special.value[0],
						 ch->Imm->cr_istats->special.value[1],
						 ch->Imm->cr_istats->special.value[2],
						 ch->Imm->cr_istats->special.value[3]);

	}
	ch->put(" --- attributes ---\r\n");
	list_flags_to(ch, ch->Imm->cr_istats->attribute_flags, str_attribute_f);
	ch->put(" --- properties ---\r\n");
	list_flags_to(ch, ch->Imm->cr_istats->property_flags,
			str_property_f);
	ch->put(" --- materials ---\r\n");
	list_flags_to(ch, ch->Imm->cr_istats->material_flags, str_material_f);
	ch->put(" --- wear ---\r\n");
	list_flags_to(ch, ch->Imm->cr_istats->wear_flags, str_wear_pos_f);
	ch->put(" --- affects ---\r\n");
	list_flags_to(ch, ch->Imm->cr_istats->special.affect_flags, str_affect_f);
	ch->put(" --- immunities & resistances ---\r\n");
	list_flags_to(ch, ch->Imm->cr_istats->special.immunity_flags,
			str_immunity_f);
	list_flags_to(ch, ch->Imm->cr_istats->special.resistance_flags,
			str_resistance_f);
}

void cr_examine_room(Character *ch)
{
	if (ch->Imm->cr_room == NULL)
	{
		ch->put("You need room data in your buffer first.\r\n");
		return;
	}
	ch->print(" --- room data ---\r\nID           : %u\r\nNAME         : %s~\r\n(ZONE ID     : %u '%s')\r\nSECTOR       : %s\r\nTREAD        : %u\r\nCLAN         : %u\r\nMAP ITEM VID : %d '%s'\r\nMAP COORDS   : %d, %d\r\n --- description ---\r\n%s~\r\n --- dark description ---\r\n%s~\r\n --- storm description ---\r\n%s~\r\n",
			ch->Imm->cr_room->virtual_id,
			C(ch->Imm->cr_room->name),
			get_room_zone(ch->Imm->cr_room->virtual_id)->getVID(),
			get_room_zone(ch->Imm->cr_room->virtual_id)->getName(),
			str_sector_type[ch->Imm->cr_room->sector],
			ch->Imm->cr_room->tread,
			ch->Imm->cr_room->clan,
			ch->Imm->cr_room->coord ? (sbyte4) ch->Imm->cr_room->coord->v : -1,
			ch->Imm->cr_room->coord
				? get_item_data(ch->Imm->cr_room->coord->v)->descr->proper : "",
			ch->Imm->cr_room->coord ? ch->Imm->cr_room->coord->y : -1,
			ch->Imm->cr_room->coord ? ch->Imm->cr_room->coord->x : -1,
			C(ch->Imm->cr_room->descr),
			C(ch->Imm->cr_room->descr_dark),
			C(ch->Imm->cr_room->descr_storm));
	ch->Imm->cr_room->extra->listTo(ch);
	ch->put(" --- room flags ---\r\n");
	list_flags_to(ch, ch->Imm->cr_room->special.flags, str_room_flags);
	ch->print(" --- creators note ---\r\n%s", ch->Imm->cr_room->creators_note);
	
	for (top_t dir = 0; dir < NO_DIR; dir++)
	{
		if (ch->Imm->cr_room->door[dir])
		{
			ch->print(" --- direction %s ---\r\n", str_dir[dir]);
			ch->print("KEYWORDS  : %s\r\nPROPER    : %s~\r\nWEIGHT    : %u\r\nTYPE      : %s\r\nROOM      : [%d] '%s'\r\nROOM KEY  : [%d] '%s'\r\n     - descripton -\r\n%s~\r\n",
					ch->Imm->cr_room->door[dir]->key ? 
						ch->Imm->cr_room->door[dir]->key->words() : "",
					C(ch->Imm->cr_room->door[dir]->proper),
					ch->Imm->cr_room->door[dir]->weight,
					str_door_type[ch->Imm->cr_room->door[dir]->door_type],

					ch->Imm->cr_room->door[dir]->room ?
						(sbyte4) ch->Imm->cr_room->door[dir]->room->getVID() : -1,
					ch->Imm->cr_room->door[dir]->room ?
						ch->Imm->cr_room->door[dir]->room->getName() : "",

					ch->Imm->cr_room->door[dir]->room_key ?
						(sbyte4) ch->Imm->cr_room->door[dir]->room_key->virtual_id
							: -1,
					ch->Imm->cr_room->door[dir]->room_key ?
						C(ch->Imm->cr_room->door[dir]->room_key->descr->proper) : "",
					C(ch->Imm->cr_room->door[dir]->descr));

			ch->print("     - flags %s -\r\n", str_dir[dir]);
			list_flags_to(ch, ch->Imm->cr_room->door[dir]->special.flags,
					str_door_flags);
		}
	}
}

void cr_examine_zone(Character *ch)
{
	if (ch->Imm->cr_zone == NULL)
	{
		ch->put("You must have zone data in your buffer first.\r\n");
		return;
	}
	ch->print("ZONE ID      : %u\r\nNAME         : %s\r\nTYPE         : %s\r\nHOMELAND     : %s\r\nLEVEL        : %u-%u\r\nLIFESPAN     : %u\r\nRANGE        : %u-%u\r\nMAP ITEM VID : %d '%s'\r\nMAP COORDS   : %d, %d\r\n --- description ---\r\n%s~\r\n",
			ch->Imm->cr_zone->virtual_id,
			C(ch->Imm->cr_zone->name),
			str_zone_type[ch->Imm->cr_zone->zone_type],
			str_homeland[ch->Imm->cr_zone->homeland],
			ch->Imm->cr_zone->min_lvl,
			ch->Imm->cr_zone->max_lvl,
			ch->Imm->cr_zone->special.lifespan,
			get_zone_bot(ch->Imm->cr_zone->virtual_id),
			ch->Imm->cr_zone->top,
			ch->Imm->cr_zone->coord ? (sbyte4) ch->Imm->cr_zone->coord->v : -1,
			ch->Imm->cr_zone->coord
				? Zone::get_zone_data(ch->Imm->cr_zone->coord->v)->name : "",
			ch->Imm->cr_zone->coord ? ch->Imm->cr_zone->coord->x : -1,
			ch->Imm->cr_zone->coord ? ch->Imm->cr_zone->coord->y : -1,
			C(ch->Imm->cr_zone->descr));
	ch->put(" --- flags ---\r\n");
	list_flags_to(ch, ch->Imm->cr_zone->special.flags, str_zone_flags);
	ch->print(" --- creators note ---\r\n%s~\r\n",
			C(ch->Imm->cr_zone->creators_note));
			
}

// STEP 2   //////////////////////////////////////////////////////////////////

void do_cr_examine(Character *ch, char *arg, cmd_t cmd)
{
	TOKENIZER(arg, argv);

	if (argv[0])
	{
		if (is_name(argv[0], "help"))
		{
			cr_examine_help(ch);
			return;
		}
		if (is_name(argv[0], "wizhelp"))
		{
			cr_examine_wizhelp(ch);
			return;
		}
		if (is_name(argv[0], "social"))
		{
			cr_examine_social(ch);
			return;
		}
		if (is_name(argv[0], "cdata"))
		{
			cr_examine_cdata(ch);
			return;
		}
		if (is_name(argv[0], "cdescr"))
		{
			cr_examine_cdescr(ch);
			return;
		}
		if (is_name(argv[0], "cstats"))
		{
			cr_examine_cstats(ch);
			return;
		}
		if (is_name(argv[0], "idata"))
		{
			cr_examine_idata(ch);
			return;
		}
		if (is_name(argv[0], "idescr"))
		{
			cr_examine_idescr(ch);
			return;
		}
		if (is_name(argv[0], "istats"))
		{
			cr_examine_istats(ch);
			return;
		}
		if (is_name(argv[0], "room"))
		{
			cr_examine_room(ch);
			return;
		}
		if (is_name(argv[0], "zone"))
		{
			cr_examine_zone(ch);
			return;
		}
	}
	ch->put("@examine [help | wizhelp | social | room | zone |\r\n"
		  	  "          cdata | cdescr | cstats | idata | idescr | istats]\r\n");
}

void cr_delete_help(Character *ch)
{
	if (ch->Imm->cr_help==NULL)
	{
		ch->put("You need a help entry in your buffer first.\r\n");
		return;
	}
	free(ch->Imm->cr_help->entry);
	FREE(ch->Imm->cr_help);
	ch->put("Ok.\r\n");
}

void cr_delete_wizhelp(Character *ch)
{
	if (ch->Imm->cr_wizhelp==NULL)
	{
		ch->put("You need a wizhelp entry in your buffer first.\r\n");
		return;
	}
	free(ch->Imm->cr_wizhelp->entry);
	FREE(ch->Imm->cr_wizhelp);
	ch->put("Ok.\r\n");
}

void cr_delete_social(Character *ch)
{
	if (ch->Imm->cr_social==NULL)
	{
		ch->put("You need a social in your buffer first.\r\n");
		return;
	}
	free_social(ch->Imm->cr_social);
	ch->Imm->cr_social = NULL;
	ch->put("Ok.\r\n");
}

void cr_delete_idata(Character *ch)
{
	if (ch->Imm->cr_idata == NULL)
	{
		ch->put("You need item data in your buffer first.\r\n");
		return;
	}
	free_tinyworld_item(ch->Imm->cr_idata);
	ch->Imm->cr_idata = NULL;
	ch->put("Ok.\r\n");
}

void cr_delete_cdata(Character *ch)
{
	if (ch->Imm->cr_cdata == NULL)
	{
		ch->put("You need character data in your buffer first.\r\n");
		return;
	}
	free_tinyworld_character(ch->Imm->cr_cdata);
	ch->Imm->cr_cdata = NULL;
	ch->put("Ok.\r\n");
}

void cr_delete_cdescr(Character *ch)
{
	if (ch->Imm->cr_cdescr == NULL)
	{
		ch->put("You need character descr data in your buffer first.\r\n");
		return;
	}
	free_tinyworld_character_description(ch->Imm->cr_cdescr);
	ch->Imm->cr_cdescr = NULL;
	ch->put("Ok.\r\n");
}
	
void cr_delete_idescr(Character *ch)
{
	if (ch->Imm->cr_idescr == NULL)
	{
		ch->put("You need item descr data in your buffer first.\r\n");
		return;
	}
	free_tinyworld_item_description(ch->Imm->cr_idescr);
	ch->Imm->cr_idescr = NULL;
	ch->put("Ok.\r\n");
}

void cr_delete_cstats(Character *ch)
{
	if (ch->Imm->cr_cstats == NULL)
	{
		ch->put("You need character stats data in your buffer first.\r\n");
		return;
	}
	free_tinyworld_character_stat(ch->Imm->cr_cstats);
	ch->Imm->cr_cstats = NULL;
	ch->put("Ok.\r\n");
}

void cr_delete_istats(Character *ch)
{
	if (ch->Imm->cr_istats == NULL)
	{
		ch->put("You need item stats data in your buffer first.\r\n");
		return;
	}
	free_tinyworld_item_stat(ch->Imm->cr_istats);
	ch->Imm->cr_istats = NULL;
	ch->put("Ok.\r\n");
}

void cr_delete_room(Character *ch)
{
	if (ch->Imm->cr_room == NULL)
	{
		ch->put("You need room data in your buffer first.\r\n");
		return;
	}
	free_tinyworld_room(ch->Imm->cr_room);
	ch->Imm->cr_room = NULL;
	ch->put("Ok.\r\n");
}

void cr_delete_zone(Character *ch)
{
	if (ch->Imm->cr_zone == NULL)
	{
		ch->put("You need zone data in your buffer first.\r\n");
		return;
	}
	free_tinyworld_zone(ch->Imm->cr_zone);
	ch->Imm->cr_zone = NULL;
	ch->put("Ok.\r\n");
}

// STEP 3   //////////////////////////////////////////////////////////////////

void do_cr_delete(Character *ch, char *arg, cmd_t cmd)
{
	TOKENIZER(arg, argv);

	if (argv[0])
	{
		if (is_name(argv[0], "help"))
		{
			cr_delete_help(ch);
			return;
		}
		if (is_name(argv[0], "wizhelp"))
		{
			cr_delete_wizhelp(ch);
			return;
		}
		if (is_name(argv[0], "social"))
		{
			cr_delete_social(ch);
			return;
		}
		if (is_name(argv[0], "cdata"))
		{
			cr_delete_cdata(ch);
			return;
		}
		if (is_name(argv[0], "cdescr"))
		{
			cr_delete_cdescr(ch);
			return;
		}
		if (is_name(argv[0], "cstats"))
		{
			cr_delete_cstats(ch);
			return;
		}
		if (is_name(argv[0], "idata"))
		{
			cr_delete_idata(ch);
			return;
		}
		if (is_name(argv[0], "idescr"))
		{
			cr_delete_idescr(ch);
			return;
		}
		if (is_name(argv[0], "istats"))
		{
			cr_delete_istats(ch);
			return;
		}
		if (is_name(argv[0], "room"))
		{
			cr_delete_room(ch);
			return;
		}
		if (is_name(argv[0], "zone"))
		{
			cr_delete_zone(ch);
			return;
		}
	}
	ch->put("@delete [help | wizhelp | social | room | zone |\r\n"
		     "         cdata | cdescr | cstats | idata | idescr | istats]\r\n");
}

void cr_save_help(Character *ch)
{
	if (ch->Imm->cr_help==NULL)
	{
		ch->put("You need a help entry in your buffer first.\r\n");
		return;
	}
	assert(ch->Imm->cr_help->keyword);
	if (ch->Imm->cr_help->entry==NULL)
	{
		ch->put("You need a help entry in your buffer first.\r\n");
		return;
	}
	helpTable->add(ch->Imm->cr_help);
	addlog_creation(ch->Imm->cr_help->keyword, ch);
	cr_delete_help(ch);
}

void cr_save_wizhelp(Character *ch)
{
	if (ch->Imm->cr_wizhelp==NULL)
	{
		ch->put("You need a wizhelp entry in your buffer first.\r\n");
		return;
	}
	assert(ch->Imm->cr_wizhelp->keyword);
	if (ch->Imm->cr_wizhelp->entry==NULL)
	{
		ch->put("You need a wizhelp entry in your buffer first.\r\n");
		return;
	}
	immhelpTable->add(ch->Imm->cr_wizhelp);
	addlog_creation(ch->Imm->cr_wizhelp->keyword, ch);
	cr_delete_wizhelp(ch);
}

void cr_save_social(Character *ch)
{
	if (ch->Imm->cr_social == NULL)
	{
		ch->put("You need a social in your buffer first.\r\n");
		return;
	}
	if (ch->Imm->cr_social->to_char_msg==NULL)
	{
		ch->put("You need a social \"to_char_msg\" first.\r\n");
		return;
	}
	if (ch->Imm->cr_social->to_others_self_msg
			&& ch->Imm->cr_social->to_char_self_msg==NULL)
	{
		ch->put( "You need a social \"to_others_self_msg\" first.\r\n");
		return;
	}
	if (ch->Imm->cr_social->to_char_vict_msg==NULL
			&& (ch->Imm->cr_social->to_others_vict_msg
				|| ch->Imm->cr_social->to_vict_vict_msg))
	{
		ch->put("You need a social \"to_char_vict_msg\" first.\r\n");
		return;
	}
	if (ch->Imm->cr_social->to_others_vict_msg==NULL
			&& (ch->Imm->cr_social->to_char_vict_msg
				|| ch->Imm->cr_social->to_vict_vict_msg))
	{
		ch->put("You need a social \"to_others_vict_msg\" first.\r\n");
		return;
	}
	if (ch->Imm->cr_social->to_vict_vict_msg==NULL
			&& (ch->Imm->cr_social->to_char_vict_msg
				|| ch->Imm->cr_social->to_others_vict_msg))
	{
		ch->put("You need a social \"to_vict_vict_msg\" first.\r\n");
		return;
	}
	if (ch->Imm->cr_social->to_char_vict_not_found_msg
			&& ch->Imm->cr_social->to_char_vict_msg==NULL)
	{
		ch->put("You need a social \"to_char_vict_msg\" first.\r\n");
		return;
	}
	assert(ch->Imm->cr_social->verb);
	save_social(ch->Imm->cr_social);
	add_social(ch->Imm->cr_social);
	addlog_creation(ch->Imm->cr_social->verb, ch);
	ch->Imm->cr_social = NULL;
	ch->put("Ok.\r\n");
}

void cr_save_cdata(Character *ch)
{
	if (ch->Imm->cr_cdata == NULL)
	{
		ch->put("You need character data in your buffer first.\r\n");
		return;
	}
	if (ch->Imm->cr_cdata->descr == NULL)
	{
		ch->put("You need to assign a character description first.\r\n");
		return;
	}
	if (ch->Imm->cr_cdata->stats == NULL)
	{
		ch->put("You need to assign a character stats first.\r\n");
		return;
	}
	assert(ch->Imm->cr_cdata->virtual_id != 0);
	save_tinyworld_character(ch->Imm->cr_cdata);
	add_char_data(ch->Imm->cr_cdata);
	addlog_creation(ch->Imm->cr_cdata->virtual_id, ch);
	ch->Imm->cr_cdata = NULL;
	ch->put("Ok.\r\n");
}

void cr_save_idata(Character *ch)
{
	if (ch->Imm->cr_idata == NULL)
	{
		ch->put("You need item data in your buffer first.\r\n");
		return;
	}
	if (ch->Imm->cr_idata->descr == NULL)
	{
		ch->put("You need to assign an item description first.\r\n");
		return;
	}
	if (ch->Imm->cr_idata->stats == NULL)
	{
		ch->put("You need to assign an item stats first.\r\n");
		return;
	}
	assert(ch->Imm->cr_idata->virtual_id != 0);
	save_tinyworld_item(ch->Imm->cr_idata);
	add_item_data(ch->Imm->cr_idata);
	addlog_creation(ch->Imm->cr_idata->virtual_id, ch);
	ch->Imm->cr_idata = NULL;
	ch->put("Ok.\r\n");
}

void cr_save_cdescr(Character *ch)
{
	if (ch->Imm->cr_cdescr == NULL)
	{
		ch->put("You need character description data in your buffer first.\r\n");
		return;
	}
	if (ch->Imm->cr_cdescr->key->count() == 0)
	{
		ch->put("You need to assign at least one keyword first.\r\n");
		return;
	}
	if (ch->Imm->cr_cdescr->proper == NULL)
	{
		ch->put("You need to assign a proper description first.\r\n");
		return;
	}
	if (ch->Imm->cr_cdescr->room == NULL)
	{
		ch->put("You need to assign an in-room description first.\r\n");
		return;
	}
	if (ch->Imm->cr_cdescr->general == NULL)
	{
		ch->put("You need to assign a general description first.\r\n");
		return;
	}
	if (ch->Imm->cr_idescr->extra->checkFor(ch))
	{
		return;
	}
	assert(ch->Imm->cr_cdescr->virtual_id != 0);
	save_tinyworld_character_description(ch->Imm->cr_cdescr);
	add_char_descr(ch->Imm->cr_cdescr);
	addlog_creation(ch->Imm->cr_cdescr->virtual_id, ch);
	ch->Imm->cr_cdescr = NULL;
	ch->put("Ok.\r\n");
}

void cr_save_idescr(Character *ch)
{
	if (ch->Imm->cr_idescr == NULL)
	{
		ch->put("You need item description data in your buffer first.\r\n");
		return;
	}
	if (ch->Imm->cr_idescr->key->count() == 0)
	{
		ch->put("You need to assign at least one keyword first.\r\n");
		return;
	}
	if (ch->Imm->cr_idescr->proper == NULL)
	{
		ch->put("You need to assign a proper description first.\r\n");
		return;
	}
	if (ch->Imm->cr_idescr->room == NULL)
	{
		ch->put("You need to assign an in-room description first.\r\n");
		return;
	}
	if (ch->Imm->cr_idescr->general == NULL)
	{
		ch->put("You need to assign a general description first.\r\n");
		return;
	}
	if (ch->Imm->cr_idescr->extra->checkFor(ch))
	{
		return;
	}
	assert(ch->Imm->cr_idescr->virtual_id != 0);
	save_tinyworld_item_description(ch->Imm->cr_idescr);
	add_item_descr(ch->Imm->cr_idescr);
	addlog_creation(ch->Imm->cr_idescr->virtual_id, ch);
	ch->Imm->cr_idescr = NULL;
	ch->put("Ok.\r\n");
}

void cr_save_cstats(Character *ch)
{
	if (ch->Imm->cr_cstats == NULL)
	{
		ch->put("You need character stats data in your buffer first.\r\n");
		return;
	}
	assert(ch->Imm->cr_cstats->virtual_id != 0);
	save_tinyworld_character_stat(ch->Imm->cr_cstats);
	add_char_stats(ch->Imm->cr_cstats);
	addlog_creation(ch->Imm->cr_cstats->virtual_id, ch);
	ch->Imm->cr_cstats = NULL;
	ch->put("Ok.\r\n");
}

void cr_save_istats(Character *ch)
{
	if (ch->Imm->cr_istats == NULL)
	{
		ch->put("You need item stats data in your buffer first.\r\n");
		return;
	}
	assert(ch->Imm->cr_istats->virtual_id != 0);
	save_tinyworld_item_stat(ch->Imm->cr_istats);
	add_item_stats(ch->Imm->cr_istats);
	addlog_creation(ch->Imm->cr_istats->virtual_id, ch);
	ch->Imm->cr_istats = NULL;
	ch->put("Ok.\r\n");
}

void cr_save_room(Character *ch)
{
	if (ch->Imm->cr_room == NULL)
	{
		ch->put("You need room data in your buffer first.\r\n");
		return;
	}
	if (ch->Imm->cr_room->name == NULL)
	{
		ch->put("You need to assign a room name first.\r\n");
		return;
	}
	if (ch->Imm->cr_room->descr == NULL)
	{
		ch->put("You need to assign a room description first.\r\n");
		return;
	}
	if (ch->Imm->cr_room->extra->checkFor(ch))
	{
		return;
	}
	assert(ch->Imm->cr_room->virtual_id != 0);
	Room *room = add_room(ch->Imm->cr_room);
	room->save();
	addlog_creation(ch->Imm->cr_room->virtual_id, ch);
	ch->Imm->cr_room = NULL;
	ch->put("Ok.\r\n");
}

void cr_save_zone(Character *ch)
{
	if (ch->Imm->cr_zone == NULL)
	{
		ch->put("You need zone data in your buffer first.\r\n");
		return;
	}
	if (ch->Imm->cr_zone->name == NULL)
	{
		ch->put("You need to assign a zone name first.\r\n");
		return;
	}
	if (ch->Imm->cr_zone->descr == NULL)
	{
		ch->put("You need to assign a zone description first.\r\n");
		return;
	}
	assert(ch->Imm->cr_zone->virtual_id != 0);
	Zone *zone = add_zone(ch->Imm->cr_zone);
	zone->save();
	addlog_creation(ch->Imm->cr_zone->virtual_id, ch);
	ch->Imm->cr_zone = NULL;
	ch->put("Ok.\r\n");
}

// STEP 4   //////////////////////////////////////////////////////////////////

void do_cr_save(Character *ch, char *arg, cmd_t cmd)
{
	TOKENIZER(arg, argv);

	if (argv[0])
	{
		if (is_name(argv[0], "help"))
		{
			cr_save_help(ch);
			return;
		}
		if (is_name(argv[0], "wizhelp"))
		{
			cr_save_wizhelp(ch);
			return;
		}
		if (is_name(argv[0], "social"))
		{
			cr_save_social(ch);
			return;
		}
		if (is_name(argv[0], "cdata"))
		{
			cr_save_cdata(ch);
			return;
		}
		if (is_name(argv[0], "cdescr"))
		{
			cr_save_cdescr(ch);
			return;
		}
		if (is_name(argv[0], "cstats"))
		{
			cr_save_cstats(ch);
			return;
		}
		if (is_name(argv[0], "idata"))
		{
			cr_save_idata(ch);
			return;
		}
		if (is_name(argv[0], "idescr"))
		{
			cr_save_idescr(ch);
			return;
		}
		if (is_name(argv[0], "istats"))
		{
			cr_save_istats(ch);
			return;
		}
		if (is_name(argv[0], "room"))
		{
			cr_save_room(ch);
			return;
		}
		if (is_name(argv[0], "zone"))
		{
			cr_save_zone(ch);
			return;
		}
	}
	ch->put("@save [help | wizhelp | social | room | zone |\r\n"
		     "       cdata | cdescr | cstats | idata | idescr | istats]\r\n");
}

void cr_destroy_help(Character *ch, char *keyword)
{
	if (keyword)
	{
		if (helpTable->exists(keyword)==0)
		{
			ch->put("That keyword does not exist.\r\n");
			return;
		}
		helpTable->del(keyword);
		addlog_creation(ch->Imm->cr_help->keyword, ch);
		ch->put("Ok.\r\n");
		return;
	}
	ch->put("@destroy help <keyword>\r\n");
}

void cr_destroy_wizhelp(Character *ch, char *keyword)
{
	if (keyword)
	{
		if (immhelpTable->exists(keyword)==0)
		{
			ch->put("That keyword does not exist.\r\n");
			return;
		}
		immhelpTable->del(keyword);
		addlog_creation(ch->Imm->cr_wizhelp->keyword, ch);
		ch->put("Ok.\r\n");
		return;
	}
	ch->put("@destroy wizhelp <keyword>\r\n");
}

void cr_destroy_social(Character *ch, char *verb)
{
	if (verb)
	{
		if (social_exists(verb)==0)
		{
			ch->put("That social does not exist.\r\n");
			return;
		}
		destroy_social(verb);
		addlog_creation(ch->Imm->cr_help->keyword, ch);
		ch->put("Ok.\r\n");
		return;
	}
	ch->put("@destroy social <action>\r\n");
}

void cr_destroy_cdata(Character *ch, char *id)
{
	vid_t i;

	if (id)
	{
		if (is_not_number(id) || (i = atoi(id)) == 0)
		{
			ch->put("The V# must be a non-zero number.\r\n");
			return;
		}
		if ((i = cdataIndex.get(i)) == (vid_t) -1)
		{
			ch->put("That V# does not exist.\r\n");
			return;
		}
		destroy_tinyworld_character(i);
		addlog_creation(ch->Imm->cr_cdata->virtual_id, ch);
		ch->put("Ok.\r\n");
		return;
	}
	ch->put("@destroy cdata <virtual-id>\r\n");
}

void cr_destroy_cdescr(Character *ch, char *id)
{
	vid_t i;

	if (id)
	{
		if (is_not_number(id) || (i = atoi(id)) == 0)
		{
			ch->put("The D# must be a non-zero number.\r\n");
			return;
		}
		if ((i = cdescrIndex.get(i)) == (vid_t) -1)
		{
			ch->put("That D# does not exist.\r\n");
			return;
		}
		destroy_tinyworld_character_description(i);
		addlog_creation(ch->Imm->cr_cdescr->virtual_id, ch);
		ch->put("Ok.\r\n");
		return;
	}
	ch->put("@destroy cdescr <description-id>\r\n");
	return;
}

void cr_destroy_idata(Character *ch, char *id)
{
	vid_t i;

	if (id)
	{
		if (is_not_number(id) || (i = atoi(id)) == 0)
		{
			ch->put("The V# must be a non-zero number.\r\n");
			return;
		}
		if ((i = idataIndex.get(i)) == (vid_t) -1)
		{
			ch->put("That V# does not exist.\r\n");
			return;
		}
		destroy_tinyworld_item(i);
		addlog_creation(ch->Imm->cr_idata->virtual_id, ch);
		ch->put("Ok.\r\n");
		return;
	}
	ch->put("@destroy idata <virtual-id>\r\n");
}

void cr_destroy_idescr(Character *ch, char *id)
{
	vid_t i;

	if (id)
	{
		if (is_not_number(id) || (i = atoi(id)) == 0)
		{
			ch->put("The D# must be a non-zero number.\r\n");
			return;
		}
		if ((i = idescrIndex.get(i)) == (vid_t) -1)
		{
			ch->put("That D# does not exist.\r\n");
			return;
		}
      destroy_tinyworld_item_description(i);
		addlog_creation(ch->Imm->cr_idescr->virtual_id, ch);
		ch->put("Ok.\r\n");
		return;
	}
	ch->put("@destroy idescr <description-id>\r\n");
}

void cr_destroy_cstats(Character *ch, char *id)
{
	vid_t i;

	if (id)
	{
		if (is_not_number(id) || (i = atoi(id)) == 0)
		{
			ch->put("The S# must be a non-zero number.\r\n");
			return;
		}
		if ((i = cstatsIndex.get(i)) == (vid_t) -1)
		{
			ch->put("That S# does not exist.\r\n");
			return;
		}
		destroy_tinyworld_character_stat(i);
		addlog_creation(ch->Imm->cr_cstats->virtual_id, ch);
		ch->put("Ok.\r\n");
		return;
	}
	ch->put("@destroy cstats <stats-id>\r\n");
}

void cr_destroy_istats(Character *ch, char *id)
{
	vid_t i;

	if (id)
	{
		if (is_not_number(id) || (i = atoi(id)) == 0)
		{
			ch->put("The S# must be a non-zero number.\r\n");
			return;
		}
		if ((i = istatsIndex.get(i)) == (vid_t) -1)
		{
			ch->put("That S# does not exist.\r\n");
			return;
		}
		destroy_tinyworld_item_stat(i);
		addlog_creation(ch->Imm->cr_istats->virtual_id, ch);
		ch->put("Ok.\r\n");
		return;
	}
	ch->put("@destroy istats <stats-id>\r\n");
}

void cr_destroy_room(Character *ch, char *id)
{
	vid_t i;

	if (id)
	{
		if (is_not_number(id) || (i = atoi(id)) == 0)
		{
			ch->put("The room number must be a non-zero number.\r\n");
			return;
		}
		if ((i == roomIndex.get(i)) == (vid_t) -1)
		{
			ch->put("That room number does not exist.\r\n");
			return;
		}
		destroy_tinyworld_room(i);
		addlog_creation(ch->Imm->cr_room->virtual_id, ch);
		ch->put("Ok.\r\n");
	}
	ch->put("@destroy room <room-number>\r\n");
}

void cr_destroy_zone(Character *ch, char *id)
{
	vid_t i;

	if (id)
	{
		if (is_not_number(id) || (i =  atoi(id)) == 0)
		{
			ch->put("The zone id must be a non-zero number.\r\n");
			return;
		}
		if ((i == roomIndex.get(i)) == (vid_t) -1)
		{
			ch->put("That zone id does not exist.\r\n");
			return;
		}
		destroy_tinyworld_zone(i);
		addlog_creation(ch->Imm->cr_zone->virtual_id, ch);
		ch->put("Ok.\r\n");
	}
	ch->put("@destroy zone <zone-id>\r\n");
}

// STEP 5   //////////////////////////////////////////////////////////////////

/** \todo check dependencies on all destroy functions */
void do_cr_destroy(Character *ch, char *arg, cmd_t cmd)
{
	TOKENIZER(arg, argv);

	if (argv[0])
	{
		if (is_name(argv[0], "help"))
		{
			cr_destroy_help(ch, argv[1]);
			return;
		}
		if (is_name(argv[0], "wizhelp"))
		{
			cr_destroy_wizhelp(ch, argv[1]);
			return;
		}
		if (is_name(argv[0], "social"))
		{
			cr_destroy_social(ch, argv[1]);
			return;
		}
		if (is_name(argv[0], "cdata"))
		{
			cr_destroy_cdata(ch, argv[1]);
			return;
		}
		if (is_name(argv[0], "cdescr"))
		{
			cr_destroy_cdescr(ch, argv[1]);
			return;
		}
		if (is_name(argv[0], "cstats"))
		{
			cr_destroy_cstats(ch, argv[1]);
			return;
		}
		if (is_name(argv[0], "idata"))
		{
			cr_destroy_idata(ch, argv[1]);
			return;
		}
		if (is_name(argv[0], "idescr"))
		{
			cr_destroy_idescr(ch, argv[1]);
			return;
		}
		if (is_name(argv[0], "istats"))
		{
			cr_destroy_istats(ch, argv[1]);
			return;
		}
		if (is_name(argv[0], "room"))
		{
			cr_destroy_room(ch, argv[1]);
			return;
		}
		if (is_name(argv[0], "zone"))
		{
			cr_destroy_zone(ch, argv[1]);
			return;
		}
	}
	ch->put("@destroy [help | wizhelp | social | room | zone |\r\n"
		  	  "          cdata | cdescr | cstats | idata | idescr | istats]\r\n");
}

void cr_load_help(Character *ch, char *keyword)
{
	if (ch->Imm->cr_help)
	{
		ch->put("You have a help entry in your buffer already.\r\n");
		return;
	}
	if (keyword)
	{
		CALLOC(ch->Imm->cr_help, HELP_ENTRY, 1);
		helpTable->load(keyword, ch->Imm->cr_help);
		if (ch->Imm->cr_help->keyword == NULL)
		{
			FREE(ch->Imm->cr_help);
			ch->put("That keyword does not exist.\r\n");
			return;
		}
		ch->put("Ok.\r\n");
		return;
	}
	ch->put("@load help <keyword>\r\n");
}

void cr_load_wizhelp(Character *ch, char *keyword)
{
	if (ch->Imm->cr_wizhelp)
	{
		ch->put("You have a wizhelp entry in your buffer already.\r\n");
		return;
	}
	if (keyword)
	{
		CALLOC(ch->Imm->cr_wizhelp, HELP_ENTRY, 1);
		immhelpTable->load(keyword, ch->Imm->cr_wizhelp);
		if (ch->Imm->cr_wizhelp->keyword == NULL)
		{
			FREE(ch->Imm->cr_wizhelp);
			ch->put("That keyword does not exist.\r\n");
			return;
		}
		ch->put("Ok.\r\n");
		return;
	}
	ch->put("@load wizhelp <keyword>\r\n");
}

void cr_load_social(Character *ch, char *verb)
{
	if (ch->Imm->cr_social)
	{
		ch->put("You have a social in your buffer already.\r\n");
		return;
	}
	if (verb)
	{
		CALLOC(ch->Imm->cr_social, SOCIAL_DATA, 1);
		load_social(verb, ch->Imm->cr_social);
		if (ch->Imm->cr_social->verb == NULL)
		{
			FREE(ch->Imm->cr_social);
			ch->put("That action does not exist.\r\n");
			return;
		}
		ch->put("Ok.\r\n");
		return;
	}
	ch->put("@load social <action>\r\n");
}

void cr_load_cdata(Character *ch, char *id)
{
	vid_t i;
	if (ch->Imm->cr_cdata)
	{
		ch->put("You have character data in your buffer already.\r\n");
		return;
	}
	if (id)
	{
		if (is_not_number(id) || (i = atoi(id))==0)
		{
			ch->put("The V# must be a non-zero number.\r\n");
			return;
		}
		if ((i = cdataIndex.get(i)) == (vid_t) -1)
		{
			ch->put("That V# does not exist.\r\n");
			return;
		}
		MALLOC(ch->Imm->cr_cdata, CHAR_DATA, 1);
		copy_tinyworld_character(ch->Imm->cr_cdata, i);
		ch->put("Ok.\r\n");
		return;
	}
	ch->put("@load cdata <virtual-id>\r\n");
}

void cr_load_idata(Character *ch, char *id)
{
	vid_t i;
	if (ch->Imm->cr_idata)
	{
		ch->put("You have item data in your buffer already.\r\n");
		return;
	}
	if (id)
	{
		if (is_not_number(id) || (i = atoi(id))==0)
		{
			ch->put("The V# must be a non-zero number.\r\n");
			return;
		}
		if ((i = cdataIndex.get(i)) == (vid_t) -1)
		{
			ch->put("That V# does not exist.\r\n");
			return;
		}
		MALLOC(ch->Imm->cr_idata, ITEM_DATA, 1);
		copy_tinyworld_item(ch->Imm->cr_idata, i);
		ch->put("Ok.\r\n");
		return;
	}
	ch->put("@load idata <virtual-id>\r\n");
}

void cr_load_cdescr(Character *ch, char *id)
{
	vid_t i;
	if (ch->Imm->cr_cdescr)
	{
		ch->put("You have character description data in your buffer already.\r\n");
		return;
	}

	if (id)
	{
		if (is_not_number(id) || (i = atoi(id)) == 0)
		{
			ch->put("The D# must be a non-zero number.\r\n");
			return;
		}
		if ((i = cdescrIndex.get(i)) == (vid_t) -1)
		{
			ch->put("That D# does not exist.\r\n");
			return;
		}
		MALLOC(ch->Imm->cr_cdescr, CHAR_DESCR_DATA, 1);
		copy_tinyworld_character_description(ch->Imm->cr_cdescr, i);
		ch->put("Ok.\r\n");
		return;
	}
	ch->put("@load cdescr <description-id>\r\n");
}

void cr_load_idescr(Character *ch, char *id)
{
	vid_t i;
	if (ch->Imm->cr_idescr)
	{
		ch->put("You have item description data in your buffer already.\r\n");
		return;
	}
	if (id)
	{
		if (is_not_number(id) || (i = atoi(id)) == 0)
		{
			ch->put("The D# must be a non-zero number.\r\n");
			return;
		}
		if ((i = idescrIndex.get(i)) == (vid_t) -1)
		{
			ch->put("That D# does not exist.\r\n");
			return;
		}
		MALLOC(ch->Imm->cr_idescr, ITEM_DESCR_DATA, 1);
		copy_tinyworld_item_description(ch->Imm->cr_idescr, i);
		ch->put("Ok.\r\n");
		return;
	}
	ch->put("@load idescr <description-id>\r\n");
}

void cr_load_cstats(Character *ch, char *id)
{
	vid_t i;
	if (ch->Imm->cr_cstats)
	{
		ch->put("You have character stats data in your buffer already.\r\n");
		return;
	}
	if (id)
	{
		if (is_not_number(id) || (i = atoi(id)) == 0)
		{
			ch->put("The S# must be a non-zero number.\r\n");
			return;
		}
		if ((i = cstatsIndex.get(i)) == (vid_t) -1)
		{
			ch->put("That S# does not exist.\r\n");
			return;
		}
		MALLOC(ch->Imm->cr_cstats, CHAR_STATS_DATA, 1);
		copy_tinyworld_character_stat(ch->Imm->cr_cstats, i);
		ch->put("Ok.\r\n");
		return;
	}
	ch->put("@load cstats <stats-id>\r\n");
}

void cr_load_istats(Character *ch, char *id)
{
	vid_t i;
	if (ch->Imm->cr_istats)
	{
		ch->put("You have item stats data in your buffer already.\r\n");
		return;
	}
	if (id)
	{
		if (is_not_number(id) || (i = atoi(id)) == 0)
		{
			ch->put("The S# must be a non-zero number.\r\n");
			return;
		}
		if ((i = istatsIndex.get(i)) == (vid_t) -1)
		{
			ch->put("That S# does not exist.\r\n");
			return;
		}
		MALLOC(ch->Imm->cr_istats, ITEM_STATS_DATA, 1);
		copy_tinyworld_item_stat(ch->Imm->cr_istats, i);
		ch->put("Ok.\r\n");
		return;
	}
	ch->put("@load istats <stats-id>\r\n");
}

void cr_load_room(Character *ch, char *id)
{
	vid_t i;
	if (ch->Imm->cr_room)
	{
		ch->put("You have room data in your buffer already.\r\n");
		return;
	}
	if (id)
	{
		if (is_not_number(id) || (i = atoi(id)) == 0)
		{
			ch->put("The room number must be a non-zero number.\r\n");
			return;
		}
		if ((i = roomIndex.get(i)) == (vid_t) -1)
		{
			ch->put("That room number does not exist.\r\n");
			return;
		}
		MALLOC(ch->Imm->cr_room, ROOM_DATA, 1);
		Room::copy_tinyworld_room(ch->Imm->cr_room, i);
		ch->put("Ok.\r\n");
		return;
	}
	ch->put("@load room <room-number>\r\n");
}

void cr_load_zone(Character *ch, char *id)
{
	vid_t i;
	if (ch->Imm->cr_zone)
	{
		ch->put("You have zone data in your buffer already.\r\n");
		return;
	}
	if (id)
	{
		if (is_not_number(id) || (i = atoi(id)) == 0)
		{
			ch->put("The zone id must be a non-zero number.\r\n");
			return;
		}
		if ((i = zoneIndex.get(i)) == (vid_t) -1)
		{
			ch->put("That zone id does not exist.\r\n");
			return;
		}
		MALLOC(ch->Imm->cr_zone, ZONE_DATA, 1);
		Zone::copy_tinyworld_zone_data(ch->Imm->cr_zone, i);
		ch->put("Ok.\r\n");
		return;
	}
	ch->put("@load zone <zone-id>\r\n");
}

// STEP 6   //////////////////////////////////////////////////////////////////

void do_cr_load(Character *ch, char *arg, cmd_t cmd)
{
	TOKENIZER(arg, argv);

	if (argv[0])
	{
		if (is_name(argv[0], "help"))
		{
			cr_load_help(ch, argv[1]);
			return;
		}
		if (is_name(argv[0], "wizhelp"))
		{
			cr_load_wizhelp(ch, argv[1]);
			return;
		}
		if (is_name(argv[0], "social"))
		{
			cr_load_social(ch, argv[1]);
			return;
		}
		if (is_name(argv[0], "cdata"))
		{
			cr_load_cdata(ch, argv[1]);
			return;
		}
		if (is_name(argv[0], "cdescr"))
		{
			cr_load_cdescr(ch, argv[1]);
			return;
		}
		if (is_name(argv[0], "cstats"))
		{
			cr_load_cstats(ch, argv[1]);
			return;
		}
		if (is_name(argv[0], "idata"))
		{
			cr_load_idata(ch, argv[1]);
			return;
		}
		if (is_name(argv[0], "idescr"))
		{
			cr_load_idescr(ch, argv[1]);
			return;
		}
		if (is_name(argv[0], "istats"))
		{
			cr_load_istats(ch, argv[1]);
			return;
		}
		if (is_name(argv[0], "room"))
		{
			cr_load_room(ch, argv[1]);
			return;
		}
		if (is_name(argv[0], "zone"))
		{
			cr_load_zone(ch, argv[1]);
			return;
		}
	}
	ch->put("@load [help | wizhelp | social | room | zone |\r\n"
			  "       cdata | cdescr | cstats | idata | idescr | istats]\r\n");
}
