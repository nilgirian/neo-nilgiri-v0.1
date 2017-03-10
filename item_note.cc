/* **************************************************************************
    Nilgiri the Forgotten World
    (C) 2004
    tinyworld_item.h: object data
   ************************************************************************** */
/** \file
    \brief Item note object source
 */
#include"item_note.h"
#include"tinyworld_character.h"

ItemNote::ItemNote(ITEM_DATA *idata, cid_t loader, cid_t container_id)
: Item(idata, loader, container_id)
{
	assert(getType() == ITEM_NOTE);
	message = NULL;
}

ItemNote::~ItemNote(void)
{
	free(message);
}

void ItemNote::writer(Character *ch)
{
	cid_writer = ch->getCID();
	ch->writer(&message);
}

void ItemNote::pageTo(Container *c)
{
	if (message == NULL)
	{
		c->put("The note is blank.\r\n");
		return;
	}
	c->put("There is something written upon it:\r\n");
	c->put(message);
}
