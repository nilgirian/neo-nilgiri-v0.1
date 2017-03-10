/* **************************************************************************
    Nilgiri the Forgotten World
    (C) 2004
   ************************************************************************** */
/** \file
    \brief general modify game commands module
 */
#include<stdio.h>
#include"tinyworld_character.h"
#include"tinyworld_zone.h"
#include"library.h"
#include"utils.h"

void ch_set_index(Character *ch, ubyte1 *index, const char *type,
		const char **strarray, const char *strcmd)
{
	if (set_index(index, type, strarray) == 1)
	{
		ch->put("Ok.\r\n");
		return;
	}
	ch->print("%s <type>\r\n<type> can be:\r\n", strcmd);
	list_flags_to(ch, (ubyte8) -1, strarray);
}

template <class bitv_t> void ch_set_flag(Character *ch, bitv_t *bitvector,
		const char *flag, const char **strarray, const char *strcmd)
{
	if (set_flag(bitvector, flag, strarray) == 1)
	{
		ch->put("Ok.\r\n");
		return;
	}
	ch->print("%s <flag>\r\n<flag> can be set as:\r\n", strcmd);
	list_flags_to(ch, (ubyte8) -1, strarray);
}

void ch_general_writer(Character *ch, char **msg, const char *strold)
{
	if (*msg)
	{
		ch->print("The %s:\r\n%s", strold, *msg);
	}
	ch->writer(msg);
}

static void cr_help_switch(Character *ch, char *keyword1, char *keyword2)
{
	if (keyword1 && keyword2)
	{
		to_upperstr(keyword1);
		to_upperstr(keyword2);
		if (helpTable->update(keyword1, keyword2)==-1)
		{
			ch->print("Could not switch keyword '%s' to '%s.'",
					keyword1,
					keyword2);
			return;
		}
		ch->put("Ok.\r\n");
		return;
	}
	ch->put("@help switch <keyword> <to-keyword>\r\n");
}

static void cr_help_attach(Character *ch, char *keyword1, char *keyword2)
{
	if (keyword1 && keyword2)
	{
		to_upperstr(keyword1);
		to_upperstr(keyword2);
		if (helpTable->add(keyword1, keyword2)==-1)
		{
			ch->print("Could not attach '%s' to '%s'\r\n", keyword1, keyword2);
			return;
		}
		ch->put("Ok.\r\n");
		return;
	}
	ch->put("@help attach <keyword> <to-keyword>\r\n");
}

static void cr_help_keyword(Character *ch, char *keyword)
{
	if (keyword)
	{
		to_upperstr(keyword);
		ch->Imm->cr_help->keyword = dictionary.add(keyword);
		ch->put("Ok.\r\n");
		return;
	}
	ch->put("@help keyword <keyword>\r\n");
}

void do_cr_help(Character *ch, char *arg, cmd_t cmd)
{
	TOKENIZER(arg, argv);

	if (argv[0])
	{
		if (is_name(argv[0], "keyword"))
		{
			if (ch->Imm->cr_help==NULL)
			{
				ch->put("You need a help entry in your buffer first.\r\n");
				return;
			}
			cr_help_keyword(ch, argv[1]);
			return;
		}
		if (is_name(argv[0], "entry"))
		{
			if (ch->Imm->cr_help==NULL)
			{
				ch->put("You need a help entry in your buffer first.\r\n");
				return;
			}
			ch_general_writer(ch, &ch->Imm->cr_help->entry, "old help entry");
			return;
		}
		if (is_name(argv[0], "switch"))
		{
			cr_help_switch(ch, argv[1], argv[2]);
			return;
		}
		if (is_name(argv[0], "attach"))
		{
			cr_help_attach(ch, argv[1], argv[2]);
			return;
		}
	}
	ch->put("@help [attach | entry | keyword | switch]\r\n");
}

static void cr_wizhelp_switch(Character *ch, char *keyword1, char *keyword2)
{
	if (keyword1 && keyword2)
	{
		to_upperstr(keyword1);
		to_upperstr(keyword2);
		if (immhelpTable->update(keyword1, keyword2)==-1)
		{
			ch->print("Could not switch keyword '%s' to '%s.'",
					keyword1,
					keyword2);
			return;
		}
		ch->put("Ok.\r\n");
		return;
	}
	ch->put("@wizhelp switch <keyword> <to-keyword>\r\n");
}

static void cr_wizhelp_attach(Character *ch, char *keyword1, char *keyword2)
{
	if (keyword1 && keyword2)
	{
		to_upperstr(keyword1);
		to_upperstr(keyword2);
		if (immhelpTable->add(keyword1, keyword2)==-1)
		{
			ch->print("Could not attach '%s' to '%s'\r\n", keyword1, keyword2);
			return;
		}
		ch->put("Ok.\r\n");
		return;
	}
	ch->put("@wizhelp attach <keyword> <to-keyword>\r\n");
}

static void cr_wizhelp_keyword(Character *ch, char *keyword)
{
	if (keyword)
	{
		to_upperstr(keyword);
		ch->Imm->cr_wizhelp->keyword = dictionary.add(keyword);
		ch->put("Ok.\r\n");
		return;
	}
	ch->put("@wizhelp keyword <keyword>\r\n");
}

void do_cr_wizhelp(Character *ch, char *arg, cmd_t cmd)
{
	TOKENIZER(arg, argv);

	if (argv[0])
	{
		if (is_name(argv[0], "keyword"))
		{
			if (ch->Imm->cr_wizhelp==NULL)
			{
				ch->put("You need a wizhelp entry in your buffer first.\r\n");
				return;
			}
			cr_wizhelp_keyword(ch, argv[1]);
			return;
		}
		if (is_name(argv[0], "entry"))
		{
			if (ch->Imm->cr_wizhelp==NULL)
			{
				ch->put("You need a wizhelp entry in your buffer first.\r\n");
				return;
			}
			ch_general_writer(ch, &ch->Imm->cr_wizhelp->entry,
					"old wizhelp entry");
			return;
		}
		if (is_name(argv[0], "switch"))
		{
			cr_wizhelp_switch(ch, argv[1], argv[2]);
			return;
		}
		if (is_name(argv[0], "attach"))
		{
			cr_wizhelp_attach(ch, argv[1], argv[2]);
			return;
		}
	}
	ch->put("@wizhelp [attach | entry | keyword | switch]\r\n");
}

static void cr_social_verb(Character *ch, char *verb)
{
	if (verb)
	{
		to_lowerstr(verb);
		ch->Imm->cr_social->verb = dictionary.add(verb);
		ch->put("Ok.\r\n");
		return;
	}
	ch->put("@social verb <verb>\r\n");
}

static void cr_social_can_see_unseen(Character *ch, char *boolean)
{
	if (boolean)
	{
		if (is_name(boolean, "true"))
		{
			ch->Imm->cr_social->can_see_unseen = 1;
			ch->put("Ok.\r\n");
			return;
		}
		if (is_name(boolean, "false"))
		{
			ch->Imm->cr_social->can_see_unseen = 0;
			ch->put("Ok.\r\n");
			return;
		}
	}
	ch->put("@social can_see_unseen ( true | false )");
}

void cr_social_message(Character *ch, char **msg)
{
	if (*msg)
	{
		ch->print("The old message:\r\n%s\r\n", *msg);
	}
	ch->cr1LWriter(msg);
}

void do_cr_social(Character *ch, char *arg, cmd_t cmd)
{
	TOKENIZER(arg, argv);

	if (ch->Imm->cr_social==NULL)
	{
		ch->put("You need a social in your buffer first.\r\n");
		return;
	}
	if (argv[0])
	{
		if (is_name(argv[0], "verb"))
		{
			cr_social_verb(ch, argv[1]);
			return;
		}
		if (is_name(argv[0], "min_vict_pos"))
		{
			ch_set_index(ch, &ch->Imm->cr_social->min_vict_pos, argv[1],
					str_position, "@social min_vict_pos");
			return;
		}
		if (is_name(argv[0], "can_see_unseen"))
		{
			cr_social_can_see_unseen(ch, argv[1]);
			return;
		}
		if (is_name(argv[0], "to_char_msg"))
		{
			cr_social_message(ch, &ch->Imm->cr_social->to_char_msg);
			return;
		}
		if (is_name(argv[0], "to_others_msg"))
		{
			cr_social_message(ch, &ch->Imm->cr_social->to_others_msg);
			return;
		}
		if (is_name(argv[0], "to_char_vict_msg"))
		{
			cr_social_message(ch, &ch->Imm->cr_social->to_char_vict_msg);
			return;
		}
		if (is_name(argv[0], "to_others_vict_msg"))
		{
			cr_social_message(ch, &ch->Imm->cr_social->to_others_vict_msg);
			return;
		}
		if (is_name(argv[0], "to_vict_vict_msg"))
		{
			cr_social_message(ch, &ch->Imm->cr_social->to_vict_vict_msg);
			return;
		}
		if (is_name(argv[0], "to_char_vict_not_found_msg"))
		{
			cr_social_message(ch,
					&ch->Imm->cr_social->to_char_vict_not_found_msg);
			return;
		}
		if (is_name(argv[0], "to_char_self_msg"))
		{
			cr_social_message(ch, &ch->Imm->cr_social->to_char_self_msg);
			return;
		}
		if (is_name(argv[0], "to_others_self_msg"))
		{
			cr_social_message(ch, &ch->Imm->cr_social->to_others_self_msg);
			return;
		}
	}
	ch->put("@social [verb | min_vict_pos | can_see_unseen | to_char_msg | to_others_msg |\r\n"
			  "         to_char_vict_msg | to_others_vict_msg | to_vict_vict_msg |\r\n"
			  "         to_char_vict_not_found_msg | to_char_self_msg | to_others_self_msg]\r\n");
}

void cr_cdata_limit(Character *ch, char *value)
{
	if (value)
	{
		if (is_not_number(value))
		{
			ch->put("The limit must be a number.\r\n");
			return;
		}
		ch->Imm->cr_cdata->limit = atoi(value);
		ch->put("Ok.\r\n");
		return;
	}
	ch->put("@cdata limit <value>\r\n");
}

void cr_cdata_clan(Character *ch, char *clan)
{
	ch->put("Not yet ready.\r\n");
}

void cr_cdata_stats(Character *ch, char *id)
{
	vid_t vid;
	CHAR_STATS_DATA *cstats;

	if (id)
	{
		if (is_not_number(id) || (vid = atoi(id)) == 0)
		{
			ch->put("The S# must be a non-zero number.\r\n");
			return;
		}
		if ((cstats = find_char_stats(vid)) == NULL)
		{
			ch->put("That S# does not exist.\r\n");
			return;
		}
		ch->Imm->cr_cdata->stats = cstats;
		ch->put("Ok.\r\n");
		return;
	}
	ch->put("@cdata stats <stats-id>\r\n");
}

void cr_cdata_descr(Character *ch, char *id)
{
	vid_t vid;
	CHAR_DESCR_DATA *cdescr;

	if (id)
	{
		if (is_not_number(id) || (vid = atoi(id)) == 0)
		{
			ch->put("The D# must be a non-zero number.\r\n");
			return;
		}
		if ((cdescr = find_char_descr(vid)) == NULL)
		{
			ch->put("That D# does not exist.\r\n");
			return;
		}
		ch->Imm->cr_cdata->descr = cdescr;
		ch->put("Ok.\r\n");
		return;
	}
	ch->put("@cdata descr <description-id>\r\n");
}

void do_cr_cdata(Character *ch, char *arg, cmd_t cmd)
{
	TOKENIZER(arg, argv);

	if (ch->Imm->cr_cdata == NULL)
	{
		ch->put("You need character data in your buffer first.\r\n");
		return;
	}
	if (is_name(argv[0], "descr"))
	{
		cr_cdata_descr(ch, argv[1]);
		return;
	}
	if (is_name(argv[0], "stats"))
	{
		cr_cdata_stats(ch, argv[1]);
		return;
	}
	if (is_name(argv[0], "creators_note"))
	{
		ch_general_writer(ch, &ch->Imm->cr_cdata->creators_note,
				"old creators note");
		return;
	}
	if (is_name(argv[0], "clan"))
	{
		cr_cdata_clan(ch, argv[1]);
		return;
	}
	if (is_name(argv[0], "limit"))
	{
		cr_cdata_clan(ch, argv[1]);
		return;
	}
	ch->put("@cdata [descr | stats | creators_note | clan | limit]\r\n");
}

void cr_idata_limit(Character *ch, char *value)
{
	if (value)
	{
		if (is_not_number(value))
		{
			ch->put("The limit must be a number.\r\n");
			return;
		}
		ch->Imm->cr_idata->limit = atoi(value);
		ch->put("Ok.\r\n");
		return;
	}
	ch->put("@idata limit <value>\r\n");
}

void cr_idata_clan(Character *ch, char *clan)
{
	ch->put("Not yet ready.\r\n");
}

void cr_idata_stats(Character *ch, char *id)
{
	vid_t vid;
	const ITEM_STATS_DATA *istats;

	if (id)
	{
		if (is_not_number(id) || (vid = atoi(id)) == 0)
		{
			ch->put("The S# must be a non-zero number.\r\n");
			return;
		}
		if ((istats = find_item_stats(vid)) == NULL)
		{
			ch->put("That S# does not exist.\r\n");
			return;
		}
		ch->Imm->cr_idata->stats = istats;
		ch->put("Ok.\r\n");
		return;
	}
	ch->put("@idata stats <stats-id>\r\n");
}

void cr_idata_descr(Character *ch, char *id)
{
	vid_t vid;
	const ITEM_DESCR_DATA *idescr;

	if (id)
	{
		if (is_not_number(id) || (vid = atoi(id)) == 0)
		{
			ch->put("The D# must be a non-zero number.\r\n");
			return;
		}
		if ((idescr = find_item_descr(vid)) == NULL)
		{
			ch->put("That D# does not exist.\r\n");
			return;
		}
		ch->Imm->cr_idata->descr = idescr;
		ch->put("Ok.\r\n");
		return;
	}
	ch->put("@idata descr <description-id>\r\n");
}

void do_cr_idata(Character *ch, char *arg, cmd_t cmd)
{
	TOKENIZER(arg, argv);

	if (ch->Imm->cr_idata == NULL)
	{
		ch->put("You need item data in your buffer first.\r\n");
		return;
	}
	if (is_name(argv[0], "descr"))
	{
		cr_idata_descr(ch, argv[1]);
		return;
	}
	if (is_name(argv[0], "stats"))
	{
		cr_idata_stats(ch, argv[1]);
		return;
	}
	if (is_name(argv[0], "creators_note"))
	{
		ch_general_writer(ch, &ch->Imm->cr_idata->creators_note,
				"old creators note");
		return;
	}
	if (is_name(argv[0], "clan"))
	{
		cr_idata_clan(ch, argv[1]);
		return;
	}
	if (is_name(argv[0], "limit"))
	{
		cr_idata_limit(ch, argv[1]);
		return;
	}
	ch->put("@idata [descr | stats | creators_note | clan | limit]\r\n");
}

void cr_descr_keyword(Character *ch, Words *key, char *cmd, char *keyword)
{
	if (cmd && keyword)
	{
		to_lowerstr(keyword);
		if (is_name(cmd, "add"))
		{
			if (key->add(keyword))
			{
				ch->put("Ok.\r\n");
			}
			else
			{
				ch->put("That keyword already exists.\r\n");
			}
			return;
		}
		if (is_name(cmd, "remove"))
		{
			if (key->remove(keyword))
			{
				ch->put("Ok.\r\n");
			}
			else
			{
				ch->put("That keyword is not found.\r\n");
			}
			return;
		}
	}
	ch->print("@%s keyword (add | remove) <keyword>\r\n",
			*key->getTable() == 'i' ? "idescr" :
			*key->getTable() == 'c' ? "cdescr" : "<command>");
}

void cr_descr_proper(Character *ch, char **msg)
{
	if (*msg)
	{
		ch->print("The old proper name: %s\r\n", *msg);
	}
	ch->cr1LWriter(msg);
}

void cr_descr_room(Character *ch, char **msg)
{
	if (*msg)
	{
		ch->print("The old room description:\r\n%s\r\n", *msg);
	}
	ch->cr1LWriter(msg);
}

void do_cr_cdescr(Character *ch, char *arg, cmd_t cmd)
{
	if (ch->Imm->cr_cdescr == NULL)
	{
		ch->put("You need character description data in your buffer first.\r\n");
		return;
	}
	TOKENIZER(arg, argv);
	if (argv[0])
	{
		if (is_name(argv[0], "keyword"))
		{
			cr_descr_keyword(ch, ch->Imm->cr_cdescr->key, argv[1], argv[2]);
			return;
		}
		if (is_name(argv[0], "proper"))
		{
			cr_descr_proper(ch, &ch->Imm->cr_cdescr->proper);
			return;
		}
		if (is_name(argv[0], "room"))
		{
			cr_descr_room(ch, &ch->Imm->cr_cdescr->room);
			return;
		}
		if (is_name(argv[0], "general"))
		{
			ch_general_writer(ch, &ch->Imm->cr_cdescr->general,
					"old general description");
			return;
		}
		if (is_name(argv[0], "creators_note"))
		{
			ch_general_writer(ch, &ch->Imm->cr_cdescr->creators_note,
					"old creators note");
			return;
		}
	}
	ch->put("@cdescr [keyword | proper | room | general | creators_note]\r\n");
}

void cr_descr_extra_add(Character *ch, ExtraDescr *extraDescr, char *proper)
{
	if (proper)
	{
		if (extraDescr->add(proper))
		{
			ch->put("Ok.\r\n");
			return;
		}
		ch->put("That proper name already exists.\r\n");
		return;
	}
	ch->print("@%sdescr extra add <proper-name>\r\n",
			*extraDescr->getTable() == 'i' ? "idescr" :
			*extraDescr->getTable() == 'c' ? "cdescr" :
			*extraDescr->getTable() == 'r' ? "room" : "descr");
}

void cr_descr_extra_delete(Character *ch, ExtraDescr *extraDescr, char *proper)
{
	if (proper)
	{
		if (extraDescr->del(proper))
		{
			ch->put("Ok.\r\n");
			return;
		}
		ch->put("That extra description's proper name does not exist.\r\n");
		return;
	}
	ch->print("@%s extra delete \"<proper-name>\"\r\n",
			*extraDescr->getTable() == 'i' ? "idescr" :
			*extraDescr->getTable() == 'c' ? "cdescr" :
			*extraDescr->getTable() == 'r' ? "room" : "descr");
}

void cr_descr_extra_add_keyword(Character *ch, ExtraDescr *extraDescr,
		char *proper, char *keyword)
{
	EXTRA_DESCR *exdescr;

	if (proper && keyword)
	{
		if ((exdescr = ch->Imm->cr_idescr->extra->get(proper)) == NULL)
		{
			ch->put("That extra description's proper name is not found.\r\n");
			return;
		}
		cr_descr_keyword(ch, exdescr->key, "add", keyword);
		return;
	}
	ch->print("@%s extra add_keyword \"<proper-name>\" <keyword>\r\n",
			*extraDescr->getTable() == 'i' ? "idescr" :
			*extraDescr->getTable() == 'c' ? "cdescr" :
			*extraDescr->getTable() == 'r' ? "room" : "descr");
}

void cr_descr_extra_delete_keyword(Character *ch, ExtraDescr *extraDescr,
		char *proper, char *keyword)
{
	EXTRA_DESCR *exdescr;

	if (proper && keyword)
	{
		if ((exdescr = extraDescr->get(proper)) == NULL)
		{
			ch->put("That extra description's proper name is not found.\r\n");
			return;
		}
		cr_descr_keyword(ch, exdescr->key, "delete", keyword);
		return;
	}
	ch->print("@%s extra del_keyword \"<proper-name>\" <keyword>\r\n",
			*extraDescr->getTable() == 'i' ? "idescr" :
			*extraDescr->getTable() == 'c' ? "cdescr" :
			*extraDescr->getTable() == 'r' ? "room" : "descr");
}

void cr_descr_extra_general(Character *ch, ExtraDescr *extraDescr, char *proper)
{
	EXTRA_DESCR *exdescr;

	if (proper)
	{
		if ((exdescr = extraDescr->get(proper)) == NULL)
		{
			ch->put("That extra description's proper name is not found.\r\n");
			return;
		}
		ch_general_writer(ch, &exdescr->general, "old general description");
		return;
	}
	ch->print("@%s extra general \"<proper-name>\"\r\n",
			*extraDescr->getTable() == 'i' ? "idescr" :
			*extraDescr->getTable() == 'c' ? "cdescr" :
			*extraDescr->getTable() == 'r' ? "room" : "descr");
}

void cr_descr_extra(Character *ch, ExtraDescr *extraDescr, char *cmd,
		char *proper, char *arg)
{
	if (cmd)
	{
		if (is_name(cmd, "add"))
		{
			cr_descr_extra_add(ch, extraDescr, proper);
			return;
		}
		if (is_name(cmd, "delete"))
		{
			cr_descr_extra_delete(ch, extraDescr, proper);
			return;
		}
		if (is_name(cmd, "addword"))
		{
			cr_descr_extra_add_keyword(ch, extraDescr, proper, arg);
			return;
		}
		if (is_name(cmd, "delword"))
		{
			cr_descr_extra_delete_keyword(ch, extraDescr, proper, arg);
			return;
		}
		if (is_name(cmd, "general"))
		{
			cr_descr_extra_general(ch, extraDescr, proper);
			return;
		}
	}
	ch->print("@%s extra [add | delete | general |\r\n"
				 "                addword | delword] \"<proper-name>\"\r\n",
			*extraDescr->getTable() == 'i' ? "idescr" :
			*extraDescr->getTable() == 'c' ? "cdescr" :
			*extraDescr->getTable() == 'r' ? "room" : "descr");
}

void do_cr_idescr(Character *ch, char *arg, cmd_t cmd)
{
	if (ch->Imm->cr_idescr == NULL)
	{
		ch->put("You need item description data in your buffer first.\r\n");
		return;
	}
	TOKENIZER(arg, argv);
	if (argv[0])
	{
		if (is_name(argv[0], "keyword"))
		{
			cr_descr_keyword(ch, ch->Imm->cr_idescr->key, argv[1], argv[2]);
			return;
		}
		if (is_name(argv[0], "proper"))
		{
			cr_descr_proper(ch, &ch->Imm->cr_idescr->proper);
			return;
		}
		if (is_name(argv[0], "room"))
		{
			cr_descr_room(ch, &ch->Imm->cr_idescr->room);
			return;
		}
		if (is_name(argv[0], "general"))
		{
			ch_general_writer(ch, &ch->Imm->cr_idescr->general,
					"old general description");
			return;
		}
		if (is_name(argv[0], "extra"))
		{
			cr_descr_extra(ch, ch->Imm->cr_idescr->extra,
					argv[1], argv[2], argv[3]);
			return;
		}
		if (is_name(argv[0], "creators_note"))
		{
			ch_general_writer(ch, &ch->Imm->cr_idescr->creators_note,
					"old creators note");
			return;
		}
	}
	ch->put("@idescr [keyword | proper | room | general | extra | creators_note]\r\n");
}

void cr_cstats_thac0(Character *ch, const char *val)
{
	thac0_t thac0;
	if (val)
	{
		thac0 = atoi(val);

		if (thac0 < -3 || thac0 > 20)
		{
			ch->put("THAC0 must be a value between -3 and 20.\r\n");
			return;
		}
		ch->Imm->cr_cstats->thac0 = thac0;
		return;
	}
	ch->put("@cstats THAC0 <THAC0>\r\n");
}

void cr_cstats_hitpoints(Character *ch, const char *hitpoints)
{
	DMM dmm;
	if (hitpoints)
	{
		if ((sscanf(hitpoints, "%hud%hu+%hd",
						&dmm.multiplier, &dmm.die, &dmm.modifier)) == 3)
		{
			memcpy(&ch->Imm->cr_cstats->hitpoints, &dmm, sizeof(DMM));
			ch->put("Ok.\r\n");
			return;
		}
	}
	ch->put("@cstats hitpoints <multiplier>d<die>+<modifier>\r\n");
}

void cr_cstats_stat(Character *ch, stat_t stat_index, const char *val)
{
	stat_t stat_val;
	if (val)
	{
		stat_val = atoi(val);
		if (stat_val < 2 || stat_val > 18)
		{
			ch->print("The %s must be a value between 1 and 18.\r\n",
					str_stat[stat_index]);
			return;
		}
		ch->Imm->cr_cstats->special.stat[stat_index] = stat_val;
		ch->put("Ok.\r\n");
		return;
	}
	ch->print("@cstats %s <value>\r\n", str_stat[stat_index]);
}

void cr_cstats_saves(Character *ch, save_types_t save_index, const char *val)
{
	save_t save_val;
	if (val)
	{
		save_val = atoi(val);
		if (save_val < -5 || save_val > 5)
		{
			ch->print("The %s must be a value between -5 and 5.\r\n",
					str_saves[save_index]);

			return;
		}
		ch->Imm->cr_cstats->special.save_vs[save_index] = save_val;
		ch->put("Ok.\r\n");
		return;
	}
	ch->print("@cstats %s <value>\r\n", str_saves[save_index]);
}

void cr_cstats_attacks_add(Character *ch, const char *atk, const char *dam)
{
	if (atk && dam)
	{
		attack_t attack_type;
		DMM dmm;

		if ((attack_type
					= find_index(atk, str_attack_type)) != (ubyte1) -1
				&& sscanf(dam, "%hud%hu+%hd",
						&dmm.multiplier, &dmm.die, &dmm.modifier) == 3)
		{
			top_t attack_top = ++ch->Imm->cr_cstats->attack_top;
			REALLOC(ch->Imm->cr_cstats->attack, ATTACK, attack_top+1);
			ch->Imm->cr_cstats->attack[attack_top].type = attack_type;
			memcpy(&ch->Imm->cr_cstats->attack[attack_top].damage,
					&dmm, sizeof(DMM));
			ch->put("Ok.\r\n");
			return;
		}
	}
	ch->put("@cstats attacks add <type> <multiplier>d<die>+<modifier>\r\n"
			"<attack_type> can be:\r\n");
	list_flags_to(ch, (ubyte8) -1, str_attack_type);
}

void cr_cstats_attacks_delete(Character *ch, const char *atk_index)
{

	if (atk_index)
	{
		if (is_number(atk_index))
		{
			top_t index = atoi(atk_index);
			if (index < 0 || index > ch->Imm->cr_cstats->attack_top)
			{
				if (ch->Imm->cr_cstats->attack_top)
				{
					ch->print("The attack index must be between 0 and %d.\r\n",
							ch->Imm->cr_cstats->attack_top);
				}
				else
				{
					ch->put("The attack index must be 0.\r\n");
				}
				return;
			}
			memmove(&ch->Imm->cr_cstats->attack[index],
					&ch->Imm->cr_cstats->attack[index+1],
					sizeof(ATTACK)*(ch->Imm->cr_cstats->attack_top - index));
			ch->Imm->cr_cstats->attack_top--;
			REALLOC(ch->Imm->cr_cstats->attack, ATTACK,
					ch->Imm->cr_cstats->attack_top+1);
			ch->put("Ok.\r\n");
			return;
		}
	}
	ch->put("@cstats attacks delete <index>\r\n");
}

void cr_cstats_attacks_modify(Character *ch,
		const char *atk_index, const char *atk_type, const char *atk_damage)
{
	if (atk_index && atk_type && atk_damage)
	{
		if (is_number(atk_index))
		{
			top_t index = atoi(atk_index);
			if (index < 0 || index > ch->Imm->cr_cstats->attack_top)
			{
				if (ch->Imm->cr_cstats->attack_top)
				{
					ch->print("The attack index must be between 0 and %d.\r\n",
							ch->Imm->cr_cstats->attack_top);
				}
				else
				{
					ch->put("The attack index must be 0.\r\n");
				}
				return;
			}
			DMM dmm;
			attack_t attack_type;
			if ((attack_type
						= find_index(atk_type, str_attack_type)) != (ubyte1) -1
					&& sscanf(atk_damage, "%hud%hu+%hd",
						&dmm.multiplier, &dmm.die, &dmm.modifier) == 3)
			{
				ch->Imm->cr_cstats->attack[index].type = attack_type;
				memcpy(&ch->Imm->cr_cstats->attack[index].damage,
						&dmm, sizeof(DMM));
				ch->put("Ok.\r\n");
				return;
			}
		}
	}
	ch->put(
"@cstats attacks modify <index> <type> <multiplier>d<die>+<modifier>\r\n"
"<attack_type> can be:\r\n");
	list_flags_to(ch, (ubyte8) -1, str_attack_type);
}

void cr_cstats_attacks(Character *ch, char *cmd,
		char *arg1, char *arg2, char *arg3)
{
	if (cmd)
	{
		if (is_name(cmd, "add"))
		{
			cr_cstats_attacks_add(ch, arg1, arg2);
			return;
		}
		if (is_name(cmd, "delete"))
		{
			cr_cstats_attacks_delete(ch, arg1);
			return;
		}
		if (is_name(cmd, "modify"))
		{
			cr_cstats_attacks_modify(ch, arg1, arg2, arg3);
			return;
		}
	}
	ch->put("@cstats attacks [add | delete | modify]\r\n");
}

void do_cr_cstats(Character *ch, char *arg, cmd_t cmd)
{
	if (ch->Imm->cr_cstats == NULL)
	{
		ch->put("You need character stats in your buffer first.\r\n");
		return;
	}
	TOKENIZER(arg, argv);
	if (argv[0])
	{
		if (is_name(argv[0], "race"))
		{
			ch_set_index(ch, &ch->Imm->cr_cstats->race, argv[1],
					str_race, "@cstats race");
			return;
		}
		if (is_name(argv[0], "gender"))
		{
			ch_set_index(ch, &ch->Imm->cr_cstats->gender, argv[1],
					str_gender, "@cstats gender");
			return;
		}
		if (is_name(argv[0], "homeland"))
		{
			ch_set_index(ch, &ch->Imm->cr_cstats->homeland, argv[1],
					str_homeland, "@cstats homeland");
			return;
		}
		if (is_name(argv[0], "profession"))
		{
			ch_set_flag(ch, &ch->Imm->cr_cstats->profession, argv[1],
					str_profession, "@cstats profession");
			return;
		}
		if (is_name(argv[0], "THAC0"))
		{
			cr_cstats_thac0(ch, argv[1]);
			return;
		}
		if (is_name(argv[0], "movement"))
		{
			ch_set_index(ch, &ch->Imm->cr_cstats->movement_type, argv[1],
					str_movement, "@cstats movement");
			return;
		}
		if (is_name(argv[0], "act_flags"))
		{
			ch_set_flag(ch, &ch->Imm->cr_cstats->act_flags, argv[1],
					str_act_flags, "@cstats act_flags");
			return;
		}
		if (is_name(argv[0], "height"))
		{
			ch_set_index(ch, &ch->Imm->cr_cstats->height_class, argv[1],
					str_height_type, "@cstats height");
			return;
		}
		if (is_name(argv[0], "weight"))
		{
			ch_set_index(ch, &ch->Imm->cr_cstats->weight_class, argv[1],
					str_weight_type, "@cstats weight");
			return;
		}
		if (is_name(argv[0], "volume"))
		{
			ch_set_index(ch, &ch->Imm->cr_cstats->volume_class, argv[1],
					str_volume_type, "@cstats volume");
			return;
		}
		if (is_name(argv[0], "hitpoints"))
		{
			cr_cstats_hitpoints(ch, argv[1]);
			return;
		}
		if (is_name(argv[0], "citizenship"))
		{
			ch_set_flag(ch, &ch->Imm->cr_cstats->special.citizenship, argv[1],
					str_city, "@cstats citizenship");
			return;
		}
		if (is_name(argv[0], "affect_flags"))
		{
			ch_set_flag(ch, &ch->Imm->cr_cstats->special.affect_flags, argv[1],
					str_affect_flags, "@cstats affect_flags");
			return;
		}
		if (is_name(argv[0], "resistance_flags"))
		{
			ch_set_flag(ch, &ch->Imm->cr_cstats->special.resistance_flags, argv[1],
					str_immunity_flags, "@cstats resistance_flags");
			return;
		}
		if (is_name(argv[0], "immunity_flags"))
		{
			ch_set_flag(ch, &ch->Imm->cr_cstats->special.immunity_flags, argv[1],
					str_immunity_flags, "@cstats immunity_flags");
			return;
		}
		for (stat_t stat_index = 0; stat_index < NO_STAT; stat_index++)
		{
			if (is_name(argv[0], str_stat[stat_index]))
			{
				cr_cstats_stat(ch, stat_index, argv[1]);
				return;
			}
		}
		if (is_name(argv[0], "position"))
		{
			ch_set_index(ch, &ch->Imm->cr_cstats->special.position, argv[1],
					str_position, "@cstats position");
			return;
		}
		for (save_types_t save_index = 0; save_index < NO_SAVES; save_index++)
		{
			if (is_name(argv[0], str_saves[save_index]))
			{
				cr_cstats_saves(ch, save_index, argv[1]);
				return;
			}
		}
		if (is_name(argv[0], "attacks"))
		{
			cr_cstats_attacks(ch, argv[1], argv[2], argv[3], argv[4]);
			return;
		}
		if (is_name(argv[0], "creators_note"))
		{
			ch_general_writer(ch, &ch->Imm->cr_cstats->creators_note,
					"old creators note");
			return;
		}
	}
	ch->put(
"@cstats [race | gender | homeland | profession | THAC0 | movement |\r\n"
"         act_flags | height | weight | volume | hitpoints | citizenship |\r\n"
"         affect_flags | resistance_flags | immunity_flags | strength |\r\n"
"         dexterity | intelligence | wisdom | constitution | charisma |\r\n"
"         strength-bonus | position | paralyzations | staves |\r\n"
"         petrificaitons | breaths | spells | attacks | creators_note]\r\n");
}

void cr_istats_weight(Character *ch, const char *val)
{
	if (val)
	{
		weight_t weight;
		if (is_not_number(val))
		{
			ch->put("The weight value must be a number.\r\n");
			return;
		}
		ch->Imm->cr_istats->weight = weight;
		ch->put("Ok.\r\n");
	}
	ch->put("@istats weight <value>\r\n");
}

void cr_istats_volume(Character *ch, const char *val)
{
	if (val)
	{
		volume_t volume;
		if (is_not_number(val))
		{
			ch->put("The volume value must be a number.\r\n");
			return;
		}
		ch->Imm->cr_istats->volume = volume;
		ch->put("Ok.\r\n");
	}
	ch->put("@istats volume <value>\r\n");
}

void cr_istats_value(Character *ch, int index, const char *val)
{
	if (val)
	{
		if (is_not_number(val))
		{
			ch->print("The item value must be a number.\r\n");
			return;
		}
		ch->Imm->cr_istats->special.value[index] = atoi(val);
		ch->put("Ok.\r\n");
		return;
	}
	ch->print("@istats value%u <value>\r\n", index+1);
}

void do_cr_istats(Character *ch, char *arg, cmd_t cmd)
{
	if (ch->Imm->cr_istats == NULL)
	{
		ch->put("You need item stats data in your buffer first.\r\n");
		return;
	}
	TOKENIZER(arg, argv);
	if (argv[0])
	{
		if (is_name(argv[0], "type"))
		{
			ch_set_index(ch, &ch->Imm->cr_istats->item_type, argv[1],
					str_item_type, "@istats type");
			return;
		}
		if (is_name(argv[0], "attribute_flags"))
		{
			ch_set_flag(ch, &ch->Imm->cr_istats->attribute_flags, argv[1],
					str_attribute_flags, "@istats attribute_flags");
			return;
		}
		if (is_name(argv[0], "property_flags"))
		{
			ch_set_flag(ch, &ch->Imm->cr_istats->property_flags, argv[1],
					str_property_flags, "@istats property_flags");
			return;
		}
		if (is_name(argv[0], "materials"))
		{
			ch_set_flag(ch, &ch->Imm->cr_istats->material_flags, argv[1],
					str_material, "@istats materials");
			return;
		}
		if (is_name(argv[0], "worn"))
		{
			ch_set_flag(ch, &ch->Imm->cr_istats->wear_flags, argv[1],
					str_wear_pos_flags, "@istats worn");
			return;
		}
		if (is_name(argv[0], "weight"))
		{
			cr_istats_weight(ch, argv[1]);
			return;
		}
		if (is_name(argv[0], "volume"))
		{
			cr_istats_volume(ch, argv[1]);
			return;
		}
		if (is_name(argv[0], "value0"))
		{
			cr_istats_value(ch, 0, argv[1]);
			return;
		}
		if (is_name(argv[0], "value1"))
		{
			cr_istats_value(ch, 1, argv[1]);
			return;
		}
		if (is_name(argv[0], "value2"))
		{
			cr_istats_value(ch, 1, argv[1]);
			return;
		}
		if (is_name(argv[0], "value3"))
		{
			cr_istats_value(ch, 1, argv[1]);
			return;
		}
		if (is_name(argv[0], "affect_flags"))
		{
			ch_set_flag(ch, &ch->Imm->cr_istats->special.affect_flags, argv[1],
					str_affect_flags, "@istats affect_flags");
			return;
		}
		if (is_name(argv[0], "resistance_flags"))
		{
			ch_set_flag(ch, &ch->Imm->cr_istats->special.resistance_flags, argv[1],
					str_immunity_flags, "@istats resistance_flags");
			return;
		}
		if (is_name(argv[0], "immunity_flags"))
		{
			ch_set_flag(ch, &ch->Imm->cr_istats->special.immunity_flags, argv[1],
					str_immunity_flags, "@istats immunity_flags");
			return;
		}
		if (is_name(argv[0], "creators_note"))
		{
			ch_general_writer(ch, &ch->Imm->cr_istats->creators_note,
					"old creators note");
			return;
		}
	}
	ch->put(
"@istats [type | attribute_flags | property_flags | materials| worn | weight | volume | value0 | value1 | value2 | value3 | affect_flags | resistance_flags | immunity_flags | creators_note]\r\n");
}


void cr_room_tread(Character *ch, const char *val)
{
	int t;
	if (val)
	{
		t = atoi(val);
		if (is_not_number(val) || t < 0 || t > 99)
		{
			ch->put("The tread value must be a number between 0 and 99.\r\n");
			return;
		}
		ch->Imm->cr_room->tread = t;
		ch->put("Ok.\r\n");
		return;
	}
	ch->put("@room tread <value>\r\n");
}

void cr_room_coord(Character *ch, const char *v, const char *x, const char *y)
{
	int vid, xcoord, ycoord;
	if (v && x && y)
	{
		vid = atoi(v);
		xcoord = atoi(x);
		ycoord = atoi(y);

		if (vid == -1 && xcoord == -1 && ycoord == -1)
		{
			FREE(ch->Imm->cr_room->coord);
			ch->put("Ok.\r\n");
			return;
		}
		if (is_not_number(v) || vid < 0)
		{
			ch->put("The item virtual-id must be a number greater than 0.\r\n");
			return;
		}
		if (idataIndex.get(vid) == (vid_t) -1)
		{
			ch->put("That item virtual-id does not exist.\r\n");
			return;
		}
		if (is_not_number(x) || xcoord < 1)
		{
			ch->put("The x-coordinate must be a number greater than 0.\r\n");
			return;
		}
		if (is_not_number(y) || ycoord < 1)
		{
			ch->put("The y-coordinate must be a number greater than 0.\r\n");
			return;
		}

		if (ch->Imm->cr_room->coord == NULL)
		{
			MALLOC(ch->Imm->cr_room->coord, COORD, 1);
		}
		ch->Imm->cr_room->coord->v = vid;
		ch->Imm->cr_room->coord->x = xcoord;
		ch->Imm->cr_room->coord->y = ycoord;
		ch->put("Ok.\r\n");
		return;
	}
	ch->put("@room coord <item-vid> <x-coord> <y-coord>\r\n");
}

void cr_room_door_add(Character *ch, DOOR **door, dir_t dir)
{
	if (door[dir])
	{
		ch->print("The door %s already exists.\r\n", str_dir[dir]);
		return;
	}
	CALLOC(door[dir], DOOR, 1);
	door[dir]->key = new Words("room", "door");
	ch->put("Ok.\r\n");
}

void cr_room_door_delete(Character *ch, DOOR **door, dir_t dir)
{
	free_door(door[dir]);
	door[dir] = NULL;
	ch->put("Ok.\r\n");
}

void cr_room_door_add_keyword(Character *ch, DOOR *door, char *keyword)
{
	if (keyword)
	{
		if (door == NULL)
		{
			ch->put("The door does not exist.\r\n");
			return;
		}
		cr_descr_keyword(ch, door->key, "add", keyword);
		return;
	}
	ch->put("@room door <direction> addword <keyword>\r\n");
}

void cr_room_door_delete_keyword(Character *ch, DOOR *door, char *keyword)
{
	if (keyword)
	{
		if (door == NULL)
		{
			ch->put("The door does not exist.\r\n");
			return;
		}
		cr_descr_keyword(ch, door->key, "remove", keyword);
		return;
	}
	ch->put("@room door <direction> delword <keyword>\r\n");
}

void cr_room_door_weight(Character *ch, DOOR *door, const char *val)
{
	if (val)
	{
		int weight = atoi(val);
		if (is_not_number(val) || weight < 0 || weight > 99)
		{
			ch->put("Weight must be a number between 0 and 99.\r\n");
			return;
		}
		door->weight = weight;
		ch->put("Ok.\r\n");
		return;
	}
	ch->put("@room door <direction> weight <value>\r\n");
}

void cr_room_door_room(Character *ch, DOOR *door, const char *id)
{
	int vid = atoi(id);
	if (id)
	{
		if (is_not_number(id) || vid < 0)
		{
			ch->put("The room-vid must be a number greater than 0.\r\n");
			return;
		}
		Room *room;
		if ((room = find_room(vid)) == NULL)
		{
			ch->put("That room-vid does not exist.\r\n");
			return;
		}
		door->room = room;
		ch->put("Ok.\r\n");
		return;
	}
	ch->put("@room door <direction> room <room-vid>\r\n");
}

void cr_room_door_key(Character *ch, DOOR *door, const char *id)
{
	int vid = atoi(id);
	if (id)
	{
		if (is_not_number(id) || vid < 0)
		{
			ch->put("The key item-vid must be a number greater than 0.\r\n");
			return;
		}
		const ITEM_DATA *idata;
		if ((idata = find_item_data(vid)) == NULL)
		{
			ch->put("That key item-vid does not exist.\r\n");
			return;
		}
		door->room_key = idata;
		ch->put("Ok.\r\n");
		return;
	}
	ch->put("@room door <direction> key <item-id>\r\n");
}

void cr_room_door(Character *ch, DOOR **door, char *dir, char *cmd, char *arg)
{
	if (cmd && dir)
	{
		dir_t d;
		if ((d = find_index(dir, str_dir)) == (ubyte1) -1)
		{
			ch->put("<direction> must be one of:\r\n");
			list_flags_to(ch, (ubyte8) -1, str_dir);
			return;
		}
		if (is_name(cmd, "add"))
		{
			cr_room_door_add(ch, door, d);
			return;
		}
		if (door[d] == NULL)
		{
			ch->print("The door %s does not exist.\r\n", str_dir[d]);
			return;
		}
		if (is_name(cmd, "delete"))
		{
			cr_room_door_delete(ch, door, d);
			return;
		}
		if (is_name(cmd, "addword"))
		{
			cr_room_door_add_keyword(ch, door[d], arg);
			return;
		}
		if (is_name(cmd, "delword"))
		{
			cr_room_door_delete_keyword(ch, door[d], arg);
			return;
		}
		if (is_name(cmd, "proper"))
		{
			cr_descr_proper(ch, &door[d]->proper);
			return;
		}
		if (is_name(cmd, "description"))
		{
			ch_general_writer(ch, &door[d]->descr, "old door description");
			return;
		}
		if (is_name(cmd, "type"))
		{
			ch_set_index(ch, &door[d]->door_type, arg,
					str_door_type, "@room door <direction> type");
			return;
		}
		if (is_name(cmd, "weight"))
		{
			cr_room_door_weight(ch, door[d], arg);
			return;
		}
		if (is_name(cmd, "room"))
		{
			cr_room_door_room(ch, door[d], arg);
			return;
		}
		if (is_name(cmd, "key"))
		{
			cr_room_door_key(ch, door[d], arg);
			return;
		}
		if (is_name(cmd, "flags"))
		{
			ch_set_flag(ch, &door[d]->special.flags, arg,
					str_door_flags, "@room door <direction> flag");
			return;
		}
	}
	ch->put(
"@room door <direction> [add | delete | addword | delword | description |"
"                        type | weight | room | key | flags]\r\n");
}

void do_cr_room(Character *ch, char *arg, cmd_t cmd)
{
	if (ch->Imm->cr_room == NULL)
	{
		ch->put("You need room data in your buffer first.\r\n");
		return;
	}
	TOKENIZER(arg, argv);
	if (argv[0])
	{
		if (is_name(argv[0], "name"))
		{
			cr_descr_proper(ch, &ch->Imm->cr_room->name);
			return;
		}
		if (is_name(argv[0], "description"))
		{
			ch_general_writer(ch, &ch->Imm->cr_room->descr,
					"old room description");
			return;
		}
		if (is_name(argv[0], "storm_descr"))
		{
			ch_general_writer(ch, &ch->Imm->cr_room->descr_storm,
					"old dark room description");
			return;
		}
		if (is_name(argv[0], "dark_descr"))
		{
			ch_general_writer(ch, &ch->Imm->cr_room->descr_dark,
					"old storm room description");
			return;
		}
		if (is_name(argv[0], "extra"))
		{
			cr_descr_extra(ch, ch->Imm->cr_room->extra, argv[1], argv[2], argv[3]);
			return;
		}
		if (is_name(argv[0], "sector"))
		{
			ch_set_index(ch, &ch->Imm->cr_room->sector, argv[1],
					str_sector_type, "@room sector");
			return;
		}
		if (is_name(argv[0], "tread"))
		{
			cr_room_tread(ch, argv[1]);
			return;
		}
		if (is_name(argv[0], "coord"))
		{
			cr_room_coord(ch, argv[1], argv[2], argv[3]);
			return;
		}
		if (is_name(argv[0], "flags"))
		{
			ch_set_flag(ch, &ch->Imm->cr_room->special.flags, argv[1],
					str_room_flags, "@room flags");
			return;
		}
		if (is_name(argv[0], "door"))
		{
			cr_room_door(ch, ch->Imm->cr_room->door, argv[1], argv[2], argv[3]);
			return;
		}
		if (is_name(argv[0], "creators_note"))
		{
			ch_general_writer(ch, &ch->Imm->cr_room->creators_note,
					"old creators note");
			return;
		}
	}
	ch->put(
"@room [name | description | storm_descr | dark_descr | extra | sector |"
"       tread | coord | flags | door | creators_note]\r\n");
}

void cr_zone_levels(Character *ch, char *l1, char *l2)
{
	int level1, level2;
	if (l1 && l2)
	{
		if (is_not_number(l1) || is_not_number(l2)
				|| (level1 = atoi(l1)) < 0
				|| (level2 = atoi(l2)) < 0
				|| level1 > 33 || level2 > 33)
		{
			ch->print("Level must be a positive number less than %d.\r\n",
					MAX_PLAYER_LEVEL);
		}
		if (level1 > level2)
		{
			ch->put("Low-level must be less than high-level.\r\n");
			return;
		}
		ch->Imm->cr_zone->min_lvl  = level1;
		ch->Imm->cr_zone->max_lvl = level2;
		ch->put("Ok.\r\n");
		return;
	}
	ch->put("@zone levels <low-level> <high-level>\r\n");
}

void cr_zone_top(Character *ch, char *top)
{
	int t;
	if (top)
	{
		if (is_not_number(top) || (t = atoi(top)) >= MAX_ROOM_VID)
		{
			ch->print("Top must be a number greater than 1 and less than %d.\r\n",
					ch->Imm->cr_zone->virtual_id * 10);
			return;
		}
		Zone *next_z = get_next_zone(ch->Imm->cr_zone->virtual_id);
		if (next_z && t >= (sbyte4) next_z->getTop())
		{
			ch->print("Top cannot be greater than (%u) the next zone's top.\r\n",
					next_z->getTop());
			return;
		}
		if (next_z)
		{
			ch->print("Bottom of Z#%u set to %u.\r\n",
					next_z->getVID(),
					t+1);
		}
		ch->Imm->cr_zone->top = t;
		ch->put("Ok.\r\n");
		return;
	}
	ch->put("@zone top <room-number>\r\n");
}

void cr_zone_lifespan(Character *ch, char *lifespan)
{
	int l;
	if (lifespan)
	{
		if (is_not_number(lifespan)
				|| (l = atoi(lifespan)) < 0 || l > MAX_LIFESPAN)
		{
			ch->print(
				"Lifespan must be a positive number less than or equal to %d.\r\n",
				MAX_LIFESPAN);
			return;
		}
		ch->Imm->cr_zone->special.lifespan = l;
		ch->put("Ok.\r\n");
		return;
	}
	ch->put("@zone lifespan <minutes>\r\n");
}

void cr_zone_coord(Character *ch, const char *v, const char *x, const char *y)
{
	int vid, xcoord, ycoord;
	if (v && x && y)
	{
		vid = atoi(v);
		xcoord = atoi(x);
		ycoord = atoi(y);

		if (vid == -1 && xcoord == -1 && ycoord == -1)
		{
			FREE(ch->Imm->cr_zone->coord);
			ch->put("Ok.\r\n");
			return;
		}
		if (is_not_number(v) || vid < 0)
		{
			ch->put("The item virtual-id must be a number greater than 0.\r\n");
			return;
		}
		if (idataIndex.get(vid) == (vid_t) -1)
		{
			ch->put("That item virtual-id does not exist.\r\n");
			return;
		}
		if (is_not_number(x) || xcoord < 1)
		{
			ch->put("The x-coordinate must be a number greater than 0.\r\n");
			return;
		}
		if (is_not_number(y) || ycoord < 1)
		{
			ch->put("The y-coordinate must be a number greater than 0.\r\n");
			return;
		}

		if (ch->Imm->cr_zone->coord == NULL)
		{
			MALLOC(ch->Imm->cr_zone->coord, COORD, 1);
		}
		ch->Imm->cr_zone->coord->v = vid;
		ch->Imm->cr_zone->coord->x = xcoord;
		ch->Imm->cr_zone->coord->y = ycoord;
		ch->put("Ok.\r\n");
		return;
	}
	ch->put("@zone coord <item-vid> <x-coord> <y-coord>\r\n");
}

void do_cr_zone(Character *ch, char *arg, cmd_t cmd)
{
	if (ch->Imm->cr_zone == NULL)
	{
		ch->put("You need zone data in your buffer first.\r\n");
		return;
	}
	TOKENIZER(arg, argv);
	if (argv[0])
	{
		if (is_name(argv[0], "name"))
		{
			cr_descr_proper(ch, &ch->Imm->cr_zone->name);
			return;
		}
		if (is_name(argv[0], "description"))
		{
			ch_general_writer(ch, &ch->Imm->cr_zone->descr,
					"old zone description");
			return;
		}
		if (is_name(argv[0], "type"))
		{
			ch_set_index(ch, &ch->Imm->cr_zone->zone_type, argv[1], str_zone_type,
					"@zone type");
			return;
		}
		if (is_name(argv[0], "homeland"))
		{
			ch_set_flag(ch, &ch->Imm->cr_zone->homeland, argv[1], str_homeland,
					"@zone homeland");
			return;
		}
		if (is_name(argv[0], "levels"))
		{
			cr_zone_levels(ch, argv[1], argv[2]);
			return;
		}
		if (is_name(argv[0], "top"))
		{
			cr_zone_top(ch, argv[1]);
			return;
		}
		if (is_name(argv[0], "flags"))
		{
			ch_set_flag(ch, &ch->Imm->cr_zone->special.flags, argv[1],
					str_zone_flags, "@zone flags");
			return;
		}
		if (is_name(argv[0], "lifespan"))
		{
			cr_zone_lifespan(ch, argv[1]);
			return;
		}
		if (is_name(argv[0], "coord"))
		{
			cr_zone_coord(ch, argv[1], argv[2], argv[3]);
			return;
		}
		if (is_name(argv[0], "creators_note"))
		{
			ch_general_writer(ch, &ch->Imm->cr_zone->creators_note,
					"old creators note");
		}
	}
	ch->put(
"@zone [name | description | type | homeland | top | flags | lifespan |"
"       coord | creators_note]\r\n");
}
