/*
 * Filename: controller.h
 * description: handles turning user input into command
 * author: Javon Harper
 */
#ifndef CONTROLLER_H
#define CONTROLLER_H

struct direction_t
{
  int x;
  int y;
};

direction_t charToDirection(char key);
bool isValidDirection(direction_t dir);
#endif
