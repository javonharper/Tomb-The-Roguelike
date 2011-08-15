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
  int att_vit_;

  //time keeping attributes
  bool turn_finished_;
  int next_turn_;

  //inventory attributes
  Inventory *inventory_;
  Item* active_weapon_;
  Item* active_body_armour_;

 public:
  Actor();
  ~Actor();
  void initProperties(enemy_data_t data, World *world);
  void move(int x, int y, int z);
  void moveTowards(int x, int y);
  void wander();
  void rest();
  void openDoor(int x, int y, int z);
  void closeDoor(int x, int y, int z);
  void descendStairs();
  void ascendStairs();
  void meleeAttack(Actor *actor);
  //    void castSpell(Spell *spell);
  void rangedAttack(Actor *actor);
  void dropItem(Item *item);
  void pickUpItem(Item *item);
  void useItem(Item *item);
  void wieldWeapon(Item *item);
  void wearItem(Item *item);
  void drinkPotion(Item *item);
  bool hasVictoryItem();
  bool hasEquipped(Item *item);

  void startTurn();
  void endTurn();
  void regenerateHealth();
  void regenerateEnergy();
  virtual void kill()=0;

  void FOV(int level);
  void doFov(Map *map, float x, float y);
  bool canSee(int level, int x, int y);
  virtual bool isEnemyAtPosition(int x, int y, int level)=0;

  //Calculators
  int calcArmourClass();
  int calcMeleeDamage();
  int calcDieRoll(int rolls, int die_sides);
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
};

#endif
