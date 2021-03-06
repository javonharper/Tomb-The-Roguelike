/*
 *  File:       world.h
 *  Summary:    holds the maps and actors.
 *  Written by: Javon Harper
 */
#ifndef WORLD_H
#define WORLD_H

#include <vector>
#include "map.h"

const static int START_LEVEL = 0;
class Actor;
class Player;
class Enemy;
class Item;
class World
{
private:
    int width_;
    int height_;
    int levels_;
    std::vector<Map*> level_list_;
    std::vector<Enemy*> enemy_list_;
    std::vector<Item*> item_list_;

    int current_level_;
    int time_step_;

    Player *player_;
public:
    World(int world_width, int world_height, int world_levels);
    ~World();
    void initMapLevels();
    Player* generatePlayer();
    std::vector<Enemy*> generateEnemies(int min, int max);
    std::vector<Item*> generateItems(int min, int max);
    position_t findPosition(int level);
    Enemy *getEnemyAt(int x, int y, int level);
    std::vector<Item*> getItemsAt(int x, int y, int level);
    void kill(Enemy *enemy);

    tile_t getTile(int x, int y, int z);
    void setTile(int x, int y, int z, int tile_type);
    void setTileColor(int x, int y, int z, TCODColor color);
    void setTileAsSeen(int x, int y, int z);
    int getWidth();
    int getHeight();
    int getLevels();
    int getCurrentLevel();
    void setCurrentLevel(int level);
    int getTimeStep();
    void incrementTimeStep();
    Player *getPlayer();
    std::vector<Enemy*> &getEnemyList();
    std::vector<Item*> &getItemList();
    Map *getMapLevel(int level);
};

#endif
