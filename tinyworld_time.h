/* **************************************************************************
    Nilgiri the Forgotten World
    (C) 2004
   ************************************************************************** */
/** \file
    \brief tinyworld time header
 */

#ifndef _x_TINYWORLD_TIME_INCLUDE_x_
#define _x_TINYWORLD_TIME_INCLUDE_x_
#include<time.h>
#include"utils.h"

enum MUD_MMONTHS
{
	MONTH_LIGHT,
	MONTH_WATERS,
	MONTH_EARTH,
	MONTH_SKY,
	MONTH_CREATURES,
	MONTH_MAN,
	MONTH_REST,
	MONTH_SIN,
	MONTH_ARK,
	MONTH_EXODUS,
	MONTH_PROMISE,
	MONTH_SAVIOR,
	NO_MMONTH
};

enum MUD_MDAYS
{
	DAY_ANCIENT_OF_DAYS,
	DAY_ANGELS,
	DAY_ARCHANGELS,
	DAY_VIRTUES,
	DAY_POWERS,
	DAY_PRINCIPALITIES,
	DAY_DOMINATIONS,
	DAY_THRONES,
	DAY_CHERUBIM,
	DAY_SERAPHIM,
	DAY_RAPHAEL,
	DAY_MICHAEL,
	DAY_GABRIEL,
	DAY_ABRAHAM,
	DAY_MOSES,
	DAY_AARON,
	DAY_MARIA,
	DAY_ELDAD,
	DAY_MEDAD,
	DAY_DEBBORA,
	DAY_HOLDA,
	DAY_SAMUEL,
	DAY_NATHAN,
	DAY_GAD,
	DAY_ISAIAH,
	DAY_ELISEUS,
	DAY_MATHEW,
	DAY_MARK,
	DAY_LUKE,
	DAY_JOHN,
	NO_MDAY
};

extern const char *str_mmonth[];
extern const char *str_mday[];

#define MSECS_PER_SEC 48
#define SEVENTEEN_MYEARS (17 * MSECS_PER_MYEAR)

#define MSECS_PER_MMONTH (SECS_PER_DAY * NO_MDAY)
#define MSECS_PER_MYEAR (MSECS_PER_MMONTH * NO_MMONTH)

#define HOUR_SUNRISE 5
#define HOUR_SUNLIGHT 6
#define HOUR_SUNSET 19
#define HOUR_SUNDOWN 20

inline time_t myears(time_t t)
{
	return t / MSECS_PER_MYEAR;
}

inline time_t mmonths(time_t t)
{
	return ((t % MSECS_PER_MYEAR) / MSECS_PER_MMONTH)+1;
}

inline time_t mdays(time_t t)
{
	return ((t % MSECS_PER_MMONTH) / SECS_PER_DAY)+1;
}

inline time_t mhours(time_t t)
{
	return (t % SECS_PER_DAY) / SECS_PER_HOUR;
}

inline time_t mminutes(time_t t)
{
	return (t % SECS_PER_HOUR) / SECS_PER_MIN;
}

inline time_t mseconds(time_t t)
{
	return t % SECS_PER_MIN;
}

inline time_t mtime0(void)
{
	return ((time0 - game_stat.time_zero) * MSECS_PER_SEC)+SEVENTEEN_MYEARS;
}

extern const char *mtime_str(time_t);
extern const char *mtime_ago(time_t);
extern const char *cmclock(void);
extern const char *cmdate(time_t);
extern void init_tinyworld_time_module(void);
#endif
