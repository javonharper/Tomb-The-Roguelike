/*
 *  File:       player.cpp
 *  Summary:    Our hero
 *  Written by: Javon Harper
 */
#include <cmath>
#include <iostream>
#include "controller.h"
#include "enemy.h"
#include "functions.h"
#include "interface.h"
#include "map.h"
#include "random.h"
#include "world.h"
#include "player.h"

Player::Player(World *world)
{
  initProperties(enemy_db[PLAYER], world);
  is_alive_ = true;
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
  displayDropItemsScreen(world_);
}















