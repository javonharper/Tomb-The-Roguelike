/*
 *  File:       interface.h
 *  Summary:    Anything that displays on the screens is located here.
 *  Written by: Javon Harper
 */
#ifndef INTERFACE_H
#define INTERFACE_H
#include <string>
#include "libtcod.hpp"
#include "world.h"

const static int SCREEN_WIDTH = 80; 
const static int SCREEN_HEIGHT = 50;

enum DimProperties {X = 0, Y, WIDTH, HEIGHT};
const static int titleScreenDims[] = {0, 0, 50, 3};
const static int worldScreenDims[] = {0, 3, 50, 39};
const static int infoScreenDims[] = {50, 0, 30, 50};
const static int logScreenDims[] = {0, 42, 50, 8};

void initScreen();
void showMainMenu();
void updateScreen();

void displayGame(World *world);
void drawTitleBar();
void drawWorldScreen(World *world);
void drawWorld(World *world);
void drawInfoScreen(World *world);
void drawLogScreen(World *world);

void displayWinScreen();
void displayGameOverScreen(std::string reason);

void drawLinedBorder(int x, int y, int w, int h, TCODColor color);
void drawDoubleLinedBorder(int x, int y, int w, int h, TCODColor color);
void drawVerticalLine(int x, int y, int height, TCODColor color);
void drawDoubleVerticalLine(int x, int y, int height, TCODColor color);

#endif
