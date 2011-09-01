Tomb: The Rogue-like
Tomb is a fantasy roguelike game set in the ancient burial ground of a long deceased ruler.
Your goal as an adventurer is to brave the depths of the labyrinths and corridors and retrieve
the icon of the Weedaula so that peace can be restored in your nearby village.

In Tomb, the maps, items, and enemies are randomly generated so no game is ever the same.

Playing Tomb:
Tomb should be built before it is ran.
At command line, type in the command

make run

Afterwards, you can run the command ./tomb or double click the executable file to start the game.

KEY COMMANDS
h   go west
j   go south
k   go north
l   go east
y   go northwest
u   go northeast
b   go southwest
n   go southeast
.   rest for a turn
o   open a door
c   close a door
>   go down a set of stairs
<   go up a set of stairs
g   get an item off the ground
i   open inventory
C   change your characters class
Q quit game


Developing Tomb:
Tomb is a roguelike written in C++ using the libtcod library.
This repository comes with the code::blocks project files I'm using to develop Tomb.
Alternatively, there is a makefile that cleans, builds, and runs the project.

Acknowlegements:
libSDL.so, and files located in the lib directory are third party libraries and belong to their respective owners.
doryen roguelike library (http://doryen.eptalys.net/)
Simple DirectMedia Layer library (http://www.libsdl.org)
Alloy_curses_12x12.png, a Dwarf Fortress Tileset by the user Alloy (http://df.magmawiki.com/index.php/User:Alloy)
