#include <iostream>

#include "Convex_hull.h"
#include "math.h"

using namespace std;
math M;

/*vector<point2d> Convex_hull::convex_hull(vector<point2d> coor)
{
	vector<point2d> scale;
	point2d on_hull;
	point2d second_point;
	point2d next_point;
	on_hull = coor[0];
	bool hasbetter = false;
	int tmp = 0;

	for (int i = 1; i < coor.size(); i++)
	{
		if (coor[i].x < on_hull.x) on_hull = coor[i];
		else if (coor[i].x == on_hull.x && coor[i].y < on_hull.y) on_hull = coor[i];
	}

	while (true)
	{
		scale.push_back(on_hull);
		next_point = coor[0];
		for (int i = 0; i < coor.size(); i++)
		{
			int o = orientation(on_hull, next_point, coor[i]);
			if ((next_point.x == on_hull.x && next_point.y == on_hull.y) || o == 1 || o == 0)
			{
				if (o == 0 && isNear(on_hull, next_point, coor[i]))
				{
					tmp = i;
				}
				else
				{
					next_point = coor[i];
				}
			}

			on_hull = next_point;
			if (on_hull.x == scale[0].x && on_hull.y == scale[0].y)
			{
				//scale.push_back(scale[0]);
				break;
			}
		}
		return scale;
	}
}*/

vector<point2d> Convex_hull::convex_hull(vector<point2d> coor)
{
	vector<int> chList;
	vector<int> visited;
	bool isAllCollinear = true;
	bool hasbetter = false;
	int tmp = 0;

	int start = 0;

	int m = 0;
	chList.push_back(start);

	for (int i = 0; i < coor.size(); i++) visited.push_back(i);

	int next = visited[1];

	for (m = 1; m < visited.size(); m++)
	{
		hasbetter = false;
		for (int i : visited)
		{
			tmp = next;

			if (i == next || i == chList[m - 1]) continue;
			double c = M.Cross(coor[chList[m - 1]], coor[i], coor[next]);

			if (c < 0)
			{
				next = i;
				isAllCollinear = false;
				hasbetter = true;
			}
			else if (c == 0 && (isNear(coor[chList[m - 1]], coor[i], coor[next]) == true) && i != visited[0])
			{
				tmp = i;
			}
			else if (c > 0)
			{
				isAllCollinear = false;
			}
		}

		if (!hasbetter) next = tmp;
		if (next == start) break;

		vector<int> visited_delete;
		chList.push_back(next);
		for (int i = 0; i < visited.size(); i++) if (next != i) visited_delete.push_back(i);

		if (visited_delete.size() > 1) next = visited_delete[1];
		else next = visited_delete[0];
	}

	if (isAllCollinear)
	{
		for (int i = chList.size() - 2; i > 0; i--)
		{
			chList.push_back(chList[i]);
		}
	}

	vector<point2d> convexHull;
	for (int i : chList) convexHull.push_back(coor[i]);
	return convexHull;
}

int Convex_hull::orientation(point2d on_hull, point2d next_point, point2d coor)
{
	double m = (coor.y - next_point.y) * (next_point.x - on_hull.x) - (next_point.y - on_hull.y) * (coor.x - next_point.x);
	if (m > 0) return 1;
	else if (m < 0) return -1;
	else return 0;
}

double Convex_hull::distance(point2d next_point, point2d coor)
{
	double d = sqrt(pow(next_point.x - coor.x, 2) + pow(next_point.y - coor.y, 2));
	return d;
}

bool Convex_hull::isNear(point2d O, point2d A, point2d B)
{
	if(distance(O, A) < distance(O, B)) return true;
	return false;
}