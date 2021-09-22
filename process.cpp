#include "process.hpp"

#include <iostream>
#include <algorithm>
#include <vector>

#include "jsonFactories.hpp"

using namespace std;
using namespace nlohmann;

Process::Process(json js, double timestep) : timestep(timestep) {
    auto speciesData = js["species"];
    for_each(speciesData.begin(), speciesData.end(), [this](json js) {
        auto species = factories::deserializeGasSpecies(js);
        speciesMap.emplace(species->name, move(species));
    });

    auto basisData = js["bases"];
    for_each(basisData.begin(), basisData.end(), [this](json js) {
        auto basis = factories::deserializeGasBasis(js, speciesMap);
        basisMap.emplace(basis->name, move(basis));
    });

    auto nodeData = js["nodes"];
    for_each(nodeData.begin(), nodeData.end(), [this](json js) {
        auto node = factories::deserializeGasNode(js, basisMap);
        nodeList.push_back(node.get());
        nodeMap.emplace(node->name, move(node));
    });

    auto edgeData = js["edges"];
    for_each(edgeData.begin(), edgeData.end(), [this](json js) {
        auto edge = factories::deserializeGasEdge(js, nodeMap);
        edgeList.push_back(edge.get());
        edgeMap.emplace(edge->name, move(edge));
    });

}

void Process::Tick() {
    std::for_each(edgeList.begin(), edgeList.end(),
                  [](GasEdge* edge) { edge->calculateFlows(); });
    std::for_each(nodeList.begin(), nodeList.end(),
                  [this](GasNode* node) { node->rungeKuttaStage1(timestep); });
                  return;
    std::for_each(edgeList.begin(), edgeList.end(),
                  [](GasEdge* edge) { edge->calculateFlows(); });
    std::for_each(nodeList.begin(), nodeList.end(),
                  [this](GasNode* node) { node->rungeKuttaStage2(timestep); });
}