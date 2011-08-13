/*
 *  File:       interface.cpp
 *  Summary:    Anything that displays on the screens is located here.
 *  Written by: Javon Harper
 */
#include <map>
#include <sstream>
#include <vector>
#include "map.h"
#include "interface.h"
#include "inventory.h"
#include "item.h"
#include "player.h"
#include "enemy.h"
#include "controller.h"

std::vector<std::string> message_log;

void initScreen()
{
  TCODConsole::setCustomFont("art/Alloy_curses_12x12.png", TCOD_FONT_LAYOUT_ASCII_INROW);
  TCODConsole::initRoot(SCREEN_WIDTH, SCREEN_HEIGHT, "Tomb");
  TCODSystem::setFps(25);
}

void displayTitleScreen()
{
  TCODConsole::root->clear();
  TCODConsole::root->print(0,1, "The Tomb, Copyright 2011");
  TCODConsole::root->print(2,2, "By Javon Harper");
  TCODConsole::root->print(2,3, "see LICENSE.txt for details");
  TCODConsole::root->print(0,5, "Press a button to continue");
  
  updateScreen();
  TCODConsole::waitForKeypress(true);
}

void updateScreen()
{
  TCODConsole::flush();
}

void displayGameScreen(World *world)
{
  TCODConsole::root->clear();
  drawWorldPanel(world);
  drawInfoPanel(world);
  drawLogPanel(world);
}

void drawWorldPanel(World *world)
{
  drawWorld(world);
}

void drawInfoPanel(World *world)
{
  drawVerticalLine(infoScreenDims[X], infoScreenDims[Y], SCREEN_HEIGHT + 1, TCODColor::orange);
  int y = infoScreenDims[Y];
  int x = infoScreenDims[X] + 1;
  
  //TODO make this accurate
  Player *player = world->getPlayer();
  
  TCODConsole::root->print(x, y++, player->getName().c_str());
  
  std::stringstream player_level;
  player_level << "Level " << player->getLevel();
  TCODConsole::root->print(x, y++,  player_level.str().c_str());
  
  drawHorizontalLine(x, y, infoScreenDims[WIDTH], TCODColor::orange); 
  TCODConsole::root->putCharEx(x - 1, y++, TCOD_CHAR_TEEE, TCODColor::orange, TCODColor::black);
  
  std::stringstream health_info;
  health_info << "Health " << player->getCurrentHealth()<<"/"<<player->getMaxHealth();
  TCODConsole::root->print(x, y++, health_info.str().c_str());
  
  std::stringstream energy_info;
  energy_info << "Energy " << player->getCurrentEnergy()<<"/"<<player->getMaxEnergy();
  TCODConsole::root->print(x, y++, energy_info.str().c_str());
  
  std::stringstream ac_info;
  ac_info << "AC " << player->calcArmourClass();
  TCODConsole::root->print(x, y++, ac_info.str().c_str());
  //!!!
  std::stringstream str_info;
  str_info << "STR " << player->getAttribute(ATT_STR);
  TCODConsole::root->print(x, y++, str_info.str().c_str());
  
  std::stringstream wis_info;
  wis_info << "WIS " << player->getAttribute(ATT_WIS);
  TCODConsole::root->print(x, y++, wis_info.str().c_str());
  
  std::stringstream dex_info;
  dex_info << "DEX " << player->getAttribute(ATT_DEX);
  TCODConsole::root->print(x, y++, dex_info.str().c_str());
  
  std::stringstream vit_info;
  vit_info << "VIT " << player->getAttribute(ATT_VIT);
  TCODConsole::root->print(x, y++, vit_info.str().c_str());
  
  drawHorizontalLine(x, y, infoScreenDims[WIDTH], TCODColor::orange);
  TCODConsole::root->putCharEx(x - 1, y++, TCOD_CHAR_TEEE, TCODColor::orange, TCODColor::black);
}

void drawLogPanel(World *world)
{
  displayMessages();
}

char prompt(std::string message)
{
  TCODConsole::root->print(logScreenDims[X] + 2, logScreenDims[Y] - 2, message.c_str());
  updateScreen();
  TCOD_key_t input = TCODConsole::waitForKeypress(true);
  return input.c;
}

void message(std::string message)
{
  message_log.push_back(message);
  displayMessages();
}

void displayMessages()
{
  int log_size = message_log.size();
  for (int i = 0; i < NUM_MESSAGES_DISPLAYED; i++)
    {
      if (log_size - 1 - i >= 0)
	{
	  TCODConsole::root->print(logScreenDims[X], logScreenDims[Y] + i, ">");
	  TCODConsole::root->print(logScreenDims[X] + 2, logScreenDims[Y] + i, message_log[message_log.size() - 1 - i].c_str());
	}
    }
}

void drawWorld(World *world)
{
  int x_offset = worldScreenDims[X];
  int y_offset = worldScreenDims[Y];
  Player *player = world->getPlayer();
  
  //draws the tiles
  for (int i = 0; i < world->getWidth(); i++)
  {
    for (int j = 0; j < world->getHeight(); j++)
    {
      tile_t this_tile = world->getTile(i, j, world->getCurrentLevel());
      if(this_tile.visible)
      {
        TCODConsole::root->putCharEx(i + x_offset, j + y_offset, this_tile.face_tile, this_tile.color, TCODColor::black);
      }
      else if(this_tile.has_been_seen)//has seen before
      {
        TCODConsole::root->putCharEx(i + x_offset, j + y_offset, this_tile.face_tile, TCODColor::darkGrey, TCODColor::black);
      }
      else if(!this_tile.has_been_seen)//never seen
      {
        TCODConsole::root->putCharEx(i + x_offset, j + y_offset, this_tile.face_tile, TCODColor::black, TCODColor::black);
      }
    }
  }
    
  //draws the items if they are on the same level
  std::vector<Item*> items = world->getItemList();
  for (unsigned int i = 0; i < items.size(); i++)
  {
    Item *item = items[i];
    if (item->isOnGround())
    {
      if (player->canSee(world->getMapLevel(item->getMapLevel()), item->getXPosition(), item->getYPosition()))
      {
        TCODConsole::root->putCharEx(item->getXPosition() + x_offset, item->getYPosition() + y_offset, item->getFaceTile(), item->getColor(), TCODColor::black);
      }
    }
  }
    
  //draws the enemy if they are on the same level at the actor
  std::vector<Enemy*> enemies = world->getEnemyList();
  for (unsigned int i = 0; i < enemies.size(); i++)
  {
    Enemy* en = enemies[i];
    if(en->getMapLevel() == world->getCurrentLevel() && player->canSee(world->getMapLevel(en->getMapLevel()), en->getXPosition(), en->getYPosition()))
    {
      TCODConsole::root->putCharEx(en->getXPosition() + x_offset, en->getYPosition() + y_offset, en->getFaceTile(), en->getColor(), TCODColor::black);
    }
  }
  
  //draws the player
  TCODConsole::root->putCharEx(player->getXPosition() + x_offset, player->getYPosition()+ y_offset, '@', TCODColor::white, TCODColor::black);
}

//void displayWorld(World *world, bool centered)
//{
//  Player *player = world->getPlayer();
//  if(!centered)
//  {
//    Display all the tiles of the world.
//    for (int i = 0; i < world->getWidth(); i++)
//      for (int j = 0; j < world->getHeight(); j++)
//      {
//        For every tile on the map; if player can see it it hass full color
//        If the player has seen it before but cannot see it now, make grey
//        If the player has never seen it, make it black.
//        tile_t this_tile = world->getTile(i, j, world->getCurrentLevel());
//        if(this_tile.visible)
//        {
//          TCODConsole::root->putCharEx(i, j, this_tile.face_tile, this_tile.color, TCODColor::black);
//        }
//        else if(this_tile.has_been_seen)has seen before
//        {
//          TCODConsole::root->putCharEx(i, j, this_tile.face_tile, TCODColor::darkGrey, TCODColor::black);
//        }
//        else if(!this_tile.has_been_seen)never seen
//        {
//          TCODConsole::root->putCharEx(i, j, this_tile.face_tile, TCODColor::black, TCODColor::black);
//        }

//      }
//      std::vector<Item*> items = world->getItemList();
//      for (unsigned int i = 0; i < items.size(); i++)
//      {
//        Item *item = items[i];
//        TCODConsole::root->putCharEx(item->getXPosition(), item->getYPosition(), item->getFaceTile(), item->getColor(), TCODColor::black);
//      }

//      draws the enemy if they are on the same level at the actor
//      std::vector<Enemy*> enemies = world->getEnemyList();
//      for (unsigned int i = 0; i < enemies.size(); i++)
//      {
//        Enemy* en = enemies[i];
//        if(en->getMapLevel() == world->getCurrentLevel() && player->canSee(world->getMapLevel(en->getMapLevel()), en->getXPosition(), en->getYPosition()))
//        {
//          TCODConsole::root->putCharEx(en->getXPosition(), en->getYPosition(), en->getFaceTile(), en->getColor(), TCODColor::black);
//        }
//      }

//    Display the player
//    TCODConsole::root->putCharEx(player->getXPosition(), player->getYPosition(), '@', TCODColor::white, TCODColor::black);
//  }
//  else IF THE FOCUS IS CENTERED ON THE PLAYER
//  {
//    int action_window_width = 80;
//    int action_window_height = 50;
//    int center_x = action_window_width/2;
//    int center_y = action_window_height/2;
//    int player_x = world->getPlayer()->getXPosition();
//    int player_y = world->getPlayer()->getYPosition();

//    int offset_x = player_x  - center_x;
//    int offset_y = player_y -  center_y;


//    Display all the tiles of the world. (centered)
//    for (int i = 0; i < action_window_width; i++)
//      for (int j = 0; j < action_window_height; j++)
//      {

//        if (offset_x + i < 0 || offset_x + i >= world->getWidth() - 1 || offset_y + j < 0 || offset_y + j>= world->getHeight())
//          TCODConsole::root->putCharEx(i, j, ' ', TCODColor::black, TCODColor::black);
//        else
//        {
//          tile_t this_tile = world->getTile(offset_x + i, offset_y + j, world->getCurrentLevel());
//          TCODConsole::root->putCharEx(i, j, this_tile.face_tile, this_tile.color, TCODColor::black);
//        }
//      }

//      std::vector<Enemy*> enemies = world->getEnemyList();
//      for (unsigned int i = 0; i < enemies.size(); i++)
//      {
//        TODOcheck if the player can actually see this enemy
//        if(enemies[i]->getMapLevel() == world->getCurrentLevel())
//        {
//          Enemy* en = enemies[i];
//          int x = center_x + en->getXPosition() - player_x;
//          int y = center_y + en->getYPosition() - player_y;
//          TCODConsole::root->putCharEx(x, y, en->getFaceTile(), en->getColor(), TCODColor::black);
//        }
//      }
//    Display the player
//    TCODConsole::root->putCharEx(40, 25, '@', TCODColor::white, TCODColor::black);
//  }
//}

void displayInventoryScreen(World *world)
{
  TCODConsole::root->clear();
  TCODConsole::root->print(2, 2, "Inventory");
  Inventory *inventory = world->getPlayer()->getInventory();
  std::map<char, Item*> item_map = inventory->getMap();
  std::string slots("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");
  int offset = 5;
  for (unsigned int i = 0; i < slots.size(); i++)
  {
      char slot = slots.at(i);
      Item* item = inventory->get(slot);
      if (item != NULL)
	    {
	      std::string str_slot;
	      str_slot.push_back(slot);
	      TCODConsole::root->print(3, i + offset, str_slot.c_str());
	      TCODConsole::setColorControl(TCOD_COLCTRL_1, item->getColor(), TCODColor::black);
	      TCODConsole::root->print(5, i + offset, "%c%s%c", TCOD_COLCTRL_1, item->getName().c_str(), TCOD_COLCTRL_STOP);
	     }
  }
  
  updateScreen();
  
  bool exited_screen = false;
  while (!exited_screen)
  {
    TCOD_key_t key = TCODConsole::waitForKeypress(true);
    switch(key.c)
    {
      case ESC: displayGameScreen(world); exited_screen = true; break;
    }
  }
}

void displayDropItemsScreen(World *world)
{
  TCODConsole::root->clear();
  TCODConsole::root->print(2, 2, "Which item do you want to drop?");
  Inventory *inventory = world->getPlayer()->getInventory();
  std::map<char, Item*> item_map = inventory->getMap();
  std::string slots("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");
  int offset = 5;
  
  for (unsigned int i = 0; i < slots.size(); i++)
  {
      char slot = slots.at(i);
      Item* item = inventory->get(slot);
      if (item != NULL)
	    {
	      std::string str_slot;
	      str_slot.push_back(slot);
	      TCODConsole::root->print(3, i + offset, str_slot.c_str());
	      TCODConsole::setColorControl(TCOD_COLCTRL_1, item->getColor(), TCODColor::black);
	      TCODConsole::root->print(5, i + offset, "%c%s%c", TCOD_COLCTRL_1, item->getName().c_str(), TCOD_COLCTRL_STOP);
	    }
  }
  
  updateScreen();
  
  bool exited_screen = false;
  while (!exited_screen)
  {
    TCOD_key_t key = TCODConsole::waitForKeypress(true);
    switch(key.c)
    {
      case ESC: displayGameScreen(world); exited_screen = true; break;
    default:{ exited_screen = (inventory->get(key.c) != NULL); if (exited_screen){world->getPlayer()->dropItem(inventory->get(key.c));}}
    }
  }
}

void displayWinScreen()
{
  TCODConsole::root->clear();
  TCODConsole::root->print(10, 10, "You Win!");
  updateScreen();
  TCODConsole::waitForKeypress(true);
  exit(0);
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

void drawVerticalLine(int x, int y, int height, TCODColor color)
{
  for (int i = y; i < y + height - 1 ; i++)
  {
    TCODConsole::root->putCharEx(x, i, TCOD_CHAR_VLINE, color, TCODColor::black);
  }
}

void drawHorizontalLine(int x, int y, int width, TCODColor color)
{
  for (int i = x; i < x + width - 1 ; i++)
  {
    TCODConsole::root->putCharEx(i, y, TCOD_CHAR_HLINE, color, TCODColor::black);
  }
}
