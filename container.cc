/* **************************************************************************
    Nilgiri the Forgotten World
    (C) 2004
   ************************************************************************** */
/** \file
    \brief Container object source
 */
#include<stdlib.h>
#include<assert.h>
#include<errno.h>
#include"container.h"
#include"err.h"
#include"addlog.h"

/** This function reads the last used CID from the CID_FILE. If the CID_FILE
    does not exist or we cannot read from it then we abort.
	 \return The last used CID
 */
static cid_t readLastCID(void)
{
	FILE *fd;
	cid_t cid = 1;

	if ((fd = fopen(CID_FILE, "r"))==NULL)
	{
		if (errno != ENOENT)
		{
			exitlog("/fopen/%s/%s", CID_FILE, strerror(errno));
		}
	}

	if (fd == NULL)
	{
		cid = CID_START + 1;
	}
	else
	{
		if (1 != fread(&cid, sizeof(cid_t), 1, fd))
		{
			exitlog("/fread/%s", CID_FILE);
		}
		if (fclose(fd)==EOF)
		{
			exitlog("/fclose/%s/%s", CID_FILE, strerror(errno));
		}
	}
	game_stat.last_cid = cid;
	return cid;
}

/** Initialize the class members. */
Container::Container(container_type_t container_type, cid_t container_id)
{
	container_was_in = NULL;
	container_in = NULL;

	head_content = NULL;
	tail_content = NULL;

	next_content = NULL;
	prev_content = NULL;

	carry_weight = 0;
	carry_volume = 0;
	carry_contents = 0;

	weight = 0;
	volume = 0;

	type = container_type;

	if (container_id)
	{
		cid = container_id;
		return;
	}
	else
	{
		cid = readLastCID();

		FILE *fd;

		if ((fd = fopen(CID_FILE, "w+"))==NULL)
		{
			exitlog("/fclose/%s/%s", CID_FILE, strerror(errno));
		}
		cid++;
		if (1 != fwrite(&cid, sizeof(cid_t), 1, fd))
		{
			exitlog("/fwrite/%s/%s", CID_FILE, strerror(errno));
		}
		cid--;
		if (fclose(fd)==EOF)
		{
			exitlog("/fclose/%s/%s", CID_FILE, strerror(errno));
		}
	}
}

/** Empty the container and extract it from the tinyworld. */
Container::~Container(void)
{
	if (this && this->container_in)
	{
		empty();
		extract();
	}
}

/** Drops all the items inside of a container into the container of the
    container.
 */
void Container::empty(void)
{
	while (head_content != NULL) container_in->hold(head_content);
}

/** This removes an item (along with its contents) from the tinyworld.  Most
    other methods may need to invoke this method before beginning their
    operation.
 */
void Container::extract(void)
{
	container_was_in = container_in;
	if (container_in == NULL)
	{
		assert(prev_content == NULL);
		assert(next_content == NULL);
		return;
	}

	if (prev_content)
	{
		// make our previous contents next content the next content
		prev_content->next_content = next_content;
	}
	else
	{
		// we are at the head of the list, we might be in something, so make our
		// next content the head of the container we're in
		assert(container_in->head_content == this);
		container_in->head_content = next_content;
	}

	if (next_content)
	{
		// make the next contents previous content the previous content
		next_content->prev_content = prev_content;
	}
	else
	{
		// we are at the tail of the list, we might be in something, so make our
		// prev content the tail of the container we're in
	 	assert(container_in->tail_content == this);
		container_in->tail_content = prev_content;
	}

	for (Container *cin = this; cin->container_in; cin = cin->container_in)
	{
		cin->container_in->carry_weight -= carry_weight + weight;
		cin->container_in->carry_volume -= carry_volume + volume;
		cin->container_in->carry_contents--;
	}
	container_in = NULL;
	next_content = NULL;
	prev_content = NULL;
}

/** Adds a container to the contents.
 \param c The container to hold.
*/
void Container::hold(Container *c)
{
	c->extract();

	c->container_in = this;

	if (tail_content)
	{
		assert(tail_content->next_content==NULL);
		tail_content->next_content = c;
		c->prev_content = tail_content;
		tail_content = c;
	}
	else
	{
		assert(head_content == NULL);
		head_content = c;
		tail_content = c;
	}

	for (Container *cin = this; cin; cin = cin->container_in)
	{
		cin->carry_weight += c->carry_weight + c->weight;
		cin->carry_volume += c->carry_volume + c->volume;
		cin->carry_contents++;
	}
}

/** Removes a container from the contents.
 \param c The container to drop.
*/
void Container::drop(Container *c)
{
	assert(c && c->container_in == this && container_in);

	c->extract();

	c->container_in = container_in;

	if (container_in->tail_content)
	{
		assert(container_in->tail_content->next_content == NULL);
		container_in->tail_content->next_content = c;
		c->prev_content = container_in->tail_content;
		container_in->tail_content = c;
	}
	else
	{
		assert(container_in->head_content == NULL);
		container_in->head_content = c;
		container_in->tail_content = c;
	}
}

/** Checks to see if it is possible that a content can fit into the Container.
    \param c The container to check.
    \return 0 - false, 1 - true
 */
int Container::canHold(Container *c)
{
	assert(c);
	return (maxCarryWeight() >= carry_weight + c->carry_weight + c->weight)
		&& (maxCarryVolume() >= carry_volume + c->carry_volume + c->volume);
}

/** Echo except to one container
    \param subj The container not to echo to also the subject
 */
void Container::echo(Container *subj, const char *format, ...)
{
	int n, size = CHUNK_SIZE_OUT;
	int is_complete;
	char *msg_s;
	char msg[CHUNK_SIZE_OUT];
	va_list vargs;

	msg_s = NULL;
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

	for (Container *c = head_content ; c; c = c->next_content)
	{
		if (c == subj)
		{
			continue;
		}
		subj->echoTo(c, msg_s ? msg_s : msg);
	}
	if (msg_s)
	{
		free(msg_s);
	}
}

/** Echo to everyone in the container except two containers
    \param ch The subject
	 \param vict The victim
 */
void Container::echo(Container *subj, Container *vict, const char *format, ...)
{
	int n, size = CHUNK_SIZE_OUT;
	int is_complete = 0;
	char *msg_s;
	char msg[CHUNK_SIZE_OUT];
	va_list vargs;

	msg_s = NULL;
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

	for (Container *c = head_content; c; c = c->next_content)
	{
		if (c == subj|| c == vict)
		{
			continue;
		}
		subj->echoTo(c, vict, msg_s ? msg_s : msg);
	}
	if (msg_s)
	{
		free(msg_s);
	}
}

/** Echo to everything in the container
  */
void Container::echo(const char *format, ...)
{
	int n, size = CHUNK_SIZE_OUT;
	int is_complete = 0;
	char *msg_s;
	char msg[CHUNK_SIZE_OUT];
	va_list vargs;

	msg_s = NULL;
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

	for (Container *c = head_content; c; c = c->next_content)
	{
		c->echoTo(msg_s ? msg_s : msg);
	}
	if (msg_s)
	{
		free(msg_s);
	}
}

Item *Container::findItem(const char *name, Container *searcher)
{
	Item *item;
	for (Container *c = head_content; c; c = c->next_content)
	{
		if ((item=c->objItem())==NULL || (searcher && c->isInvisTo(searcher)))
		{
			continue;
		}
		if (c->isCalled(name))
		{
			return item;
		}
	}
	return NULL;
}

void Container::listItemsTo(Container *vict)
{
	Container *c;
	if (vict == NULL)
	{
		int cnt = 0;
		put("You are carrying:\r\n");
		for (c = head_content; c; c = c->next_content)
		{
			cnt++;
			echo(c, "$G$N");
		}
		if (cnt == 0)
		{
			put("Nothing.\r\n");
		}
	}
	else if (vict->container_in == this)
	{

		for (c = head_content; c; c = c->next_content)
		{
			if (c->type != CONTAINER_ITEM
				  	|| (vict && c->isInvisTo(vict)) || c == vict)
			{
				continue;
			}
			vict->echo(c, "$G%s", c->getDescrRoom());
		}
	}
}

void Container::printStats(Container *vict)
{
	vict->print("[wt %u][vol %u] max : [wt %u/%d][vol %u/%d][cont %u]\r\n",
			weight,
			volume,
			carry_weight,
			maxCarryWeight()==(weight_t)-1 ? -1 : maxCarryWeight(),
			carry_volume,
			maxCarryVolume()==(volume_t)-1 ? -1 : maxCarryVolume(),
			carry_contents);
}

void Container::listCharactersTo(Container *vict)
{
	Container *c;
	if (vict->container_in == this)
	{

		for (c = head_content; c; c = c->next_content)
		{
			if (c->type != CONTAINER_CHARACTER
				  	|| (vict && c->isInvisTo(vict)) || c == vict)
			{
				continue;
			}
			vict->echo(c, "$G%s", c->getDescrRoom());
		}
	}
	else
	{
		for (c = head_content; c; c = c->next_content)
		{
			if (c->type != CONTAINER_CHARACTER
				  	|| (vict && c->isInvisTo(vict)) || c == vict)
			{
				continue;
			}
			vict->echo(c, "$G%s", c->getName());
		}
	}
}

Container *Container::findContent(const char *name, Container *searcher)
{
	for (Container *c = head_content; c; c = c->next_content)
	{
		if (searcher &&  c->isInvisTo(searcher))
		{
			continue;
		}
		if (c->isCalled(name))
		{
			return c;
		}
	}
	return NULL;
}

Character *Container::findCharacter(const char *name, Container *searcher)
{
	Character *ch;

	for (Container *c = head_content; c; c = c->next_content)
	{
		if ((ch=c->objCharacter())==NULL || (searcher && c->isInvisTo(searcher)))
		{
			continue;
		}
		if (c->isCalled(name))
		{
			return  ch;
		}
	}
	return NULL;
}


void init_container_module(void)
{
	addlog_next_cid(readLastCID());
}

/** This macro expands the correct pronouns for a given subject.  It is
    strongly tied to the va_format() function.
	 \sa comm
 */
#define PRONOUN_HELPER(ch, pron1, pron2, pron3) \
{ 																\
	if ((ch) == NULL)										\
	{															\
		errlog("/no container/%s", format);			\
		*q = '.';											\
		q++;													\
		*q = '.';											\
		q++;													\
		*q = '.';											\
		q++;													\
	}															\
	else														\
	{															\
		if ((ch)->isMale())								\
		{														\
			for (c = pron1; *c; c++)					\
			{													\
				*q = *c;										\
				q++;											\
			}													\
		}														\
		else if ((ch)->isFemale())						\
		{														\
			for (c = pron2; *c; c++)					\
			{													\
				*q = *c;										\
				q++;											\
			}													\
		}														\
		else													\
		{														\
			for (c = pron3; *c; c++)					\
			{													\
				*q = *c;										\
				q++;											\
			}													\
		} 														\
	} 															\
}

/**
  This function expands the dollar-sign ($) macros. This function assumes that
  the first container can see a second unseen container as something
  or someone. This function is not reentrant safe.  Any subsequent calls to
  va_reformat() will free any previously allocated memory using this function.
  - lower-case macros are the first container
  - upper-case macros are the last container
  - macros are defined as:
    - $n,$N - container name
    - $e,$E - he,she,it
    - $m,$M - him,her,it
    - $s,$S - his,hers,its

  \param *format This is the original format string.  If format is NULL then
  any previously allocated memory made by this function is freed.
  interaction at all
  \param ch The first container, usually the subject
  \param vict The second container, usually the object
  \sa PRONOUN_HELPER
 */
char *comm(const char *format, Container *ch, Container *vict)
{
	static char fmt[MAX_FORMAT_SIZE+1];
	static char *fmt_s = NULL;
	size_t size, count;
	const char *p, *c;
	char *q;
	int s;

	if (fmt_s)
	{
		FREE(fmt_s);
	}

	if (format==NULL)
	{
		return NULL;
	}

	p = format;
	size = 0;
	count = 0;

	while (*p)
	{
		if (*p == '$')
		{
			count++;
		}
		size++;
		p++;
	}
	int resize = size + (count * MAX_SHORT_DESCR_SIZE)+1;
	if (resize > MAX_FORMAT_SIZE)
	{
		MALLOC(fmt_s, char, resize);
	}

	p = format;
	q = fmt_s ? fmt_s : fmt;
	s = 0;
	while (*p)
	{
		if (s == 0)
		{
			if (*p == '$')
			{
				s = 1;
			}
			else
			{
				*q = *p;
				q++;
			}
		}
		else if (s == 1)
		{
			if (*p == 'n')
			{
				if (ch == NULL)
				{
					errlog("/no container/%s", format);
					*q = '.';
					q++;
					*q = '.';
					q++;
					*q = '.';
					q++;
				}
				else
				{
					for (c = ch->getName(); *c; c++) //strcpy
					{
						*q = *c;
						q++;
					}
				}
			}
			else if (*p == 'e')
			{
				PRONOUN_HELPER(ch, "he", "she", "it");
			}
			else if (*p == 'm')
			{
				PRONOUN_HELPER(ch, "him", "her", "it");
			}
			else if (*p == 's')
			{
				PRONOUN_HELPER(ch, "his", "hers", "its");
			}
			else if (*p == 'g')
			{
				for (c = c_sex(ch); *c; c++)
				{
					*q = *c;
					q++;
				}
			}
			else if (*p == 'N')
			{
				if (vict == NULL)
				{
					errlog("/no container/%s", format);
					*q = '.';
					q++;
					*q = '.';
					q++;
					*q = '.';
					q++;
				}
				else
				{
					for (c = vict->getName(); *c; c++) //strcpy
					{
						*q = *c;
						q++;
					}
				}
			}
			else if (*p == 'E')
			{
				PRONOUN_HELPER(vict, "he", "she", "it");
			}
			else if (*p == 'M')
			{
				PRONOUN_HELPER(vict, "him", "her", "it");
			}
			else if (*p == 'S')
			{
				PRONOUN_HELPER(vict, "his", "hers", "its");
			}
			else if (*p == 'G')
			{
				for (c = c_sex(vict); *c; c++)
				{
					*q = *c;
					q++;
				}
			}
			else
			{
				errlog("/bad macro/%s", format);
				*q = '.';
				q++;
				*q = '.';
				q++;
				*q = '.';
				q++;
			}
			s = 0;
		}
		else
		{
			exitlog("/bad state/%d", s);
		}
		p++;
	}
	*q = '\0';
	if  (fmt_s)
	{
		return fmt_s;
	}
	return fmt;
}

