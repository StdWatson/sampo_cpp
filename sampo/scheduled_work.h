#pragma once
#include <string>
#include <vector>
#include <iostream>

#include "works.h"
#include "resources.h"
#include "contractor.h"
#include "landscape.h"
//#include "seria"

using namespace std;

class ScheduledWork {
public:
	vector<string> ignored_fields = { "equipments", "materials", "object" };
	WorkUnit* work_unit;
	pair<Time, Time> start_end_time;
	vector< Worker*> workers;
	Contractor *contractor;
	//string contractor;
	vector< Equipment*> equipments;
	std::vector< ZoneTransition*> zones_pre;
	vector< MaterialDelivery> materials;
	ConstructionObject* object;
private:
	int _cost;

public:
	ScheduledWork(WorkUnit* work_unit,
				std::pair<Time, Time> start_end_time,
				vector< Worker*> workers,
				/*string contractor,*/
				Contractor *contractor,
				vector< Equipment*> equipments = {},
				vector< ZoneTransition*> zones_pre = {},
				vector< MaterialDelivery> materials = {},
				ConstructionObject* object = NULL) : work_unit(work_unit), start_end_time(start_end_time),
				workers(workers), contractor(contractor), equipments(equipments), materials(materials), object(object) {

		//if contractor is not None:
		//	if isinstance(contractor, str) :
		//		self.contractor = contractor
		//	else :
		//		self.contractor = contractor.name if contractor.name else contractor.id
		//else:
		//	self.contractor = ""

		//self.cost = 0
		//for worker in self.workers :
		//	self.cost += worker.get_cost() * self.duration.value

		//if (!(contractor == NULL)) {
		/*if (!(contractor == NULL)) {
			contractor->get_id
		}*/

		_cost = 0;

		for (auto worker : workers) {
			_cost += worker->get_cost() * this->duration().get_val();
		}
	}

	Time duration() {
		Time start = get<0>(start_end_time), end = get<1>(start_end_time);
		return end - start;
	}
	void __str__ (void){
		cout << "ScheduledWork[work_unit = " << this->work_unit->get_id() << "start_end_time = " << this->start_end_time.first.get_val()
			<< this->start_end_time.second.get_val() << "workers = " << this->workers.size() << "contractor = " << this->contractor << "]" << endl;
	}
	void __repr__(void) {
		this->__str__();
	}
	Time start_time() {
		return start_end_time.first;
	}
	void start_time(Time val) {
		start_end_time = make_pair(val, start_end_time.second);
	}
	Time finish_time(void) {
		return start_end_time.second;
	}
	Time min_child_start_time() {
		return work_unit->is_service_unit ? finish_time() : finish_time() + 1;
	}

};