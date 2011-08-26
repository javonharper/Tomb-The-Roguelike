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
  //int seed = init_rand(2);
  std::cout << "seed:" << seed << std::endl;

  initScreen();
  displayTitleScreen();

  int world_width = worldScreenDims[WIDTH];
  int world_height = worldScreenDims[HEIGHT];
  int world_depth = 5;

  world = new World(world_width, world_height, world_depth);
  setWorld(world);//interface.h, just to keep a pointer to the world so It doesn't have to get passed with every message, prompt, etc. probably a better way to do this but i dunno it
  player = world->generatePlayer();
  enemies = world->generateEnemies(0,0);
  world->generateItems(10,20);

  std::stringstream intro_stream;
  intro_stream << "You are " << player->getName() << ", an adventurer in search of the Icon of Weedaula.";
  message(intro_stream.str());
  message("You find yourself in a dank labyrinth of the tomb.");
  message("Your heart drops as you begin your trek.");
  message("Press '?' for help");

  //While the game is still going, iterate over all actors
  while (!isGameOver())
  {
    world->incrementTimeStep();
//    std::cout<< "=WORLD STEP" << world->getTimeStep()<<"=" << std::endl;

    player->FOV(player->getMapLevel());
    displayGameScreen();
    updateScreen();

    player->startTurn();
    while(!player->isTurnFinished() && player->isTurn() && !isGameOver())
    {
      //std::cout << player->getName() << &player << " taking turn at world step " << world->getTimeStep() << std::endl;
      handleKeyPress();
    }
    player->endTurn();

    //make the actors take their turns
    //NOTE only actors +- one level of the player can take turns.
    enemies = world->getEnemyList();
    for (unsigned int i = 0; i < enemies.size(); i++)
    {
      Enemy *en = enemies[i];

      en->startTurn();
      //NOTE messing with level ranges
      if(en->isTurn() && withinRange(en->getMapLevel(), player->getMapLevel() - 0, player->getMapLevel() + 0))
      {
        //std::cout << en->getName() << &enemies[i] <<" taking turn at world step " << world->getTimeStep() << std::endl;
        en->FOV(en->getMapLevel());
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
  switch(key.c)
    {
    case MOVE_WEST: player->moveAction(player->getXPosition() - 1, player->getYPosition(), player->getMapLevel()); break;
    case MOVE_SOUTH: player->moveAction(player->getXPosition(), player->getYPosition() + 1, player->getMapLevel()); break;
    case MOVE_NORTH: player->moveAction(player->getXPosition(), player->getYPosition() - 1, player->getMapLevel()); break;
    case MOVE_EAST: player->moveAction(player->getXPosition() + 1, player->getYPosition(), player->getMapLevel()); break;
    case MOVE_NORTHWEST: player->moveAction(player->getXPosition() - 1, player->getYPosition() - 1, player->getMapLevel()); break;
    case MOVE_NORTHEAST: player->moveAction(player->getXPosition() + 1, player->getYPosition() - 1, player->getMapLevel()); break;
    case MOVE_SOUTHWEST: player->moveAction(player->getXPosition() - 1, player->getYPosition() + 1, player->getMapLevel()); break;
    case MOVE_SOUTHEAST: player->moveAction(player->getXPosition() + 1, player->getYPosition() + 1, player->getMapLevel()); break;
    case REST: player->rest(); break;
    case MOVE_DOWNSTAIRS: player->descendStairs(); break;
    case MOVE_UPSTAIRS: player->ascendStairs(); break;
    case OPEN_DOOR: player->promptDoorAction(key.c); break;
    case CLOSE_DOOR: player->promptDoorAction(key.c); break;
    case PICKUP: player->promptPickupAction(); break;
    case DROP: player->promptDropAction(); break;
    case USE_ITEM: player->promptUseItemAction(); break;
    case SHOW_INVENTORY: displayInventoryScreen(); break;
    case HELP: displayHelpScreen(); break;
    case ESC: displayGameOverScreen("You have exited the game."); break;
    }
}


