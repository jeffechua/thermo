#include "gasEdges.hpp"

#include <algorithm>
#include <cmath>
#include <iostream>

using namespace std;

GasEdge::GasEdge(const string& name, GasNode& start, GasNode& end)
    : name(name),
      start(start),
      end(end),
      flow(start.size()),
      state(start.state.basis) {}

size_t GasEdge::size() const { return state.size(); }

void GasEdge::bookFlow(double amount) {
    state = amount > 0 ? start.state : end.state;
    flow.zero();
    for (int i = 0; i < start.size(); i++)
        flow.ns[i] = (amount > 0 ? start : end).state.xs[i] * amount;
    flow.recalculateN();
    start.dCdt -= flow;
    end.dCdt += flow;
}

VelocityHeadLossPipe::VelocityHeadLossPipe(const string& name, GasNode& start,
                                           GasNode& end, double diameter,
                                           double headsLost)
    : GasEdge(name, start, end), diameter(diameter), headsLost(headsLost) {}

void VelocityHeadLossPipe::calculateFlows() {
    GasNode& source = (start.state.P >= end.state.P) ? start : end;
    GasNode& sink = (&source == &(start)) ? end : start;
    bookFlow(M_PI * diameter * diameter / 4 / source.state.V_ *
             sqrt(2 * (source.state.P - sink.state.P) / headsLost /
                  source.state.density));
}