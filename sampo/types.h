#pragma once
/*sheduler/shemas/types - набор нужных типов*/
#include <string>

std::string ContractorName;
//std::string WorkerName;
typedef std::string WorkerName;

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
	EventType event_type;
	//double Time;	//time : Time
	Time time;
	//swork: Optional['ScheduledWork']
	int available_workers_count;
public:
	ScheduleEvent() {};
};