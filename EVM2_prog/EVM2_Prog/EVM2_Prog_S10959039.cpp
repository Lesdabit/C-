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
			vd.point_list.push_back(point2d(x, y));
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
	for (int i = 0; i < point_n - 1; i++)
	{
		for (int j = 0; j < point_n - i - 1; j++)
		{
			if (vd.point_list[j].x > vd.point_list[j + 1].x)
			{
				double temp_x = vd.point_list[j].x;
				vd.point_list[j].x = vd.point_list[j + 1].x;
				vd.point_list[j + 1].x = temp_x;

				double temp_y = vd.point_list[j].y;
				vd.point_list[j].y = vd.point_list[j + 1].y;
				vd.point_list[j + 1].y = temp_y;
			}
		}
	}

	for (int i = 0; i < point_n - 1; i++)
	{
		for (int j = 0; j < point_n - i - 1; j++)
		{
			if (vd.point_list[j].x == vd.point_list[j + 1].x && vd.point_list[j].y > vd.point_list[j + 1].y)
			{
				double temp_x = vd.point_list[j].x;
				vd.point_list[j].x = vd.point_list[j + 1].x;
				vd.point_list[j + 1].x = temp_x;

				double temp_y = vd.point_list[j].y;
				vd.point_list[j].y = vd.point_list[j + 1].y;
				vd.point_list[j + 1].y = temp_y;
			}
		}
	}



	for (int i = 0; i < point_n; i++)
	{
		cout << vd.point_list[i].x << " " << vd.point_list[i].y << endl;
	}

	vector<double> coor_x;
	vector<double> coor_y;

	for (int i = 0; i < point_n; i++)
	{
		//cout << points[i].x << " " << points[i].y << endl;
		coor_x.push_back(vd.point_list[i].x);
		coor_y.push_back(vd.point_list[i].y);
	}

	// convex hull
	vector<double> scale_coor_x;
	vector<double> scale_coor_y;
	vector<point2d> scale = ch.convex_hull(vd.point_list);

	for (int i = 0; i < scale.size(); i++)
	{
		scale_coor_x.push_back(scale[i].x);
		scale_coor_y.push_back(scale[i].y);
		//cout << ch.scale[i].x << " " << ch.scale[i].y << endl;
	}

	vd = vd.voronoi(vd.point_list);
	vector<double> edge_mid_a;
	vector<double> edge_mid_b;

	for (int i = 0; i < vd.voronoi_list.size(); i++)
	{
		if (i % 2 == 0)
		{
			edge_mid_a.push_back(vd.voronoi_list[i].mid_a.x);
			edge_mid_a.push_back(vd.voronoi_list[i].mid_a.y);
		}
		else
		{
			edge_mid_b.push_back(vd.voronoi_list[i].mid_b.x);
			edge_mid_b.push_back(vd.voronoi_list[i].mid_b.y);
		}
	}

	vector<int> tick = { 0,5,10,15,20,25,30 };
	plt::scatter(coor_x, coor_y, 25, { {"label", "charge stop"}, {"color", "r"} });
	//plt::plot(scale_coor_x, scale_coor_y);
	plt::plot(edge_mid_a, edge_mid_b);
	plt::title("Charge stop map");
	plt::xticks(tick);
	plt::yticks(tick);
	//plt::grid(true);
	plt::legend();
	plt::show();
}