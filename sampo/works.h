#pragma once
//schedaler/schemas/works -
#include <vector>

#include "identifiable.h"
#include "time.h"
#include "requirements.h"
#include "resources.h"

using namespace std;

//class WorkerReq {
//public:
//    string kind;
//    Time volume;
//    int min_count;
//    int max_count;
//
//    explicit WorkerReq(string& kind, Time volume, int min_count, int max_count)
//        : kind(kind), volume(volume), min_count(min_count), max_count(max_count) {}
//
//    string get_kind() {
//        return kind;
//    }
//    Time get_volume() {
//        return volume;
//    }
//    int get_min_count() {
//        return min_count;
//    }
//    int get_max_count() {
//        return max_count;
//    }
//};
class WorkUnit : public Identifiable {
public:
    /*string id;
    string name;*/
    //Identifiable idef;
    vector<WorkerReq> worker_reqs;
    vector< EquipmentReq> equipment_reqs;
    vector< MaterialReq> material_reqs;
    vector< ConstructionObjectReq> object_reqs;
    vector< ZoneReq> zone_reqs;
    string group;
    float volume;
    bool is_service_unit;
    string volume_type;
    string display_name;
    int workground_size;

public:
    /*explicit WorkUnit(const vector<WorkerReq>& worker_reqs = vector<WorkerReq>(),
        float volume = 1, bool isServiceUnit = false) : worker_reqs(worker_reqs), volume(volume), isServiceUnit(isServiceUnit) {}*/
    /*explicit WorkUnit(const vector<WorkerReq>& worker_reqs = vector<WorkerReq*>(),
        float volume = 1, bool is_service_unit = false) : worker_reqs(worker_reqs), volume(volume), is_service_unit(is_service_unit) {}*/
    explicit WorkUnit(vector<WorkerReq>& worker_reqs,
        float volume = 1, bool is_service_unit = false) : worker_reqs(worker_reqs), volume(volume), is_service_unit(is_service_unit), Identifiable(id, name) {}
    WorkUnit(string id,
            string name,
            const vector<WorkerReq>& worker_reqs = vector<WorkerReq>(),
            const vector<EquipmentReq>& equipment_reqs = vector<EquipmentReq>(),
            const vector<MaterialReq>& material_reqs = vector<MaterialReq>(),
            const vector<ConstructionObjectReq>& object_reqs = vector<ConstructionObjectReq>(),
            const vector<ZoneReq>& zone_reqs = vector<ZoneReq>(),
            string group,
            bool is_service_unit = false,
            float volume = 0,
            string volume_type = "unit",
            string display_name = "",
            int workground_size = 100) : Identifiable(id, name), worker_reqs(worker_reqs),
            equipment_reqs(equipment_reqs), material_reqs(material_reqs), object_reqs(object_reqs),
            zone_reqs(zone_reqs), group(group), is_service_unit(is_service_unit), volume(volume), 
            volume_type(volume_type), display_name(display_name), workground_size(workground_size) {
            //Identifiable idef(id, name);
    }
    float get_volume() {
        return volume;
    }
   /* bool get_isServiceUnit() {
        return isServiceUnit;
    }*/
    /*WorkerReq get_worker_reqs_0() {
        return worker_reqs[0];
    }*/
    vector<WorkerReq> get_worker_reqs() {
        return worker_reqs;
    }
    vector< Material>& need_materials() {
        vector<Material> material;

        for (const auto& req : material_reqs) {
            material.emplace_back(req);
        }

        return material;
    }
};