/* **************************************************************************
    Nilgiri the Forgotten World
    (C) 2004
   ************************************************************************** */
/** \file
    \brief general item game commands module
 */
#include"tinyworld_item.h"
#include"tinyworld_character.h"
#include"item_note.h"

void do_get(Character *ch, char *arg, cmd_t cmd)
{
	Item *item;

	if ((item = searchItem(arg, ch))==NULL)
	{
		ch->put("No item found by that name.\r\n");
		return;
	}
	ch->hold(item);
	ch->print("You get %s.\r\n", item->getName());
	ch->inContainer()->echo(ch, item, "$g$n gets $N.");
}

void do_drop(Character *ch, char *arg, cmd_t cmd)
{
	Item *item;

	if ((item = searchItem(arg, ch))==NULL)
	{
		ch->put("No item found by that name.\r\n");
		return;
	}
	ch->inContainer()->hold(item);
	ch->print("You drop %s.\r\n", item->getName());
	ch->inContainer()->echo(ch, item, "$g$n drops $N.");
}

void do_write(Character *ch, char *arg, cmd_t cmd)
{
	ItemNote *note;

	note = (ItemNote *) create_item(ITEM_NOTE_VID, ch);

	note->writer(ch);
}

/** \todo size probably should be less than MAX_SHORT_DESCR_SIZE */
void do_read(Character *ch, char *arg, cmd_t cmd)
{
	Item *note;
	char notename[MAX_SHORT_DESCR_SIZE];

	one_argument(arg, notename);
	if ((note = searchItem(notename, ch)) == NULL)
	{
		ch->put("No item found by that name.\r\n");
		return;
	}
	if ((note->getType() != ITEM_NOTE))
	{
		ch->put("This is not readable.\r\n");
		return;
	}
	((ItemNote *) note)->pageTo(ch);
}

