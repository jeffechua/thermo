#include "jsonFactories.hpp"

#include <iostream>
#include <stdexcept>

#include "gas.hpp"
#include "gasEdges.hpp"
#include "gasNodes.hpp"
#include "json.hpp"

using namespace std;

namespace factories {

bool contains_all(const nlohmann::json& js, const vector<string>& keys) {
    for (int i = 0; i < keys.size(); i++)
        if (!js.contains(keys[i])) return false;
    return true;
}

unique_ptr<GasSpecies> deserializeGasSpecies(const nlohmann::json& js) {
    if (contains_all(js, {"name", "molarMass", "chemH_"})) {
        return unique_ptr<GasSpecies>(
            new GasSpecies(js["name"], js["molarMass"], js["chemH_"]));
    } else {
        cout << "Pattern not found: GasSpecies" << js["name"] << ".";
        return unique_ptr<GasSpecies>();
    }
}

unique_ptr<GasBasis> deserializeGasBasis(
    const nlohmann::json& js,
    const map<string, unique_ptr<GasSpecies>>& speciesMap) {
    if (contains_all(js, {"name", "components", "gamma"})) {
        auto componentsData = js["components"];
        vector<GasSpecies*> componentsPointers(componentsData.size());
        transform(componentsData.begin(), componentsData.end(),
                  componentsPointers.begin(),
                  [&speciesMap](string speciesName) {
                      return speciesMap.at(speciesName).get();
                  });
        return unique_ptr<GasBasis>(
            new GasBasis(js["name"], js["gamma"], componentsPointers));
    } else {
        cout << "Pattern not found: GasBasis" << js["name"] << ".";
        return unique_ptr<GasBasis>();
    }
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
    } else {
        cout << "GasNode type not found: " << js["type"] << " " << js["name"]
             << ".";
        return unique_ptr<GasNode>();
    }
}

unique_ptr<GasNode> deserializeGasSourceSink(
    const nlohmann::json& js,
    const map<string, unique_ptr<GasBasis>>& basisMap) {
    if (contains_all(js, {"name", "basis", "P", "T", "xs"})) {
        return unique_ptr<GasNode>(new GasSourceSink(
            js["name"],
            GasState(*(basisMap.at(js["basis"])), js["P"], js["T"], js["xs"])));
    } else {
        cout << "Pattern not found: IsochoricCell " << js["name"] << ".";
        return unique_ptr<GasNode>();
    }
}

// {name, basis, V, xs, P, T}
// {name, basis, V, ns, physH}
unique_ptr<GasNode> deserializeIsochoricCell(
    const nlohmann::json& js,
    const map<string, unique_ptr<GasBasis>>& basisMap) {
    if (contains_all(js, {"name", "basis"})) {
        if (contains_all(js, {"P", "V", "T", "xs"}))
            return unique_ptr<GasNode>(
                new IsochoricCell(js["name"], *(basisMap.at(js["basis"])),
                                  js["P"], js["V"], js["T"], js["xs"]));
        else if (contains_all(js, {"V", "ns", "physH"}))
            return unique_ptr<GasNode>(
                new IsochoricCell(js["name"], *(basisMap.at(js["basis"])),
                                  js["V"], GasElement(js["ns"], js["physH"])));
    }
    cout << "Pattern not found: IsochoricCell " << js["name"] << ".";
    return unique_ptr<GasNode>();
}

// {name, basis, P, xs, V, T}
// {name, basis, P, ns, T}
// {name, basis, P, ns, physH}
unique_ptr<GasNode> deserializeIsobaricCell(
    const nlohmann::json& js,
    const map<string, unique_ptr<GasBasis>>& basisMap) {
    if (contains_all(js, {"name", "basis"})) {
        if (contains_all(js, {"P", "V", "T", "xs"}))
            return unique_ptr<GasNode>(
                new IsobaricCell(js["name"], *(basisMap.at(js["basis"])),
                                 js["P"], js["V"], js["T"], js["xs"]));
        else if (contains_all(js, {"P", "T", "ns"}))
            return unique_ptr<GasNode>(
                new IsobaricCell(js["name"], *(basisMap.at(js["basis"])),
                                 js["P"], js["T"], js["ns"]));
        else if (contains_all(js, {"P", "ns", "physH"}))
            return unique_ptr<GasNode>(
                new IsobaricCell(js["name"], *(basisMap.at(js["basis"])),
                                 js["P"], GasElement(js["ns"], js["physH"])));
    }
    cout << "Pattern not found: IsobaricCell " << js["name"] << ".";
    return unique_ptr<GasNode>();
}

unique_ptr<GasEdge> deserializeGasEdge(
    const nlohmann::json& js, const map<string, unique_ptr<GasNode>>& nodeMap) {
    if (js["type"] == "VelocityHeadLossPipe") {
        return deserializeVelocityHeadLossPipe(js, nodeMap);
    } else {
        cout << "GasEdge type not found: " << js["type"] << " " << js["name"]
             << ".";
        return unique_ptr<GasEdge>();
    }
}

unique_ptr<GasEdge> deserializeVelocityHeadLossPipe(
    const nlohmann::json& js, const map<string, unique_ptr<GasNode>>& nodeMap) {
    if (contains_all(js, {"name", "start", "end", "diameter", "headsLost"})) {
        return unique_ptr<GasEdge>(new VelocityHeadLossPipe(
            js["name"], *(nodeMap.at(js["start"])), *(nodeMap.at(js["end"])),
            js["diameter"], js["headsLost"]));
    } else {
        cout << "Pattern not found: VelocityHeadLossPipe " << js["name"] << ".";
        return unique_ptr<GasEdge>();
    }
}

}  // namespace factories
