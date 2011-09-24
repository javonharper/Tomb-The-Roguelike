/*
 *  File:       main.cpp
 *  Summary:    main
 *  Written by: Javon Harper
 */

#include <iostream>
#include <vector>
#include <sstream>
#include "libtcod.hpp"
#include "enemy.h"
#include "functions.h"
#include "interface.h"
#include "player.h"
#include "random.h"
#include "world.h"
#include "controller.h"

void initScreen();
bool isGameOver();
void handleKeyPress();

World *world;
Player *player;
std::vector<Enemy*> enemies;

int main(int argc, char* argv[])
{
    int seed = init_rand();
    //int seed = init_rand(1314485160);
    std::cout << "seed:" << seed << std::endl;

    initScreen();
    displayTitleScreen();

    int world_width = worldScreenDims[WIDTH];
    int world_height = worldScreenDims[HEIGHT];
    int world_depth = 5;

    world = new World(world_width, world_height, world_depth);
    setWorld(world);//interface.h, just to keep a pointer to the world so It doesn't have to get passed with every message, prompt, etc. probably a better way to do this but i cba
    player = world->generatePlayer();
    enemies = world->generateEnemies(4,8);
    world->generateItems(5,10);

    std::stringstream intro_stream;
    intro_stream << "You are " << player->getName() << ", an Adventurer in search of the Icon of Weedaula.";
    message(intro_stream.str());
    message("Bring the Icon back to the surface to win the game!");
    message("Press '?' for help");

    while (!isGameOver())
    {
        world->incrementTimeStep();
        player->FOV(player->getMapLevel());
        displayGameScreen();
        updateScreen();

        player->startTurn();
        while (!player->isTurnFinished() && player->isTurn() && !isGameOver())
        {
            handleKeyPress();
        }
        player->endTurn();

        enemies = world->getEnemyList();
        for (unsigned int i = 0; i < enemies.size(); i++)
        {
            Enemy *en = enemies[i];
            en->FOV(en->getMapLevel());
            en->startTurn();
            if (en->isTurn() && withinRange(en->getMapLevel(), player->getMapLevel(), player->getMapLevel()))
            {
                en->takeTurn();
            }
            en->endTurn();
        }
    }

    if (isGameOver())
    {
        if (!player->isAlive())
        {
            displayGameOverScreen("You have been struck down");
        }
    }
}

bool isGameOver()
{
    return TCODConsole::isWindowClosed() || !player->isAlive();
}

void handleKeyPress()
{
    TCOD_key_t key = TCODConsole::waitForKeypress(true);
    switch (key.c)
    {
        //actions
    case MOVE_WEST:
        player->moveAction(player->getXPosition() - 1, player->getYPosition(), player->getMapLevel());
        break;
    case MOVE_SOUTH:
        player->moveAction(player->getXPosition(), player->getYPosition() + 1, player->getMapLevel());
        break;
    case MOVE_NORTH:
        player->moveAction(player->getXPosition(), player->getYPosition() - 1, player->getMapLevel());
        break;
    case MOVE_EAST:
        player->moveAction(player->getXPosition() + 1, player->getYPosition(), player->getMapLevel());
        break;
    case MOVE_NORTHWEST:
        player->moveAction(player->getXPosition() - 1, player->getYPosition() - 1, player->getMapLevel());
        break;
    case MOVE_NORTHEAST:
        player->moveAction(player->getXPosition() + 1, player->getYPosition() - 1, player->getMapLevel());
        break;
    case MOVE_SOUTHWEST:
        player->moveAction(player->getXPosition() - 1, player->getYPosition() + 1, player->getMapLevel());
        break;
    case MOVE_SOUTHEAST:
        player->moveAction(player->getXPosition() + 1, player->getYPosition() + 1, player->getMapLevel());
        break;
    case REST:
        player->restAction();
        break;
    case MOVE_DOWNSTAIRS:
        player->descendStairsAction();
        break;
    case MOVE_UPSTAIRS:
        player->ascendStairsAction();
        break;
    case OPEN_DOOR:
        player->openDoorAction();
        break;
    case CLOSE_DOOR:
        player->closeDoorAction();
        break;
    case PICKUP:
        player->pickupItemAction();
        break;
    case CHANGE_CLASS:
        player->changeClassAction();
        displayGameScreen();
        updateScreen();
        break;

        //screens
    case SHOW_INVENTORY:
        displayInventoryScreen();
        break;
    case HELP:
        displayHelpScreen();
        displayGameScreen();
        updateScreen();
        break;
    case QUIT:
        displayGameOverScreen("You have exited the game.");
        break;
    }
}


