/* **************************************************************************
    Nilgiri the Forgotten World
    (C) 2004
   ************************************************************************** */
/** \file
    \brief general game movement commands source
 */
#include"tinyworld_character.h"

void do_dir(Character *ch, char *arg, cmd_t cmd)
{
	Room *room;
	extern const char *strDir[];
	if ((room = ch->inContainer()->objRoom())==NULL)
	{
		ch->put("The exits here are not that apparant.\r\n");
		return;
	}
	if ((room = room->getRoomDir(cmd))==NULL)
	{
		ch->put("Alas, you cannot go that way.\r\n");
		return;
	}
	ch->moveTo(room, cmd);
}
