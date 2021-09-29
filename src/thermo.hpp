#pragma once
#include <cmath>

// Thermo is header-only as most functions are small.
// Convention of ordering is HPVT
// Underscore suffixes indicate molar quantities. However, they are omitted in
// function names for clarity; thermo functions are all assumed to be molar.
// Underscore notation is only used for symbols (i.e. H_ but not density_).

// Thermodynamically, only three of molar V, H, T, P and Cp are needed to fully
// specify a pure system. They are related by
//    PV = RT
//     H = Cp(T-T0) + RT ln(P/P0)
// We are assuming ideal gas and modelling all species as the same in a mixture
// for physical calculations. This is essentially asserting they have the same
// (constant) molar Cp.

namespace thermo {

const double R = 8.3145;
const double T0 = 298;
const double P0 = 1e5;

// Ideal gas equation
inline double getPFromVT(double V_, double T) { return R * T / V_; }
inline double getVFromPT(double P, double T) { return R * T / P; }
inline double getTFromPV(double P, double V_) { return V_ * P / R; }

// Ideal gas enthalpy
inline double getHFromPT(double P, double T, double Cp_) {
    return Cp_ * (T - T0) + R * T * log(P / P0);
}
inline double getTFromHP(double H_, double P, double Cp_) {
    return (H_ + Cp_ * T0) / (Cp_ + R * log(P / P0));
}
inline double getTFromHV(double H_, double V_, double Cp_) {
    // H = Cp (T-T0) + RT ln(P/P0)
    //   = Cp (T-T0) + RT ln(RT/VP0)
    //   = Cp (T-T0) + RT ln(RT0/VP0) + RT ln(T/T0)
    // 0 ~ (-H - CpT0) + (R ln(nRT0/VP0) + Cp)T + RT ln(T/T0)
    //   = (-H - CpT0) + (R ln(nRT0/VP0) + Cp)T + RT(T/T0 - 1)    (Taylor)
    //   = (-H - CpT0) + (R ln(nRT0/VP0) + Cp - R)T + RT^2/T0
    const double a = R / T0;
    const double b0 = R * log(R * T0 / V_ / P0) + Cp_;
    const double b = b0 - R;
    const double c = -H_ - Cp_ * T0;
    // first estimate from quadratic Taylor expansion
    double T = (-b + sqrt(b * b - 4 * a * c)) / 2 / a;
    // Then Newton-Raphson some number of times
    const int iterations = 2;
    for (int i = 0; i < iterations; i++) {
        // f' = df/dT = b0 + RT0 + R ln(T/T0)
        const double f = c + b0 * T + R * T * log(T / T0);
        const double fdash = b0 + R * T0 + R * log(T / T0);
        T = T - f / fdash;
    }
    return T;
}

}  // namespace thermo