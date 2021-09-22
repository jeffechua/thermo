#include "jsonFactories.hpp"

#include "gas.hpp"
#include "gasEdges.hpp"
#include "gasNodes.hpp"
#include "json.hpp"

using namespace std;

namespace factories {

unique_ptr<GasSpecies> deserializeGasSpecies(const nlohmann::json& js) {
    return unique_ptr<GasSpecies>(
        new GasSpecies(js["name"], js["molarMass"], js["chemH_"]));
}

unique_ptr<GasBasis> deserializeGasBasis(const nlohmann::json& js,
                                         map<string, unique_ptr<GasSpecies>>& speciesMap) {
    auto componentsData = js["components"];
    vector<GasSpecies*> componentsPointers(componentsData.size());
    transform(componentsData.begin(), componentsData.end(),
              componentsPointers.begin(), [&speciesMap](string speciesName) {
                  return speciesMap[speciesName].get();
              });
    return unique_ptr<GasBasis>(
        new GasBasis(js["name"], js["gamma"], componentsPointers));
}
/*
unique_ptr<GasNode> deserializeGasNode(const string& str);
unique_ptr<GasSourceSink> deserializeGasSourceSink(const string& str);
unique_ptr<IsochoricCell> deserializeIsochoricCell(const string& str);
unique_ptr<IsobaricCell> deserializeIsobaricCell(const string& str);

unique_ptr<GasEdge> deserializeGasEdge(const string& str);
unique_ptr<VelocityHeadLossPipe> deserializeVelocityHeadLossPipe(
    const string& str);
*/
}  // namespace factories
