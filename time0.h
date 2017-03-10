/* **************************************************************************
    Nilgiri the Forgotten World
    (C) 2004
   ************************************************************************** */
/** \file
    \brief central time and timing header
 */

#ifndef _x_TIME0_INCLUDE_x_
#define _x_TIME0_INCLUDE_x_

#include<time.h>
#include<sys/time.h>

/** \brief the event structure */
/** The event structure keeps a basic set of information for triggering events.
    The event should be in a <I>void fn(void *)</I> function format.  The event
    function takes in a void pointer argument for passing data.  The schedule
    indicates at what time after the Epoch to trigger the event.
 */
typedef struct _event
{
  timeval schedule;
  void (*fn)(void *);
  void *ctx;
}
EVENT;

extern void init_time0_module(void);
extern void queue_event(long, long, void (*)(void *), void *);
extern void deliver_events(void);

extern time_t time0;
extern timeval timeval0;

inline void queue_event(long offset_usec, void (*fn)(void *), void *ctx)
{
	queue_event(0, offset_usec, fn, ctx);
}

#define POINT_UPDATE_TIMEs 75
#define POINT_UPDATE_TIMEus 0

#define COMBAT_ACTIVITY_TIMEs 3
#define COMBAT_ACTIVITY_TIMEus 500000

#endif
