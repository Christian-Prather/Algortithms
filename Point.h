//
// Created by christian on 1/28/20.
//

#ifndef ALGORITHMS_PROJECT1_POINT_H
#define ALGORITHMS_PROJECT1_POINT_H

#include <vector>

using namespace std;

// General class for storing point data
class Point
{
public:
    // X,Y coordinates
    int x;
    int y;

    // Constructors
    Point(int x, int y)
    {
        this->x = x;
        this->y = y;
    }
    Point()
    {
        this->x = 0;
        this->y = 0;
    }

private:
    // Operation overload for sort comparison
    friend bool operator<(const Point &leftPoint, const Point &rightPoint)
    {

        // Check x's first
        if (leftPoint.x != rightPoint.x)
        {
            return leftPoint.x < rightPoint.x;
        }
        // Now check y's
        else
        {
            return leftPoint.y < rightPoint.y;
        }
    }
};

#endif //ALGORITHMS_PROJECT1_POINT_H
