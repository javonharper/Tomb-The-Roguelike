/*
 *  File:       actor.h
 *  Summary:    holds the general functionality of the player and his/her enemies.
 *  Written by: Javon Harper
 */
#ifndef ACTOR_H
#define ACTOR_H

#include <string>
#include <vector>
#include "libtcod.hpp"
#include "enemy_db.h"

class World;
class Item;
class Inventory;
class Weapon;
class Armour;
class Potion;
class Scroll;
class Spell;
class Map;

enum EquippedSlots
{
    ACTIVE_WEAPON_SLOT = 0,
    ACTIVE_ARMOUR_SLOT,
    NUM_EQUIPPED_SLOTS
};

class Actor
{
protected:
    //location attributes
    World *world_;
    std::vector<TCODMap*> vision_map_;

    int x_;
    int y_;
    int map_level_;

    //personal attributes
    std::string name_;
    int face_tile_;
    TCODColor color_;

    //sense attributes
    int sight_range_;

    //combat attributes
    int unarmed_damage_[2];
    double damage_multiplier_;
    int mod_speed_;
    int mod_size_;
    int mod_base_ac_;

    //status attributes
    int current_health_points_;
    int max_health_points_;
    int current_energy_points_;
    int max_energy_points_;

    //RPG attributes
    int att_str_;
    int att_dex_;
    int att_int_;
    int att_wis_;
    int att_vit_;

    //time keeping attributes
    bool turn_finished_;
    int next_turn_;

    //inventory attributes
    Inventory *inventory_;
    Item* equipped_items_[NUM_EQUIPPED_SLOTS];

public:
    Actor();
    ~Actor();
    void initProperties(enemy_data_t data, World *world);
    void initVisionMap();
    void move(int x, int y, int level);
    void moveTowards(int x, int y);
    void wander();
    void rest();
    void openDoor(int x, int y, int z);
    void closeDoor(int x, int y, int z);
    void descendStairs();
    void ascendStairs();
    void meleeAttack(Actor *actor);
    void dropItem(Item *item);
    char pickUpItem(Item *item);
    void useItem(Item *item);
    void wieldWeapon(Item *item);
    void wearItem(Item *item);
    void drinkPotion(Item *item);
    bool hasVictoryItem();
    bool hasEquipped(Item *item);

    void setBodyArmour(Item *item);
    void setWeapon(Item *item);

    void startTurn();
    void endTurn();
    void regenerateHealth();
    void regenerateEnergy();
    virtual void kill()=0;
    void bleed();
    void loseHealth(int points);
    void gainHealth(int points);
    void loseEnergy(int points);
    void gainEnergy(int points);

    void FOV(int level);
    bool canSee(int level, int x, int y);
    virtual bool isEnemyAtPosition(int x, int y, int level)=0;

    //Calculators
    int calcArmourClass();
    int calcMeleeDamage();
    int calcAtt(int attribute);
    int calcSight();
    int calcSmell();
    int calcHear();
    int calcSpeed();;
    int calcSize();
    int calcAC();

    //Accessors and Mutators
    int getXPosition();
    int getYPosition();
    int getMapLevel();
    int getFaceTile();
    TCODColor getColor();
    bool isTurnFinished();
    bool isTurn();
    void setXPosition(int x);
    void setYPosition(int y);
    void setMapLevel(int z);
    void setPosition(int x, int y, int z);
    std::string getName();
    int getCurrentHealth();
    void setCurrentHealth(int  health);
    int getMaxHealth();
    int getCurrentEnergy();
    void setCurrentEnergy(int  energy);
    int getMaxEnergy();
    bool hasBodyArmour();
    bool hasWeapon();
    Inventory *getInventory();
    Item *getWeapon();
    Item *getBodyArmour();
    int getAttribute(int att_type);
    void setVisionProperties(int x, int y, int z, int transparent, int walkable);
    bool getIsPlayer();
};

#endif
