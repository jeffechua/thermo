#include "process.hpp"

#include <algorithm>
#include <vector>

#include "gasEdges.hpp"
#include "gasNodes.hpp"

using namespace std;

Process::Process(const std::vector<GasNode*>& nodes,
                 const std::vector<GasEdge*>& edges, double timestep)
    : nodes(nodes), edges(edges), timestep(timestep) {}

void Process::Tick() {
    std::for_each(edges.begin(), edges.end(),
                  [](GasEdge* edge) { edge->calculateFlows(); });
    std::for_each(nodes.begin(), nodes.end(),
                  [this](GasNode* node) { node->rungeKuttaStage1(timestep); });
    std::for_each(edges.begin(), edges.end(),
                  [](GasEdge* edge) { edge->calculateFlows(); });
    std::for_each(nodes.begin(), nodes.end(),
                  [this](GasNode* node) { node->rungeKuttaStage2(timestep); });
}