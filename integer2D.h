#pragma once

#include "stdafx.h"
#include <iostream>
#include <vector>

using namespace std;

struct point {
	uint64_t x;
	uint64_t y;
	int cluster_relac;
};

class cluster {
public:
	point center;
	vector<point> members;

	cluster(point to_center);

	void update_centroid();
};

int dist_point(point one, point two);

vector<point> read_file(string filename);