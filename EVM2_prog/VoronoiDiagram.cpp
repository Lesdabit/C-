#include <vector>

#include "Point2D.h"
#include "Edge.h"
#include "VoronoiDiagram.h"
#include "Convex_hull.h"

using namespace std;
VoronoiDiagram vd;
Convex_hull ch;

VoronoiDiagram VoronoiDiagram::voronoi(vector<point2d> points)
{
	VoronoiDiagram v;
	vector<point2d> left_points;
	vector<point2d> right_points;
	VoronoiDiagram vl;
	VoronoiDiagram vr;
	if (points.size() == 1) return v;
	else if (points.size() == 2) return two_voronoi(points);
	else if (points.size() == 3) return three_voronoi(points);
	else
	{
		vector<vector<point2d>> list = divide(points, left_points, right_points);
		vl.point_list = list[0];
		vr.point_list = list[1];
		vl = voronoi(list[0]);
		vr = voronoi(list[1]);
		v = merge(vl, vr);
	}
	return v;
}

VoronoiDiagram VoronoiDiagram::two_voronoi(vector<point2d> points)
{
	VoronoiDiagram v;
	vector<point2d> mid_point;
	point2d midpoint = point2d((points[0].x + points[1].x) / 2, (points[0].y + points[1].y) / 2);
	point2d diff = point2d(points[0].x - points[1].x, points[0].y - points[1].y);
	diff = swap(diff.x, diff.y);
	point2d normal = point2d(diff.x * -1, diff.y);
	mid_point.push_back(midpoint);
	mid_point.push_back(point2d(midpoint.x + normal.x, midpoint.y + normal.y));
	v.voronoi_list.push_back(edge(points[0], points[1], mid_point[0], mid_point[1]));
	return v;
}

VoronoiDiagram VoronoiDiagram::three_voronoi(vector<point2d> points)
{
	vector<point2d> mid_point;
	VoronoiDiagram v;
	if (isCollinear(points[0], points[1], points[2]))
	{
		for (int i = 0; i < 2; i++)
		{
			point2d midpoint = point2d((points[i].x + points[i + 1].x) / 2, (points[i].y + points[i + 1].y) / 2);
			point2d diff = point2d(points[i].x - points[i + 1].x, points[i].y - points[i + 1].y);
			diff = swap(diff.x, diff.y);
			point2d normal = point2d(diff.x * -1, diff.y);
			mid_point.push_back(midpoint);
			mid_point.push_back(point2d(midpoint.x + normal.x, midpoint.y + normal.y));
			v.voronoi_list.push_back(edge(points[i], points[i + 1], mid_point[i], mid_point[i + 1]));
		}
	}
	else
	{
		for (int i = 0; i < 2; i++)
		{
			for (int j = i + 1; j < 3; j++)
			{
				point2d midpoint = point2d((points[i].x + points[j].x) / 2, (points[i].y + points[j].y) / 2);
				point2d diff = point2d(points[i].x - points[j].x, points[i].y - points[j].y);
				diff = swap(diff.x, diff.y);
				point2d normal = point2d(diff.x * -1, diff.y);
				mid_point.push_back(midpoint);
				mid_point.push_back(point2d(midpoint.x + normal.x, midpoint.y + normal.y));
				v.voronoi_list.push_back(edge(points[i], points[j], mid_point[i], mid_point[j]));
			}
		}
	}
	return v;
}

bool VoronoiDiagram::isCollinear(point2d a, point2d b, point2d c)
{
	if (a.x * (b.y - c.y) + b.x * (c.y - a.y) + c.x * (a.y - b.y) == 0)
	{
		return true;
	}
	return false;
}

vector<vector<point2d>> VoronoiDiagram::divide(vector<point2d> points, vector<point2d> left_points, vector<point2d> right_points)
{
	vector<vector<point2d>> edge_list;
	double half_point_num = points.size() / 2.0;
	for (int i = 0; i < points.size(); i++)
	{
		if (i < half_point_num) left_points.push_back(points[i]);
		else right_points.push_back(points[i]);
	}
	edge_list.push_back(left_points);
	edge_list.push_back(right_points);
	return edge_list;
}

VoronoiDiagram VoronoiDiagram::merge(VoronoiDiagram vl, VoronoiDiagram vr)
{
	VoronoiDiagram v;
	vector<edge> edge_list;
	vector<point2d> chL = ch.convex_hull(vl.point_list);
	return v;
}

point2d VoronoiDiagram::swap(double a, double b)
{
	return point2d(b, a);
}