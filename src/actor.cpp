/*
 *  File:       actor.cpp
 *  Summary:    holds the general functionality of the player and his/her enemies.
 *  Written by: Javon Harper
 */
#include <cmath>
#include <iostream>
#include <sstream>
#include "actor.h"
#include "controller.h"
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

    //60 is the lcm of the different speed 1, 2, 3, 4, 5
    //actors with normal speed have a multiplier of 1
    //therefore, we 20 by the speed to set the multilpier to 1
    //with a speed of 3 and everything else follows
    damage_multiplier_ = 60/20/(double)(6 - data.mod_speed);
    //std::cout << damage_multiplier_ << std::endl;

    inventory_ = new Inventory();
    active_weapon_ = NULL;
    active_body_armour_ = NULL;

    current_health_points_ = max_health_points_ = data.base_health;
    current_energy_points_ = max_energy_points_ = data.base_energy;

    att_str_ = data.att_str;
    att_dex_ = data.att_dex;
    att_int_ = data.att_int;

    world_ = world;

    for (int z = 0; z < world->getLevels(); z++)
    {
	TCODMap *map = new TCODMap(world->getWidth(), world->getHeight());

	for (int y = 0; y < world->getHeight(); y++)
	{
	    for (int x = 0; x < world->getWidth(); x++)
	    {
		tile_t tile = world->getTile(x,y,z);
		map->setProperties(x, y, tile.is_passable, tile.is_passable);
	    }
	}

	vision_map_.push_back(map);
    }

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
    else if(world_->getTile(x, y, level).is_passable
	    && world_->getEnemyAt(x, y, level) == NULL)
    {
	this->setPosition(x, y, level);
	this->turn_finished_ = true;
    }
    //If a closed door is there, automatically open it and end the turn.
    else if(world_->getTile(x, y, level).tile_type == TILE_DOOR_CLOSED)//
    {
	this->openDoor(x, y, level);
    }
}

void Actor::moveTowards(int x, int y)
{
    TCODPath *path = new TCODPath(vision_map_[map_level_]);
    path->compute(x_, y_, x, y);
    if (! path->isEmpty())
    {
	int x,y;
	path->get(0, &x, &y);
	move(x, y, map_level_);
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
    tile_t tile = world_->getTile(this->getXPosition(), this->getYPosition(), this->getMapLevel());

    if(tile.tile_type == TILE_DOWNSTAIR)
    {
	world_->setCurrentLevel(world_->getCurrentLevel() + 1);
	position_t stair_pos = world_->getMapLevel(world_->getCurrentLevel())->getUpStairPos();
	int level = world_->getCurrentLevel();
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
	    if(!hasVictoryItem())
	    {
		char result = prompt("Are you sure you want to go to the surface? (y)es/(n)o");
		if (result == YES)
		{
		    displayGameOverScreen("You arrive at the surface embarrased and without the icon");
		}
		else
		{
		    message("Okay, then.");
		}
	    }
	    else
	    {
		displayWinScreen("Congratulations! You have braved the tomb!");
	    }
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

bool Actor::hasVictoryItem()
{
    std::map<char, Item*> item_map = inventory_->getMap();
    std::string slots("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");
    for (unsigned int i = 0; i < slots.size(); i++)
    {
	char slot = slots.at(i);
	Item* item = inventory_->get(slot);
	if (item != NULL)
	{
	    if (item->getCategory() == CATEGORY_VICTORY_ITEM)
	    {
		return true;
	    }
	}
    }
    return false;
}

//DND style melee attack.
void Actor::meleeAttack(Actor *actor)
{
    std::cout << "atk=" << name_ << ",def=" << actor->getName() << ",";
    int attack_roll = random(1, 20) + calcAtt(att_str_);
    int opponent_ac = actor->calcArmourClass();
    std::cout << "atkroll=" << attack_roll << ",defac=" << opponent_ac << ",";
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
	    damage_roll = setBoundedValue(damage_roll, 1, damage_roll);
	}

	if (critical_hit)
	{

	}

	std::cout << ",crit=" << critical_hit <<  ",rawdmg=" << damage_roll << ",mult=" << (double)damage_multiplier_;
	damage_roll = setBoundedValue(damage_roll * damage_multiplier_, 1, damage_roll * damage_multiplier_);
	std::cout << ",dmg=" << damage_roll << std::endl;
    }
    else
    {
	std::cout << "MISS" << std::endl;
    }


    //check if the actor is dead.
    if(damage_roll >= actor->getCurrentHealth())
    {
	actor->kill();
    }
    else
    {
	actor->setCurrentHealth(actor->getCurrentHealth() - damage_roll);
    }

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

char Actor::pickUpItem(Item *item)
{
    char slot = inventory_->add(item);
    item->setPosition(-1, -1, -1);
    item->setOnGround(false);
    turn_finished_ = true;
    return slot;
}

void Actor::useItem(Item *item)
{
    switch(item->getCategory())
    {
    case CATEGORY_WEAPON: wieldWeapon(item); break;
    case CATEGORY_BODY_ARMOUR: wearItem(item); break;
    case CATEGORY_POTION: drinkPotion(item); inventory_->remove(item); item->destroy(); break;
    case CATEGORY_VICTORY_ITEM: message("Bring this to the surface to win the game!"); break;
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

bool Actor::hasEquipped(Item *item)
{
    if(active_body_armour_ == item)
    {
	return true;
    }
    else if (active_weapon_ == item)
    {
	return true;
    }
    return false;
}

//Handles anything that needs to happen before taking a turn,
//including checking to see if it is actually the actor turn.
void Actor::startTurn()
{
    turn_finished_ = false;
    if (current_health_points_ < (double) 0.25 * max_health_points_)
    {
	    bleed();
    }

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

void Actor::bleed()
{
    world_->setTileColor(x_, y_, map_level_, TCODColor::desaturatedRed);
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
    if(att_int_ >= roll)
    {
	if(current_energy_points_ < max_energy_points_)
	{
	    current_energy_points_ = current_energy_points_ + 1;
	}
    }
}

int Actor::calcSpeed()
{
    return mod_speed_;
}

int Actor::calcSize()
{
    return mod_size_;
}

int Actor::calcSight()
{
    return sight_range_;
}

int Actor::calcAtt(int attribute)
{
    return (attribute - 10 )/5;
}

bool Actor::canSee(int level, int x, int y)
{
      return vision_map_[level]->isInFov(x,y);
}

void Actor::FOV(int level)
{
    vision_map_[level]->computeFov(x_, y_, calcSight());//FOV_DIAMOND
}

void Actor::setVisionProperties(int x, int y, int z, int transparent, int walkable)
{
    vision_map_[z]->setProperties(x, y, transparent, walkable);
}


//returns the sum of the base ac, armour(if any), dex modifier and size modifier
int Actor::calcArmourClass()
{
    int armour_bonus = 0;

    if (this->hasBodyArmour())
    {
	armour_bonus = active_body_armour_->getValue(0);

	//armour_bonus += active_shield_->getArmourBonus();
    }
    return mod_base_ac_ + armour_bonus + calcAtt(att_dex_) + calcSize();
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
    case ATT_INT: return att_int_;
    case ATT_VIT: return att_vit_;
    default: message("ERROR: malformed att_type"); return -1;
    }
}

//Accessors and Mutators
bool Actor::isTurnFinished(){return turn_finished_;}
bool Actor::isTurn(){return next_turn_ == world_->getTimeStep();}
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


