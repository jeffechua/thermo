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
    virtual ~GasNode() = default;
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
    // V inference for isobaric makes sense; P inference for isochoric does not.
    // IsochoricCell(const std::string& name, GasBasis& basis, double V,
    //               const std::vector<double>& ns, double T);
    IsochoricCell(const std::string& name, GasBasis& basis, double P, double V,
                  double T, const std::vector<double>& xs);

   protected:
    void recalculateState() override;
};

class IsobaricCell : public GasNode {
   public:
    double P;
    IsobaricCell(const std::string&, GasBasis& basis, double P,
                 GasElement contents);
    IsobaricCell(const std::string&, GasBasis& basis, double P, double T,
                 const std::vector<double>& ns);
    IsobaricCell(const std::string&, GasBasis& basis, double P, double V,
                 double T, const std::vector<double>& xs);

   protected:
    void recalculateState() override;
};