#pragma once
#include <iostream>
#include "contractor.h"

using namespace std;

int test_Contractor(void) {
	int min_val = 1, max_val = 10;
	IntervalGaussian inter2(5, 2, min_val, max_val);
	string w_id[] = { "fd_1", "fd_2" }, w_name[] = { "John", "Max" }, w_con_id[] = { "kek1", "kek2" };
	int w_count_1 = 3, w_count_2 = 3;
	IntervalGaussian inter1(5, 2, min_val, max_val);
	Worker w1(w_id[0], w_name[0], w_count_1, w_con_id[0], inter1), w2(w_id[1], w_name[1], w_count_1, w_con_id[1], inter2);
	vector<Worker*> w = { &w1, &w2 };
	Contractor c1(w);
	c1.id = "idi_kek";

	if (c1.get_id() != c1.id) {
		cout << "string contractor id error\t";
		return 0;
	}
	//cout << "c1 worker: " << c1.workers[0] << endl;

	return 1;
}