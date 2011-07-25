/*
 *  File:      enemy_db.h
 *  Summary:    all of the monsters
 *  Written by: Javon Harper
 */

enum EnemyStates
{
  STATE_WANDERING = 0,
  STATE_SEARCHING,
  STATE_HOSTILE,
  TOTAL_ENEMY_STATES
};

const static int ATT_MAX           = 20;
const static int ATT_GOOD          = 15;
const static int ATT_AVERAGE       = 10;
const static int ATT_BAD           = 5;
const static int ATT_MIN           = 1;

enum EnemyTypes
{
  PLAYER = 0,
  EN_TEST_ENEMY,
//  EN_DIRE_RAT,
//  EN_FERAL_HOUND,
//  EN_CRYPT_WORM,
//  EN_BARBED_BOAR,
//  EN_GIANT_LEECH,
//  EN_DIRE_BAT,
//  EN_GOBLIN_MASTIFF,
//  EN_REGAL_EYE,
//  EN_REACHER,
//  EN_CHUUL,
//  EN_DUST_DEVIL,
//  EN_DOPPELGANGER,
//  EN_FLASH_FIEND,
//  EN_FIRE_ELEMENTAL,
//  EN_EARTH_ELEMENTAL,
//  EN_WATER_ELEMENTAL,
//  EN_CLAY_GOLEM,
//  EN_FLASH_GOLEM,
//  EN_STONE_GOLEM,
//  EN_SAND_GOLEM,
//  EN_GOPAGA,
//  EN_MONZOL,
//  EN_DARK_KNIGHT_OF INGLIP,
//  EN_SORCERER_OF_INGLIP,
//  EN_PRIEST_OF_INGLIP,
//  EN_TOMB_THEIF,
//  EN_DEMON_GUARDIAN,
//  EN_VERVAL,
//  EN_SCREAMING_HORROR,
//  EN_INFERNO_HORROR,
//  EN_ABOMINATION,

//uniques lie beyond here.
//  EN_IMECISH,
//  EN_FEATIOUL,
//  EN_MOUTICK,
//  EN_BROOK,
//  EN_JDAMENT,
//  EN_DRAPITY,
//  EN_SCUTICA,
//  EN_PLASBA,
//  EN_LERVIDAN,
//  EN_GRIGAN,
    TOTAL_ENEMY_TYPES
};

struct enemy_data_t {

    int habitat[2];//min and max levels this enemy resides
    std::string name;
    int face_tile;
    TCODColor color;

    int base_health;
    int base_mana;

    int unarmed_damage[2];

    int att_str;
    int att_dex;
    int att_wis;
    int att_vit;

    int mod_speed;
    int mod_size;
    int mod_base_ac;

    int sight_range;
    int smell_range;
    int hear_range;
};

const static enemy_data_t enemy_db[] = {
//  {
//    {habmin, habmax},
//    "name", 'char', TCODColor::color,
//    health,   mana, damage
//    str,   dex,  wis, vit,
//    speed  size, base armour,
//    sight,   smell,  hearing
//  },
    {
        {-1, -1},
        "player", '@', TCODColor::white,
        10,   5, {1, 3},
        ATT_AVERAGE, ATT_AVERAGE, ATT_AVERAGE, ATT_AVERAGE,
        ATT_AVERAGE, ATT_AVERAGE, ATT_AVERAGE,
        ATT_AVERAGE, ATT_AVERAGE, ATT_AVERAGE
    },
    {
        {0, 10},
        "test enemy", 't', TCODColor::green,
        10,   5, {1, 3},
        ATT_AVERAGE, ATT_AVERAGE, ATT_AVERAGE, ATT_AVERAGE,
        ATT_AVERAGE, ATT_AVERAGE, ATT_AVERAGE,
        ATT_AVERAGE, ATT_AVERAGE, ATT_AVERAGE
    }
};
