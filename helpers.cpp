//
// Created by christian on 1/28/20.
//

#include "helpers.h"
#include "math.h"
#include <time.h>

double calculateDistance(Point current, Point option)
{
    // Euclidian distance formula
    return (sqrt(pow((option.x - current.x), 2) + pow((option.y - current.y), 2)));
}

int generateRandomPoints(int min, int max)
{
    return (rand() % (max - min + 1)) + min;
    // Generic random generator
}
int generateRandomN()
{
    return (rand() % (10 - 4 + 1)) + 4;
    // Random N for [4-10]
}
int generateBigRandomN()
{
    return (rand() % (50 - 35 + 1)) + 35;
    // Random N for [4-10]
}
