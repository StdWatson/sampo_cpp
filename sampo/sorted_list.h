#pragma once
// SAMPO-main/sampo/schemas/sorted_list - 

#ifndef SAMPO_SORTED_LIST_H
#define SAMPO_SORTED_LIST_H

#include <set>

#include "types.h"

//#include "native/schemas/dtime.h"

//enum EventType {
//    START = 0,
//    END = 1
//};

template <typename T>
class ScheduleEvent {           //�������� � types.h
public:
    EventType type;
    Time time;
    int event_idx;
    T* obj;
    int available_workers_count;

    ScheduleEvent(EventType type, Time time, int event_idx, T* obj = nullptr, available_workers_count)
        : type(type), time(time), event_idx(event_idx), obj(obj), available_workers_count(available_workers_count) {}
};

struct event_cmp {
    template<typename T>
    bool operator()(const ScheduleEvent<T>& a,
        const ScheduleEvent<T>& b) const {
        if (a.time != b.time) { 
            return a.time > b.time;
        }
        if (a.type != b.type) {
            return a.type > b.type;
        }
        return a.event_idx > b.event_idx;
    }
};

template <typename T>
class EventSortedList {
private:
    std::set<ScheduleEvent<T>, event_cmp> data;
public:
    std::set<ScheduleEvent<T>>::const_iterator begin() const {
        return data.cbegin();
    }

    std::set<ScheduleEvent<T>>::const_iterator end() const {
        return data.cend();
    }

    std::set<ScheduleEvent<T>>::const_iterator bisect_right(const ScheduleEvent<T>& value) const {
        // TODO Test it is equals to Python variant
        typename std::set<ScheduleEvent<T>>::const_iterator bound = data.upper_bound(value);
        if (bound != data.cbegin()) {
            bound++;
            if (bound == value) {
                return bound - 1;
            }
        }
        return bound;
    }

    std::set<ScheduleEvent<T>>::const_iterator bisect_right(const Time& timestamp) const {
        return bisect_right(ScheduleEvent<T>(EventType::END, timestamp, TIME_INF));
    }

    std::set<ScheduleEvent<T>>::const_iterator bisect_left(const ScheduleEvent<T>& value) const {
        // TODO Test it is equals to Python variant
        return data.lower_bound(value);
    }

    std::set<ScheduleEvent<T>>::const_iterator bisect_left(const Time& timestamp) const {
        return bisect_left(ScheduleEvent<T>(EventType::END, timestamp, TIME_INF));
    }

    void add(const ScheduleEvent<T>& value) {
        data.insert(value);
    }

    [[nodiscard]] size_t size() const {
        return data.size();
    }
};

#endif //SAMPO_SORTED_LIST_H