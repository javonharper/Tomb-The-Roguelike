/*
 *  File:       player.cpp
 *  Summary:    Our hero
 *  Written by: Javon Harper
 */
#include <cmath>
#include <iostream>
#include <sstream>
#include <string>
#include "controller.h"
#include "enemy.h"
#include "functions.h"
#include "interface.h"
#include "inventory.h"
#include "item_db.h"
#include "map.h"
#include "random.h"
#include "world.h"
#include "player.h"
#include "class.h"

Player::Player(World *world)
{
    initProperties(enemy_db[PLAYER], world);
    level_ = 1;
    experience_ = 0;
    is_alive_ = true;
    is_player_ = true;

    TCODNamegen::parse("data/names.txt");
    name_ = TCODNamegen::generate((char*)"player", false);
    TCODNamegen::destroy();

    class_ = new Class();

    att_str_ = random(ATT_AVERAGE, ATT_GOOD);
    att_int_ = random(ATT_AVERAGE, ATT_GOOD);
    att_dex_ = random(ATT_AVERAGE, ATT_GOOD);
    att_wis_ = random(ATT_AVERAGE, ATT_GOOD);
    att_vit_ = random(ATT_AVERAGE, ATT_GOOD);

    max_health_points_ = current_health_points_ = att_vit_ + random(1, 10);
    max_energy_points_ = current_energy_points_ = att_int_ + random(1, 10);

    active_weapon_ = new Item(weapon_db[TYPE_WOODEN_CLUB], world);
    active_body_armour_ = new Item(armour_db[ TYPE_SILK_TUNIC], world);
    inventory_->add(active_weapon_);
    inventory_->add(active_body_armour_);
    inventory_->add(new Item(potion_db[TYPE_CURE_LIGHT_WOUNDS], world));
    inventory_->add(new Item(potion_db[TYPE_CURE_LIGHT_WOUNDS], world));
    inventory_->add(new Item(potion_db[TYPE_CURE_LIGHT_WOUNDS], world));
}

Player::~Player()
{
}

bool Player::isEnemyAtPosition(int x, int y, int level)
{
    bool found_enemy = false;
    Enemy *enemy = world_->getEnemyAt(x, y, level);
    if (enemy != NULL)
    {
        found_enemy = true;
        meleeAttack(enemy);
        if (enemy->getCurrentHealth() == 0)
        {
            addExp(enemy->getExpReward());
        }
    }
    return found_enemy;
}

void Player::kill()
{
    is_alive_ = false;
    setCurrentHealth(0);
}

bool Player::isAlive()
{
    return is_alive_;
}

void Player::openDoorAction()
{
    char input = prompt("In what direction?");
    direction_t direction = charToDirection(input);
    tile_t tile = world_->getTile(x_ + direction.x, y_ + direction.y, map_level_);

    if (!isValidDirection(direction))
    {
        message("Invalid Key");
    }
    else
    {
        if (world_->getEnemyAt(x_ + direction.x, y_ + direction.y, map_level_ ) != NULL)
        {
            message("Something is standing there");
        }
        else if (tile.tile_type == TILE_DOOR_CLOSED)
        {
            openDoor(x_ + direction.x, y_ + direction.y, map_level_);
        }
        else if (tile.tile_type == TILE_DOOR_OPEN)
        {
            message("That door is already open");
        }
        else
        {
            message("You see no door there");
        }
    }
}

void Player::closeDoorAction()
{
    char input = prompt("In what direction?");
    direction_t direction = charToDirection(input);
    tile_t tile = world_->getTile(x_ + direction.x, y_ + direction.y, map_level_);

    if (!isValidDirection(direction))
    {
        message("Invalid Key");
    }
    else
    {
        if (world_->getEnemyAt(x_ + direction.x, y_ + direction.y, map_level_ ) != NULL)
        {
            message("Something is standing there");
        }
        else if (tile.tile_type == TILE_DOOR_OPEN)
        {
            openDoor(x_ + direction.x, y_ + direction.y, map_level_);
        }
        else if (tile.tile_type == TILE_DOOR_CLOSED)
        {
            message("That door is already open");
        }
        else
        {
            message("You see no door there");
        }
    }
}

void Player::ascendStairsAction()
{
    tile_t tile = world_->getTile(x_, y_, map_level_);
    if (tile.tile_type == TILE_UPSTAIR)
    {
        if (map_level_ == 0)
        {
            if (!hasVictoryItem())
            {
                char result = prompt("Are you sure you want to go to the surface? (y)es/(n)o");
                if (result == YES)
                {
                    displayGameOverScreen("You arrive at the surface embarrased and without the icon");
                }
                else
                {
                    message("Okay, then.");
                }
            }
            else
            {
                displayWinScreen("Congratulations! You have braved the tomb!");
            }
        }
        else
        {
            ascendStairs();
        }
    }
}

void Player::descendStairsAction()
{
    descendStairs();
}

void Player::pickupItemAction()
{
    std::vector<Item*> items = world_->getItemsAt(x_, y_, map_level_);
    if (items.size() == 1)
    {
        char free_slot = pickUpItem(items[0]);
        std::stringstream new_item_stream;
        new_item_stream << free_slot << " - " << items[0]->getName();
        message(new_item_stream.str());
    }
    else if (items.size() > 1)
    {
        for (unsigned int i = 0; i < items.size();i++ )
        {
            std::stringstream prompt_stream;
            prompt_stream << "Pick up the " << items[i]->getName() << "? (y)es/(n)o";
            if (prompt(prompt_stream.str()) == YES)
            {
                char free_slot = pickUpItem(items[i]);
                std::stringstream new_item_stream;
                new_item_stream << free_slot << " - " << items[i]->getName();
                message(new_item_stream.str());
            }
        }
    }
    else
    {
        message("There are no items here");
    }
}

void Player::restAction()
{
    rest();
}

void Player::useItemAction(Item *item)
{
    if (item != NULL)
    {
        std::stringstream use_stream;
        use_stream << "you ";

        switch (item->getCategory())
        {
        case CATEGORY_WEAPON:
            use_stream << "wield";
            break;
        case CATEGORY_BODY_ARMOUR:
            use_stream << "put on";
            break;
        case CATEGORY_POTION:
            use_stream << "drink the";
            break;
        default:
            use_stream << "ERROR: malformed item category on use";
            break;
        }

        use_stream << " the " << item->getName();

        displayGameScreen();
        message(use_stream.str());
        useItem(item);
    }
}

void Player::moveAction(int x, int y, int z)
{
    move(x,y,z);
    if (x_ == x && y_ == y && map_level_ == z)//if they actually moved there
    {
        std::vector<Item*> items = world_->getItemsAt(x_, y_, map_level_);
        if (items.size() == 1)
        {
            std::stringstream ground_stream;
            ground_stream << "You see a " << items[0]->getName() << " here.";
            message(ground_stream.str());
            updateScreen();
        }
        else if (items.size() > 1)
        {
            message("You see many items here");
        }
    }
}

void Player::changeClassAction()//Lawsuit dohoho
{
    int class_type = class_->getActiveClassType();
    displayChangeClassScreen();

    if (class_type != class_->getActiveClassType())
    {
        std::stringstream notify_stream;
        notify_stream << "You are now a " << class_->getActiveClassTypeString();
        message(notify_stream.str());
        turn_finished_ = true;
    }
}

void Player::addExp(int exp)
{
    experience_ = experience_ + exp;
    checkForLevelUp();
}

void Player::checkForLevelUp()
{
    bool done = false;
    while (!done)
    {
        int level = findLevelByExp(experience_);
        if (level > level_)
        {
            levelUp();
        }
        else
        {
            done = true;
        }
    }
}

void Player::levelUp()
{
    level_++;
    int to_class_level = class_->levelUp();
    std::stringstream levelup_stream;
    levelup_stream << "You are now a level " << to_class_level << " " << class_->getActiveClassTypeString();
    message(levelup_stream.str());

    bool made_selection = false;
    while (!made_selection)
    {
        char result = prompt("Increase (S)trength, (D)exterity, (I)ntelligence, (W)isdom, or (V)itality?");
        switch (tolower(result))
        {
        case STRENGTH:
            att_str_++;
            message("You feel stronger");
            made_selection = true;
            break;
        case DEXTERITY:
            att_dex_++;
            message("You feel more nimble");
            made_selection = true;
            break;
        case INTELLIGENCE:
            att_int_++;
            message("You feel more intelligent");
            made_selection = true;
            break;
        case WISDOM:
            att_wis_++;
            message("You feel wise");
            made_selection = true;
            break;
        case VITALITY:
            att_vit_++;
            message("You feel healthier and more vibrant");
            made_selection = true;
            break;
        }
    }

    current_health_points_ = max_health_points_ = max_health_points_ + random(1, att_vit_);
    current_energy_points_ = max_energy_points_ = max_energy_points_ + random(1, att_wis_);
}

int Player::findLevelByExp(int exp)
{
    for (unsigned int i = 0; i < sizeof(experience_thresholds)/sizeof(int); i++)
    {
        if (exp < experience_thresholds[i])
        {
            return i;
        }
    }
    return 0;
}

Class *Player::getClass()
{
    return class_;
}

int Player::getLevel()
{
    return level_;
}

int Player::getExp()
{
    return experience_;
}















