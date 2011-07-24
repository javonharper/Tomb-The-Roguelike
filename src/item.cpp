/*
 *  File:       item.cpp
 *  Summary:    things the player can wear and weild
 *  Written by: Javon Harper
 */
#include "item.h"
#include "item_db.h"
#include "random.h"

//Create an item
Item::Item()
{
  int category = chooseCategory();
  int type = chooseType(category);
  init(category, type);
}

//Create an item within a given category
Item::Item(int category)
{
  int type = chooseType(category);
  init(category, type);
}

Item::~Item()
{
}

//TODO fix so this chooses by weights (category_probabilities)
int Item::chooseCategory()
{
  return random(0, TOTAL_ITEM_CATEGORIES - 1);
}

//TODO fix so this chooses by weights (type probs for each type)
int Item::chooseType(int category)
{
  switch (category)
  {
    case CATEGORY_WEAPON: return random(0, TOTAL_WEAPON_TYPES - 1);
    case CATEGORY_BODY_ARMOUR: return random(0, TOTAL_BODY_ARMOUR_TYPES - 1);
    default: return -1;
  }
}

void Item::init(int category, int type)
{
   switch (category)
  {
    case CATEGORY_WEAPON: init(weapon_db[type]);
    case CATEGORY_BODY_ARMOUR: init(armour_db[type]);
  }
}

void Item::init(item_data_t item_data)
{
    name_ = item_data.name;
    category_ = item_data.category;
    type_ = item_data.type;

    for (int i = 0; i < NUM_ITEM_VALUES; i++)
    {
      item_values_[i] = item_data.item_values[i];
    }

    setFaceTile(item_data.category);
    color_ = item_data.color;
}


void Item::setPosition(int x, int y, int level)
{
  this->x_ = x;
  this->y_ = y;
  this->map_level_ = level;
}

void Item::setFaceTile(int category)
{
  switch(category)
  {
    case CATEGORY_WEAPON: face_tile_ = '/';
    case CATEGORY_BODY_ARMOUR: face_tile_ = ']';
    default: face_tile_ = -1;
  }
}

int Item::getXPosition(){return x_;}
int Item::getYPosition(){return y_;}
char Item::getFaceTile(){return face_tile_;}
TCODColor Item::getColor(){return color_;}

