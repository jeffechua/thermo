#pragma once
#include <stddef.h>

#include "gas.hpp"

class GasNode {
   public:
    const std::string name;
    GasState state;
    GasElement contents;
    GasElement dCdt;
    GasNode(const std::string& name, const GasBasis& basis);
    GasNode(const std::string& name, const GasState& state);
    GasNode(const std::string& name, const GasState& state,
            const GasElement& contents);
    void rungeKuttaStage1(double timestep);
    void rungeKuttaStage2(double timestep);

   protected:
    virtual void recalculateState() = 0;

   private:
    GasElement dCdtAlt;
};

class GasSourceSink : public GasNode {
   public:
    GasSourceSink(const std::string& name, GasState state);

   protected:
    void recalculateState() override;
};

class IsochoricCell : public GasNode {
   public:
    double V;
    IsochoricCell(const std::string& name, GasBasis& basis, double V,
                  GasElement contents);
    IsochoricCell(const std::string& name, GasBasis& basis, double V,
                  const std::vector<double>& ns, double T);
    IsochoricCell(const std::string& name, GasBasis& basis, double V,
                  const std::vector<double>& xs, double P, double T);

   protected:
    void recalculateState() override;
};

class IsobaricCell : public GasNode {
   public:
    double P;
    IsobaricCell(const std::string&, GasBasis& basis, double P,
                 GasElement contents);
    IsobaricCell(const std::string&, GasBasis& basis, double P,
                 const std::vector<double>& ns, double T);
    IsobaricCell(const std::string&, GasBasis& basis, double P,
                 const std::vector<double>& xs, double V, double T);

   protected:
    void recalculateState() override;
};