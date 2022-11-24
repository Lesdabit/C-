#include <vector>

#include "Point2D.h"
#include "Edge.h"
#include "VoronoiDiagram.h"
#include "Convex_hull.h"
#include "math.h"

using namespace std;
VoronoiDiagram vd;
Convex_hull ch;
math m;

VoronoiDiagram VoronoiDiagram::voronoi(vector<point2d> points)
{
	VoronoiDiagram v;
	VoronoiDiagram vl;
	VoronoiDiagram vr;
	if (points.size() == 1) return v;
	else if (points.size() == 2) return two_voronoi(points);
	else if (points.size() == 3) return three_voronoi(points);
	else
	{
		vector<point2d> left_points;
		vector<point2d> right_points;
		vector<vector<point2d>> list = divide(points, left_points, right_points);
		vl = voronoi(list[0]);
		vr = voronoi(list[1]);
		vl.point_list = list[0];
		vr.point_list = list[1];
		v = merge(vl, vr);
	}
	return v;
}

VoronoiDiagram VoronoiDiagram::two_voronoi(vector<point2d> points)
{
	VoronoiDiagram v;
	vector<point2d> mid_point;
	point2d midpoint = m.getMidpoint(points[0], points[1]);
	point2d normal = m.getNormal(points[0], points[1]);
	mid_point.push_back(midpoint);
	mid_point.push_back(point2d(midpoint.x + normal.x, midpoint.y + normal.y));
	v.voronoi_list.push_back(edge(points[0], points[1], mid_point[0], mid_point[1]));
	return v;
}

VoronoiDiagram VoronoiDiagram::three_voronoi(vector<point2d> points)
{
	vector<point2d> mid_point;
	VoronoiDiagram v;
	if (isCollinear(points[0], points[1], points[2]) == true)
	{
		for (int i = 0; i < 2; i++)
		{
			point2d midpoint = m.getMidpoint(points[i], points[i + 1]);
			point2d normal = m.getNormal(points[i], points[i + 1]);
			mid_point.push_back(midpoint);
			mid_point.push_back(point2d(midpoint.x + normal.x, midpoint.y + normal.y));
			v.voronoi_list.push_back(edge(points[i], points[i + 1], mid_point[i], mid_point[i + 1]));
		}
	}
	else
	{
		for (int i = 0; i < 3; i++)
		{
			for (int j = i + 1; j < 4; j++)
			{
				point2d midpoint = m.getMidpoint(points[i], points[j % 3]);
				point2d normal = m.getNormal(points[i], points[j % 3]);
				mid_point.push_back(midpoint);
				mid_point.push_back(point2d(midpoint.x + normal.x, midpoint.y + normal.y));
				v.voronoi_list.push_back(edge(points[i], points[j % 3], mid_point[i], mid_point[j % 3]));
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

	//find tangent
	vector<point2d> chL = ch.convex_hull(vl.point_list);
	vector<point2d> chR = ch.convex_hull(vr.point_list);
	vector<edge> tangent = GetTangent(chL, chR);

	//find hyper plane
	vector<edge> hpList;
	point2d p;
	point2d nearPoint;
	point2d lastNearPoint;
	edge scan; //0->scan 1->candidate 2->last 3->hyperPlane
	edge candidate;
	edge last;
	edge hyperPlane;
	vector<int> eliminate;
	vector<int> Delete;

	for (int i = 0; i < vl.point_list.size(); i++) v.point_list.push_back(vl.point_list[i]);
	for (int i = 0; i < vr.point_list.size(); i++) v.point_list.push_back(vr.point_list[i]);
	for (int i = 0; i < vl.voronoi_list.size(); i++) v.voronoi_list.push_back(vl.voronoi_list[i]);
	for (int i = 0; i < vr.voronoi_list.size(); i++) v.voronoi_list.push_back(vr.voronoi_list[i]);

	scan = edge(tangent[0].a, tangent[0].b);
	lastNearPoint = m.getBisector(scan).a;
	while (scan.a.x != tangent[0].a.x && scan.a.y != tangent[0].a.y && scan.b.x != tangent[0].b.x && scan.mid_a.x != tangent[0].mid_a.x
		&& scan.mid_a.y != tangent[0].mid_a.y && scan.mid_b.x != tangent[0].mid_b.x && scan.mid_b.y != tangent[0].mid_b.y)
	{
		hyperPlane = m.getBisector(scan);

		nearPoint = NULL;
		for (int i = 0; i < v.voronoi_list.size(); i++)
		{
			if (last.a.x == v.voronoi_list[i].a.x && last.a.y == v.voronoi_list[i].a.y && last.b.x == v.voronoi_list[i].b.x
				&& last.b.y == v.voronoi_list[i].b.y && last.mid_a.x == v.voronoi_list[i].mid_a.x && last.mid_a.y == v.voronoi_list[i].mid_a.y
				&& last.mid_b.x == v.voronoi_list[i].mid_b.x && last.mid_b.y == v.voronoi_list[i].mid_b.y)
				continue;

			p = m.getIntersection(hyperPlane, v.voronoi_list[i]);
			if (lastNearPoint.y >= p.y)
			{
				//find first intersection
				if (nearPoint.x == NULL && nearPoint.y == NULL)
				{
					nearPoint = p;
					candidate = v.voronoi_list[i];
					continue;
				}
				if (ch.distance(hyperPlane.a, p) < ch.distance(hyperPlane.a, nearPoint))
				{
					nearPoint = p;
					candidate = v.voronoi_list[i];
				}
			}
		}

		//draw from the last hyper plane
		if (lastNearPoint.x != NULL && lastNearPoint.y != NULL) hyperPlane.a = lastNearPoint;
		hpList.push_back(edge(scan.a, scan.b, hyperPlane.a, nearPoint));

		eliminate.push_back(v.voronoi_list.indexOf(candidate));
	}

	return v;
}

point2d VoronoiDiagram::swap(double a, double b)
{
	return point2d(b, a);
}

vector<edge> VoronoiDiagram::GetTangent(vector<point2d> chL, vector<point2d> chR)
{
	vector<edge> tangent;
	vector<point2d> point_list;
	vector<point2d> chList;

	point_list = chL;
	for (int i = 0; i < chR.size(); i++) point_list.push_back(chR[i]);

	chList = ch.convex_hull(point_list);

	for (int i = 0; i < chList.size(); i++)
	{
		if (((chList[i].x == chL[i].x && chList[i].y == chL[i].y) && (chList[i + 1 % chList.size()].x == chR[i + 1 % chList.size()].x && chList[i + 1 % chList.size()].y == chR[i + 1 % chList.size()].y))
			|| (chList[i].x == chR[i].x && chList[i].y == chR[i].y) && (chList[i + 1 % chList.size()].x == chL[i + 1 % chList.size()].x && chList[i + 1 % chList.size()].y == chL[i + 1 % chList.size()].y))
			tangent.push_back(edge(chList[i], chList[i + 1 % chList.size()]));
	}

	return tangent;
}