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
	return point2d(b.x - a.x, b.y - a.y);
}

point2d math::getTriangleExcenterPoint(point2d a, point2d b, point2d c)
{
	if ((a.x == b.x && a.y == b.y) && (a.x == c.x && a.y == c.y)) return a;

	double x1 = a.x, x2 = b.x, x3 = c.x, y1 = a.y, y2 = b.y, y3 = c.y;
	double c1 = (pow(x1, 2) + pow(y1, 2)) * (y2 - y3) + (pow(x2, 2) + pow(y2, 2)) * (y3 - y1) + (pow(x3, 2) + pow(y3, 2)) * (y1 - y2);
	double c2 = (pow(x1, 2) + pow(y1, 2)) * (x3 - x2) + (pow(x2, 2) + pow(y2, 2)) * (x1 - x3) + (pow(x3, 2) + pow(y3, 2)) * (x2 - x1);
	double c3 = ((y2 - y3) * x1 + (y3 - y1) * x2 + (y1 - y2) * x3) * 2;
	double x = c1 / c3;
	double y = c2 / c3;
	return point2d((double)(x), (double)(y));
}

edge math::getBisector(edge e)
{
	point2d midpoint = getMidpoint(e.a, e.b);
	point2d normal1 = getNormal(e.a, e.b);
	point2d normal2 = getNormal(e.b, e.a);
	return edge(Multi(point2d(midpoint.x + normal1.x, midpoint.y + normal1.y), 10), Multi(point2d(midpoint.x + normal2.x,
		midpoint.y + normal2.y), 10), e.a, e.b);
}

edge math::getBisector_normal(edge e)
{
	point2d midpoint = getMidpoint(e.a, e.b);
	point2d normal1 = getNormal(e.a, e.b);
	point2d normal2 = getNormal(e.b, e.a);
	return edge(point2d(midpoint.x + normal1.x, midpoint.y + normal1.y), point2d(midpoint.x + normal2.x,
		midpoint.y + normal2.y), e.a, e.b);
}

point2d math::getIntersection(edge e, edge e2)
{
	edge e1 = getBisector_normal(e);
	double m1 = (e1.b.y - e1.a.y) / (e1.b.x - e1.a.x);
	double a1, b1, c1;
	double a2, b2, c2;
	if (e1.a.x == e1.b.x)
	{
		a1 = 1, b1 = 0, c1 = e1.a.x;
	}
	else
	{
		a1 = m1, b1 = -1, c1 =  (-1) * m1 * e1.a.x + e1.a.y;
	}

	double m2 = (e2.b.y - e2.a.y) / (e2.b.x - e2.a.x);
	if (e2.a.x == e2.b.x)
	{
		a2 = 1, b2 = 0, c2 = e2.a.x;
	}
	else
	{
		a2 = m2, b2 = -1, c2 = (-1) * m2 * e2.a.x + e2.a.y;
	}

	return point2d((b1 * c2 - b2 * c1) / (b2 * a1 - b1 * a2), (a1 * c2 - c1 * a2) / (b1 * a2 - a1 * b2));
}

point2d math::getIntersection_normal(edge e1, edge e2)
{
	double m1 = (e1.b.y - e1.a.y) / (e1.b.x - e1.a.x);
	double a1, b1, c1;
	double a2, b2, c2;
	if (e1.a.x == e1.b.x)
	{
		a1 = 1, b1 = 0, c1 = e1.a.x;
	}
	else
	{
		a1 = m1, b1 = -1, c1 = (-1) * m1 * e1.a.x + e1.a.y;
	}

	double m2 = (e2.b.y - e2.a.y) / (e2.b.x - e2.a.x);
	if (e2.a.x == e2.b.x)
	{
		a2 = 1, b2 = 0, c2 = e2.a.x;
	}
	else
	{
		a2 = m2, b2 = -1, c2 = (-1) * m2 * e2.a.x + e2.a.y;
	}

	return point2d((b1 * c2 - b2 * c1) / (b2 * a1 - b1 * a2), (a1 * c2 - c1 * a2) / (b1 * a2 - a1 * b2));
}

double math::Cross(point2d a, point2d b)
{
	return a.x * b.y - a.y * b.x;
}

double math::Cross(point2d o, point2d a, point2d b)
{
	return (a.x - o.x) * (b.y - o.y) - (a.y - o.y) * (b.x - o.x);
}

point2d math::Multi(point2d p, double v)
{
	return point2d(p.x * v, p.y * v);
}

point2d math::GetCenterPoint(vector<point2d> points)
{
	double x = 0, y = 0;
	for (int i = 0; i < points.size(); i++)
	{
		x += points[i].x;
		y += points[i].y;
	}

	return point2d(x / points.size(), y / points.size());
}

vector<point2d> math::bubblesort(vector<point2d> points)
{
	point2d center = GetCenterPoint(points);

	for (int i = 0; i < points.size() - 1; i++)
	{
		for (int j = 0; j < points.size() - i - 1; j++)
		{
			if (Cross(center, points[j], points[j + 1]) > 0)
			{
				point2d p;
				p = points[j + 1];
				points[j + 1] = points[j];
				points[j] = p;
			}
		}
	}
	return points;
}

vector<point2d> math::sort(vector<point2d> points)
{
	for (int i = 0; i < points.size() - 1; i++)
	{
		for (int j = 0; j < points.size() - i - 1; j++)
		{
			if (points[j].x > points[j + 1].x)
			{
				point2d temp = points[j + 1];
				points[j + 1] = points[j];
				points[j] = temp;
			}
		}
	}

	for (int i = 0; i < points.size() - 1; i++)
	{
		for (int j = 0; j < points.size() - i - 1; j++)
		{
			if (points[j].x == points[j + 1].x && points[j].y > points[j + 1].y)
			{
				point2d temp = points[j + 1];
				points[j + 1] = points[j];
				points[j] = temp;
			}
		}
	}
	return points;
}

bool math::search(point2d key, vector<point2d> points)
{
	bool find = false;
	for (int i = 0; i < points.size(); i++) if (points[i].x == key.x && points[i].y == key.y) find = true;
	return find;
}

bool math::inLine(point2d p, edge e)
{
	double m = (e.b.y - e.a.y) / (e.b.x - e.a.x);
	double solve = m * p.x - m * e.a.x + e.a.y - p.y;
	if (solve == 0) return true;
	else return false;
}