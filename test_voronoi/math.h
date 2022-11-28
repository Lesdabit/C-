#pragma once
#ifndef MATH_H
#define MATH_H

#include "Point2D.h"
#include "Edge.h"

#include <vector>

class math
{
public:
	point2d getMidpoint(point2d a, point2d b);
	point2d getNormal(point2d a, point2d b);
	point2d getVector(point2d a, point2d b);
	point2d getTriangleExcenterPoint(point2d a, point2d b, point2d c);
	double Cross(point2d a, point2d b);
	double Cross(point2d a, point2d b, point2d c);
	edge getBisector(edge e);
	edge getBisector_normal(edge e);
	point2d getIntersection(edge e, edge e2);
	point2d getIntersection_normal(edge e1, edge e2);
	point2d Multi(point2d p, double v); //vector expand
	vector<point2d> bubblesort(vector<point2d> points);//counter sort
	vector<point2d> sort(vector<point2d> points);
	point2d GetCenterPoint(vector<point2d> center);
	bool search(point2d key, vector<point2d> points);
	bool inLine(point2d p, edge e);
};

#endif