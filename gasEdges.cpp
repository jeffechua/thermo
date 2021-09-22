#include "gasEdges.hpp"

#include <cmath>

GasEdge::GasEdge(const std::string& name, GasNode& start, GasNode& end)
    : name(name), start(start), end(end) {}

void GasEdge::bookFlow(double amount) {
    flow.zero();
    int factor = amount / start.contents.n;
    for (int i = 0; i < start.contents.ns.size(); i++)
        flow.ns[i] = (amount > 0 ? start : end).contents.ns[i] * factor;
    start.dCdt -= flow;
    end.dCdt += flow;
}

VelocityHeadLossPipe::VelocityHeadLossPipe(const std::string& name,
                                           GasNode& start, GasNode& end,
                                           double headsLost, double diameter)
    : GasEdge(name, start, end), headsLost(headsLost), diameter(diameter) {}

void VelocityHeadLossPipe::calculateFlows() {
    GasNode& source = (start.state.P > end.state.P) ? start : end;
    GasNode& sink = (&source == &(start)) ? end : start;
    bookFlow(M_PI * diameter * diameter / 4 / source.state.V_ *
             sqrt(2 * (source.state.P - sink.state.P) / headsLost /
                  source.state.density));
}