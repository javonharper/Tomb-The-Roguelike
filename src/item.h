/*
 *  File:       item.h
 *  Summary:    things the player can wear and weild
 *  Written by: Javon Harper
 */
#ifndef ITEM_H
#define ITEM_H

#include <string>
#include "libtcod.hpp"

const static int NUM_ITEM_VALUES = 5;

struct item_data_t
{
  std::string name;
  int category;
  int type;
  TCODColor color;
  int item_values[NUM_ITEM_VALUES];
};

class Item
{

  private:
    int x_;
    int y_;
    int map_level_;

    std::string name_;
    int category_;
    int type_;
    int item_values_[NUM_ITEM_VALUES];

    char face_tile_;
    TCODColor color_;

  public:
    Item();
    Item(int category);
    ~Item();
    int chooseCategory();
    int chooseType(int category);
    void init(int category, int type);
    void init(item_data_t item_data);
    int getNumTypesFromCategory(int category);
    void setPosition(int x, int y, int level);
    char getItemFaceTile(int category);
};

#endif
