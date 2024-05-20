#pragma once
//schedaler/schemas/zones -
#include <map>
#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

struct Zone {
	Zone(std::string name, int status) : name(name), status(status) {}
	std::string name;
	int status;
};
class ZoneStatuses {
	virtual int statuses_available() = 0;

	virtual bool match_status(int target, int to_compare) = 0;
};

//class Zone {
//public:
//	string name;
//	int status;
//
//public:
//	//explicit Zone(string name, int status) : name(name), status(status) {};
//	Zone(string name, int status) : name(name), status(status) {};
//};
class DefaultZoneStatuses : ZoneStatuses {
public:

	int statuses_available()
	{
		return 3;
	}
	bool match_status(int status_to_check, int required_status) {
	/*bool match_status(int target, int to_compare) {
		int status_to_check = target;
		int required_status = to_compare;*/
		return ((required_status == 0) || (status_to_check == required_status));
	}
};
struct ZoneConfiguration {
	/*ZoneConfiguration(unordered_map<string, int> start_statuses,
		vector<int>& time_costs,
		DefaultZoneStatuses statuses) : start_statuses(start_statuses), time_costs(time_costs), statuses(statuses) {}*/

	unordered_map<string, int> start_statuses;
	vector<int>& time_costs;
	DefaultZoneStatuses statuses;

	vector<int> change_cost(int from_status, int to_status) {
		vector<int> time_cost_from_to;

		for (auto el : time_costs) {
			time_cost_from_to.emplace_back(el);
		}

		return time_cost_from_to;
	}
};
//class ZoneConfiguration {
//public:
//	unordered_map<string, int>& start_statuses;
//	vector<int>& time_costs;
//	DefaultZoneStatuses statuses;
//public:
//	//change_cost
//};
struct ZoneTransition {
public:
	ZoneTransition(string name,
	int from_status,
	int	to_status,
	Time& start_time,
	Time& end_time) : name(name), from_status(from_status), to_status(to_status), start_time(start_time), end_time(end_time) {}

	string name;
	int from_status;
	int	to_status;
	Time& start_time;
	Time& end_time;
};
//class ZoneTransition {
//public:
//	string name;
//	int from_status;
//	int	to_status;
//	Time start_time;
//	Time end_time;
//public:
//	ZoneTransition(string name, int from_status, int to_status, Time start_time, Time end_time) : name(name), from_status(from_status),
//																								to_status(to_status), start_time(start_time),
//																								end_time(end_time) {}
//};