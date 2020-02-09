#include "exhaustive.h"
#include "helpers.h"
#include <algorithm>
using namespace std;

// Globals for tracking during 3 different runs
int permutations = 0;
double minDist;
vector<vector<Point>> potentialPaths; // Data structure to handle matrix of potential paths

// Getters
int getPermutations()
{
    return permutations;
}

double getMinDist()
{
    return minDist;
}
// Function for generating the n! possible permutations and
// placing them in a data structure
void createPermutations(vector<Point> allPoints, int numberOfPoints)

{
    minDist = 100000000; // Default min distance to something overly large

    permutations = 0;                         // Reset permutations
    potentialPaths.clear();                   // Safe guard for multiple runs
    sort(allPoints.begin(), allPoints.end()); // Sort algortithm believe O(n)

    // While there is a valid permutation available add it to the data structure
    // Referenced from: https://www.geeksforgeeks.org/all-permutations-of-an-array-using-stl-in-c/
    do
    {
        vector<Point> temp; // Temporary row vector

        for (int i = 0; i < numberOfPoints; i++) // 0-n
        {

            temp.push_back(allPoints.at(i)); // Add each point to the row
        }

        permutations++;                 // Track number of permutations
        potentialPaths.push_back(temp); // Push the row to the data structure

    } while (next_permutation(allPoints.begin(), allPoints.end()));

    cout << "Number of permutations = " << permutations << endl;
    cout << "////////////////////////////////////////////////////////////////////////////" << endl;
    for (vector<Point> points : potentialPaths)
    {
        cout << "[ ";
        for (Point point : points)
        {
            cout << "( " << point.x << " , " << point.y << " )";
        }
        cout << " ]" << endl;
    }
}
//////////////////////Actuall do the darn math//////////////////////
void calculateExhaustive()
{
    //minDist = 100000000;
    int pathIndex = 0;
    for (int i = 0; i < potentialPaths.size(); i++)
    {
        double pathDist = 0;
        Point startPoint = potentialPaths.at(i).at(0);
        Point lastPoint = potentialPaths.at(i).at(potentialPaths.at(i).size() - 1);
        // Path
        for (int j = 0; j < potentialPaths.at(i).size() - 1; j++)
        {
            // Points
            Point currentPoint = potentialPaths.at(i).at(j);
            Point nextPoint = potentialPaths.at(i).at(j + 1);
            pathDist += (calculateDistance(currentPoint, nextPoint));
        }
        // Add last point to frist dist
        pathDist += calculateDistance(lastPoint, startPoint);
        if (pathDist < minDist)
        {
            minDist = pathDist;
            cout << "PATH DIST: " << minDist << endl;
            pathIndex = i;
        }
    }

    cout << "Path index: " << pathIndex << " Dist: " << minDist << endl;
    cout << "Path: [";

    // Print out matrix to console (alot of data!)
    for (int i = 0; i < potentialPaths.at(pathIndex).size(); i++)
    {
        cout << "(X: " << potentialPaths.at(pathIndex).at(i).x << " Y: " << potentialPaths.at(pathIndex).at(i).y << " )";
    }
    cout << "(X: " << potentialPaths.at(pathIndex).at(0).x << " Y: " << potentialPaths.at(pathIndex).at(0).y << " )]" << endl;
}
