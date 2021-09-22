#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>

#include "json.hpp"
#include "jsonFactories.hpp"

using namespace std;
using namespace nlohmann;

int main() {
    ifstream stream("sampleProcess.json");
    stringstream buffer;
    buffer << stream.rdbuf();
    json js = json::parse(buffer.str());

    auto speciesData = js["species"];
    map<string, unique_ptr<GasSpecies>> speciesMap;
    for_each(speciesData.begin(), speciesData.end(), [&speciesMap](json js) {
        auto species = factories::deserializeGasSpecies(js);
        speciesMap.emplace(species->name, move(species));
    });

    auto basisData = js["bases"];
    map<string, unique_ptr<GasBasis>> basisMap;
    for_each(basisData.begin(), basisData.end(), [&basisMap, &speciesMap](json js) {
        auto basis = factories::deserializeGasBasis(js, speciesMap);
        basisMap.emplace(basis->name, move(basis));
    });

    cout << basisMap["air"]->components[1]->name;

}