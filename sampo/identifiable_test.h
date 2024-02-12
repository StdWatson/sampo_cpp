#pragma once

#include <iostream>
#include "identifiable.h"

using namespace std;

int test_idenf(void) {
	string test_str = "kek1";

	Identifiable t1;

	t1.id = test_str;

	if (t1.get_id() != test_str) {
		cout << "idenf error\t";
		return 0;
	}

	return 1;
}