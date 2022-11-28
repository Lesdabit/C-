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
	vector<point2d> point_list;
	vector<edge> hyper_plane_list;
	vector<edge> voronoi_list;

	VoronoiDiagram voronoi(vector<point2d> points);
	VoronoiDiagram two_voronoi(vector<point2d> points);
	VoronoiDiagram three_voronoi(vector<point2d> points);
	bool isCollinear(point2d a, point2d b, point2d c);
	vector<vector<point2d>> divide(vector<point2d> points, vector<point2d> left_points, vector<point2d> right_points);
	VoronoiDiagram merge(VoronoiDiagram vl, VoronoiDiagram vr, VoronoiDiagram v);
	vector<edge> GetTangent(vector<point2d> chL, vector<point2d> chR);

	point2d swap(double a, double b);
};

#endif