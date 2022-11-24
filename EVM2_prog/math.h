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
	edge getBisector(edge e);
	point2d getIntersection(edge e1, edge e2);
	point2d Multi(point2d p, double v); //vector expand
};

#endif