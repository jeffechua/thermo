#include "process.hpp"

#include <algorithm>
#include <iostream>
#include <vector>

#include "jsonFactories.hpp"

using namespace std;
using namespace nlohmann;

Process::Process(json js) : timestep(js["timestep"]) {
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

void Process::tick() {
    for (int i = 0; i < edgeList.size(); i++) edgeList[i]->calculateFlows();
    for (int i = 0; i < nodeList.size(); i++)
        nodeList[i]->rungeKuttaStage1(timestep);
    for (int i = 0; i < edgeList.size(); i++) edgeList[i]->calculateFlows();
    for (int i = 0; i < nodeList.size(); i++)
        nodeList[i]->rungeKuttaStage2(timestep);
}

void Process::writeToBindings() {
    for (int i = 0; i < nodeStateBindings.size(); i++)
        *get<1>(nodeStateBindings[i]) = get<0>(nodeStateBindings[i])->state;
    for (int i = 0; i < nodeElementBindings.size(); i++)
        *get<1>(nodeElementBindings[i]) =
            get<0>(nodeElementBindings[i])->contents;
    for (int i = 0; i < edgeStateBindings.size(); i++)
        *get<1>(edgeStateBindings[i]) = get<0>(edgeStateBindings[i])->state;
    for (int i = 0; i < edgeElementBindings.size(); i++)
        *get<1>(edgeElementBindings[i]) = get<0>(edgeElementBindings[i])->flow;
}

void Process::bindNode(string name, IopGasState* view) {
    nodeStateBindings.push_back(
        tuple<GasNode*, IopGasState*>(nodeMap[name].get(), view));
}

void Process::bindNode(string name, IopGasElement* view) {
    nodeElementBindings.push_back(
        tuple<GasNode*, IopGasElement*>(nodeMap[name].get(), view));
}

void Process::bindEdge(string name, IopGasState* view) {
    edgeStateBindings.push_back(
        tuple<GasEdge*, IopGasState*>(edgeMap[name].get(), view));
}

void Process::bindEdge(string name, IopGasElement* view) {
    edgeElementBindings.push_back(
        tuple<GasEdge*, IopGasElement*>(edgeMap[name].get(), view));
}