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
  
  //make the player character unique
  TCODNamegen::parse("data/names.txt", NULL);
  name_ = std::string((char*)TCOD_namegen_generate("player", false));
  TCODNamegen::destroy();
  att_str_ = random(ATT_BAD, ATT_GOOD);
  att_wis_ = random(ATT_BAD, ATT_GOOD);
  att_dex_ = random(ATT_BAD, ATT_GOOD);
  att_vit_ = random(ATT_BAD, ATT_GOOD);
}

Player::~Player()
{
}

bool Player::isEnemyAtPosition(int x, int y, int level)
{
  bool found_enemy = false;
  Enemy *enemy = this->getWorld()->getEnemyAt(x, y, level);
  if(enemy != NULL)
    {
      found_enemy = true;
      this->meleeAttack(enemy);
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
	pickUpItem(items[0]);
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
  std::stringstream use_stream;
  use_stream << "you ";
  
  switch(item->getCategory())
  {
    case CATEGORY_WEAPON: use_stream << "wield"; break;
    case CATEGORY_BODY_ARMOUR: use_stream << "put on"; break;
    default: use_stream << "ERROR: malformed item category on use"; break;
  }
  
  use_stream << " the " << item->getName() << ".";
  
  displayGameScreen();
  message(use_stream.str());
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

int Player::getLevel()
{return level_;}















