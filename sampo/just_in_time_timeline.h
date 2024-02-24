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
	map<string, vector<pair<Time, int>>> _timeline;
public:
	JustInTimeTimeline (vector < GraphNode>, vector< Contractor> contractors,
		WorkerContractorPool worker_pool, LandscapeConfiguration landscape) {

		for (auto worker : worker_pool) {
			string worker_type = worker.first;
			vector<pair<string, Worker>> worker_offers = worker.second;

			for (pair<string, Worker> workeroffers : worker_offers) {
				_timeline[workeroffers.second.get_contractor_id()][0].first = Time(0);
				_timeline[workeroffers.second.get_contractor_id()][0].second = workeroffers.second.count;//workeroffers.second.count();
				/*_timeline[workeroffers.second.get_contractor_id()].second = workeroffers.second.count();*/
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
			Time max_zone_time = zone_timeline->find_min_start_time(node.getwork_unit()->zone_reqs, max_material_time, Time(0));
			map<GraphNode, pair<Time, Time>> non_rez;
			tuple<Time, Time, map<GraphNode, pair<Time, Time>>> rez_one(max_zone_time, max_zone_time, non_rez);
			
			return rez_one;
		}

		Time max_parent_time = max(node.min_start_time(node2swork), assigned_parent_time);
		Time max_agent_time = Time(0);
		vector<pair<Time, int>> offer_stack = { make_pair(Time(0), 0) };
		int needed_count;

		if (spec.is_independent) {
			for (auto worker : work_team) {
				offer_stack = _timeline[worker.get_agent_id()];
				/*max_agent_time = max(max_agent_time, offer_stack[0][0])*/
				max_agent_time = max(max_agent_time, offer_stack[0].first);
			}
		}
		else {
			for (auto worker : work_team) {
				needed_count = worker.count;
				offer_stack = _timeline[worker.get_contractor_id()];
				int ind = offer_stack.size() - 1;

				while (needed_count > 0) {
					Time offer_time = offer_stack[ind].first;
					int offer_count = offer_stack[ind].second;
					max_agent_time = max(max_agent_time, offer_time);

					if (needed_count < offer_count)
						offer_count = needed_count;
					needed_count -= offer_count;
					ind--;
				}
			}
		}
		Time c_st = max(max_agent_time, max_parent_time);
		Time new_finish_time = c_st;

		//for (auto dep_node : )
	}

};