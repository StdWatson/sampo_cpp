#pragma once
//SAMPO-main/sampo/scheduler/timeline - 

#include <vector>
#include <iostream>
#include <tuple>
#include <map>
#include <vector>

#include "graph.h"
#include "contractor.h"
#include "landscape.h"
#include "scheduled_work.h"
#include "sheduler_spec.h"
#include "time_estimator.h"
#include "material_timeline.h"
#include "zone_timeline.h"

using namespace std;

class JustInTimeTimeline {
public:
	SupplyTimeline *_material_timeline;
	ZoneTimeline *zone_timeline;
	map<string, pair<Time, int>> _timeline;
public:
	JustInTimeTimeline (vector < GraphNode>, vector< Contractor> contractors,
		WorkerContractorPool worker_pool, LandscapeConfiguration landscape) {

		for (auto worker : worker_pool) {
			string worker_type = worker.first;
			vector<pair<string, Worker>> worker_offers = worker.second;

			for (pair<string, Worker> workeroffers : worker_offers) {
				_timeline[workeroffers.second.get_contractor_id]
			}
		}

		SupplyTimeline *_material_timeline = &SupplyTimeline(landscape);
		ZoneTimeline* zone_timeline = &ZoneTimeline(landscape.zone_config);
	}

	tuple<Time, Time, map<GraphNode, pair<Time, Time>>> find_min_start_time_with_additional(GraphNode node,
		vector<Worker> work_team, map<GraphNode, ScheduledWork> node2swork,
		WorkSpec spec, Time assigned_start_time, Time assigned_parent_time = Time(0),
		DefaultWorkEstimator work_estimator = DefaultWorkEstimator()) {

		if (node2swork.size() == 0) {
			Time max_material_time = _material_timeline->find_min_material_time(node.id(), assigned_parent_time,
				node.getwork_unit()->need_materials(),
				node.getwork_unit()->workground_size);
			Time max_zone_time = zone_timeline

			

		}
	}

};