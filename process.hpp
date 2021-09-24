#pragma once
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
    std::vector<std::tuple<GasNode*, IopGasState*>> nodeStateBindings;
    std::vector<std::tuple<GasNode*, IopGasElement*>> nodeElementBindings;
    std::vector<std::tuple<GasEdge*, IopGasState*>> edgeStateBindings;
    std::vector<std::tuple<GasEdge*, IopGasElement*>> edgeElementBindings;

    Process(nlohmann::json js);
    void tick();
    void writeToBindings();
    void bindNode(std::string name, IopGasState* view);
    void bindNode(std::string name, IopGasElement* view);
    void bindEdge(std::string name, IopGasState* view);
    void bindEdge(std::string name, IopGasElement* view);
};