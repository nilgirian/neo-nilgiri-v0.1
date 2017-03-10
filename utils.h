/*ll **************************************************************************
    Nilgiri the Forgotten World
    (C) 2004
   ************************************************************************** */
/** \file
    \brief Basic global utilities header
 */

#ifndef _x_UTILS_INCLUDE_x_
#define _x_UTILS_INCLUDE_x_

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>
#include<errno.h>
#include"db.h"
#include"err.h"
#include"library.h"
#include"time0.h"

class Container;
class Character;


#define SECS_PER_MHOUR (POINT_UPDATE_TIME)
#define SECS_PER_MIN (60)
#define SECS_PER_HOUR (SECS_PER_MIN * 60)
#define SECS_PER_DAY (SECS_PER_HOUR * 24)
#define SECS_PER_YEAR 31556926
#define SECS_PER_MONTH 2629744
#define MAX_SHORT_DESCR_SIZE 32

#define MAX_FORMAT_SIZE 512

#define MAX_STAT  25

#define MAXHOSTNAMELEN 64
#define MAXIPLEN 15

#define MAX_BUG_TITLE_LEN 72

#define CHARBITS 8
#define MAXINT 0x7FFFFFFF

/** Defines the length of all escape color sequence used in Nilgiri. */
#define COLORLEN 7

/** The number of characters in a line. */
#define LINE_LENGTH 80

#define LONG_LINE_LENGTH 132

/** Make a type into a bitvector flag */
#define f_(type) ( 1ULL << (type))

#define TYPE_CHECK(max, type_t) \
	if (max >= (1<<sizeof(type_t)*CHARBITS)-1) \
	{ \
		exitlog("/max " "type_t"); \
	}

#define FLAG_CHECK(max, flag_t) \
	if (max >= sizeof(flag_t)*CHARBITS) \
	{ \
		exitlog("/max " "flag_t"); \
	}

#define FREE(ptr) { free(ptr); ptr = NULL; }

#define MALLOC(ptr, type, items) \
{\
	if (((ptr) = (type *) malloc(sizeof(type) * (items)))==NULL)\
	{\
		exitlog("/malloc/%s", strerror(errno));\
	}\
}

#define CALLOC(ptr, type, items) \
{\
	if (((ptr) = (type *) calloc(items, sizeof(type)))==NULL)\
	{\
		exitlog("/calloc/%s", strerror(errno));\
	}\
}

#define REALLOC(ptr, type, items) \
{\
	if (((ptr) = (type *) realloc((ptr), sizeof(type) * (items)))==NULL)\
	{\
		exitlog("/realloc/%s", strerror(errno));\
	}\
}

#define IS_SET(bit, flags) ((flags) & (bit))
#define SET(bit, flags) ((flags) |= (bit))
#define RESET(bit, flags) ((flags) &= ~(bit))

/**   0xFFFFFFFFFFFFFFFF,
    18446744073709551615 */
typedef unsigned long long int ubyte8;

/** 0xFFFFFFFF,
    4294967295 */
typedef unsigned long int ubyte4;

/** 0xFFFF,
     65535 */
typedef unsigned short int ubyte2;

/** 0xFF,
     255 */
typedef unsigned char ubyte1;

/**   0x8000000000000000 -  0x7FFFFFFFFFFFFFFF,
    -9223372036854775808 - 9223372036854775807 */
typedef long long int sbyte8;

/**  0x80000000 - 0x7FFFFFFF,
    -2147483648-  2147483647 */
typedef long int sbyte4;

/** 0x8000 - 0x7FFF,
    -32768 -  32767 */
typedef short int sbyte2;

/** 0x80 - 0x7F,
    -128 -  127 */
typedef char sbyte1;

/** \brief Sorted word class. */
/** This class is intended to keep a sorted list of keywords that can be
    easily searched upon to compare for equality.  This creates memory for
    the words in the dictionary.  Once words are added to the dictionary
    they cannot be removed.
 */

/** The virtual id type.  This cannot be 0. */
typedef ubyte4 vid_t;

/** Top of arrays */
typedef sbyte4 top_t;

typedef sbyte1 thac0_t;

/** \brief Die-Multiplier-Modifier structure */
/** This defines your basic dice roll by what sided die being used and how
    many dice are being rolled.  A modifier is added to the total.
 */
typedef struct _die_multiplier_modifier
{
	ubyte2 die;
	ubyte2 multiplier; //MARK maybe this should be ubyte1
	sbyte2 modifier;
}
DMM;

/** We use a x10 multiplier so that we do not have to use float */
inline int averagex10(DMM *dmm)
{
	return dmm->modifier + ((dmm->multiplier*(dmm->die+1))<<1);
}

/** The coordinate type.  This cannot be 0. */
typedef ubyte1 coord_t;

/** The coordinate system for mapping. Coordinates start at (1,1).  */
typedef struct _coord
{
	coord_t x;
	coord_t y;
	vid_t  v;
}
COORD;

/** Checks to see if the character is printable.
    \return 0 - false, 1 - true
 */
inline int is_print(char c)
{
	return c >=' ' && c <='~';
}

/** Checks if a character is not alphabetical.
    \param c The character to check
    \return 1 if it is not alphabetical, 0 if it is alphabetical
 */
inline int is_not_alpha(char c)
{
	return c < 'A' || c > 'z' || (c > 'Z' && c < 'a');
}

/** Converts a single digit number to ASCII character.
    \param i A single digit number
	 \return an ASCII representation of the digit
	 \warn unspecified behavior will occur should i be more than one digit
 */
inline char itoc(char i)
{
	return i + '0';
}

inline char *itoa(int i)
{
	static char a[10];
	char c;
	int found_gsd = 0;

	a[10] = '\0';

	c = itoc(i / 1000000000);
	if (c == '0' && found_gsd == 0)
	{
		a[9] = '\0';
	}
	else
	{
		a[9] = c;
		found_gsd = 9;
	}

	c = itoc((i % 1000000000)/100000000);
	if (c == '0' && found_gsd == 0)
	{
		a[8] = '\0';
	}
	else
	{
		a[8] = c;
		if (found_gsd == 0)
		{
			found_gsd = 8;
		}
	}

	c = itoc((i % 100000000)/10000000);
	if (c == '0' && found_gsd == 0)
	{
		a[7] = '\0';
	}
	else
	{
		a[7] = c;
		if (found_gsd == 0)
		{
			found_gsd = 7;
		}
	}

	c = itoc((i % 10000000)/1000000);
	if (c == '0' && found_gsd == 0)
	{
		a[6] = '\0';
	}
	else
	{
		a[6] = c;
		if (found_gsd == 0)
		{
			found_gsd = 6;
		}
	}

	c = itoc((i % 1000000)/100000);
	if (c == '0' && found_gsd == 0)
	{
		a[5] = '\0';
	}
	else
	{
		a[5] = c;
		if (found_gsd == 0)
		{
			found_gsd = 5;
		}
	}

	c = itoc((i % 100000)/10000);
	if (c == '0' && found_gsd == 0)
	{
		a[4] = '\0';
	}
	else
	{
		a[4] = c;
		if (found_gsd == 0)
		{
			found_gsd = 4;
		}
	}

	c = itoc((i % 10000)/1000);
	if (c == '0' && found_gsd == 0)
	{
		a[3] = '\0';
	}
	else
	{
		a[3] = c;
		if (found_gsd == 0)
		{
			found_gsd = 3;
		}
	}

	c = itoc((i % 1000)/100);
	if (c == '0' && found_gsd == 0)
	{
		a[2] = '\0';
	}
	else
	{
		a[2] = c;
		if (found_gsd == 0)
		{
			found_gsd = 2;
		}
	}

	c = itoc((i % 100)/10);
	if (c == '0' && found_gsd == 0)
	{
		a[1] = '\0';
	}
	else
	{
		a[1] = c;
		if (found_gsd == 0)
		{
			found_gsd = 1;
		}
	}

	a[0] = itoc(i % 10);

	if (found_gsd)
	{
		for (int x = 0; x < (found_gsd+1)>>1; x++)
		{
			a[found_gsd-x] ^= a[x];
			a[x] ^= a[found_gsd-x];
			a[found_gsd-x] ^= a[x];
		}
	}

	return a;
}

inline char upper(char c)
{
	if (c >= 'a' && c <= 'z')
	{
		return c - 'a' + 'A';
	}
	return c;
}

/** If the character c is lowercase then this function will turn it into its
    uppercase counterpart.
    \param c The lowercase character to convert.
 */
inline void to_upper(char *c)
{
	*c = upper(*c);
}

inline char lower(char c)
{
	if (c >= 'A' && c <= 'Z')
	{
		return c + 'a' - 'A';
	}
	return c;
}

/** If the character c is uppercase then this function will turn it into its
    lowercase counterpart.
    \param c The uppercase character to convert.
 */
inline void to_lower(char *c)
{
	*c = lower(*c);
}


/** Takes a string and converts it in all lowercase.
    \param str The string to lowercase.
 */
inline void to_lowerstr(char *str)
{
	while (*str)
	{
		to_lower(str);
		str++;
	}
}

/** Takes a string and converts it in all uppercase.
    \param str The string to uppercase.
 */
inline void to_upperstr(char *str)
{
	while(*str)
	{
		to_upper(str);
		str++;
	}
}

inline void cleanl(char *str)
{
	while (*str)
	{
		if (*str == '\n' || *str == '\r')
		{
			*str = '\0';
		}
		str++;
	}
}

inline int is_digit(char c)
{
	return c >= '0' && c <= '9';
}

inline int is_not_number(const char *str)
{
	while (*str)
	{
		if (is_digit(*str) == 0)
		{
			return 1;
		}
		str++;
	}
	return 0;
}

inline int is_number(const char *str)
{
	while (*str)
	{
		if (is_digit(*str) == 0)
		{
			return 0;
		}
		str++;
	}
	return 1;
}

inline const char *ordinalize(unsigned int i)
{
	if (i == 1)
	{
		return "st";
	}
	if (i == 2)
	{
		return "nd";
	}
	if (i == 3)
	{
		return "rd";
	}
	return "th";
}

#define C(ptr) ((ptr) ? (ptr) : "")

//MARK consider putting 40m at the end for black background
#define K_RED "\x1B[0;31m"
#define K_GRN "\x1B[0;32m"
#define K_YEL "\x1B[0;33m"
#define K_BLU "\x1B[0;34m"
#define K_MAG "\x1B[0;35m"
#define K_CYN "\x1B[0;36m"
#define K_WHT "\x1B[0;37m"

#define K_BLD_RED "\x1B[1;31m"
#define K_BLD_GRN "\x1B[1;32m"
#define K_BLD_YEL "\x1B[1;33m"
#define K_BLD_BLU "\x1B[1;34m"
#define K_BLD_MAG "\x1B[1;35m"
#define K_BLD_CYN "\x1B[1;36m"
#define K_BLD_WHT "\x1B[1;37m"

template<class t> class IntList
{
public:
	IntList(void);
	~IntList(void);
	int add(t);
	int remove(t);
	int has(t);
	int toFile(FILE *);
	int frFile(FILE *);
private:
	t *list;
	int listTop;
};

template<class t> IntList<t>::IntList(void)
{
	listTop = -1;
	list = NULL;
};

template<class t> IntList<t>::~IntList(void)
{
	free(list);
};

/** Adds a number to the IntList.
    \param t the number.
    \return 0 on failure, 1 on success
	 \todo maybe use a better data structure
 */
template<class t> int IntList<t>::add(t n)
{
	int bot, top, i, j;

	if (listTop == -1)
	{
		if ((list = (t *) malloc(sizeof(t)))==NULL)
		{
			errlog("/malloc/%s", strerror(errno));
			return 0;
		}
		listTop++;
		list[listTop] = n;
		return 1;
	}
	bot = 0;
	top = listTop;

	while (1)
	{
		i = (bot + top) / 2;

		if (list[i] == n)
		{
			return 1;
		}
		if (bot >= top)
		{
			listTop++;
			if ((list = (t *) realloc(list, sizeof(t) * (listTop+1)))==NULL)
			{
				errlog("/realloc/%s", strerror(errno));
				listTop--;
				return 0;
			}
			for (; i <= listTop-1; i++)
			{
				if (list[i] > n)
				{
					for (j = listTop; j > i; j--)
					{
						list[j] = list[j-1];
					}
				}
				break;
			}
			list[i] = n;
			return 1;
		}
		if (list[i] > n)
		{
			top = i - 1;
		}
		else
		{
			bot = i + 1;
		}
	}
}

/** Remove a number from the IntList.
    \param n The number to remove.
	 \return 0 - nothing removed, 1 - successful remove
	 \todo realloc this list shorter and use memmove
 */
template<class t> int IntList<t>::remove(t n)
{
	int bot, top, i;

	bot = 0;
	top = listTop;

	while (1)
	{
		i = (bot + top) / 2;
		if (list[i] == n)
		{
			listTop--;
			while (i <= listTop)
			{
				list[i] = list[++i];
			}
			return 1;
		}
		if (bot >= top)
		{
			return 0;
		}
		if (list[i] > n)
		{
			top = i - 1;
		}
		else
		{
			bot = i + 1;
		}
	}
}

/** Checks to see if the number n is in the list.
    \param n The number to check for.
    \return 0 is false, 1 is true.
 */
template<class t> int IntList<t>::has(t n)
{
	int bot, top, mid;

	if (listTop == -1)
	{
		return 0;
	}

	bot = 0;
	top = listTop;

	while (1)
	{
		mid = (bot + top) / 2;

		if (list[mid] == n)
		{
			return 1;
		}
		if (bot >= top)
		{
			return 0;
		}
		if (list[mid] > n)
		{
			top = mid - 1;
		}
		else
		{
			bot = mid + 1;
		}
	}
}

/** Writes the number list to the file.  We did not open the file here so
    do error analysis elsewhere.
    \param fd The file descriptor.
    \todo check the use of sizeof() , do not treat it like a function
 */
template<class t> int IntList<t>::toFile(FILE *fd)
{
	if (1 != fwrite(&listTop, sizeof(int), 1, fd))
	{
		return -1;
	}
	if (listTop > -1)
	{
		if (((size_t)listTop+1 != fwrite(list, sizeof(t), listTop+1, fd)))
		{
			return -1;
		}
	}
	return 0;
}

template<class t> int IntList<t>::frFile(FILE *fd)
{
	size_t size, list_size;

	if (1 != fread(&listTop, sizeof(int), 1, fd))
	{
		return -1;
	}
	assert(listTop >= -1);
	if (listTop > -1)
	{
		if ((list = (t *) malloc(((size_t)listTop+1)*sizeof(t)))==NULL)
		{
			return -1;
		}
		if ((size_t)listTop+1 != fread(list, sizeof(t), listTop+1, fd))
		{
			return -1;
		}
	}
	return 0;
}

/** A managed list of virtual ID's.
 */
typedef IntList<vid_t> VidList;

/** Compares two integer numbers and returns the maximum.
    \param a The first integer number.
    \param b The second integer number.
    \return The maximum integer number.
 */
inline int max(int a, int b)
{
	return a > b ? a : b;
}

/** Compares two integer numbers and returns the minimum.
    \param a The first integer number.
    \param b The second integer number.
    \return The minimum integer number.
 */
inline int min(int a, int b)
{
	return a < b ? a : b;
}

inline int nostr(const char *str)
{
	return (str==NULL || *str=='\0');
}

#define STRDUP(ptr, str) \
{\
	if (nostr(str))\
	{\
		ptr = NULL;\
	}\
	else if (((ptr) = strdup(str))==NULL)\
	{\
		exitlog("/strdup/%s", strerror(errno));\
	}\
}\

/** Returns a value indicating where a potential parent of the current index
    is located at according to the heap array data structure.
	 \param index The index being analyzed
	 \return An index to the parent of index
 */
inline size_t parent_heap(size_t index)
{
	return index >> 1;
}

/** Returns a value indicating where a potential left-child of the current
    index is lcoated at according to the heap array data structure.
	 \param index The index being analyzed
	 \return An index to the left-child of the index
 */
inline size_t left_heap(size_t index)
{
	return index << 1;
}

/** Returns a value indicating where a potential right-child of the current
    index is located at according to the heap array data structure.
	 \param index The index being analyzed
	 return An index to the right-child of the index
 */
inline size_t right_heap(size_t index)
{
	return left_heap(index) + 1;
}

/** Checks to see if name1 is equivalant to name2 regardless of case.
    \param name1 The first name
	 \param name2 The second name
	 \return 1 - true, 0 false
 */
inline int is_name(const char *name1, const char *name2)
{
	while (*name1 && *name2)
	{
		if (*name1 >= 'a' && *name1 <= 'z')
		{
			if (*name2 >= 'a' && *name2 <= 'z')
			{
				if (*name1 != *name2)
				{
					return 0;
				}
			}
			else
			{
				if (upper(*name1) != *name2)
				{
					return 0;
				}
			}
		}
		else
		{
			if (*name2 >= 'A' && *name2 <= 'Z')
			{
				if (*name1 != *name2)
				{
					return 0;
				}
			}
			else
			{
				if (*name1 != upper(*name2))
				{
					return 0;
				}
			}
		}
		name1++;
		name2++;
	}
	if (*name1 == '\0' && *name2 == '\0')
	{
		return 1;
	}
	return 0;
}

inline int is_equalstr(const char *str1, const char *str2)
{
	while (*str1 && *str2)
	{
		if (*str1 != *str2)
		{
			break;
		}
		str1++;
		str2++;
	}
	if (*str1 == '\0' && *str2 == '\0')
	{
		return 1;
	}
	return 0;
}

inline int is_substr(const char *little, const char *big)
{
	const char *q, *p;

	while (*big)
	{
		q = little;

		p = big;
		while (*p == *q)
		{
			q++;
			p++;
		}
		if (*q == '\0')
		{
			return 1;
		}
		big++;
	}
	return 0;
}

inline int is_abbrev(const char *little, const char *big)
{
	while (*little && *big)
	{
		if (*little >= 'a' && *little <= 'z')
		{
			if (*big >= 'a' && *big <= 'z')
			{
				if (*little != *big)
				{
					return 0;
				}
			}
			else
			{
				if (upper(*little) != *big)
				{
					return 0;
				}
			}
		}
		else
		{
			if (*big >= 'A' && *big <= 'Z')
			{
				if (*little != *big)
				{
					return 0;
				}
			}
			else
			{
				if (*little != upper(*big))
				{
					return 0;
				}
			}
		}
		little++;
		big++;
	}
	if (*little == '\0')
	{
		return 1;
	}
	return 0;
}

inline void copystr(char *to, const char *fr)
{
	while (*fr)
	{
		*to = *fr;
		to++;
		fr++;
	}
	*to = '\0';
}

inline void copylowerstr(char *to, const char *fr)
{
	while (*fr)
	{
		*to = lower(*fr);
		to++;
		fr++;
	}
	*to = '\0';
}

class Dictionary
{
public:
	Dictionary(void);
	virtual ~Dictionary(void);
	const char *add(const char *);
	int equals(const char *);
	const char *lookup(const char *);
	inline int count(void);
	char *addword(const char *);
	void listTo(Character *ch);
protected:
	char **word;
	int wordTop;
};

int Dictionary::count(void)
{
	return wordTop+1;
}

/** \brief Dictionary subset of sorted words. */
/** All the words in the Words class must be a subset of those in the
    dictionary.  The words entered must point to an entry in the dictionary
    and not create new memory for the word.
 */
class Words : public Dictionary
{
public:
	Words(const char *, const char *c = "extras");
	Words(Words *);
	~Words(void);
	int add(const char *);
	const char *words(void);
	int remove(const char *);
	void save(vid_t);
	void save(vid_t, top_t);
	void load(vid_t vid);
	void load(vid_t, top_t);
	inline const char *getTable(void);

	inline void debug(void);
private:
	const char *table;
	const char *column;
};

inline void Words::debug(void)
{
	for (top_t i = 0; i <= wordTop; i++)
	{
		fprintf(stderr, "  - %s\n", word[i]);
	}
}

inline const char *Words::getTable(void)
{
	return table;
}

/** \brief extra description structure */
/** This structure is for a keywords and associating description.
 */
typedef struct _extra_descr
{
	Words *key;
	char *proper;
	char *general;
}
EXTRA_DESCR;

class ExtraDescr
{
public:
	ExtraDescr(ExtraDescr *);
	ExtraDescr(const char *, vid_t);
	~ExtraDescr(void);
	EXTRA_DESCR *get(const char *);
	void save(vid_t);
	inline const char *getTable(void);
	void create(vid_t, top_t, char *, char *);
	void listTo(Container *);
	int add(const char *);
	int del(const char *);
	int checkFor(Container *);
private:
	const char *table;
	top_t exdescr_top;
	EXTRA_DESCR *exdescr;
	void free_exdescr(EXTRA_DESCR *);
	void load(vid_t);
};

inline const char *ExtraDescr::getTable(void)
{
	return table;
}

/** \brief This is for mapping virtual-id's to real-id's */
/** We will assume that creators will use up their space efficiently,
 */
class VidIndex
{
public:
	VidIndex(void);
	~VidIndex(void);
	void set(vid_t, vid_t);
	inline void reset(vid_t);
	inline vid_t get(vid_t);
	inline vid_t get(const char *);
	inline void clear(void);
private:
	int vidTop;
	vid_t *vidList;
};

/** Returns the value (realId) given a key (virtualId).
    \param virtualId the key 
    \return the value which is the realId, otherwise return (vid_t) -1
 */
vid_t VidIndex::get(vid_t virtualId)
{
	if ((sbyte4) virtualId > vidTop)
	{
		return (vid_t) -1;
	}
	return vidList[virtualId];
}

vid_t VidIndex::get(const char *virtualId)
{
	assert(is_not_number(virtualId)==0);
	return get(atoi(virtualId));
}

void VidIndex::reset(vid_t realId)
{
	for (int virtualId = 0; virtualId <= vidTop; virtualId++)
	{
		if (vidList[virtualId] == realId)
		{
			vidList[virtualId] = (vid_t) -1;
		}
	}
}

void VidIndex::clear(void)
{
	free(vidList);
	vidList = NULL;
	vidTop = -1;
}

#define MAX_TOKENS 10

template <class bitv_t> int set_flag(bitv_t *bitvector,
		const char *flag, const char **strarray)
{
	ubyte1 t, reset = 0;
	if (flag == NULL)
	{
		return 0;
	}
	if (flag[0] == '!')
	{
		flag++;
		reset = 1;
	}
	if (*flag == '\0')
	{
		return 0;
	}
	if ((t = find_index(flag, strarray)) == (ubyte1) -1)
	{
		return 0;
	}
	if (reset)
	{
		RESET(f_(t), *bitvector);
	}
	else
	{
		SET(f_(t), *bitvector);
	}
	return 1;
}

extern void strip_color(char *);
extern char *strarray_to_str(const char **, size_t max=0);
extern char *caps(const char *);
extern void init_utils_module(void);
extern ubyte2 number(ubyte2);
extern ubyte2 dice(ubyte2);
extern ubyte2 dice(ubyte2, ubyte2);
extern int file_exists(const char *);
extern void save_flags(vid_t, ubyte8, const char **, const char *);
extern void save_flags(vid_t, vid_t, ubyte8, const char **,
		const char *, const char *);
extern ubyte8 get_bit(const char *, const char **);

#define ADD_BIT(flag, list, flags) \
	if ((flags |= get_bit(flag, list)) == 0) \
	{ \
		exitlog("/get_bit/%s", argv[0]); \
	}

extern ubyte1 get_index(const char *, const char **);
extern ubyte1 find_index(const char *, const char **);
extern char *file_to_string(const char *);
extern void free_extra_descr(EXTRA_DESCR *);
extern char *va_complete(int *, char *, char *, int *, int);
extern void finit_utils_module(void);
extern void one_argument(char *, char *);
extern void tokenizer(char *, char **);
extern const char *time_ago(time_t);
extern const char *time_str(time_t);
extern void log_delay(void);
extern const int *order_list_index(const char **, int);
extern char *wrap(const char *);
extern void list_flags_to(Container *, ubyte8, const char **);
extern int set_index(ubyte1 *, const char *, const char **);
extern void coord_save(const char *, vid_t, COORD *);
extern void coord_load(const char *, vid_t, COORD *);

#define TOKENIZER(arg, argv)\
	char *argv[MAX_TOKENS];\
	tokenizer(arg, argv);

extern Dictionary dictionary;

#endif
