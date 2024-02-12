#pragma once

#include <iostream>
#include "interval.h"

using namespace std;

int test_IntervalGaussian(void) {
	int min_val = 1, max_val = 10;
	IntervalGaussian inter1(5, 2, min_val, max_val);
	float val_fl = inter1.randFloat();
	int val_int = inter1.randInt();

	//cout << inter1.randFloat() << endl;
	if ((val_fl < min_val) && (val_fl > max_val))
		return 0;
	if ((val_int < min_val) && (val_int > max_val))
		return 0;

	return 1;
}