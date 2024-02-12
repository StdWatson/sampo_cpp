#pragma once

#include <iostream>
#include "works.h"

using namespace std;

int test_WorkerReq(void) {
	string test_str[] = { "kek1", "kek2" };
	Time time_t1(5);

	WorkerReq t1(test_str[0], time_t1, 3, 5);

	if (t1.get_kind() != test_str[0]) {
		cout << "string test_str error\t";
		return 0;
	}
	if (!(t1.get_volume() == time_t1)) {
		cout << "get volume error\t";
		return 0;
	}
	if (t1.get_max_count() != 5) {
		cout << "max count error\t";
		return 0;
	}
	if (t1.get_min_count() != 3) {
		cout << "max count error\t";
		return 0;
	}

	return 1;
}

int test_WorkUnit(void) {
	string id_name = "name_1";
	string test_str[] = { "kek1", "kek2" };
	Time time_t1(5);
	int volume1 = 1, volume2 = 2, volume3 = 3;
	bool isServiceUnit1 = true, isServiceUnit2 = true, isServiceUnit3 = true;

	WorkerReq w1(test_str[0], time_t1, 3, 5), w2(test_str[1], time_t1, 3, 5);
	vector<WorkerReq> w = { w1, w2 };

	WorkUnit t1(w, volume1, isServiceUnit1);

	t1.id = id_name;

	if (t1.get_id() != id_name) {
		cout << "string test_str error\t";
		return 0;
	}
	if (t1.get_volume() != volume1) {
		cout << "get volume error\t";
		return 0;
	}
	if (t1.get_isServiceUnit() != isServiceUnit1) {
		cout << "get isServiceUnit1 error\t";
		return 0;
	}
	if (t1.get_worker_reqs()[0].get_kind() != "kek1") {
		cout << "max count error\t";
		return 0;
	}
	if (t1.get_worker_reqs()[1].get_max_count() != 5) {
		cout << "max count error\t";
		return 0;
	}

	return 1;
}