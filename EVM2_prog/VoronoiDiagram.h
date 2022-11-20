#pragma once
#ifndef VORONOIDIAGRAM_H
#define VORONOIDIAGRAM_H

#include <vector>

#include "Point2D.h"
#include "Edge.h"

using namespace std;

class VoronoiDiagram
{
public:
	vector<edge> hyper_plane_list;
	vector<edge> voronoi_list;

	vector<point2d> left_points;
	vector<point2d> right_points;

	vector<edge> voronoi(vector<point2d> points);
	vector<edge> two_voronoi(vector<point2d> points);
	vector<edge> three_voronoi(vector<point2d> points);
	void divide();
	vector<edge> merge(vector<edge> vl, vector<edge> vr);
};

#endif
