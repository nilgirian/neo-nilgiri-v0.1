/* **************************************************************************
    Nilgiri the Forgotten World
    (C) 2004
    tinyworld_item.h: object data
   ************************************************************************** */
/** \file
    \brief Item note object header
 */
#ifndef _x_ITEM_NOTE_INCLUDE_x_
#define _x_ITEM_NOTE_INCLUDE_x_
#include"tinyworld_item.h"

class ItemNote : public Item
{
public:
	ItemNote(ITEM_DATA *idata, cid_t loader_id=0, cid_t container_id=0);
	~ItemNote(void);
	void writer(Character *);
	void pageTo(Container *);
	inline cid_t getWriterCID(void);
	
private:
	char *message;
	cid_t cid_writer;
};

inline cid_t ItemNote::getWriterCID(void)
{
	return cid_writer;
}

#endif
