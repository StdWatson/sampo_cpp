#pragma once
//schedaler/schemas/works -
#include <vector>
#include "identifiable.h"
#include "time.h"

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
class WorkUnit : public Identifiable {
public:
    vector<WorkerReq> worker_reqs;
    float volume;
    bool isServiceUnit;

    explicit WorkUnit(const vector<WorkerReq>& worker_reqs = vector<WorkerReq>(),
        float volume = 1, bool isServiceUnit = false) : worker_reqs(worker_reqs), volume(volume), isServiceUnit(isServiceUnit) {}
    float get_volume() {
        return volume;
    }
    bool get_isServiceUnit() {
        return isServiceUnit;
    }
    /*WorkerReq get_worker_reqs_0() {
        return worker_reqs[0];
    }*/
    vector<WorkerReq> get_worker_reqs() {
        return worker_reqs;
    }
};