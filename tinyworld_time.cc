/* **************************************************************************
    Nilgiri the Forgotten World
    (C) 2004
   ************************************************************************** */
/** \file
    \brief tinyworld time source
 */
#include"tinyworld_time.h"
#include"descriptor.h"

const char *str_mday[NO_MDAY+1] =
{
	"Ancient of Days",
	"Angels",
	"Archangels",
	"Virtues",
	"Powers",
	"Principalities",
	"Dominations",
	"Thrones",
	"Cherubim",
	"Seraphim",
	"Raphael",
	"Michael",
	"Gabriel",
	"Abraham",
	"Moses",
	"Aaron",
	"Maria",
	"Eldad",
	"Medad",
	"Debbora",
	"Holda",
	"Samuel",
	"Nathan",
	"Gad",
	"Isaiah",
	"Eliseus",
	"Mathew",
	"Mark",
	"Luke",
	"John",
	"\n"
};

const char *str_mmonth[NO_MMONTH+1] =
{
	"Light",
	"Waters",
	"Earth",
	"Sky",
	"Creatures",
	"Man",
	"Rest",
	"Sin",
	"Flood",
	"Exodus",
	"Promise",
	"Savior",
	"\n"
};

const char *mtime_ago(time_t t)
{
	if (t == 0)
	{
		return "no time";
	}

	static char buf[LINE_LENGTH];
	size_t buflen;

	buf[0] = '\0';
	buflen = 0;

	if (myears(t))
	{
		buflen += sprintf(buf+buflen, "%s%lu year%s",
				buflen ? " " : "",
				myears(t),
				myears(t) == 1 ? "" : "s");
	}
	if (mmonths(t))
	{
		buflen += sprintf(buf+buflen, "%s%lu month%s",
				buflen ? " " : "",
				mmonths(t),
				mmonths(t) == 1 ? "" : "s");
	}
	if (mdays(t))
	{
		buflen += sprintf(buf+buflen, "%s%lu day%s",
				buflen ? " " : "",
				mdays(t),
				mdays(t) == 1 ? "" : "s");
	}
	if (mhours(t))
	{
		buflen += sprintf(buf+buflen, "%s%lu hour%s",
				buflen ? " " : "",
				mhours(t),
				mhours(t) == 1 ? "" : "s");
	}
	if (mminutes(t))
	{
		buflen += sprintf(buf+buflen, "%s%lu minute%s",
				buflen ? " " : "",
				mminutes(t),
				mminutes(t) == 1 ? "" : "s");
	}
	if (mseconds(t))
	{
		buflen += sprintf(buf+buflen, "%s%lu second%s",
				buflen ? " " : "",
				mseconds(t),
				mseconds(t) == 1 ? "" : "s");
	}
	return buf;
}

const char *mtime_str(time_t t)
{
	static char buf[LONG_LINE_LENGTH+1];

	if (t == 0)
	{
		t = mtime0();
	}

	sprintf(buf, "%lu:%02lu:%02lu %s on the Day of %s\r\nThe %lu%s Day of the Month of %s in the Year %lu of Our Lord",
			mhours(t)==0 ? 12 :  mhours(t) > 12 ? mhours(t)-12 : mhours(t),
			mminutes(t),
			mseconds(t),
			mhours(t) >= 12  ? "P.M." : "A.M.",
			str_mday[mdays(t)-1],
			mdays(t),
			ordinalize(mdays(t)),
			str_mmonth[mmonths(t)-1],
			myears(t));
	return buf;
}

const char *cmclock(void)
{
	static char buf[LINE_LENGTH+1];

	sprintf(buf, "%02lu:%02lu:%02lu",
			mhours(mtime0()),
			mminutes(mtime0()),
			mseconds(mtime0()));
	return buf;
}

const char *cmdate(time_t t)
{
	static char buf[LINE_LENGTH+1];

	if (t == 0)
	{
		t = mtime0();
	}
	sprintf(buf, "Day of %s, Month of %s, Year %lu of Our Lord",
			str_mday[mdays(t)-1],
			str_mmonth[mmonths(t)-1],
			myears(t));
	return buf;
}

static void send_to_outdoors(const char *format, ...)
{

	int n, size = CHUNK_SIZE_OUT;
	int is_complete = 0;
	char *msg_s = NULL;
	char msg[CHUNK_SIZE_OUT];
	va_list vargs;

	while (1)
	{
		va_start(vargs, format);
		n = vsnprintf(msg_s ? msg_s : msg, size, format, vargs);
		va_end(vargs);
		msg_s = va_complete(&is_complete, msg_s, msg, &size, n);
		if (is_complete)
		{
			break;
		}
	}
	for (Descriptor *d = descriptor_list; d; d = d->getNext())
	{
		if (d->isPlayingGame() && d->envFlag(DENV_WEATHER))
		{
			d->put("\r\n");
			d->put(msg_s ? msg_s : msg);
		}
	}
	if (msg_s)
	{
		free(msg_s);
	}
}

/**
   \todo Would be nice if the mud wished people a happy birthday
 */
void mud_time_update(void *ctx)
{
	static int event = 0;
	time_t msec_of_the_day = mtime0() % SECS_PER_DAY;
	if (event == 0)
	{
		if (msec_of_the_day < HOUR_SUNRISE * SECS_PER_HOUR)
		{
			event = HOUR_SUNRISE;
		}
		else if (msec_of_the_day < HOUR_SUNLIGHT * SECS_PER_HOUR)
		{
			event = HOUR_SUNLIGHT;
		}
		else if (msec_of_the_day < HOUR_SUNSET * SECS_PER_HOUR)
		{
			event = HOUR_SUNSET;
		}
		else if (msec_of_the_day < HOUR_SUNDOWN * SECS_PER_HOUR)
		{
			event = HOUR_SUNDOWN;
		}
		else
		{
			event = HOUR_SUNRISE;
		}
	}
	else if (event == HOUR_SUNRISE)
	{
		send_to_outdoors("The sun rises in the east.\r\n");
		event = HOUR_SUNLIGHT;
	}
	else if (event == HOUR_SUNLIGHT)
	{
		send_to_outdoors("The day of %s has begun.\r\n"
				"The %u%s day of of %s in the Year %u of Our Lord\r\n",
				str_mday[mdays(mtime0())-1],
				mdays(mtime0()),
				ordinalize(mdays(mtime0())),
				str_mmonth[mmonths(mtime0())-1],
				myears(mtime0()));
		event = HOUR_SUNSET;
	}
	else if (event == HOUR_SUNSET)
	{
		send_to_outdoors("The sun slowly disappers in the west.\r\n");
		event = HOUR_SUNDOWN;
	}
	else if (event == HOUR_SUNDOWN)
	{
		send_to_outdoors("The night has begun.\r\n");
		event = HOUR_SUNRISE;
	}
	else
	{
		exitlog("/mud_time_update");
	}
	time_t secs=0;
	time_t msec_of_event = event * SECS_PER_HOUR;

	secs = event * SECS_PER_HOUR;

	if (msec_of_event < msec_of_the_day)
	{
		secs = SECS_PER_DAY - msec_of_the_day + msec_of_event;
	}
	else
	{
		secs = msec_of_event - msec_of_the_day;
	}
	secs = secs/MSECS_PER_SEC;
	queue_event(secs, 0, mud_time_update, NULL);
}

void init_tinyworld_time_module(void)
{
	mud_time_update(NULL);
}
