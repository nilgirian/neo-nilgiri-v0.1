/* **************************************************************************
    Nilgiri the Forgotten World
    (C) 2004
   ************************************************************************** */
/** \file
    \brief general offensive game commands module
 */
#include"tinyworld_character.h"
#include"combat.h"
#include"addlog.h"

void do_hit(Character *ch, char *arg, cmd_t cmd)
{
	Character *vict;
	char victname[MAX_SHORT_DESCR_SIZE];
	one_argument(arg, victname);

	if ((vict = ch->inContainer()->findCharacter(victname, ch))==NULL)
	{
		ch->echo("They are not here.");
		return;
	}
	if (vict == ch)
	{
		ch->echo("You hit yourself... OUCH!");
		ch->addHitpoints(-1);
		ch->inContainer()->echo(ch, "$g$n hits $mself and says, \"OUCH!\"");
		if (ch->isDead())
		{
			ch->inContainer()->echo(ch, "$g$n has killed $mself!");
			ch->put(K_BLD_RED "You are dead. R.I.P.\r\n");
			ch->extract();
		}
		return;
	}
	if (ch->fighting())
	{
		ch->echo("You are already fighting!");
		return;
	}
	addlog_offensive(ch, vict, cmd);
	initiate_hit(vict, ch);
}
