#include <vector>

#include "Point2D.h"
#include "Edge.h"
#include "VoronoiDiagram.h"

using namespace std;
point p;

vector<edge> VoronoiDiagram::voronoi(vector<point2d> points)
{
	vector<edge> edge_list;
	if (points.size() == 1) return edge_list;
	else if (points.size() == 2) edge_list = two_voronoi(points);
	else if (points.size() == 3) edge_list = three_voronoi(points);
	else
	{
		divide();
		vector<edge> vl = voronoi(left_points);
		vector<edge> vr = voronoi(right_points);
		edge_list = merge(vl, vr);
	}
	return edge_list;
}

vector<edge> VoronoiDiagram::two_voronoi(vector<point2d> points)
{
	vector<edge> edge_list;
	vector<point2d> mid_point;
	point2d midpoint = point2d(abs(points[0].x - points[1].x) / 2, abs(points[0].y - points[1].y) / 2);
	point2d diff = point2d(points[0].x - points[1].x, points[0].y - points[1].y);
	swap(diff.x, diff.y);
	point2d normal = point2d(diff.x * -1, diff.y);
	mid_point.push_back(midpoint);
	mid_point.push_back(point2d(midpoint.x + normal.x, midpoint.y + normal.y));
	edge_list.push_back(edge(points[0], points[1], mid_point[0], mid_point[1]));
}

vector<edge> VoronoiDiagram::three_voronoi(vector<point2d> points)
{
	vector<edge> edge_list;
	for (int i = 0; i < 1; i++)
	{
		for (int j = i + 1; j < 2; j++)
		{
			vector<point2d> mid_point;
			point2d midpoint = point2d(abs(points[i].x - points[j].x) / 2, abs(points[i].y - points[j].y) / 2);
			point2d diff = point2d(points[i].x - points[j].x, points[i].y - points[j].y);
			swap(diff.x, diff.y);
			point2d normal = point2d(diff.x * -1, diff.y);
			mid_point.push_back(midpoint);
			mid_point.push_back(point2d(midpoint.x + normal.x, midpoint.y + normal.y));
			edge_list.push_back(edge(points[i], points[j], mid_point[i], mid_point[j]));
		}
	}
	return edge_list;
}

void VoronoiDiagram::divide()
{
	int half_point_num = p.points.size() / 2;
	for (int i = 0; i < half_point_num; i++)
	{
		if (p.points[i].x < half_point_num) VoronoiDiagram::left_points.push_back(p.points[i]);
		else VoronoiDiagram::right_points.push_back(p.points[i]);
	}
}

vector<edge> VoronoiDiagram::merge(vector<edge> vl, vector<edge> vr)
{
	vector<edge> edge_list;

	return edge_list;
}