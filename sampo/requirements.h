#pragma once
//sheduler/shemas/requirements -

#include <string>

#include "time.h"
#include "zones.h"

#define DEFAULT_MAX_COUNT 100

using namespace std;

class WorkerReq {
public:
    string kind;
    Time volume;
    int min_count;
    int max_count;

    explicit WorkerReq(string& kind, Time volume, int min_count, int max_count)
        : kind(kind), volume(volume), min_count(min_count), max_count(max_count) {}

    string get_kind() {
        return kind;
    }
    Time get_volume() {
        return volume;
    }
    int get_min_count() {
        return min_count;
    }
    int get_max_count() {
        return max_count;
    }
};
class ZoneReq {
public:
    std::string kind;
    int required_status;
    std::string name;
public:

    Zone to_zone() {
        Zone Z(kind, required_status);
        return Z;
    }
};
