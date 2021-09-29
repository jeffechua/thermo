
#include "gas.hpp"

#include <algorithm>
#include <iostream>
#include <numeric>

#include "thermo.hpp"

using namespace std;

/* GAS SPECIES */

GasSpecies::GasSpecies(string name, double molarMass, double chemH_)
    : name(name), molarMass(molarMass), chemH_(chemH_) {}

/* GAS BASIS */

GasBasis::GasBasis(string name, double gamma,
                   const vector<GasSpecies*>& components)
    : name(name),
      gamma(gamma),
      Cv_(thermo::R / (gamma - 1)),
      Cp_(thermo::R / (gamma - 1) * gamma),
      components(components) {}

size_t GasBasis::size() const { return components.size(); }

/* GAS STATE */

GasState::GasState(const GasBasis& basis)
    : basis(basis),
      P(thermo::P0),
      T(thermo::T0),
      xs(vector<double>(basis.components.size())) {}

GasState::GasState(const GasBasis& basis, double P, double T,
                   const vector<double>& xs)
    : basis(basis), T(T), P(P), xs(xs) {
    recalculateDerivatives();
}

size_t GasState::size() const { return basis.components.size(); }

GasState& GasState::operator=(const GasState& state) {
    P = state.P;
    T = state.T;
    V_ = state.V_;
    xs = state.xs;
    averageMolarMass = state.averageMolarMass;
    density = state.density;
    physH_ = state.physH_;
    chemH_ = state.chemH_;
    totalH_ = state.totalH_;
    return *this;
}

double GasState::getMolarAverage(vector<double> componentValues) const {
    return inner_product(xs.begin(), xs.end(), componentValues.begin(), 0);
}

// Uncomputed derivative properties are represented as NaN. The procedure for
// computing a state should be:
// 1. clearDerivatives()
// 2. Compute and set P, V_, T, and possibly some derivatives in the process.
// 3. recalculateDerivatives() to standard compute remaining unknowns.

void GasState::clearDerivatives() {
    V_ = NAN;
    averageMolarMass = NAN;
    density = NAN;
    physH_ = NAN;
    chemH_ = NAN;
    totalH_ = NAN;
}

void GasState::recalculateDerivatives() {
    if (isnan(V_)) V_ = thermo::getVFromPT(P, T);
    if (isnan(averageMolarMass)) {
        averageMolarMass = 0;
        for (int i = 0; i < size(); i++)
            averageMolarMass += basis.components[i]->molarMass * xs[i];
    }
    if (isnan(density)) density = averageMolarMass / V_;
    if (isnan(physH_)) physH_ = thermo::getHFromPT(P, T, basis.Cp_);
    if (isnan(chemH_)) {
        chemH_ = 0;
        for (int i = 0; i < size(); i++)
            chemH_ = basis.components[i]->chemH_ * xs[i];
    }
    if (isnan(totalH_)) totalH_ = physH_ + chemH_;
}

/* GAS ELEMENT */

GasElement::GasElement(size_t size) : ns(vector<double>(size)) {}

size_t GasElement::size() const { return ns.size(); }

GasElement::GasElement(const vector<double>& ns, double physH)
    : ns(ns), physH(physH) {
    recalculateN();
}

GasElement& GasElement::recalculateN() {
    n = accumulate(ns.begin(), ns.end(), 0.0);
    return *this;
}

GasElement& GasElement::zero() {
    for (int i = 0; i < size(); i++) ns[i] = 0;
    physH = 0;
    return *this;
}

GasElement& GasElement::operator+=(GasElement& other) {
    for (int i = 0; i < size(); i++) ns[i] += other.ns[i];
    physH += other.physH;
    return *this;
}

GasElement& GasElement::operator-=(GasElement& other) {
    for (int i = 0; i < size(); i++) ns[i] -= other.ns[i];
    physH -= other.physH;
    return *this;
}

GasElement& GasElement::operator*=(double multiplier) {
    for (int i = 0; i < size(); i++) ns[i] *= multiplier;
    physH *= multiplier;
    return *this;
}

GasElement& GasElement::operator/=(double divisor) {
    for (int i = 0; i < size(); i++) ns[i] /= divisor;
    physH /= divisor;
    return *this;
}

GasStateView::GasStateView(size_t componentCount)
    : size(componentCount + 7),
      underlier(unique_ptr<double[]>(new double[size])),
      P(underlier.get()[0]),
      T(underlier.get()[1]),
      V_(underlier.get()[2]),
      averageMolarMass(underlier.get()[3]),
      density(underlier.get()[3]),
      physH_(underlier.get()[4]),
      chemH_(underlier.get()[5]),
      totalH_(underlier.get()[6]),
      xs(underlier.get() + 7) {}

GasStateView& GasStateView::operator=(const GasState& state) {
    P = state.P;
    T = state.T;
    V_ = state.V_;
    averageMolarMass = state.averageMolarMass;
    density = state.density;
    physH_ = state.physH_;
    chemH_ = state.chemH_;
    totalH_ = state.totalH_;
    for (int i = 0; i < state.size() && i < 10; i++) xs[i] = state.xs[i];
    return *this;
}

GasElementView::GasElementView(size_t componentCount)
    : size(componentCount + 2),
      underlier(unique_ptr<double[]>(new double[componentCount + 2])),
      n(underlier.get()[0]),
      physH(underlier.get()[1]),
      ns(underlier.get() + 2) {}

GasElementView& GasElementView::operator=(const GasElement& element) {
    n = element.n;
    physH = element.physH;
    for (int i = 0; i < element.size() && i < 10; i++) ns[i] = element.ns[i];
    return *this;
}