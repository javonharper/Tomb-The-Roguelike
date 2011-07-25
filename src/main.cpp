/*
 *  File:       main.cpp
 *  Summary:    main
 *  Written by: Javon Harper
 */
#include <iostream>
#include <vector>
#include "libtcod.hpp"
#include "enemy.h"
#include "functions.h"
#include "interface.h"
#include "player.h"
#include "random.h"
#include "world.h"

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
  //showMainMenu();

  world = new World(48, 37, 1);
  player = world->generatePlayer();
  enemies = world->generateEnemies();
  world->generateItems();

  //While the game is still going, iterate over all actors
  while (!isGameOver())
  {
    world->incrementTimeStep();
    std::cout<< "=WORLD STEP" << world->getTimeStep()<<"=" << std::endl;
    displayGame(world);
    player->FOV(world->getMapLevel(player->getMapLevel()));
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
        en->FOV(en->getVisibilityMap());
        en->takeTurn();
      }
      en->endTurn();
    }
  }

  if (isGameOver())
  {
    displayGameOverScreen("You have died.");
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
    case 'h': player->move(player->getXPosition() - 1, player->getYPosition(), player->getMapLevel()); break;
    case 'j': player->move(player->getXPosition(), player->getYPosition() + 1, player->getMapLevel()); break;
    case 'k': player->move(player->getXPosition(), player->getYPosition() - 1, player->getMapLevel()); break;
    case 'l': player->move(player->getXPosition() + 1, player->getYPosition(), player->getMapLevel()); break;
    case 'y': player->move(player->getXPosition() - 1, player->getYPosition() - 1, player->getMapLevel()); break;
    case 'u': player->move(player->getXPosition() + 1, player->getYPosition() - 1, player->getMapLevel()); break;
    case 'b': player->move(player->getXPosition() - 1, player->getYPosition() + 1, player->getMapLevel()); break;
    case 'n': player->move(player->getXPosition() + 1, player->getYPosition() + 1, player->getMapLevel()); break;
    case '.': player->rest();
    case '>': player->descendStairs(); break;
    case '<': player->ascendStairs(); break;
    case 27: displayGameOverScreen("You have exited the game."); break;
  }
}

