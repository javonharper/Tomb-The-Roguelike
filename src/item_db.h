/*
 *  File:      item_db.h
 *  Summary:    all of the items
 *  Written by: Javon Harper
 */
#include "libtcod.hpp"
#include "item.h"

enum ItemCategories
{
    CATEGORY_WEAPON = 0,
    CATEGORY_BODY_ARMOUR,
    CATEGORY_POTION,
    TOTAL_ITEM_CATEGORIES
};

enum WeaponTypes
{
    TYPE_DAGGER = 0,
    TYPE_WOODEN_CLUB,
    TYPE_QUARTERSTAFF,
    TYPE_BATTLEAXE,
    TYPE_LONGSWORD,
    TYPE_WARHAMMER,
    TYPE_SPEAR,
    TYPE_WIDOWMAKER,
    TYPE_BRASSKNUCKLES,
    TYPE_MAGICWAND,
    TYPE_HOLYSCEPTRE,
    TOTAL_WEAPON_TYPES
};

enum BodyArmourTypes
{
    TYPE_SILK_TUNIC = 0,
    TYPE_CLOTH_ROBE,
    TYPE_LEATHER_ARMOUR,
    TYPE_HIDE_ARMOUR,
    TYPE_RING_MAIL_ARMOUR,
    TYPE_BREAST_PLATE,
    TYPE_SPLINT_MAIL_ARMOUR,
    TYPE_HALF_PLATE_ARMOUR,
    TYPE_FULL_PLATE_ARMOUR,
    TOTAL_BODY_ARMOUR_TYPES
};

enum PotionTypes
{
    TYPE_CURE_LIGHT_WOUNDS = 0,
    TYPE_CURE_MODERATE_WOUNDS,
    TYPE_LIGHT_ENERGY_RESTORE,
    TYPE_MODERATE_ENERGY_RESTORE,
    TOTAL_POTION_TYPES
};

enum WeaponValues
{
    WPNV_ROLLS = 0,
    WPNV_DIE_SIDES
};

enum ArmourValues
{
    ARMV_AC = 0,
    ARMV_DEX_PEN
};

enum PotionValues
{
    PTNV_ROLLS = 0,
    PTNV_DIE_SIDES
};
const static item_data_t erroneous_item = {"item error", -1, -1, TCODColor::red, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}};

const static item_data_t weapon_db[] =
{
    {"dagger",        CATEGORY_WEAPON, TYPE_DAGGER,       TCODColor::grey,       {1, 4, 0, 0, 0}, {0, 0, 0, 0, 0}},
    {"wooden club",   CATEGORY_WEAPON, TYPE_WOODEN_CLUB,  TCODColor::orange,  {1, 6, 0, 0, 0}, {0, 0, 0, 0, 0}},
    {"quarterstaff",  CATEGORY_WEAPON, TYPE_QUARTERSTAFF, TCODColor::orange,      {1, 6, 0, 0, 0}, {0, 0, 0, 0, 0}},
    {"battle-axe",    CATEGORY_WEAPON, TYPE_BATTLEAXE,    TCODColor::lightOrange,  {1, 8, 0, 0, 0}, {0, 0, 0, 0, 0}},
    {"long sword",    CATEGORY_WEAPON, TYPE_LONGSWORD,    TCODColor::lightGrey,        {1, 8, 0, 0, 0}, {0, 0, 0, 0, 0}},
    {"warhammer",     CATEGORY_WEAPON, TYPE_WARHAMMER,    TCODColor::lightOrange,  {1, 8, 0, 0, 0}, {0, 0, 0, 0, 0}},
    {"spear",         CATEGORY_WEAPON, TYPE_SPEAR,        TCODColor::lightOrange,  {1, 8, 0, 0, 0}, {0, 0, 0, 0, 0}},

    //fighter weapons
    {"widowmaker",        CATEGORY_WEAPON, TYPE_WIDOWMAKER,          TCODColor::darkViolet, {1, 10, 0, 0, 0}, {0, 1, 0, 0, 0}},

    //rogue weapons
    {"brass knuckles",    CATEGORY_WEAPON, TYPE_BRASSKNUCKLES,    TCODColor::darkYellow, {1, 10, 0, 0, 0}, {0, 0, 1, 0, 0}},

    //magician weapons
    {"magic wand",        CATEGORY_WEAPON, TYPE_MAGICWAND,       TCODColor::amber,       {1, 10, 0, 0, 0}, {0, 0, 0, 0, 1}},

    //priest weapons
    {"holy sceptre",       CATEGORY_WEAPON, TYPE_HOLYSCEPTRE,    TCODColor::lightYellow, {1, 10, 0, 0, 0}, {0, 0, 0, 1, 0}},

};

const static item_data_t armour_db[] =
{
    {"silk tunic",          CATEGORY_BODY_ARMOUR, TYPE_SILK_TUNIC,          TCODColor::lightYellow,       {1, 0, 0, 0, 0}, {0, 0, 0, 0, 0}},
    {"cloth robe",          CATEGORY_BODY_ARMOUR, TYPE_CLOTH_ROBE,          TCODColor::lightBlue,         {1, 0, 0, 0, 0}, {0, 0, 0, 0, 0}},
    {"leather armour",      CATEGORY_BODY_ARMOUR, TYPE_LEATHER_ARMOUR,      TCODColor::darkOrange,        {2, 0, 0, 0, 0}, {0, 0, 0, 0, 0}},
    {"hide armour",         CATEGORY_BODY_ARMOUR, TYPE_HIDE_ARMOUR,         TCODColor::desaturatedYellow, {3, -3, 0, 0, 0}, {0, 0, 0, 0, 0}},
    {"ring mail armour",    CATEGORY_BODY_ARMOUR, TYPE_RING_MAIL_ARMOUR,    TCODColor::darkGrey,          {4, -4, 0, 0, 0}, {0, 0, 0, 0, 0}},
    {"breast plate",        CATEGORY_BODY_ARMOUR, TYPE_BREAST_PLATE,        TCODColor::grey,              {5, -5, 0, 0, 0}, {0, 0, 0, 0, 0}},
    {"splint mail armour",  CATEGORY_BODY_ARMOUR, TYPE_SPLINT_MAIL_ARMOUR,  TCODColor::grey,              {6, -6, 0, 0, 0}, {0, 0, 0, 0, 0}},
    {"half-plate armour",   CATEGORY_BODY_ARMOUR, TYPE_HALF_PLATE_ARMOUR,   TCODColor::lightGrey,         {7, -7, 0, 0, 0}, {0, 0, 0, 0, 0}},
    {"full plate armour",   CATEGORY_BODY_ARMOUR, TYPE_FULL_PLATE_ARMOUR,   TCODColor::white,             {8, -8, 0, 0, 0}, {0, 0, 0, 0, 0}}
};

const static item_data_t potion_db[] =
{
    {"potion of cure light wounds",       CATEGORY_POTION, TYPE_CURE_LIGHT_WOUNDS,    TCODColor::red,  {1, 8, 0, 0, 0}, {0, 0, 0, 0, 0}},
    {"potion of cure moderate wounds",    CATEGORY_POTION, TYPE_CURE_MODERATE_WOUNDS, TCODColor::red,  {2, 8, 0, 0, 0}, {0, 0, 0, 0, 0}},
    {"potion of light energy restore",    CATEGORY_POTION, TYPE_LIGHT_ENERGY_RESTORE, TCODColor::blue, {1, 8, 0, 0, 0}, {0, 0, 0, 0, 0}},
    {"potion of moderate energy restore", CATEGORY_POTION, TYPE_MODERATE_ENERGY_RESTORE,TCODColor::blue, {2, 8, 0, 0, 0}, {0, 0, 0, 0, 0}}
};

const static int CATEGORY_VICTORY_ITEM = -100;
const static int TYPE_VICTORY_ITEM = -100;
const static item_data_t victory_item = {"icon of the weedaula", CATEGORY_VICTORY_ITEM, TYPE_VICTORY_ITEM, TCODColor::red,  {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}};

