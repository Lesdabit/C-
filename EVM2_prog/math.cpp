#include <iostream>

#include "math.h"
#include "VoronoiDiagram.h"

using namespace std;
VoronoiDiagram vd;

point2d math::getMidpoint(point2d a, point2d b)
{
	return point2d((a.x + b.x) / 2, (a.y + b.y) / 2);
}

point2d math::getNormal(point2d a, point2d b)
{
	point2d vector = getVector(a, b);
	vector = vd.swap(vector.x, vector.y);
	return point2d(vector.x * -1, vector.y);
}

point2d math::getVector(point2d a, point2d b)
{
	return point2d(a.x - b.x, a.y - b.y);
}

edge math::getBisector(edge e)
{
	point2d midpoint = point2d((e.a.x + e.b.x) / 2, (e.a.y + e.b.y) / 2);
	point2d normal = getNormal(e.a, e.b);
	return edge(e.a, e.b, midpoint, point2d(midpoint.x + normal.x , midpoint.y + normal.y));
}

point2d math::getIntersection(edge e1, edge e2)
{
	point2d a = getVector(e1.a, e1.b), b = getVector(e2.a, e2.b), s = getVector(e1.a, e2.a);
	double c1 = Cross(a, b);
	double c2 = Cross(s, b);
	double c4 = Cross(s, a);

	if (c1 < 0)
	{
		c1 = -c1;
		c2 = -c2;
		c4 = -c4;
	}

	if (c1 != 0 && c2 >= 0 && c2 <= c1 && c4 >= 0 && c4 <= c1) return Multi(a, (c2 / c1));
	else return NULL;
}

double math::Cross(point2d a, point2d b)
{
	return a.x * b.y + a.y * b.x;
}

double math::Cross(point2d o, point2d a, point2d b)
{
	return (a.x - o.x) * (b.y - o.y) - (a.y - o.y) * (b.x - o.x);
}

point2d math::Multi(point2d p, double v)
{
	return point2d(p.x * v, p.y * v);
}