#pragma once
//SAMPO-main/sampo/scheduler/timeline - 
#include <map>
#include <iostream>

#include "landscape.h"
#include "time.h"
#include "helping_fitch.h"

using namespace std;

class SupplyTimeline {
public:
	map<string, int> _capasity;
	map<string, map<string, int>> _resource_sources;
	map<string, int> _timeline = {};
	map<string, int> _capacity = {};
	vector<pair<Time, int>> material_delivery_list = {};
public:

	SupplyTimeline(LandscapeConfiguration landscape_config) {
		//map<string, >

		for (auto landscape : landscape_config.get_all_resources()) {
			_capasity[landscape.id] = landscape._count;

			for (auto res : landscape)
		}


	}
	/*string _find_best_supply(string material, int count, Time deadline) {
		if (this->_resource_sources.find(material) == this->_resource_sources.end()) {
			cout << "Schedule can not be built. No available resource sources with material" << material << endl;
		}
		vector<string> depots;
		const string depot_id;
		int depotCount;
		for (auto const& [depot_id, depotCount] : this->_resource_sources[material]) {
			if (depotCount >= count) {
				depots.push_back(depotId);
			}
		}
	}*/
	
	string _find_best_supply(string material, int count, Time deadline)
	{
		if (this->_resource_sources.find(material) == this->_resource_sources.end()) {
			cout << "Schedule can not be built. No available resource sources with '" << material << "' material" << endl;
			return;
		}
		//vector<string> depots;
		map<string, int> depots;
		//const string depot_id;
		int depotCount;
		for (auto& depot_el : this->_resource_sources[material]) {
			if (depot_el.second >= count) {
				//depots.push_back(depot_el.first);
				depots.insert(depot_el);
			}
		}

		if (depots.empty()) {
			cout << "Schedule can not be built. No one supplier has enough '" << material << "' material" << endl;
			return;
		}

		for (auto& depot_el : this->_resource_sources[material]) {
			if (depot_el.second >= count) {
				depots.push_back(depot_el.first);
			}
		}
	}
	void append_in_material_delivery_list(Time time, int count, vector<pair<Time, int>> delivery_list, bool simulate, int need_count)
	{
		if (!simulate) {
			if (count > need_count)
				count = need_count;
			delivery_list.push_back(make_pair(time, count));
		}
	}

	void update_material_timeline_and_res_sources(map<int, pair<Time, int>> timeline, map<string, int> mat_sources, string depot)
	{
		for (auto material : material_delivery_list) {
			mat_sources[depot] -= material.second;
			int ind = timeline.max_size() / 2;
			Time timeline_time = timeline[ind].first;
			int timeline_count = timeline[ind].second;
			if (timeline_time == material.first) {
				timeline[ind] = make_pair(material.first, timeline_count - material.second);
			}
			else {
				timeline.insert(material.first, capacity - material.second);
			}
		}
	}

	pair< MaterialDelivery, Time> supply_resources(string work_id, Time deadline,
		vector<Material> materials, bool simulate, Time min_supply_start_time = Time(0)) {
		//assert min_supply_start_time <= deadline;
		deadline = Time(0);
		if (min_supply_start_time <= deadline)
			exit(1);
		MaterialDelivery delivery(work_id);
		Time min_work_start_time = deadline;

		for (Material material : materials) {
			if (material.count == 0)
				continue;
			map<string, int> material_sources = this->_resource_sources[material.name];
			string depot = _find_best_supply(material.name, material.count, deadline);
			int material_timeline = _timeline[depot];
			int capacity = _capacity[depot];
			int need_count = material._count;
			int idx_left = 0;		//rewrite
			int idx_base = 0;
			Time cur_time = deadline - 1;
			//vector<pair<Time, int>> material_delivery_list = {};

			bool going_right = 0;

			while (need_count > 0) {
				int time_left = material_timeline[idx_left][0];
				int time_right = material_timeline[idx_left + 1][0];

				if (going_right) {
					if (cur_time == time_left) {
						int time_left_capacity = material_timeline[idx_left][1];

						if (time_left_capacity) {
							append_in_material_delivery_list(cur_time, time_left_capacity, material_delivery_list, simulate, need_count);
							need_count -= time_left_capacity;
						}
						cur_time = cur_time + 1;
					}
					while ((need_count > 0) && (cur_time < time_right)) {
						append_in_material_delivery_list(cur_time, capacity, material_delivery_list, simulate, need_count);
						need_count -= capacity;
						cur_time = cur_time + 1;
					}
					if (need_count > 0) {
						idx_left += 1;
					}
				}
				else {
					while ((need_count > 0) && (time_left < cur_time) && (min_supply_start_time <= cur_time)) {
						append_in_material_delivery_list(cur_time, capacity, material_delivery_list, simulate, need_count);
						need_count -= capacity;
						cur_time = cur_time - 1;
					}
					if ((need_count > 0) && (cur_time == time_left) && (min_supply_start_time <= cur_time)) {
						int time_left_capacity = material_timeline[idx_left][1];
						if (time_left_capacity) {
							append_in_material_delivery_list(cur_time, time_left_capacity, material_delivery_list, simulate, need_count);
							need_count -= time_left_capacity;
							cur_time = cur_time - 1;
						}

					}
					if (need_count > 0) {
						idx_left -= 1;
						if (idx_left < 0 or cur_time < min_supply_start_time) {
							idx_left = idx_base;
							cur_time = deadline;
							going_right = 1;
						}
					}
				}
			}

			if (!simulate) {
				update_material_timeline_and_res_sources(material_timeline, material_sources);
				delivery.add_deliveries(material.name, material_delivery_list);
			}

			min_work_start_time = max(min_work_start_time, cur_time);
		}
		pair< MaterialDelivery, Time> rez(delivery, min_work_start_time);

		return rez;
	}
	Time find_min_material_time(string id, Time start_time, vector< Material> materials, int batch_size) {
		int sum_materials = 0;

		for (auto material : materials) {
			sum_materials += material.count;
		}

		int ratio = sum_materials / batch_size;
		int batches = max(1, ceill(ratio));

		vector<Material> first_batch;
		for (auto material : materials) {
			material.with_count(material.count / batches);
			first_batch.push_back(material);// (material.with_count(material.count / batches);
		}
		pair< MaterialDelivery, Time> rez = supply_resources(id, start_time, first_batch, 1);

		return rez.second();
	}
};