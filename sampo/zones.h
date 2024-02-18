#pragma once
//schedaler/schemas/zones -
#include <map>
#include <string>
#include <vector>

using namespace std;

class DefaultZoneStatuses {
	int statuses_available() {
		return 3;
	}
	int match_status(int status_to_check, int required_status) {
		return ((required_status == 0) || (status_to_check == required_status));
	}
};
class ZoneConfiguration {
public:
	map<string, int> start_statuses;
	vector<int> time_costs;
	int statuses;
public:
	//change_cost
};