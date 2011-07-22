/*
 *  File:       enemy.cpp
 *  Summary:    things trying to  the player
 *  Written by: Javon Harper
 */
#include <cmath>
#include <iostream>
#include <string>
#include "enemy.h"
#include "map.h"
#include "player.h"
#include "random.h"
#include "world.h"


Enemy::Enemy(enemy_data_t data, World *world)
{
  initProperties(data, world);
  this->state_ = STATE_WANDERING;
  this->setLastKnownPlayerPosition(-1, -1);
}

Enemy::~Enemy()
{
}

//Basic AI for the enemy
void Enemy::takeTurn()
{
  std::cout << this->getName() << " taking turn." << std::endl;

  Player *player = this->getWorld()->getPlayer();
  if(canSee(visibility_map_, player->getXPosition(), player->getYPosition()))
  {
    std::cout << this->getName() << " can see player" << std::endl;
    this->state_ = STATE_HOSTILE;
    this->setLastKnownPlayerPosition(player->getXPosition(), player->getYPosition());
    moveTowards(player->getXPosition(), player->getYPosition());
  }
  else //if the enemy cannot see the monster
  {
    std::cout << this->getName() << " cannot see player" << std::endl;
    //if it was hostile, then make it go to where it last saw the monster
    if(state_ == STATE_HOSTILE)
    {
          std::cout << this->getName() << "was hostile but now searching" << std::endl;

      this->state_ = STATE_SEARCHING;
      moveTowards(last_known_player_pos_.x,last_known_player_pos_.y);
    }
    //if searching, keep searching unless already at the spot. Then, just wander
    else if (state_ == STATE_SEARCHING)
    {
      if (x_ != last_known_player_pos_.x && y_ !=last_known_player_pos_.y)
      {
                  std::cout << this->getName() << "searching for player" << std::endl;

        moveTowards(last_known_player_pos_.x,last_known_player_pos_.y);
      }
      else
      {
                  std::cout << this->getName() << "was searching but now wandering" << std::endl;

        this->state_ = STATE_WANDERING;
        wander();
      }
    }
    //if can't see the player and wandering, then keep wandering.
    else if (state_ == STATE_WANDERING)
    {
      //how about instead of moving in random direction,
    //the enemy chooses a random location and walks there?
              std::cout << this->getName() << "wwandering" << std::endl;

      wander();
    }
  }
}

//checks if the player is at a given position.
bool  Enemy::isEnemyAtPosition(int x, int y, int level)
{
  bool found_player = false;
  Player* player = this->getWorld()->getPlayer();
  if(player->getXPosition() == x && player->getYPosition() == y && player->getMapLevel() == level)
  {
    found_player = true;
    meleeAttack(player);
  }
  return found_player;
}

//removes this enemy from the enemylist stored in the world
void Enemy::kill()
{
  std::vector<Enemy*> &enemy_list = this->getWorld()->getEnemyList();

  for(unsigned int i = 0; i < enemy_list.size(); i++)
  {
    if(this == enemy_list[i])
    {
      enemy_list.erase(enemy_list.begin() + i);
    }
  }
}

//Makes a copy of the map level that the enemy is on, but
//creates a personal set of tiles for it to do fov/pathfinding stuffs.
void Enemy::setVisibilityMap(Map *map)
{
  visibility_map_ = new Map(*map);

  tile_t **tile_map_;
  //Create new references to the tile map
  tile_map_ = new tile_t*[map->getWidth()];
  for (int i = 0; i < map->getWidth(); i++)
    tile_map_[i] = new tile_t[map->getHeight()];

  //copy the properties from the existing map to the visibility map
  for (int i = 0; i < map->getWidth(); i++)
  {
    for (int j = 0; j < map->getHeight(); j++)
    {
      tile_map_[i][j] = map->getTile(i, j);
    }
  }
  visibility_map_->setTileMap(tile_map_);
}

Map* Enemy::getVisibilityMap()
{return this->visibility_map_;}

void Enemy::setLastKnownPlayerPosition(int x, int y)
{
  this->last_known_player_pos_.x = x;
  this->last_known_player_pos_.y = y;
}
