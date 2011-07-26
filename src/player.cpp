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
{is_alive_ = false;}

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
	    closeDoor(x_ + direction.x, y_ + direction.y, map_level_);
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
	    openDoor(x_ + direction.x, y_ + direction.y, map_level_);
	  }
      }
    else
      {
	message("You see no door there");
      }
    } 
}















