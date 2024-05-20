//SAMPO-main/sampo/scheduler/timeline -

#include "momentum_timeline.h"

MomentumTimeline::MomentumTimeline(WorkerContractorPool worker_pool, LandscapeConfiguration landscape) : worker_pool(worker_pool), landscape(landscape) {
	for (const auto worker_itm : worker_pool) {
		for (const auto& contractor_itm : worker_itm.second) {
			if (!this->timeline[contractor_itm.first].size())
				this->timeline[contractor_itm.first] = {};
			EventType eventInitial = INITIAL;
			ScheduleEvent<Time> shudevent(eventInitial, Time(0), contractor_itm.second->count);
			this->timeline[contractor_itm.first][worker_itm.first].add(shudevent);
		}
	}
	this->task_index = 0;
	this->material_timeline = SupplyTimeline(landscape);
	//this->zone_timeline = &ZoneTimeline::ZoneTimeline(landscape->zone_config);
	//this->zone_timeline = ZoneTimeline::ZoneTimeline(landscape.zone_config);
	/*zone_timeline = ZoneTimeline::ZoneTimeline(landscape.zone_config);*/
	zone_timeline = ZoneTimeline(landscape.zone_config);
}
Time MomentumTimeline::apply_time_spec(const Time& time, Time& assigned_start_time)
{
	if (assigned_start_time != 0)
		return max(time, assigned_start_time);
	else
		return time;
}
//Time MomentumTimeline::find_earliest_time_slot(vector< ScheduleEvent>& state,
//		Time& parent_time,
//		Time& exec_time,
//		int required_worker_count,
//		WorkSpec& spec)
//Time MomentumTimeline::find_earliest_time_slot(EventSortedList< ScheduleEvent>& state,
Time MomentumTimeline::find_earliest_time_slot(EventSortedList< Time>& state,
	Time& parent_time,
	Time& exec_time,
	int required_worker_count,
	WorkSpec* spec)
{
	if (exec_time == 0) {
		return parent_time;
	}

	Time current_start_time = parent_time;
	//int current_start_idx = state.bisect_right(current_start_time)--;
	std::set<ScheduleEvent<Time>>::const_iterator current_start_idx = state.bisect_right(current_start_time);
	std::set<ScheduleEvent<Time>>::const_iterator last_time = state.end()--;

	if (spec->is_independent) {
		//return state[state.size() - 1].time + 1;
		return (*state.end()--).time + 1;
	}

	for (auto i = current_start_idx; i != state.end(); i++) {
		Time tmp = current_start_time + exec_time;
		std::set<ScheduleEvent<Time>>::const_iterator end_idx = state.bisect_right(tmp);

		//if (spec.is_independent) {
		//	//if (end_idx - current_start_idx > 1)
		//		return state[state.size() - 1].time + 1;
		//}
		bool not_enough_workers_found = false;

		for (std::set<ScheduleEvent<Time>>::iterator idx = --end_idx; idx != --current_start_idx; idx--) {
			//if ((state[idx]->available_workers_count < required_worker_count) || (state[idx]->time < parent_time)) {
			if (((*idx).available_workers_count < required_worker_count) || ((*idx).time < parent_time)) {
				current_start_idx = max(idx, current_start_idx);
				current_start_idx++;
				not_enough_workers_found = true;
				break;
			}
		}

		if (!not_enough_workers_found)
			break;
		/*if (current_start_idx >= state.size()) {
			current_start_time = max(parent_time, (*state.begin()).time + 1);
			break;
		}*/
		current_start_time = (*current_start_idx).time;
	}
	return current_start_time;
}
//Time MomentumTimeline::_find_min_start_time(map<string, vector< ScheduleEvent*>> resource_timeline,
//Time MomentumTimeline::find_min_start_time(map<string, vector< ScheduleEvent<Time>>>& resource_timeline,
//											vector< GraphNode*> inseparable_chain,
//											WorkSpec* spec,
//											Time parent_time,
//											Time exec_time,
//											vector<Worker*> passed_workers) 
Time MomentumTimeline::find_min_start_time(unordered_map<string, EventSortedList<Time>>& resource_timeline,
	vector< GraphNode>& inseparable_chain,
	WorkSpec* spec,
	Time& parent_time,
	Time& exec_time,
	vector<Worker>& passed_workers)
{
	bool all = true;

	for (const auto& node : inseparable_chain) {
		all = node.getWorkUnit()->is_service_unit;
		if (!all)
			break;
	}
	if (all) {
		return parent_time;
	}

	for (const auto& node : inseparable_chain) {
		int i = 0;
		//for (const auto& wreq : node->getwork_unit()->worker_reqs) {
		for (const auto& wreq : node.getWorkUnit()->worker_reqs) {
			ScheduleEvent<Time> initial_event = *resource_timeline[wreq.kind].begin();
			EventType tmp_event = INITIAL;

			if (initial_event.event_idx != tmp_event)
				return Time::inf();
			if (initial_event.available_workers_count < passed_workers[i].count)
				return Time::inf();
			i++;
		}
	}
	queue<WorkerReq> queue;
	//for (auto chain : inseparable_chain[0]->getwork_unit()->worker_reqs) {
	for (auto chain : inseparable_chain[0].getWorkUnit()->worker_reqs) {
		queue.push(chain);
	}
	Time start = parent_time;
	vector< WorkerReq> scheduled_wreqs = {};
	unordered_map<string, int> type2count = build_index_str_int(passed_workers);

	for (int i = 0; i < type2count.size(); i++) {
		WorkerReq wreq = queue.front();
		EventSortedList<Time> state = resource_timeline[wreq.get_kind()];

		Time found_start = find_earliest_time_slot(state, start, exec_time, type2count[wreq.get_kind()], spec);
		if (found_start >= start) {
			cout << "found_start >= start" << endl;
			return found_start;
		}
		if ((scheduled_wreqs.size() == 0) || (start == found_start)) {
			scheduled_wreqs.push_back(wreq);
			start = max(found_start, start);
		}
		else {
			for (auto el : scheduled_wreqs) {
				queue.push(el);
			}
			scheduled_wreqs.clear();
			scheduled_wreqs.push_back(wreq);
			start = max(found_start, start);
		}

	}

	return start;
}
//tuple<Time, Time, unordered_map<GraphNode*, pair<Time, Time>>> MomentumTimeline::find_min_start_time_with_additional(GraphNode* node,
//		vector< Worker*> worker_team,
//		unordered_map< GraphNode*, ScheduledWork*> node2swork,
//		WorkSpec* spec,
//		Time* assigned_start_time = NULL,
//		Time assigned_parent_time = Time(0),
//		DefaultWorkEstimator* work_estimator)

//tuple<Time, Time, unordered_map<GraphNode*, pair<Time, Time>>>& MomentumTimeline::find_min_start_time_with_additional(GraphNode* node,
//	vector< Worker>& worker_team,
//	//unordered_map< GraphNode*, ScheduledWork>& node2swork,
//	unordered_map<string, ScheduledWork> node2swork,
//	WorkSpec& spec,
//	Time& assigned_start_time,
//	Time& assigned_parent_time,
//	DefaultWorkEstimator work_estimator)

tuple<Time, Time, unordered_map<GraphNode, pair<Time, Time>>> MomentumTimeline::find_min_start_time_with_additional(GraphNode* node,
	vector< Worker>& worker_team,
	const swork_dict_t node2swork,
	WorkSpec* spec,
	Time& assigned_start_time,
	Time& assigned_parent_time,
	DefaultWorkEstimator* work_estimator)
{
	vector<GraphNode> inseparable_chain = node->getInseparableChainWithSelf();
	string contractor_id;

	if (worker_team[0].contractor_id == "")
		contractor_id = worker_team[0].contractor_id;
	else
		contractor_id = "";

	Time max_parent_time = max(apply_time_spec(node->min_start_time(node2swork), assigned_start_time), assigned_parent_time);
	std::unordered_map< const GraphNode, Time> nodes_max_parent_times;
	for (auto& ins_node : inseparable_chain) {
		nodes_max_parent_times[ins_node] = max(apply_time_spec(ins_node.min_start_time(node2swork), assigned_start_time), assigned_parent_time);
		//nodes_max_parent_times.insert(pair< GraphNode, Time>(ins_node, max(apply_time_spec(ins_node->min_start_time(node2swork), assigned_start_time), assigned_parent_time));
		//nodes_max_parent_times.insert({ ins_node, max(apply_time_spec(ins_node->min_start_time(node2swork), assigned_start_time), assigned_parent_time) });

	}
	Time exec_time = Time(0);
	//unordered_map< const GraphNode*, pair<Time, Time>> exec_times;
	unordered_map<GraphNode, pair<Time, Time>> exec_times;
	Time node_exec_time = Time(0);

	for (const auto& chain_node : inseparable_chain) {
		//pair<Time, Time> pair_time = { Time(0), Time(0) };
		if (chain_node.getWorkUnit()->worker_reqs.size() == 0)
			node_exec_time = Time(0);
		else
			work_estimator->estimate_time(chain_node.getWorkUnit(), worker_team);
		Time lag_req = nodes_max_parent_times[chain_node] - max_parent_time - exec_time;
		Time lag = Time(0);

		if (lag_req > 0)
			lag = lag_req;
		else
			lag = Time(0);
		Time lag = exec_times[chain_node].first;
		Time node_exec_timeg = exec_times[chain_node].second;
		Time time_tmp = (lag + node_exec_time);
		exec_time = exec_time + time_tmp;
	}
	if (worker_team.size() == 0) {
		Time max_material_time = material_timeline.find_min_material_time(node->getWorkUnit()->id, max_parent_time,
			node->getWorkUnit()->need_materials(),
			node->getWorkUnit()->workground_size);
		Time max_zone_time = zone_timeline.find_min_start_time(node->getWorkUnit()->zone_reqs, max_parent_time, exec_time);
		max_parent_time = max(max_parent_time, max_material_time);
		max_parent_time = max(max_parent_time, max_zone_time);

		tuple<Time, Time, map<GraphNode*, pair<Time, Time>>> rez;// = { max_parent_time, max_parent_time, exec_times };

		return std::make_tuple(max_parent_time, max_parent_time, exec_times);
		//return { max_parent_time, max_parent_time, exec_times };
	}
	/*Time* tmp;
	tmp = &Time::Time(0);*/
	Time st = Time(0);
	if (assigned_start_time != NULL)
		st = assigned_start_time;
	else {
		Time prev_st = max_parent_time;
		Time start_time = find_min_start_time(timeline[contractor_id], inseparable_chain, spec, prev_st, exec_time, worker_team);
		Time max_material_time = material_timeline.find_min_material_time(node->getWorkUnit()->id,
			start_time,
			node->getWorkUnit()->need_materials(),
			node->getWorkUnit()->workground_size);
		Time  max_zone_time = zone_timeline.find_min_start_time(node->getWorkUnit()->zone_reqs,
			max_material_time,
			exec_time);
		st = max(max_material_time, max_zone_time);
		st = max(st, start_time);

		int j = 0;
		while (st != prev_st) {
			j++;

			start_time = find_min_start_time(timeline[contractor_id], inseparable_chain, spec, st, exec_time, worker_team);
			max_material_time = material_timeline.find_min_material_time(node->getWorkUnit()->id,
				start_time,
				node->getWorkUnit()->need_materials(),
				node->getWorkUnit()->workground_size);
			max_zone_time = zone_timeline.find_min_start_time(node->getWorkUnit()->zone_reqs, start_time, exec_time);

			prev_st = st;
			st = max(max_material_time, max_zone_time);
			st = max(st, start_time);
		}
	}
	//tuple<Time, Time, map<GraphNode, pair<Time, Time>>> rez = { st, st + exec_time, exec_times };

	return { st, st + exec_time, exec_times };
}
//bool MomentumTimeline::can_schedule_at_the_moment(GraphNode* node,
//													vector< Worker*> worker_team,
//													WorkSpec spec,
//													map< GraphNode, ScheduledWork> node2swork,
//													Time start_time,
//													Time exec_time)
bool MomentumTimeline::can_schedule_at_the_moment(GraphNode* node,
		vector< Worker>& worker_team,
		WorkSpec* spec,
		//map< GraphNode, ScheduledWork>& node2swork,
		swork_dict_t node2swork,
		Time& start_time,
		Time& exec_time)
{
	if (spec->is_independent) {
		EventSortedList<Time> worker_timeline;
		for (const auto& worker : worker_team) {
			worker_timeline = timeline[worker.contractor_id][worker.name];
			Time last_cpkt_time = (*worker_timeline.begin()).time;

			if (last_cpkt_time >= start_time)
				return false;
		}
		return true;
	}
	else {
		Time start = start_time;
		Time end = start_time + exec_time;

		for (const auto& w : worker_team) {
			EventSortedList<Time> state = timeline[w.contractor_id][w.name];
			std::set<ScheduleEvent<Time>>::const_iterator start_idx = state.bisect_right(start);
			std::set<ScheduleEvent<Time>>::const_iterator end_idx = state.bisect_right(end);

			int available_workers_count = (*start_idx--).available_workers_count;

			for (auto i = start_idx; i != end_idx; i++) {
				ScheduleEvent<Time> evnt = (*i);

				if (!(evnt.available_workers_count >= w.count))
					return false;
			}
			if (!(available_workers_count >= w.count))
				return false;
			if (start_idx != end_idx) {
				ScheduleEvent<Time> event = *end_idx--;

				if (!((*state.begin()).available_workers_count >= event.available_workers_count + w.count))
					return false;
			}
			else {
				if (!((*state.begin()).available_workers_count >= available_workers_count))
					return false;
			}
		}
		if (!(material_timeline.can_schedule_at_the_moment(node->getWorkUnit()->id, start_time, node->getWorkUnit()->need_materials(), node->getWorkUnit()->workground_size)))
			return false;
		if (!(zone_timeline.can_schedule_at_the_moment(node->getWorkUnit()->zone_reqs, start_time, exec_time)))
			return false;
		return true;
	}
}
//void MomentumTimeline::update_timeline(Time finish_time, Time exec_time, GraphNode* node, vector<Worker*> worker_team, WorkSpec* spec)
void MomentumTimeline::update_timeline(Time& finish_time, Time& exec_time, GraphNode* node, vector<Worker>& worker_team, WorkSpec* spec)
{
	//int task_index = task_index;
	task_index += 1;
	Time start = finish_time - exec_time;
	Time end = finish_time;

	for (const auto& w : worker_team) {
		EventSortedList<Time> state = timeline[w.contractor_id][w.name];
		std::set<ScheduleEvent<Time>>::const_iterator start_idx = state.bisect_right(start);
		std::set<ScheduleEvent<Time>>::const_iterator end_idx = state.bisect_right(end);
		//int available_workers_count = state[start_idx - 1]->available_workers_count;
		int available_workers_count = (*start_idx--).available_workers_count;

		for (auto i = start_idx; i != end_idx; i++) {
			ScheduleEvent<Time> evnt = (*i);

			if (evnt.available_workers_count < w.count) {
				cout << "evnt->available_workers_count < w->count" << endl;
				return;
			}

			evnt.available_workers_count -= w.count;
		}

		if (available_workers_count < w.count) {
			cout << "available_workers_count < w->count" << endl;
			return;
		}

		ScheduleEvent<Time> evnt = *(--end_idx);
		//ScheduleEvent* evnt = state[end_idx - 1];
		int end_count;
		if (start_idx != end_idx) {
			if (state.begin()->available_workers_count < evnt.available_workers_count + w.count) {
				cout << "state[0]->available_workers_count < evnt->available_workers_count + w->count" << endl;
				return;
			}
			end_count = evnt.available_workers_count + w.count;
		}
		else {
			if (state.begin()->available_workers_count < evnt.available_workers_count) {
				int nm1 = state.begin()->available_workers_count, nm2 = evnt.available_workers_count;
				cout << "state.begin()->available_workers_count < evnt.available_workers_count: " << (nm1 < nm2) << std::endl;
				return;
			}
			end_count = evnt.available_workers_count;
		}
		EventType eventtype = START;
		/*state.add(ScheduleEvent<Time>(task_index, eventtype, start, available_workers_count - w.count));
		state.emplace_back(ScheduleEvent(task_index, eventtype.END, end, end_count));*/
		state.add(ScheduleEvent<Time>(eventtype, start, available_workers_count - w.count));
		state.add(ScheduleEvent<Time>(eventtype = END, end, end_count));
	}
}
//void MomentumTimeline::_schedule_with_inseparables(GraphNode* node,
//												std::map< GraphNode*, ScheduledWork*> node2swork,
//												std::vector< GraphNode*> inseparable_chain,
//												WorkSpec* spec,
//												std::vector<Worker*> worker_team,
//												Contractor* contractor,
//												Time start_time,
//												std::map< GraphNode*, std::pair<Time, Time>> exec_times)
void MomentumTimeline::schedule_with_inseparables(GraphNode* node,
	//std::unordered_map< GraphNode*, ScheduledWork> node2swork,
	swork_dict_t node2swork,
	std::vector<GraphNode>& inseparable_chain,
	WorkSpec* spec,
	std::vector<Worker>& worker_team,
	const Contractor* contractor,
	Time start_time,
	std::unordered_map< GraphNode, pair<Time, Time>>& exec_times)
{
	Time curr_time = start_time;
	Time start_work = Time::Time(0);

	for (int i = 0; i < inseparable_chain.size(); i++) {
		GraphNode chain_node = inseparable_chain[i];
		Time node_lag = exec_times[chain_node].first;
		Time node_time = exec_times[chain_node].second;
		
		start_work = curr_time + node_lag;
		//std::pair<Time, Time> tmp_pair = { start_work, start_work + node_time };

		//ScheduledWork swork(kek, tmp_pair, worker_team, contractor);
		//ScheduledWork swork = ScheduledWork(chain_node->getWorkUnit(), tmp_pair, worker_team, contractor);
		ScheduledWork swork = ScheduledWork::ScheduledWork(chain_node.getWorkUnit(), { start_work, start_work + node_time }, worker_team, contractor);

		curr_time = curr_time + node_time + node_lag;
		node2swork.insert( {chain_node.id(), swork });
	}
	Time tmp = curr_time - start_time;
	this->update_timeline(curr_time, tmp, node, worker_team, spec);

	std::vector<Zone> zones = {};
	for (auto& zone_req : node->getWorkUnit()->zone_reqs) {
		zones.emplace_back(zone_req.to_zone());
	}
	Time tmp = curr_time - start_time;
	node2swork[node->id()].zones_pre = zone_timeline.update_timeline(node2swork.size(), zones, start_time, tmp);
}
//void MomentumTimeline::schedule(GraphNode* node,
//								map< GraphNode*, ScheduledWork*> node2swork,
//								vector< Worker*> workers,
//								Contractor* contractor,
//								WorkSpec* spec,
//								Time* assigned_start_time = NULL,
//								Time* assigned_time = NULL,
//								Time assigned_parent_time = Time::Time(0),
//								DefaultWorkEstimator* work_estimator)
Time MomentumTimeline::schedule(GraphNode* node,
	//map< GraphNode*, ScheduledWork> node2swork,
	swork_dict_t node2swork,
	vector< Worker>& workers,
	Contractor* contractor,
	WorkSpec* spec,
	Time& assigned_start_time,
	Time& assigned_time,
	Time assigned_parent_time = Time::Time(0),
	//WorkTimeEstimator work_estimator = DefaultWorkEstimator());
	DefaultWorkEstimator* work_estimator)
{
	vector< GraphNode >inseparable_chain = node->getInseparableChainWithSelf();
	tuple<Time, Time, unordered_map<GraphNode, pair<Time, Time>>> min_start_time = find_min_start_time_with_additional(node, workers, node2swork, spec, assigned_start_time, assigned_parent_time, work_estimator);
	Time start_time = get<0>(min_start_time);
	unordered_map<GraphNode, pair<Time, Time>> exec_times = get<2>(min_start_time);

	if (assigned_time != NULL) {
		for (const auto& n : inseparable_chain) {
			exec_times[n] = make_pair(Time(0), Time(assigned_time.val() / inseparable_chain.size()));
		}
	}
	schedule_with_inseparables(node, node2swork, inseparable_chain, spec, workers, contractor, start_time, exec_times);
}