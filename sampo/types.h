#pragma once
/*sheduler/shemas/types - набор нужных типов*/
#include <string>
#include <vector>

#include "time.h"

typedef std::string ContractorName;
//std::string WorkerName;
typedef std::string WorkerName;
typedef std::pair< ContractorName, WorkerName> AgentId;

class EventType {
//private:
public:
	int INITIAL = -1;
	int START = 0;
	int END = 1;
//public:
//	int get_INITIAL() {
//		return INITIAL;
//	}
//	int get_START() {
//		return START;
//	}
//	int get_END() {
//		return END;
//	}
};

class ScheduleEvent : public EventType {
//private:
public:
	int seq_id;
	//EventType event_type;
	int event_type;
	//double Time;	//time : Time
	Time time;
	//swork: Optional['ScheduledWork']
	int available_workers_count;
public:
	explicit ScheduleEvent(Time time = Time(0)) : time(time) {
	}
	//ScheduleEvent(int seq_id, EventType event_type, Time time, int available_workers_count) : seq_id(seq_id),
	//	event_type(event_type),	time(time = Time::Time(0)), available_workers_count(available_workers_count) {
	///*	time = Time(0);
	//	this->time = time;*/
	//}
	ScheduleEvent(int seq_id, int event_type, Time time, int available_workers_count) : seq_id(seq_id),
		event_type(event_type), time(time = Time::Time(0)), available_workers_count(available_workers_count) {
		/*	time = Time(0);
			this->time = time;*/
	}

};