/* **************************************************************************
    Nilgiri the Forgotten World
    (C) 2004
   ************************************************************************** */
/** \file
    \brief e-mail confirmation source
 */
#include<string.h>
#include<errno.h>
#include"err.h"
#include"time0.h"
#include"email_conf.h"
#include"utils.h"
#include"player.h"

static FILE *email_conf_fd = NULL;

/** Rotates the email confirmation file so that an external process can pick
    it up.  The file should be moved and after a minute+ of moving it
	 the file should be ready to processed and a new email-confirmation-file
	 will be created in its place. .This should occur every minutes.
 */
void rotate_email_confirm_file(void *ctx=NULL)
{
/*
	if (fclose(email_conf_fd)==EOF)
	{
		errlog("/fclose/%s/%s", EMAIL_CONF_FILE, strerror(errno));
		email_conf_fd = NULL;
	}
*/

	if ((email_conf_fd = fopen(EMAIL_CONF_FILE, "a+"))==NULL)
	{
		exitlog("/fopen/%s/%s", EMAIL_CONF_FILE, strerror(errno));
	}
	//queue_event(60, 0, rotate_email_confirm_file, NULL);
}

/** Initialize the email conf module. */
void init_email_conf_module(void)
{
	rotate_email_confirm_file();
}

/** Finalize the email conf module. */
void finit_email_conf_module(void)
{
	if (fclose(email_conf_fd)==EOF)
	{
		errlog("/fclose/%s/%s", EMAIL_CONF_FILE, strerror(errno));
	}
}

/** Checks to see if the e-mail is invalid.
    \param email The e-mail address to check.
    \return 1 if it is invalid, 0 if it is valid.
 */
int is_invalid_email(const char *email)
{
	int s;
	size_t len, i;

	len = strlen(email);

	if (len > MAX_EMAIL_LEN)
	{
		return 1;
	}

	s = 0;
	i = 0;
	while (i < len)
	{
		if (s == 0 && email[i] == '@')
	{
	s = 1;
	}
	else if (s == 0
	&& (!((email[i] >= 'a' && email[i] <= 'z')
	|| (email[i] >= 'A' && email[i] <= 'Z')
	|| (email[i] >= '0' && email[i] <= '9')
	|| email[i] == '_' || email[i] == '+'
	|| email[i] == '-' || email[i] == '.')))
	{
		return 1;
	}
	else if ((s == 1 || s == 3)
	&& (!((email[i] >= 'a' && email[i] <= 'z')
	|| (email[i] >= 'A' && email[i] <= 'Z')
	|| (email[i] >= '0' && email[i] <= '9')
	|| (email[i] == '-'))))
	{
		return 1;
	}
	else if (s == 1)
	{
		s = 2;
	}
	else if (s == 3)
	{
		s = 4;
	}
	else if ((s == 2 || s == 4) && email[i] == '.')
	{
		s = 3;
	}
	else if ((s == 2 || s == 4)
	&& (!((email[i] >= 'a' && email[i] <= 'z')
	|| (email[i] >= 'A' && email[i] <= 'Z')
	|| (email[i] >= '0' && email[i] <= '9')
	|| (email[i] == '-'))))
	{
		return 1;
	}
	i++;
	}
	if (s != 4)
	{
		return 1;
	}
	return 0;
}

/** Writes an entry into the email conf file.
    \param name The name of the player's character.
    \param email The email of the player.
    \return 0 on success, -1 on failure.
 */
static void write_email_confirm(const char *email,
                                         const char *name, const char *password)
{
	if (fprintf(email_conf_fd, "%s %s %s\n", email, name, password)==0)
	{
		errlog("/fprintf/%s:%s:%s", email, name, password);
		return;
	}
	fflush(email_conf_fd);
	return;
}

void write_email_confirm(Player *pc, const char *password)
{
	write_email_confirm(pc->getEmail(), pc->getName(), password);
}

void write_email_confirm(BASIC_PC *pc, const char *password)
{
	write_email_confirm(pc->email, pc->name, password);
}

/** Generates a random password consisting of a capital vowel followed by
    series of 6 lowercase consonants and vowels and then 1 number.  In total
    the password is 10 characters.
    \return The password.
 */
char *generate_password(void)
{
	const char consonant[21]   = { 'b', 'c', 'd', 'f', 'g', 'h', 'j',
											 'k', 'l', 'm', 'n', 'p', 'q', 'r',
											 's', 't', 'v', 'w', 'x', 'y', 'z' };
	const char vowel[5]        = { 'a', 'e', 'i', 'o', 'u' };

	static char buffer[8];

	buffer[0] = consonant[number(21)];
	buffer[1] = vowel[number(5)];
	buffer[2] = consonant[number(21)];
	buffer[3] = vowel[number(5)];
	buffer[4] = consonant[number(21)];
	buffer[5] = vowel[number(5)];
	buffer[6] = '0' + number(10);
	buffer[7] = '\0';
	to_upper(buffer);

	return buffer;
}
