/*
 *  File:       interface.cpp
 *  Summary:    Anything that displays on the screens is located here.
 *  Written by: Javon Harper
 */
#include <map>
#include <sstream>
#include <vector>
#include <iostream>
#include "map.h"
#include "interface.h"
#include "inventory.h"
#include "item.h"
#include "player.h"
#include "enemy.h"
#include "controller.h"

std::vector<std::string> message_log;
void setWorld(World *world)
{
    _world_ = world;
}

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

void displayGameScreen()
{
    TCODConsole::root->clear();
    drawWorldPanel();
    drawInfoPanel();
    drawLogPanel();
}

void drawWorldPanel()
{
    drawWorld();
}

void drawInfoPanel()
{
    drawVerticalLine(infoScreenDims[X], infoScreenDims[Y], SCREEN_HEIGHT + 1, TCODColor::orange);
    int y = infoScreenDims[Y];
    int x = infoScreenDims[X] + 1;

    //TODO make this accurate
    Player *player = _world_->getPlayer();

    TCODConsole::root->print(x, y++, player->getName().c_str());

    std::stringstream player_level;
    player_level << "Level " << player->getLevel();
    TCODConsole::root->print(x, y++,  player_level.str().c_str());

    std::stringstream player_EXP;
    player_EXP << "EXP " << player->getExp();
    TCODConsole::root->print(x, y++,  player_EXP.str().c_str());

    std::stringstream d_level;
    d_level << "Tomb Level " << player->getMapLevel() + 1;
    TCODConsole::root->print(x, y++,  d_level.str().c_str());

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

    std::stringstream str_info;
    str_info << "STR " << player->getAttribute(ATT_STR);
    TCODConsole::root->print(x, y++, str_info.str().c_str());

    std::stringstream int_info;
    int_info << "INT " << player->getAttribute(ATT_INT);
    TCODConsole::root->print(x, y++, int_info.str().c_str());

    std::stringstream dex_info;
    dex_info << "DEX " << player->getAttribute(ATT_DEX);
    TCODConsole::root->print(x, y++, dex_info.str().c_str());

    std::stringstream vit_info;
    vit_info << "VIT " << player->getAttribute(ATT_VIT);
    TCODConsole::root->print(x, y++, vit_info.str().c_str());

    drawHorizontalLine(x, y, infoScreenDims[WIDTH], TCODColor::orange);
    TCODConsole::root->putCharEx(x - 1, y++, TCOD_CHAR_TEEE, TCODColor::orange, TCODColor::black);
  
    //draws the enemy if they are on the same level at the actor
    std::vector<Enemy*> enemies = _world_->getEnemyList();
    for (unsigned int i = 0; i < enemies.size(); i++)
    {
	Enemy* en = enemies[i];
	if(en->getMapLevel() == _world_->getCurrentLevel() && player->canSee(en->getMapLevel(), en->getXPosition(), en->getYPosition()))
	{
            
	    showEnemyStatus(x, y, en);
	}
    }
}

void showEnemyStatus(int &x, int &y, Enemy *enemy)
{

    TCODConsole::setColorControl(TCOD_COLCTRL_1, enemy->getColor(), TCODColor::black);
    TCODConsole::root->print(x, y++, "%c%c%c  %s", TCOD_COLCTRL_1, enemy->getFaceTile(), TCOD_COLCTRL_STOP, enemy->getName().c_str());
    std::stringstream health_info;
    health_info << "Health " << enemy->getCurrentHealth()<<"/"<<enemy->getMaxHealth();
    TCODConsole::root->print(x, y++, health_info.str().c_str());
    y++;
}

void drawLogPanel()
{
    displayMessages();
}

char prompt(std::string message)
{
    message_log.push_back(message);
    displayGameScreen();
    updateScreen();
    TCOD_key_t input = TCODConsole::waitForKeypress(true);
    return input.c;
}

void message(std::string message)
{
    message_log.push_back(message);
    displayGameScreen();
    updateScreen();
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

void drawWorld()
{
    int x_offset = worldScreenDims[X];
    int y_offset = worldScreenDims[Y];
    Player *player = _world_->getPlayer();

    //draws the tiles
    for (int i = 0; i < _world_->getWidth(); i++)
    {
	for (int j = 0; j < _world_->getHeight(); j++)
	{
	    tile_t this_tile = _world_->getTile(i, j, player->getMapLevel());
	     if(player->canSee(player->getMapLevel(), i, j))
	    {
		TCODConsole::root->putCharEx(i + x_offset, j + y_offset, this_tile.face_tile, this_tile.color, TCODColor::black);
		_world_->setTileAsSeen(i,j, player->getMapLevel());
	    }
	    else if(this_tile.has_been_seen)//has seen before
	    {
		TCODConsole::root->putCharEx(i + x_offset, j + y_offset, this_tile.face_tile, TCODColor::darkGrey, TCODColor::black);
	    }
	}
    }

    //draws the items if they are on the same level
    std::vector<Item*> items = _world_->getItemList();
    for (unsigned int i = 0; i < items.size(); i++)
    {
	Item *item = items[i];
	if (item->isOnGround())
	{
	    if (player->canSee(item->getMapLevel(), item->getXPosition(), item->getYPosition()))
	    {
		TCODConsole::root->putCharEx(item->getXPosition() + x_offset, item->getYPosition() + y_offset, item->getFaceTile(), item->getColor(), TCODColor::black);
	    }
	}
    }

    //draws the enemy if they are on the same level at the actor
    std::vector<Enemy*> enemies = _world_->getEnemyList();
    for (unsigned int i = 0; i < enemies.size(); i++)
    {
	Enemy* en = enemies[i];
	if(en->getMapLevel() == _world_->getCurrentLevel() && player->canSee(en->getMapLevel(), en->getXPosition(), en->getYPosition()))
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

void displayInventoryScreen()
{
    TCODConsole::root->clear();
    TCODConsole::root->print(2, 2, "Inventory");
    showInventoryContents();
    bool exited_screen = false;
    while (!exited_screen)
    {
	TCOD_key_t key = TCODConsole::waitForKeypress(true);
	switch(key.c)
	{
	case ESC: displayGameScreen(); exited_screen = true; updateScreen(); break;
	}
    }
}

void showInventoryContents()
{
    Inventory *inventory = _world_->getPlayer()->getInventory();
    std::map<char, Item*> item_map = inventory->getMap();
    std::string slots("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");
    int offset = 5;
    int count = 0;
    for (unsigned int i = 0; i < slots.size(); i++)
    {
	char slot = slots.at(i);
	Item* item = inventory->get(slot);
	if (item != NULL)
	{
	    std::string str_slot;
	    str_slot.push_back(slot);
	    TCODConsole::root->print(3, count + offset, str_slot.c_str());
	    TCODConsole::setColorControl(TCOD_COLCTRL_1, item->getColor(), TCODColor::black);
	    std::string being_worn = "";
	    if (_world_->getPlayer()->hasEquipped(item))
	    {
		being_worn = item->getEquippedString();
	    }
	    TCODConsole::root->print(5, count++ + offset, "%c%s%c %s", TCOD_COLCTRL_1, item->getName().c_str(), TCOD_COLCTRL_STOP, being_worn.c_str());
	}
    }
    updateScreen();
}

void displayDropItemsScreen()
{
    TCODConsole::root->clear();
    TCODConsole::root->print(2, 2, "Which item do you want to Drop?");
    showInventoryContents();
    updateScreen();

    bool exited_screen = false;
    while (!exited_screen)
    {
	TCOD_key_t key = TCODConsole::waitForKeypress(true);
	switch(key.c)
	{
	case ESC: displayGameScreen(); exited_screen = true; updateScreen(); break;
	default:{ exited_screen = (_world_->getPlayer()->getInventory()->get(key.c) != NULL); if (exited_screen){_world_->getPlayer()->dropItem(_world_->getPlayer()->getInventory()->get(key.c));}}
	}
    }
}

Item* displayUseItemScreen()
{
    TCODConsole::root->clear();
    TCODConsole::root->print(2, 2, "Which item do you want to use");
    showInventoryContents();

    bool exited_screen = false;
    while (!exited_screen)
    {
	TCOD_key_t key = TCODConsole::waitForKeypress(true);
	switch(key.c)
	{
	case ESC: displayGameScreen(); exited_screen = true; updateScreen(); return NULL; break;
	default:{ exited_screen = (_world_->getPlayer()->getInventory()->get(key.c) != NULL); if (exited_screen){return _world_->getPlayer()->getInventory()->get(key.c);}}
	}
    }
    return NULL;
}

void displayWinScreen(std::string reason)
{
    TCODConsole::root->clear();
    TCODConsole::root->print(10, 10, "You Win!");
    TCODConsole::root->print(12, 11, reason.c_str());
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

void displayHelpScreen()
{
    TCODConsole::root->clear();
    int y = 2;
    TCODConsole::root->print(2, y++, "Tomb keybindings");
    TCODConsole::root->print(4, y++, "k Move north"); 
    TCODConsole::root->print(4, y++, "j Move south");
    TCODConsole::root->print(4, y++, "l Move east");
    TCODConsole::root->print(4, y++, "h Move west");
    TCODConsole::root->print(4, y++, "u Move northeast"); 
    TCODConsole::root->print(4, y++, "y Move northwest");
    TCODConsole::root->print(4, y++, "n Move southeast");
    TCODConsole::root->print(4, y++, "b Move south west");
    TCODConsole::root->print(4, y++, ". Rest"); 
    TCODConsole::root->print(4, y++, "< Go up stairs");
    TCODConsole::root->print(4, y++, "> Go down stairs");
    TCODConsole::root->print(4, y++, "c Close a door");
    TCODConsole::root->print(4, y++, "o Open a door"); 
    TCODConsole::root->print(4, y++, "d Drop an item");
    TCODConsole::root->print(4, y++, "g get an item off the floor");
    TCODConsole::root->print(4, y++, "i show your inventory");
    TCODConsole::root->print(4, y++, "a active or use an item");   
    TCODConsole::root->print(4, y++, "? show in-game help");  
    TCODConsole::root->print(4, y++, "Esc quit the game");  
    updateScreen();
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
