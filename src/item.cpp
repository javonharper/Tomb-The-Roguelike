#include "item.h"


//Create an item
Item::Item()
{
  //will call choose Cat to find category,
  //then choose type to find type
}

//Create an item within a given category
Item::Item(int category)
{
  //will only call choose type
}

//Item::Item(item_data_t item_data)
//{
//  //makes the item directly from the data type object/
//}

Item::~Item()
{
}

int Item::chooseCategory()
{
  //TODO fix
  return 0;
}

int Item::chooseType(int category)
{
  return 0;
}

void Item::setPosition(int x, int y, int level)
{
  this->x_ = x;
  this->y_ = y;
  this->map_level_ = level;
}

