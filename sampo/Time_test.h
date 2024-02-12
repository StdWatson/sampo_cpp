#pragma once
#include "time.h"

using namespace std;

int test_time(void) {
	int test_num1 = 5, test_num2 = 1, tmp1, tmp2;;

	Time t1(test_num1);
	Time t2(test_num2);
	bool sr1;

	if (t1.get_val() != test_num1) {
		cout << "t1 enable error\t";
		return 0;
	}
	if (t1.get_val() != test_num1) {
		cout << "t2 enable error\t";
		return 0;
	}

	t1 = t1 + t2;

	if (t1.get_val() != (test_num1 + test_num2)) {
		cout << "sum error\t";
		return 0;
	}

	tmp1 = t1.get_val();
	tmp2 = t2.get_val();

	t1 = t1 * t2;

	if (t1.get_val() != (tmp1 * tmp2)) {
		cout << "prosv error\t";
		return 0;
	}

	sr1 = t1 < t2;
	if (sr1) {
		cout << "oper less error\t";
		return 0;
	}

	return 1;
}