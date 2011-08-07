/*
 *  File:       inventory.h
 *  Summary:    container of the list of actor's items
 *  Written by: Javon Harper
 */
#ifndef INVENTORY_H
#define INVENTORY_H
#include <map>

class Item;
class Inventory
{
  private:
    std::map<char, Item*> item_map_;
  public:
    Inventory();
    ~Inventory();
    void add(Item *item);
    void remove(Item *item);
    void remove(char item);
    unsigned int size();
    Item *get(char slot);
    char findFreeSlot();
    std::map<char, Item*> getMap();
};
#endif


