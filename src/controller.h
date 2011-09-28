/*
 * File:    controller.h
 * Summary:  Handles user input and converts into objects the game can use
 * Written by: Javon Harper
 */

#ifndef CONTROLLER_H
#define CONTROLLER_H

const static char MOVE_NORTH = 'k';
const static char MOVE_SOUTH = 'j';
const static char MOVE_EAST = 'l';
const static char MOVE_WEST = 'h';
const static char MOVE_NORTHEAST = 'u';
const static char MOVE_NORTHWEST = 'y';
const static char MOVE_SOUTHEAST = 'n';
const static char MOVE_SOUTHWEST = 'b';
const static char REST = '.';
const static char MOVE_UPSTAIRS = '<';
const static char MOVE_DOWNSTAIRS = '>';
const static char CLOSE_DOOR  = 'c';
const static char OPEN_DOOR = 'o';
const static char PICKUP = 'g';
const static char SHOW_INVENTORY = 'i';
const static char USE_ITEM = 'a';
const static char DROP_ITEM =  'd';
const static char INSPECT_ITEM = 'i';
const static char CHANGE_CLASS = 'C';
const static char HELP = '?';

const static char YES = 'y';
const static char NO = 'n';

const static char STRENGTH   = 's';
const static char DEXTERITY  = 'd';
const static char INTELLIGENCE = 'i';
const static char WISDOM   = 'w';
const static char VITALITY =  'v';

const static char ESC = 27;
const static char QUIT = 'Q';

struct direction_t
{
    int x;
    int y;
};

direction_t charToDirection(char key);
bool isValidDirection(direction_t dir);
int charToClassInt(char letter);
#endif
