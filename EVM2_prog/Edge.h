#pragma once
#ifndef EDGE_H
#define EDGE_H

#include "Point2D.h"

using namespace std;

class edge
{
public:
	point2d a;
	point2d b;
	point2d mid_a;
	point2d mid_b;

	edge(point2d a, point2d b, point2d mid_a, point2d mid_b);
};

#endif