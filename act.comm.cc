/* **************************************************************************
    Nilgiri the Forgotten World
    (C) 2004
   ************************************************************************** */
/** \file
    \brief general communication game commands source
 */
#include"tinyworld_character.h"

void do_say(Character *ch, char *arg, cmd_t cmd)
{
	size_t s;
	char *p, last_char = '\0';

	assert(arg);

	s = 0;
	for (p = arg; *p; p++)
	{
		s++;
	}
	if (s > 1)
	{
		last_char = *(arg+s-1);
	}

	ch->echo("You %s, \"%s\"",
			last_char == '!' ? "exclaim" :
			last_char == '?' ? "ask"  : "say",
			arg);
	ch->inContainer()->echo(ch, "$g$n %s, \"%s\"",
			last_char == '!' ? "exclaims" :
			last_char == '?' ? "asks" : "says",
			arg);
}

