CREATE TABLE char_stats_data
(
	char_stats_id integer primary key,
	race text,
	gender text,
	movement_type text,

	weight_class text,
	height_class text,
	volume_class text,

	homeland text,

	strength integer,
	dexterity integer,
	intelligence integer,
	wisdom integer,
	constitution integer,
	charisma integer,
	strength_add integer,

	save_vs_paralyzation integer,
	save_vs_rod integer,
	save_vs_petrification integer,
	save_vs_breath integer,
	save_vs_spell integer,

	hitpoints_die integer,
	hitpoints_multiplier integer,
	hitpoints_modifier integer,

	thac0 integer,
	position text,

	creators_note text
);

CREATE TABLE char_stats_flags
(
	char_stats_id integer,
	flag text not null
);

CREATE TABLE char_stats_dmm
(
	char_stats_id integer key,
	attack_id integer key,
	attack_type integer not null,
	die integer not null,
	multiplier integer not null,
	modifier integer not null
);

CREATE TABLE char_descr_data
(
	char_descr_id integer primary key,

	proper text not null,
	room text not null,
	general text not null,

	creators_note text not null
);

CREATE TABLE char_descr_keywords
(
	char_descr_id integer key,
	keyword text key
);

CREATE TABLE char_data
(
	char_data_id integer primary key,

	char_descr_id integer not null,
	char_stats_id integer not null,
	clan integer,
	max_existing integer not null,

	creators_note text
);

CREATE TABLE players
(
 	player_id integer primary key,
	name text not null unique,
	password text not null,
	email text not null,
	birth_host text not null,

	char_data_id integer,

	hair_color text,
	hair_length text not null,
	hair_shape text,
	eye_color text not null,
	eye_shape text not null,
	skin_complexion text not null,
	weight integer not null,
	height integer not null,
	demeanor text not null,
	handed text not null,

	strength integer not null,
	dexterity integer not null,
	intelligence integer not null,
	wisdom integer not null,
	constitution integer not null,
	charisma integer not null,
	strength_add integer not null,

	birth integer,
	last_save integer,
	last_logon integer,
	played integer,
	age_mod integer
);

CREATE TABLE player_flags
(
	player_id integer key,
	flag text key
);

CREATE TABLE character_deaths
(
   is_player_npc integer not null,
	player_id integer not null,
	player_level integer not null,
	room_id integer not null,
	is_victim_npc integer not null,
	victim_id integer not null,
	victim_level integer not null,
	time integer not null
);

CREATE TABLE player_explored
(
	player_id integer key,
	room_id integer key
);

CREATE TABLE room
(
	room_id integer primary key,
	container_id integer not null,
	name text not null,
	descr text not null,
	descr_dark text,
	descr_storm text,
	sector text,
	tread integer not null,
	clan integer,
	creators_note
);

CREATE TABLE room_coord
(
	room_id integer not null,
	x integer not null,
	y integer not null,
	v integer not null
);

CREATE TABLE room_extras
(
 	room_id integer key,
	extras_id integer key,
	proper text not null,
	general text not null
);

CREATE TABLE room_extras_keywords
(
   room_id integer key,
	extras_id integer key,
	keyword text key
);

CREATE TABLE room_flags
(
	room_id integer key,
	flag text not null
);

CREATE TABLE room_door
(
	room_id integer key,
	door_id integer key,
	proper text,
	descr text,
	door_type text not null,
	weight integer not null,
	to_room_id integer,
	room_key integer
);

CREATE UNIQUE INDEX room_door_index ON room_door (room_id, door_id);

CREATE TABLE room_door_keywords
(
	room_id integer key,
	door_id integer key,
	keyword text key
);

CREATE TABLE room_door_flags
(
	room_id integer key,
	door_id integer key,
	flag text not null
);

CREATE TABLE social_data
(
	verb text primary key,
	min_vict_pos text not null,
	can_see_unseen integer not null,
	to_char_msg text not null,
	to_others_msg text,
	to_char_vict_msg,
	to_others_vict_msg,
	to_vict_vict_msg,
	to_char_vict_not_found_msg,
	to_char_self_msg,
	to_others_self_msg
);

CREATE TABLE item_stats_data
(
	item_stats_id integer primary key,
	item_type text not null,

	value0 text not null,
	value1 text not null,
	value2 text not null,
	value3 text not null,
	weight integer not null,
	volume integer not null,

	creators_note text
);

CREATE TABLE item_stats_flags
(
	item_stats_id integer primary key,
	flag text not null
);

CREATE TABLE item_descr_keywords
(
	item_descr_id integer key,
	keyword text key
);

CREATE TABLE item_descr_extras_keywords
(
	item_descr_id integer not null,
	extras_id integer not null,
	keyword text not null
);

CREATE TABLE item_descr_extras
(
	item_descr_id integer key,
	extras_id integer key,
	proper text not null,
	general text not null
);

CREATE TABLE item_descr_data
(
	item_descr_id integer primary key,
	proper text not null,
	room text not null,
	general text,
	creators_note text
);

CREATE TABLE item_data
(
	item_data_id integer primary key,
	item_descr_id integer key,
	item_stats_id integer key,
	clan integer,
	max_existing integer not null,
	creators_note text not null
);

CREATE TABLE help_mortal_table
(
	id integer primary key,
	entry text not null
);

CREATE TABLE help_mortal_table_keywords
(
	keyword text primary key,
	help_mortal_table_id integer not null
);

CREATE TRIGGER help_mortal_table_keywords_delete_trigger
AFTER DELETE ON help_mortal_table_keywords
BEGIN
	DELETE 
	FROM help_mortal_table
	WHERE id NOT IN
		(SELECT help_mortal_table_id FROM help_mortal_table_keywords);
END;

CREATE TRIGGER help_mortal_table_keywords_insert_trigger
AFTER INSERT ON help_mortal_table_keywords
BEGIN
	DELETE 
	FROM help_mortal_table
	WHERE id NOT IN
		(SELECT help_mortal_table_id FROM help_mortal_table_keywords);
END;

CREATE TRIGGER help_mortal_table_keywords_update_trigger
AFTER UPDATE ON help_mortal_table_keywords
BEGIN
	DELETE 
	FROM help_mortal_table
	WHERE id NOT IN
		(SELECT help_mortal_table_id FROM help_mortal_table_keywords);
END;

CREATE TABLE help_immortal_table
(
	id integer primary key,
	entry text not null
);

CREATE TABLE help_immortal_table_keywords
(
 	keyword text primary key,
	help_immortal_table_id integer not null
);

CREATE TRIGGER help_immortal_table_keywords_delete_trigger
AFTER DELETE ON help_immortal_table_keywords
BEGIN
	DELETE 
	FROM help_immortal_table
	WHERE id NOT IN
		(SELECT help_immortal_table_id FROM help_immortal_table_keywords);
END;

CREATE TRIGGER help_immortal_table_keywords_insert_trigger
AFTER INSERT ON help_immortal_table_keywords
BEGIN
	DELETE 
	FROM help_immortal_table
	WHERE id NOT IN
		(SELECT help_immortal_table_id FROM help_immortal_table_keywords);
END;

CREATE TRIGGER help_immortal_table_keywords_update_trigger
AFTER UPDATE ON help_immortal_table_keywords
BEGIN
	DELETE 
	FROM help_immortal_table
	WHERE id NOT IN
		(SELECT help_immortal_table_id FROM help_immortal_table_keywords);
END;

CREATE TABLE zone
(
	zone_id integer primary key,
	container_id integer not null,
	name text not null,
	descr text not null,
	zone_type text not null,
	homeland text not null,
	min_lvl integer,
	max_lvl integer,
	lifespan integer,
	top integer not null,
	creators_note text
);

CREATE TABLE zone_coord
(
	zone_id integer primary key,
	x,
	y,
	v
);

CREATE TABLE zone_flags
(
	zone_id integer not null,
	flag integer not null
);
