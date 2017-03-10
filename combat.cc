/* **************************************************************************
    Nilgiri the Forgotten World
    (C) 2004
   ************************************************************************** */
/** \file
    \brief combat activity source
 */
#include"combat.h"
#include"tinyworld_room.h"
#include"addlog.h"

/** The list of combat that has stopped and needs to be freed */
static CHARACTER_CTX **combat_ctx_free_list = NULL;
/** The top index of the combat_ctx_free_list */
static int combat_ctx_free_list_top = -1;

/** The list of combatants */
static FIGHT *fight_list = NULL;
/** Pointer to the last item in the fight list */
static FIGHT *last_in_fight_list = NULL;
/** The top index of the fight_list */
static int fight_list_top = -1;

/** This is for delaying any freeing of combatCTX's that are hidden in the
    fight_list before the fight_list has a change to remove its fight.
 */
static int just_freed_combat_ctx = 0;

/** Clears the character combat context and places the character combat context
    in free_combat_ctx_free_list to be free'd later.
 */
void free_combatCTX(CHARACTER_CTX *chCTX)
{
	just_freed_combat_ctx = 2;
	assert(chCTX);
	chCTX->character = NULL;
	REALLOC(combat_ctx_free_list, CHARACTER_CTX *, combat_ctx_free_list_top+2);
	combat_ctx_free_list_top++;
	combat_ctx_free_list[combat_ctx_free_list_top] = chCTX;
}

/** Perform freeing of all the fighters in the combat_ctx_free_list */
static void free_combat_ctx_free_list(void)
{
	for (int i = 0; i <= combat_ctx_free_list_top; i++)
	{
		free(combat_ctx_free_list[i]);
	}
	free(combat_ctx_free_list);
	combat_ctx_free_list = NULL;
	combat_ctx_free_list_top = -1;
	just_freed_combat_ctx = 0;
}

/** Adds a combat to the fight_list
    \param ch The character attempting assault
	 \param vict The victim of the assault
 */
FIGHT *add_fight(Character *ch, Character *vict)
{
	assert(ch && vict);

	FIGHT *fight;

	MALLOC(fight, FIGHT, 1);
	fight->chCTX = ch->getCombatCTX();
	fight->victCTX = vict->getCombatCTX();
	fight->prev = last_in_fight_list;
	fight->next = NULL;
	
	if (fight_list)
	{
		last_in_fight_list->next = fight;
		last_in_fight_list = fight;
	}
	else
	{
		fight_list = last_in_fight_list = fight;
	}
	return fight;
}

/** This is a removes one fight from a doubly-linked list of fights.
    \param fight The fight to remove
 */
static void remove_fight(FIGHT *fight)
{
	if (fight->prev)
	{
		fight->prev->next = fight->next;
	}
	else
	{
		fight_list = fight->next;
	}
	if (fight->next)
	{
		fight->next->prev = fight->prev;
	}
	else
	{
		last_in_fight_list = fight->prev;
	}
	if (fight->chCTX->character)
	{
		fight->chCTX->character->setFighting(NULL);
	}
	free(fight);
}

void perform_init_death_blow(Character *vict, Character *ch)
{
	ch->echo(ch, vict, K_BLD_RED "You kill $N!");

	Container *bystander = ch->inContainer()->getHeadContent();
	for (; bystander; bystander = bystander->getNextContent())
	{
		if (bystander != ch && bystander != vict)
		{
			ch->echoTo(bystander, vict, K_BLD_RED "$n kills $N!");
		}
	}
	ch->echoTo(vict, K_BLD_RED "$n kills you!\r\n");
	addlog_death(vict, ch);
	vict->extract();
}

void perform_death_blow(Character *vict, Character *ch)
{
	ch->echoTo(ch, vict, K_BLD_RED "You kill $N!");

	Container *bystander = ch->inContainer()->getHeadContent();
	for (; bystander; bystander = bystander->getNextContent())
	{
		if (bystander != ch && bystander != vict)
		{
			ch->echoTo(bystander, vict, K_BLD_RED "$n kills $N!");
		}
	}
	ch->echoTo(vict, K_BLD_RED "$n kills you!\r\n");
	addlog_death(vict, ch);
	vict->extract();
}

/** Land the first blow and then add that to the fight_list
	 \param vict The victim of the assault
    \param ch The character attempting the assault
 */
void initiate_hit(Character *vict, Character *ch)
{
	hitpoints_t hits = -2;

	ch->echo(vict, K_WHT "You hit $N.");
	vict->addHitpoints(hits);
	ch->echoTo(vict, K_BLD_RED "$n hits you.");
	ch->inContainer()->echo(ch, vict, K_RED "$n hits $N.");
	if (vict->isDead())
	{
		perform_init_death_blow(vict, ch);
		return;
	}
	vict->setFighting(ch);
	ch->setFighting(vict);
}

/** The subsequent hits following the initial hit as controlled by the
     fight_list
    \param vict The victim of the assault
	 \param ch The character attempting the assault
 */
static void hit(Character *vict, Character *ch)
{
	hitpoints_t hits = -2;

	ch->echoTo(ch, vict, K_WHT "You hit $N.");
	vict->addHitpoints(hits);
	ch->echoTo(vict, K_BLD_RED "$n hits you.");

	Container *bystander = ch->inContainer()->getHeadContent();
	for (; bystander; bystander = bystander->getNextContent())
	{
		if (bystander != ch && bystander != vict)
		{
			ch->echoTo(bystander, vict, K_RED "$n hits $N.");
		}
	}
	if (vict->isDead())
	{
		perform_death_blow(vict, ch);
	}
}

/** Checks to see if a fight can continue, if so perform the assault
    \param fight The fight we are checking
 */
static void deal_damage(FIGHT *fight)
{
	Character *ch;
	Character *vict;

	if ((ch = fight->chCTX->character)==NULL || ch->inContainer()==NULL)
	{
		remove_fight(fight);
		return;
	}
	if ((vict = fight->victCTX->character)==NULL || vict->inContainer()==NULL)
	{
		remove_fight(fight);
		return;
	}

	if (ch->inContainer() != vict->inContainer())
	{
		remove_fight(fight);
		return;
	}

	hit(vict, ch);
	if (vict->isDead()==0 && vict->fighting()==NULL)
	{
		vict->setFighting(ch);
	}
}

/** The combat round of events */
static void combat_activity(void *ctx=NULL)
{
	FIGHT *fight;
	FIGHT *fight_next;
	for (fight = fight_list; fight; fight = fight_next)
	{
		fight_next = fight->next;
		deal_damage(fight);
	}
	if (just_freed_combat_ctx && --just_freed_combat_ctx==0)
	{
		free_combat_ctx_free_list();
	}
	queue_event(COMBAT_ACTIVITY_TIMEs, COMBAT_ACTIVITY_TIMEus,
		combat_activity, ctx);
}

/** Initialize the combat module */
void init_combat_module(void)
{
	combat_activity();
}
