#include "gasEdges.hpp"

#include <algorithm>
#include <cmath>
#include <iostream>

using namespace std;

GasEdge::GasEdge(const string& name, GasNode& origin, GasNode& target)
    : name(name),
      origin(origin),
      target(target),
      flow(origin.size()),
      state(origin.state.basis) {}

size_t GasEdge::size() const { return state.size(); }

void GasEdge::bookFlow(double amount) {
    state = amount > 0 ? origin.state : target.state;
    flow.zero();
    for (int i = 0; i < origin.size(); i++)
        flow.ns[i] = (amount > 0 ? origin : target).state.xs[i] * amount;
    flow.recalculateN();
    origin.dCdt -= flow;
    target.dCdt += flow;
}

VelocityHeadLossPipe::VelocityHeadLossPipe(const string& name, GasNode& origin,
                                           GasNode& target, double diameter,
                                           double headsLost)
    : GasEdge(name, origin, target), diameter(diameter), headsLost(headsLost) {}

void VelocityHeadLossPipe::calculateFlows() {
    GasNode& source = (origin.state.P >= target.state.P) ? origin : target;
    GasNode& sink = (&source == &(origin)) ? target : origin;
    bookFlow(M_PI * diameter * diameter / 4 / source.state.V_ *
             sqrt(2 * (source.state.P - sink.state.P) / headsLost /
                  source.state.density));
}