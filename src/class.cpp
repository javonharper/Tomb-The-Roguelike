#include "class.h"

Class::Class()
{
  active_class_ = CLASS_ADVENTURER;
  for (int i = 0; i < NUM_CLASS_TYPES; i++)
  {
    class_levels_[i] = 1;
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
  switch(active_class_)
  {
    case CLASS_ADVENTURER: return "Adventurer";
    case CLASS_FIGHTER: return "Fighter";
    case CLASS_PRIEST: return "Priest";
    case CLASS_MAGICIAN: return "Magician";
    case CLASS_ROGUE: return "Rogue";
  }
   return "ERROR: malformed class";
}

int Class::getClassLevel(int class_type)
{
  return class_levels_[class_type];
}

int Class::levelUp()
{
  class_levels_[active_class_] = class_levels_[active_class_] + 1;
  return class_levels_[active_class_];
}
