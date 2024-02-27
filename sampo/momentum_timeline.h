#pragma once
//SAMPO-main/sampo/scheduler/timeline -
#include <map>
#include <string>
#include <vector>
#include <tuple>

#include "contractor.h"
#include "landscape.h"
#include "material_timeline.h"
#include "zone_timeline.h"
#include "graph.h"
#include "sheduler_spec.h"
#include "time.h"
#include "time_estimator.h"

class MomentumTimeline {
public:
	WorkerContractorPool worker_pool;
	LandscapeConfiguration landscape;
	map<string, map<string, vector< ScheduleEvent>>> _timeline;
	int _task_index;
	SupplyTimeline _material_timeline;
	ZoneTimeline zone_timeline;

public:
	MomentumTimeline(WorkerContractorPool worker_pool, LandscapeConfiguration landscape) {

	}
	Time apply_time_spec(Time time, Time assigned_start_time) {
		if (assigned_start_time != 0)
			return max(time, assigned_start_time);
		else
			return time;
	}
	Time _find_earliest_time_slot(vector< ScheduleEvent> state,
									Time parent_time,
									Time exec_time,
									int required_worker_count,
									WorkSpec spec) {
		Time current_start_time = parent_time;
		int current_start_idx = state.bisect_right(current_start_time) - 1;

		for (int i = current_start_idx; i < state.size(); i++) {
			int end_idx = state.bisect_right(current_start_time + exec_time);

			if (spec.is_independent) {
				if (end_idx - current_start_idx > 1)
					return state[state.size() - 1].time + 1;
			}
			bool not_enough_workers_found = false;

			for (int idx = end_idx - 1; idx > current_start_idx - 2; idx--) {
				if ((state[idx].available_workers_count < required_worker_count) || (state[idx].time < parent_time)) {
					current_start_idx = max(idx, current_start_idx) + 1;
					not_enough_workers_found = true;
					break;
				}
			}

			if (!not_enough_workers_found)
				break;
			if (current_start_idx >= state.size()) {
				current_start_time = max(parent_time, state[-1].time + 1);
				break;
			}
			current_start_time = state[current_start_idx].time;
		}
		return current_start_time;
	}
	Time _find_min_start_time(map<string, vector< ScheduleEvent>> resource_timeline,
							vector< GraphNode*> inseparable_chain,
							WorkSpec spec,
							Time parent_time,
							Time exec_time,
							vector<Worker*> passed_workers) {
		bool all = true;

		for (auto node : inseparable_chain) {
			all = node->getwork_unit()->is_service_unit;
			if (!all)
				break;
		}
		if (all) {
			return parent_time;
		}

		for (auto node : inseparable_chain) {
			int i = 0;
			for (auto wreq : node->getwork_unit()->worker_reqs) {
				ScheduleEvent initial_event = resource_timeline[wreq->get_kind()][0];
				EventType tmp_event;

				if (initial_event.event_type != tmp_event.INITIAL)
					exit(0);
				if (initial_event.available_workers_count < passed_workers[i]->count)
					return Time::inf();
				i++;
			}
		}
		queue<WorkerReq*> queue;
		for (auto chain : inseparable_chain[0]->getwork_unit()->worker_reqs) {
			queue.push(chain);
		}
		Time start = parent_time;
		vector< WorkerReq *> scheduled_wreqs = {};
		map<string, int> type2count = build_index_str_int(passed_workers);

		for (int i = 0; i < type2count.size(); i++) {
			WorkerReq* wreq = queue.front();
			vector< ScheduleEvent> state = resource_timeline[wreq->get_kind()];

			Time found_start = _find_earliest_time_slot(state, start, exec_time, type2count[wreq->get_kind()], spec);
			if (found_start >= start)
				exit(0);
			if ((scheduled_wreqs.size() == 0) || (start == found_start)) {
				scheduled_wreqs.push_back(wreq);
				start = max(found_start, start);
			}
			else {
				queue.pop();
			}

		}
	}
	tuple<Time, Time, map<GraphNode, pair<Time, Time>>> find_min_start_time_with_additional(GraphNode* node, vector< Worker *> worker_team,
		map< GraphNode, ScheduledWork> node2swork, WorkSpec spec, Time assigned_start_time = Time(0), Time assigned_parent_time = Time(0), DefaultWorkEstimator work_estimator) {
		
		vector< GraphNode*> inseparable_chain = node->getInseparableChainWithSelf();
		string contractor_id;

		if (worker_team[0]->contractor_id == "")
			contractor_id = worker_team[0]->contractor_id;
		else
			contractor_id = "";

		Time max_parent_time = max(apply_time_spec(node->min_start_time(node2swork), assigned_start_time), assigned_parent_time);
		std::map< GraphNode*, Time> nodes_max_parent_times;
		for (auto ins_node : inseparable_chain) {
			nodes_max_parent_times[ins_node] = max(apply_time_spec(ins_node->min_start_time(node2swork), assigned_start_time), assigned_parent_time);
			//nodes_max_parent_times.insert(pair< GraphNode, Time>(ins_node, max(apply_time_spec(ins_node->min_start_time(node2swork), assigned_start_time), assigned_parent_time));
			//nodes_max_parent_times.insert({ ins_node, max(apply_time_spec(ins_node->min_start_time(node2swork), assigned_start_time), assigned_parent_time) });

		}
		Time exec_time = Time(0);
		map< GraphNode *, pair<Time, Time>> exec_times;
		Time node_exec_time = Time(0);

		for (auto chain_node : inseparable_chain) {
			//pair<Time, Time> pair_time = { Time(0), Time(0) };
			if (chain_node->getwork_unit()->worker_reqs.size() == 0)
				node_exec_time = Time(0);
			else
				work_estimator.estimate_time(chain_node->getwork_unit(), worker_team);
			Time lag_req = nodes_max_parent_times[chain_node] - max_parent_time - exec_time;
			Time lag = 0;

			if (lag_req > 0)
				lag = lag_req;
			else
				lag = 0;
			Time lag = exec_times[chain_node].first;
			Time node_exec_timeg = exec_times[chain_node].second;
			Time time_tmp = (lag + node_exec_time);
			exec_time = exec_time + time_tmp;
		}
		if (worker_team.size() == 0) {
			Time max_material_time = _material_timeline.find_min_material_time(node->id(), max_parent_time,
																				node->getwork_unit()->need_materials(),
																				node->getwork_unit()->workground_size);
			Time max_zone_time = this->zone_timeline.find_min_start_time(node->getwork_unit()->zone_reqs, max_parent_time, exec_time);
			max_parent_time = max(max_parent_time, max_material_time);
			max_parent_time = max(max_parent_time, max_zone_time);

			tuple<Time, Time, map<GraphNode, pair<Time, Time>>> rez(max_parent_time, max_parent_time, exec_times);
		}
		Time tmp = Time(0);
		if (assigned_start_time != tmp)
			Time st = assigned_start_time;
		else {
			Time prev_st = max_parent_time;
			Time start_time = _find_min_start_time(_timeline[contractor_id], )
		}

	}
};