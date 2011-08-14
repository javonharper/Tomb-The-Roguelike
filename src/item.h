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

class World;
class Item
{

  private:
    World *world_;
    int x_;
    int y_;
    int map_level_;
    bool on_ground_;

    std::string name_;
    int category_;
    int type_;
    int item_values_[NUM_ITEM_VALUES];

    char face_tile_;
    TCODColor color_;

  public:
    Item(World *world);
    Item(item_data_t item_data, World *world);
    Item(int category, World *world);
    ~Item();
    int chooseCategory();
    int chooseType(int category);
    void init(int category, int type);
    void init(item_data_t item_data);
    int getNumTypesFromCategory(int category);
    void setPosition(int x, int y, int level);
    void setFaceTile(int category);
    int getXPosition();
    int getYPosition();
    int getMapLevel();
    char getFaceTile();
    TCODColor getColor();
    std::string getName();
    bool isOnGround();
    void setOnGround(bool on_ground);
    void destroy();
    int getCategory();
    int getType();
    int getValue(int i);
};

#endif
