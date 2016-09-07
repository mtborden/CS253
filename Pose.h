/*! \file Pose.h
    \brief Contains the Pose class declaration (header)
*/
#ifndef POSE_H_INCLUDED
#define POSE_H_INCLUDED

#include<vector>
#include<Point3D.h>
using namespace std;


/*! 
  Pose class is available in order to easily understand the concept of "vector of vectors."  Since this 	concept can be quite confusing, it will be easier to see that it truly means "vector of poses." 	Furthermore, since a pose is easily understood as a vector of points, the true meaning is seen.
 */
class Pose {
public:
  /// Constructor with up to 3 coordinate arguments. All arguments default to 0.
  Pose(vector<Point3D> points) : points(points) {}

	void push_back(Point3D & point);

	Point3D& operator[](const int index);
	inline unsigned int size() const {return points.size();}
	
	bool operator() (Pose b, int index);

  vector<Point3D> points;
};

#endif // POSE_H_INCLUDED
