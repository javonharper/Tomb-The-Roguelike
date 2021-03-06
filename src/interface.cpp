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
#include "class.h"

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
    drawVerticalLine(infoScreenDims[X], infoScreenDims[Y], infoScreenDims[HEIGHT], TCODColor::orange);
    int y = infoScreenDims[Y];
    int x = infoScreenDims[X] + 1;

    //TODO make this accurate
    Player *player = _world_->getPlayer();
    TCODConsole::root->print(x, y++, player->getName().c_str());

    std::stringstream class_stream;
    class_stream << "L" << player->getClass()->getClassLevel(player->getClass()->getActiveClassType()) << " " << player->getClass()->getActiveClassTypeString();
    TCODConsole::root->print(x, y++,  class_stream.str().c_str());

//  std::stringstream player_level;
//  player_level << "Level " << player->getLevel();
//  TCODConsole::root->print(x, y++,  player_level.str().c_str());

//    std::stringstream player_EXP;
//    player_EXP << "EXP " << player->getExp();
//    TCODConsole::root->print(x, y++,  player_EXP.str().c_str());

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
    ac_info << "DEFENSE " << player->calcArmourClass();
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

    std::stringstream wis_info;
    wis_info << "WIS " << player->getAttribute(ATT_WIS);
    TCODConsole::root->print(x, y++, wis_info.str().c_str());

    std::stringstream vit_info;
    vit_info << "VIT " << player->getAttribute(ATT_VIT);
    TCODConsole::root->print(x, y++, vit_info.str().c_str());

    drawHorizontalLine(x, y, infoScreenDims[WIDTH], TCODColor::orange);
    TCODConsole::root->putCharEx(x - 1, y++, TCOD_CHAR_TEEE, TCODColor::orange, TCODColor::black);

    std::stringstream wep_info;
    Item *wep = player->getWeapon();
    wep_info << "Wpn: " ;
    if (wep != NULL)
    {
        wep_info << wep->getName();
    }
    TCODConsole::root->print(x, y++, wep_info.str().c_str());

    std::stringstream arm_info;
    Item *arm = player->getBodyArmour();
    arm_info << "Amr: ";
    if (arm != NULL)
    {
        arm_info << arm->getName();
    }
    TCODConsole::root->print(x, y++, arm_info.str().c_str());

    //drawHorizontalLine(x, y, infoScreenDims[WIDTH], TCODColor::orange);
    //TCODConsole::root->putCharEx(x - 1, y++, TCOD_CHAR_TEEE, TCODColor::orange, TCODColor::black);


//    //draws the enemy if they are on the same level at the actor
//    std::vector<Enemy*> enemies = _world_->getEnemyList();
//    for (unsigned int i = 0; i < enemies.size(); i++)
//    {
//      Enemy* en = enemies[i];
//      if(en->getMapLevel() == _world_->getCurrentLevel() && player->canSee(en->getMapLevel(), en->getXPosition(), en->getYPosition()))
//      {
//        showEnemyStatus(x, y, en);
//      }
//   }
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
    drawHorizontalLine(logScreenDims[X], logScreenDims[Y] - 1, logScreenDims[WIDTH], TCODColor::orange);
    TCODConsole::root->putCharEx(worldScreenDims[WIDTH], worldScreenDims[HEIGHT] - 1, TCOD_CHAR_TEEN, TCODColor::orange, TCODColor::black);
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
    drawMap();
    drawItems();
    drawEnemies();
    //draws the player
    TCODConsole::root->putCharEx(player->getXPosition() + x_offset, player->getYPosition()+ y_offset, '@', TCODColor::white, TCODColor::black);
}

void drawMap()
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
            if (player->canSee(player->getMapLevel(), i, j))
            {
                TCODConsole::root->putCharEx(i + x_offset, j + y_offset, this_tile.face_tile, this_tile.color, TCODColor::black);
                _world_->setTileAsSeen(i,j, player->getMapLevel());
            }
            //else //uncomment to see whole map, but greyed out
            else if(this_tile.has_been_seen)
            {
                TCODConsole::root->putCharEx(i + x_offset, j + y_offset, this_tile.face_tile, TCODColor::darkGrey, TCODColor::black);
            }
        }
    }
}

void drawItems()
{
    int x_offset = worldScreenDims[X];
    int y_offset = worldScreenDims[Y];
    Player *player = _world_->getPlayer();
    //draws the items if they are on the same level
    std::vector<Item*> items = _world_->getItemList();
    for (unsigned int i = 0; i < items.size(); i++)
    {
        Item *item = items[i];
        if (item->isOnGround())
        {
            if (item->getMapLevel() == _world_->getCurrentLevel())
            {
                if (player->canSee(item->getMapLevel(), item->getXPosition(), item->getYPosition()))
                {

                    TCODConsole::root->putCharEx(item->getXPosition() + x_offset, item->getYPosition() + y_offset, item->getFaceTile(), item->getColor(), TCODColor::black);
                }
            }
        }
    }
}

void drawEnemies()
{
    int x_offset = worldScreenDims[X];
    int y_offset = worldScreenDims[Y];
    Player *player = _world_->getPlayer();
    //draws the enemy if they are on the same level at the actor
    std::vector<Enemy*> enemies = _world_->getEnemyList();
    for (unsigned int i = 0; i < enemies.size(); i++)
    {
        Enemy* en = enemies[i];
        if (en->getMapLevel() == _world_->getCurrentLevel())
        {
            if (player->canSee(en->getMapLevel(), en->getXPosition(), en->getYPosition()))
            {
                TCODConsole::root->putCharEx(en->getXPosition() + x_offset, en->getYPosition() + y_offset, en->getFaceTile(), en->getColor(), TCODColor::black);
            }
        }
    }
}

void displayInventoryScreen()
{
    bool exited_screen = false;
    while (!exited_screen)
    {
        TCODConsole::root->clear();
        TCODConsole::root->print(2, 2, "Inventory");
        TCODConsole::root->print(3, 3, "What will you do?  (A)pply or use - (D)rop - (I)nspect");
        showInventoryContents();

        TCOD_key_t key = TCODConsole::waitForKeypress(true);
        switch (key.c)
        {
        case USE_ITEM:
            exited_screen = displayUseItemScreen();
            break;
        case DROP_ITEM:
            exited_screen = displayDropItemsScreen();
            break;
        case INSPECT_ITEM:
            exited_screen = displayInspectItemsScreen();
            break;
        case ESC:
            displayGameScreen();
            exited_screen = true;
            updateScreen();
            break;
        }
    }
}

bool displayDropItemsScreen()
{
    TCODConsole::root->clear();
    TCODConsole::root->print(2, 2, "Inventory");
    TCODConsole::root->print(3, 3, "Which item do you want to drop?");
    showInventoryContents();
    updateScreen();

    bool exited_screen = false;
    while (!exited_screen)
    {
        TCOD_key_t key = TCODConsole::waitForKeypress(true);
        switch (key.c)
        {
        case ESC:
            exited_screen = true;
            return false;
        default:
        {
            exited_screen = (_world_->getPlayer()->getInventory()->get(key.c) != NULL);
            if (exited_screen)
            {
                _world_->getPlayer()->dropItem(_world_->getPlayer()->getInventory()->get(key.c));
            }
            return true;
        }
        }
    }

    return exited_screen;
}

//TODO add descs to items
bool displayInspectItemsScreen()
{
    TCODConsole::root->clear();
    TCODConsole::root->print(2, 2, "Inventory");
    TCODConsole::root->print(3, 3, "Which item do you want to inspect?  [!!NOT IMPLEMENTED!!]");
    showInventoryContents();
    updateScreen();

    bool exited_screen = false;
    while (!exited_screen)
    {
        TCOD_key_t key = TCODConsole::waitForKeypress(true);
        switch (key.c)
        {
        case ESC:
            exited_screen = true;
            return false;
        }
    }
    return exited_screen;
}

bool displayUseItemScreen()
{
    TCODConsole::root->clear();
    TCODConsole::root->print(2, 2, "Inventory");
    TCODConsole::root->print(3, 3, "Which item do you want to use?");
    showInventoryContents();

    bool exited_screen = false;
    while (!exited_screen)
    {
        TCOD_key_t key = TCODConsole::waitForKeypress(true);
        switch (key.c)
        {
        case ESC:
            exited_screen = true;
            break;
        default:
        {
            exited_screen = (_world_->getPlayer()->getInventory()->get(key.c) != NULL);
            if (exited_screen)
            {
                _world_->getPlayer()->useItemAction(_world_->getPlayer()->getInventory()->get(key.c));
            }
            break;
        }
        }
    }

    return exited_screen;
}

void showInventoryContents()
{
    Inventory *inventory = _world_->getPlayer()->getInventory();
    std::map<char, Item*> item_map = inventory->getMap();
    int offset = 5;
    int count = 0;
    for (unsigned int i = 0; i < inventory_slots.size(); i++)
    {
        char slot = inventory_slots.at(i);
        Item* item = inventory->get(slot);
        if (item != NULL)
        {
            std::string str_slot;
            str_slot.push_back(slot);
            TCODConsole::root->print(4, count + offset, str_slot.c_str());
            TCODConsole::setColorControl(TCOD_COLCTRL_1, item->getColor(), TCODColor::black);
            std::string being_worn = "";
            if (_world_->getPlayer()->hasEquipped(item))
            {
                being_worn = item->getEquippedString();
            }
            TCODConsole::root->print(6, count++ + offset, "%c%s%c %s", TCOD_COLCTRL_1, item->getName().c_str(), TCOD_COLCTRL_STOP, being_worn.c_str());
        }
    }
    updateScreen();
}

void displayChangeClassScreen()
{
    TCODConsole::root->clear();
    TCODConsole::root->print(2, 2, "Change class");
    TCODConsole::root->print(3, 3, "Press the corresponding letter to change class or Esc to leave this screen");

    for (int i = 0; i < NUM_CLASS_TYPES; i++)
    {
        TCODConsole::root->print(3, 5 + i, class_data[i].class_letter.c_str());
        TCODConsole::root->print(5, 5 + i, class_data[i].class_string.c_str());
        std::stringstream level_stream;
        int level = _world_->getPlayer()->getClass()->getClassLevel(i);
        if(level == 0)
        {
            level_stream << "Not Accessible";
        }
        else
        {
            level_stream << "Level " << level;
        }

        TCODConsole::root->print(20, 5 + i, level_stream.str().c_str());
    }
    updateScreen();

    Class * pclass = _world_->getPlayer()->getClass();
    bool exited_screen = false;
    while (!exited_screen)
    {
        TCOD_key_t key = TCODConsole::waitForKeypress(true);

        int class_int = -1;
        if (key.c != ESC)
        {
            class_int = charToClassInt(key.c);
        }
        else
        {
            exited_screen = true;
        }

        switch (class_int)
        {
        case CLASS_ADVENTURER:
            exited_screen = true;
            pclass->changeClass(CLASS_ADVENTURER);
            break;
        case CLASS_FIGHTER:
            exited_screen = true;
            pclass->changeClass(CLASS_FIGHTER);
            break;
        case CLASS_MAGICIAN:
            exited_screen = true;
            pclass->changeClass(CLASS_MAGICIAN);
            break;
        case CLASS_PRIEST:
            exited_screen = true;
            pclass->changeClass(CLASS_PRIEST);
            break;
        case CLASS_ROGUE:
            exited_screen = true;
            pclass->changeClass(CLASS_ROGUE);
            break;
        }
    }
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
    TCODConsole::root->print(4, y++, "k - Move north");
    TCODConsole::root->print(4, y++, "j - Move south");
    TCODConsole::root->print(4, y++, "l - Move east");
    TCODConsole::root->print(4, y++, "h - Move west");
    TCODConsole::root->print(4, y++, "u - Move northeast");
    TCODConsole::root->print(4, y++, "y - Move northwest");
    TCODConsole::root->print(4, y++, "n - Move southeast");
    TCODConsole::root->print(4, y++, "b - Move south west");
    TCODConsole::root->print(4, y++, ". - Rest");
    TCODConsole::root->print(4, y++, "< - Go up stairs");
    TCODConsole::root->print(4, y++, "> - Go down stairs");
    TCODConsole::root->print(4, y++, "c - Close a door");
    TCODConsole::root->print(4, y++, "o - Open a door");
    TCODConsole::root->print(4, y++, "g - get an item off the floor");
    TCODConsole::root->print(4, y++, "i - show your inventory");
    TCODConsole::root->print(4, y++, "C - change character class");
    TCODConsole::root->print(4, y++, "? - show in-game help");
    TCODConsole::root->print(4, y++, "Q - quit the game");
    updateScreen();
    TCODConsole::waitForKeypress(true);
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
