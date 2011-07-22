/*
 *  File:       player.cpp
 *  Summary:    Our hero
 *  Written by: Javon Harper
 */
#include <cmath>
#include <iostream>
#include "enemy.h"
#include "functions.h"
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
    //enemy->meleeAttack(this);
  }
  return found_enemy;
}

void Player::kill()
{is_alive_ = false;}

bool Player::isAlive()
{return is_alive_;}
