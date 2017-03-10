/* **************************************************************************
    Nilgiri the Forgotten World
    (C) 2004
   ************************************************************************** */
/** \file
    \brief Server socket setup and communication header
 */

#ifndef _x_TCPHOST_INCLUDE_x_
#define _x_TCPHOST_INCLUDE_x_

/** This should be about the average input length where the length makes sense
    of how to cut up blocks of input from the socket without over allocating
    space.  Allocating this to 1024 does not make sense if the average input
    length of your average user only inputs 8 characters.  <B>CHUNK_SIZE</B>
    should be much much less than <B>MAX_INP_LEN</B>.
    \sa MAX_INP_LEN
    \todo Set a proper CHUNK_SIZE.
 */
#define CHUNK_SIZE 16

extern void init_tcphost_module(const char *, const unsigned short int);
extern int new_connection(unsigned long int *);
extern void destroy_connection(int);
extern int get_num_descs(void);
extern const char *get_hostname(unsigned long int);

extern int tcpread(int, char *, size_t *, size_t);
extern int tcpwrite(int, char *);

extern int tcphostfd;

extern const char echo_on[];
extern const char echo_off[];
#endif
