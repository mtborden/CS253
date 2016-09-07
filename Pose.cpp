#include <cstdlib>
#include <vector>
#include <Point3D.h>
#include <Pose.h>

using namespace std;

/*! \file Pose.cpp: implements the Pose class */

void Pose::push_back(Point3D & point)
{
	points.push_back(point);
}

Point3D& Pose::operator[](const int index)
{
	return points[index];
}
