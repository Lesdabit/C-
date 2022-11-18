#include <iostream>
#include <vector>
#include <fstream>

#include "Point2D.h"
#include "matplotlibcpp.h"

using namespace std;
namespace plt = matplotlibcpp;

int main()
{
	vector<point2d> points;
	ifstream in("map.txt");

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

	vector<double> coor_x;
	vector<double> coor_y;

	for (int i = 0; i < points.size(); i++)
	{
		//cout << points[i].x << " " << points[i].y << endl;
		coor_x.push_back(points[i].x);
		coor_y.push_back(points[i].y);
	}

	vector<int> tick = { 0,1,2,3,4,5,6,7,8 };
	plt::scatter(coor_x, coor_y, 25, { {"label", "charge stop"}, {"color", "r"} });
	plt::title("Charge stop map");
	plt::xticks(tick);
	plt::yticks(tick);
	//plt::grid(true);
	plt::legend();
	plt::show();
}