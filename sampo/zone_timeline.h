#pragma once
//SAMPO-main/sampo/scheduler/timeline -

#include <vector>
#include <queue>
#include <iostream>

#include "zones.h"
#include "types.h"
#include "time.h"
#include "requirements.h"
#include "helping_fitch.h"
#include "types.h"

using namespace std;

class ZoneTimeline {
public:
	ZoneConfiguration _config;
	map<string, int> _timeline;
public:
	ZoneTimeline(ZoneConfiguration _config) {

	}
	bool _match_status(int target, int match) {
		return _config.statuses.match_status(target, match);
	}
	bool _is_inside_interval(vector< ScheduleEvent> state, int idx) {
		int starts_count = 0, ends_count = 0;

		EventType start;
		for (auto cpkt : state[idx]) {
			if (cpkt.event_type == start.START) {
				starts_count++;
			}
			else {
				ends_count++;
			}
		}

		return (starts_count == ends_count);
	}

	Time find_min_start_time(vector< ZoneReq> zones, Time parent_time, Time exec_time) {
		Time start = parent_time;
		vector<ZoneReq> scheduled_wreqs = {};

		queue< ZoneReq> queue;
		for (auto element : zones) {
			queue.push(element);
		}

		ZoneReq wreq;
		int state;
		for (int i = 0; i < queue.size(); i++) {
			wreq = queue.front();
			queue.pop();

			state = _timeline[wreq.kind];
		}

	}
	Time _find_earliest_time_slot(vector< ScheduleEvent> state, Time parent_time, Time exec_time, int required_status) {
		Time current_start_time = current_start_time;
		int current_start_idx = (state.size() / 2) - 1;
		EventType eventtype;

		//for(int i = 0; i < )
		int i = 0;
		while ((state.size() / 2 + 1) > 0) {
			if ((i > 0) && (i % 50 == 0)) {
				cout << "Warning! Probably cycle in looking for earliest time slot: " << i << " iteration" << endl;
				cout << "Current start time: " << int(current_start_time) << ", current start idx: " << current_start_idx << endl;
			}
			i++;

			ScheduleEvent current_element = state.at(current_start_idx);
			int current_start_status = current_element.available_workers_count;
			int end_idx = state.bisect_right(current_start_time + exec_time);

			if (!(_match_status(current_start_status, required_status))) {
				if (current_start_idx == (state.size() - 1)) {
					current_start_time += max(0, (_config.time_costs[current_start_status, required_status] - (current_start_time - state[-1].Time)));
					break;
				}
				if ((state[current_start_idx].event_type == eventtype.START) && (_match_status(current_start_status, required_status))) {
					current_start_idx += 1;
					current_start_time += state[current_start_idx].time;
					continue;
				}
				if (_is_inside_interval(state, current_start_idx)) {
					if (!_match_status(current_start_status, required_status)) {
						current_start_idx++;
						current_start_time = state[current_start_idx].time;
						continue;
					}
				}
				else {
					current_start_time += _config.time_costs[current_start_status, required_status];
					end_idx = state.bisect_right(current_start_time + exec_time)
				}
			}
		}
	}
};