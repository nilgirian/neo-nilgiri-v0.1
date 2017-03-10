/* **************************************************************************
    Nilgiri the Forgotten World
    (C) 2004
   ************************************************************************** */
/** \file
    \brief Socket descriptor class handling header
 */

#ifndef _x_DESCRIPTOR_INCLUDE_x_
#define _x_DESCRIPTOR_INCLUDE_x_

#include<crypt.h>
#include"utils.h"
#include"player.h"
#include"constants.h"

#define SCROLL 22

typedef ubyte4 desc_flags_t;

enum DESC_FLAGS
{
		DESC_TO_GAME_PROMPT, ///<-
		DESC_START_TEXT_ROUND, ///<-
		DESC_CHARACTER_WRITING, ///<-
		DESC_CR_1L_WRITER, ///<-
		NO_DESC_FLAGS
};

extern void write_email_confirm(Player *, const char *);
extern void write_email_confirm(BASIC_PC *, const char *);

#define BASIC_PC_DIR "Players.new"

/** The minimum total of a players primary stats should be defined here. */
#define MIN_TOTAL_STATS 66

/** The maximum total of a players primary stats should be defined here. */
#define MAX_TOTAL_STATS 80

/** This should be the maximum input length per entry.  <B>MAX_INP_LEN</B>
    needs to be much much greater than <B>CHUNK_SIZE</B>.
    \todo Set a proper MAX_INP_LEN.
    \sa CHUNK_SIZE
  */
#define MAX_INP_LEN 512

#define MAX_NOTE_LEN 8192

/** \brief text block structure */
/** This is the structure for a text block.  It contains link-listed series of
    strings.
 */
typedef struct _txt_block
{
	char *txt;
	struct _txt_block *next;
}
TXT_BLOCK;

/** \brief text queue structure */
/** This is a text FIFO queue.  A <b>head</b> and <b>tail</b> are provided for
    queue manipulation.  The <b>head</b> text block can be popped off after
    use, and new text blocks can be queued to the <b>tail</b>.
 */
class TxtQ
{
public:
	TxtQ(void);
	~TxtQ(void);
	void push(char *);
	inline void pop(void);
	inline char *getTxt(void);
	inline int isEmpty(void);
private:
	TXT_BLOCK *head;
	TXT_BLOCK *tail;
};

/** Pops the head from a text queue and frees the text block memory.
 */
inline void TxtQ::pop(void)
{
	TXT_BLOCK *old_block;

	if (this->head==NULL)
	{
		return;
	}

	old_block = this->head;
	this->head = this->head->next;
	free(old_block->txt);
	free(old_block);

	if (this->head==NULL)
	{
		this->tail = NULL;
	}
}

/** \return The head of the text queue. */
char *TxtQ::getTxt(void)
{
	return this->head->txt;
}

/** \return non-zero - if the queue is empty, 0 - otherwise */
int TxtQ::isEmpty(void)
{
	return this->head == NULL;
}

/** The client_type type.
    \sa CLIENT_TYPES
 */
typedef ubyte1 client_types_t;
/** For enumerating the different client types.  */
enum CLIENT_TYPES
{
	CLIENT_APPLET, ///<-
	CLIENT_J2ME, ///<-
	CLIENT_SYMBIAN, ///<-
	CLIENT_AIM, ///<-
	CLIENT_PUEBLO, ///<-
	NO_CLIENT
};

union pc_data
{
	Player *data;
	BASIC_PC *temp;
};

typedef struct _descriptor_ctx
{
	Descriptor *descriptor;
}
DESCRIPTOR_CTX;

/** \brief the socket descriptor structure */
/** The descriptor contains basic socket handling information, threading and
    thread locking information, and ultimately a pointer to character
    information.
    \todo mtxQueueInp mtxQueueOut for asynchronous communication
 */
class Descriptor
{
public:
	Descriptor(void);
	~Descriptor(void);
	void print(const char *, ...);
	void put(const char *);
	void m_put(char *);
	int processInp(void);
	int processOut(void);
	inline int getDesc(void);
	inline unsigned long int getIP(void);
	char *getHostname(void);
	void setHostname(const char *);
	inline Descriptor *getNext(void);
	void handleInput(void);
	inline void resetThrID(void);
	void handleStateConnect(void);
	inline void interpret(void);
	inline int extracting(void);
	inline int isPlayingGame(void);
	inline const char *getName(void);
	inline const char *getEmail(void);
	inline const char *getState(void);
	inline time_t getIdleTime();
	void handleStateMotd(void);
	inline void extract(void);
	inline Character *getPlayer(void);
	inline void setPlayer(Player *);
	inline void setToGamePrompt(void);
	inline int characterWriting(void);
	inline void setCharacterWriting(void);
	inline void resetCharacterWriting(void);
	inline int cr1LWriter(void);
	inline void setCr1LWriter(void);
	inline void writer(char **, void (*fn)(void *), void *);

	void clearScreen(void);
	void scroll(int, int);
	void cursorHome(int, int);
	void moveCursor(int, int);
	void eraseDown(void);
	void scrollAll(void);
	inline void initSplitScreen(void);
	inline void finitSplitScreen(void);

	inline desc_env_flags_t envFlag(desc_env_flags_t);
	inline void setEnvFlag(desc_env_flags_t);
	inline void resetEnvFlag(desc_env_flags_t);
private:
	// file descriptor
	int desc;
	// the internet address in raw form
	ubyte4 ip;

	char hostname[MAXHOSTNAMELEN];

	client_types_t client;
	desc_flags_t flags;
	desc_env_flags_t environment;

	ubyte1 extraction;

	inline int toGamePrompt(void);
	inline void resetToGamePrompt(void);
	inline int startTextRound(void);
	inline void resetStartTextRound(void);
	inline void setStartTextRound(void);
	inline void resetCr1LWriter(void);

	void (Descriptor::*interpreter)(void);
	void handleStateQClient(void);
	void handleStateQName(void);
	void handleStateQNameC(void);
	void handleStateQPassword(void);
	void handleStateQEmail(void);
	void handleStateQEmailC(void);
	void handleStateQGender(void);
	void handleStateQRace(void);
	void handleStateQRaceC(void);
	void handleStateQHairLength(void);
	void handleStateQHairColor(void);
	void handleStateQHairShape(void);
	void handleStateQEyeColor(void);
	void handleStateQEyeShape(void);
	void handleStateQSkinComplexion(void);
	void handleStateQHeight(void);
	void handleStateQWeight(void);
	void handleStateQDemeanor(void);
	void handleStateQSelf(void);
	void handleStateQHomeland(void);
	void handleStateQOrientStats(void);
	void handleStateQProfession(void);
	void handleStateQStats(void);
	void handleStateQRandStats(void);
	void handleStateQRandProfession(void);
	void handleStateQHanded(void);
	void handleStateQMotdPassword(void);
	void handleStateQMenu(void);
	void handleStateQNewPasswordOld(void);
	void handleStateQNewPassword(void);
	void handleStateQNewPasswordC(void);
	void handleStateQNewEmail(void);
	void handleStateQNewEmailC(void);
	void handleStateQMenuPassword(void);
	void handleStateQMenuPause(void);
	void handleStateGameInterpreter(void);
	void handleStateWritingInterpreter(void);

	TxtQ inp;
	TxtQ out;

	// for retrieving input from the socket
	struct
	{
		char txt[MAX_INP_LEN];
		size_t len;
	}
	msg;

	struct
	{
		char txt[MAX_INP_LEN*2];
		size_t len;
	}
	buf;

	struct
	{
		char **target;
		char *text;
		size_t len;
		void (*fn)(void *);
		void *ctx;
	}
	notepad;

	pc_data pc;
	inline int isPCDataStruct(void);
	inline int isPCDataClass(void);

	// descriptor gets one thread
	pthread_t thrID;
	// mutex lock on hostname
	pthread_mutex_t mtxHostname;

	Descriptor *prev;
	Descriptor *next;

	Descriptor *prev_prompt;
	Descriptor *next_prompt;

	time_t lastActiveTime;

	DESCRIPTOR_CTX *interpreterCTX;
};

/** For putting already memory allocated strings onto the text queue
 */
inline void Descriptor::m_put(char *txt)
{
	if (envFlag(DENV_COLOR)==0)
	{
		strip_color(txt);
	}
	out.push(txt);
}

inline desc_env_flags_t Descriptor::envFlag(desc_env_flags_t t)
{
	return IS_SET(f_(t), environment);
}

inline void Descriptor::setEnvFlag(desc_env_flags_t t)
{
	SET(f_(t), environment);
}

inline void Descriptor::resetEnvFlag(desc_env_flags_t t)
{
	RESET(f_(t), environment);
}

inline void Descriptor::writer(char **target,
		void (*fn)(void *) = NULL, void *ctx = NULL)
{
	FREE(*target);
	put("Ok.  Go ahead and write. End the note with an '@' on a line by itself.\r\n] ");
	notepad.len = 0;
	notepad.fn = fn;
	notepad.ctx = ctx;
	FREE(notepad.text);
	notepad.target = target;
	interpreter = &Descriptor::handleStateWritingInterpreter;
}

inline int Descriptor::characterWriting(void)
{
	return interpreter == &Descriptor::handleStateWritingInterpreter;
}

inline void Descriptor::setCharacterWriting(void)
{
	SET(f_(DESC_CHARACTER_WRITING), flags);
}

inline void Descriptor::resetCharacterWriting(void)
{
	RESET(f_(DESC_CHARACTER_WRITING), flags);
}

inline int Descriptor::cr1LWriter(void)
{
	return IS_SET(f_(DESC_CR_1L_WRITER), flags);
}

inline void Descriptor::setCr1LWriter(void)
{
	SET(f_(DESC_CR_1L_WRITER), flags);
}

inline void Descriptor::resetCr1LWriter(void)
{
	RESET(f_(DESC_CR_1L_WRITER), flags);
}

inline void Descriptor::setToGamePrompt(void)
{
	SET(f_(DESC_TO_GAME_PROMPT), flags);
}

inline int Descriptor::toGamePrompt(void)
{
	return IS_SET(f_(DESC_TO_GAME_PROMPT), flags);
}

inline void Descriptor::resetToGamePrompt(void)
{
	RESET(f_(DESC_TO_GAME_PROMPT), flags);
}

inline int Descriptor::startTextRound(void)
{
	return IS_SET(f_(DESC_START_TEXT_ROUND), flags);
}

inline void Descriptor::setStartTextRound(void)
{
	SET(f_(DESC_START_TEXT_ROUND), flags);
}

inline void Descriptor::resetStartTextRound(void)
{
	RESET(f_(DESC_START_TEXT_ROUND), flags);
}

/** This begins the exctraction process.  The extraction process uses a counter
    that allows the descriptor a clean exit by delivering remaning messages and
	 not allowing any new input to be taken.
 */
inline void Descriptor::extract(void)
{
	if (extraction)
	{
		return;
	}
	extraction = 3;
}

/** This funciton returns a boolean value letting us know if the descriptor 
    should be extracted.
	 \return 0 - no, do not extract; 1 - yes, extract
 */
inline int Descriptor::extracting(void)
{
	if (extraction)
	{
		if (--extraction==0)
		{
			delete this;
		}
		return 1;
	}
	return 0;
}

/** Check to see if the descriptor is currently playing the game.  If this is
    set then pc.data->inRoom() should not be NULL.
	 \return 1 if true, otherwise return 0
 */
inline int Descriptor::isPlayingGame(void)
{
	return interpreter == &Descriptor::handleStateGameInterpreter
		|| interpreter == &Descriptor::handleStateWritingInterpreter;
}

/** \return The socket descriptor. */
inline int Descriptor::getDesc(void)
{
	return desc;
}

/** \return The 4-byte ip field */
inline ubyte4 Descriptor::getIP(void)
{
	return ip;
}

/** Clears the thread id field. */
inline void Descriptor::resetThrID(void)
{
	this->thrID = 0;
}

/** This is for loop statements.
    \return A pointer to the next descriptor in the list.
 */
inline Descriptor *Descriptor::getNext(void)
{
	return this->next;
}

inline void Descriptor::interpret(void)
{
	(this->*interpreter)();
}

extern Descriptor *descriptor_list;

extern void init_descriptor_module(void);

inline int Descriptor::isPCDataStruct(void)
{
	if (interpreter == &Descriptor::handleStateQNameC
	|| interpreter == &Descriptor::handleStateQEmail
	|| interpreter == &Descriptor::handleStateQEmailC
	|| interpreter == &Descriptor::handleStateQGender
	|| interpreter == &Descriptor::handleStateQRace
	|| interpreter == &Descriptor::handleStateQRaceC
	|| interpreter == &Descriptor::handleStateQHairLength
	|| interpreter == &Descriptor::handleStateQHairColor
	|| interpreter == &Descriptor::handleStateQHairShape
	|| interpreter == &Descriptor::handleStateQEyeColor
	|| interpreter == &Descriptor::handleStateQEyeShape
	|| interpreter == &Descriptor::handleStateQSkinComplexion
	|| interpreter == &Descriptor::handleStateQHeight
	|| interpreter == &Descriptor::handleStateQWeight
	|| interpreter == &Descriptor::handleStateQDemeanor
	|| interpreter == &Descriptor::handleStateQSelf
	|| interpreter == &Descriptor::handleStateQHomeland
	|| interpreter == &Descriptor::handleStateQOrientStats
	|| interpreter == &Descriptor::handleStateQProfession
	|| interpreter == &Descriptor::handleStateQStats
	|| interpreter == &Descriptor::handleStateQRandStats
	|| interpreter == &Descriptor::handleStateQRandProfession
	|| interpreter == &Descriptor::handleStateQHanded
	|| interpreter == &Descriptor::handleStateQMotdPassword)
	{
		return 1;
	}
	return 0;
}

inline int Descriptor::isPCDataClass(void)
{
	if (interpreter == &Descriptor::handleStateQPassword
	|| interpreter == &Descriptor::handleStateMotd
	|| interpreter == &Descriptor::handleStateQMenu
	|| interpreter == &Descriptor::handleStateQNewPasswordOld
	|| interpreter == &Descriptor::handleStateQNewPassword
	|| interpreter == &Descriptor::handleStateQNewPasswordC
	|| interpreter == &Descriptor::handleStateQNewEmail
	|| interpreter == &Descriptor::handleStateQNewEmailC
	|| interpreter == &Descriptor::handleStateQMenuPassword
	|| interpreter == &Descriptor::handleStateQMenuPause
	|| isPlayingGame())
	{
		return 1;
	}
	return 0;
}

inline const char *Descriptor::getState(void)
{
	if (interpreter == &Descriptor::handleStateQClient)
	{
		return "QClient";
	}
	if (interpreter == &Descriptor::handleStateQName)
	{
		return "QName";
	}
	if (interpreter == &Descriptor::handleStateQNameC)
	{
		return "QNameC";
	}
	if (interpreter == &Descriptor::handleStateQEmail)
	{
		return "QEmail";
	}
	if (interpreter == &Descriptor::handleStateQEmailC)
	{
		return "QEmailC";
	}
	if (interpreter == &Descriptor::handleStateQGender)
	{
		return "QGender";
	}
	if (interpreter == &Descriptor::handleStateQRace)
	{
		return "QRace";
	}
	if (interpreter == &Descriptor::handleStateQRaceC)
	{
		return "QRaceC";
	}
	if (interpreter == &Descriptor::handleStateQHairLength)
	{
		return "QHairLength";
	}
	if (interpreter == &Descriptor::handleStateQHairColor)
	{
		return "QHairColor";
	}
	if (interpreter == &Descriptor::handleStateQHairShape)
	{
		return "QHairShape";
	}
	if (interpreter == &Descriptor::handleStateQEyeColor)
	{
		return "QEyeColor";
	}
	if (interpreter == &Descriptor::handleStateQEyeShape)
	{
		return "QEyeShape";
	}
	if (interpreter == &Descriptor::handleStateQSkinComplexion)
	{
		return "QSkinComplexion";
	}
	if (interpreter == &Descriptor::handleStateQHeight)
	{
		return "QHeight";
	}
	if (interpreter == &Descriptor::handleStateQWeight)
	{
		return "QWeight";
	}
	if (interpreter == &Descriptor::handleStateQDemeanor)
	{
		return "QDemeanor";
	}
	if (interpreter == &Descriptor::handleStateQSelf)
	{
		return "QSelf";
	}
	if (interpreter == &Descriptor::handleStateQHomeland)
	{
		return "QHomeland";
	}
	if (interpreter == &Descriptor::handleStateQOrientStats)
	{
		return "QOrientStats";
	}
	if (interpreter == &Descriptor::handleStateQProfession)
	{
		return "QProfession";
	}
	if (interpreter == &Descriptor::handleStateQStats)
	{
		return "Qstats";
	}
	if (interpreter == &Descriptor::handleStateQRandStats)
	{
		return "QRandStats";
	}
	if (interpreter == &Descriptor::handleStateQRandProfession)
	{
		return "QRandProfession";
	}
	if (interpreter == &Descriptor::handleStateQHanded)
	{
		return "QHanded";
	}
	if (interpreter == &Descriptor::handleStateQMotdPassword)
	{
		return "QMotdPassword";
	}
	if (interpreter == &Descriptor::handleStateQPassword)
	{
		return "QPassword";
	}
	if (interpreter == &Descriptor::handleStateMotd)
	{
		return "Motd";
	}
	if (interpreter == &Descriptor::handleStateQMenu)
	{
		return "QMenu";
	}
	if (interpreter == &Descriptor::handleStateQNewPasswordOld)
	{
		return "QNewPasswordOld";
	}
	if (interpreter == &Descriptor::handleStateQNewPassword)
	{
		return "QNewPassword";
	}
	if (interpreter == &Descriptor::handleStateQNewPasswordC)
	{
		return "QNewPasswordC";
	}
	if (interpreter == &Descriptor::handleStateQNewEmail)
	{
		return "QNewEmail";
	}
	if (interpreter == &Descriptor::handleStateQNewEmailC)
	{
		return "QNewEmailC";
	}
	if (interpreter == &Descriptor::handleStateQMenuPassword)
	{
		return "QMenuPassword";
	}
	if (interpreter == &Descriptor::handleStateQMenuPause)
	{
		return "QMenuPause";
	}
	if (isPlayingGame())
	{
		return "PlayingGame";
	}
	return "n/a";
}

inline const char *Descriptor::getName(void)
{
	if (isPCDataStruct())
	{
		return pc.temp->name;
	}
	if (isPCDataClass())
	{
		return pc.data->getName();
	}
	return "";
}

inline const char *Descriptor::getEmail(void)
{
	if (isPCDataStruct())
	{
		return pc.temp->email;
	}
	if (isPCDataClass())
	{
		return pc.data->getEmail();
	}
	return "";
}

inline Character *Descriptor::getPlayer(void)
{
	if (isPCDataClass())
	{
		return (Character *) pc.data;
	}
	return NULL;
}

inline void Descriptor::setPlayer(Player *ch)
{
	if (isPCDataClass())
	{
		pc.data = ch;
	}
}

inline time_t Descriptor::getIdleTime(void)
{
	return time0 - lastActiveTime;
}

inline void Descriptor::initSplitScreen(void)
{
	clearScreen();
	scroll(1, SCROLL);
	moveCursor(SCROLL, 1);
}

inline void Descriptor::finitSplitScreen(void)
{
	scrollAll();
	moveCursor(SCROLL+1, 1);
	eraseDown();
}

extern const char *basic_description(const BASIC_PC *);
extern void send_to_descriptors(const char *, ...);

#endif
