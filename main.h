/* **************************************************************************
    Nilgiri the Forgotten World
    (C) 2004
   ************************************************************************** */
/** \file
    \brief start-up and game loop header
 */

#ifndef _x_MAIN_INCLUDE_x_
#define _x_MAIN_INCLUDE_x_

/// where are we running the game
#define HOSTNAME "127.0.0.1"
/// what port are we running the game on
#define PORT 3141

extern pthread_attr_t thr_attr;

extern int shutting_down(void);
extern void shutdown_now(int);

extern int shutdown_time;
#endif
