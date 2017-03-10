/* **************************************************************************
    Nilgiri the Forgotten World
    (C) 2004
    tinyworld_item.h: object data
   ************************************************************************** */
/** \file
    \brief Item container object header
 */
#ifndef _x_ITEM_CONTAINER_INCLUDE_x_
#define _x_ITEM_CONTAINER_INCLUDE_x_
#include"tinyworld_item.h"

class ItemContainer : public Item
{
public:
	ItemContainer(ITEM_DATA *idata, cid_t loader_id=0, cid_t container_id=0);
};

#endif
