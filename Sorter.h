/*! \file Sorter.h
    \brief Contains the Pose class declaration (header)
*/
#ifndef SORTER_H_INCLUDED
#define SORTER_H_INCLUDED

#include<vector>
#include<Point3D.h>
#include<Pose.h>

using namespace std;

class Sorter {
public:  
  Sorter(int i) : index(i-1){}

  bool operator() (const vector<double> & vec1, const vector<double> & vec2) {return vec1[index] < vec2[index];};
  int index;
};

#endif // SORTER_H_INCLUDED
