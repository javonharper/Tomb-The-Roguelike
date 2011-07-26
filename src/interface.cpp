/*
 *  File:       interface.cpp
 *  Summary:    Anything that displays on the screens is located here.
 *  Written by: Javon Harper
 */
#include "map.h"
#include "interface.h"
#include "item.h"
#include "player.h"
#include "enemy.h"

void initScreen()
{
  TCODConsole::setCustomFont("art/Alloy_curses_12x12.png", TCOD_FONT_LAYOUT_ASCII_INROW);
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

char prompt(std::string message)
{
  TCODConsole::root->print(logScreenDims[X], logScreenDims[Y], message.c_str());
  updateScreen();
  TCOD_key_t input = TCODConsole::waitForKeypress(true);
  return input.c;
}

void message(std::string message)
{
  TCODConsole::root->print(0, 49, message.c_str());
  updateScreen();
}

void displayGame(World *world)
{
  drawTitleBar();
  drawWorldScreen(world);
  drawInfoScreen(world);
  drawLogScreen(world);
}

void drawWorldScreen(World *world)
{
  drawLinedBorder(worldScreenDims[X], worldScreenDims[Y], worldScreenDims[WIDTH], worldScreenDims[HEIGHT], TCODColor::orange);
  drawWorld(world);
  //drawWorldCentered(world);
}

void drawWorld(World *world)
{
  int x_offset = worldScreenDims[X] + 1;
  int y_offset = worldScreenDims[Y] + 1;
//  int screen_width = worldScreenDims[WIDTH];
//  int screen_height = worldScreenDims[HEIGHT];
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
    
  std::vector<Item*> items = world->getItemList();
  for (unsigned int i = 0; i < items.size(); i++)
  {
    Item *item = items[i];
    TCODConsole::root->putCharEx(item->getXPosition() + x_offset, item->getYPosition() + y_offset, item->getFaceTile(), item->getColor(), TCODColor::black);
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

void drawInfoScreen(World *world)
{
  drawLinedBorder(infoScreenDims[X], infoScreenDims[Y], infoScreenDims[WIDTH], infoScreenDims[HEIGHT], TCODColor::orange);
}

void drawLogScreen(World *world)
{
  drawLinedBorder(logScreenDims[X], logScreenDims[Y], logScreenDims[WIDTH], logScreenDims[HEIGHT], TCODColor::yellow);
}

void drawTitleBar()
{
  drawLinedBorder(titleScreenDims[X], titleScreenDims[Y], titleScreenDims[WIDTH], titleScreenDims[HEIGHT], TCODColor::darkOrange);
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
//        TODO check if the player can actually see this enemy
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

void drawLinedBorder(int x, int y, int width, int height, TCODColor color)
{
  width = width - 1;
  height = height - 1;
  for (int i = x; i <= x + width; i++)
	{
	  TCODConsole::root->putCharEx(i, y, TCOD_CHAR_HLINE, color, TCODColor::black);
    TCODConsole::root->putCharEx(i, y + height, TCOD_CHAR_HLINE, color, TCODColor::black);
	}
	
  for (int j = y; j <= y + height; j++)
	{
	  TCODConsole::root->putCharEx(x, j, TCOD_CHAR_VLINE, color, TCODColor::black);
    TCODConsole::root->putCharEx(x + width, j, TCOD_CHAR_VLINE, color, TCODColor::black);
	}
	
	TCODConsole::root->putCharEx(x, y, TCOD_CHAR_NW, color, TCODColor::black);
  TCODConsole::root->putCharEx(x + width, y, TCOD_CHAR_NE, color, TCODColor::black);
	TCODConsole::root->putCharEx(x, y + height, TCOD_CHAR_SW, color, TCODColor::black);
	TCODConsole::root->putCharEx(x + width, y + height, TCOD_CHAR_SE, color, TCODColor::black);
}

void drawDoubleLinedBorder(int x, int y, int width, int height, TCODColor color)
{
  width = width - 1;
  height = height - 1;
  for (int i = x; i <= x + width; i++)
	{
	  TCODConsole::root->putCharEx(i, y, TCOD_CHAR_DHLINE, color, TCODColor::black);
    TCODConsole::root->putCharEx(i, y + height, TCOD_CHAR_DHLINE, color, TCODColor::black);
	}
	
  for (int j = y; j <= y + height; j++)
	{
	  TCODConsole::root->putCharEx(x, j, TCOD_CHAR_DVLINE, color, TCODColor::black);
    TCODConsole::root->putCharEx(x + width, j, TCOD_CHAR_DVLINE, color, TCODColor::black);
	}
	
	TCODConsole::root->putCharEx(x, y, TCOD_CHAR_DNW, color, TCODColor::black);
  TCODConsole::root->putCharEx(x + width, y, TCOD_CHAR_DNE, color, TCODColor::black);
	TCODConsole::root->putCharEx(x, y + height, TCOD_CHAR_DSW, color, TCODColor::black);
	TCODConsole::root->putCharEx(x + width, y + height, TCOD_CHAR_DSE, color, TCODColor::black);
}

void drawVerticalLine(int x, int y, int height, TCODColor color)
{
  for (int i = y; i < y + height - 1 ; i++)
  {
    TCODConsole::root->putCharEx(x, i, TCOD_CHAR_VLINE, color, TCODColor::black);
  }
}

void drawDoubleVerticalLine(int x, int y, int height, TCODColor color)
{
  for (int i = y; i < y + height - 1 ; i++)
  {
    TCODConsole::root->putCharEx(x, i, TCOD_CHAR_DVLINE, color, TCODColor::black);
  }
}

void printChar(int x, int y, char character, TCODColor foreground, TCODColor background)
{
    TCODConsole::root->putCharEx(x, y, character, foreground, background);
}

