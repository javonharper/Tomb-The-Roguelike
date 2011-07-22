/*
 *  File:      item_db.h
 *  Summary:    all of the items
 *  Written by: Javon Harper
 */

 struct item_data_t
{
  std::string name;
  int category;
  int type;

  int item_values[5];
};

const static int category_probabilities[] = {70, 30};

enum ItemCategories
{
  CATEGORY_WEAPON = 0,
  CATEGORY_BODY_ARMOUR,
  TOTAL_ITEM_CATEGORIES
};

enum WeaponTypes
{
  TYPE_WOODEN_DAGGER = 0,
  TYPE_METAL_SWORD,
  TOTAL_WEAPON_TYPES
};

enum BodyArmourTypes
{
  TYPE_LEATHER_ARMOUR = 0,
  TYPE_METAL_ARMOUR,
  TOTAL_BODY_ARMOUR_TYPES
};

const static item_data_t weapon_db[] =
{
  {"wooden dagger", CATEGORY_WEAPON, TYPE_WOODEN_DAGGER, {2, 3, 0, 0, 0}},
  {"metal sword", CATEGORY_WEAPON, TYPE_METAL_SWORD, {2, 5, 0, 0, 0}}
};

const static item_data_t armour_db[] =
{
  {"leather armour", CATEGORY_BODY_ARMOUR, TYPE_LEATHER_ARMOUR, {5, 0, 0, 0, 0}},
  {"metal armour", CATEGORY_BODY_ARMOUR, TYPE_METAL_ARMOUR, {10, 0, 0, 0, 0}}
};

