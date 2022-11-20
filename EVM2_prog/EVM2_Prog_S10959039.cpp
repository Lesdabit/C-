#include <iostream>
#include <vector>
#include <fstream>

#include "Point2D.h"
#include "matplotlibcpp.h"
#include "Convex_hull.h"
#include "VoronoiDiagram.h"
//#include "Convex_hull.cpp"

using namespace std;
namespace plt = matplotlibcpp;


int main()
{
	ifstream in("map.txt");
	point p;
	Convex_hull ch;
	VoronoiDiagram vd;

	double x, y;
	int point_n;

	if (in)
	{
		in >> point_n;
		for (int i = 0; i < point_n; i++)
		{
			in >> x >> y;
			p.points.push_back(point2d(x, y));
		}
	}

	/*
	for (int i = 0; i < points.size(); i++)
	{
		cout << points[i].x << " " << points[i].y << endl;
	}
	*/

	cout << "\n\nAfter sort\n\n";

	// bubble sort to sort points
	for (int i = 0; i < p.points.size() - 1; i++)
	{
		for (int j = 0; j < p.points.size() - i - 1; j++)
		{
			if (p.points[j].x > p.points[j + 1].x)
			{
				double temp_x = p.points[j].x;
				p.points[j].x = p.points[j + 1].x;
				p.points[j + 1].x = temp_x;

				double temp_y = p.points[j].y;
				p.points[j].y = p.points[j + 1].y;
				p.points[j + 1].y = temp_y;
			}
		}
	}

	for (int i = 0; i < p.points.size() - 1; i++)
	{
		for (int j = 0; j < p.points.size() - i - 1; j++)
		{
			if (p.points[j].x == p.points[j + 1].x && p.points[j].y > p.points[j + 1].y)
			{
				double temp_x = p.points[j].x;
				p.points[j].x = p.points[j + 1].x;
				p.points[j + 1].x = temp_x;

				double temp_y = p.points[j].y;
				p.points[j].y = p.points[j + 1].y;
				p.points[j + 1].y = temp_y;
			}
		}
	}

	
	for (int i = 0; i < p.points.size(); i++)
	{
		cout << p.points[i].x << " " << p.points[i].y << endl;
	}
	
	vector<double> coor_x;
	vector<double> coor_y;

	for (int i = 0; i < p.points.size(); i++)
	{
		//cout << points[i].x << " " << points[i].y << endl;
		coor_x.push_back(p.points[i].x);
		coor_y.push_back(p.points[i].y);
	}

	// convex hull
	vector<double> scale_coor_x;
	vector<double> scale_coor_y;
	ch.convex_hull(p.points);

	for (int i = 0; i < ch.scale.size(); i++)
	{
		scale_coor_x.push_back(ch.scale[i].x);
		scale_coor_y.push_back(ch.scale[i].y);
		//cout << ch.scale[i].x << " " << ch.scale[i].y << endl;
	}

	//vector<edge> edge_list = vd.voronoi(p.points);
	vector<point2d> edge_mid_a;
	vector<point2d> edge_mid_b;

	vector<int> tick = { 0,5,10,15,20,25,30 };
	plt::scatter(coor_x, coor_y, 25, { {"label", "charge stop"}, {"color", "r"} });
	plt::plot(scale_coor_x, scale_coor_y);

	

	plt::title("Charge stop map");
	plt::xticks(tick);
	plt::yticks(tick);
	//plt::grid(true);
	plt::legend();
	plt::show();
}
