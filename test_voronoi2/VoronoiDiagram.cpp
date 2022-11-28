#include <vector>
#include <iostream>

#include "Point2D.h"
#include "Edge.h"
#include "VoronoiDiagram.h"
#include "Convex_hull.h"
#include "math.h"
//#include "matplotlibcpp.h"

using namespace std;
//namespace plt = matplotlibcpp;
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
		vector<vector<double>> x1;
		vector<vector<double>> y1;
		vector<vector<double>> x2;
		vector<vector<double>> y2;

		vector<double> coor_x;
		vector<double> coor_y;

		v = merge(vl, vr, v);
	}
	return v;
}

VoronoiDiagram VoronoiDiagram::two_voronoi(vector<point2d> points)
{
	vector<point2d> mid_point;
	VoronoiDiagram v;
	v.point_list.push_back(points[0]);
	v.point_list.push_back(points[1]);
	point2d midpoint = m.getMidpoint(points[0], points[1]);
	point2d normal = m.Multi(m.getNormal(points[0], points[1]), 10);
	mid_point.push_back(point2d(midpoint.x + normal.x, midpoint.y + normal.y)); //expand the line
	mid_point.push_back(point2d(midpoint.x - normal.x, midpoint.y - normal.y));
	v.voronoi_list.push_back(edge(mid_point[0], mid_point[1], points[0], points[1]));
	return v;
}

VoronoiDiagram VoronoiDiagram::three_voronoi(vector<point2d> points)
{
	vector<point2d> mid_point;
	VoronoiDiagram v;
	v.point_list.push_back(points[0]);
	v.point_list.push_back(points[1]);
	v.point_list.push_back(points[2]);
	if (isCollinear(v.point_list[0], v.point_list[1], v.point_list[2]) == true)
	{
		points = m.sort(points);

		for (int i = 0; i < 2; i++)
		{
			point2d midpoint = m.getMidpoint(v.point_list[i], v.point_list[i + 1]);
			point2d normal = m.Multi(m.getNormal(v.point_list[i], v.point_list[i + 1]), 10);
			mid_point.push_back(point2d(midpoint.x + normal.x, midpoint.y + normal.y));
			mid_point.push_back(point2d(midpoint.x - normal.x, midpoint.y - normal.y));
			v.voronoi_list.push_back(edge(mid_point[i], mid_point[i + 1], v.point_list[i], v.point_list[i + 1]));
		}
	}
	else
	{
		point2d triEx = m.getTriangleExcenterPoint(points[0], points[1], points[2]);
		points = m.bubblesort(points);

		for (int i = 0; i < points.size(); i++)
		{
			point2d midpoint = m.getMidpoint(points[i], points[(i + 1) % 3]);
			point2d normal = m.Multi(m.getNormal(points[i], points[(i + 1) % 3]), 10);
			v.voronoi_list.push_back(edge(triEx, point2d(midpoint.x + normal.x, midpoint.y + normal.y), points[i], points[(i + 1) % 3]));
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

VoronoiDiagram VoronoiDiagram::merge(VoronoiDiagram vl, VoronoiDiagram vr, VoronoiDiagram v)
{
	//find tangent
	//vector<point2d> points = vl.point_list;
	//for (int i = 0; i < vr.point_list.size(); i++) points.push_back(vr.point_list[i]);
	//vector<point2d> con = ch.convex_hull(points);
	vector<point2d> chL = ch.convex_hull(vl.point_list);
	vector<point2d> chR = ch.convex_hull(vr.point_list);
	vector<edge> tangent = GetTangent(chL, chR);

	//find hyper plane
	vector<edge> hpList;
	edge scan;
	vector<point2d> voronoi;
	vector<int> voronoi_index;
	point2d lastmax;

	for (point2d i : vl.point_list) v.point_list.push_back(i);
	for (point2d i : vr.point_list) v.point_list.push_back(i);
	for (edge i : vl.voronoi_list) v.voronoi_list.push_back(i);
	for (edge i : vr.voronoi_list) v.voronoi_list.push_back(i);

	scan = tangent[0];
	int count = 0;

	while (scan.a.x != tangent[1].a.x || scan.a.y != tangent[1].a.y ||
			scan.b.x != tangent[1].b.x || scan.b.y != tangent[1].b.y)
	{	
		vector<point2d> candidate_p;
		vector<int> candidate_index;
		point2d p;
		point2d max;
		edge max_candidate;
		int max_index;

		for (int i = 0; i < v.voronoi_list.size(); i++)
		{
			if (count == 0)
			{
				p = m.getIntersection(scan, v.voronoi_list[i]);
			}
			else
			{
				p = m.getIntersection_normal(scan, v.voronoi_list[i]);
			}
			if (p.x != NULL && p.y != NULL)
			{
				candidate_p.push_back(p);
				candidate_index.push_back(i);
			}
		}

		if (lastmax.x == NULL && lastmax.y == NULL)
		{
			for (int i = 0; i < candidate_p.size(); i++)
			{
				if (candidate_p[i].y <= m.getMidpoint(scan.a, scan.b).y)
				{
					max_candidate = v.voronoi_list[i];
					max = candidate_p[i];
					max_index = i;
					break;
				}
			}
			for (int i = 1; i < candidate_p.size(); i++)
			{
				if (max.y < candidate_p[i].y && candidate_p[i].y < m.getMidpoint(scan.a, scan.b).y)
				{
					max_candidate = v.voronoi_list[i];
					max = candidate_p[i];
					max_index = i;
				}
				if (max.y == candidate_p[i].y && max.x < candidate_p[i].x)
				{
					max_candidate = v.voronoi_list[i];
					max = candidate_p[i];
					max_index = i;
				}
			}
		}
		else
		{
			for (int i = 0; i < candidate_p.size(); i++)
			{
				if (candidate_p[i].y <= lastmax.y)
				{
					max_candidate = v.voronoi_list[i];
					max = candidate_p[i];
					max_index = i;
					break;
				}
			}
			for (int i = 1; i < candidate_p.size(); i++)
			{
				if (max.y < candidate_p[i].y && candidate_p[i].y < lastmax.y)
				{
					max_candidate = v.voronoi_list[i];
					max = candidate_p[i];
					max_index = i;
				}
				if (max.y == candidate_p[i].y && max.x < candidate_p[i].x)
				{
					max_candidate = v.voronoi_list[i];
					max = candidate_p[i];
					max_index = i;
				}
			}
		}

		voronoi.push_back(max);
		lastmax = max;

		if (scan.a.x == max_candidate.mid_a.x && scan.a.y == max_candidate.mid_a.y)
			scan.a = max_candidate.mid_b;
		else if (scan.a.x == max_candidate.mid_b.x && scan.a.y == max_candidate.mid_b.y)
			scan.a = max_candidate.mid_a;
		else if (scan.b.x == max_candidate.mid_a.x && scan.b.y == max_candidate.mid_a.y)
			scan.b = max_candidate.mid_b;
		else if (scan.b.x == max_candidate.mid_b.x && scan.b.y == max_candidate.mid_b.y)
			scan.b= max_candidate.mid_a;
		count++;
	}

	for (int i = 0; i < voronoi.size(); i++)
	{
		v.voronoi_list.push_back(edge(voronoi[i], point2d(0, 0)));
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

	chList = m.sort(point_list);
	chList = ch.convex_hull(chList);

	//if (m.binary_search(0, chList.size(), chList[0], chL)) cout << "true" << endl;
	//else cout << "false" << endl;

	for (int i = 0; i < chList.size(); i++)
	{
		if ((m.search(chList[i], chL) && m.search(chList[(i + 1) % chList.size()], chR))
			|| (m.search(chList[i], chR) && m.search(chList[(i + 1) % chList.size()], chL)))
			tangent.push_back(edge(chList[i], chList[(i + 1) % chList.size()]));
	}
	return tangent;
}