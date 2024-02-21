#pragma once
#include <iostream>
#include "resources.h"

using namespace std;

int test_Worker(void) {
	int min_val = 1, max_val = 10;
	IntervalGaussian inter2(5, 2, min_val, max_val);
	string w_id = "fd", w_name = "John", w_con_id = "kek";
	int w_count = 3;
	IntervalGaussian inter1(5, 2, min_val, max_val);
	Worker w1(w_id, w_name, w_count, max_val, w_con_id, inter2);

	if (w1.get_id() != w_id) {
		cout << "string id error\t";
		return 0;
	}
	if (w1.get_count() != w_count) {
		cout << "get count error\t";
		return 0;
	}
	if (w1.get_name() != w_name) {
		cout << "get name error\t";
		return 0;
	}
	if (w1.get_contractor_id() != w_con_id) {
		cout << "get contractor id error\t";
		return 0;
	}
	float prod1_fl = w1.get_productivity().randFloat();
	int prod1_int = w1.get_productivity().randInt();

	if ((prod1_fl < min_val) && (prod1_fl > max_val)) {
		cout << "interval float error\t";
		return 0;
	}
	if ((prod1_int < min_val) && (prod1_int > max_val)) {
		cout << "interval int error\t";
		return 0;
	}

	return 1;
}