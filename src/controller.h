/*
 * Filename: controller.h
 * description: handles turning user input into command
 * author: Javon Harper
 */
#ifndef CONTROLLER_H
#define CONTROLLER_H

#define MOVE_NORTH      'k'
#define MOVE_SOUTH      'j'
#define MOVE_EAST       'l'
#define MOVE_WEST       'h'
#define MOVE_NORTHEAST  'u'
#define MOVE_NORTHWEST  'y'
#define MOVE_SOUTHEAST  'n'
#define MOVE_SOUTHWEST  'b'
#define REST            '.'
#define MOVE_UPSTAIRS   '<'
#define MOVE_DOWNSTAIRS '>'
#define CLOSE_DOOR      'c'
#define OPEN_DOOR       'o'
#define DROP            'd'
#define PICKUP          'g'
#define SHOW_INVENTORY  'i'
#define USE_ITEM        'a'
#define HELP            '?'

#define YES 'y'
#define NO  'n'

#define ESC 27

struct direction_t
{
  int x;
  int y;
};

direction_t charToDirection(char key);
bool isValidDirection(direction_t dir);
#endif
