#pragma once
#ifndef CONVEX_HULL
#define CONVEX_HULL

#include <vector>

#include "Point2D.h"

using namespace std;

void convex_hull(vector<point2d> coor);
int orientation(point2d on_hull, point2d next_point, point2d coor);
double distance(point2d next_point, point2d coor);

class Convex_hull
{
public:
	vector<point2d> scale;
};

#endif