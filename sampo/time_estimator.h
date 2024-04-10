#pragma once
//	SAMPO-main/sampo/schemas/time_estimator -
#include <string>
#include <map>
#include <vector>

#include "resources.h"
#include "collections_util.h"


using namespace std;

//class WorkEstimationMode {
//public:
//	int Pessimistic = -1;
//	int Realistic = 0;
//	int Optimistic = 1;
//public:
//	int get_Pessimistic() {
//		return Pessimistic;
//	}
//	int get_Realistic() {
//		return Realistic;
//	}
//	int get_Optimistic() {
//		return Optimistic;
//	}
//};
enum WorkEstimationMode {
	Pessimistic = -1,
	Realistic = 0,
	Optimistic = 1
};
class WorkTimeEstimator {
public:
	//WorkEstimationMode workestimationmode;
public:
	virtual void set_estimation_mode(bool use_idle = true, int node = 0) const = 0;

	virtual void set_estimation_mode(std::string mode = "static") const = 0;

	virtual std::map<std::string, int> find_work_resources(std::string work_name, float work_volume, std::vector< std::string> resource_name = {}) const = 0;

	virtual Time estimate_time(WorkUnit* work_unit, std::vector< Worker*> worker_list) const = 0;
};

//typedef struct {
//	int Pessimistic = -1;
//	int Realistic = 0;
//	int Optimistic = 1;
//}WorkEstimationMode;


//class WorkTimeEstimator {
//
//};

float communication_coefficient(int groups_count, int max_groups) {
	int n = groups_count;
	int m = max_groups;

	return 1 / ((6 * m * m) * (-2 * n * n * n + 3 * n * n + (6 * m * m - 1) * n));
}

class DefaultWorkEstimator : virtual WorkTimeEstimator {
//public:
private:
	bool _use_idle;
	//WorkEstimationMode _WorkEstimationMode;
	WorkerProductivityMode _WorkerProductivityMode;
	int _estimation_mode;
	string _productivity_mode;
	//Random rand;

public:
	DefaultWorkEstimator(bool _use_idle = 1, int _estimation_mode, string _productivity_mode) : _estimation_mode(_estimation_mode),
		_productivity_mode(_productivity_mode) {
		_estimation_mode = WorkEstimationMode::Realistic;
		_productivity_mode = _WorkerProductivityMode.Static;
	}
	//map<string, int> find_work_resources(std::string work_name, float work_volume, std::vector< std::string> resource_name = {}) {
	std::map<std::string, int> find_work_resources(std::string work_name, float work_volume, std::vector< std::string> resource_name = {}) {
		if (resource_name.max_size() == 0) {
			resource_name = { "driver", "fitter", "manager", "handyman", "electrician", "engineer" };
		}
		std::map<std::string, int> resources;
		for (const std::string& name : resource_name) {
			resources[name] = rand.poisson(pow(work_volume, 0.5));
		}
		return resources;
	}
	float get_productivity_of_worker(Worker* worker, int max_groups = 0, string productivity_mode = "Static") {
		return worker->get_productivity(productivity_mode) * communication_coefficient(worker->count, max_groups);
	}
	Time estimate_time(WorkUnit* work_unit, std::vector< Worker*> worker_list) {
		if (worker_list.size() == 0)
			return Time(0);
		vector<Time> times = { Time(0) };
		std::map<std::string, Worker*> name2worker = build_index_str_wrk(worker_list);

		for (auto req : work_unit->worker_reqs) {
			int worker_count = 0;
			//DefaultWorkEstimator def_est;

			if (req->get_min_count() == 0)
				continue;
			string name = req->get_kind();
			Worker* worker = name2worker[name];

			if (worker == NULL)
				worker_count = 0;
			else
				worker_count = worker->count;
			if (worker_count < req->get_min_count())
				return Time::inf();
			float productivity = this->get_productivity_of_worker(worker, req->get_max_count(), _productivity_mode) / worker_count;

			if (productivity == 0)
				return Time::inf();
			times.push_back(req->get_volume() / productivity);
		}
		Time prev_time = times[0], max_time = Time(0);

		for (auto ob_time : times) {
			max_time = max(ob_time, prev_time);
			prev_time = max_time;
		}

		return max(max_time, Time(0));
	}

};