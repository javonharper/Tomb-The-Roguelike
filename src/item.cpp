/*
 *  File:       item.cpp
 *  Summary:    things the player can wear and weild
 *  Written by: Javon Harper
 */
#include "item.h"
#include "item_db.h"
#include "world.h"
#include "random.h"

//Create an item
Item::Item(World *world)
{
  world_ = world;
  int category = chooseCategory();
  int type = chooseType(category);
  init(category, type);
  setPosition(-1, -1, -1);
  setOnGround(true);
}

//Create an item within a given category
Item::Item(int category, World *world)
{
  world_ = world;
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
    case CATEGORY_WEAPON: return random(0, TOTAL_WEAPON_TYPES - 1); break;
    case CATEGORY_BODY_ARMOUR: return random(0, TOTAL_BODY_ARMOUR_TYPES - 1); break;
    case CATEGORY_POTION: return random(0, TOTAL_POTION_TYPES - 1); break;
    default: return -1; break;
  }
}

void Item::init(int category, int type)
{
   switch (category)
  {
    case CATEGORY_WEAPON: init(weapon_db[type]); break;
    case CATEGORY_BODY_ARMOUR: init(armour_db[type]); break;
    case CATEGORY_POTION: init(potion_db[type]); break;
    default: init(erroneous_item);
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
    case CATEGORY_WEAPON: face_tile_ = '/'; break;
    case CATEGORY_BODY_ARMOUR: face_tile_ = ']'; break;
    case CATEGORY_POTION: face_tile_ = '!'; break;
    default: face_tile_ = '~'; break;
  }
}

void Item::destroy()
{
  std::vector<Item*> &item_list = world_->getItemList();

  for(unsigned int i = 0; i < item_list.size(); i++)
    {
      if (this == item_list[i])
	{
	  item_list.erase(item_list.begin() + i);
	}
    }
}

int Item::getXPosition(){return x_;}
int Item::getYPosition(){return y_;}
int Item::getMapLevel(){return map_level_;}
char Item::getFaceTile(){return face_tile_;}
TCODColor Item::getColor(){return color_;}
std::string Item::getName(){return name_;}
bool Item::isOnGround(){return on_ground_;}
void Item::setOnGround(bool on_ground){on_ground_ = on_ground;}
int Item::getCategory(){return category_;}
int Item::getValue(int i){return item_values_[i];}
int Item::getType(){return type_;}

