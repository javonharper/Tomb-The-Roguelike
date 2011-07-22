/*
 *  File:       enemy.h
 *  Summary:    things trying to kill the player
 *  Written by: Javon Harper
 */
#ifndef ENEMY_H
#define ENEMY_H

#include "actor.h"
#include "map.h"

class Map;
class Player;
class Enemy : public Actor
{
  private:
    Map *visibility_map_;
    position_t last_known_player_pos_;
    int state_;
  public:
    Enemy(enemy_data_t data, World *world);
    ~Enemy();
    void takeTurn();
    bool isEnemyAtPosition(int x, int y, int level);
    void kill();
    void setVisibilityMap(Map *map);
    Map* getVisibilityMap();
    void setLastKnownPlayerPosition(int x, int y);
};

#endif
