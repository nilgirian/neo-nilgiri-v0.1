/* **************************************************************************
    Nilgiri the Forgotten World
    (C) 2004
   ************************************************************************** */
/** \file
    \brief e-mail confirmation header
 */
#ifndef _x_EMAIL_CONF_INCLUDE_x_
#define _x_EMAIL_CONF_INCLUDE_x_

/** The maximum size a player's character name can be. */
#define MAX_NAME_LEN 11

/** The maximum size for a player email. */
#define MAX_EMAIL_LEN 64

#define EMAIL_CONF_FILE "email_conf_data"

extern int is_invalid_email(const char *);
extern char *generate_password(void);
/* extern void write_email_confirm(Player *, const char *); */
/* extern void write_email_confirm(BASIC_PC *, const char *); */
extern void init_email_conf_module(void);
extern void finit_email_conf_module(void);

#endif
