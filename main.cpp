/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: mtborden
 *
 * Created on January 28, 2016, 1:51 PM
 */

#include <cstdlib>
#include <vector>
#include <PoseDisplay.h>
#include <Point3D.h>
#include <iostream>
#include <sstream>
#include <string>
#include <istream>
#include <ostream>
#include <Sorter.h>
#include <algorithm>

using namespace std;

/*
 * 
 */

void addPts(istringstream & ss, vector< vector<double> > & points);
char checkChars(string s);

int main(int argc, char** argv) 
{
	if(argc != 5)
	{
		cerr << "error : " << argc << " arguments, should be 5" << endl;
		return -1;
	}	
	vector< vector<double> > points;
	string line;
	int permLineCount = 0;
	
	//instantiate an ifstream from the input file specified in the terminal
	ifstream tempifs(argv[1]);

	if(tempifs.is_open())
	{
		while(getline(tempifs,line))
		{
			permLineCount++;
		}
		tempifs.close();
	}

	ifstream ifs(argv[1]);

	//make sure the stream/input file are valid
	if(ifs.is_open())
	{
		int lineCount = 0; //count number of lines
		
		//loop through every line in the file
		while(getline(ifs, line))
		{
			lineCount++; //increment the line count
			int counter = 0; //count number of tokens in line
			istringstream iss(line); //create an stream from the single line
			string number;
			while(iss >> number) //loop through all tokens in the line
			{
				counter++; //increment the token counter
				char b = checkChars(number); //check if token = valid double
				if(b != '1') //if not, throw an error
				{
					cerr << "error : invalid character "					<< b << " on line " << lineCount
					<< endl;
					return -1;
				}
			}
			
			if(counter == 75) //check to see if the number of tokens is 75
			{
				istringstream iss2(line);
				addPts(iss2, points); //add the line to the points vector
			}
			else //if not 75 tokens, throw an error
			{
				//print error about wrong number of numbers
				if(lineCount != permLineCount)
				{
					cerr << "error : " << counter << " numbers" 						" on line " << lineCount << ", should be 75" 						<< endl;
					return -1;
				}
			}					
		}
		ifs.close();
		string option(argv[3]);
		if(option.compare("sort") == 0)
		{
			int index = atoi(argv[4]);
			if(index < 1 || index > 75)
			{
				cerr << "error : invalid index" << endl;
				return -1;
			}
			Sorter s(index);
			sort(points.begin(), points.end(), s);
			vector<vector<Point3D>> posePoints;
			for(unsigned int x = 0; x < points.size(); x++)
			{
				vector<double> vec = points[x];
				vector<Point3D> newVec;
				for(int y = 0; y < 25; y++)
				{
					double xCoord = vec[(y*3)];
					double yCoord = vec[(y*3) + 1];
					double zCoord = vec[(y*3) + 2];
					Point3D p(xCoord, yCoord, zCoord);
					newVec.push_back(p);
				}
				posePoints.push_back(newVec);
			}
			PoseDisplay pd(argv[2]); //instantiate a PoseDisplay object
			for(unsigned int x = 0; x < posePoints.size(); x++) //loop through vectors of points
			{
				pd.Pose(posePoints[x]); //display the pose
			}
		}
		else if(option.compare("swap") == 0)
		{
			unsigned int index = atoi(argv[4]);
			if(index < 1 || index > points.size())
			{
				cerr << "error : invalid index" << endl;
				return -1;
			}
			//vector<double> vec1 = points[0];
			//vector<double> vec2 = points[index];
			swap(points[0], points[index-1]);
			vector<vector<Point3D>> posePoints;
			for(unsigned int x = 0; x < points.size(); x++)
			{
				vector<double> vec = points[x];
				vector<Point3D> newVec;
				for(int y = 0; y < 25; y++)
				{
					double xCoord = vec[(y*3)];
					double yCoord = vec[(y*3) + 1];
					double zCoord = vec[(y*3) + 2];
					Point3D p(xCoord, yCoord, zCoord);
					newVec.push_back(p);
				}
				posePoints.push_back(newVec);
			}
			PoseDisplay pd(argv[2]); //instantiate a PoseDisplay object
			for(unsigned int x = 0; x < posePoints.size(); x++) //loop through vectors of points
			{
				pd.Pose(posePoints[x]); //display the pose
			}
		}
		else if(option.compare("left") == 0)
		{
			vector<vector<double>> newPoints;
			int counter = 1;
			for(unsigned int x = 0; x < points.size(); x++)
			{
				vector<double> vec = points[x];
				if(vec[20] <= vec[32])
				{
					newPoints.push_back(vec);
					counter++;
				}
			}
			vector<vector<Point3D>> posePoints;
			for(unsigned int x = 0; x < newPoints.size(); x++)
			{
				vector<double> vec = newPoints[x];
				vector<Point3D> newVec;
				for(int y = 0; y < 25; y++)
				{
					double xCoord = vec[(y*3)];
					double yCoord = vec[(y*3) + 1];
					double zCoord = vec[(y*3) + 2];
					Point3D p(xCoord, yCoord, zCoord);
					newVec.push_back(p);
				}
				posePoints.push_back(newVec);
			}
			PoseDisplay pd(argv[2]); //instantiate a PoseDisplay object
			for(unsigned int x = 0; x < posePoints.size(); x++) //loop through vectors of points
			{
				pd.Pose(posePoints[x]); //display the pose
			}
		}
		else if(option.compare("transform") == 0)
		{
			double toAdd = (double)(atoi(argv[4]));
			for(unsigned int x = 0; x < points.size(); x++)
			{
				for(unsigned int y = 0; y < points[x].size(); y++)
				{
					points[x][y] = points[x][y] + toAdd;
				}
			}
			vector<vector<Point3D>> posePoints;
			for(unsigned int x = 0; x < points.size(); x++)
			{
				vector<double> vec = points[x];
				vector<Point3D> newVec;
				for(int y = 0; y < 25; y++)
				{
					double xCoord = vec[(y*3)];
					double yCoord = vec[(y*3) + 1];
					double zCoord = vec[(y*3) + 2];
					Point3D p(xCoord, yCoord, zCoord);
					newVec.push_back(p);
				}
				posePoints.push_back(newVec);
			}
			PoseDisplay pd(argv[2]); //instantiate a PoseDisplay object
			for(unsigned int x = 0; x < posePoints.size(); x++) //loop through vectors of points
			{
				pd.Pose(posePoints[x]); //display the pose
			}
		}
		else if(option.compare("rotate") == 0)
		{
			int num = atoi(argv[4]);
			vector<vector<double>> newPoints;
			for(unsigned int x = 0; x < points.size(); x++)
			{
				vector<double> vec = points[x];
				vector<double> newVec;
				for(unsigned int y = 0; y < vec.size(); y++)
				{
					newVec.push_back(0.0);
				}
				newPoints.push_back(newVec);				
			}
			for(unsigned int x = 0; x < points.size(); x++)
			{
				vector<double> vec = points[x];
				for(unsigned int y = 0; y < vec.size(); y++)
				{
					unsigned int otherIndex = y + num;
					if(otherIndex > vec.size()-1)
					{
						otherIndex %= vec.size();
					}
					cout << otherIndex << endl;
					newPoints[x][y] = points[x][otherIndex];
				}								
			}
			vector<vector<Point3D>> posePoints;
			for(unsigned int x = 0; x < newPoints.size(); x++)
			{
				vector<double> vec = newPoints[x];
				vector<Point3D> newVec;
				for(int y = 0; y < 25; y++)
				{
					double xCoord = vec[(y*3)];
					double yCoord = vec[(y*3) + 1];
					double zCoord = vec[(y*3) + 2];
					Point3D p(xCoord, yCoord, zCoord);
					newVec.push_back(p);
				}
				posePoints.push_back(newVec);
			}
			PoseDisplay pd(argv[2]); //instantiate a PoseDisplay object
			for(unsigned int x = 0; x < posePoints.size(); x++) //loop through vectors of points
			{
				pd.Pose(posePoints[x]); //display the pose
			}
		}
		 //close the stream
		/*PoseDisplay pd(argv[2]); //instantiate a PoseDisplay object
		for(unsigned int x = 0; x < points.size(); x++) //loop through vectors of points
		{
			pd.Pose(points[x]); //display the pose
		}*/
	}
	else // throw error if input file not valid
	{
		cerr << "error : input file not valid" << endl;
		return -1;
	}
    return 0;
}

//param1: ss = stream containing a line
//param2: points = vector of vectors of points
//return: N/A
//function: takes a line of points, puts them into a vector, then adds that
//		vector to the larger vector
void addPts(istringstream & ss, vector< vector<double> > & points)
{
	vector<double> linePoints;
	for(int i = 0; i < 75; i++)
	{		
		double x;
		ss >> x;
		linePoints.push_back(x);
	}
	points.push_back(linePoints);
}

//param1: s = string that should represent a double
//return: '1' if all characters are ok, the bad character if not
//function: takes a string, checks each character to see if it is valid for a double
char checkChars(string s)
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

