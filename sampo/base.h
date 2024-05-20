#pragma once
//SAMPO-main/sampo/scheduler/timeline - 

#include <map>
#include <string>
#include <vector>
#include <tuple>
#include <cstdlib>
#include <cstring>
#include <unordered_map>

#include "contractor.h"
#include "landscape.h"
#include "material_timeline.h"
#include "zone_timeline.h"
#include "graph.h"
#include "sheduler_spec.h"
#include "time.h"
#include "time_estimator.h"

//typedef map< GraphNode*, ScheduledWork*> swork_dict_t;

using swork_dict_t = unordered_map<string, ScheduledWork>;
using exec_times_t = unordered_map<string, pair<Time, Time>>;
using worker_pool_t = unordered_map<string, unordered_map<string, Worker>>;


class Timeline {
public:
    Time find_min_start_time(const GraphNode* node,
        const vector<Worker>& worker_team,
        swork_dict_t& node2swork,
        const WorkSpec& spec,
        Time parent_time,
        const WorkTimeEstimator& work_estimator) {
        auto t = this->find_min_start_time_with_additional(node, worker_team, node2swork,
            spec, Time::unassigned(),
            parent_time, work_estimator);
        return get<0>(t);
    }

    //Time schedule_with_inseparables(const GraphNode* node,
    //    const vector<Worker>& worker_team,
    //    swork_dict_t& node2swork,
    //    const WorkSpec& spec,
    //    const Contractor* contractor,
    //    Time start_time,
    //    const vector<const GraphNode*>& inseparable_chain,
    //    const exec_times_t& exec_times,
    //    const WorkTimeEstimator& work_estimator) {
    //    Time c_ft = start_time;
    //    for (const auto* dep_node : inseparable_chain) {
    //        Time max_parent_time = dep_node->min_start_time(node2swork);

    //        pair<Time, Time> node_lag_exec_time;
    //        auto it = exec_times.find(dep_node->id());
    //        //            cout << "111" << endl;
    //        if (it == exec_times.end()) {
    //            node_lag_exec_time = { Time(0), work_estimator.estimateTime(*node->getWorkUnit(), worker_team) };
    //        }
    //        else {
    //            node_lag_exec_time = it->second;
    //        }
    //        //            cout << "222" << endl;

    //        Time c_st = max(c_ft + node_lag_exec_time.first, max_parent_time);
    //        Time new_finish_time = c_st + node_lag_exec_time.second;

    //        vector<MaterialDelivery> deliveries;

    //        node2swork[dep_node->id()] = ScheduledWork(
    //            dep_node->getWorkUnit(),
    //            { c_st, new_finish_time },
    //            worker_team, contractor, vector<Equipment>(), deliveries, ConstructionObject());
    //        //            cout << "333" << endl;
    //        c_ft = new_finish_time;
    //    }

    //    //        cout << "Works scheduled, update timeline start" << endl;

    //    this->update_timeline(node, worker_team, spec, c_ft, c_ft - start_time);
    //    return c_ft;
    //}

    virtual tuple<Time, Time, exec_times_t> find_min_start_time_with_additional(const GraphNode* node,
        const vector<Worker>& worker_team,
        swork_dict_t& node2swork,
        const WorkSpec& spec,
        Time assigned_start_time,
        Time assigned_parent_time,
        const WorkTimeEstimator& work_estimator) const = 0;

    virtual bool can_schedule_at_the_moment(const GraphNode* node,
        const vector<Worker>& worker_team,
        const swork_dict_t& node2swork,
        const WorkSpec& spec,
        Time start_time,
        Time exec_time) const = 0;

    virtual void update_timeline(const GraphNode* node,
        const vector<Worker>& worker_team,
        const WorkSpec& spec,
        Time finish_time,
        Time exec_time) = 0;

    virtual Time schedule(const GraphNode* node,
        const vector<Worker>& worker_team,
        swork_dict_t& node2swork,
        const WorkSpec& spec,
        const Contractor* contractor,
        Time assigned_start_time,
        Time assigned_time,
        Time assigned_parent_time,
        const WorkTimeEstimator& work_estimator) = 0;
};

//class Timeline {
//	virtual Time schedule(GraphNode* node,
//						map< GraphNode&, ScheduledWork*> node2swork,
//						vector< Worker&>& workers,
//						Contractor& contractor,
//						WorkSpec& spec,
//						Time& assigned_start_time = NULL,
//						Time& assigned_time = NULL,
//						Time& assigned_parent_time = Time::Time(0),
//						DefaultWorkEstimator* work_estimator) = 0;
//	virtual tuple<Time, Time, exec_times_t> find_min_start_time_with_additional(const GraphNode* node,
//		const vector<Worker>& worker_team,
//		swork_dict_t& node2swork,
//		const WorkSpec& spec,
//		Time assigned_start_time,
//		Time assigned_parent_time,
//		const WorkTimeEstimator& work_estimator) const = 0;
//	virtual Time find_min_start_time(map<string&, vector< ScheduleEvent>&>& resource_timeline,
//		vector< GraphNode>& inseparable_chain,
//		const WorkSpec* spec,
//		Time parent_time,
//		Time exec_time,
//		vector<Worker*> passed_workers) = 0;
//	virtual bool can_schedule_at_the_moment(const GraphNode* node,
//		vector< Worker>& worker_team,
//		const WorkSpec& spec,
//		map< GraphNode&, ScheduledWork> node2swork,
//		Time start_time,
//		Time exec_time) = 0;
//};