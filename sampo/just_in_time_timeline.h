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

using namespace std;

class JustInTimeTimeline {
public:
	JustInTimeTimeline (vector < GraphNode>, vector< Contractor> contractors,
		WorkerContractorPool worker_pool, LandscapeConfiguration landscape){


		//for(auto worker_type : worker_pool.)

		SupplyTimeline _material_timeline(landscape);
	}

	tuple<Time, Time, map<GraphNode, pair<Time, Time>>> find_min_start_time_with_additional(GraphNode node,
		vector<Worker> work_team, map<GraphNode, ScheduledWork> node2swork,
		WorkSpec spec, Time assigned_start_time, Time assigned_parent_time = Time(0),
		DefaultWorkEstimator work_estimator = DefaultWorkEstimator()) {

		if (node2swork.size() == 0) {
			Time max_material_time = 

			Time max_zone_time = ;

		}
	}

};