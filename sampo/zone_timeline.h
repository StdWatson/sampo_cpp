#pragma once
//SAMPO-main/sampo/scheduler/timeline -

#include <vector>
#include <queue>
#include <iostream>
#include <typeinfo>
#include <unordered_map>

#include "zones.h"
#include "types.h"
#include "time.h"
#include "requirements.h"
#include "helping_fitch.h"
#include "types.h"
#include "collections_util.h"
#include "sorted_list.h"

using namespace std;

template <typename T>
tuple<Time, int, int> event_cmp(const T) {
	if (typeid(T) == typeid(ScheduleEvent)) {
		if (T.event_type == EventType.INITIAL)
			return (Time(-1), -1, T.event_type.priority);
		return (T.time, T.seq_id, T.event_type.priority);
	}
	if (typeid(T) == typeid(Time)) {
		return (T, T.inf().value, 2);
	}
	if (typeid(T) == typeid(tuple<Time, int, int>)) {
		return T;
	}
	cout << "Incorrect type of value: " << typeid(T) << endl;
	return -10;
}
class ZoneTimeline {
private:
	ZoneConfiguration config;
	unordered_map<string, vector<ScheduleEvent>> timeline;

	bool match_status(int target, int match);
	bool is_inside_interval(vector< ScheduleEvent>& state, int idx);
	int validate(Time& start_time, Time& exec_time, vector<ScheduleEvent>& state, int required_status);
	Time& find_earliest_time_slot(vector< ScheduleEvent>& state, Time& parent_time, Time& exec_time, int required_status);
public:
	ZoneTimeline(ZoneConfiguration config);
//private:
//	bool match_status(int target, int match);
//	bool is_inside_interval(vector< ScheduleEvent*> state, int idx);
//	int validate(Time start_time, Time exec_time, vector<ScheduleEvent*> state, int required_status);
//public:
	Time& find_min_start_time(vector< ZoneReq>& zones, Time& parent_time, Time& exec_time);
//private:
//	Time _find_earliest_time_slot(vector< ScheduleEvent*> state, Time parent_time, Time exec_time, int required_status);
//public:
	bool can_schedule_at_the_moment(vector< ZoneReq>& zones, Time& start_time, Time& exec_time);
	vector< ZoneTransition>& update_timeline(int index, vector< Zone>& zones, Time& start_time, Time& exec_time);
};

//class ZoneTimeline {
////public:
//private:
//	ZoneConfiguration* _config;
//	map<string, vector<ScheduleEvent*>> _timeline;
//public:
//	ZoneTimeline(ZoneConfiguration* _config);
//private:
//	bool _match_status(int target, int match);
//	bool _is_inside_interval(vector< ScheduleEvent*> state, int idx);
//	int _validate(Time start_time, Time exec_time, vector<ScheduleEvent*> state, int required_status);
//public:
//	Time find_min_start_time(vector< ZoneReq*> zones, Time parent_time, Time exec_time);
//private:
//	Time _find_earliest_time_slot(vector< ScheduleEvent*> state, Time parent_time, Time exec_time, int required_status);
//public:
//	bool can_schedule_at_the_moment(vector< ZoneReq*> zones, Time start_time, Time exec_time);
//	vector< ZoneTransition*> update_timeline(int index, vector< Zone*> zones, Time start_time, Time exec_time);
//};