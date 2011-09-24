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

std::string capitalize(std::string str)
{
	std::string::iterator it(str.begin());

	if (it != str.end())
		str[0] = toupper((unsigned char)str[0]);

	while(++it != str.end())
	{
		*it = tolower((unsigned char)*it);
	}
	return str;
}
