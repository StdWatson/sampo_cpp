#pragma once
#include <map>
#include <string>
#include <vector>

#include "resources.h"

//using namespace std;

std::unordered_map<std::string, Worker> build_index_str_wrk(vector<Worker> workers) {
	std::unordered_map<std::string, Worker> diction;
	int i = 0;

	//for (auto item : items) {
	//	if (i < workers.size()) {
	//		diction[item] = workers[i];
	//		/*Worker worker = workers[i];
	//		Worker tmp = diction[item];
	//		tmp = worker;*/
	//	}
	//	else
	//		continue;
	//}

	for (auto item : workers) {
		diction[item.get_name()] = item;
	}

	return diction;
}
std::map<std::string, int> build_index_str_zone_int(vector<ZoneReq>& zones) {
	std::map<std::string, int> diction;
	int i = 0;

	for (auto item : zones) {
		diction[item.kind] = item.required_status;
	}

	return diction;
}
std::unordered_map<std::string, int> build_index_str_int(vector<Worker> workers) {
	std::unordered_map<std::string, int> diction;
	int i = 0;

	for (auto item : workers) {
		diction[item.get_name()] = item.get_count();
	}

	return diction;
}