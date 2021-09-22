#pragma once
#include <vector>

#include "gasEdges.hpp"
#include "gasNodes.hpp"

class Process {
   public:
    double timestep;
    std::vector<GasSpecies*> species;
    std::vector<GasBasis*> basis;
    std::vector<GasNode*> nodes;
    std::vector<GasEdge*> edges;
    Process(const std::vector<GasNode*>&nodes, const std::vector<GasEdge*>& edges,
            double timestep = 0.01);
    void Tick();
};