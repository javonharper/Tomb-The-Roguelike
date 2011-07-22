/*
 *  File:       actor.cpp
 *  Summary:    holds the general functionality of the player and his/her enemies.
 *  Written by: Javon Harper
 */
#include <cmath>
#include <iostream>
#include "actor.h"
#include "functions.h"
#include "interface.h"
#include "random.h"
#include "world.h"

Actor::Actor(){}
Actor::~Actor(){}

void Actor::initProperties(enemy_data_t data, World *world)
{
  this->x_ = -1;
  this->y_ = -1;
  this->map_level_ = -1;

  this->name_ = data.name;
  this->face_tile_ = data.face_tile;
  this->color_ = data.color;

  this->sight_range_ = data.sight_range;
  this->smell_range_ = data.smell_range;
  this->hear_range_ = data.hear_range;

  this->mod_speed_ = data.mod_speed;
  this->mod_size_ = data.mod_size;
  this->mod_base_ac_ = data.mod_base_ac;

  this->unarmed_damage_[0] = data.unarmed_damage[0];
  this->unarmed_damage_[1] = data.unarmed_damage[1];
  this->active_weapon_ = NULL;
  this->active_body_armour_ = NULL;

  this->current_health_points_ = this->max_health_points_ = data.base_health;
  this->current_mana_points_ = this->max_mana_points_ = data.base_health;

  this->att_str_ = data.att_str;
  this->att_dex_ = data.att_dex;
  this->att_wis_ = data.att_wis;

  this->world_ = world;
  this->turn_finished_ = true;
  this->next_turn_ = this->calcSenseMod(mod_speed_);
}

void Actor::move(int x, int y, int level)
{
  //In the case there is nothing there, move there.
  //is enemy at position method will delegate attacking the actor to the subclass.
  bool walking_into_enemy = this->isEnemyAtPosition(x, y, level);
  if(walking_into_enemy)
  {
    this->turn_finished_ = true;
  }
  //if nothing is there, go there and end the turn.
  else if(this->getWorld()->getTile(x, y, level).is_passable
    && world_->getEnemyAt(x, y, level) == NULL)
  {
    this->setPosition(x, y, level);
    this->turn_finished_ = true;
  }
  //If a closed door is there, automatically open it and end the turn.
  else if(this->getWorld()->getTile(x, y, level).tile_type == TILE_DOOR_CLOSED)//
  {
    this->openDoor(x, y, level);
  }
}

void Actor::moveTowards(int x, int y)
{
  int src_x = this->getXPosition();
  int src_y = this->getYPosition();
  int dest_x = x;
  int dest_y = y;
  int d_x = 0;
  int d_y = 0;

  //very basic and primitive pathfinding, move in the direction, no matter what.
  if(src_x < dest_x)
  {
    d_x = 1;
  }
  else if(src_x > dest_x)
  {
    d_x = -1;
  }

  if(src_y < dest_y)
  {
    d_y = 1;
  }
  else if(src_y > dest_y)
  {
    d_y = -1;
  }

  //small hack to make it turn corners and such. not perfect
  int new_x = this->getXPosition() + d_x;
  int new_y = this->getYPosition() + d_y;
  if (this->getWorld()->getTile(new_x, new_y, map_level_).is_passable)
  {
    this->move(new_x, new_y, this->getMapLevel());
  }
  else if(x - x_ > y - y_)
  {
    this->move(new_x - d_x, new_y, this->getMapLevel());
  }
  else if(x - x_ > y - y_)
  {
    this->move(new_x, new_y - d_y, this->getMapLevel());
  }
}

void Actor::wander()
{
  this->move(random(x_ - 1, x_ + 1), random(y_ - 1, y_ + 1), this->getMapLevel());
}

void Actor::rest()
{
  this->turn_finished_ = true;
}

//If the door at the coordinates is a closed door, open.
void Actor::openDoor(int x, int y, int z)
{
  if(this->getWorld()->getTile(x, y, z).tile_type == TILE_DOOR_CLOSED)
  {
    this->getWorld()->setTile(x, y, z, TILE_DOOR_OPEN);
    this->turn_finished_ = true;
  }
}

//if the door at the coordinates is an open door, close it
//UNTESTED
void Actor::closeDoor(int x, int y, int z)
{
  if(this->getWorld()->getTile(x, y, z).tile_type == TILE_DOOR_OPEN)
  {
    this->getWorld()->setTile(x, y, z, TILE_DOOR_CLOSED);
    this->turn_finished_ = true;
  }
}

//If the player is on top of a down stair, let him go to the next level and place him on the upward stair there.
void Actor::descendStairs()
{
  World *world = this->getWorld();
  tile_t tile = world->getTile(this->getXPosition(), this->getYPosition(), this->getMapLevel());

  if(tile.tile_type == TILE_DOWNSTAIR)
  {
    world->setCurrentLevel(world->getCurrentLevel() + 1);
    position_t stair_pos = world->getMapLevel(world->getCurrentLevel())->getUpStairPos();
    int level = world->getCurrentLevel();
    this->setPosition(stair_pos.x, stair_pos.y, level);
    this->turn_finished_ = true;
  }
}

//If the player is on top of an up stair, let him go to the previous level.
void Actor::ascendStairs()
{
  World *world = this->getWorld();
  tile_t tile = this->getWorld()->getTile(this->getXPosition(), this->getYPosition(), this->getMapLevel());
  if(tile.tile_type == TILE_UPSTAIR)
  {
    if (this->getMapLevel() == 0)
    {
      std::cout << "Should be asking player if they want to end the game." << std::endl;
      displayGameOverScreen("You have gone back to the surface.");
    }
    else
    {
      world->setCurrentLevel(world->getCurrentLevel() - 1);
      position_t stair_pos = world->getMapLevel(world->getCurrentLevel())->getDownStairPos();
      this->setPosition(stair_pos.x, stair_pos.y, world->getCurrentLevel());
      this->turn_finished_ = true;
    }
  }
}

//DND style melee attack.
void Actor::meleeAttack(Actor *actor)
{
  std::cout << "== Starting melee attack sequence. ==" << std::endl;
  int attack_roll = random(1, 20) + calcMod(att_str_);
  int opponent_ac = actor->calcArmourClass();
  std::cout <<"ATK roll:" << attack_roll << ", actor ac:" << opponent_ac << std::endl;

  int damage_roll = 0;
  if(attack_roll >= opponent_ac || attack_roll >= 20)
  {
    damage_roll = calcMeleeDamage() + calcMod(att_str_);
    damage_roll = setBoundedValue(damage_roll, 1, damage_roll);

    //if critical hit, roll again to see if actor can hit again.
    if(attack_roll >= 20 && random(1, 20) + calcMod(att_str_) >= opponent_ac)
    {
      damage_roll = damage_roll + calcMeleeDamage() + calcMod(att_str_);
    }
    std::cout << actor->getName() << " hit for " <<  damage_roll << " damage." << std::endl;
  }
  else
  {
    std::cout << this->getName() << " misses." << std::endl;
  }

  //check if the actor is dead.
  if(damage_roll >= actor->getCurrentHealth())
  {
    std::cout << actor->getName() << " killed." << std::endl;
    actor->setCurrentHealth(0);
    actor->kill();
  } else
  {
    actor->setCurrentHealth(actor->getCurrentHealth() - damage_roll);
    std::cout << actor->getName() << " has " << actor->getCurrentHealth() << " life left" << std::endl;
  }
  std::cout << "== Ending melee attack sequence. ==" << std::endl;
}

//void Actor::castSpell(Spell *spell){}
void Actor::rangedAttack(Actor *actor){}
void Actor::dropItem(Item *item){}
void Actor::pickUpItem(Item *item){}
void Actor::useItem(Item *item){}
void Actor::weildWeapon(Item *item){}
void Actor::wearItem(Item *item){}
void Actor::drinkPotion(Item *item){}
void Actor::readScroll(Item *item){}

//Handles anything that needs to happen before taking a turn,
//including checking to see if it is actually the actor turn.
void Actor::startTurn()
{
  this->turn_finished_ = false;
  if(!this->isTurn()){turn_finished_ = true;}
}

//Calculates the actors next turn.
void Actor::endTurn()
{
  //Calculate the actor's next turn.
  if(this->isTurn())
  {
    next_turn_ = this->getWorld()->getTimeStep() + this->calcSenseMod(mod_speed_);
  }
}

//returns true if the actor can see the position with a given map.
bool Actor::canSee(Map *map, int x, int y)
{
  return map->getTile(x, y).visible;
}

int Actor::calcMod(int att)
{return att;
  //return (att-10)/5;
}

int Actor::calcSenseMod(int sense)
{
  return ((sense/5) + 1) * 2;
}

//Sets the players field of vision.
//Adapted from elig's los psuedocode on rogueebasin
//http://roguebasin.roguelikedevelopment.org/index.php?title=Eligloscode
void Actor::FOV(Map *map)
{
  for(int i = 0; i < map->getWidth(); i++)
    for(int j = 0; j < map->getHeight(); j++)
    {
      map->setTileVisibility(i, j, false);
      //map->setTileVisibility(i, j, true); //View whole map by enabling this.
    }

  for (int i = 0; i < 360; i++)
  {
    float x = cos((float)i*0.01745f);
    float y = sin((float)i*0.01745f);
    doFov(map, x, y);
  }
}

//Calculates if the player can see in a direction
//Adapted from elig's los psuedocode on rogueebasin
//http://roguebasin.roguelikedevelopment.org/index.php?title=Eligloscode
//TODO: Fix sight for att values that are not 10
void Actor::doFov(Map *map, float x, float y)
{
  float ox = 0, oy = 0;
  ox = (float) this->getXPosition() + 0.5f;
  oy = (float) this->getYPosition() + 0.5f;

  for(int i = 0; i <= calcSenseMod(sight_range_); i++)
  {
    map->setTileVisibility((int) ox, (int) oy, true);
    map->setTileAsSeen((int) ox, (int) oy);
    if(!map->getTile((int) ox, (int) oy).is_passable)
    {
      return;
    }
    ox+=x;
    oy+=y;
  }
}

//returns the sum of the base sc, armour(if any), dex modifier and size modifier
int Actor::calcArmourClass()
{
  int armour_bonus = 0;

  if (this->hasBodyArmour())
  {
    //TODO: Should get the armour bonus from the item of with type armour
    //armour_bonus = active_body_armour_->getArmourBonus();
  }
//std::cout << this->mod_base_ac_ <<":"<< armour_bonus <<":"<< this->calcMod(this->att_dex_) <<":"<< this->calcMod(-mod_size_) <<":"<<std::endl;
  return this->mod_base_ac_ + armour_bonus + this->calcMod(att_dex_) + this->calcMod(-mod_size_);
//  return 100;
}

//Does a DnD-style die roll.
int Actor::calcMeleeDamage()
{
  int total_damage = 0;
  int rolls;
  int die_sides;

  if(this->hasWeapon())
  {
//    int* weapon_damage = this->getWeapon()->getDamage();
//    rolls = weapon_damage[0];
//    die_sides = weapon_damage[1];
  } else
  {
    rolls = unarmed_damage_[0];
    die_sides = unarmed_damage_[1];
  }

  for (int i = 0; i < rolls; i++)
  {
    total_damage = total_damage + random(1, die_sides);
  }
  return total_damage;
}
//Accessors and Mutators
bool Actor::isTurnFinished(){return turn_finished_;}
bool Actor::isTurn(){return next_turn_ == this->getWorld()->getTimeStep();}
int Actor::getXPosition(){return x_;}
int Actor::getYPosition(){return y_;}
int Actor::getMapLevel(){return map_level_;}
int Actor::getFaceTile(){return face_tile_;}
TCODColor Actor::getColor(){return color_;}
void Actor::setXPosition(int x){this->x_ = x;}
void Actor::setYPosition(int y){this->y_ = y;}
void Actor::setMapLevel(int z){this->map_level_ = z;}
void Actor::setPosition(int x, int y, int z){this->x_ = x; this->y_ = y; this->map_level_ = z;}
std::string Actor::getName(){return name_;}
World* Actor::getWorld(){return world_;}
int Actor::getCurrentHealth(){return current_health_points_;}
void Actor::setCurrentHealth(int health){current_health_points_ = health;}
bool Actor::hasWeapon(){return active_weapon_ != NULL;}
bool Actor::hasBodyArmour(){return active_body_armour_ != NULL;}
Item* Actor::getWeapon(){return active_weapon_;}
Item* Actor::getBodyArmour(){return active_body_armour_;}
