/*! \file Point3D.h
    \brief Contains the Point3D class declaration (header)

    Unless/until extended by students, there is no Point3D.cpp file, as all the methods of Point3D are inline.
*/
#ifndef POINT3D_H_INCLUDED
#define POINT3D_H_INCLUDED

#include<iostream>
#include <string>
using std::ostream;

//! Real-valued points in 3 space, i.e. (x, y, z)
/*! 
  Point3D is the base class for storing points in 3 space and accessing their coordinates. The
  PoseDisplay class will write out and display vectors of 25 Point3Ds (obviously, their order matters!).
  
  The Point3D class as implemented here is sufficient for Programming Assignment #1. As the semester
  progresses, however, students are encouraged to extend this class by adding new methods, new overloaded
  operators, and new data fields as necessary. As long as the X(), Y(), Z() methods and the << operator
  below are retained, the PoseDisplay class will work with modifications of the Pose3D class. Students
  are also encouraged new classes, including classes that might contain Point3D fields.
 */
class Point3D {
public:
  /// Constructor with up to 3 coordinate arguments. All arguments default to 0.
  Point3D(double x = 0.0, double y = 0.0, double z = 0.0) : x(x), y(y), z(z) {}

  inline double X() const {return x;}          //!< Return the X coordinate of the point
  inline double Y() const {return y;}          //!< Return the Y coordinate of the point
  inline double Z() const {return z;}          //!< Return the Z coordindate of the point

	inline void normalizeX(double maxVal) {x = x/maxVal;}
	inline void normalizeY(double maxVal) {y = y/maxVal;}
	inline void normalizeZ(double maxVal) {z = z/maxVal;}

protected:
  double x;                                     //!< x coordinate (real value)
  double y;                                     //!< y coordinate (real value)
  double z;                                     //!< z coordinate (real value)
};

/// Overload of << operator for Point3D. Prints out as three real values, separated by spaces with a trailing space.
inline ostream& operator << (ostream& ostr, const Point3D& pt) {ostr << pt.X() << " " << pt.Y() << " " << pt.Z() << " "; return ostr;}

#endif // POINT3D_H_INCLUDED
