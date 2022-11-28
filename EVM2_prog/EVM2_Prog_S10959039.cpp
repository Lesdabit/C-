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
	
	while (true)
	{
		int q;
		cout << "1)依據充電站的位置(建構一個Voronoi Diagram with time complexity O(𝑛log𝑛)." << endl;
		cout << "2)依序列出充電站的服務範圍(面積)。" << endl;
		cout << "3)找出離充電站最遠的點以及距離(Largest Empty Circle Problem with Voronoi Diagram, also called toxic waste dump problem)。" << endl;
		cout << "4)根據Sub-problem(在該位置安裝一個充電站,則須在原來的Voronoi Diagram外加一個點變成有n+1個點的Voronoi Diagram設計一程式法建構該新的Voronoi Diagram。" << endl;
		cout << "  (重複3、4直到所有最遠距離小於150m(1個tick 1m)" << endl;
		cin >> q;

		if (q == 1)
		{
			vector<double> coor_x;
			vector<double> coor_y;

			for (int i = 0; i < point_n; i++)
			{
				//cout << points[i].x << " " << points[i].y << endl;
				coor_x.push_back(vd.point_list[i].x);
				coor_y.push_back(vd.point_list[i].y);
			}

			//vd = vd.voronoi(vd.point_list);
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

			plt::scatter(coor_x, coor_y, 25, { {"label", "charge stop"} });
			//plt::plot(scale_coor_x, scale_coor_y);
			//plt::plot(vd.voronoi_list, vd.voronoi_list);
			plt::title("Charge stop map");
			plt::grid(true);
			plt::legend();
			plt::show();
		}

	}
}
