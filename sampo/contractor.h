#ifndef CONTRACTOR_H
#define CONTRACTOR_H

#include <map>

#include "identifiable.h"
#include "resources.h"

//using namespace std;

typedef std::map<std::string, std::vector<std::pair<std::string, Worker>>> WorkerContractorPool;
                                                     
class Contractor : public Identifiable {
public:
    vector<Worker*> workers;

    explicit Contractor(vector<Worker*>& workers) : workers(workers) {}
};

#endif