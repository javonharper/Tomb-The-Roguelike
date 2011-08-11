/*
 *  File:       interface.h
 *  Summary:    Anything that displays on the screens is located here.
 *  Written by: Javon Harper
 */
#ifndef INTERFACE_H
#define INTERFACE_H
#include <string>
#include "libtcod.hpp"
#include "player.h"
#include "world.h"

const static int SCREEN_WIDTH = 80; 
const static int SCREEN_HEIGHT = 24;
enum DimProperties {X = 0, Y, WIDTH, HEIGHT};
const static int worldScreenDims[] = {0, 0, 60, 20};
const static int infoScreenDims[] = {worldScreenDims[WIDTH], 0, SCREEN_WIDTH - worldScreenDims[WIDTH], SCREEN_HEIGHT};
const static int logScreenDims[] = {0, worldScreenDims[HEIGHT], SCREEN_WIDTH - infoScreenDims[WIDTH], worldScreenDims[HEIGHT] - SCREEN_HEIGHT};
const static int NUM_MESSAGES_DISPLAYED = 4;

void initScreen();
void updateScreen();

void displayTitleScreen();
void displayGameScreen(World *world);
void displayInventoryScreen(World *world);
void displayDropItemsScreen(World *world);
void displayWinScreen();
void displayGameOverScreen(std::string reason);

void drawWorldPanel(World *world);
void drawWorld(World *world);
void drawInfoPanel(World *world);
void drawLogPanel(World *world);

char prompt(std::string message);
void message(std::string message);
void displayMessages();

void drawVerticalLine(int x, int y, int height, TCODColor color);
void drawHorizontalLine(int x, int y, int width, TCODColor color);
#endif
