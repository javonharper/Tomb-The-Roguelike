/*
 *  File:       functions.cpp
 *  Summary:    misc useful functions
 *  Written by: Javon Harper
 */

#include <string>
#include <sstream>
#include "functions.h"
#include "actor.h"


bool withinRange(int x, int min, int max)
{
    return x >= min && x <= max;
}

int setBoundedValue(int var, int min, int max)
{
    if (var <= min)
        return min;
    else if (var >= max)
        return max;
    else return var;
}

int min(int a, int b)
{
    return (a < b)? a:b;
}

int max(int a, int b)
{
    return (a > b)? a:b;
}
