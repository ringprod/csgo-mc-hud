#ifndef DATA_H
#define DATA_H
#if 0
typedef struct csgoData
{
	bool isLive; // map.phase: live OR round.phase
	bool bombPlanted; // map.bomb: planted
	bool bombExploded; // map.bomb: exploded
	char name[33]; // player.name
	bool isActive; // player.activity: playing
	playerData player;
} csgoData;

typdef struct playerData
{
	int health;
	int armor;
	bool burning; // player.state.burning 0-255
	int deaths; // player.match_stats.deaths
	int totalKills; // player.match_stats.kills
	int roundKills; // player.state.round_kills
	int roundKillsHeadshot; // player.state.round_killhs
	int mvps; // player.match_stats.mvps

	weaponData weapon;
} playerData;

typedef struct weaponData {
	char name[20];
	bool isActive; // player.weapons.weapon_[i].state == "active"
	bool isEnchanted; // player.weapons.weapon_[i].paintkit != "default"
	weaponType type;
	int ammo_reserve;
} playerData;


typedef enum weaponType {                //Type(isMelee, isUtility, isGun, isPrimary, isSecondary)

	Pistol		  = "Pistol",            //PISTOL(false, false, true, false, true),
	Rifle		  = "Rifle",             //RIFLE(false, false, true, true, false),
	Knife		  = "Knife",             //KNIFE(true, false, false, false, false),
	Fists		  = "Fists",             //FISTS(true, false, false, false, false),
	Tablet		  = "Tablet",            //TABLET(false, false, false, false, false),
	StackableItem = "StackableItem",     //STACKABLE_ITEM(false, true, false, false, false),
	SubmachineGun = "Submachine Gun",    //SUBMACHINE_GUN(false, false, true, true, false),
	C4            =  "C4",               //BOMB(false, false, false, false, false),
	Melee         =  "Melee",            //MELEE(true, false, false, false, false),
	BreachCharge  =  "Breach Charge",    //BREACH_CHARGE(false, false, false, false, false),
	Grenade       =  "Grenade",          //GRENADE(false, true, false, false, false),
	MachineGun    =  "Machine Gun",      //MACHINE_GUN(false, false, true, true, false),
	Shotgun       =  "Shotgun",          //SHOTGUN(false, false, true, true, false),
	SniperRifle   =  "SniperRifle",      //SNIPER_RIFLE(false, false, true, true, false),
	BumpMine,     =  "Bump Mine",        //BUMP_MINE(false, false, false, false, false);
} weaponType;
#endif
typedef struct {
	char* phase; // warmup, live, intermission, gameover
	int round;
} Map;

typedef struct {
	char* phase;
	char* bomb;
} Round;

typedef struct {
	char* name;
	/*bool*/int hasSkin; //char* paintkit;
	char* type;
	/*bool*/int isActive; //char* state;
	int ammo_clip;
	int ammo_clip_max;
	int ammo_reserve;
} Weapon;

typedef struct {
	size_t count;
	Weapon weaponArray[16];
} Weapons;

typedef struct {
	char* name;
	char* activity;

	struct {
		int health;
		int armor;
		int burning;
		int round_kills;
		int round_killhs;
	} state;

	struct {
		int kills;
		int deaths;
		int mvps;
	} match_stats;

	Weapons weapons;
} Player;

typedef struct {
	Map map;
	Round round;
	Player player;
} GameData;
#endif // !DATA_H
