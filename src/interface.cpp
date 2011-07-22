/*
 *  File:       interface.cpp
 *  Summary:    Anything that displays on the screens is located here.
 *  Written by: Javon Harper
 */
#include <vector>
#include "libtcod.hpp"
#include "map.h"
#include "interface.h"
#include "player.h"
#include "enemy.h"

void initScreen()
{
  TCODConsole::setCustomFont("art/terminal.png"/*, TCOD_FONT_LAYOUT_ASCII_INROW*/);
  TCODConsole::initRoot(SCREEN_WIDTH, SCREEN_HEIGHT, "Tomb");
  TCODSystem::setFps(25);

  for (int i = 0; i < SCREEN_WIDTH; i++)
  {
    for (int j = 0; j < SCREEN_HEIGHT; j++)
    {
 //      TCODConsole::root->printCenter(i, j, TCOD_BKGND_NONE, " ");
    }
  }
}

void showMainMenu()
{
  //display the libtcod display
  TCODConsole::credits();

  //Show the Main Menu
  TCODConsole::root->clear();
//  TCODConsole::root->printCenter(40, 25, TCOD_BKGND_NONE, "New Game");
  TCODConsole::flush();
  TCODConsole::waitForKeypress(true);
}

void updateScreen()
{
  TCODConsole::flush();
}

void displayWorld(World *world, bool centered)
{
  Player *player = world->getPlayer();
  if(!centered)
  {
    //Display all the tiles of the world.
    for (int i = 0; i < world->getWidth(); i++)
      for (int j = 0; j < world->getHeight(); j++)
      {
        //For every tile on the map; if player can see it it hass full color
        //If the player has seen it before but cannot see it now, make grey
        //If the player has never seen it, make it black.
        tile_t this_tile = world->getTile(i, j, world->getCurrentLevel());
        if(this_tile.visible)
        {
          TCODConsole::root->putCharEx(i, j, this_tile.face_tile, this_tile.color, TCODColor::black);
        }
        else if(this_tile.has_been_seen)//has seen before
        {
          TCODConsole::root->putCharEx(i, j, this_tile.face_tile, TCODColor::darkGrey, TCODColor::black);
        }
        else if(!this_tile.has_been_seen)//never seen
        {
          TCODConsole::root->putCharEx(i, j, this_tile.face_tile, TCODColor::black, TCODColor::black);
        }

      }

      std::vector<Item*> items = world->getItemList();
      for (unsigned int i = 0; i < items.size(); i++)
      {
        ///TODO display the items that the player can see
      }

      //draws the enemy if they are on the same level at the actor.
      std::vector<Enemy*> enemies = world->getEnemyList();
      for (unsigned int i = 0; i < enemies.size(); i++)
      {
        Enemy* en = enemies[i];
        if(en->getMapLevel() == world->getCurrentLevel() && player->canSee(world->getMapLevel(en->getMapLevel()), en->getXPosition(), en->getYPosition()))
        {
          TCODConsole::root->putCharEx(en->getXPosition(), en->getYPosition(), en->getFaceTile(), en->getColor(), TCODColor::black);
        }
      }

    //Display the player
    TCODConsole::root->putCharEx(player->getXPosition(), player->getYPosition(), '@', TCODColor::white, TCODColor::black);
  }
  else //IF THE FOCUS IS CENTERED ON THE PLAYER
  {
    int action_window_width = 80;
    int action_window_height = 50;
    int center_x = action_window_width/2;
    int center_y = action_window_height/2;
    int player_x = world->getPlayer()->getXPosition();
    int player_y = world->getPlayer()->getYPosition();

    int offset_x = player_x  - center_x;
    int offset_y = player_y -  center_y;


    //Display all the tiles of the world. (centered)
    for (int i = 0; i < action_window_width; i++)
      for (int j = 0; j < action_window_height; j++)
      {

        if (offset_x + i < 0 || offset_x + i >= world->getWidth() - 1 || offset_y + j < 0 || offset_y + j>= world->getHeight())
          TCODConsole::root->putCharEx(i, j, ' ', TCODColor::black, TCODColor::black);
        else
        {
          tile_t this_tile = world->getTile(offset_x + i, offset_y + j, world->getCurrentLevel());
          TCODConsole::root->putCharEx(i, j, this_tile.face_tile, this_tile.color, TCODColor::black);
        }
      }

      std::vector<Enemy*> enemies = world->getEnemyList();
      for (unsigned int i = 0; i < enemies.size(); i++)
      {
        //TODO check if the player can actually see this enemy
        if(enemies[i]->getMapLevel() == world->getCurrentLevel())
        {
          Enemy* en = enemies[i];
          int x = center_x + en->getXPosition() - player_x;
          int y = center_y + en->getYPosition() - player_y;
          TCODConsole::root->putCharEx(x, y, en->getFaceTile(), en->getColor(), TCODColor::black);
        }
      }
    //Display the player
    TCODConsole::root->putCharEx(40, 25, '@', TCODColor::white, TCODColor::black);
  }
}

void displayUserInterface()
{
}

void displayGameOverScreen(std::string reason)
{
  TCODConsole::root->clear();
  TCODConsole::root->print(10, 10, "Game Over!");
  TCODConsole::root->print(12, 11, reason.c_str());
  updateScreen();
  TCODConsole::waitForKeypress(true);
  exit(0);
}

void displayWinScreen()
{
  TCODConsole::root->clear();
  TCODConsole::root->print(10, 10, "You Win!");
  updateScreen();
  TCODConsole::waitForKeypress(true);
  exit(0);
}
