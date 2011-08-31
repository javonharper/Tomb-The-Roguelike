/*
 *  File:       player.h
 *  Summary:    Our hero
 *  Written by: Javon Harper
 */
#ifndef PLAYER_H
#define PLAYER_H

#include "actor.h"

const static int experience_thresholds[] = {
    0,
    1000,
    3000,
    6000,
    10000,
    15000,
    21000,
    28000,
    36000,
    45000,
    55000,
    66000,
    78000,
    91000,
    105000,
    120000,
    136000,
    153000,
    171000,
    190000,
    210000,
    231000,
    253000,
    276000,
    300000,
    325000,
    351000,
    378000,
    406000,
    435000,
    465000,
    496000,
    528000,
    561000,
    595000,
    630000,
    666000,
    703000,
    741000,
    780000,
    820000,
    861000,
    903000,
    946000,
    990000,
    1035000,
    1081000,
    1128000,
    1176000,
    1225000
};

class Enemy;
class Class;
class Player : public Actor
{
private:
    int level_;
    int experience_;
    bool is_alive_;
    Class *class_;
public:
    Player(World *world);
    ~Player();
    bool isEnemyAtPosition(int x, int y, int level);
    void kill();
    bool isAlive();

    void addExp(int exp);
    void levelUp();
    void checkForLevelUp();
    int findLevelByExp(int exp);

    void moveAction(int x, int y, int z);
    void restAction();
    void openDoorAction();
    void closeDoorAction();
    void pickupItemAction();
    void dropItemAction();
    void useItemAction(Item *item);
    void changeClassAction();//Lawsuit

    int getLevel();
    int getExp();

    Class *getClass();
};

#endif
