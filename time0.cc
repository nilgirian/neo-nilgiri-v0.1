/* **************************************************************************
    Nilgiri the Forgotten World
    (C) 2004
    time0.cc: central time and timing
   ************************************************************************** */
/** \file 
    \brief central time and timing source
 */

#include<errno.h>
#include<string.h>
#include"time0.h"
#include"err.h"
#include"utils.h"

/** We should never schedule more events than stated here. */
#define MAX_EVENTS 256

/** This is a global variable that is set to the number of seconds since the
    Epoch.  This variable should only be read and never set.  It is set by
    the deliver_events() function.  This variable is dependent on the
	 timeval0 variable.
 */
time_t time0;
struct timeval timeval0;

static EVENT *event_list[MAX_EVENTS];
static size_t event_list_len = 0;

/** This function compares events at index i1, and i2.  We assume i1 and
    i2 are in the event_list.
    \param i1 - first index of an event to compare
	 \param i2 - second index of an event to compare
    \return 1 - event at i1 is less than event at i2, 0 - otherwise
  */
inline int queue_lt(size_t i1, size_t i2)
{
	if (event_list[i1]->schedule.tv_sec < event_list[i2]->schedule.tv_sec)
	{
		return 1;
	}
	if (event_list[i1]->schedule.tv_sec == event_list[i2]->schedule.tv_sec)
	{
		return event_list[i1]->schedule.tv_usec < event_list[i2]->schedule.tv_usec;
	}
	return 0;
}

/** Swaps the events.  This assumes all indexes exist.
    \param i1 The first index of the event to swap
	 \param i2 The second index of the event to swap
 */
inline void swap_event(size_t i1, size_t i2)
{
	EVENT *tmp;
	tmp = event_list[i1];
	event_list[i1] = event_list[i2];
	event_list[i2] = tmp;
}
/** Upheap function for the event_list
    \param index The index where to perform the upheap from
 */
static void upheap_event(size_t index)
{
	int pindex;
	if ((pindex = parent_heap(index)) > 0)
	{
		if (queue_lt(index-1, pindex-1))
		{
			swap_event(index-1, pindex-1);
			upheap_event(pindex);
		}
	}
}

/** This queues an event.  The event can queue itself to create a recurring
    event.  This function inserts events in order of when the event is to
	 occur.  Events happening at the same instance may occur in random order.
	 \param offset_sec The number of secs from now when this event will occur
	 \param offset_usec The number of usecs from now when this event will occur
    \param fn The event function
    \param ctx The pointer argument for the event function
 */
void queue_event(long offset_sec, long offset_usec,
		void (*fn)(void *), void *ctx)
{
	EVENT *event;

	if (event_list_len == MAX_EVENTS)
	{
		exitlog("/MAX_EVENTS");
	}

	MALLOC(event, EVENT, 1);
	event->schedule.tv_usec = timeval0.tv_usec + offset_usec;
	event->schedule.tv_sec
		= timeval0.tv_sec + offset_sec + (event->schedule.tv_usec/1000000);
	event->schedule.tv_usec %= 1000000;
	event->fn = fn;
	event->ctx = ctx;

	event_list_len++;
	event_list[event_list_len-1] = event;
	upheap_event(event_list_len);
}

/** Downheap function for the event list.
    \param index The index where to perform the downheap from
 */
void downheap_event(size_t index)
{
	size_t lindex = left_heap(index);
	size_t rindex = right_heap(index);
	size_t small_index;

	if (lindex <= event_list_len && queue_lt(lindex-1, index-1))
	{
		small_index = lindex;
	}
	else
	{
		small_index = index;
	}
	if (rindex <= event_list_len && queue_lt(rindex-1, small_index-1))
	{
		small_index = rindex;
	}
	if (small_index != index)
	{
		swap_event(index-1, small_index-1);
		downheap_event(small_index);
	}
}

/** This function compares timevals of tv1, and tv2 and looks for greater than
    \param tv1 - first timeval
	 \param tv2 - second timeval
    \return 1 - tv1 is greater than to tv2, 0 - otherwise
  */
inline int timeval_gt(timeval *tv1, timeval *tv2)
{
	if (tv1->tv_sec > tv2->tv_sec)
	{
		return 1;
	}
	if (tv1->tv_sec == tv2->tv_sec)
	{
		return tv1->tv_usec > tv2->tv_usec;
	}
	return 0;
}

/** Finds the events that correspond to the current time as well as the events
    that should have occured previous to the current time and then executes
    the event function.
	 \todo possibly use memmove() for efficiency ?
	 \todo deliver events in blocks instead of one at a time
 */
void deliver_events(void)
{
	gettimeofday(&timeval0, NULL);
	time0 = timeval0.tv_sec;

	while (event_list_len > 0 && timeval_gt(&timeval0, &event_list[0]->schedule))
	{
		event_list[0]->fn(event_list[0]->ctx);
		free(event_list[0]);

		event_list[0] = event_list[event_list_len-1];
		if (--event_list_len)
		{
			downheap_event(1);
		}
	}
}

void queue_game_load_average(void *ctx)
{
	static clock_t clks = (clock_t) -1;
	clock_t now;
	if ((now = clock()) == (clock_t) -1)
	{
		clks = (clock_t) -1;
	}
	else
	{
		if (clks != (clock_t) -1)
		{
			memmove(game_stat.load_average+1, game_stat.load_average,
					14 * sizeof(unsigned long int));
			game_stat.load_average[0] = (now - clks)/(CLOCKS_PER_SEC);
		}
		clks = now;
		//addlog("/load average/%u (%d)", now, game_stat.load_average[0]);
	}
	queue_event(60*1, 0, queue_game_load_average, NULL);
}

/** Initialize the time0 module.  This should be the first init function
    instantiated.
 */
void init_time0_module(void)
{
	gettimeofday(&timeval0, NULL);
	time0 = timeval0.tv_sec;
	queue_game_load_average(NULL);
}
