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
	tuple<Time, Time, map<GraphNode, pair<Time, Time>>> find_min_start_time_with_additional(GraphNode node, vector< Worker> worker_team,
		map< GraphNode, ScheduledWork> node2swork, WorkSpec spec, Time assigned_start_time = 0, Time assigned_parent_time = Time(0), DefaultWorkEstimator work_estimator) {
		
		vector< GraphNode> inseparable_chain = node.getInseparableChainWithSelf();
	}
};