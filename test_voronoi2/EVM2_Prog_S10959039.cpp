#include <iostream>
#include <vector>
#include <fstream>

#include "Point2D.h"
//#include "matplotlibcpp.h"
#include "Convex_hull.h"
#include "VoronoiDiagram.h"
#include "math.h"
//#include "Convex_hull.cpp"

using namespace std;
//namespace plt = matplotlibcpp;
//math m;

int main()
{
	ifstream in("map2.txt");
	Convex_hull ch;
	VoronoiDiagram vd;
	vector<point2d> points;

	double x, y;
	int point_n;

	if (in)
	{
		in >> point_n;
		for (int i = 0; i < point_n; i++)
		{
			in >> x >> y;
			points.push_back(point2d(x, y));
		}
	}

	


	/*
	for (int i = 0; i < points.size(); i++)
	{
		cout << points[i].x << " " << points[i].y << endl;
	}
	*/

	//cout << "\n\nAfter sort\n\n";

	// bubble sort to sort points
	//vd.point_list = m.bubblesort(vd.point_list);

	/*
	for (int i = 0; i < point_n; i++)
	{
		cout << vd.point_list[i].x << " " << vd.point_list[i].y << endl;
	}
	*/

	// convex hull
	/*vector<double> scale_coor_x;
	vector<double> scale_coor_y;
	vector<point2d> scale = ch.convex_hull(vd.point_list);

	for (int i = 0; i < scale.size(); i++)
	{
		scale_coor_x.push_back(scale[i].x);
		scale_coor_y.push_back(scale[i].y);
		cout << scale[i].x << " " << scale[i].y << endl;
	}*/

	for (int i = 0; i < points.size() - 1; i++)
	{
		for (int j = 0; j < points.size() - i - 1; j++)
		{
			if (points[j].x > points[j + 1].x)
			{
				point2d temp = points[j + 1];
				points[j + 1] = points[j];
				points[j] = temp;
			}
		}
	}

	vd = vd.voronoi(points);

	
	for (int i = 0; i < vd.voronoi_list.size(); i++)
	{
		cout << vd.voronoi_list[i].a.x << " " << vd.voronoi_list[i].a.y << " " << vd.voronoi_list[i].b.x << " " << vd.voronoi_list[i].b.y << " " 
			<< vd.voronoi_list[i].mid_a.x << " " << vd.voronoi_list[i].mid_a.y << " " << vd.voronoi_list[i].mid_b.x << " " << vd.voronoi_list[i].mid_b.y << endl;
	}

	vector<double> coor_x;
	vector<double> coor_y;

	for (int i = 0; i < point_n; i++)
	{
		//cout << points[i].x << " " << points[i].y << endl;
		coor_x.push_back(vd.point_list[i].x);
		coor_y.push_back(vd.point_list[i].y);
	}

	
}