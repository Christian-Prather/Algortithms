/*!
 * \author Christian Prather
 * \version 1.0
 * \date 2020-1-31
 * \warning Do not run over n = 11
 * \mainpage TSP 
 * \section compile_sec Compilation
 * To compile this was built on a linx system with gcc and was only ever tested on a linux system 
 * was testes with clion
*/

#include <iostream>
#include <fstream>
#include "Point.h"
#include "helpers.h"
#include "exhaustive.h"
#include <sstream>
#include <queue>
#include <chrono>
#include <thread>
using namespace std;

using namespace std;

int numberPoints = 10;
double finalDistance = 0;
int numberOfFiles = 4;

vector<Point> thePath;
vector<Point> allPoints;
vector<Point> copyAllPoints;

/*! 
 *\ A function for loading a parsing test files
*/
void loadFile(string fileName)
{
    string line;
    string tempX;
    string tempY;
    int x;
    int y;
    bool firstLine = true;
    ifstream pointFile(fileName);

    if (pointFile.is_open())
    {
        while (!pointFile.eof())
        {

            if (firstLine)
            {
                getline(pointFile, line);

                numberPoints = stoi(line);
                firstLine = false;
                continue;
            }

            pointFile >> tempX;
            pointFile >> tempY;
            x = stoi(tempX);
            y = stoi(tempY);

            Point tempPoint(x, y);
            allPoints.push_back(tempPoint);
        }
    }
    else
    {
        cout << "Critical error cant open file.." << fileName << endl;
    }
    pointFile.close();
}
/*! 
 *\ A function for generating test files
*/
void generateFiles(int min, int max)
{

    for (int i = 4; i < numberOfFiles + 4; i++)
    {
        //int n = generateRandomN();
        int n = generateBigRandomN();
        string fileName = "TestFile_" + to_string(i);
        cout << fileName << endl;

        ofstream testFile;
        testFile.open(fileName);
        testFile << n << endl;
        for (int j = 0; j < n - 1; j++)
        {
            testFile << generateRandomPoints(min, max) << " " << generateRandomPoints(min, max) << endl;
        }
        testFile << generateRandomPoints(min, max) << " " << generateRandomPoints(min, max);
    }
}
/*! 
 *\Function used for nearest-neigbor calculations
*/
void primaryLogic(Point startPoint)
{
    if (allPoints.size() != 0) // Base case
    {

        // Tracking variables
        double currentShortest = 0; // Shortes distce from current point to any other
        Point shortestPoint(0, 0);  // Point to add to path
        double distance = 0;        // Path Distance
        int pointIndex = 0;         // Index of point in allPoints vector
        // Main distance check
        for (int i = 0; i < allPoints.size(); i++) // Loop through all points left in vector
        {
            distance = calculateDistance(startPoint, allPoints.at(i)); // Distance form startPoint to current point in allPoints at index i
            // Set tracking variables if current distance is less then current minimum
            if (currentShortest == 0 || distance < currentShortest)
            {
                currentShortest = distance;
                shortestPoint.x = allPoints.at(i).x;
                shortestPoint.y = allPoints.at(i).y;
                pointIndex = i;
            }
        }
        // Add next point to the path vector
        thePath.push_back(shortestPoint);
        // Remove checked point from the list of points to check
        allPoints.erase(allPoints.begin() + pointIndex);
        finalDistance += currentShortest;

        // Recursivly call fucntion now starting at last added point
        primaryLogic(shortestPoint);
    }
}

/*! Used when testing manually */
void buildRandomList(int n, int min, int max)
{
    for (int i = 0; i < n; i++)
    {

        Point temp(generateRandomPoints(min, max), generateRandomPoints(min, max));
        cout << "X:" << temp.x << " Y:" << temp.y << "| ";
        allPoints.push_back(temp);

        cout << endl;
    }
}
void cleanUp()
{
    allPoints.clear();
    thePath.clear();
    copyAllPoints.clear();
    finalDistance = 0;
}
/*! Primary analysis file */

void analysis()
{
    ofstream analysisFile;
    analysisFile.open("Algorithm Analysis");

    //Run each file generated
    for (int j = 0; j < numberOfFiles; j++)
    {
        cleanUp(); // Reset tracking variables

        string fileName = "TestFile_" + to_string(j); //0-3
        analysisFile << endl;
        analysisFile << endl;
        analysisFile << "Results for: " << endl;

        double average = 0;
        double permAverage = 0;
        for (int i = 0; i < 3; i++)
        {
            cleanUp();

            loadFile(fileName);
            // Nearest Neigbor /////////////////////////////////////////////////////
            Point startPoint = allPoints.at(0); // Store first point and take it out of list
            allPoints.erase(allPoints.begin());
            thePath.push_back(startPoint);

            // Start the clock
            auto start = chrono::high_resolution_clock::now();
            primaryLogic(startPoint);

            // Calculate distance from last point back to start
            finalDistance += calculateDistance(thePath.back(), startPoint);
            thePath.push_back(startPoint);
            // Stop the clock, calculate duration
            auto stop = chrono::high_resolution_clock::now();
            chrono::duration<double, milli> duration = stop - start;

            average += duration.count(); // Add run times for 3 runs through to runnning average
            // Send results to log file
            analysisFile << fileName << " NN: "
                         << "Number of Points: " << to_string(numberPoints) << " Run: " << to_string(i) << " Time: " << to_string(duration.count()) << " Distance: " << finalDistance;
            analysisFile << "[";
            for (auto point : thePath)
            {
                analysisFile << " (X: " << point.x << " Y: " << point.y << " )";
            }
            analysisFile << "]";
            analysisFile << endl;
        }
        analysisFile << "Average: " << average / 3 << endl;

        //Exhaustive Search////////////////////////////////////////////////
        // Run the Exhaustive search on test files 0-3
        for (int i = 0; i < 3; i++)
        {
            cleanUp(); // Clean up

            loadFile(fileName);        // Relaod/parse the file (hate this but easy to code)
            copyAllPoints = allPoints; // Copy incase want to run NN and Exhaustive on same load run

            // Generate the n! list of potential paths (not included in timing as next_permutation and sort were used)
            // A significant amount of overall program execution however
            createPermutations(copyAllPoints, numberPoints);
            // Start timer
            auto permStart = chrono::high_resolution_clock::now();
            calculateExhaustive();
            auto permStop = chrono::high_resolution_clock::now();
            chrono::duration<double, milli> permDuration = permStop - permStart;
            cout << "Execution Permutations time (milli)" << permDuration.count() << endl;
            cout << "Permutations: " << getPermutations() << endl;
            // Save results to log file
            analysisFile << fileName << " EXHASUTIVE "
                         << "Number of Points: " << to_string(numberPoints) << " Run: " << to_string(i) << " Time: " << to_string(permDuration.count()) << " Dist: "
                         << getMinDist() << " Permutations: " << to_string(getPermutations()) << endl;
            permAverage += permDuration.count();
        }

        analysisFile << "Exhaustive Search Average: " << permAverage / 3;
    }
    // Run Nearest-Neighbor on second batch of test files containing between 35-50 n values
    analysisFile << endl
                 << "/////////////////////////////////////////////////////////////////" << endl;

    for (int i = numberOfFiles; i < numberOfFiles * 2; i++)
    {
        cleanUp(); // Reset

        string fileName = "TestFile_" + to_string(i); // 4-7

        double average = 0;
        for (int i = 0; i < 3; i++)
        {
            cleanUp(); // Safe than sorry
            loadFile(fileName);
            // Nearest Neigbor ///////////////////////////////////
            // Same execution process as above
            Point startPoint = allPoints.at(0);
            allPoints.erase(allPoints.begin());
            thePath.push_back(startPoint);
            auto start = chrono::high_resolution_clock::now();
            primaryLogic(startPoint); // Where the magic happens

            finalDistance += calculateDistance(thePath.back(), startPoint); // Distance of path

            thePath.push_back(startPoint);
            auto stop = chrono::high_resolution_clock::now();
            chrono::duration<double, milli> duration = stop - start;
            average += duration.count();
            // Log
            analysisFile << fileName << " NN: "
                         << "Number of Points: " << to_string(numberPoints) << " Run: " << to_string(i) << " Time: " << to_string(duration.count()) << " Distance: " << finalDistance;

            analysisFile << endl;
        }
        analysisFile << "Average: " << average / 3 << endl;
    }

    analysisFile.close();
}

int main()
{
    srand(time(NULL)); // Set random seed for file generations
    analysis();        // Main program for analysis
    //Dandger will overwrite dataset
    //generateFiles(10, 30);
    return 0;
}
