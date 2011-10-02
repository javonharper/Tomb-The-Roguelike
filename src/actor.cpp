/*
 *  File:       actor.cpp
 *  Summary:    holds the general functionality of the player and his/her enemies.
 *  Written by: Javon Harper
 */
#include <cmath>
#include <iostream>
#include <sstream>
#include "actor.h"
#include "controller.h"
#include "functions.h"
#include "interface.h"
#include "item.h"
#include "item_db.h"
#include "inventory.h"
#include "random.h"
#include "world.h"

Actor::Actor(){}
Actor::~Actor(){}

void Actor::initProperties(enemy_data_t data, World *world)
{
    x_ = -1;
    y_ = -1;
    world_ = world;
    map_level_ = -1;
    initVisionMap();

    name_ = data.name;
    face_tile_ = data.face_tile;
    color_ = data.color;

    sight_range_ = data.sight_range;

    mod_speed_ = data.mod_speed;
    mod_size_ = data.mod_size;
    mod_base_ac_ = data.mod_base_ac;

    unarmed_damage_[0] = data.unarmed_damage[0];
    unarmed_damage_[1] = data.unarmed_damage[1];

    //60 is the lcm of the different speed 1, 2, 3, 4, 5
    //actors with normal speed have a multiplier of 1
    //therefore, we 20 by the speed to set the multilpier to 1
    //with a speed of 3 and everything else follows
    damage_multiplier_ = 60/20/(double)(6 - data.mod_speed);
    //std::cout << damage_multiplier_ << std::endl;

    inventory_ = new Inventory();
    for (int i = 0; i < NUM_EQUIPPED_SLOTS; i++)
    {
        equipped_items_[i] = NULL;
    }

    current_health_points_ = max_health_points_ = data.base_health;
    current_energy_points_ = max_energy_points_ = data.base_energy;

    att_str_ = data.att_str;
    att_dex_ = data.att_dex;
    att_int_ = data.att_int;
    att_wis_ = data.att_wis;
    att_vit_ = data.att_vit;

    turn_finished_ = true;
    next_turn_ = calcSpeed();
}

void Actor::initVisionMap()
{
    for (int z = 0; z < world_->getLevels(); z++)
    {
        TCODMap *map = new TCODMap(world_->getWidth(), world_->getHeight());
        for (int y = 0; y < world_->getHeight(); y++)
        {
            for (int x = 0; x < world_->getWidth(); x++)
            {
                tile_t tile = world_->getTile(x,y,z);
                map->setProperties(x, y, tile.is_passable, tile.is_passable);
            }
        }
        vision_map_.push_back(map);
    }
}

void Actor::move(int x, int y, int level)
{
    //In the case there is nothing there, move there.
    //is enemy at position method will delegate attacking the actor to the subclass.
    bool walking_into_enemy = isEnemyAtPosition(x, y, level);
    if (walking_into_enemy)
    {
        turn_finished_ = true;
    }
    //if nothing is there, go there and end the turn.
    else if (world_->getTile(x, y, level).is_passable && world_->getEnemyAt(x, y, level) == NULL)
    {
        setPosition(x, y, level);
        turn_finished_ = true;
    }
    //If a closed door is there, automatically open it and end the turn.
    else if (world_->getTile(x, y, level).tile_type == TILE_DOOR_CLOSED)//
    {
        this->openDoor(x, y, level);
    }
}

void Actor::moveTowards(int x, int y)
{
    vision_map_[map_level_]->setProperties(x_, y_, true, true);
    TCODPath *path = new TCODPath(vision_map_[map_level_]);
    path->compute(x_, y_, x, y);
    if (! path->isEmpty())
    {
        int x,y;
        path->get(0, &x, &y);
        move(x, y, map_level_);
    }
}

void Actor::wander()
{
    move(random(x_ - 1, x_ + 1), random(y_ - 1, y_ + 1), map_level_);
}

void Actor::rest()
{
    turn_finished_ = true;
}

//If the door at the coordinates is a closed door, open.
void Actor::openDoor(int x, int y, int z)
{
    if (world_->getTile(x, y, z).tile_type == TILE_DOOR_CLOSED)
    {
        world_->setTile(x, y, z, TILE_DOOR_OPEN);
        turn_finished_ = true;
    }
}

//if the door at the coordinates is an open door, close it
void Actor::closeDoor(int x, int y, int z)
{
    if (world_->getTile(x, y, z).tile_type == TILE_DOOR_OPEN)
    {
        world_->setTile(x, y, z, TILE_DOOR_CLOSED);
        turn_finished_ = true;
    }
}

//If the player is on top of a down stair, let him go to the next level and place him on the upward stair there.
void Actor::descendStairs()
{
    tile_t tile = world_->getTile(x_, y_, map_level_);

    if (tile.tile_type == TILE_DOWNSTAIR)
    {
        world_->setCurrentLevel(world_->getCurrentLevel() + 1);
        position_t stair_pos = world_->getMapLevel(world_->getCurrentLevel())->getUpStairPos();
        setPosition(stair_pos.x, stair_pos.y, world_->getCurrentLevel());
        turn_finished_ = true;
    }
}

void Actor::ascendStairs()
{
    tile_t tile = world_->getTile(x_, y_, map_level_);
    if (tile.tile_type == TILE_UPSTAIR)
    {
        world_->setCurrentLevel(world_->getCurrentLevel() - 1);
        position_t stair_pos = world_->getMapLevel(world_->getCurrentLevel())->getDownStairPos();
        setPosition(stair_pos.x, stair_pos.y, world_->getCurrentLevel());
        turn_finished_ = true;
    }
}

bool Actor::hasVictoryItem()
{
    std::map<char, Item*> item_map = inventory_->getMap();
    for (unsigned int i = 0; i < inventory_slots.size(); i++)
    {
        char slot = inventory_slots.at(i);
        Item* item = inventory_->get(slot);
        if (item != NULL)
        {
            if (item->getCategory() == CATEGORY_VICTORY_ITEM)
            {
                return true;
            }
        }
    }
    return false;
}

//DND style melee attack.
void Actor::meleeAttack(Actor *actor)
{
    std::stringstream attack_stream;
    attack_stream << capitalize(name_) << " attacks " << actor->getName();
    message(attack_stream.str());

    int die_roll = random(1, 20);
    int attack_roll =  die_roll + calcAtt(att_str_) + calcSize();
    int opponent_ac = actor->calcArmourClass();
    int damage_roll = 0;

    if (attack_roll >= opponent_ac || die_roll == 20)
    {
        damage_roll = calcMeleeDamage() + calcAtt(att_str_);
        damage_roll = setBoundedValue(damage_roll, 1, damage_roll);

        bool critical_hit = false;
        if (die_roll == 20)
        {
            critical_hit = true;
            damage_roll = damage_roll + calcMeleeDamage() + calcAtt(att_str_);
            damage_roll = setBoundedValue(damage_roll, 1, damage_roll);
        }
        damage_roll = setBoundedValue(damage_roll * damage_multiplier_, 1, damage_roll * damage_multiplier_);
    }
    else
    {
        std::stringstream miss_stream;
        miss_stream << capitalize(name_) << " misses";
        message(miss_stream.str());
    }

    if (damage_roll > 0)
    {
        actor->loseHealth(damage_roll);
    }

    turn_finished_ = true;
}

void Actor::loseHealth(int points)
{
    if (points >= 1)
    {
        std::stringstream damage_stream;
        damage_stream << capitalize(name_) << " takes " << points << " damage";

        int health = current_health_points_;
        current_health_points_ = current_health_points_ - points;

        if (current_health_points_ <= 0)
        {
            damage_stream << " and dies";
            kill();
        }
        else if (current_health_points_ < (double) 0.25 * max_health_points_ && health >= (double) 0.25 * max_health_points_)
        {
            damage_stream << " and is badly wounded";
        }
        message(damage_stream.str());
    }
}

void Actor::gainHealth(int points)
{
    if (points >= 1)
    {
        current_health_points_ = setBoundedValue(current_health_points_ + points, 0, max_health_points_);
    }
}

void Actor::loseEnergy(int points)
{
    if (points >= 1)
    {
        current_energy_points_ = setBoundedValue(current_energy_points_ - points, 0, current_energy_points_);
    }
}

void Actor::gainEnergy(int points)
{
    if (points >= 1)
    {
        current_energy_points_ = setBoundedValue(current_energy_points_ + points, 0, max_energy_points_);
    }
}

void Actor::dropItem(Item *item)
{
    inventory_->remove(item);

    //unequip if it is being worn
    for (int i = 0; i < NUM_EQUIPPED_SLOTS; i++)
    {
        if (equipped_items_[i] == item)
        {
            equipped_items_[i] = NULL;
        }
    }

    item->setPosition(x_, y_, map_level_);
    item->setOnGround(true);
    turn_finished_ = true;
}

char Actor::pickUpItem(Item *item)
{
    char slot = inventory_->add(item);
    item->setPosition(-1, -1, -1);
    item->setOnGround(false);
    turn_finished_ = true;
    return slot;
}

void Actor::useItem(Item *item)
{
    switch (item->getCategory())
    {
    case CATEGORY_WEAPON:
        wieldWeapon(item);
        break;
    case CATEGORY_BODY_ARMOUR:
        wearItem(item);
        break;
    case CATEGORY_POTION:
        drinkPotion(item);
        inventory_->remove(item);
        item->destroy();
        break;
    case CATEGORY_VICTORY_ITEM:
        message("Bring this to the surface to win the game!");
        break;
    default:
        message("ERROR: Malformed item category");
        break;
    }
}

void Actor::wieldWeapon(Item *item)
{
    setWeapon(item);
    turn_finished_ = true;
}

void Actor::wearItem(Item *item)
{
    switch (item->getCategory())
    {
    case CATEGORY_BODY_ARMOUR:
        setBodyArmour(item);
        turn_finished_ = true;
        break;
    default:
        message("ERROR: Something went wrong trying to wear an item");
    }
    turn_finished_ = true;
}

void Actor::drinkPotion(Item *item)
{
    switch (item->getType())
    {
    case TYPE_CURE_LIGHT_WOUNDS:
        gainHealth(roll_die(item->getValue(VALUES_POTION_ROLLS), item->getValue(VALUES_POTION_DIE_SIDES)));
        break;
    case TYPE_CURE_MODERATE_WOUNDS:
        gainHealth(roll_die(item->getValue(VALUES_POTION_ROLLS), item->getValue(VALUES_POTION_DIE_SIDES)));
        break;
    case TYPE_LIGHT_ENERGY_RESTORE:
        gainEnergy(roll_die(item->getValue(VALUES_POTION_ROLLS), item->getValue(VALUES_POTION_DIE_SIDES)));
        break;
    case TYPE_MODERATE_ENERGY_RESTORE:
        gainEnergy(roll_die(item->getValue(VALUES_POTION_ROLLS), item->getValue(VALUES_POTION_DIE_SIDES)));
        break;
    default:
        message("ERROR actor tried to quaff potion with bad type");
        break;
    }
    turn_finished_ = true;
}

bool Actor::hasEquipped(Item *item)
{
    for (int i = 0; i < NUM_EQUIPPED_SLOTS; i++)
    {
        if (item == equipped_items_[i])
        {
            return true;
        }
    }
    return false;
}

//Handles anything that needs to happen before taking a turn,
//including checking to see if it is actually the actor turn.
void Actor::startTurn()
{
    turn_finished_ = false;
    if (!isTurn())
    {
        turn_finished_ = true;
    }
}

//Calculates the actors next turn.
void Actor::endTurn()
{
    if (isTurn())
    {
        regenerateHealth();
        regenerateEnergy();
        next_turn_ = world_->getTimeStep() + calcSpeed();
    }

    if (current_health_points_ < (double) 0.25 * max_health_points_)
    {
        bleed();
    }
}

void Actor::bleed()
{
    world_->setTileColor(x_, y_, map_level_, TCODColor::desaturatedRed);
}

void Actor::regenerateHealth()
{
    int roll = random(1, 20);
    if (att_vit_ >= roll)
    {
        if (current_health_points_ < max_health_points_)
        {
            gainHealth(1);
        }
    }
}

void Actor::regenerateEnergy()
{
    int roll = random(1, 20);
    if (att_int_ >= roll)
    {
        if (current_energy_points_ < max_energy_points_)
        {
            gainEnergy(1);
        }
    }
}

int Actor::calcSpeed()
{
    return mod_speed_;
}

int Actor::calcSize()
{
    return mod_size_;
}

int Actor::calcSight()
{
    return sight_range_;
}

int Actor::calcAtt(int attribute)
{
    return (attribute - 10 )/5;
}

bool Actor::canSee(int level, int x, int y)
{
    return vision_map_[level]->isInFov(x,y);
}

void Actor::FOV(int level)
{
    vision_map_[level]->computeFov(x_, y_, calcSight());
}

void Actor::setVisionProperties(int x, int y, int z, int transparent, int walkable)
{
    vision_map_[z]->setProperties(x, y, transparent, walkable);
}

//returns the sum of the base ac, armour(if any), dex modifier and size modifier
int Actor::calcArmourClass()
{
    int armour_bonus = 0;

    if (hasBodyArmour())
    {
        armour_bonus = getBodyArmour()->getValue(VALUES_ARMOUR_AC);

        //armour_bonus += active_shield_->getArmourBonus();
    }
    return mod_base_ac_ + armour_bonus + calcAtt(att_dex_) + calcSize();
}

//Does a DnD-style die roll.
int Actor::calcMeleeDamage()
{
    int rolls;
    int die_sides;

    if (hasWeapon())
    {
        rolls = getWeapon()->getValue(VALUES_WEAPON_ROLLS);
        die_sides = getWeapon()->getValue(VALUES_WEAPON_DIE_SIDES);
    }
    else
    {
        rolls = unarmed_damage_[0];
        die_sides = unarmed_damage_[1];
    }
    return roll_die(rolls, die_sides);
}

int Actor::getAttribute(int att_type)
{
    switch (att_type)
    {
    case ATT_STR:
        return att_str_;
    case ATT_DEX:
    {
        int armour_penalty = 0;
        if (hasBodyArmour())
        {
            armour_penalty = getBodyArmour()->getValue(VALUES_ARMOUR_DEX_PEN);
            //armour_bonus += active_shield_->getArmourBonus();
        }

        return att_dex_ + armour_penalty;
    }
    case ATT_INT:
        return att_int_;
    case ATT_WIS:
        return att_wis_;
    case ATT_VIT:
        return att_vit_;
    default:
        message("ERROR: malformed att_type");
        return -1;
    }
}

void Actor::setPosition(int x, int y, int z)
{
    x_ = x;
    y_ = y;
    map_level_ = z;
}

//Accessors and Mutators
bool Actor::isTurnFinished()
{
    return turn_finished_;
}

bool Actor::isTurn()
{
    return next_turn_ == world_->getTimeStep();
}

int Actor::getXPosition()
{
    return x_;
}

int Actor::getYPosition()
{
    return y_;
}

int Actor::getMapLevel()
{
    return map_level_;
}

int Actor::getFaceTile()
{
    return face_tile_;
}

TCODColor Actor::getColor()
{
    return color_;
}

void Actor::setXPosition(int x)
{
    this->x_ = x;
}

void Actor::setYPosition(int y)
{
    this->y_ = y;
}

void Actor::setMapLevel(int z)
{
    this->map_level_ = z;
}

std::string Actor::getName()
{
    return name_;
}

int Actor::getCurrentHealth()
{
    return current_health_points_;
}

void Actor::setCurrentHealth(int health)
{
    current_health_points_ = health;
}

int Actor::getMaxHealth()
{
    return max_health_points_;
}

int Actor::getCurrentEnergy()
{
    return current_energy_points_;
}

void Actor::setCurrentEnergy(int energy)
{
    current_energy_points_ = energy;
}

int Actor::getMaxEnergy()
{
    return max_energy_points_;
}

bool Actor::hasWeapon()
{
    return getWeapon() != NULL;
}

bool Actor::hasBodyArmour()
{
    return getBodyArmour() != NULL;
}

Inventory* Actor::getInventory()
{
    return inventory_;
}

Item* Actor::getWeapon()
{
    return equipped_items_[ACTIVE_WEAPON_SLOT];
}

Item* Actor::getBodyArmour()
{
    return equipped_items_[ACTIVE_ARMOUR_SLOT];
}

void Actor::setWeapon(Item * item)
{
    equipped_items_[ACTIVE_WEAPON_SLOT] = item;
}

void Actor::setBodyArmour(Item *item)
{
    equipped_items_[ACTIVE_ARMOUR_SLOT] = item;
}
