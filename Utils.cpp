#include <Utils.h>
#include <cstdlib>
#include <vector>
#include <Point3D.h>
#include <iostream>
#include <sstream>
#include <string>
#include <istream>
#include <ostream>
#include <fstream>
#include <cmath>
#include <Pose.h>

using namespace std;

/*! \file Utils.cpp: implements the Utils class */

/*param1: ss = stream containing a line
 *param2: points = vector of vectors of points
 *return: N/A
 *function: takes a line of points, puts them into a vector, then adds that
 *		vector to the larger vector
 */
void Utils::addPts(istringstream & ss, vector<Pose> & points)
{
	vector<Point3D> pts;
	Pose linePoints = Pose(pts);
	for(int i = 0; i < 25; i++)
	{
		double x;
		double y;
		double z;
		ss >> x;
		ss >> y;
		ss >> z;
		Point3D point(x, y, z);
		linePoints.push_back(point);
	}
	points.push_back(linePoints);
}

/*param1: s = string that should represent a double
 *return: '1' if all characters are ok, the bad character if not
 *function: takes a string, checks each character to see if it is valid for a double
*/
char Utils::checkChars(string s)
{
  	for(unsigned int i = 0; i < s.size(); i++)
	{
		char c = s[i];
		if(!(isdigit(c) || c == '-' || c == '.' || c == 'e' || c == 				'+'))
		{
			return c;
		}
	}
	return '1';
}
/*param1: ifs = stream that reads lines
 *return: number of lines read
 *function: takes in a stream, reads through the lines and returns the number of lines read
*/
int Utils::getPermLineCount(ifstream & ifs)
{
	int permLineCount = 0;
	/*string line;
	if(ifs.is_open())
	{
		while(getline(ifs,line))
		{
			permLineCount++;
		}
	}*/
	return permLineCount;
}

/*param1: lineCount = line number currently being processed
 *param2: line = line currently being processed
 *param3: points = vector of Point3D vectors that will be added to as lines are read
 *param4: permLineCount = number of lines in entire file (previously determined)
 *param5: u = Utils object to process functions on
 *return: 0 if successful, -1 if not
 *function: processes the lines and adds the coordinates to the Point3D vector
*/
int Utils::doLoops(int lineCount, string line, vector<Pose> & points, int permLineCount, Utils & u)
{
	int counter = 0; //count number of tokens in line
	istringstream iss(line); //create an stream from the single line
	string number;
	while(iss >> number) //loop through all tokens in the line
	{
		counter++; //increment the token counter
		char b = u.checkChars(number); //check if token = valid double
		if(b != '1') //if not, throw an error
		{
			cerr << "error : invalid character " << b << " on line " << lineCount
			<< endl;
			return -1;
		}
	}
	if(counter == 75) //check to see if the number of tokens is 75
	{
		istringstream iss2(line);
		u.addPts(iss2, points); //add the line to the points vector
	}
	else //if not 75 tokens, throw an error
	{
		//print error about wrong number of numbers
		if(lineCount == permLineCount && counter == 0)
		{
			//ok
		}
		else
		{
			cerr << "error : " << counter << " numbers" 						" on line " << lineCount << ", should be 75" 						<< endl;
			return -1;
		}
	}
	return 0;
}
/*param1: points = poses to process
 *param2: number of lines in file
 *return: a Point3D that represents the average X, Y, and Z of all first points on each line
 *function: takes a vector of poses and computes the average of all the first points (base of spine)
*/
Point3D Utils::getAvgPoint(vector<Pose> & points, int lineCount)
{
	double totalX = 0;
	double totalY = 0;
	double totalZ = 0;
	double avgX = -10;
	double avgY = -10;
	double avgZ = -10;
	for(unsigned int x = 0; x < points.size(); x++)
	{
		totalX += points[x][0].X();
		totalY += points[x][0].Y();
		totalZ += points[x][0].Z();
	}
	if(lineCount != 0)
	{
		avgX = totalX/lineCount;
		avgY = totalY/lineCount;
		avgZ = totalZ/lineCount;
	}
	else
	{
		cerr << "error: divide by zero getting average point" << endl;
	}
	Point3D p = Point3D(avgX, avgY, avgZ);
	return p;
}

/*
 *param1: points = poses to process
 *param2: newPoints = where the translated points will go
 *param3: avgPoint = the average of all points relating to the base of the spine in all poses
 *return: the maximum absolute value of all coordinates AFTER translation
 *function: takes a vector of poses, translates them based on the average point, and puts the translated 	points and poses into a new vector
*/
double Utils::createTranslatedVector(const vector<Pose> & points, vector<Pose> & newPoints, const Point3D & avgPoint)
{
	double maxVal = 0.0;
	for(unsigned int x = 0; x < points.size(); x++)
	{
		Pose pose = points[x];
		vector<Point3D> pts;
		Pose newPose = Pose(pts);
		for(unsigned int y = 0; y < pose.size(); y++)
		{
			Point3D p = pose[y];
			Point3D newPoint = Point3D(p.X() - avgPoint.X(), p.Y() - avgPoint.Y(), p.Z() - avgPoint.Z());
			if(abs(newPoint.X()) > maxVal)
			{
				maxVal = abs(newPoint.X());
			}
			if(abs(newPoint.Y()) > maxVal)
			{
				maxVal = abs(newPoint.Y());
			}
			if(abs(newPoint.Z()) > maxVal)
			{
				maxVal = abs(newPoint.Z());
			}
			newPose.push_back(newPoint);
		}
		newPoints.push_back(newPose);
	}
	return maxVal;
}

vector<Pose> Utils::getNormalizedPoints(const vector<Pose> & newPoints, double maxVal, vector<Pose> & finalPoints)
{
	for(unsigned int x = 0; x < newPoints.size(); x++) //loop through vectors of points
	{
		if(maxVal != 1)
		{
			if(maxVal == 0)
			{
				cerr << "error : divide by 0 when scaling" << endl;
				vector<Point3D> pts;
				Pose pose = Pose(pts);
				Point3D p = Point3D(1, 2, 3);
				pose.push_back(p);
				finalPoints.push_back(pose);
				return finalPoints;
			}
			Pose pose = newPoints[x];//get pose
			vector<Point3D> pts;
			Pose newPose = Pose(pts);
			for(unsigned int y = 0; y < pose.size(); y++)
			{
				//vec[y] represents a point in the pose
				pose[y].normalizeX(maxVal);
				pose[y].normalizeY(maxVal);
				pose[y].normalizeZ(maxVal);
				newPose.push_back(pose[y]);
			}
			finalPoints.push_back(newPose);
		}
	}
	return finalPoints;
}

void Utils::writeDistances(ofstream & ofs, const vector<Pose> & finalPoints)
{
	for(unsigned int x = 0; x < finalPoints.size()-1; x++)
	{
		Pose p1 = finalPoints[x];
		Pose p2 = finalPoints[x+1];
		double distance = 0.0;
		for(unsigned int y = 0; y < p1.size(); y++)
		{
			double xDiff = pow(p1[y].X() - p2[y].X(), 2);
			double yDiff = pow(p1[y].Y() - p2[y].Y(), 2);
			double zDiff = pow(p1[y].Z() - p2[y].Z(), 2);
			distance += sqrt(xDiff + yDiff + zDiff);		
		}
		ofs << distance << endl;
	}
}



