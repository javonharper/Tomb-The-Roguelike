/*
 *  File:       player.h
 *  Summary:    Our hero
 *  Written by: Javon Harper
 */
#ifndef PLAYER_H
#define PLAYER_H

#include "actor.h"

class Enemy;
class Player : public Actor
{
  private:
    bool is_alive_;
  public:
    Player(World *world);
    ~Player();
    bool isEnemyAtPosition(int x, int y, int level);
    void kill();
    bool isAlive();

    void promptDoorAction(char key);
    void promptPickupAction();
    void promptDropAction();
};

#endif
