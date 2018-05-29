#pragma once
#include "stdafx.h"
#include <iostream>
#include <vector>

using namespace std;

struct point3D {
	double x;
	double y;
	double z;
	int cluster_relac;
};

class cluster3D {
public:
	point3D center;
	vector<point3D> members;

	cluster3D(point3D to_center);

	void update_centroid();
};

int dist_point_3D(point3D one, point3D two);

vector<point3D> read_file_2(string filename);

vector<string> split(string input, string delimiters);