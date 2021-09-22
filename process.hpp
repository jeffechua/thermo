#pragma once
#include <vector>

#include "gasEdges.hpp"
#include "gasNodes.hpp"
#include "json.hpp"

class Process {
   public:
    double timestep;
    std::map<std::string, std::unique_ptr<GasSpecies>> speciesMap;
    std::map<std::string, std::unique_ptr<GasBasis>> basisMap;
    std::map<std::string, std::unique_ptr<GasNode>> nodeMap;
    std::map<std::string, std::unique_ptr<GasEdge>> edgeMap;
    std::vector<GasNode*> nodeList;
    std::vector<GasEdge*> edgeList;
    Process(nlohmann::json js, double timestep = 0.01);
    void Tick();
};