/*
 *  File:       interface.h
 *  Summary:    Anything that displays on the screens is located here.
 *  Written by: Javon Harper
 */
#ifndef INTERFACE_H
#define INTERFACE_H
#include <string>
#include "world.h"


const static int SCREEN_WIDTH = 80;
const static int SCREEN_HEIGHT = 50;

void initScreen();
void showMainMenu();
void updateScreen();
void displayWorld(World *world, bool centered);
void displayUserInterface();
void displayGameOverScreen(std::string reason);
void displayWinScreen();

#endif
