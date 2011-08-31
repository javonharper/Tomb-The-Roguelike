/*
 *  File:       controller.cpp
 *  Summary:    Handles user input and converts into objects the game can use
 *  Written by: Javon Harper
 */

#include "controller.h"
#include "class.h"

direction_t charToDirection(char input)
{
  int dx = 0;
  int dy = 0;

  switch(input)
  {
    case 'h': dx = -1; dy = 0; break;
    case 'j': dx = 0; dy = 1; break;
    case 'k': dx = 0; dy = -1; break;
    case 'l': dx = 1; dy = 0; break;
    case 'y': dx = -1; dy = -1; break;
    case 'u': dx = 1; dy = -1; break;
    case 'b': dx = -1; dy = 1; break;
    case 'n': dx = 1; dy = 1; break;
  }

  direction_t dir;
  dir.x = dx;
  dir.y = dy;
  return dir;
}

bool isValidDirection(direction_t dir)
{
  return !(dir.x == 0 && dir.y == 0);
}

int charToClassInt(char letter)
{
  for (int i = 0; i < NUM_CLASS_TYPES; i++)
  {
    if(class_data[i].class_letter.at(0) == letter)
    {
      return i;
    }
  }
  return -1;
}
