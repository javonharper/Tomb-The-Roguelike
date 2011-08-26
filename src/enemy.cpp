/*
 *  File:       enemy.cpp
 *  Summary:    things trying to  kill the player
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
    exp_reward_ = data.exp_reward;
    state_ = STATE_WANDERING;
    setLastKnownPlayerPosition(-1, -1);
    world->getEnemyList().push_back(this);
}

Enemy::~Enemy()
{
}

//Basic AI for the enemy
//STATES:
//HOSTILE sees the player and is trying to attack it
//SEARCHING saw the player recently and is going to the last spot it spotted the pc
//WANDERING does not see the pc and is not searching

//All enemies start off wandering, if one see the player they become hostile and tries to attack him.
//If by any reason the enemy no longer sees the pc, they will move to where they last spotted him/her
//if they get to that spot and still don't see the player, they will wander again.
void Enemy::takeTurn()
{
    Player *player = world_->getPlayer();
    if(canSee(map_level_, player->getXPosition(), player->getYPosition()))
    {
	this->state_ = STATE_HOSTILE;
	this->setLastKnownPlayerPosition(player->getXPosition(), player->getYPosition());
	moveTowards(player->getXPosition(), player->getYPosition());
    }
    else
    {
	if(state_ == STATE_HOSTILE)
	{
	    this->state_ = STATE_SEARCHING;
	    moveTowards(last_known_player_pos_.x,last_known_player_pos_.y);
	}
	else if (state_ == STATE_SEARCHING)
	{
	    if (x_ != last_known_player_pos_.x && y_ !=last_known_player_pos_.y)
	    {
		moveTowards(last_known_player_pos_.x,last_known_player_pos_.y);
	    }
	    else
	    {
		this->state_ = STATE_WANDERING;
		wander();
	    }
	}
	else if (state_ == STATE_WANDERING)
	{
	    //how about instead of moving in random direction,
	    //the enemy chooses a random location and walks there?
	    wander();
	}
    }
}

//checks if the player is at a given position.
bool  Enemy::isEnemyAtPosition(int x, int y, int level)
{
    bool found_player = false;
    Player* player = world_->getPlayer();
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
    setCurrentHealth(0);
    std::vector<Enemy*> &enemy_list = world_->getEnemyList();

    for(unsigned int i = 0; i < enemy_list.size(); i++)
    {
	if(this == enemy_list[i])
	{
	    enemy_list.erase(enemy_list.begin() + i);
	}
    }

    bleed();
    //will need to implement something that checks if the blood is red, and if they bleed at all
    if(mod_size_ < SIZE_MEDIUM)//lower numbers correspond to bigger sizes
    {
	world_->setTileColor(x_+1, y_, map_level_, TCODColor::desaturatedRed);
	world_->setTileColor(x_-1, y_, map_level_, TCODColor::desaturatedRed);
	world_->setTileColor(x_, y_+1, map_level_, TCODColor::desaturatedRed);
	world_->setTileColor(x_, y_-1, map_level_, TCODColor::desaturatedRed);
    }
}

void Enemy::setLastKnownPlayerPosition(int x, int y)
{
    this->last_known_player_pos_.x = x;
    this->last_known_player_pos_.y = y;
}

int Enemy::getExpReward(){return exp_reward_;}
