#include "integer2D.h"
#include "stdafx.h"

using namespace std;

cluster::cluster(point to_center) {
	center = to_center;
}

void cluster::update_centroid() {
	if (members.size()) {
		double new_x = 0, new_y = 0, new_z = 0;

		for (point& element : members) {
			new_x += element.x;
			new_y += element.y;
		}

		const int size = members.size();

		center.x = new_x / size;
		center.y = new_y / size;

	}
	members.erase(members.begin(), members.end());
}

int dist_point(point one, point two) {
	return ((one.x - two.x) * (one.x - two.x)) + ((one.y - two.y) * (one.y - two.y));
}

vector<point> read_file(string filename) {
	fstream thefile(filename, fstream::in);
	string line;
	vector<point> output;
	if (thefile.is_open()) {
		while (getline(thefile, line)) {

			point temp;
			temp.x = stoi(line.substr(0, 11));
			temp.y = stoi(line.substr(11));
			temp.cluster_relac = 0;
			output.push_back(temp);

		}
	}

	thefile.close();

	return output;
}
