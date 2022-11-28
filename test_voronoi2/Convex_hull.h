#pragma once
#ifndef CONVEX_HULL_H
#define CONVEX_HULL_H

#include <vector>

#include "Point2D.h"

using namespace std;

class Convex_hull
{
public:
	vector<point2d> convex_hull(vector<point2d> coor);
	int orientation(point2d on_hull, point2d next_point, point2d coor);
	double distance(point2d next_point, point2d coor);
	bool isNear(point2d O, point2d A, point2d B);
};

#endif