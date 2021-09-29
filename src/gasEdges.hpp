#pragma once
#include <stddef.h>

#include "gas.hpp"
#include "gasNodes.hpp"

class GasEdge {
   public:
    std::string name;
    GasNode& start;
    GasNode& end;
    GasState state;
    GasElement flow;
    virtual void calculateFlows() = 0;
    GasEdge(const std::string& name, GasNode& start, GasNode& end);
    size_t size() const;
    virtual ~GasEdge() = default;

   protected:
    void bookFlow(double amount);
};

class VelocityHeadLossPipe : public GasEdge {
   public:
    double headsLost;
    double diameter;
    void calculateFlows() override;
    VelocityHeadLossPipe(const std::string& name, GasNode& start, GasNode& end,
                         double diameter, double headsLost);
};