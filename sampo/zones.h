#pragma once
//schedaler/schemas/zones -
#include <map>
#include <string>
#include <vector>

using namespace std;

class Zone {
public:
	string name;
	int status;

public:
	//explicit Zone(string name, int status) : name(name), status(status) {};
	Zone(string name, int status) : name(name), status(status) {};
};
class DefaultZoneStatuses {
public:

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
	DefaultZoneStatuses statuses;
public:
	//change_cost
};
class ZoneTransition {
public:
	string name;
	int from_status;
	int	to_status;
	Time start_time;
	Time end_time;
public:
	ZoneTransition(string name, int from_status, int to_status, Time start_time, Time end_time) : name(name), from_status(from_status),
																								to_status(to_status), start_time(start_time),
																								end_time(end_time) {}
};