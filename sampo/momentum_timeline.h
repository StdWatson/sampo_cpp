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
	LandscapeConfiguration* landscape;
	ZoneTimeline* zone_timeline;
private:
	map<string, map<string, vector< ScheduleEvent*>>> _timeline = {};
	int _task_index;
	SupplyTimeline* _material_timeline;

public:
	MomentumTimeline(WorkerContractorPool worker_pool, LandscapeConfiguration* landscape);
	Time apply_time_spec(Time time, Time assigned_start_time);
private:
	Time _find_earliest_time_slot(vector< ScheduleEvent*> state,
		Time parent_time,
		Time exec_time,
		int required_worker_count,
		WorkSpec* spec);
	Time _find_min_start_time(map<string, vector< ScheduleEvent*>> resource_timeline,
		vector< GraphNode*> inseparable_chain,
		WorkSpec* spec,
		Time parent_time,
		Time exec_time,
		vector<Worker*> passed_workers);
public:
	tuple<Time, Time, map<GraphNode*, pair<Time, Time>>> find_min_start_time_with_additional(GraphNode* node,
		vector< Worker*> worker_team,
		map< GraphNode*, ScheduledWork*> node2swork,
		WorkSpec* spec,
		Time* assigned_start_time = NULL,
		Time assigned_parent_time = Time(0),
		DefaultWorkEstimator* work_estimator);

	bool can_schedule_at_the_moment(GraphNode* node,
									vector< Worker*> worker_team,
									WorkSpec spec,
									map< GraphNode, ScheduledWork> node2swork,
									Time start_time,
									Time exec_time);
	void update_timeline(Time finish_time, Time exec_time, GraphNode* node, vector<Worker*> worker_team, WorkSpec* spec);
	void schedule(GraphNode* node,
									map< GraphNode*, ScheduledWork*> node2swork,
									vector< Worker*> workers,
									Contractor* contractor,
									WorkSpec* spec,
									Time* assigned_start_time = NULL,
									Time* assigned_time = NULL,
									Time assigned_parent_time = Time::Time(0),
									//WorkTimeEstimator work_estimator = DefaultWorkEstimator());
									DefaultWorkEstimator* work_estimator);
	void _schedule_with_inseparables(GraphNode* node,
									std::map< GraphNode*, ScheduledWork*> node2swork,
									std::vector< GraphNode*> inseparable_chain,
									WorkSpec* spec,
									std::vector<Worker*> worker_team,
									Contractor* contractor,
									Time start_time,
									std::map< GraphNode*, pair<Time, Time>> exec_times);
};