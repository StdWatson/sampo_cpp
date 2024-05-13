//SAMPO-main/sampo/scheduler/timeline -

#include "zone_timeline.h"

using namespace std;


ZoneTimeline::ZoneTimeline(ZoneConfiguration config) : config(config) {
	for (const auto& config : config.start_statuses) {
		EventType eventInitial = INITIAL;
		timeline[config.first].emplace_back(ScheduleEvent(-1, eventInitial, Time(0), config.second));//.push_back(&ScheduleEvent(-1, eventtype.INITIAL, Time(0), config.second));
	}
}
bool ZoneTimeline::match_status(int target, int match) {
	return config.statuses.match_status(target, match);
}
bool ZoneTimeline::is_inside_interval(vector< ScheduleEvent>& state, int idx) {
	int starts_count = 0, ends_count = 0;

	EventType start = START;
	for (int i = idx; i < state.size(); i++) {
		if (state[i].event_type == start) {
			starts_count++;
		}
		else {
			ends_count++;
		}
	}

	return (starts_count == ends_count);
}
int ZoneTimeline::validate(Time& start_time, Time& exec_time, vector<ScheduleEvent>& state, int required_status) {
	int start_idx = state.bisect_right(start_time);
	int end_idx = state.bisect_right(start_time + exec_time);
	int start_status = state[start_idx - 1].available_workers_count;

	//ScheduleEvent &event = state[;
	for (int i = start_idx; i < end_idx; i++) {
		ScheduleEvent& event = state[i];
		if (!config.statuses.match_status(event.available_workers_count, required_status)) {
			cout << "we change the between statuses" << endl;
			return 0;
		}
	}

	EventType eventtype;
	if ((state[start_idx - 1].event_type == (eventtype = END)) ||
		((state[start_idx - 1].event_type == (eventtype = START)) &&
			config.statuses.match_status(start_status, required_status)) ||
		(state[start_idx - 1].event_type == (eventtype = INITIAL))) {
		cout << state[start_idx - 1].time.val() << " " << state[start_idx - 1].event_type << " " << required_status << start_status << endl;

		return 0;
	}

	return 1;
}
Time& ZoneTimeline::find_min_start_time(vector< ZoneReq>& zones, Time& parent_time, Time& exec_time) {
	Time& start = parent_time;
	vector<ZoneReq> scheduled_wreqs;
	unordered_map<string, int> type2status = build_index_str_zone_int(zones);

	queue< ZoneReq> queue;
	for (const auto& element : zones) {
		queue.push(element);
	}

	ZoneReq wreq;
	std::vector<ScheduleEvent> state;
	Time found_start = Time(0);
	for (int i = 0; i < queue.size(); i++) {
		wreq = queue.front();
		queue.pop();

		state = timeline[wreq.kind];
		found_start = find_earliest_time_slot(state, start, exec_time, type2status[wreq.kind]);

		if (found_start >= start) {
			cout << "found_start >= start" << endl;
			return found_start;
		}
		if ((scheduled_wreqs.size() == 0) || (start == found_start)) {
			scheduled_wreqs.push_back(wreq);
			start = max(start, found_start);
			}
		else {
			for (const auto& itm : scheduled_wreqs) {
				queue.push(itm);
			}
			scheduled_wreqs.clear();
			scheduled_wreqs.push_back(wreq);
			start = max(start, found_start);
		}
	}
	return start;
}
Time& ZoneTimeline::find_earliest_time_slot(vector< ScheduleEvent>& state, Time& parent_time, Time& exec_time, int required_status) {
	Time current_start_time = Time(0);
	int current_start_idx = (state.size() / 2) - 1;
	EventType eventtype;

	int i = 0;
	while ((state.size() / 2 + 1) > 0) {
		if ((i > 0) && (i % 50 == 0)) {
			cout << "Warning! Probably cycle in looking for earliest time slot: " << i << " iteration" << endl;
			cout << "Current start time: " << current_start_time.val() << ", current start idx: " << current_start_idx << endl;
		}
		i++;

		ScheduleEvent current_element = state.at(current_start_idx);
		int current_start_status = current_element.available_workers_count;
		int end_idx = state.bisect_right(current_start_time + exec_time);

		if (!(match_status(current_start_status, required_status))) {
			if (current_start_idx == (state.size() - 1)) {
				current_start_time = current_start_time + max(0, (config.time_costs[current_start_status, required_status] - (current_start_time - state[-1].time).val()));
				break;
			}
			if ((state[current_start_idx].event_type == (eventtype = START)) && (match_status(current_start_status, required_status))) {
				current_start_idx += 1;
				current_start_time = current_start_time + state[current_start_idx].time;
				continue;
			}
			if (is_inside_interval(state, current_start_idx)) {
				if (!match_status(current_start_status, required_status)) {
					current_start_idx++;
					current_start_time = state[current_start_idx].time;
					continue;
				}
			}
			else {
				current_start_time = current_start_time + config.time_costs[current_start_status, required_status];
				end_idx = state.bisect_right(current_start_time + exec_time);
			}
		}
		bool not_compatible_status_found = 0;

		for (int idx = end_idx - 1; idx > -1; idx = (current_start_idx - 1)) {
			if (!match_status(state[idx].available_workers_count, required_status)) {
				current_start_idx = max(idx, current_start_idx) + 1;
				not_compatible_status_found = 1;
				break;
			}
		}
		if (!not_compatible_status_found)
			break;
		if (current_start_idx >= state.size()) {
			current_start_time = max(parent_time, (state[-1].time + 1));
			break;
		}
		current_start_time = state[current_start_idx].time;
	}
	validate(current_start_time, exec_time, state, required_status);

	return current_start_time;
}
bool ZoneTimeline::can_schedule_at_the_moment(vector< ZoneReq>& zones, Time& start_time, Time& exec_time) {
	for (const auto& zone : zones) {
		vector<ScheduleEvent> &state = timeline[zone.kind];
		int start_idx = state.bisect_right(start_time);
		int end_idx = state.bisect_right(start_time + exec_time);
		int start_status = state[start_idx - 1].available_workers_count;

		if (!match_status(start_status, zone.required_status)) {
			int change_cost = config.time_costs[start_status, zone.required_status];
			int new_start_idx = state.bisect_right(start_time - change_cost);

			if (new_start_idx != start_idx)
				return 0;
			if (!is_inside_interval(state, start_idx))
				return 0;
		}

		//ScheduleEvent event;
		for (int idx = start_idx; idx < end_idx; idx++) {
			const ScheduleEvent &event = state[idx];

			if (!match_status(event.available_workers_count, zone.required_status))
				return 0;
		}
	}

	return 1;
}

vector< ZoneTransition>& ZoneTimeline::update_timeline(int index, vector< Zone>& zones, Time& start_time, Time& exec_time)
{
	vector<ZoneTransition> sworks;

	for (const auto& zone : zones) {
		vector<ScheduleEvent> &state = timeline[zone.name];
		int start_idx = state.bisect_right(start_time);
		int start_status = state[start_idx - 1].available_workers_count;

		validate(start_time, exec_time, state, zone.status);
		bool change_cost = config.time_costs[start_status, zone.status] == 0 ? 1 : 0;

		EventType eventtype;
			/*ScheduleEvent evnt1 = ScheduleEvent(index, eventtype.START, start_time - change_cost, 0, zone.status);*/
		ScheduleEvent evnt_start = ScheduleEvent(index, (eventtype = START), start_time - change_cost, zone.status);
		ScheduleEvent evnt_end = ScheduleEvent(index, (eventtype = END), start_time - change_cost + exec_time, zone.status);

		state.emplace_back(evnt_start);
		state.emplace_back(evnt_end);

		if ((start_status != zone.status) && (zone.status != 0)) {
			string _name = "Access card " + zone.name + " status: " + to_string(start_status) + " -> " + to_string(zone.status);
			ZoneTransition zone_lok(_name, start_status, zone.status, (start_time - change_cost), start_time);
			sworks.emplace_back(zone_lok);
			}
		}

	return sworks;
}