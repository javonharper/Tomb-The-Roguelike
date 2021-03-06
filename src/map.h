/*
 *  File:       map.h
 *  Summary:    2d array of tiles.
 *  Written by: Javon Harper
 */
#ifndef MAP_H
#define MAP_H

#include "libtcod.hpp"

const static int DOWNSTAIR_INDEX = 0;
const static int UPSTAIR_INDEX = 1;

struct position_t
{
    int x;
    int y;
};

enum tileTypes
{
    TILE_UNKNOWN = 0,
    TILE_BLANK,
    TILE_FLOOR,
    TILE_WALL,
    TILE_DOWNSTAIR,
    TILE_UPSTAIR,
    TILE_DOOR_CLOSED,
    TILE_DOOR_OPEN
};

struct tile_t
{
    int tile_type;
    int face_tile;
    bool is_passable;
    bool is_transparent;
    TCODColor color;
    bool has_been_seen;
};

//If this project ever leaves libtcod, these color types have to change.
const static tile_t tile_db[] =
{

    {TILE_UNKNOWN,      '?',              false, false, TCODColor::red,         false},
    {TILE_BLANK,        ' ',              false, false, TCODColor::yellow,      false},
    {TILE_FLOOR,        '.',              true,  true,  TCODColor::white,       false},
    {TILE_WALL,         TCOD_CHAR_BLOCK1, false, false, TCODColor::lightOrange, false},
    {TILE_DOWNSTAIR,    '>',              true,  true,  TCODColor::white,       false},
    {TILE_UPSTAIR,      '<',              true,  true,  TCODColor::white,       false},
    {TILE_DOOR_CLOSED,  '+',              false, false, TCODColor::darkYellow,  false},
    {TILE_DOOR_OPEN,    '/',              true,  true,  TCODColor::darkYellow,  false}
    //BROKEN DOORS?
    //TRAP DOORS?
};

//These patterns are used to find appropriate spots for tile by rotating it until it fits.s
static const int stair_spot_pattern[3][3] = {{TILE_FLOOR, TILE_FLOOR, TILE_FLOOR},{TILE_WALL, TILE_WALL, TILE_WALL},{TILE_WALL, TILE_WALL, TILE_WALL}};
static const int door_spot_pattern[3][3] = {{TILE_UNKNOWN, TILE_FLOOR, TILE_UNKNOWN},{TILE_WALL, TILE_FLOOR, TILE_WALL},{TILE_FLOOR, TILE_FLOOR, TILE_FLOOR}};

class Map
{
private:
    int width_;
    int height_;
    tile_t **tile_map_;
    position_t stair_positions_[2];

public:
    Map(int map_width, int map_height);
    ~Map();
    void initializeMap();
    void makeMap();
    void makeRoom(int x, int y, int w, int h);
    position_t placeFeature(int tile_type);
    bool matchesPattern(tile_t tile[3][3], const int const_tile_pattern[3][3]);

    void placeCorridor(int src_x, int src_y, int dest_x, int dest_y);
    void cleanWalls();


    tile_t getTile(int x, int y);
    void setTile(int x, int y, int tile_type);
    void setTileAsSeen(int x, int y);
    void setTileColor(int x, int y, TCODColor color);
    int getWidth();
    int getHeight();
    position_t getDownStairPos();
    position_t getUpStairPos();
};

#endif
