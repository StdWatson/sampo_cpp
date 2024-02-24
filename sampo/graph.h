//файл sampo/schedaler/schemas/graph - это файл Стаса, но тут раскидано по папкам (дадада дирректориям)

#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <tuple>
#include <string>

#include "time.h"
#include "identifiable.h"
#include "works.h"
#include "scheduled_work.h"


using namespace std;

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
    GraphNode start;
    GraphNode finish;
    float lag;
    EdgeType type;

    explicit GraphEdge(GraphNode* start, GraphNode* finish, float lag = 0, EdgeType type = EdgeType::None)
        : start(start), finish(finish), lag(lag), type(type) {}
};

class GraphNode {
private:
    WorkUnit* work_unit;
    vector<GraphEdge> parent_edges = vector<GraphEdge>();
    vector<GraphEdge> children_edges = vector<GraphEdge>();
    vector< GraphEdge> _parent_edges = vector< GraphEdge>();
public:
    explicit GraphNode(WorkUnit* work_unit) : work_unit(work_unit) {};

    // Create plain node with all edge types = FS
    GraphNode(WorkUnit* work_unit, vector<GraphNode*>& parents) : GraphNode(work_unit) {
        for (auto p : parents) {
            auto edge = GraphEdge(p, this, -1, EdgeType::FinishStart);
            parent_edges.emplace_back(edge);
            p->children_edges.emplace_back(edge);
        }
    }

    // Create node with custom edge types
    GraphNode(WorkUnit* work_unit, vector<tuple<GraphNode*, float, EdgeType>>& parents) : GraphNode(work_unit) {
        for (auto& tuple : parents) {
            auto p = get<0>(tuple);
            auto edge = GraphEdge(p, this, get<1>(tuple), get<2>(tuple));
            parent_edges.emplace_back(edge);
            p->children_edges.emplace_back(edge);
        }
    }

    vector< GraphEdge> edges_to() {
        return _parent_edges;
    }

    GraphNode* inseparableSon() {
        for (GraphEdge& son : children_edges) {
            if (son.type == EdgeType::InseparableFinishStart) {
                return son.finish;
            }
        }
        return nullptr;
    }

    GraphNode* inseparableParent() {
        for (GraphEdge& parent : parent_edges) {
            if (parent.type == EdgeType::InseparableFinishStart) {
                return parent.start;
            }
        }
        return nullptr;
    }

    vector<GraphNode*> parents() {
        auto parents = vector<GraphNode*>();
        for (GraphEdge& parent : parent_edges) {
            parents.push_back(parent.start);
        }
        return parents;
    }

    vector<GraphNode*> children() {
        auto children = vector<GraphNode*>();
        for (GraphEdge& child : children_edges) {
            children.push_back(child.start);
        }
        return children;
    }

    vector<GraphEdge> edgesTo() {
        return parent_edges;
    }

    vector<GraphEdge> edgesFrom() {
        return children_edges;
    }

    WorkUnit* getwork_unit() {
        return work_unit;
    }
    /*WorkUnit work_unit() {
        return work_unit;
    }*/

    string id() {
        return getwork_unit()->id;
    }

    vector<GraphNode*> getInseparableChainWithSelf() {
        auto chain = vector<GraphNode*>();
        chain.push_back(this);
        auto child = inseparableSon();
        if (child) {
            auto subChain = child->getInseparableChainWithSelf();
            chain.insert(chain.end(), subChain.begin(), subChain.end());
        }
        return chain;
    }

    Time min_start_time(map< GraphNode, ScheduledWork> node2swork) {
        vector<GraphEdge> edges = edges_to();

        for (auto edge : edges) {
            if (node2swork[edge.start] != NULL)
                node2swork[edge.start].finish_time() + int(edge.lag);
        }

        return Time(0);
    }
};

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