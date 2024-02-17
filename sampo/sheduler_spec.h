#pragma once
//SAMPO-main/sampo/schemas/schedule_spec - 

#include <vector>
#include <map>

#include "works.h"
#include "types.h"

using namespace std;

class WorkSpec {
	vector< WorkUnit> chain;
	map<WorkerName, int> assigned_workers;
	Time assigned_time;
	bool is_independent = 0;
};