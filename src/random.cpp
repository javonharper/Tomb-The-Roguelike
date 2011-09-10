/*
 *  File:       random.ccp
 *  Summary:    Randomized numbers
 *  Written by: Javon Harper
 */
#include <cstdlib>
#include <ctime>

//Initializes the generator using the time.
int init_rand()
{
    int seed = time(0);
    srand(seed);
    return seed;
}

//Initializes the generator using a seed.
int init_rand(int seed)
{
    srand(seed);
    return seed;
}

//returns a random number
int random(int min, int max)
{
    int range = max - min + 1;
    int random = rand() % range;
    return min + random;
}

int die_roll(int rolls, int die_sides)
{
    int total = 0;
    for (int i = 0; i < rolls; i++)
    {
        total = total + random(1, die_sides);
    }
    return total;
}
