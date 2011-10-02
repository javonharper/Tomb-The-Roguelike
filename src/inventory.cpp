/*
 *  File:       inventory.cpp
 *  Summary:    container of the list of actor's items
 *  Written by: Javon Harper
 */
#include "inventory.h"
#include "item.h"
#include "interface.h"
#include <sstream>

Inventory::Inventory()
{
    for (unsigned int i = 0; i < inventory_slots.size(); i ++)
    {
        char new_slot = inventory_slots.at(i);
        item_map_[new_slot] = NULL;
    }
}

Inventory::~Inventory(){}

char Inventory::add(Item *item)
{
    char free_slot = findFreeSlot();
    item_map_[free_slot] = item;
    return free_slot;
}

void Inventory::remove(char slot)
{
    item_map_[slot] = NULL;
}

void Inventory::remove(Item *item)
{
    std::map<char,Item*>::iterator iter;
    for (iter = item_map_.begin(); iter != item_map_.end(); iter++)
    {
        if (iter->second == item)
        {
            item_map_[iter->first] = NULL;
        }
    }
}

unsigned int Inventory::size()
{
    return item_map_.size();
}

Item* Inventory::get(char slot)
{
    std::map<char,Item*>::iterator iter;
    for (iter = item_map_.begin(); iter != item_map_.end(); iter++)
    {
        if (iter->first == slot)
        {
            return iter->second;
        }
    }
    return NULL;
}

char Inventory::findFreeSlot()
{
    for (unsigned int i = 0; i < inventory_slots.size(); i ++)
    {
        char slot = inventory_slots.at(i);
        if (get(slot) == NULL)
        {
            return slot;
        }
    }
    return '?';
}

std::map<char, Item*> Inventory::getMap()
{
    return item_map_;
}

