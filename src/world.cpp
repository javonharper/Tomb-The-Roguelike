/*
 *  File:       world.cpp
 *  Summary:    holds the maps and actors.
 *  Written by: Javon Harper
 */
#include <iostream>
#include "functions.h"
#include "item.h"
#include "item_db.h"
#include "random.h"
#include "world.h"
#include "player.h"
#include "enemy.h"

//Creates a world with a given width, height, and depth
World::World(int world_width, int world_height, int world_levels)
{
  this->width_ = world_width;
  this->height_ = world_height;
  this->levels_ = world_levels;
  this->current_level_ = START_LEVEL;
  this->time_step_ = 0;

  //Create a Map for each level
  for (int i = START_LEVEL; i < world_levels; i++)
  {
    level_list_.push_back(new Map(world_width, world_height));
  }

  position_t pos = this->getMapLevel(world_levels - 1)->getDownStairPos();
  this->getMapLevel(world_levels - 1)->setTile(pos.x, pos.y, TILE_WALL);
}

World::~World()
{
  level_list_.clear();
  delete player_;
}

//Creates the player and finds a position on the start level for it.
Player* World::generatePlayer()
{
  this->player_ = new Player(this);
  position_t stair_pos = this->getMapLevel(this->getCurrentLevel())->getUpStairPos();
  player_->setPosition(stair_pos.x, stair_pos.y, this->getCurrentLevel());
  return this->player_;
}

//Creates enemies for the map
//for each level, it generates a random number of enemies that will be on that level.
//Then if generate a random number corrosponding to an enemy type.
//If that enemy's habiat lies at the level generated earlier,
//the monster created, added to the enemy list, and given a spot on that level.
std::vector<Enemy*> World::generateEnemies()
{
  //For each level generate enemeis
  for(int i = START_LEVEL; i < this->getLevels(); i++)
  {
    //int num_enemies = random(5, 10);
    int num_enemies = 1;

    //for each enemy, assign a type that will be on that level.
    for (int j = 0; j < num_enemies; j++)
    {
      bool found_enemy_type = false;
      while(!found_enemy_type)
      {
        int type = random(0, TOTAL_ENEMY_TYPES - 1);
        if(withinRange(type, enemy_db[type].habitat[0], enemy_db[type].habitat[1]))
        {
          Enemy *enemy = new Enemy(enemy_db[type], this);
          position_t new_pos = this->findPosition(i);
          enemy->setPosition(new_pos.x, new_pos.y, i);
          enemy->setVisibilityMap(this->getMapLevel(i));
          enemy_list_.push_back(enemy);
          found_enemy_type = true;
        }
      }
    }
  }
  return enemy_list_;
}

std::vector<Item*> World::generateItems()
{
  int num_items = 5;
  for (int i = 0; i < num_items ;i++ )
  {
    int level = random(0, this->getLevels() - 1);
    position_t position = findPosition(level);
    Item *item = new Item();
    item->setPosition(position.x, position.y, level);
    item_list_.push_back(item);
  }
  return item_list_;
}

//finds a passable position on a map level indicated by the param
position_t World::findPosition(int level)
{
  bool found_spot = false;
  position_t new_position;
  new_position.x = -1;
  new_position.y = -1;

  for (int trials = 0; trials < 1000 && !found_spot; trials++)
  {
    int x = random(0, this->getWidth() - 1);
    int y = random(0, this->getHeight() - 1);

    if(this->getTile(x, y, level).is_passable)
    {
      new_position.x = x;
      new_position.y = y;
      found_spot = true;
    }
  }
  return new_position;
}

//Gets the enemy at the corresponding coordinates
//Iterated through all the enemies and checks their x and y values against the given params
//returns null otherwise
Enemy* World::getEnemyAt(int x, int y, int level)
{
  Enemy *the_enemy = NULL;
  std::vector<Enemy*> enemies = this->getEnemyList();
  for (unsigned int i = 0; i < enemies.size(); i++)
  {
    Enemy *en = enemies[i];
    if(en->getXPosition() == x && en->getYPosition() == y && en->getMapLevel() == level)
    {
      the_enemy = en;
      break;
    }
  }
  return the_enemy;
}

//Accessors and Mutators
Player* World::getPlayer(){return player_;}
std::vector<Enemy*>& World::getEnemyList(){return enemy_list_;}
tile_t World::getTile(int x, int y, int z){return level_list_[z]->getTile(x, y);}
void World::setTile(int x, int y, int z, int tile_type){this->getMapLevel(z)->setTile(x,y, tile_type);}
void World::setTileAsSeen(int x, int y, int z){this->getMapLevel(z)->setTileAsSeen(x, y);}
int World::getWidth(){return width_;}
int World::getHeight(){return height_;}
int World::getLevels(){return levels_;}
int World::getCurrentLevel(){return current_level_;}
void World::setCurrentLevel(int level){this->current_level_ = level;}
int World::getTimeStep(){return time_step_;}
void World::incrementTimeStep(){time_step_ =  time_step_ + 1;}
Map* World::getMapLevel(int level){return level_list_[level];}

