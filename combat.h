/* **************************************************************************
    Nilgiri the Forgotten World
    (C) 2004
   ************************************************************************** */
/** \file
    \brief combat activity header
 */
#ifndef _x_COMBAT_MODULE_INCLUDE_x_
#define _x_COMBAT_MODULE_INCLUDE_x_

#include"tinyworld_character.h"

typedef struct _fight
{
	CHARACTER_CTX *chCTX;
	CHARACTER_CTX *victCTX;
	struct _fight *prev;
	struct _fight *next;
}
FIGHT;

extern void init_combat_module(void);
extern void initiate_hit(Character *, Character *);
extern void free_combatCTX(CHARACTER_CTX *);
extern FIGHT *add_fight(Character *, Character *);
#endif
