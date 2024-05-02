#pragma once
//SAMPO-main/sampo/scheduler/timeline - 
#include <map>
#include <iostream>
#include <cmath>
#include <unordered_map>

#include "landscape.h"
#include "time.h"
#include "helping_fitch.h"
#include "sorted_list.h"

using namespace std;

class SupplyTimeline {
private:
	//map<string, int> _capasity;
	std::unordered_map<std::string, std::unordered_map<std::string, int>> resource_sources;
	std::unordered_map<std::string, std::vector<std::pair<Time, int>>> timeline;
	std::unordered_map<std::string, int> capacity;

	std::string& find_best_supply(const std::string &material, int count, Time &deadline);
	void append_in_material_delivery_list(Time& time, int count, std::vector<std::pair<Time, int>>& delivery_list, bool simulate, int need_count);
	void update_material_timeline_and_res_sources(std::vector<std::pair<Time, int>>& timeline,
		std::unordered_map<std::string, int> mat_sources, std::string& depot, int capacity);
	std::unordered_map<std::string, std::unordered_map<std::string, int>> _resource_sources() // палочку не убирать!
	{
		return resource_sources;
	}

public:
	std::vector<std::pair<Time, int>> material_delivery_list = {};
//public:

	SupplyTimeline(LandscapeConfiguration landscape_config);
//private:
//	string _find_best_supply(string material, int count, Time deadline);
//public:
	/*void append_in_material_delivery_list(Time& time, int count, std::vector<std::pair<Time, int>>& delivery_list, bool simulate, int need_count);
	void update_material_timeline_and_res_sources(std::vector<std::pair<Time, int>>& timeline,
		std::unordered_map<std::string, int> mat_sources, std::string& depot, int capacity);*/
	std::pair< MaterialDelivery, Time>& supply_resources(std::string& work_id, Time& deadline,
		std::vector<Material> &materials, bool simulate, Time min_supply_start_time = Time::Time(0));
	bool can_schedule_at_the_moment(std::string& id, Time& start_time, std::vector< Material>& materials, int batch_size);
	Time& find_min_material_time(std::string& id, Time& start_time, std::vector< Material>& materials, int batch_size);
	std::tuple<std::vector< MaterialDelivery>, Time, Time> deliver_materials(std::string& id, Time& start_time, Time& finish_time,
		std::vector< Material>& materials, int batch_size);
	//std::unordered_map<std::string, std::unordered_map<std::string, int>> resource_sources();
};