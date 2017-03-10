/* **************************************************************************
    Nilgiri the Forgotten World by:
    (C) 2004
   ************************************************************************** */
/** \file
    \brief Player object source
 */
#include"player.h"
#include"descriptor.h"
#include"addlog.h"

top_t cid_list_top = -1;
CID_NAME_DATA **cid_list;

top_t name_list_top = -1;
NAME_CID_DATA **name_list;

/** A map from homeland/race/gender/profession to the char data real ID */
vid_t pc_data_idx[NO_HOMELAND][NO_RACE][NO_GENDER][NO_PROFESSION];

/** A list of player characters in the game regardless if they are playing
    the game or in the pre-game, post-game screens.
 */
Player *player_character_list = NULL;

/** The array this function maps the CHAR_DATA vid based upon the homeland,
    race, gender and profession stats of a player character.  This function
    needs to mimic the create_tinyworld_characters() function.
 */
static void init_pc_data_idx(void)
{
	vid_t idx = 0;
	for (homeland_t h = JORA; h <= ARGO; h++)
	{
		for (race_t r = HUMAN; r <= PIXIE; r++)
		{
			for (gender_t g = FEMALE; g <= MALE; g++)
			{
				for (profession_t p = WARRIOR; p <= DRUID; p++)
				{
					pc_data_idx[h][r][g][p] = idx;
					idx++;
				}
			}
		}
	}
}

/** Checks to see if a name is currently a player character in the game.
    \param name This is the name to check in the playing list.  This assumes
	 name is alphabetical
	 \return 1 if true, 0 if false
 */
Player *getPlaying(Player *player)
{
	const char *p, *q;
	for (Player *c = player_character_list;
			c; c = c->getNextPlayer())
	{
		if (player == c)
		{
			continue;
		}
		if (is_name(player->getName(), c->getName()))
		{
			return c;
		}
	}
	return NULL;
}

/** Finds a player by name.
    \param victname The name of the player to find
	 \return The player object
 */
Player *Character::findPlayer(const char *victname)
{
	Player *p;
	for (p = player_character_list; p; p = p->getNextPlayer())
	{
		if (is_name(victname, p->getName()))
		{
			return p;
		}
	}
	return NULL;
}

/** Updates the points of the player at the appointed recurring point update
     time.
 */
void Player::pointUpdate(void)
{
	addPlayed(time0 - getLastLogon());
	setLastLogon(time0);
	Character::pointUpdate();
}

/** Load a standard player.
    \param pc Loaded pc data from the db
	 \param desc The descriptor to attach the player to
 */
Player::Player(LOAD_PC_DATA *pc, Descriptor *desc) : Character(desc, pc->cid)
{
	extern CHAR_DATA **char_data;

	if (is_name("Sin", pc->name) || is_name("Rivin", pc->name))
	{
		Imm = new ImmortalHelper(IMPLEMENTOR);
	}
	if (is_name("Natedog", pc->name) || is_name("Mobius", pc->name)
			|| is_name("Russ", pc->name))
	{
		Imm = new ImmortalHelper(DEUCE);
	}
	if (is_name("Motorola", pc->name) || is_name("Mark", pc->name))
	{
		Imm = new ImmortalHelper(GOD);
	}
	if (is_name("Barkeman", pc->name) || is_name("Mandessa", pc->name)
			|| is_name("Kath", pc->name) || is_name("Sephiroth", pc->name)
			|| is_name("Jazmin", pc->name) || is_name("Aryen", pc->name))
	{
		Imm = new ImmortalHelper(IMMORTAL);
	}

	age_mod = 0;
	played = 0;
	environment = 0;
	email = NULL;

	prev_player_character = NULL;
	if (player_character_list == NULL)
	{
		next_player_character = NULL;
	}
	else
	{
		next_player_character = player_character_list;
		player_character_list->prev_player_character = this;
	}
	player_character_list = this;

	setName(pc->name);
	strncpy(password, pc->password, MAX_ENCRYPTED_LEN);
	password[MAX_ENCRYPTED_LEN] = '\0';
	setEmail(pc->email);
	setBirthHost(pc->birth_host);
	data = char_data[pc->char_data_id];
	setHairColor(pc->hair_color);
	setHairLength(pc->hair_length);
	setHairShape(pc->hair_shape);
	setEyeColor(pc->eye_color);
	setEyeShape(pc->eye_shape);
	setSkinComplexion(pc->skin_complexion);
	setWeight(pc->weight);
	setHeight(pc->height);
	setDemeanor(pc->demeanor);
	setHanded(pc->handed);
	setStat(STRENGTH, pc->strength);
	setStat(DEXTERITY, pc->dexterity);
	setStat(INTELLIGENCE, pc->intelligence);
	setStat(WISDOM, pc->wisdom);
	setStat(CONSTITUTION, pc->constitution);
	setStat(CHARISMA, pc->charisma);
	setStat(STRENGTH_ADD, pc->strength_add);
	setBirth(pc->birth);
	setLastSave(pc->last_save);
	setLastLogon(pc->last_logon);
	addPlayed(pc->played);
	addAgeMod(pc->age_mod);

	game_stat.old_player_characters_created++;
	setWeight(10);
	setVolume(11);
}

/** Load a basic player made new in player creation.
    \param pc The basic pc data
	 \param desc The descriptor to attach the player to
 */
Player::Player(BASIC_PC *pc, Descriptor *desc) : Character(desc, 0)
{
	extern CHAR_DATA **char_data;

	vid_t idx = pc_data_idx[pc->homeland][pc->race][pc->gender][pc->profession]; 
	data = char_data[idx];

	STRDUP(description, basic_description(pc));

	if (is_name("Sin", pc->name) || is_name("Rivin", pc->name))
	{
		Imm = new ImmortalHelper(IMPLEMENTOR);
	}
	email = NULL;

	setName(pc->name);
	strncpy(password, pc->password, MAX_ENCRYPTED_LEN);
	password[MAX_ENCRYPTED_LEN] = '\0';
	setEmail(pc->email);
	setHairColor(pc->hair_color);
	setHairLength(pc->hair_length);
	setHairShape(pc->hair_shape);
	setEyeColor(pc->eye_color);
	setEyeShape(pc->eye_shape);
	setSkinComplexion(pc->skin_complexion);
	setDemeanor(pc->demeanor);
	setHanded(pc->handed);
	setStat(STRENGTH, pc->stat[STRENGTH]);
	setStat(DEXTERITY, pc->stat[DEXTERITY]);
	setStat(INTELLIGENCE, pc->stat[INTELLIGENCE]);
	setStat(WISDOM, pc->stat[WISDOM]);
	setStat(CONSTITUTION, pc->stat[CONSTITUTION]);
	setStat(CHARISMA, pc->stat[CHARISMA]);
	setStat(STRENGTH_ADD, pc->stat[STRENGTH_ADD]);
	setBirthHost(pc->birth_host);
fprintf(stderr, "C birth_host: [%s] [%s]\n", pc->birth_host, getBirthHost());
	setBirth(time0);
	setLastSave(time0);
	setLastLogon(time0);
	played = 0;
	age_mod = 0;
	environment = 0;

	prev_player_character = NULL;
	if (player_character_list == NULL)
	{
		next_player_character = NULL;
	}
	else
	{
		next_player_character = player_character_list;
		player_character_list->prev_player_character = this;
	}
	player_character_list = this;

	game_stat.new_player_characters_created++;
	setWeight(10); //MARK
	setVolume(11); //MARK
	addlog_create_player(this);
}

/** Player object destructor */
Player::~Player(void)
{
	if (birthHost)
	{
		free(birthHost);
	}

	if (prev_player_character)
	{
		prev_player_character->next_player_character = next_player_character;
	}
	else
	{
		player_character_list = next_player_character;
	}
	if (next_player_character)
	{
		next_player_character->prev_player_character = prev_player_character;
	}
	free(email);

	game_stat.player_characters_destroyed++;
}

/** Save the player to the database. */
void Player::save(void)
{
	char *dbErrMsg = NULL;

	setLastSave(time0);

	setHeight(9); //MARK some nifty formula

#ifdef NEVER_DEFINED
fprintf(stderr, "cid:%u\n", getCID());
fprintf(stderr,"name:%s\n", getName());
fprintf(stderr, "password:%s\n", password);
fprintf(stderr, "email:%s\n", getEmail());
fprintf(stderr, "birthHost:%s\n", getBirthHost());
fprintf(stderr, "charDataId:%u\n", getCharDataVid());
fprintf(stderr, "race:%s\n",  str_race[getRace()]);
fprintf(stderr, "hair color:%s\n", str_hair_color[getHairColor()]);
fprintf(stderr, "hair length:%s\n",  str_hair_length[getHairLength()]);
fprintf(stderr, "hair shape:%s\n", str_hair_shape[getHairShape()]);
fprintf(stderr, "eye color:%s\n", str_eye_color[getEyeColor()]);
fprintf(stderr, "eye shape:%s\n", str_eye_shape[getEyeShape()]);
fprintf(stderr, "skin complexion:%s\n", str_skin_complexion[getSkinComplexion()]);
fprintf(stderr, "demeanor:%s\n", str_demeanor[getDemeanor()]);
fprintf(stderr, "handed:%s\n", str_handed[getHanded()]);

fprintf(stderr, "weight:%d\n", getWeight());
fprintf(stderr, "height:%d\n", getHeight());

fprintf(stderr, "strength:%d\n", getStat(STRENGTH));
fprintf(stderr, "dexterity:%d\n", getStat(DEXTERITY));
fprintf(stderr, "intelligence:%d\n", getStat(INTELLIGENCE));
fprintf(stderr, "wisdom:%d\n", getStat(WISDOM));
fprintf(stderr, "constitution:%d\n", getStat(CONSTITUTION));
fprintf(stderr, "charisma:%d\n", getStat(CHARISMA));
fprintf(stderr, "strength add:%d\n", getStat(STRENGTH_ADD));
fprintf(stderr, "birth:%u\n", getBirth());
fprintf(stderr, "lastLogon:%u\n", getLastLogon());
fprintf(stderr, "played:%u\n", getPlayed());
fprintf(stderr, "age:%u\n", getAgeMod());
#endif

	SQLITE_EXEC_PRINTF(db, "REPLACE INTO players "
				"(player_id, name, password, email, birth_host,"
				" char_data_id,"
				" hair_color, hair_length, hair_shape,"
				" eye_color, eye_shape, skin_complexion,"
				" weight, height,"
				" demeanor, handed,"
				" strength, dexterity, intelligence, wisdom, constitution,"
				" charisma, strength_add,"
				" birth, last_save, last_logon, played, age_mod)"
				" VALUES "
				"(%u, %Q, %Q, %Q, %Q,"
				" %u,"
				" %Q, %Q, %Q,"
				" %Q, %Q, %Q,"
				" %u, %u,"
				" %Q, %Q,"
				" %u, %u, %u, %u, %u,"
				" %u, %u,"
				" %u, %u, %u, %u, %u)",
				NULL,
				NULL,
				dbErrMsg,
				getCID(),
					getName(),
					password,
					getEmail(),
					getBirthHost(),
				getCharDataVid(),
				getHairColor()==NO_HAIR_COLOR
						? NULL : str_hair_color[getHairColor()],
					str_hair_length[getHairLength()],
					getHairShape()==NO_HAIR_SHAPE
						? NULL : str_hair_shape[getHairShape()],
				str_eye_color[getEyeColor()],
					str_eye_shape[getEyeShape()],
					str_skin_complexion[getSkinComplexion()],
				getWeight(),
					getHeight(),
				str_demeanor[getDemeanor()],
					str_handed[getHanded()],
				getStat(STRENGTH),
					getStat(DEXTERITY),
					getStat(INTELLIGENCE),
					getStat(WISDOM),
					getStat(CONSTITUTION),
				getStat(CHARISMA),
					getStat(STRENGTH_ADD),
				getBirth(),
					getLastSave(),
					getLastLogon(),
					getPlayed(),
					getAgeMod());
}
 
/** We rely on the database constraint to limit us to one unique player
    name per name.
 */
int callback_players(void *pArg,
		int argc, char **argv, char **columnName)
{
	LOAD_PC_DATA *pc = (LOAD_PC_DATA *) pArg;

	assert (argc == 28);

	for (int i = 0; i < argc; i++)
	{
		if (strcmp(columnName[i], "player_id")==0)
		{
			pc->cid = atoi(argv[i]);
			continue;
		}
		if (strcmp(columnName[i], "name")==0)
		{
			STRDUP(pc->name, argv[i]);
			continue;
		}
		if (strcmp(columnName[i], "password")==0)
		{
			STRDUP(pc->password, argv[i]);
			continue;
		}
		if (strcmp(columnName[i], "email")==0)
		{
			STRDUP(pc->email, argv[i]);
			continue;
		}
		if (strcmp(columnName[i], "birth_host")==0)
		{
			STRDUP(pc->birth_host, argv[i]);
			continue;
		}
		if (strcmp(columnName[i], "char_data_id")==0)
		{
			pc->char_data_id = atoi(argv[i]);
			continue;
		}
		if (strcmp(columnName[i], "hair_color")==0)
		{
			hair_color_t h;

			if (argv[i]==NULL)
			{
				h = NO_HAIR_COLOR;
			}
			else
			{
				h = get_index(argv[i], str_hair_color);
			}
			pc->hair_color = h;
			continue;
		}
		if (strcmp(columnName[i], "hair_length")==0)
		{
			hair_length_t h;

			h = get_index(argv[i], str_hair_length);
			pc->hair_length = h;
			continue;
		}
		if (strcmp(columnName[i], "hair_shape")==0)
		{
			hair_shape_t h;

			if (argv[i]==NULL)
			{
				h = NO_HAIR_SHAPE;
			}
			else
			{
				h = get_index(argv[i], str_hair_shape);
			}
			pc->hair_shape = h;
			continue;
		}
		if (strcmp(columnName[i], "eye_color")==0)
		{
			eye_color_t e;

			e = get_index(argv[i], str_eye_color);
			pc->eye_color = e;
			continue;
		}
		if (strcmp(columnName[i], "eye_shape")==0)
		{
			eye_shape_t e;

			e = get_index(argv[i], str_eye_shape);
			pc->eye_shape = e;
			continue;
		}
		if (strcmp(columnName[i], "skin_complexion")==0)
		{
			skin_complexion_t s;

			s = get_index(argv[i], str_skin_complexion);
			pc->skin_complexion = s;
			continue;
		}
		if (strcmp(columnName[i], "weight")==0)
		{
			pc->weight = atoi(argv[i]);
			continue;
		}
		if (strcmp(columnName[i], "height")==0)
		{
			pc->height = atoi(argv[i]);
			continue;
		}
		if (strcmp(columnName[i], "demeanor")==0)
		{
			pc->demeanor = get_index(argv[i], str_demeanor);
			continue;
		}
		if (strcmp(columnName[i], "handed")==0)
		{
			pc->handed = get_index(argv[i], str_handed);
			continue;
		}
		if (strcmp(columnName[i], "strength")==0)
		{
			pc->strength = atoi(argv[i]);
			continue;
		}
		if (strcmp(columnName[i], "dexterity")==0)
		{
			pc->dexterity = atoi(argv[i]);
			continue;
		}
		if (strcmp(columnName[i], "intelligence")==0)
		{
			pc->intelligence = atoi(argv[i]);
			continue;
		}
		if (strcmp(columnName[i], "wisdom")==0)
		{
			pc->wisdom = atoi(argv[i]);
			continue;
		}
		if (strcmp(columnName[i], "constitution")==0)
		{
			pc->constitution = atoi(argv[i]);
			continue;
		}
		if (strcmp(columnName[i], "charisma")==0)
		{
			pc->charisma = atoi(argv[i]);
			continue;
		}
		if (strcmp(columnName[i], "strength_add")==0)
		{
			pc->strength_add = atoi(argv[i]);
			continue;
		}
		if (strcmp(columnName[i], "birth")==0)
		{
			pc->birth = atoi(argv[i]);
			continue;
		}
		if (strcmp(columnName[i], "last_save")==0)
		{
			pc->last_save = atoi(argv[i]);
			continue;
		}
		if (strcmp(columnName[i], "last_logon")==0)
		{
			continue;
		}
		if (strcmp(columnName[i], "played")==0)
		{
			pc->played = atoi(argv[i]);
			continue;
		}
		if (strcmp(columnName[i], "age_mod")==0)
		{
			pc->age_mod = atoi(argv[i]);
			continue;
		}
		exitlog("/column not used/%s", columnName[i]);
	}

	return 0;
}

/** Load the character from the database.
    \param pname The player's name
    \return 0 - success, 1 - no file, -1 error
 */
LOAD_PC_DATA *load_player(const char *pname)
{
	char *dbErrMsg = NULL;
	static LOAD_PC_DATA load_pc_data;

	load_pc_data.last_logon = time0;
	load_pc_data.cid = 0;

	SQLITE_EXEC_PRINTF(db, "SELECT * FROM players WHERE name like %Q",
				callback_players,
				&load_pc_data,
				dbErrMsg,
				pname);
	if (load_pc_data.cid)
	{
		return &load_pc_data;
	}
	return NULL;
}

/** Reassign the current players descriptor to a new descriptor.
    \param d The new descriptor
 */
void Player::reconnect(Descriptor *d)
{
	if (getDescriptor())
	{
		put("\r\n\r\nYou are being reconnected. Goodbye!\r\n");
		getDescriptor()->setPlayer(NULL);
		getDescriptor()->extract();
	}
	setDescriptor(d);
	getDescriptor()->setPlayer(this);
}

/** This is where the proper name is set.  This function should be called
    once.
	 \param proper The name of the player with proper capitalization
 */
void Player::setName(const char *proper)
{
	name = add_cid_name(getCID(), proper);
}

/** \todo check for null ips with no hostname */
inline void Player::setBirthHost(const char *arg)
{
	if (*arg == '\0' || arg == NULL)
	{
		STRDUP(birthHost, itoa(getDescriptor()->getIP()));
	}
	else
	{
		STRDUP(birthHost, arg);
	}
}

/** Adds a name information to the name list of players
    \param name The name from the dictionary
	 \param proper The name as it should be displayed
	 \param cid The CID of the player
 */
static void add_name_list(const char *name, const char *proper, cid_t cid)
{
	int bot, top, i;

	bot = 0;
	top = name_list_top;
	while (1)
	{
		i = (bot + top) / 2;
		if (name_list_top >= i && is_name(name_list[i]->name, name))
		{
			return;
		}
		if (bot >= top)
		{
			name_list_top++;
			REALLOC(name_list, NAME_CID_DATA *, name_list_top+1);

			for (; i <= name_list_top-1; i++)
			{
				if (strcmp(name_list[i]->name, name) > 0)
				{
					memmove(name_list+i+1, name_list+i,
							sizeof(NAME_CID_DATA *) * (name_list_top -i));
					break;
				}
			}
			NAME_CID_DATA *name_cid_data;
			MALLOC(name_cid_data, NAME_CID_DATA, 1);
			name_cid_data->name = name;
			name_cid_data->proper = proper;
			name_cid_data->cid = cid;
			name_list[i] = name_cid_data;
			return;
		}
		if (strcmp(name_list[i]->name, name) > 0)
		{
			top = i - 1;
		}
		else
		{
			bot = i + 1;
		}
	}
}

/** Adds a name to the cid_list of players
    \param cid The CID of the player
	 \param name The Dictionary name of the player
 */
const char *add_cid_name(cid_t cid, const char *name)
{
	int bot, top, i;

	bot = 0;
	top = cid_list_top;
	while (1)
	{
		i = (bot + top) / 2;
		if (cid_list_top >= i && cid_list[i]->cid == cid)
		{
			return cid_list[i]->proper;
		}
		if (bot >= top)
		{
			cid_list_top++;
			REALLOC(cid_list, CID_NAME_DATA *, cid_list_top+1);
			
			for (; i <= cid_list_top-1; i++)
			{
				if (cid_list[i]->cid > cid)
				{
					memmove(cid_list+i+1, cid_list+i,
							sizeof(CID_NAME_DATA *) * (cid_list_top -i));
					break;
				}
			}
			CID_NAME_DATA *cid_name_data;
			MALLOC(cid_name_data, CID_NAME_DATA, 1);
			cid_name_data->cid = cid;
			STRDUP(cid_name_data->proper, name);
			cid_list[i] = cid_name_data;
			add_name_list(dictionary.add(name), name, cid);
			return cid_list[i]->proper;
		}
		if (cid_list[i]->cid > cid)
		{
			top = i - 1;
		}
		else
		{
			bot = i + 1;
		}
	}
}

static int callback_load_cid_list(void *pArg,
		int argc, char **argv, char **columnName)
{
	assert (argc == 2);
	CID_NAME_DATA *cid_name_data;

	cid_list_top++;
	REALLOC(cid_list, CID_NAME_DATA *, cid_list_top+1);
	MALLOC(cid_name_data, CID_NAME_DATA, 1);

	for (int i = 0; i < argc; i++)
	{
		if (is_equalstr(columnName[i], "player_id"))
		{
			cid_name_data->cid = atoi(argv[i]);
			continue;
		}
		if (is_equalstr(columnName[i], "name"))
		{
			STRDUP(cid_name_data->proper, argv[i]);
			continue;
		}
		exitlog("/column not used/%s", columnName[i]);
	}
	cid_list[cid_list_top] = cid_name_data;
	add_name_list(dictionary.add(cid_name_data->proper),
			cid_name_data->proper,
			cid_name_data->cid);
	return 0;
}

/** Lodas the CID list of players. */
static void load_cid_list(void)
{
	char *dbErrMsg;

	SQLITE_EXEC(db, "SELECT player_id, name FROM players ORDER BY player_id",
			callback_load_cid_list,
			NULL,
			dbErrMsg);
}

/** Return the proper name of the player with the given name.
    \param name The name of the player
 */
const char *get_player_name(const char *name)
{
	int bot, top, mid;

	if (name_list_top == -1)
	{
		return 0;
	}

	bot = 0;
	top = name_list_top;

	while (1)
	{
		mid = (bot + top) / 2;

		if (is_name(name_list[mid]->name, name))
		{
			return name_list[mid]->proper;
		}
		if (bot >= top)
		{
			return NULL;
		}
		if (strcasecmp(name_list[mid]->name, name) > 0)
		{
			top = mid - 1;
		}
		else
		{
			bot = mid + 1;
		}
	}
}

/** Returns the cid of the name in the name list.
    \param name The name of the player
	 return The cid of the player
 */
cid_t get_cid_name_list(const char *name)
{
	int bot, top, mid;

	if (name_list_top == -1)
	{
		return 0;
	}

	bot = 0;
	top = name_list_top;

	while (1)
	{
		mid = (bot + top) / 2;

		if (is_name(name_list[mid]->name, name))
		{
			return name_list[mid]->cid;
		}
		if (bot >= top)
		{
			return (cid_t) -1;
		}
		if (strcasecmp(name_list[mid]->name, name) > 0)
		{
			top = mid - 1;
		}
		else
		{
			bot = mid + 1;
		}
	}
}

/** Get the name of the player from the cid list
    \param cid The cid of the player
	 \return The name of the player with the given CID
 */
const char *get_name_cid_list(cid_t cid)
{
	int bot, top, mid;

	if (cid_list_top == -1)
	{
		return 0;
	}

	bot = 0;
	top = cid_list_top;

	while (1)
	{
		mid = (bot + top) / 2;

		if (cid_list[mid]->cid == cid)
		{
			return cid_list[mid]->proper;
		}
		if (bot >= top)
		{
			return NULL;
		}
		if (cid_list[mid]->cid > cid)
		{
			top = mid - 1;
		}
		else
		{
			bot = mid + 1;
		}
	}
}
void init_player_module(void)
{
	load_cid_list();
	init_pc_data_idx();
}


void finit_player_module(void)
{
	for (top_t i = 0; i <= cid_list_top; i++)
	{
		free(cid_list[i]->proper);
		free(cid_list[i]);
	}
	free(cid_list);

	for (top_t i = 0; i<= name_list_top; i++)
	{
		free(name_list[i]);
	}
	free(name_list);
}
