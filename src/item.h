#ifndef ITEM_H
#define ITEM_H

#include <string>
#include "libtcod.hpp"

class Item
{
  private:
    int x_;
    int y_;
    int map_level_;

    std::string name_;
    int category_;
    int type_;

    char face_tile_;
    TCODColor color_;

  public:
    Item();
    Item(int category);
//    Item(item_data_t item_data);
    ~Item();
    int chooseCategory();
    int chooseType(int category);
    void setPosition(int x, int y, int level);

};

#endif
