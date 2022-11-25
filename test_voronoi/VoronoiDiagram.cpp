#include <vector>
#include <iostream>

#include "Point2D.h"
#include "Edge.h"
#include "VoronoiDiagram.h"
#include "Convex_hull.h"
#include "math.h"

using namespace std;
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
	//vector<point2d> points = vl.point_list;
	//for (int i = 0; i < vr.point_list.size(); i++) points.push_back(vr.point_list[i]);
	//vector<point2d> con = ch.convex_hull(points);
	vector<point2d> chL = ch.convex_hull(vl.point_list);
	vector<point2d> chR = ch.convex_hull(vr.point_list);
	vector<edge> tangent = GetTangent(chL, chR);

	//find hyper plane
	vector<edge> hpList;		//Hyper plane
	point2d p;					//Store the intersection of Hyper plane and voronoi
	point2d nearPoint;			//first intersection
	point2d lastNearPoint;		//last intersection
	edge scan;					//scan the line
	edge candidate;				//line
	edge last;					//last line
	edge hyperPlane;			//Hyper plane now
	vector<int> eliminate;		//the index needs to be eliminated
	vector<int> Delete;			//the index needs to be deleted

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
					eliminate.push_back(i);
					continue;
				}
				if (ch.distance(hyperPlane.a, p) < ch.distance(hyperPlane.a, nearPoint))
				{
					nearPoint = p;
					candidate = v.voronoi_list[i];
					eliminate.push_back(i);
				}
			}
		}

		//draw from the last hyper plane
		if (lastNearPoint.x != NULL && lastNearPoint.y != NULL) hyperPlane.a = lastNearPoint;
		hpList.push_back(edge(hyperPlane.a, nearPoint, scan.a, scan.b));

		last = candidate;
		lastNearPoint = nearPoint;

		//fine next scan line
		if (scan.a.x == candidate.a.x && scan.a.y == candidate.a.y) scan.a = candidate.b;
		else if (scan.a.x == candidate.b.x && scan.a.y == candidate.b.y) scan.a = candidate.a;
		else if (scan.b.x == candidate.a.x && scan.b.y == candidate.a.y) scan.b = candidate.b;
		else if (scan.b.x == candidate.b.x && scan.b.y == candidate.b.y) scan.b = candidate.a;
	}

	//上切線等於下切線:共線
	if (tangent[0].a.x == tangent[1].a.x && tangent[0].a.y == tangent[1].a.y && tangent[0].b.x == tangent[1].b.x &&
		tangent[0].b.y == tangent[1].b.y && tangent[0].mid_a.x == tangent[1].mid_a.x && tangent[0].mid_a.y == tangent[1].mid_a.y &&
		tangent[0].mid_b.x == tangent[1].mid_b.x && tangent[0].mid_b.y == tangent[1].mid_b.y)
		hpList.push_back(edge(m.getBisector(tangent[0]).a, m.getBisector(tangent[0]).b, scan.a, scan.b));
	else
		hpList.push_back(edge(nearPoint, m.getBisector(tangent[1]).a, scan.a, scan.b));

	int count = 0;
	//消線
	for (int i = 0; i < eliminate.size(); i++)
	{
		count = 0;
		if (m.Cross(hpList[i].a, hpList[i].b, hpList[i + 1].b) >= 0)
		{
			if (m.Cross(hpList[i].a, hpList[i].b, v.voronoi_list[eliminate[i]].a) > 0)
			{
				for (edge j : v.voronoi_list)
				{
					if (j.a.x == v.voronoi_list[eliminate[i]].a.x && j.a.y == v.voronoi_list[eliminate[i]].a.y &&
						j.b.x != v.voronoi_list[eliminate[i]].b.x && j.b.y != v.voronoi_list[eliminate[i]].b.y)
					{
						if (m.Cross(hpList[i].b, j.a, j.b) > 0) Delete.push_back(count);
					}
					else if (j.b.x != v.voronoi_list[eliminate[i]].a.x && j.b.y != v.voronoi_list[eliminate[i]].a.y &&
							j.a.x == v.voronoi_list[eliminate[i]].b.x && j.a.y == v.voronoi_list[eliminate[i]].b.y)
					{
						if (m.Cross(hpList[i].b, j.b, j.a) > 0) Delete.push_back(count);
					}
					count++;
				}
				v.voronoi_list[eliminate[i]].a = hpList[i].b;
			}
			else
			{
				for (edge j : v.voronoi_list)
				{
					if (j.a.x == v.voronoi_list[eliminate[i]].b.x && j.a.y == v.voronoi_list[eliminate[i]].b.y &&
						j.b.x != v.voronoi_list[eliminate[i]].a.x && j.b.y != v.voronoi_list[eliminate[i]].a.y)
					{
						if (m.Cross(hpList[i].b, j.a, j.b) > 0) Delete.push_back(count);
					}
					else if (j.b.x != v.voronoi_list[eliminate[i]].b.x && j.b.y != v.voronoi_list[eliminate[i]].b.y &&
							j.a.x == v.voronoi_list[eliminate[i]].a.x && j.a.y == v.voronoi_list[eliminate[i]].a.y)
					{
						if (m.Cross(hpList[i].b, j.b, j.a) > 0) Delete.push_back(count);
					}
					count++;
				}
				v.voronoi_list[eliminate[i]].b = hpList[i].b;
			}
		}
		else if (m.Cross(hpList[i].a, hpList[i].b, hpList[i + 1].b) < 0)
		{
			if (m.Cross(hpList[i].a, hpList[i].b, v.voronoi_list[eliminate[i]].a) < 0)
			{
				for (edge j : v.voronoi_list)
				{
					if (j.a.x == v.voronoi_list[eliminate[i]].a.x && j.a.y == v.voronoi_list[eliminate[i]].a.y &&
						j.b.x != v.voronoi_list[eliminate[i]].b.x && j.b.y != v.voronoi_list[eliminate[i]].b.y)
					{
						if (m.Cross(hpList[i].b, j.a, j.b) > 0) Delete.push_back(count);
					}
					else if (j.b.x != v.voronoi_list[eliminate[i]].a.x && j.b.y != v.voronoi_list[eliminate[i]].a.y &&
						j.a.x == v.voronoi_list[eliminate[i]].b.x && j.a.y == v.voronoi_list[eliminate[i]].b.y)
					{
						if (m.Cross(hpList[i].b, j.b, j.a) > 0) Delete.push_back(count);
					}
					count++;
				}
				v.voronoi_list[eliminate[i]].a = hpList[i].b;
			}
			else
			{
				for (edge j : v.voronoi_list)
				{
					if (j.a.x == v.voronoi_list[eliminate[i]].b.x && j.a.y == v.voronoi_list[eliminate[i]].b.y &&
						j.b.x != v.voronoi_list[eliminate[i]].a.x && j.b.y != v.voronoi_list[eliminate[i]].a.y)
					{
						if (m.Cross(hpList[i].b, j.a, j.b) > 0) Delete.push_back(count);
					}
					else if (j.b.x != v.voronoi_list[eliminate[i]].b.x && j.b.y != v.voronoi_list[eliminate[i]].b.y &&
						j.a.x == v.voronoi_list[eliminate[i]].a.x && j.a.y == v.voronoi_list[eliminate[i]].a.y)
					{
						if (m.Cross(hpList[i].b, j.b, j.a) > 0) Delete.push_back(count);
					}
					count++;
				}
				v.voronoi_list[eliminate[i]].b = hpList[i].b;
			}
		}
	}

	vector<edge> delete_edge;
	count = 0;
	for (int i : Delete)
	{
		if (count != 0) delete_edge.push_back(v.voronoi_list[count]);
	}
	v.voronoi_list.clear();
	v.voronoi_list = delete_edge;

	for (edge i : hpList)
	{
		v.voronoi_list.push_back(edge(i.a, i.b, i.mid_a, i.mid_b));
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
	chList = m.bubblesort(chList);

	//if (m.binary_search(0, chList.size(), chList[0], chL)) cout << "true" << endl;
	//else cout << "false" << endl;

	for (int i = 0; i < chList.size(); i++)
	{
		/*
		for (int j = 0; j < chL.size(); j++)
		{
			for (int k = 0; k < chR.size(); k++)
			{
				if (((chList[i].x == chL[j].x && chList[i].y == chL[j].y) && (chList[(i + 1) % chList.size()].x == chR[k].x
					&& chList[(i + 1) % chList.size()].y == chR[k].y))
					|| ((chList[i].x == chR[j].x && chList[i].y == chR[j].y) && (chList[(i + 1) % chList.size()].x == chL[k].x
					&& chList[(i + 1) % chList.size()].y == chL[k].y)))
					tangent.push_back(edge(chList[i], chList[i + 1 % chList.size()]));
			}
		}
		*/
		
		if ((m.binary_search(0, chL.size(), chList[i], chL) && m.binary_search(0, chR.size(), chList[(i + 1) % chList.size()], chR))
			|| (m.binary_search(0, chR.size(), chList[i], chR) && m.binary_search(0, chL.size(), chList[(i + 1) % chList.size()], chL)))
			tangent.push_back(edge(chList[i], chList[(i + 1) % chList.size()]));
	}

	return tangent;
}