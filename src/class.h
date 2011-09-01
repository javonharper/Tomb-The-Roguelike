/*
 *  File:       class.h
 *  Summary:    The various classes the player can take on
 *  Written by: Javon Harper
 */

#ifndef CLASS_H
#define CLASS_H

#include <string>

enum ClassType
{
    CLASS_ADVENTURER = 0,
    CLASS_FIGHTER,
    CLASS_ROGUE,
    CLASS_PRIEST,
    CLASS_MAGICIAN,
    NUM_CLASS_TYPES
};

struct class_data_t
{
    int class_type;
    std::string class_letter;
    std::string class_string;
    int class_requirements[NUM_CLASS_TYPES];
};

const static class_data_t class_data [NUM_CLASS_TYPES] =
{
    { CLASS_ADVENTURER,  "a", "Adventurer", {0,0,0,0,0} },
    { CLASS_FIGHTER,     "f", "Fighter",    {3,0,0,0,0} },
    { CLASS_ROGUE,       "r", "Rogue",      {3,0,0,0,0} },
    { CLASS_PRIEST,      "p", "Priest",     {3,0,0,0,0} },
    { CLASS_MAGICIAN,    "m", "Magician",   {3,0,0,0,0} }
};

class Class
{
private:
    int active_class_;
    int class_levels_ [NUM_CLASS_TYPES];
public:
    Class();
    ~Class();
    int getActiveClassType();
    std::string getActiveClassTypeString();
    int getClassLevel(int class_type);
    int levelUp();
    void changeClass(int class_type);
    bool hasRequirementsForClass(int class_type);

};

#endif // CLASS_H
