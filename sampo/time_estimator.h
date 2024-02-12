#pragma once
//	SAMPO-main/sampo/schemas/time_estimator -
#include <string>
#include <map>
#include <vector>

#include "resources.h"

using namespace std;

class WorkEstimationMode {
public:
	int Pessimistic = -1;
	int Realistic = 0;
	int Optimistic = 1;
public:
	int get_Pessimistic() {
		return Pessimistic;
	}
	int get_Realistic() {
		return Realistic;
	}
	int get_Optimistic() {
		return Optimistic;
	}
};

//typedef struct {
//	int Pessimistic = -1;
//	int Realistic = 0;
//	int Optimistic = 1;
//}WorkEstimationMode;


//class WorkTimeEstimator {
//
//};

class DefaultWorkEstimator {
public:
	bool _use_idle;
	WorkEstimationMode _WorkEstimationMode;
	WorkerProductivityMode _WorkerProductivityMode;
	int _estimation_mode;
	string _productivity_mode;
	//Random rand;

public:
	DefaultWorkEstimator(bool _use_idle = 1, int _estimation_mode, string _productivity_mode) : _estimation_mode(_estimation_mode),
		_productivity_mode(_productivity_mode) {
		_estimation_mode = _WorkEstimationMode.Realistic;
		_productivity_mode = _WorkerProductivityMode.Static;
	}
	map<string, int> find_work_resources(string work_name, float work_volume, vector<string> resource_name) {
		if (resource_name.max_size() == 0) {
			resource_name = { "driver", "fitter", "manager", "handyman", "electrician", "engineer" };
		}
		std::map<std::string, int> resources;
		for (const std::string& name : resource_name) {
			resources[name] = rand.poisson(pow(work_volume, 0.5));
		}
		return resources;
	}

};