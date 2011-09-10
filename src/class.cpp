/*
 *  File:       class.cpp
 *  Summary:    The various classes the player can take on
 *  Written by: Javon Harper
 */

#include "class.h"
#include "interface.h"

Class::Class()
{
    active_class_ = CLASS_ADVENTURER;
    for (int i = 0; i < NUM_CLASS_TYPES; i++)
    {
        if (i == CLASS_ADVENTURER)
        {
            class_levels_[i] = 1;
        }
        else
        {
            class_levels_[i] = 0;
        }
    }
}

Class::~Class()
{
    //dtor
}

int Class::getActiveClassType()
{
    return active_class_;
}

std::string Class::getActiveClassTypeString()
{
    return class_data[active_class_].class_string;
}

int Class::getClassLevel(int class_type)
{
    return class_levels_[class_type];
}

int Class::levelUp()
{
    class_levels_[active_class_] = class_levels_[active_class_] + 1;

    for (int i = 0; i < NUM_CLASS_TYPES; i++)
    {
        if (hasRequirementsForClass(i) && getClassLevel(i) == 0)
        {
            class_levels_[i] = 1;
        }
    }

    return class_levels_[active_class_];
}

void Class::changeClass(int class_type)
{
    if (hasRequirementsForClass(class_type))
    {
        active_class_ = class_type;
    }
    else
    {
        //TODO tell them what they actually need to be this class
        message("You do not have the required experience to be this class");
    }
}

bool Class::hasRequirementsForClass(int class_type)
{
    for (int i = 0; i < NUM_CLASS_TYPES; i++)
    {
        if (class_data[class_type].class_requirements[i] > getClassLevel(i))
        {
            return false;
        }
    }
    return true;
}
