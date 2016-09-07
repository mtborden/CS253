#ifndef UTILS_H_INCLUDE
#define UTILS_H_INCLUDE

/*! \file Utils.h: Declares the Complex number class 
 */

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


/*! \brief A class that simply does operations on Point3D and PoseDisplay classes.
	Basically exists simply to make the code in main much cleaner.
 */
class Utils {
public:
	Utils(int ind){index = ind;}

	void addPts(istringstream & ss, vector<Pose> & points);
	char checkChars(string s);
	int getPermLineCount(ifstream & ifs);
	int doLoops(int lineCount, string line, vector<Pose> & points, int permLineCount, 			Utils & u);
	Point3D getAvgPoint(vector<Pose> & points, int lineCount);
	double createTranslatedVector(const vector<Pose> & points, vector<Pose> & newPoints, const Point3D & avgPoint);
	vector<Pose> getNormalizedPoints(const vector<Pose> & newPoints, double maxVal, vector<Pose> & finalPoints);
	void writeDistances(ofstream & ofs, const vector<Pose> & finalPoints);
	
	int index;
};


#endif // UTILS_H_INCLUDE

