#include "gasNodes.hpp"

#include <cmath>
#include <numeric>

using namespace std;

/* GAS NODE */

GasNode::GasNode(const string& name, const GasBasis& basis)
    : name(name), state(basis) {}
GasNode::GasNode(const string& name, const GasState& state)
    : name(name), state(state) {}
GasNode::GasNode(const string& name, const GasState& state,
                 const GasElement& contents)
    : name(name), state(state), contents(contents) {}

void GasNode::rungeKuttaStage1(double timestep) {
    dCdtAlt = dCdt;
    for (int i = 0; i < contents.ns.size(); i++)
        contents.ns[i] += dCdt.ns[i] * 2 / 3 * timestep;
    contents.physH += dCdt.physH * 2 / 3 * timestep;
    dCdt.zero();
    recalculateState();
}

void GasNode::rungeKuttaStage2(double timestep) {
    // At this point, dCdt is the "naive" rate, and dCdtAlt is
    // the RK 2/3 step rate. Ralston's method (2nd order RK) y(t+dt) = y(t)
    // + 0.25 y'(y(t)) dt + 0.75 y'(y(t + 2/3 dt)) dt
    for (int i = 0; i < contents.ns.size(); i++)
        contents.ns[i] =
            contents.ns[i] +
            ((0.25 - 2.0 / 3) * dCdtAlt.ns[i] + 0.75 * dCdt.ns[i]) * timestep;
    contents.physH =
        contents.physH +
        ((0.25 - 2.0 / 3) * dCdtAlt.physH + 0.75 * dCdt.physH) * timestep;
    dCdt.zero();
    recalculateState();
}

/* GAS SOURCE SINK */

GasSourceSink::GasSourceSink(const string& name, GasState state)
    : GasNode(name, state) {
    state.recalculateDerivatives();
}
void GasSourceSink::recalculateState() {}

/* GAS CELL ISOCHORIC */

IsochoricCell::IsochoricCell(const string& name, GasBasis& basis, double V,
                             GasElement contents)
    : GasNode(name, basis, contents), V(V) {
    recalculateState();
}

IsochoricCell::IsochoricCell(const string& name, GasBasis& basis, double V,
                             const vector<double>& ns, double T)
    : GasNode(name, basis), V(V) {
    double n = accumulate(ns.begin(), ns.end(), 0);
    contents.ns = ns;
    contents.physH =
        n * thermo::getHFromPT(thermo::getPFromVT(V, T), T,
                               basis.Cp_);  // thermo H is molar enthalpy
    recalculateState();
}

IsochoricCell::IsochoricCell(const string& name, GasBasis& basis, double V,
                             const vector<double>& xs, double P, double T)
    : GasNode(name, basis), V(V) {
    double n = V / thermo::getVFromPT(P, T);  // thermo V is molar volume
    for (int i = 0; i < contents.ns.size(); i++) contents.ns[i] = xs[i] * n;
    contents.physH =
        n * thermo::getHFromPT(P, T, basis.Cp_);  // thermo H is molar
    recalculateState();
}

void IsochoricCell::recalculateState() {
    contents.n = accumulate(contents.ns.begin(), contents.ns.end(), 0);
    double negative = (contents.n <= 0) ? -1 : 1;
    state.clearDerivatives();
    state.physH_ = contents.physH / contents.n;
    state.V_ =
        V / abs(contents.n);  // if we let this be negative, getTFromHV breaks
    state.T = thermo::getTFromHV(state.physH_, state.V_, state.basis.Cp_);
    // allow negative pressure; it assists RK and shouldn't break anything.
    state.P = (contents.n < 0 ? -1 : 1) * thermo::getPFromVT(state.V_, state.P);
    state.recalculateDerivatives();
}

/* GAS CELL ISOCBARIC */

IsobaricCell::IsobaricCell(const string& name, GasBasis& basis, double P,
                           GasElement contents)
    : GasNode(name, basis, contents), P(P) {
    recalculateState();
}

IsobaricCell::IsobaricCell(const string& name, GasBasis& basis, double P,
                           const vector<double>& ns, double T)
    : GasNode(name, basis), P(P) {
    contents.ns = ns;
    contents.n = accumulate(ns.begin(), ns.end(), 0);
    contents.physH = contents.n * thermo::getHFromPT(P, T, basis.Cp_);
    recalculateState();
}

IsobaricCell::IsobaricCell(const string& name, GasBasis& basis, double P,
                           const vector<double>& xs, double V, double T)
    : GasNode(name, basis), P(P) {
    double n = V / thermo::getVFromPT(P, T);
    for (int i = 0; i < contents.ns.size(); i++) contents.ns[i] = xs[i] * n;
    contents.physH = n * thermo::getHFromPT(P, T, basis.Cp_);
    recalculateState();
}

void IsobaricCell::recalculateState() {
    contents.n = accumulate(contents.ns.begin(), contents.ns.end(), 0);
    double negative = (contents.n <= 0) ? -1 : 1;
    state.clearDerivatives();
    state.physH_ = contents.physH / contents.n;
    state.T = thermo::getTFromHP(contents.physH, state.P, state.basis.Cp_);
    state.V_ = thermo::getVFromPT(state.P, state.T);
    state.recalculateDerivatives();
}