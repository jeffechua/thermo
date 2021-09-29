#pragma once

#ifndef NO_EMSCRIPT
#include <emscripten/bind.h>
#endif

#include <vector>

#include "gasEdges.hpp"
#include "gasNodes.hpp"
#include "json.hpp"

class Process {
   public:
    double timestep;
    // name-element maps
    std::map<std::string, std::unique_ptr<GasSpecies>> speciesMap;
    std::map<std::string, std::unique_ptr<GasBasis>> basisMap;
    std::map<std::string, std::unique_ptr<GasNode>> nodeMap;
    std::map<std::string, std::unique_ptr<GasEdge>> edgeMap;
    // lists for convenience
    std::vector<GasNode*> nodeList;
    std::vector<GasEdge*> edgeList;
    // bindings are memory locations where we write data to on WriteToBindings()
    std::vector<std::tuple<GasNode*, std::unique_ptr<GasStateView>>>
        nodeStateViews;
    std::vector<std::tuple<GasNode*, std::unique_ptr<GasElementView>>>
        nodeElementViews;
    std::vector<std::tuple<GasEdge*, std::unique_ptr<GasStateView>>>
        edgeStateViews;
    std::vector<std::tuple<GasEdge*, std::unique_ptr<GasElementView>>>
        edgeElementViews;

    Process(std::string jsData);
    ~Process() = default;
    void tick(int times);

#ifndef NO_EMSCRIPT
    void updateViews();
    emscripten::val openNodeStateView(std::string name);
    emscripten::val openNodeElementView(std::string name);
    emscripten::val openEdgeStateView(std::string name);
    emscripten::val openEdgeElementView(std::string name);
#endif

};