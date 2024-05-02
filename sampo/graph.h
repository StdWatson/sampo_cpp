//файл sampo/schedaler/schemas/graph - это файл Стаса, но тут раскидано по папкам (дадада дирректориям)

#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <tuple>
#include <string>
#include <unordered_map>

#include "time.h"
#include "identifiable.h"
#include "works.h"
#include "scheduled_work.h"


using namespace std;

using swork_dict_t = unordered_map<string, ScheduledWork>;

enum EdgeType {
    InseparableFinishStart,
    LagFinishStart,
    StartStart,
    FinishFinish,
    FinishStart,
    None
};

class GraphNode;

class GraphEdge {
public:
    GraphNode* start;
    GraphNode* finish;
    float lag;
    EdgeType type;

    explicit GraphEdge(GraphNode* start, GraphNode* finish, float lag = 0, EdgeType type = EdgeType::None)
        : start(start), finish(finish), lag(lag), type(type) {}
};
class GraphNode {
private:
    WorkUnit* work_unit;
    std::vector<GraphEdge> parent_edges;
    std::vector<GraphEdge> children_edges;
public:
    // Create plain node with all edge types = FS
    explicit GraphNode(WorkUnit* work_unit, const std::vector<GraphNode*>& parents = {}) : work_unit(work_unit) {
        for (auto* p : parents) {
            GraphEdge edge(p, this, -1, EdgeType::FinishStart);
            parent_edges.push_back(edge);
            p->children_edges.push_back(edge);
        }
    }

    // Create node with custom edge types
    GraphNode(WorkUnit* workUnit, const std::vector<tuple<GraphNode*, float, EdgeType>>& parents)
        : GraphNode(workUnit) {
        add_parents(parents);
    }

    void add_parents(const std::vector<tuple<GraphNode*, float, EdgeType>>& parents) {
        for (const auto& [p, lag, type] : parents) {
            auto edge = GraphEdge(p, this, lag, type);
            parent_edges.emplace_back(edge);
            p->children_edges.emplace_back(edge);
        }
    }

    GraphNode* inseparable_son() const {
        for (const GraphEdge& son : children_edges) {
            if (son.type == EdgeType::InseparableFinishStart) {
                return son.finish;
            }
        }
        return nullptr;
    }

    GraphNode* inseparable_parent() const {
        for (const GraphEdge& parent : parent_edges) {
            if (parent.type == EdgeType::InseparableFinishStart) {
                return parent.start;
            }
        }
        return nullptr;
    }

    std::vector<GraphNode*> parents() const {
        auto parents = std::vector<GraphNode*>();
        for (const GraphEdge& parent : parent_edges) {
            parents.push_back(parent.start);
        }
        return parents;
    }

    std::vector<GraphNode*> children() const {
        auto children = std::vector<GraphNode*>();
        for (const GraphEdge& child : children_edges) {
            children.push_back(child.start);
        }
        return children;
    }

    std::vector<GraphEdge> edgesTo() const {
        return parent_edges;
    }

    std::vector<GraphEdge> edgesFrom() const {
        return children_edges;
    }

    WorkUnit* getWorkUnit() const {
        return work_unit;
    }

    string id() const {
        return getWorkUnit()->id;
    }

    inline bool is_inseparable_parent() {
        return inseparable_son() != nullptr;
    }

    inline bool is_inseparable_son() {
        return inseparable_parent() != nullptr;
    }

    std::vector<const GraphNode*> getInseparableChainWithSelf() const {
        std::vector<const GraphNode*> chain;
        chain.push_back(this);
        auto* child = inseparable_son();
        if (child) {
            auto sub_chain = child->getInseparableChainWithSelf();
            chain.insert(chain.end(), sub_chain.begin(), sub_chain.end());
        }
        return chain;
    }

    Time min_start_time(const swork_dict_t& node2swork) const {
        Time time;
        for (auto& edge : this->parent_edges) {
            auto it = node2swork.find(edge.start->id());
            if (it == node2swork.end()) {
                return Time::inf();
            }
            time = max(time, it->second.start_time());
        }
        return time;
    }
};


//class GraphNode {
//private:
//    WorkUnit* work_unit;
//    vector<GraphEdge*> parent_edges = vector<GraphEdge*>();
//    vector<GraphEdge> children_edges = vector<GraphEdge>();
//    vector< GraphEdge*> _parent_edges = vector< GraphEdge*>();
//public:
//    explicit GraphNode(WorkUnit* work_unit) : work_unit(work_unit) {};
//
//    // Create plain node with all edge types = FS
//    GraphNode(WorkUnit* work_unit, vector<GraphNode*>& parents) : GraphNode(work_unit) {
//        for (auto p : parents) {
//            auto edge = GraphEdge(p, this, -1, EdgeType::FinishStart);
//            parent_edges.emplace_back(edge);
//            p->children_edges.emplace_back(edge);
//        }
//    }
//
//    // Create node with custom edge types
//    GraphNode(WorkUnit* work_unit, vector<tuple<GraphNode*, float, EdgeType>>& parents) : GraphNode(work_unit) {
//        for (auto& tuple : parents) {
//            auto p = get<0>(tuple);
//            auto edge = GraphEdge(p, this, get<1>(tuple), get<2>(tuple));
//            parent_edges.emplace_back(edge);
//            p->children_edges.emplace_back(edge);
//        }
//    }
//
//    vector< GraphEdge*> edges_to() {
//        return _parent_edges;
//    }
//
//    GraphNode* inseparableSon() {
//        for (GraphEdge& son : children_edges) {
//            if (son.type == EdgeType::InseparableFinishStart) {
//                return son.finish;
//            }
//        }
//        return nullptr;
//    }
//
//    GraphNode* inseparableParent() {
//        for (const auto& parent : parent_edges) {
//            if (parent->type == EdgeType::InseparableFinishStart) {
//                return parent->start;
//            }
//        }
//        return nullptr;
//    }
//
//    vector<GraphNode*> parents() {
//        auto parents = vector<GraphNode*>();
//        for (const auto& parent : parent_edges) {
//            parents.emplace_back(parent->start);
//        }
//        return parents;
//    }
//
//    vector<GraphNode*> children() {
//        auto children = vector<GraphNode*>();
//        for (const auto& child : children_edges) {
//            children.emplace_back(child.start);
//        }
//        return children;
//    }
//
//    vector<GraphEdge*> edgesTo() {
//        return parent_edges;
//    }
//
//    vector<GraphEdge> edgesFrom() {
//        return children_edges;
//    }
//
//    WorkUnit* getwork_unit() {
//        return work_unit;
//    }
//    /*WorkUnit work_unit() {
//        return work_unit;
//    }*/
//
//    string id() {
//        return getwork_unit()->id;
//    }
//
//    vector<GraphNode*> getInseparableChainWithSelf() {
//        auto chain = vector<GraphNode*>();
//        chain.push_back(this);
//        auto child = inseparableSon();
//        if (child) {
//            auto subChain = child->getInseparableChainWithSelf();
//            chain.insert(chain.end(), subChain.begin(), subChain.end());
//        }
//        return chain;
//    }
//    /*vector<GraphNode> get_inseparable_chain_with_self() {
//        auto chain = vector<GraphNode>();
//        chain.push_back(this);
//        auto child = inseparableSon();
//        if (child) {
//            auto subChain = child->getInseparableChainWithSelf();
//            chain.insert(chain.end(), subChain.begin(), subChain.end());
//        }
//        return chain;
//    }*/
//    Time min_start_time(map< GraphNode*, ScheduledWork*> node2swork) {
//        vector<GraphEdge*> edges = edges_to();
//
//        for (auto edge : edges) {
//            if (node2swork[edge->start] != NULL)
//                node2swork[edge->start]->finish_time() + int(edge->lag);
//        }
//
//        return Time(0);
//    }
//};

class WorkGraph {
public:
    GraphNode* start;
    GraphNode* finish;
    vector<GraphNode*> nodes;

    // `nodes` param MUST be a vector with topologically-ordered nodes
    explicit WorkGraph(const vector<GraphNode*>& nodes)
        : start(nodes[0]), finish(nodes[nodes.size() - 1]), nodes(nodes) {}
};

#endif