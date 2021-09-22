#pragma once
#include "gas.hpp"
#include "gasEdges.hpp"
#include "gasNodes.hpp"
#include "json.hpp"

namespace factories {

std::unique_ptr<GasSpecies> deserializeGasSpecies(const nlohmann::json& str);
std::unique_ptr<GasBasis> deserializeGasBasis(
    const nlohmann::json& str,
    std::map<std::string, std::unique_ptr<GasSpecies>>& speciesMap);
/*
std::unique_ptr<GasNode> deserializeGasNode(const nlohmann::json& str);
std::unique_ptr<GasSourceSink> deserializeGasSourceSink(const nlohmann::json&
str); std::unique_ptr<IsochoricCell> deserializeIsochoricCell(const
nlohmann::json& str); std::unique_ptr<IsobaricCell>
deserializeIsobaricCell(const nlohmann::json& str);

std::unique_ptr<GasEdge> deserializeGasEdge(const nlohmann::json& str);
std::unique_ptr<VelocityHeadLossPipe> deserializeVelocityHeadLossPipe(const
nlohmann::json& str);
*/
}  // namespace factories