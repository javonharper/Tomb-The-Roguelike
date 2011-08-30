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

};

#endif // CLASS_H
