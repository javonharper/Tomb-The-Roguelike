/*
 *  File:       player.cpp
 *  Summary:    Our hero
 *  Written by: Javon Harper
 */
#include <cmath>
#include <iostream>
#include <sstream>
#include "controller.h"
#include "enemy.h"
#include "functions.h"
#include "interface.h"
#include "inventory.h"
#include "item_db.h"
#include "map.h"
#include "random.h"
#include "world.h"
#include "player.h"

Player::Player(World *world)
{
    initProperties(enemy_db[PLAYER], world);
    level_ = 1;
    experience_ = 0;
    is_alive_ = true;
    //name_ = std::string((char*)TCOD_namegen_generate("player", false));
    //TCODNamegen::destroy();
    att_str_ = random(ATT_AVERAGE, ATT_GOOD);
    att_int_ = random(ATT_AVERAGE, ATT_GOOD);
    att_dex_ = random(ATT_AVERAGE, ATT_GOOD);
    att_vit_ = random(ATT_AVERAGE, ATT_GOOD);

    max_health_points_ = current_health_points_ = att_vit_ + random(1, 10);
    max_energy_points_ = current_energy_points_ = att_int_ + random(1, 10);

    active_weapon_ = new Item(weapon_db[TYPE_WOODEN_CLUB], world);
    active_body_armour_ = new Item(armour_db[ TYPE_SILK_TUNIC], world);
    inventory_->add(active_weapon_);
    inventory_->add(active_body_armour_);
    inventory_->add(new Item(potion_db[TYPE_CURE_LIGHT_WOUNDS], world));
    inventory_->add(new Item(potion_db[TYPE_CURE_LIGHT_WOUNDS], world));
    inventory_->add(new Item(potion_db[TYPE_CURE_LIGHT_WOUNDS], world));
}

Player::~Player()
{
}

bool Player::isEnemyAtPosition(int x, int y, int level)
{
    bool found_enemy = false;
    Enemy *enemy = world_->getEnemyAt(x, y, level);
    if(enemy != NULL)
    {
	    found_enemy = true;
	    meleeAttack(enemy);
      if(enemy->getCurrentHealth() == 0)
      {
        addExp(enemy->getExpReward());
      }
    }
    return found_enemy;
}

void Player::kill()
{is_alive_ = false; setCurrentHealth(0);}

bool Player::isAlive()
{return is_alive_;}

void Player::promptDoorAction(char key)
{
    char input = prompt("In what direction?");
    direction_t direction = charToDirection(input);
    tile_t tile = world_->getTile(x_ + direction.x, y_ + direction.y, map_level_);

    if(!isValidDirection(direction))
    {
  message("Invalid Key");
    }
    else
    {
  if (tile.tile_type == TILE_DOOR_OPEN)
  {
      if (key == 'o')
      {
    message("The door is already open");
      }
      else
      {
    if (world_->getEnemyAt(x_ + direction.x, y_ + direction.y, map_level_) == NULL)
    {

        closeDoor(x_ + direction.x, y_ + direction.y, map_level_);
    }
    else
    {
        message("There is something standing here");
    }
      }
  }
  else if (tile.tile_type == TILE_DOOR_CLOSED)
  {
      if (key == 'c')
      {
    message("The door is already closed");
      }
      else
      {
    if (world_->getEnemyAt(x_ + direction.x, y_ + direction.y, map_level_ ) == NULL)
    {
        openDoor(x_ + direction.x, y_ + direction.y, map_level_);
    }
    else
    {
        message("ERROR something is standing here but door is closed");
    }
      }
  }
  else
  {
      message("You see no door there");
  }
    }
}

void Player::promptPickupAction()
{
    std::vector<Item*> items = world_->getItemsAt(x_, y_, map_level_);
    if(items.size() == 1)
    {
  char free_slot = pickUpItem(items[0]);
  std::stringstream new_item_stream;
  new_item_stream << free_slot << " - " << items[0]->getName();
  message(new_item_stream.str());
    }
    else if (items.size() > 1)
    {
  //char item_char = prompt("Which item do you want?");
  message("TODO: handle picking up multiple objects");
  pickUpItem(items[0]);
  //TODO show player list in which they can choose an item for them to pick
    }
    else
    {
  message("There are no items here");
    }
}

void Player::promptDropAction()
{
  displayDropItemsScreen();
}

void Player::promptUseItemAction()
{
    Item *item = displayUseItemScreen();
    if (item != NULL)
    {
  std::stringstream use_stream;
  use_stream << "you ";

  switch(item->getCategory())
  {
  case CATEGORY_WEAPON: use_stream << "wield"; break;
  case CATEGORY_BODY_ARMOUR: use_stream << "put on"; break;
  case CATEGORY_POTION: use_stream << "drink the"; break;
  default: use_stream << "ERROR: malformed item category on use"; break;
  }

  use_stream << " the " << item->getName() << ".";

  displayGameScreen();
  message(use_stream.str());
  useItem(item);
    }
}

void Player::moveAction(int x, int y, int z)
{
    move(x,y,z);
    if(x_ == x && y_ == y && map_level_ == z)//if they actually moved there
    {
	std::vector<Item*> items = world_->getItemsAt(x_, y_, map_level_);
	if(items.size() == 1)
	{
	    std::stringstream ground_stream;
	    ground_stream << "You see a " << items[0]->getName() << " here.";
	    message(ground_stream.str());
	    updateScreen();
	}
	else if (items.size() > 1)
	{
	    message("You see many items here");
	}
    }
}

void Player::addExp(int exp)
{
    experience_ = experience_ + exp;
    checkForLevelUp();
}

void Player::checkForLevelUp()
{
    int level = findLevelByExp(experience_);
    if(level > level_)
    {
	    levelUp(level);
    }
}

void Player::levelUp(int to_level)
{
    level_ = to_level;
    current_health_points_ = max_health_points_;
    current_energy_points_ = max_energy_points_;
    std::stringstream levelup_stream;
    levelup_stream << "You are now level " << level_;
    message(levelup_stream.str());

    bool made_selection = false;
    while(!made_selection)
    {
	char result = prompt("Which attribute will you raise? [(s)trength, (d)exterity, (i)ntelligence, (v)itality]");
	switch(result)
	{
	case 's': att_str_++; message("You feel yourself get stronger"); made_selection = true; break;
	case 'd': att_dex_++; message("You feel more nimble"); made_selection = true; break;
	case 'i': att_int_++; message("You feel more knowledgable"); made_selection = true; break;
	case 'v': att_vit_++; message("You feel healthier and more vibrant"); made_selection = true; break;
	}
    }
}

int Player::findLevelByExp(int exp)
    {
	for (unsigned int i = 0; i < sizeof(experience_thresholds)/sizeof(int); i++)
	{
	    if (exp < experience_thresholds[i])
	    {
		return i;
	    }
	}
	return 0;
    }

int Player::getLevel()
{return level_;}

int Player::getExp()
{return experience_;}















