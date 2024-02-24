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
	tuple<Time, Time, map<GraphNode, pair<Time, Time>>> find_min_start_time_with_additional(GraphNode node, vector< Worker> worker_team,
		map< GraphNode, ScheduledWork> node2swork, WorkSpec spec, Time assigned_start_time = Time(0), Time assigned_parent_time = Time(0), DefaultWorkEstimator work_estimator) {
		
		vector< GraphNode*> inseparable_chain = node.getInseparableChainWithSelf();
		string contractor_id;

		if (worker_team[0].contractor_id == "")
			contractor_id = worker_team[0].contractor_id;
		else
			contractor_id = "";

		Time max_parent_time = max(apply_time_spec(node.min_start_time(node2swork), assigned_start_time), assigned_parent_time);
		std::map< GraphNode*, Time> nodes_max_parent_times;
		for (auto ins_node : inseparable_chain) {
			nodes_max_parent_times[ins_node] = max(apply_time_spec(ins_node->min_start_time(node2swork), assigned_start_time), assigned_parent_time));
			//nodes_max_parent_times.insert(pair< GraphNode, Time>(ins_node, max(apply_time_spec(ins_node->min_start_time(node2swork), assigned_start_time), assigned_parent_time));
			//nodes_max_parent_times.insert({ ins_node, max(apply_time_spec(ins_node->min_start_time(node2swork), assigned_start_time), assigned_parent_time) });

		}

		Time exec_time = Time(0);

	}
};