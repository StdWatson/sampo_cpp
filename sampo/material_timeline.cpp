//SAMPO-main/sampo/scheduler/timeline - 

#include "material_timeline.h"

using namespace std;

SupplyTimeline::SupplyTimeline(LandscapeConfiguration landscape_config) {
		//map<string, >
	//this->resource_sources = {};
	this->timeline = {};
	this->capacity = {};
		//map<string, int> res_source = {};
	//std::unordered_map<std::string, int> count;

	for (const auto& landscape : landscape_config.get_all_resources()) {
		this->capacity[landscape.id] = landscape.get_count();
		/*int count;
		string res;*/

		//for (const auto& el : landscape.get_available_resources()) {
		//	//std::unordered_map<std::string, int> res_source = resource_sources[res.second];
		//	this->resource_sources[el.]
		//	res_source[landscape->id] = res.first;
		//}

		//for (const auto& res : landscape.get_available_resources()) {
		//	//std::unordered_map<std::string, int> res_source = resource_sources[res.second];

		//	if (!res_source.size()) {
		//		res_source = {};
		//		resource_sources[res.second] = res_source;
		//	}
		//	res_source[landscape->id] = res.first;
		//}
	}
}
std::string& SupplyTimeline::find_best_supply(const string &material, int count, Time &deadline)
{
	/*переделать*/
	//if (this->resource_sources.find(material) == this->resource_sources.end()) {
	if (this->resource_sources[material].empty()) {
		cout << "Schedule can not be built. No available resource sources with '" << material << "' material" << endl;
		return;
	}
	//vector<string> depots;
	//std::vector<std::unordered_map<string&, int>> depots;
	std::vector<std::unordered_map<int, string&>> depots;
	//const string depot_id;
	//int depotCount;
	for (const auto& depot_el : this->resource_sources[material]) {
	/*const string depot_id;
	int depot_count;
	for (auto [depot_id, depot_count] : this->resource_sources[material]) {*/
		if (depot_el.second >= count) {
			//depots.push_back(depot_el.first);
			//depots.insert(depot_el);
			depots.emplace_back(make_pair( depot_el.second, depot_el.first ));
		}
	}

	if (depots.empty()) {
		cout << "Schedule can not be built. No one supplier has enough '" << material << "' material" << endl;
		return;
	}

	for (const auto& depot_id : depots) {
		//depots[this->timeline[depot_id].bisect_key_left(deadline)] = -this->capacity[depot_id];
		std::unordered_map<string, int> el[this->timeline[depot_id].bisect_key_left(deadline)] = -this->capacity[depot_id];
		depots.emplace_back(el);
	}

	//depots.sort();

	return depots[0][0];
}
void SupplyTimeline::append_in_material_delivery_list(Time& time, int count, std::vector<std::pair<Time, int>>& delivery_list, bool simulate, int need_count)
{
	if (!simulate) {
		if (count > need_count)
			count = need_count;
		//delivery_list.push_back(make_pair(time, count));
		delivery_list.emplace_back(make_pair(time, count));
	}
}
void SupplyTimeline::update_material_timeline_and_res_sources(std::vector<std::pair<Time, int>>& timeline,
	std::unordered_map<std::string, int> mat_sources,
	std::string& depot, int capacity)
{
	for (const auto& material : material_delivery_list) {
		mat_sources[depot] -= material.second;
		//int ind = timeline.max_size() / 2;
		int ind = ind = timeline.bisect_key_left(time);
		const Time& timeline_time = timeline[ind].first;
		int timeline_count = timeline[ind].second;
		if (timeline_time == material.first) {
			timeline[ind] = make_pair(material.first, timeline_count - material.second);
		}
		else {
			//timeline.insert(material.first, capacity - material.second);
			timeline[ind] = make_pair(material.first, timeline_count - material.second);
		}
	}
	Time& time = timeline[0].first;
	int count = timeline[0].second;

	if (!count) {
		int ind = 1;
		bool is_zero_count = true;

		while ((ind < timeline.size() - 1) && (is_zero_count)) {
			Time& next_time = timeline[ind].first;
			int next_count = timeline[ind].second;

			if (!next_count && (next_time == time + 1)) {
				ind++;
				time = next_time;
			}
			else
				is_zero_count = false;
		}
		Time tm_tmp = time + 1;
		if ((ind == timeline.size() - 1) || (timeline[ind].first.val() != tm_tmp.val()))
			ind--;
		/*for (int i = 0; i < ind; i++)
			timeline[i]*/
		auto begin = timeline.cbegin();
		//auto end = &timeline[ind];

		timeline.erase(begin, begin + (ind - 1));
	}
}
pair< MaterialDelivery, Time>& SupplyTimeline::supply_resources(string& work_id, Time& deadline,
	vector<Material>& materials, bool simulate, Time min_supply_start_time = Time::Time(0))
{
	//assert min_supply_start_time <= deadline;
	//deadline = Time(0);
	if (min_supply_start_time <= deadline)
		std::cout << "min_supply_start_time <= deadline\n" << std::endl;
	MaterialDelivery delivery = MaterialDelivery(work_id);
	//*delivery = MaterialDelivery(work_id));
	Time& min_work_start_time = deadline;

	for (const auto& material : materials) {
		if (material.count == 0)
			continue;
		unordered_map<string, int>& material_sources = this->resource_sources[material.name];
		string& depot = find_best_supply(material.name, material.count, deadline);
		vector<pair<Time, int>> material_timeline = timeline[depot];
		int cap = capacity[depot];
		int need_count = material.count;
		int idx_left = material_timeline.bisect_key_right(deadline) - 1;		//rewrite
		int idx_base = material_timeline.bisect_key_right(deadline) - 1;
		Time cur_time = deadline - 1;
		//vector<pair<Time, int>> material_delivery_list = {};

		bool going_right = 0;

		while (need_count > 0) {
			Time time_left = material_timeline[idx_left].first;
			Time time_right = material_timeline[idx_left + 1].first;

			if (going_right) {
				if (cur_time == time_left) {
					int time_left_capacity = material_timeline[idx_left].second;

					if (time_left_capacity) {
						append_in_material_delivery_list(cur_time, time_left_capacity, material_delivery_list, simulate, need_count);
						need_count -= time_left_capacity;
					}
					cur_time = cur_time + 1;
				}
				while ((need_count > 0) && (cur_time < time_right)) {
					append_in_material_delivery_list(cur_time, cap, material_delivery_list, simulate, need_count);
					need_count -= cap;
					cur_time = cur_time + 1;
				}
				if (need_count > 0) {
					idx_left += 1;
				}
			}
			else {
				while ((need_count > 0) && (time_left < cur_time) && (min_supply_start_time <= cur_time)) {
					append_in_material_delivery_list(cur_time, cap, material_delivery_list, simulate, need_count);
					need_count -= cap;
					cur_time = cur_time - 1;
				}
				if ((need_count > 0) && (cur_time == time_left) && (min_supply_start_time <= cur_time)) {
					int time_left_capacity = material_timeline[idx_left].second;
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
			update_material_timeline_and_res_sources(material_timeline, material_sources, depot, cap);
			delivery.add_deliveries(material.name, material_delivery_list);
		}

		min_work_start_time = max(min_work_start_time, cur_time);
	}
	pair< MaterialDelivery, Time> rez(delivery, min_work_start_time);

	//return rez;
	return rez;
}
bool SupplyTimeline::can_schedule_at_the_moment(string& id, Time& start_time, vector< Material>& materials, int batch_size)
{
	return (this->find_min_material_time(id, start_time, materials, batch_size) == start_time);
}
Time& SupplyTimeline::find_min_material_time(string& id, Time& start_time, vector< Material>& materials, int batch_size)
{
	int sum_materials = 0;

	for (const auto& material : materials) {
		sum_materials += material.count;
	}

	int ratio = sum_materials / batch_size;
	int batches = max(1, ceill(ratio));

	vector<Material> first_batch;
	for (auto& material : materials) {
		material.with_count(material.count / batches);
		first_batch.push_back(material);// (material.with_count(material.count / batches);
	}
	pair< MaterialDelivery, Time> rez = supply_resources(id, start_time, first_batch, 1);

	return rez.second;
}
std::tuple<std::vector< MaterialDelivery>, Time, Time> SupplyTimeline::deliver_materials(string& id, Time& start_time, Time& finish_time,
	std::vector< Material>& materials, int batch_size)
{
	int sum_materials = 0;
	for (const auto& material : materials) {
		sum_materials += material.count;
	}
	float ratio = (sum_materials * 1.0) / batch_size;
	int batches = max(1, ceil(ratio));
	vector<Material> first_batch = {};

	for (auto& material : materials) {
		first_batch.emplace_back(material.with_count(material.count / batches));
	}
	vector<Material*> other_batches = {};
	for (int i = 0; i < batches - 2; i++) {
		other_batches.emplace_back(first_batch[i]);
	}
	if (batches > 1) {
		//vector<Material*> zip_materials_first_batch = {};
		for (int i = 0; i < materials.size(); i++) {
			Material& material = materials[i];
			const Material& batch_material = first_batch[i];

			material.copy();

			other_batches.emplace_back(material.with_count(material.count - batch_material.count * (batches - 1)));
		}

		//MaterialDelivery del;
		vector< MaterialDelivery> deliveries = {};
		/*std::pair< MaterialDelivery, Time> delivery(del, Time(0));
		delivery = this->supply_resources(id, start_time, first_batch, false);*/
		std::pair< MaterialDelivery, Time>& delivery = this->supply_resources(id, start_time, first_batch, false);
		deliveries.emplace_back(delivery.first);
		start_time = delivery.second;
		Time max_finish_time = finish_time;

		for (auto& batch : other_batches) {
			vector<Material> vec_batch = {};
			vec_batch.emplace_back(batch);
			//MaterialDelivery& del = this->supply_resources(id, max_finish_time, vec_batch, false, start_time).first;
			//del = this->supply_resources(id, max_finish_time, vec_batch, false, start_time).first;
			//deliveries.emplace_back(del);
			deliveries.emplace_back(this->supply_resources(id, max_finish_time, vec_batch, false, start_time).first);
			max_finish_time = finish_time > max_finish_time ? finish_time : max_finish_time;
		}

		return { deliveries, start_time, max_finish_time };
	}
}
//std::unordered_map<std::string, std::unordered_map<std::string, int>> SupplyTimeline::resource_sources()
//{
//	return resource_sources;
//}