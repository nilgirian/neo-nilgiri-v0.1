/* **************************************************************************
    Nilgiri the Forgotten World
    (C) 2004
   ************************************************************************** */
/** \file 
    \brief Things of spells and magic header
 */
#ifndef _x_SPELL_HEADER_x_
#define _x_SPELL_HEADER_x_

#include"constants.h"
#include"container.h"
#include"tinyworld_character.h"
#include"tinyworld_item.h"
#include"utils.h"

struct _spell_struct
{
public:
	ubyte2 type;
	char *name;
	ubyte1 min_level[NO_PROFESSION];
	ubyte1 min_position;
	ubyte1 min_usemana;
	ubyte1 delay;
	ubyte2 types;
	ubyte2 targets;
	int (*fn)(ubyte1, Character *, char *, Character *, Item *, int);
}
SPELL_STRUCT;

#endif
