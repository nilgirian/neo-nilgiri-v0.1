/* **************************************************************************
    Nilgiri the Forgotten World
    (C) 2004
   ************************************************************************** */
/** \file
    \brief Player object header
 */

#ifndef _x_PLAYER_INCLUDE_x_
#define _x_PLAYER_INCLUDE_x_
#include"tinyworld_character.h"

typedef struct _cid_name_data
{
	cid_t cid;
	char *proper;
}
CID_NAME_DATA;

typedef struct _name_cid_data
{
	const char *name;
	const char *proper;
	cid_t cid;
}
NAME_CID_DATA;

/** \brief For temporary character data. */
/** This is for temporary character information during user player creation.
 */
typedef struct _basic_pc
{
	char name[MAX_NAME_LEN+1];
	char password[MAX_ENCRYPTED_LEN+1];
	char email[MAX_EMAIL_LEN+1];
	char birth_host[MAXHOSTNAMELEN+1+MAXIPLEN+1];

	gender_t gender;
	homeland_t homeland;
	profession_flags_t profession;
	race_t race;

	hair_color_t hair_color;
	hair_length_t hair_length;
	hair_shape_t hair_shape;
	eye_color_t eye_color;
	eye_shape_t eye_shape;
	skin_complexion_t skin_complexion;
	weight_t weight;
	height_t height;
	demeanor_t demeanor;
	handed_t handed;

	stat_t stat[NO_STAT];
}
BASIC_PC;

typedef struct _load_pc_data
{
	cid_t cid;
	char *name;
	char *password;
	char *email;
	char *birth_host;
	vid_t char_data_id;
	hair_color_t hair_color;
	hair_length_t hair_length;
	hair_shape_t hair_shape;
	eye_color_t eye_color;
	eye_shape_t eye_shape;
	skin_complexion_t skin_complexion;
	weight_t weight;
	height_t height;
	demeanor_t demeanor;
	handed_t handed;
	stat_t strength;
	stat_t dexterity;
	stat_t intelligence;
	stat_t wisdom;
	stat_t constitution;
	stat_t charisma;
	stat_t strength_add;
	time_t birth;
	time_t last_save;
	time_t last_logon;
	time_t played;
	int age_mod;
}
LOAD_PC_DATA;

/** Return the ANSI color code associated with a characters profession.
    \param *ch The character being analyzed
 */
inline const char *c_prf(Character *ch)
{
	if (ch->Imm)
	{
		return K_BLD_YEL;
	}
	if (ch->getPrimaryProfession() == WARRIOR)
	{
		return K_BLD_RED;
	}
	if (ch->getPrimaryProfession() == THIEF)
	{
		return K_BLD_BLU;
	}
	if (ch->getPrimaryProfession() == MONK)
	{
		return K_BLD_WHT;
	}
	if (ch->getPrimaryProfession() == MAGE)
	{
		return K_BLD_MAG;
	}
	if (ch->getPrimaryProfession() == CLERIC)
	{
		return K_BLD_CYN;
	}
	if (ch->getPrimaryProfession() == DRUID)
	{
		return K_BLD_GRN;
	}
	return "";
}

/** \brief The Player Character class is an extension of your standard
     character.
 */
class Player : public Character
{
public:
	Player(BASIC_PC *, Descriptor *);
	Player(LOAD_PC_DATA *, Descriptor *);
	~Player(void);
	void save(void);
	cid_t load(const char *);

	virtual inline Player *objPlayer(void);

	const char *getName(void);
	const char *getDescr(void);
	const char *getDescrRoom(void);
	void setName(const char *);
	int isCalled(const char *);

	void pointUpdate(void);

   int isPassword(const char *);
	void setPassword(const char *);

	const char *getEmail(void);
	void setEmail(const char *);

	const char *getBirthHost(void);
	void setBirthHost(const char *);

	hair_color_t getHairColor(void);
	void setHairColor(hair_color_t);

	hair_length_t getHairLength(void);
	void setHairLength(hair_length_t);

	hair_shape_t getHairShape(void);
	void setHairShape(hair_shape_t);

	eye_color_t getEyeColor(void);
	void setEyeColor(eye_color_t);

	eye_shape_t getEyeShape(void);
	void setEyeShape(eye_shape_t);

	skin_complexion_t getSkinComplexion(void);
	void setSkinComplexion(skin_complexion_t);

	demeanor_t getDemeanor(void);
	void setDemeanor(demeanor_t);

	handed_t getHanded(void);
	void setHanded(handed_t);

	time_t getPlayed(void);
	void addPlayed(time_t);

	time_t getAge(void);
	time_t getAgeMod(void);
	time_t getBirthDate(void);
	void addAgeMod(int);

	time_t getLastSave(void);
	void setLastSave(time_t);

	time_t getLastLogon(void);
	void setLastLogon(time_t);

	level_t getLevel(void);

	Player *getNextPlayer(void);
	void reconnect(Descriptor *);

	char *header(void);
	player_env_flags_t envFlag(player_env_flags_t);
	void setEnvFlag(player_env_flags_t);
	void resetEnvFlag(player_env_flags_t);

	virtual const char *getMovement(void);

private:
	void createPlayerDescr(const char *);

	profession_t profession;

	hair_color_t hair_color;
	hair_length_t hair_length;
	hair_shape_t hair_shape;
	eye_color_t eye_color;
	eye_shape_t eye_shape;
	skin_complexion_t skin_complexion;
	demeanor_t demeanor;
	handed_t handed;

	ubyte1 realId;
	const char *name;
	const char *description;

	char password[MAX_ENCRYPTED_LEN+1];
	char *email;
	char *birthHost;
	char *title;

	time_t birth;
	time_t last_save;
	time_t last_logon;
	time_t played;
	int age_mod;

	player_env_flags_t environment;
	player_affect_flags_t affects;

	ubyte4 experience;
	ubyte4 max_experience;
	ubyte4 battle_experience;

	vid_t room_reception;

	ubyte1 page_size;

	ubyte2 deaths;
	ubyte2 kills;
	ubyte2 pkills;
	ubyte2 pdeaths;
	VidList explored;

	level_t level;

	Player *next_player_character;
	Player *prev_player_character;
};

inline Player *Player::objPlayer(void)
{
	return this;
}

inline const char *Player::getMovement(void)
{
	if (Imm)
	{
		return str_movement[FLOATS];
	}
	return Character::getMovement();
}

#define MAX_HEADER_LEN 9

/** Checks to see if the player can be called by a particular name.
    \param keyword the name to check against the player
 */
inline int Player::isCalled(const char *keyword)
{
	assert(keyword);
	if (is_name(keyword, name))
	{
		return 1;
	}
	return Character::isCalled(keyword);
}

inline player_env_flags_t Player::envFlag(player_env_flags_t t)
{
	return IS_SET(f_(t), environment);
}

inline void Player::setEnvFlag(player_env_flags_t t)
{
	SET(f_(t), environment);
}

inline void Player::resetEnvFlag(player_env_flags_t t)
{
	RESET(f_(t), environment);
}

inline level_t Player::getLevel(void)
{
	return 28;
}

inline char *Player::header(void)
{
	static char header_text[MAX_HEADER_LEN+COLORLEN+1];
	const char *c = c_prf(this);

	header_text[0] = *c;
	header_text[1] = *(c+1);
	header_text[2] = *(c+2);
	header_text[3] = *(c+3);
	header_text[4] = *(c+4);
	header_text[5] = *(c+5);
	header_text[6] = *(c+6);
	header_text[7] = '[';
	if (Imm)
	{
		header_text[8] = ' ';
		header_text[9] = ' ';
		header_text[10] = *str_immortal_types_u[Imm->getLevel()];
		header_text[11] = *(str_immortal_types_u[Imm->getLevel()]+1);
		header_text[12] = *(str_immortal_types_u[Imm->getLevel()]+2);
		header_text[13] = ' ';
		header_text[14] = ' ';
	}
	else
	{
		header_text[8] = getLevel() / 10 ? itoc(getLevel() / 10) : ' ';
		header_text[9] = itoc(getLevel() % 10);
		header_text[10] = ' ';
		header_text[11] = *str_race_header[getRace()];
		header_text[12] = *(str_race_header[getRace()]+1);
		header_text[13] = *str_profession_header[getPrimaryProfession()];
		header_text[14] = *(str_profession_header[getPrimaryProfession()]+1);
	}
	header_text[15] = ']';
	header_text[16] = '\0';
	return header_text;
}
inline Player *Player::getNextPlayer(void)
{
	return next_player_character;
}

inline const char *Player::getName(void)
{
	return name;
}

inline const char *Player::getDescr(void)
{
	return "MARK player description";
}

inline const char *Player::getDescrRoom(void)
{
	static char buf[LONG_LINE_LENGTH];

	sprintf(buf, "%s is here.%s",
			getName(),
			isUnlinked() ? " (linkdead)" : "");
	return buf;
}

inline int Player::isPassword(const char *arg)
{
	return strncmp(password, crypt(arg, password), MAX_ENCRYPTED_LEN)==0;
}

inline void Player::setPassword(const char *arg)
{
	strncpy(password, crypt(arg, name), MAX_ENCRYPTED_LEN);
	password[MAX_ENCRYPTED_LEN] = '\0';
}

inline const char *Player::getEmail(void)
{
	return email;
}

inline void Player::setEmail(const char *arg)
{
	size_t len = strlen(arg);
	assert(len <= MAX_EMAIL_LEN);
	if (email)
	{
		free(email);
	}
	MALLOC(email, char, len+1);
	strcpy(email, arg);
}

inline const char *Player::getBirthHost(void)
{
	return birthHost;
}

inline hair_color_t Player::getHairColor(void)
{
	return hair_color;
}

inline void Player::setHairColor(hair_color_t h)
{
	assert(h <= NO_HAIR_COLOR);
	hair_color = h;
}

inline hair_length_t Player::getHairLength(void)
{
	return hair_length;
}

inline void Player::setHairLength(hair_length_t h)
{
	assert(h <= NO_HAIR_LENGTH);
	hair_length = h;
}

inline hair_shape_t Player::getHairShape(void)
{
	return hair_shape;
}

inline void Player::setHairShape(hair_shape_t h)
{
	assert(h <= NO_HAIR_SHAPE);
	hair_shape = h;
}

inline eye_color_t Player::getEyeColor(void)
{
	return eye_color;
}

inline void Player::setEyeColor(eye_color_t e)
{
	assert(e <= NO_EYE_COLOR);
	eye_color = e;
}

inline eye_shape_t Player::getEyeShape(void)
{
	return eye_shape;
}

inline void Player::setEyeShape(eye_shape_t e)
{
	assert(e <= NO_EYE_SHAPE);
	eye_shape = e;
}

inline skin_complexion_t Player::getSkinComplexion(void)
{
	return skin_complexion;
}

inline void Player::setSkinComplexion(skin_complexion_t s)
{
	assert(s <= NO_SKIN_COMPLEXION);
	skin_complexion = s;
}

inline demeanor_t Player::getDemeanor(void)
{
	return demeanor;
}

inline void Player::setDemeanor(demeanor_t d)
{
	assert(d <= NO_DEMEANOR);
	demeanor = d;
}

inline handed_t Player::getHanded(void)
{
	return handed;
}

inline void Player::setHanded(handed_t h)
{
	assert( h <= NO_HANDED);
	handed = h;
}

inline time_t Player::getLastSave(void)
{
	return last_save;
}

inline void Player::setLastSave(time_t t)
{
	assert(t);
	last_save = t;
}

inline time_t Player::getPlayed(void)
{
	return played;
}

inline void Player::addPlayed(time_t t)
{
	played += t;
}

inline time_t Player::getAge(void)
{
	return ((time0 - getBirth()-getAgeMod()) * MSECS_PER_SEC) + SEVENTEEN_MYEARS;
}

inline time_t Player::getBirthDate(void)
{
	return mtime0()-(((time0-getBirth()) * MSECS_PER_SEC) + SEVENTEEN_MYEARS);
}

inline time_t Player::getAgeMod(void)
{
	return age_mod;
}

inline void Player::addAgeMod(int t)
{
	age_mod += t;
}

inline time_t Player::getLastLogon(void)
{
	return last_logon;
}

inline void Player::setLastLogon(time_t t)
{
	assert(t);
	last_logon = t;
}

extern Player *getPlaying(Player *);
extern Player *start_character(BASIC_PC *);
extern Player *player_character_list;
extern LOAD_PC_DATA *load_player(const char *);
extern const char *get_name_cid_list(cid_t);
extern cid_t get_cid_name_list(const char *);
extern const char *get_player_name(const char *);
extern const char *add_cid_name(cid_t, const char *);
extern void init_player_module(void);
extern void finit_player_module(void);

#endif
