#include <iostream>

#include "Convex_hull.h"
#include "Point2D.h"

using namespace std;

void convex_hull(vector<point2d> coor)
{
	point2d on_hull;
	point2d second_point;
	point2d next_point;
	on_hull = coor[0];

	for (int i = 1; i < coor.size(); i++)
	{
		if (coor[i].x < on_hull.x) on_hull = coor[i];
		else if (coor[i].x == on_hull.x && coor[i].y < on_hull.y) on_hull = coor[i];
	}

	while (true)
	{
		Convex_hull::scale.push_back(on_hull);
		next_point = coor[0];
		for (int i = 0; i < coor.size(); i++)
		{
			int o = orientation(on_hull, next_point, coor[i]);
			if ((next_point.x == on_hull.x && next_point.y == on_hull.y) || o == 1
				|| (o == 0 && distance(on_hull, next_point) < distance(on_hull, coor[i]))) next_point = coor[i];
		}
		on_hull = next_point;
		if (on_hull.x == Convex_hull::scale[0].x && on_hull.y == Convex_hull::scale[0].y) break;
	}
}

int orientation(point2d on_hull, point2d next_point, point2d coor)
{
	double m = (coor.y - next_point.y) * (next_point.x - on_hull.x) - (next_point.y - on_hull.y) * (coor.x - next_point.x);
	if (m > 0) return 1;
	else if (m < 0) return -1;
	else return 0;
}

double distance(point2d next_point, point2d coor)
{
	double d = sqrt(pow(next_point.x - coor.x, 2) + pow(next_point.y - coor.y, 2));
	return d;
}