#pragma once

#include <algorithm>
#include <numeric>
#include <string>
#include <vector>

#include "thermo.hpp"

struct GasSpecies {
    const std::string name;
    const double molarMass;
    const double chemH_;
    GasSpecies(std::string name, double molarMass, double chemH_);
};

struct GasBasis {
    const std::string name;
    const double gamma;  // Cp/Cv
    const double Cp_;    // isobaric
    const double Cv_;    // isochoric
    std::vector<GasSpecies*> components;
    GasBasis(std::string name, double gamma,
             const std::vector<GasSpecies*>& components);
};

// GasState and GasElement are similar, but also not.
// GasState describes state information, and provides thermo/physics helpers.
// GasElement describes quantitites, and provides arithmetic helpers.
// Note that in many cases some helper functions should be overridden to
// short-circuit unnecessary thermodynamic calculations.
// Everything inlined for speed; most of these are very simple or wrap thermo.

// Every time GasState is modified (typically recalculateState()), you should
// call clearCache(). It is permissible to externally populate cache values.

struct GasState {
    // Fundamental properties
    double V_;
    double T;
    double P;
    const GasBasis& basis;
    std::vector<double> xs;
    // Thermodynamic property caches
    double averageMolarMass;
    double density;
    double physH_;
    double chemH_;
    double totalH_;
    GasState(const GasBasis& basis);
    double getMolarAverage(std::vector<double> componentValues);
    inline void clearDerivatives();
    inline void recalculateDerivatives();
};

struct GasElement {
    double n;
    std::vector<double> ns;
    double physH;
    // Arithmetic helpers
    inline GasElement& zero();
    inline GasElement& operator+=(GasElement& other);
    inline GasElement& operator-=(GasElement& other);
    inline GasElement& operator*=(double multiplier);
    inline GasElement& operator/=(double denominator);
};

/* IMPLEMENTATIONS: Since most of the code is inline, I did away with the .cpp
 * altogether. */

/* GAS SPECIES */

inline GasSpecies::GasSpecies(std::string name, double molarMass, double chemH_)
    : name(name), molarMass(molarMass), chemH_(chemH_) {}

/* GAS BASIS */

inline GasBasis::GasBasis(std::string name, double gamma,
                   const std::vector<GasSpecies*>& components)
    : name(name),
      gamma(gamma),
      Cv_(thermo::R / (gamma - 1)),
      Cp_(thermo::R / (gamma - 1) * gamma),
      components(components) {}

/* GAS STATE */

inline GasState::GasState(const GasBasis& basis)
    : basis(basis),
      T(thermo::T0),
      P(thermo::P0),
      V_(thermo::getVFromPT(thermo::P0, thermo::T0)),
      averageMolarMass(NAN),
      density(NAN),
      physH_(NAN),
      chemH_(NAN),
      totalH_(NAN) {}

inline double GasState::getMolarAverage(std::vector<double> componentValues) {
    return std::inner_product(xs.begin(), xs.end(), componentValues.begin(), 0);
}

/* ACTUAL INLINE IMPLEMENTATIONS */

// Uncomputed derivative properties are represented as NaN. The procedure for
// computing a state should be:
// 1. clearDerivatives()
// 2. Compute and set P, V_, T, and possibly some derivatives in the process.
// 3. recalculateDerivatives() to standard compute remaining unknowns.

inline void GasState::clearDerivatives() {
    averageMolarMass = NAN;
    density = NAN;
    physH_ = NAN;
    chemH_ = NAN;
    totalH_ = NAN;
}

inline void GasState::recalculateDerivatives() {
    if (averageMolarMass != NAN) {
        averageMolarMass = 0;
        for (int i = 0; i < xs.size(); i++)
            averageMolarMass += basis.components[i]->molarMass * xs[i];
    }
    if (density != NAN) density = averageMolarMass / V_;
    if (physH_ != NAN) physH_ = thermo::getHFromPT(P, T, basis.Cp_);
    if (chemH_ != NAN) {
        chemH_ = 0;
        for (int i = 0; i < xs.size(); i++)
            chemH_ = basis.components[i]->chemH_ * xs[i];
    }
    if (totalH_ != NAN) totalH_ = physH_ + chemH_;
}

/* GAS ELEMENT */

inline GasElement& GasElement::zero() {
    for (int i = 0; i < ns.size(); i++) ns[i] = 0;
    physH = 0;
    return *this;
}

inline GasElement& GasElement::operator+=(GasElement& other) {
    for (int i = 0; i < ns.size(); i++) ns[i] += other.ns[i];
    physH += other.physH;
    return *this;
}

inline GasElement& GasElement::operator-=(GasElement& other) {
    for (int i = 0; i < ns.size(); i++) ns[i] -= other.ns[i];
    physH -= other.physH;
    return *this;
}

inline GasElement& GasElement::operator*=(double multiplier) {
    for (int i = 0; i < ns.size(); i++) ns[i] *= multiplier;
    physH *= multiplier;
    return *this;
}

inline GasElement& GasElement::operator/=(double divisor) {
    for (int i = 0; i < ns.size(); i++) ns[i] /= divisor;
    physH /= divisor;
    return *this;
}
