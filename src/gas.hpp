#pragma once

#include <cmath>
#include <string>
#include <vector>

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
    size_t size() const;
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
    const GasBasis& basis;
    double P;
    double T;
    std::vector<double> xs;
    // Thermodynamic property caches
    double V_ = NAN;
    double averageMolarMass = NAN;
    double density = NAN;
    double physH_ = NAN;
    double chemH_ = NAN;
    double totalH_ = NAN;
    GasState(const GasBasis& basis);
    GasState(const GasBasis& basis, double P, double T,
             const std::vector<double>& xs);
    GasState& operator=(const GasState& state);

    size_t size() const;
    double getMolarAverage(std::vector<double> componentValues) const;
    void clearDerivatives();
    void recalculateDerivatives();
};

struct GasElement {
    double n;
    std::vector<double> ns;
    double physH;
    GasElement() = default;
    GasElement(size_t size);
    GasElement(const std::vector<double>& ns, double physH);
    // Arithmetic helpers
    size_t size() const;
    GasElement& recalculateN();
    GasElement& zero();
    GasElement& operator+=(GasElement& other);
    GasElement& operator-=(GasElement& other);
    GasElement& operator*=(double multiplier);
    GasElement& operator/=(double denominator);
};

// For interoperation, data is represented in double array buffers.
// These wrappers provide views into these double arrays.

struct GasStateView {
    const size_t size;
    const std::unique_ptr<double[]> underlier;
    double& P;
    double& T;
    double& V_;
    double& averageMolarMass;
    double& density;
    double& physH_;
    double& chemH_;
    double& totalH_;
    double* const xs;
    GasStateView(size_t componentCount);
    GasStateView& operator=(const GasState& state);

   private:
    GasStateView(size_t size, std::unique_ptr<double[]>&& underlier);
};

struct GasElementView {
    const size_t size;
    const std::unique_ptr<double[]> underlier;
    double& n;
    double& physH;
    double* const ns;
    GasElementView(size_t componentCount);
    GasElementView& operator=(const GasElement& element);

   private:
    GasElementView(size_t size, std::unique_ptr<double[]>&& underlier);
};