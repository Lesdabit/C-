#pragma once
#ifndef MATH_H
#define MATH_H

#include "Point2D.h"
#include "Edge.h"

class math
{
public:
	point2d getMidpoint(point2d a, point2d b);
	point2d getNormal(point2d a, point2d b);
	point2d getVector(point2d a, point2d b);
	double Cross(point2d a, point2d b);
	double Cross(point2d a, point2d b, point2d c);
	edge getBisector(edge e);
	point2d getIntersection(edge e1, edge e2);
	point2d Multi(point2d p, double v); //vector expand
	vector<point2d> bubblesort(vector<point2d> points);
	bool binary_search(int l, int m, point2d key, vector<point2d> points);
};

#endif