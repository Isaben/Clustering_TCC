#include "stdafx.h"
#include "real3D.h"

using namespace std;

cluster3D::cluster3D(point3D to_center) {
	center = to_center;
}

void cluster3D::update_centroid() {
	if (members.size()) {
		double new_x = 0, new_y = 0, new_z = 0;

		for (point3D& element : members) {
			new_x += element.x;
			new_y += element.y;
			new_z += element.z;
		}

		const int size = members.size();

		center.x = new_x / size;
		center.y = new_y / size;
		center.z = new_z / size;

	}
	members.erase(members.begin(), members.end());
}

int dist_point_3D(point3D one, point3D two) {
	return ((one.x - two.x) * (one.x - two.x)) + ((one.y - two.y) * (one.y - two.y)) + ((one.z - two.z) * (one.z - two.z));
}


vector<string> split(string input, string delimiters) {
	size_t current;
	size_t next = -1;
	vector<string> output;
	do {
		current = next + 1;
		next = input.find_first_of(delimiters, current);
		output.push_back(input.substr(current, next - current));
	} while (next != string::npos);

	return output;
}

vector<point3D> read_file_2(string filename) {
	ifstream thefile(filename);
	string line;
	vector<point3D> output;
	if (thefile.is_open()) {
		while (getline(thefile, line)) {
			point3D temp;
			vector<string> splitted = split(line, ",");
			temp.x = stod(splitted[1]);
			temp.y = stod(splitted[2]);
			temp.z = stod(splitted[3]);
			temp.cluster_relac = 0;

			output.push_back(temp);
		}
	}

	thefile.close();

	return output;
}
