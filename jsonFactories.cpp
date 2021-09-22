#include "jsonFactories.hpp"

#include <stdexcept>

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

unique_ptr<GasBasis> deserializeGasBasis(
    const nlohmann::json& js,
    const map<string, unique_ptr<GasSpecies>>& speciesMap) {
    auto componentsData = js["components"];
    vector<GasSpecies*> componentsPointers(componentsData.size());
    transform(componentsData.begin(), componentsData.end(),
              componentsPointers.begin(), [&speciesMap](string speciesName) {
                  return speciesMap.at(speciesName).get();
              });
    return unique_ptr<GasBasis>(
        new GasBasis(js["name"], js["gamma"], componentsPointers));
}

unique_ptr<GasNode> deserializeGasNode(
    const nlohmann::json& js,
    const map<string, unique_ptr<GasBasis>>& basisMap) {
    if (js["type"] == "SourceSink") {
        return deserializeGasSourceSink(js, basisMap);
    } else if (js["type"] == "IsochoricCell") {
        return deserializeIsochoricCell(js, basisMap);
    } else if (js["type"] == "IsobaricCell") {
        return deserializeIsobaricCell(js, basisMap);
    }
    throw std::invalid_argument("Invalid gas node type.");
}

unique_ptr<GasNode> deserializeGasSourceSink(
    const nlohmann::json& js,
    const map<string, unique_ptr<GasBasis>>& basisMap) {
    return unique_ptr<GasNode>(new GasSourceSink(
        js["name"],
        GasState(*(basisMap.at(js["basis"])), js["P"], js["T"], js["xs"])));
}

// {name, basis, V, xs, P, T}
// {name, basis, V, ns, physH}
unique_ptr<GasNode> deserializeIsochoricCell(
    const nlohmann::json& js,
    const map<string, unique_ptr<GasBasis>>& basisMap) {
    if (js.contains("xs"))
        return unique_ptr<GasNode>(
            new IsochoricCell(js["name"], *(basisMap.at(js["basis"])), js["P"],
                              js["V"], js["T"], js["xs"]));
    else
        return unique_ptr<GasNode>(
            new IsochoricCell(js["name"], *(basisMap.at(js["basis"])), js["V"],
                              GasElement(js["ns"], js["physH"])));
}

// {name, basis, P, xs, V, T}
// {name, basis, P, ns, T}
// {name, basis, P, ns, physH}
unique_ptr<GasNode> deserializeIsobaricCell(
    const nlohmann::json& js,
    const map<string, unique_ptr<GasBasis>>& basisMap) {
    if (js.contains("xs"))
        return unique_ptr<GasNode>(
            new IsobaricCell(js["name"], *(basisMap.at(js["basis"])), js["V"],
                             js["xs"], js["P"], js["T"]));
    else if (js.contains("T"))
        return unique_ptr<GasNode>(
            new IsobaricCell(js["name"], *(basisMap.at(js["basis"])), js["P"],
                             js["T"], js["ns"]));
    else
        return unique_ptr<GasNode>(
            new IsobaricCell(js["name"], *(basisMap.at(js["basis"])), js["P"],
                             GasElement(js["ns"], js["physH"])));
}

unique_ptr<GasEdge> deserializeGasEdge(
    const nlohmann::json& js, const map<string, unique_ptr<GasNode>>& nodeMap) {
    if (js["type"] == "VelocityHeadLossPipe") {
        return deserializeVelocityHeadLossPipe(js, nodeMap);
    }
    throw std::invalid_argument("Invalid gas node type.");
}

unique_ptr<GasEdge> deserializeVelocityHeadLossPipe(
    const nlohmann::json& js, const map<string, unique_ptr<GasNode>>& nodeMap) {
    return unique_ptr<GasEdge>(new VelocityHeadLossPipe(
        js["name"], *(nodeMap.at(js["start"])), *(nodeMap.at(js["end"])),
        js["diameter"], js["headsLost"]));
}

}  // namespace factories
