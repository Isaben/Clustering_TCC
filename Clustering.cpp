// Clustering.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <cstdlib>
#include <mutex>
#include <future>
#include <cmath>
#include <windows.h>

using namespace std;

static mutex barrier;

vector<cluster3D> clusters3D;
vector<point3D> db;

bool execute_thread(int begin, int end, const int NUM_OF_CLUSTERS, int thread) {
	bool has_changes = false;
	
	for (; begin != end; ++begin) {
		int min_dist = dist_point_3D(db[begin], clusters3D[db[begin].cluster_relac].center), cluster_ref = db[begin].cluster_relac;
		for (int i = 0; i < NUM_OF_CLUSTERS; i++) {
			if (dist_point_3D(db[begin], clusters3D[i].center) <= min_dist) {
				cluster_ref = i;
			}
		}
		if (cluster_ref != db[begin].cluster_relac) {
			has_changes = true;
		}
		barrier.lock();
		clusters3D[cluster_ref].members.push_back(db[begin]);
		barrier.unlock();
		db[begin].cluster_relac = cluster_ref;
		
	}
	
	return has_changes;
}

vector<float> do_multithread_k(string filename, const int NUM_OF_CLUSTERS, const int DB_SIZE, int num_tests) {
	vector<float> times;
	for (int i = 0; i < num_tests; i++) {
		db = read_file_2(filename);

		const int NUM_THREADS = 16;

		bool has_changes = true;
		LARGE_INTEGER t1;
		QueryPerformanceCounter(&t1);
		for (int i = 0; i < NUM_OF_CLUSTERS; i++) {
			clusters3D.push_back(cluster3D(db[rand() % DB_SIZE]));
		}
		int iteractions = 0;

		while (has_changes) {
			int index = 0;

			vector<future<bool>> threads;

			const int size = floor(DB_SIZE / (float)NUM_THREADS);

			for (int i = 0; i < (NUM_THREADS - 1); i++) {
				threads.push_back(async(&execute_thread, index, index + size, NUM_OF_CLUSTERS, i));
				index += size;
			}
			has_changes = execute_thread(index, db.size(), NUM_OF_CLUSTERS, 16);

			for (int i = 0; i < (NUM_THREADS - 1); i++) {
				has_changes |= threads[i].get();
			}

			for (cluster3D& element : clusters3D) {
				element.update_centroid();
			}

			cout << "Iteraction " << iteractions++ << endl;
		}

		LARGE_INTEGER t2;
		QueryPerformanceCounter(&t2);
		LARGE_INTEGER freq;
		QueryPerformanceFrequency(&freq);
		times.push_back((t2.QuadPart - t1.QuadPart) / (float)(freq.QuadPart));

		cout << "MULTITHREAD SIZE" << DB_SIZE << " TIME: " << (t2.QuadPart - t1.QuadPart) / (float)(freq.QuadPart) << endl;
	}

	return times;

}


vector<float> do_single_thread_k(string filename, const int NUM_OF_CLUSTERS, const int DB_SIZE, int num_tests) {
	vector<float> times;
	for (int i = 0; i < num_tests; i++) {
		vector<point3D> db = read_file_2(filename);

		vector<cluster3D> clusters;
		bool has_changes = true;

		LARGE_INTEGER t1;
		QueryPerformanceCounter(&t1);
		for (int i = 0; i < NUM_OF_CLUSTERS; i++) {
			clusters.push_back(cluster3D(db[rand() % DB_SIZE]));
		}

		int iteractions = 0;
		while (has_changes) {
			has_changes = false;

			for (point3D& element : db) {
				int min_dist = dist_point_3D(element, clusters[element.cluster_relac].center), cluster_ref = element.cluster_relac;
				for (int i = 0; i < NUM_OF_CLUSTERS; i++) {
					if (dist_point_3D(element, clusters[i].center) <= min_dist) {
						cluster_ref = i;
					}
				}
				if (cluster_ref != element.cluster_relac) {
					has_changes = true;
				}
				clusters[cluster_ref].members.push_back(element);
				element.cluster_relac = cluster_ref;
			}

			for (cluster3D& element : clusters) {
				element.update_centroid();
			}

			cout << "Iteraction " << iteractions++ << endl;
		}

		LARGE_INTEGER t2;
		QueryPerformanceCounter(&t2);
		LARGE_INTEGER freq;
		QueryPerformanceFrequency(&freq);
		times.push_back((t2.QuadPart - t1.QuadPart) / (float)(freq.QuadPart));

		cout << "SINGLETHREAD SIZE" << DB_SIZE << " TIME: " << (t2.QuadPart - t1.QuadPart) / (float)(freq.QuadPart) << endl;
	}

	return times;
}

int main()
{
	// qtd CLUSTERS 3dDB: 57329 half: 29158 1/4: 14912
	const int NUM_OF_CLUSTERS = 57329, DB_SIZE = 434874;
	const int HALF_CLUSTERS = 29158, HALF_SIZE = 217437;
	const int ONEFOUR_CLUSTERS = 14912, ONEFOUR_SIZE = 108719;
	const int NUM_OF_TESTS = 5;

	vector<float>	multi_full = do_multithread_k("3dDB", NUM_OF_CLUSTERS, DB_SIZE, NUM_OF_TESTS),
		single_full = do_single_thread_k("3dDB", NUM_OF_CLUSTERS, DB_SIZE, NUM_OF_TESTS),
		multi_half = do_multithread_k("3dDB", HALF_CLUSTERS, HALF_SIZE, NUM_OF_TESTS),
		single_half = do_single_thread_k("3dDB", HALF_CLUSTERS, HALF_SIZE, NUM_OF_TESTS),
		multi_onefour = do_multithread_k("3dDB", ONEFOUR_CLUSTERS, ONEFOUR_SIZE, NUM_OF_TESTS),
		single_onefour = do_single_thread_k("3dDB", ONEFOUR_CLUSTERS, ONEFOUR_SIZE, NUM_OF_TESTS);

	ofstream out_file("results.txt");

	for (auto& time : multi_full) {
		cout << "MULTI FULL TIME: " << time << endl;
		out_file << "MULTI FULL TIME: " << time << endl;

	}
	for (auto& time : multi_half) {
		cout << "MULTI HALF TIME: " << time << endl;
		out_file << "MULTI HALF TIME: " << time << endl;
	}
	for (auto& time : multi_onefour) {
		cout << "MULTI ONEFOUR TIME: " << time << endl;
		out_file << "MULTI ONEFOUR TIME: " << time << endl;
	}
	for (auto& time : single_full) {
		cout << "SINGLE FULL TIME: " << time << endl;
		out_file << "SINGLE FULL TIME: " << time << endl;
	}
	for (auto& time : single_half) {
		cout << "SINGLE HALF TIME: " << time << endl;
		out_file << "SINGLE HALF TIME: " << time << endl;
	}
	for (auto& time : single_onefour) {
		cout << "SINGLE ONEFOUR TIME: " << time << endl;
		out_file << "SINGLE ONEFOUR TIME: " << time << endl;
	}
	out_file.close();
	system("pause");
    return 0;
}

