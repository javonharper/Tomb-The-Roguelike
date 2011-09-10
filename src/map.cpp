/*
 *  File:       map.cpp
 *  Summary:    2d array of tiles.
 *  Written by: Javon Harper
 */
#include <iostream>
#include "functions.h"
#include "random.h"
#include "map.h"
#include "libtcod.hpp"

Map::Map(int map_width, int map_height)
{
    width_ = map_width;
    height_ = map_height;

    tile_map_ = new tile_t*[map_width];
    for (int i = 0; i < map_width; i++)
    {
        tile_map_[i] = new tile_t[map_height];
    }
    initializeMap();
    makeMap();

    //Build the stairs
    position_t down_stair = placeFeature(TILE_DOWNSTAIR);
    position_t up_stair = placeFeature(TILE_UPSTAIR);
    stair_positions_[DOWNSTAIR_INDEX] = down_stair;
    stair_positions_[UPSTAIR_INDEX] = up_stair;
    cleanWalls();
}

Map::~Map()
{
    delete tile_map_;
}

//For every tile in the map, make it a wall.
void Map::initializeMap()
{
    for (int i = 0; i < width_; i++)
    {
        for (int j = 0; j < height_; j++)
        {
            tile_map_[i][j] = tile_db[TILE_WALL];
        }
    }
}

void Map::makeMap()
{
    //Simple parameters of the map
    int min_room_width = 5;
    int max_room_width = 10;
    int min_room_height = 5;
    int max_room_height = 10;
    int num_rooms = random(5, 10);
    //int num_rooms = random(1,1);

    //These variables will be assigned ot be random positions within the room.
    int ox = -1;
    int oy = -1;
    int nx = -1;
    int ny = -1;

    //While still making rooms, choose random position and size, put the floor down, and connect them by corridors.
    while (num_rooms--)
    {
        int x = random(2, width_ - max_room_width - 2);
        int y = random(2, height_ - max_room_height - 2);
        int w = random(min_room_width, max_room_width);
        int h = random(min_room_height, max_room_height);

        makeRoom(x,y,w,h);

        //choose a random spot within the room.
        nx = random(x, x + w - 1);
        ny = random(y, y + h - 1);

        if (ox != -1 && oy != -1)
        {
            placeCorridor(ox, oy, nx, ny);
        }
        ox = nx;
        oy = ny;
    }
}

void Map::makeRoom(int x, int y, int w, int h)
{
//Makes the floor starting at x,y and going to x+w, y+h
//    for (int i = -1; i < w + 1; i++)
//      for (int j = -1; j < h + 1; j++)
//        tile_map_[x + i][y + j] = tile_db[TILE_WALL];
    for (int i = 0; i < w; i++)
        for (int j = 0; j < h; j++)
            tile_map_[x + i][y + j] = tile_db[TILE_FLOOR];
}

position_t Map::placeFeature(int tile_type)
{
    position_t pos;
    pos.x = -1;
    pos.y = -1;
    bool found_spot = false;

    while (!found_spot)
    {
        int pos_x = random(2, this->getWidth() - 2);
        int pos_y = random(2, this->getHeight() - 2);

        //get the 3x3 tiles around the random tile
        tile_t tile_array[3][3];
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
            {
                tile_array[i][j] = this->getTile(pos_x + i - 1, pos_y + j - 1);
            }

        if (tile_type == TILE_UPSTAIR || tile_type == TILE_DOWNSTAIR)
        {
            found_spot = matchesPattern(tile_array, stair_spot_pattern);
        }
        else if (tile_type == TILE_DOOR_CLOSED || tile_type == TILE_DOOR_OPEN)
        {
            found_spot = matchesPattern(tile_array, door_spot_pattern);
        }

        if (found_spot)
        {
            pos.x = pos_x;
            pos.y = pos_y;
            this->setTile(pos.x, pos.y, tile_type);
        }
    }
    return pos;
}

//Returns true if the tile array matches the pattern of the 3x3 tiles
//flips the 3x3 matrix of ints clockwise and tests the array against it.
bool Map::matchesPattern(tile_t tile_array[3][3], const int const_tile_pattern[3][3])
{
    int tile_pattern[3][3];
    //Copy the pattern over to a non-const int array.
    for (int j = 0; j < 3; j++ )
        for (int i = 0; i < 3; i++)
            tile_pattern[i][j] = const_tile_pattern[i][j];


    //for each rotation of the pattern, see if the tile array fits the pattern.
    const int MAX_TURNS = 4;
    for (int turns = 0; turns < MAX_TURNS; turns++)
    {
        //Assume that this pattern matches
        bool matches_pattern = true;

        //Check if the pattern matches.
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
            {
                //std::cout << tile_array[i][j].tile_type;
                if (tile_array[i][j].tile_type != tile_pattern[i][j] && tile_pattern[i][j] != TILE_UNKNOWN)
                {
                    matches_pattern = false;
                }
            }

        //check if this pattern matches before moving on
        if (matches_pattern)
            return matches_pattern;

        //flip the int matrix clock wise
        int temp_pattern_array[3][3];
        for (int j = 0; j < 3; j++ )
            for (int i = 0; i < 3; i++)
            {
                temp_pattern_array[3 - 1- j][i] = tile_pattern[i][j];
            }

        //copy the array.
        for (int j = 0; j < 3; j++ )
            for (int i = 0; i < 3; i++)
                tile_pattern[i][j] = temp_pattern_array[i][j];
    }
    return false;
}

//Places a corridor leading from srcx, srcy to destx, desty.
void Map::placeCorridor(int src_x, int src_y, int dest_x, int dest_y)
{
    //Finds the right most of the two and prints a straight line until it reaches the other x
    int start_x = min(src_x, dest_x);
    int end_x = max(src_x, dest_x);
    int start_y = 0;

    if (start_x == src_x)
    {
        start_y = src_y;
    }
    if (start_x == dest_x)
    {
        start_y = dest_y;
    }

    //NOTE: the +1 is to prevent the corners that can only be traversed diagnoly.
    for (int i = start_x; i < end_x + 1; i++)
    {
        this->tile_map_[i][start_y] = tile_db[TILE_FLOOR];
    }

    //Finds the y with the lowest value and prints a straight line until it reaches the other y.
    start_y = min(src_y, dest_y);
    int end_y = max(src_y, dest_y);

    if (start_y == src_y)
    {
        start_y = src_y;
    }
    if (start_y == dest_y)
    {
        start_y = dest_y;
    }

    for (int i = start_y; i < end_y; i++)
    {
        this->tile_map_[end_x][i] = tile_db[TILE_FLOOR];
    }
}

//Removes any walls that are not near a floor
//EWWWW quaduple for loops!!
void Map::cleanWalls()
{
    for (int i = 0; i < width_; i++)
    {
        for (int j = 0; j < height_; j++)
        {
            if (i == 0 || i == width_ - 1 || j == 0 || j == height_ - 1)
            {
                setTile(i, j, TILE_BLANK);
            }
            else
            {
                //Assume the tile should be blank. If it is next to a floor don't make it blank.
                //this could be another procedure.
                bool make_blank = true;
                //NOTE: Notice the (+2)s int he for statements.
                for (int k = i - 1; k < i + 2; k++)
                {
                    for (int l = j - 1; l < j + 2; l++)
                    {
                        if (getTile(k, l).is_passable == true)
                        {
                            make_blank = false;
                        }
                    }
                }
                if (make_blank)
                {
                    setTile(i, j, TILE_BLANK);
                }
            }
        }
    }
}

void Map::setTile(int x, int y, int tile_type)
{
    tile_map_[x][y] = tile_db[tile_type];
}
void Map::setTileAsSeen(int x, int y)
{
    tile_map_[x][y].has_been_seen = true;
}
void Map::setTileColor(int x, int y, TCODColor color)
{
    tile_map_[x][y].color = color;
}
tile_t Map::getTile(int x, int y)
{
    return tile_map_[x][y];
}
int Map::getWidth()
{
    return width_;
}
int Map::getHeight()
{
    return height_;
}
position_t Map::getDownStairPos()
{
    return stair_positions_[DOWNSTAIR_INDEX];
}
position_t Map::getUpStairPos()
{
    return stair_positions_[UPSTAIR_INDEX];
}
