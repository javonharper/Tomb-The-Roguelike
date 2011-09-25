Tomb: The Rogue-like
Tomb is a fantasy roguelike game set in the ancient burial ground of a long deceased ruler.
Your goal as an adventurer is to brave the depths of the labyrinths and corridors and retrieve
the icon of the Weedaula so that peace can be restored in your nearby village.
Tomb is designed to be a coffeebreak roguelike where the player can experiment with character classes into powerful combinations.

Class Mechanics:
The player starts as a level 1 Adventurer and gains experience for killing enemies. On level up,
they get 1 point in an attribute of their choice and 1 point in your current player class.

Some weapons are special in that some classes can utilize their special abilities. For example,
level 2 paladins could use a Sword of Retribution to use the Smite ability, which uses 4 energy.

An important aspect of the game would be finding these weapons that help you specialize into your class.

Classes can be upgraded to other classes. For example, a level 3 Adventurer can become
a Magician, Priest, Fighter, or Rogue. Similarly, classes can be combined to create hybrid-classes.
For example, a player with experience at a level 2 Magician and a level 2 Figter can have the oppurtunity
to become a Psychic Warrior.

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
