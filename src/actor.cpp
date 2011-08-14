/*
 *  File:       actor.cpp
 *  Summary:    holds the general functionality of the player and his/her enemies.
 *  Written by: Javon Harper
 */
#include <cmath>
#include <iostream>
#include <sstream>
#include "actor.h"
#include "functions.h"
#include "interface.h"
#include "item.h"
#include "item_db.h"
#include "inventory.h"
#include "random.h"
#include "world.h"

Actor::Actor(){}
Actor::~Actor(){}

void Actor::initProperties(enemy_data_t data, World *world)
{
  x_ = -1;
  y_ = -1;
  map_level_ = -1;

  name_ = data.name;
  face_tile_ = data.face_tile;
  color_ = data.color;

  sight_range_ = data.sight_range;

  mod_speed_ = data.mod_speed;
  mod_size_ = data.mod_size;
  mod_base_ac_ = data.mod_base_ac;

  unarmed_damage_[0] = data.unarmed_damage[0];
  unarmed_damage_[1] = data.unarmed_damage[1];

  inventory_ = new Inventory();
  active_weapon_ = NULL;
  active_body_armour_ = NULL;

  current_health_points_ = max_health_points_ = data.base_health;
  current_energy_points_ = max_energy_points_ = data.base_energy;

  att_str_ = data.att_str;
  att_dex_ = data.att_dex;
  att_wis_ = data.att_wis;

  world_ = world;
  turn_finished_ = true;
  next_turn_ = calcSpeed();
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

  //small hack to make it turn corners and such. not perfect. actually, it barely even works
  int new_x = getXPosition() + d_x;
  int new_y = getYPosition() + d_y;
  if (world_->getTile(new_x, new_y, map_level_).is_passable)
  {
    move(new_x, new_y, map_level_);
  }
  else if(x - x_ > y - y_)
  {
    move(new_x - d_x, new_y, map_level_);
  }
  else if(x - x_ > y - y_)
  {
    move(new_x, new_y - d_y, map_level_);
  }
}

void Actor::wander()
{
  move(random(x_ - 1, x_ + 1), random(y_ - 1, y_ + 1), map_level_);
}

void Actor::rest()
{
  turn_finished_ = true;
}

//If the door at the coordinates is a closed door, open.
void Actor::openDoor(int x, int y, int z)
{
  if(world_->getTile(x, y, z).tile_type == TILE_DOOR_CLOSED)
  {
    world_->setTile(x, y, z, TILE_DOOR_OPEN);
    turn_finished_ = true;
  }
}

//if the door at the coordinates is an open door, close it
void Actor::closeDoor(int x, int y, int z)
{
  if(world_->getTile(x, y, z).tile_type == TILE_DOOR_OPEN)
  {
    world_->setTile(x, y, z, TILE_DOOR_CLOSED);
    turn_finished_ = true;
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
  tile_t tile = world_->getTile(x_, y_, map_level_);
  if(tile.tile_type == TILE_UPSTAIR)
  {
    if (map_level_ == 0)
    {
      std::cout << "Should be asking player if they want to end the game." << std::endl;
      displayGameOverScreen("You have gone back to the surface.");
    }
    else
    {
      world_->setCurrentLevel(world_->getCurrentLevel() - 1);
      position_t stair_pos = world_->getMapLevel(world_->getCurrentLevel())->getDownStairPos();
      setPosition(stair_pos.x, stair_pos.y, world_->getCurrentLevel());
      turn_finished_ = true;
    }
  }
}

//DND style melee attack.
void Actor::meleeAttack(Actor *actor)
{
  std::stringstream message_stream;
  std::cout << "== Melee Attack:"<< getName() << "-->"<< actor->getName() << " == "<< std::endl;
  int attack_roll = random(1, 20) + calcAtt(att_str_);
  int opponent_ac = actor->calcArmourClass();
  std::cout <<"ATK roll:" << attack_roll << ",  AC:" << opponent_ac << std::endl;

  int damage_roll = 0;
  if(attack_roll >= opponent_ac || attack_roll >= 20)
  {
    damage_roll = calcMeleeDamage() + calcAtt(att_str_);
    damage_roll = setBoundedValue(damage_roll, 1, damage_roll);

    //if critical hit, roll again to see if actor can hit again.
    bool critical_hit = false;
    if(attack_roll >= 20 && random(1, 20) + calcAtt(att_str_) >= opponent_ac)
    {
      critical_hit = true;
      damage_roll = damage_roll + calcMeleeDamage() + calcAtt(att_str_);
    }
    message_stream << actor->getName() << " hit for " <<  damage_roll << " damage";
    if (critical_hit)
      {
	      message_stream << " critically";
      }
    message_stream << "." <<std::endl;
  }
  else
  {
    message_stream << getName() << " misses.";
  }
  
  std::cout << message_stream.str() << std::endl;

  //check if the actor is dead.
  if(damage_roll >= actor->getCurrentHealth())
  {
    std::cout << actor->getName() << " killed." << std::endl;
    message_stream << getName() << actor->getName() << " killed.";
    actor->kill();
  } else
  {
    actor->setCurrentHealth(actor->getCurrentHealth() - damage_roll);
    std::cout << actor->getName() << " has " << actor->getCurrentHealth() << " life left" << std::endl;
  }
  std::cout << "== Ending melee attack sequence. ==" << std::endl;
  
  message(message_stream.str());
  turn_finished_ = true;
}

void Actor::rangedAttack(Actor *actor){}

void Actor::dropItem(Item *item)
{
  inventory_->remove(item);

  //unequip if it is being worn
  if (item == active_body_armour_)
  {
    active_body_armour_ = NULL;
  }
  else if(item == active_weapon_)
  {
    active_weapon_ = NULL;
  }

  item->setPosition(x_, y_, map_level_);
  item->setOnGround(true);
  turn_finished_ = true;
}

void Actor::pickUpItem(Item *item)
{
  inventory_->add(item);
  item->setPosition(-1, -1, -1);
  item->setOnGround(false);
  turn_finished_ = true;
}

void Actor::useItem(Item *item)
{
  switch(item->getCategory())
  {
    case CATEGORY_WEAPON: wieldWeapon(item); break;
    case CATEGORY_BODY_ARMOUR: wearItem(item); break;
    case CATEGORY_POTION: drinkPotion(item); inventory_->remove(item); item->destroy(); break;
    default: message("ERROR: Malformed item category"); break;
  }
}

void Actor::wieldWeapon(Item *item)
{
  active_weapon_ = item;
  turn_finished_ = true;
}

void Actor::wearItem(Item *item)
{

  switch(item->getCategory())
  {
    case CATEGORY_BODY_ARMOUR: active_body_armour_ = item; turn_finished_ = true; break;
    default: message("ERROR: Something went wrong trying to wear an item");
  }
  turn_finished_ = true;
}

void Actor::drinkPotion(Item *item)
{
  switch(item->getType())
  {
    case TYPE_CURE_LIGHT_WOUNDS: current_health_points_ = setBoundedValue(current_health_points_ + calcDieRoll(item->getValue(0), item->getValue(1)), 0, max_health_points_); break;
    case TYPE_CURE_MODERATE_WOUNDS: current_health_points_ = setBoundedValue(current_health_points_ + calcDieRoll(item->getValue(0), item->getValue(1)), 0, max_health_points_); break;
    case TYPE_LIGHT_ENERGY_RESTORE: current_energy_points_ = setBoundedValue(current_energy_points_ + calcDieRoll(item->getValue(0), item->getValue(1)), 0, max_energy_points_); break;
    case TYPE_MODERATE_ENERGY_RESTORE:current_energy_points_ = setBoundedValue(current_energy_points_ + calcDieRoll(item->getValue(0), item->getValue(1)), 0, max_energy_points_); break;
    default: message("ERROR actor tried to quaff potion with bad type");break;
  }
  turn_finished_ = true;
}

//Handles anything that needs to happen before taking a turn,
//including checking to see if it is actually the actor turn.
void Actor::startTurn()
{
  turn_finished_ = false;
  if(!isTurn()){turn_finished_ = true;}
}

//Calculates the actors next turn.
void Actor::endTurn()
{
  //Calculate the actor's next turn.
  if(isTurn())
  {
    regenerateHealth();
    regenerateEnergy();
    next_turn_ = world_->getTimeStep() + calcSpeed();
  }
}

void Actor::regenerateHealth()
{
  int roll = random(1, 20);
  if(att_vit_ >= roll)
  {
    if(current_health_points_ < max_health_points_)
    {
      current_health_points_ = current_health_points_ + 1;
    }
  }
}

void Actor::regenerateEnergy()
{
  int roll = random(1, 20);
  if(att_wis_ >= roll)
  {
    if(current_energy_points_ < max_energy_points_)
    {
      current_energy_points_ = current_energy_points_ + 1;
    }
  }
}

//returns true if the actor can see the position with a given map.
bool Actor::canSee(Map *map, int x, int y)
{
  return map->getTile(x, y).visible;
}

int Actor::calcSpeed()
{
  return ((mod_speed_ / 5) + 1);
}

int Actor::calcSize()
{
  return (mod_size_ - 10) * (1 - (1/5));
}

int Actor::calcSight()
{
  return sight_range_;
}

int Actor::calcAtt(int attribute)
{
  return (attribute - 10 )/5;
}

//Sets the actors field of vision.
//Adapted from elig's los psuedocode on rogueebasin
//http://roguebasin.roguelikedevelopment.org/index.php?title=Eligloscode
void Actor::FOV(Map *map)
{
  for(int i = 0; i < map->getWidth(); i++)
    for(int j = 0; j < map->getHeight(); j++)
    {
      //map->setTileVisibility(i, j, false);
      map->setTileVisibility(i, j, true); //View whole map by enabling this.
    }

  for (int i = 0; i < 360; i++)
  {
    float x = cos((float)i*0.01745f);
    float y = sin((float)i*0.01745f);
    doFov(map, x, y);
  }
}

//Calculates if the actor can see in a direction
//Adapted from elig's los psuedocode on rogueebasin
//http://roguebasin.roguelikedevelopment.org/index.php?title=Eligloscode
//TODO: Fix sight for att values that are not 10
void Actor::doFov(Map *map, float x, float y)
{
  float ox = 0, oy = 0;
  ox = (float) this->getXPosition() + 0.5f;
  oy = (float) this->getYPosition() + 0.5f;

  for(int i = 0; i <= calcSight(); i++)
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
    armour_bonus = active_body_armour_->getValue(0);
    //armour_bonus += active_shield_->getArmourBonus();
  }
//std::cout << this->mod_base_ac_ <<":"<< armour_bonus <<":"<< this->calcMod(this->att_dex_) <<":"<< this->calcMod(-mod_size_) <<":"<<std::endl;
  return mod_base_ac_ + armour_bonus + calcAtt(att_dex_) + calcSize();
//  return 100;
}

//Does a DnD-style die roll.
int Actor::calcMeleeDamage()
{
  int rolls;
  int die_sides;

  if(this->hasWeapon())
  {
    rolls = active_weapon_->getValue(0);
    die_sides = active_weapon_->getValue(1);
  } 
  else
  {
    rolls = unarmed_damage_[0];
    die_sides = unarmed_damage_[1];
  }
  return calcDieRoll(rolls, die_sides);
}

int Actor::calcDieRoll(int rolls, int die_sides)
{
  int total = 0;
  for (int i = 0; i < rolls; i++)
  {
    total = total + random(1, die_sides);
  }
  return total;
}

int Actor::getAttribute(int att_type)
{
  switch(att_type)
  {
    case ATT_STR: return att_str_;
    case ATT_DEX: return att_dex_;
    case ATT_WIS: return att_wis_;
    case ATT_VIT: return att_vit_;
    default: message("ERROR: malformed att_type"); return -1;
  }
}

//Accessors and Mutators
bool Actor::isTurnFinished(){return turn_finished_;}
bool Actor::isTurn(){return next_turn_ == this->getWorld()->getTimeStep();}
int Actor::getXPosition(){return x_;}
int Actor::getYPosition(){return y_;}
int Actor::getMapLevel(){return map_level_;}
int Actor::getFaceTile(){return face_tile_;}
TCODColor Actor::getColor(){return color_;}//XXX should not explicity get TCODColor type, struct rather
void Actor::setXPosition(int x){this->x_ = x;}
void Actor::setYPosition(int y){this->y_ = y;}
void Actor::setMapLevel(int z){this->map_level_ = z;}
void Actor::setPosition(int x, int y, int z){this->x_ = x; this->y_ = y; this->map_level_ = z;}
std::string Actor::getName(){return name_;}
World* Actor::getWorld(){return world_;}
int Actor::getCurrentHealth(){return current_health_points_;}
void Actor::setCurrentHealth(int health){current_health_points_ = health;}
int Actor::getMaxHealth(){return max_health_points_;}
int Actor::getCurrentEnergy(){return current_energy_points_;}
void Actor::setCurrentEnergy(int energy){current_energy_points_ = energy;}
int Actor::getMaxEnergy(){return max_energy_points_;}
bool Actor::hasWeapon(){return active_weapon_ != NULL;}
bool Actor::hasBodyArmour(){return active_body_armour_ != NULL;}
Inventory* Actor::getInventory(){return inventory_;}
Item* Actor::getWeapon(){return active_weapon_;}
Item* Actor::getBodyArmour(){return active_body_armour_;}


