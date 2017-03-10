/* **************************************************************************
    Nilgiri the Forgotten World
    (C) 2004
    err.h: Logging
   ************************************************************************** */
/** \file
    \brief bug reporting and tracking header
 */
#ifndef _x_BUG_INCLUDE_x_
#define _x_BUG_INCLUDE_x_

#include"utils.h"

class BugPermit
{
public:
	BugPermit(const char *, const char *);
	~BugPermit(void);
	boolean_t isOwner(const char *);
	const char *getOwner(void);

	static const char *allowed(const char *, const char *);
	static void add(const char *, const char *);
	static void del(const char *, const char *);
	static void indexTo(Character *);
	static void save(const char *, const char *);
	static void delDB(const char *, const char *);
	static void load(void);
	static inline void debug(void);
private:
	const char *owner;
	Words *submitters;
	boolean_t canSubmit(const char *);

	static BugPermit **list;
	static top_t list_top;
};

inline void BugPermit::debug(void)
{
	fprintf(stderr, "BugPermit\r\n");
	for (top_t i = 0; i <= list_top; i++)
	{
		fprintf(stderr, "Owner: %s\n", list[i]->owner);
		list[i]->submitters->debug();
	}
}

inline const char *BugPermit::getOwner(void)
{
	return owner;
}

inline boolean_t BugPermit::isOwner(const char *name)
{
	return is_name(name, owner);
}

inline boolean_t BugPermit::canSubmit(const char *name)
{
	return submitters->lookup(name) != NULL;
}

extern void init_bug_module(void);
extern void finit_bug_module(void);
extern void list_bugs(Character *, const char *, const char *, const char *,
		const char *, const char *);
boolean_t can_bug(const char *, Character *);
extern void bug_writer(Character *, const char *, priority_t, const char *);
extern int bug_exists(int);
extern void followup_writer(Character *, int);
extern void bug_detail_to(Character *, int);
extern void list_bugs_owner(Character *, const char *);
extern void count_bugs(Character *);
extern status_t get_bug_status(int);
extern int is_bug_owner(int, const char *);
extern int is_bug_submitter(int, const char *);
extern int can_reopen_bug(int, const char *);
extern void new_status_writer(Character *, int, status_t);
#endif
