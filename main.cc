/* **************************************************************************
    Nilgiri the Forgotten World
    (C) 2004
   ************************************************************************** */
/** \file 
    \brief start-up and game loop source
 */

/** \mainpage 
    This is the source code documentation for Nilgiri the Forgotten World.
    <P>
    <CENTER>
    <I><B>If you're not on this list you do not belong here!</B></I><BR>
    <TABLE CELLBORDER=0 CELLPADDING=10 CELLSPACING=0>
    <TR VALIGN=TOP><TD>
    <B>Implementors:</B><BR>
    <UL>
    <LI>Rivin
    <LI>Sin
    </UL>
    <TD>
    <B>World Builders:</B><BR>
    <UL>
    <LI>Mobius
    <LI>Natedog
    <LI>Mandessa
    </UL>
    <TD>
    <B>Administrators:</B><BR>
    <UL>
    <LI>Russ
	 <LI>Mark
    <LI>Motorola
    </UL>
    </TR>
    </TABLE>
    </CENTER>
    <P>
    Correspond through the Nilgiri development mailing list:
    <I>nilgiri-dev@russwong.com</I>
   
    
    <B>NOTE TO WORLD BUILDERS:</B><BR>
    <UL>
    <LI>
    Please refer to the flags in <TT>constants.h</TT>.  To read the file you
    probably want to skip the first half of the file and scroll down to the
    <B>Enumeration Type Documentation</B> section which begins with
    <B>APPLY_TYPES</B>.
    \sa APPLY_TYPES
    <LI>
    These flags need to be checked and rechecked for deprecation, or
    enhancements.  For example, I deprecated a few apply types that we never
    set such as age, height, weight, etc.  I enhanced the list by adding
    flags such as material types.
    <LI>
    Generally all the flags listed are in desperate need of descriptions.  Take
    a look at the <B>SAVING_THROW_TYPES</B> as an example of properly
    described flags.
    \sa SAVING_THROW_TYPES
    <LI>
    The game tables that indicate assignment and limits should be located in
    <TT>constants.h</TT>.  However it is not built out so please be patient.
    </UL>
    <P>

    <B>NOTE TO IMPLEMENTORS (Coding Guidelines):</B><BR>
    <UL>
    <LI>A good place to start out with is the Container class.  All things
    Nilgiri evolve from here.
    <LI> Use tabs spaced to two spaces to indent
    <LI> Always bracket <TT>if</TT>, <TT>for</TT> and <TT>while</TT> statements
    <LI> Use doxygen to comment your code.
    <LI> Use <TT>\\todo</TT> for keeping track of code that needs to be changed
    <LI> All structs should begin with an undercore and words seperated by
         an underscore: <TT>struct _my_struct { ... };</TT>
    <LI> All structs should be typedef'd and the typedef should be the same
         name of the all capitalized name of the struct without the leading
         underscore: <TT>typedef _my_struct { ... } MY_STRUCT;</TT>
    <LI> Classes should capitalize the first letter in each word and have no
         underscore: <TT>class MyClass { ... }; </TT>
    <LI> Class methods and members should capitalize the first letter in each
         word except the first letter and have no underscore:
         <TT>int MyClass::myClassMethod(); int MyClass::myClassMember;</TT>
    <LI> Functions and variables should be lowercase with words seperated by
         an underscore: <TT> int my_function(); int my_int;</TT>
    <LI> Any constants that the world builders may touch upon should be located
         in <TT>constants.h</TT>.
    <LI> Any tables that the world builders may touch upon should be located
         in <TT>constants.cc</TT>.
    <LI> Structs with the name containg SPECIAL (i.e. CHAR_SPECIAL_DATA)
         indicate data that is transitional within the course of the game
         like hitpoints.
	 <LI> When creating player functions try to put the usage statement
	      the bottom of the function rather than somewhere lost in the middle.
	 <LI> If creating a new enumeration and data type remember to use the
	      TYPE_CHECK() and FLAG_CECK() macros within the init module function.
	 <LI> Use const often
	 <LI> On unexpected errors it is better to abort to try and get a core for
	      analysis later than to try and fix the error in progress
	 <LI> Wrap aborts and error messages in #define statements or low level
	      functions.
    </UL>
    <P>

	 <B>Tasks completed:</B><BR>
	 <UL>
	 <LI> basic character loading/unloading
	 <LI> player creation
	 <LI> basic room loading/unloading
	 <LI> character creation and pregame loading
	 <LI> player login
	 <LI> game interpreter
	 <LI> basic combat
	 <LI> basic social loading/unloading
	 <LI> basic help/wizhelp loading/unloading
	 <LI> basic object loading/unloading
	 <LI> note item
	 <LI> online help and wizhelp creation
	 <LI> online social creation
	 <LI> basic grant and revoke immortal commands
	 <LI> online cdata, idata creation
	 <LI> online cdescr, idescr creation
	 <LI> onilne cstats, istats creation
	 <LI> online room creation
	 <LI> online zone creation
	 <LI> bug reporting and workflow
	 </UL>
	 <P>

 */
#include<stdlib.h>
#include<unistd.h>
#include<sys/select.h>
#include<errno.h>
#include<string.h>
#include<pthread.h>
#include<signal.h>
#include<sys/socket.h>
#include"main.h"
#include"err.h"
#include"time0.h"
#include"tcphost.h"
#include"descriptor.h"
#include"constants.h"
#include"library.h"
#include"tinyworld_character.h"
#include "tinyworld_zone.h"
#include"tinyworld_room.h"
#include"tinyworld_item.h"
#include"interpreter.h"
#include"combat.h"
#include"db.h"
#include"command.h"
#include"tinyworld_time.h"
#include"addlog.h"
#include"bug.h"

static int main_shutdown = 1;

int shutdown_time = -1;

#define SHUTTING_DOWN_MESG \
"\r\n" K_BLD_YEL \
"*** Nilgiri the Forgotten World is rebooting now. Come back again soon! ***" \
"\r\n\r\n" K_WHT

/// main thread attribute, the attributes all other threads want to use
pthread_attr_t thr_attr;

void shutdown_now(void)
{
	for (Descriptor *d = descriptor_list; d; d = d->getNext())
	{
		if (d->envFlag(DENV_SPLITSCREEN))
		{
			d->finitSplitScreen();
		}
		d->put(SHUTTING_DOWN_MESG);
	}
	main_shutdown = 16;
}

int shutting_down(void)
{
	return main_shutdown;
}

void sig_int(int i)
{
	fprintf(stderr, "Received signal interrupt.. shutting down\n");
	addlog_sig_int();
	if (main_shutdown)
	{
		exit(0);
	}
	shutdown_now();
}

/** Initialize the main module. */
void init_main_module(void)
{
	pthread_attr_init(&thr_attr);
	pthread_attr_setscope(&thr_attr, PTHREAD_SCOPE_SYSTEM);
	pthread_attr_setdetachstate(&thr_attr, PTHREAD_CREATE_DETACHED);
	pthread_attr_setinheritsched(&thr_attr, PTHREAD_EXPLICIT_SCHED);
	pthread_attr_setschedpolicy(&thr_attr, SCHED_OTHER);
	signal(SIGINT, sig_int);
	if (sizeof(ubyte8) != 8)
	{
		exitlog("/sizeof/ubyte8/%u", sizeof(ubyte8));
	}
	assert(sizeof(int)==4);
	assert(sizeof(int)==sizeof(long int));
}

void queue_shutdown(void *ctx)
{
	time_t t = 0;
  
	if (shutdown_time == -1)
	{
		return;
	}

	time_t diff = (time_t) shutdown_time - time0;

	if (diff > (60*60*24)) //1 day
	{
		t = 60*60*24;
	}
	else if (diff >= (3600)) //3600 min
	{
		t = 3600;
	}
	else if (diff >= 900) //15 min
	{
		t = 300;
	}
	else if (diff >= 300) //5 min
	{
		t = 180;
	}
	else if (diff >= 120) //3 minutes 
	{
		t = 60;
	}
	else if (diff >= 60) //60 seconds
	{
		t = 30;
	}
	else if (diff >= 15) //15 seconds
	{
		t = 5;
	}
	else if (diff >=  6) //10 seconds
	{
		t = 2;
	}
	else if (diff > 0)
	{
		t = 1;
	}

	if (diff > 0)
	{
		send_to_descriptors(K_BLD_YEL
				"\r\n*** The game is rebooting in %s ***\r\n" K_WHT,
				time_ago((time_t) diff));
		queue_event(t, 0, queue_shutdown, NULL);
	}
	else
	{
		shutdown_now();
	}
}

/** Let's get this party started! */
int main(int argc, char **argv)
{
	fd_set inp_set, out_set;
	int maxfd;
	Descriptor *d, *d_next;
	struct timeval poll_time;
	clock_t last_clks, clks;

	poll_time.tv_sec = 0;
	poll_time.tv_usec = 100000;

	fprintf(stderr, "Nilgiri starting up.\n");

	init_time0_module(); // this must always go first
	init_err_module(); // this must always go second
	init_utils_module();
	init_descriptor_module();
	init_main_module();
	init_constants_module();
	init_db_module(); // this should go before the library
	init_library_module(); // this should go towards the end
	init_container_module(); // this should go after library
	init_tinyworld_zone_module(); // this should go right before room
	init_tinyworld_room_module(); // this should go after library
	init_tinyworld_character_module(); // this should go after library
	init_player_module(); // this should go immediately after tinyworld char
	init_bug_module(); // this should go after the player module
	init_tinyworld_item_module(); // this should go after the library
	init_email_conf_module(); // this should be almost last
	init_command_module(); // this should be before interpreter
	init_interpreter_module(); // this should not be last
	init_combat_module(); // this should be almost last
	init_tinyworld_time_module(); // this should be almost last
	init_tcphost_module(HOSTNAME, PORT); // this must always last

	main_shutdown = 0;
	while (1)
	{
		FD_ZERO(&inp_set);
		FD_ZERO(&out_set);
		FD_SET(tcphostfd, &inp_set);
		maxfd = tcphostfd;

		for (d = descriptor_list; d; d = d->getNext())
		{
			FD_SET(d->getDesc(), &inp_set);
			FD_SET(d->getDesc(), &out_set);
			if (maxfd < d->getDesc())
			{
				maxfd = d->getDesc();
			}
		}

		if (select(maxfd+1, &inp_set, &out_set, NULL, &poll_time) == -1)
		{
			if (errno != EINTR)
			{
				exitlog("/select/%s", strerror(errno));
			}
		}

		if (shutting_down()==0)
		{
			if (FD_ISSET(tcphostfd, &inp_set))
			{
				if ((d = new Descriptor)==NULL)
				{
					errlog("/new/%s", strerror(errno));
				}
				else
				{
					d->handleStateConnect();
				}
			}
		}

		for (d = descriptor_list; d; d = d_next)
		{
			d_next = d->getNext();

			//MARK is this the absolutely right way to do this?
			if (FD_ISSET(d->getDesc(), &out_set) && d->processOut())
			{
				continue;
			}

			if (d->extracting())
			{
				continue;
			}

			if (FD_ISSET(d->getDesc(), &inp_set) && d->processInp())
			{
				continue;
			}
		}

		deliver_events();
		sched_yield();

		if (main_shutdown && --main_shutdown==0)
		{
			break;
		}
	}

	sleep(2);
	while (descriptor_list)
	{
		addlog_shut_player(descriptor_list->getPlayer());
		delete descriptor_list;
	}
	shutdown(tcphostfd, SHUT_RDWR);
	finit_email_conf_module();
	finit_bug_module();
	finit_db_module(); //this goes after all db calls are made
	finit_utils_module();
	finit_player_module();
	finit_tinyworld_item_module();
	finit_tinyworld_room_module();
	finit_tinyworld_zone_module();
	fprintf(stderr, "Nilgiri has shut down.\n");
	addlog_shutdown();
	finit_err_module(); //this should go dead last
}
