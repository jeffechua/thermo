#include "process.hpp"

#include <algorithm>
#include <iostream>
#include <vector>

#include "jsonFactories.hpp"

using namespace nlohmann;

#ifndef NO_EMSCRIPT
using namespace emscripten;
#endif

using namespace std;

Process::Process(string jsData) {
    json js = json::parse(jsData);
    timestep = js["timestep"];
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

void Process::tick(int times) {
    if (times == 0) return;
    for (int i = 0; i < edgeList.size(); i++) edgeList[i]->calculateFlows();
    for (int i = 0; i < nodeList.size(); i++)
        nodeList[i]->rungeKuttaStage1(timestep);
    for (int i = 0; i < edgeList.size(); i++) edgeList[i]->calculateFlows();
    for (int i = 0; i < nodeList.size(); i++)
        nodeList[i]->rungeKuttaStage2(timestep);
    tick(times - 1);
}

#ifndef NO_EMSCRIPT

void Process::updateViews() {
    for (int i = 0; i < nodeStateViews.size(); i++)
        *get<1>(nodeStateViews[i]) = get<0>(nodeStateViews[i])->state;
    for (int i = 0; i < nodeElementViews.size(); i++)
        *get<1>(nodeElementViews[i]) = get<0>(nodeElementViews[i])->contents;
    for (int i = 0; i < edgeStateViews.size(); i++)
        *get<1>(edgeStateViews[i]) = get<0>(edgeStateViews[i])->state;
    for (int i = 0; i < edgeElementViews.size(); i++)
        *get<1>(edgeElementViews[i]) = get<0>(edgeElementViews[i])->flow;
}

val Process::openNodeStateView(string name) {
    GasStateView* newView = new GasStateView(nodeMap[name]->size());
    nodeStateViews.push_back(tuple<GasNode*, unique_ptr<GasStateView>>(
        nodeMap[name].get(), unique_ptr<GasStateView>(newView)));
    return val(typed_memory_view(newView->size, newView->underlier.get()));
}

val Process::openNodeElementView(string name) {
    GasElementView* newView = new GasElementView(nodeMap[name]->size());
    nodeElementViews.push_back(tuple<GasNode*, unique_ptr<GasElementView>>(
        nodeMap[name].get(), unique_ptr<GasElementView>(newView)));
    return val(typed_memory_view(newView->size, newView->underlier.get()));
}

val Process::openEdgeStateView(string name) {
    GasStateView* newView = new GasStateView(edgeMap[name]->size());
    edgeStateViews.push_back(tuple<GasEdge*, unique_ptr<GasStateView>>(
        edgeMap[name].get(), unique_ptr<GasStateView>(newView)));
    return val(typed_memory_view(newView->size, newView->underlier.get()));
}

val Process::openEdgeElementView(string name) {
    GasElementView* newView = new GasElementView(edgeMap[name]->size());
    edgeElementViews.push_back(tuple<GasEdge*, unique_ptr<GasElementView>>(
        edgeMap[name].get(), unique_ptr<GasElementView>(newView)));
    return val(typed_memory_view(newView->size, newView->underlier.get()));
}

EMSCRIPTEN_BINDINGS(process) {
    class_<Process>("Process")
        .constructor<std::string>()
        .function("tick", &Process::tick)
        .function("updateViews", &Process::updateViews)
        .function("openNodeStateView", &Process::openNodeStateView)
        .function("openNodeElementView", &Process::openNodeElementView)
        .function("openEdgeStateView", &Process::openEdgeStateView)
        .function("openEdgeElementView", &Process::openEdgeElementView);
}

#endif