/* **************************************************************************
    Nilgiri the Forgotten World
    (C) 2004
   ************************************************************************** */
/** \file
    \brief general other game commands module
 */
#include"tinyworld_character.h"
#include"descriptor.h"

void do_quit(Character *ch, char *arg, cmd_t cmd)
{
	ch->echo("Goodbye, friend.. Come back again soon!");
	ch->inContainer()->echo(ch, "$g$n has left the game.");
	((Player *) ch)->extract();
}
