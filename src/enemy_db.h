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

enum AttributeTypes
{
  ATT_STR = 0,
  ATT_DEX,
  ATT_INT,
  ATT_VIT
};

const static int ATT_MAX           = 20;
const static int ATT_VERY_GOOD     = 17;
const static int ATT_GOOD          = 15;
const static int ATT_ABOVE_AVERAGE = 13;
const static int ATT_AVERAGE       = 10;
const static int ATT_BELOW_AVERAGE = 7;
const static int ATT_BAD           = 5;
const static int ATT_VERY_BAD      = 3;
const static int ATT_MIN           = 1;

const static int SIZE_COLOSSAL    = -8;
const static int SIZE_GARGANTUAN  = -4;
const static int SIZE_HUGE        = -2;
const static int SIZE_LARGE       = -1;
const static int SIZE_MEDIUM      =  0;
const static int SIZE_SMALL       =  1;
const static int SIZE_TINY        =  2;
const static int SIZE_DIMINUTIVE  =  4;
const static int SIZE_FINE        =  8;

const static int SPEED_VERY_SLOW  = 5;
const static int SPEED_SLOW       = 4;
const static int SPEED_NORMAL     = 3;
const static int SPEED_FAST       = 2;
const static int SPEED_VERY_FAST  = 1;

const static int BASE_AC = 10;
enum EnemyTypes
{
  PLAYER = 0,
  EN_BAT,
  EN_RAT,
  EN_DIRE_RAT,
  EN_FERAL_HOUND,
  EN_CRYPT_WORM,
  EN_BARBED_BOAR,
//  EN_DIRE_BAT,
  EN_GOBLIN_MASTIFF,
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
    int base_energy;

    int unarmed_damage[2];

    int att_str;
    int att_dex;
    int att_int;
    int att_vit;

    int mod_speed;
    int mod_size;
    int mod_base_ac;

    int sight_range;
};

const static enemy_data_t enemy_db[] = {
//  {
//    {habmin, habmax},
//    "name", 'char', TCODColor::color,
//    health,   energy, damage
//    str,   dex,  int, vit,
//    speed  size, base armour,
//    sight
//  },
    {
        {-1, -1},
        "player", '@', TCODColor::white,
        15,   5, {1, 5},
        ATT_AVERAGE, ATT_AVERAGE, ATT_AVERAGE, ATT_AVERAGE,
        SPEED_NORMAL, SIZE_MEDIUM, BASE_AC,
        ATT_AVERAGE
    },
    {
        {0, 10},
        "rat", 'r', TCODColor::grey,
        2,   1, {1, 2},
        ATT_BAD, ATT_GOOD, ATT_BAD, ATT_AVERAGE,
        SPEED_FAST, SIZE_TINY, BASE_AC,
        ATT_AVERAGE
    },
    {
        {0, 10},
        "bat", 'b', TCODColor::grey,
        2,   1, {1, 2},
        ATT_MIN, ATT_GOOD, ATT_MIN, ATT_AVERAGE,
        SPEED_VERY_FAST, SIZE_DIMINUTIVE, BASE_AC,
        ATT_AVERAGE
    },
    {
        {0, 10},
        "dire rat", 'r', TCODColor::darkGrey,
        5,   1, {1, 4},
        ATT_AVERAGE, ATT_GOOD, ATT_MIN, ATT_AVERAGE,
        SPEED_VERY_FAST, SIZE_SMALL, BASE_AC + 1,
        ATT_AVERAGE
    },
    {
        {0, 10},
        "feral hound", 'd', TCODColor::desaturatedOrange,
        6,   1, {1, 4},
        ATT_ABOVE_AVERAGE, ATT_VERY_GOOD, ATT_VERY_BAD, ATT_GOOD,
        SPEED_FAST, SIZE_SMALL, BASE_AC + 2,
        ATT_AVERAGE
    },
    {
        {0, 10},
        "crypt worm", 'w', TCODColor::lightRed,
        34,   5, {2, 6},
        ATT_AVERAGE, ATT_GOOD, ATT_MIN, ATT_MAX,
        SPEED_VERY_SLOW, SIZE_MEDIUM, BASE_AC + 5,
        ATT_AVERAGE
    },
    {
        {0, 10},
        "barbed boar", 'B', TCODColor::darkerOrange,
        52,   0, {1, 8},
        ATT_MAX, ATT_AVERAGE, ATT_VERY_BAD, ATT_VERY_GOOD,
        SPEED_FAST, SIZE_MEDIUM, BASE_AC + 6,
        ATT_AVERAGE
    },
//   {
//        {0, 10},
//        "dire bat", 'b', TCODColor::grey,
//        30,   1, {1, 8},
//        ATT_VERY_GOOD, ATT_MAX, ATT_MIN, ATT_ABOVE_AVERAGE,
//        SPEED_FAST, SIZE_LARGE, BASE_AC + 5,
//        ATT_AVERAGE
//    },
    {
        {0, 10},
        "goblin mastiff", 'd', TCODColor::desaturatedYellow,
        30,   0, {1, 6},
        ATT_VERY_GOOD, ATT_ABOVE_AVERAGE, ATT_VERY_BAD, ATT_ABOVE_AVERAGE,
        SPEED_FAST, SIZE_MEDIUM, BASE_AC + 3,
        ATT_AVERAGE
    }
};
