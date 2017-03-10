/* **************************************************************************
    Nilgiri the Forgotten World
    (C) 2004
   ************************************************************************** */
/** \file
    \brief Item container object source
 */
#include"item_container.h"

ItemContainer::ItemContainer(ITEM_DATA *idata, cid_t loader, cid_t container_id)
: Item(idata, loader, container_id)
{
	assert(getType() == ITEM_CONTAINER);
}

