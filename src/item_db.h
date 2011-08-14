/*
 *  File:      item_db.h
 *  Summary:    all of the items
 *  Written by: Javon Harper
 */
#include "libtcod.hpp"
#include "item.h"
//const static int category_probabilities[] = {50, 40};

enum ItemCategories
{
  CATEGORY_WEAPON = 0,
  CATEGORY_BODY_ARMOUR,
  CATEGORY_POTION,
  TOTAL_ITEM_CATEGORIES
};

enum WeaponTypes
{
  TYPE_WOODEN_CLUB = 0,
  TYPE_DAGGER,
  TYPE_BATTLEAXE,
  TYPE_LONGSWORD,
  TYPE_WARHAMMER,
  TYPE_QUARTERSTAFF,
  TYPE_SPEAR,
  TOTAL_WEAPON_TYPES
};

enum BodyArmourTypes
{
  TYPE_SILK_TUNIC = 0,
  TYPE_CLOTH_ROBE,
  TYPE_HARDLEATHER_ARMOUR,
  TYPE_RING_MAIL_ARMOUR,
  TYPE_SPLINT_MAIL_ARMOUR,
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

const static item_data_t erroneous_item = {"item error", -1, -1, TCODColor::red, {0, 0, 0, 0, 0}};

const static item_data_t weapon_db[] =
{
//TODO values should be set right
  {"wooden club",   CATEGORY_WEAPON, TYPE_WOODEN_CLUB,  TCODColor::darkOrange,  {1, 6, 0, 0, 0}},
  {"dagger",        CATEGORY_WEAPON, TYPE_DAGGER,       TCODColor::white,       {1, 4, 0, 0, 0}},
  {"battle-axe",    CATEGORY_WEAPON, TYPE_BATTLEAXE,    TCODColor::darkOrange,  {1, 8, 0, 0, 0}},
  {"long sword",    CATEGORY_WEAPON, TYPE_LONGSWORD,    TCODColor::grey,        {1, 8, 0, 0, 0}},
  {"warhammer",     CATEGORY_WEAPON, TYPE_WARHAMMER,    TCODColor::darkYellow,  {1, 8, 0, 0, 0}},
  {"quarterstaff",  CATEGORY_WEAPON, TYPE_QUARTERSTAFF, TCODColor::orange,      {1, 6, 0, 0, 0}},
  {"spear",         CATEGORY_WEAPON, TYPE_SPEAR,        TCODColor::darkOrange,  {1, 8, 0, 0, 0}}
};

const static item_data_t armour_db[] =
{
  {"silk tunic",          CATEGORY_BODY_ARMOUR, TYPE_SILK_TUNIC,          TCODColor::lightYellow, {1, 0, 0, 0, 0}},
  {"cloth robe",          CATEGORY_BODY_ARMOUR, TYPE_CLOTH_ROBE,          TCODColor::lightBlue,   {1, 0, 0, 0, 0}},
  {"hardleather vest",    CATEGORY_BODY_ARMOUR, TYPE_HARDLEATHER_ARMOUR,  TCODColor::darkOrange,  {3, 0, 0, 0, 0}},
  {"ring mail armour",    CATEGORY_BODY_ARMOUR, TYPE_RING_MAIL_ARMOUR,    TCODColor::darkGrey,    {4, 0, 0, 0, 0}},
  {"splint mail armour",  CATEGORY_BODY_ARMOUR, TYPE_SPLINT_MAIL_ARMOUR,  TCODColor::lightGrey,   {6, 0, 0, 0, 0}},
  {"full plate armour",   CATEGORY_BODY_ARMOUR, TYPE_FULL_PLATE_ARMOUR,   TCODColor::white,       {8, 0, 0, 0, 0}}
};

const static item_data_t potion_db[] =
{
  {"potion of cure light wounds",       CATEGORY_POTION, TYPE_CURE_LIGHT_WOUNDS,    TCODColor::red,  {1, 8, 0, 0, 0}},
  {"potion of cure moderate wounds",    CATEGORY_POTION, TYPE_CURE_MODERATE_WOUNDS, TCODColor::red,  {2, 8, 0, 0, 0}},
  {"potion of light energy restore",    CATEGORY_POTION, TYPE_LIGHT_ENERGY_RESTORE, TCODColor::blue, {1, 8, 0, 0, 0}},
  {"potion of moderate energy restore", CATEGORY_POTION, TYPE_MODERATE_ENERGY_RESTORE,TCODColor::blue, {2, 8, 0, 0, 0}}
};

const static int CATEGORY_VICTORY_ITEM = -100;
const static int TYPE_VICTORY_ITEM = -100;
const static item_data_t victory_item = {"icon of the weedaula", CATEGORY_VICTORY_ITEM, TYPE_VICTORY_ITEM, TCODColor::red,  {0, 0, 0, 0, 0}};

