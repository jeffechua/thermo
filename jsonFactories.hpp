#pragma once
#include "gas.hpp"
#include "gasEdges.hpp"
#include "gasNodes.hpp"
#include "json.hpp"

namespace factories {

std::unique_ptr<GasSpecies> deserializeGasSpecies(const nlohmann::json& js);
std::unique_ptr<GasBasis> deserializeGasBasis(
    const nlohmann::json& js,
    const std::map<std::string, std::unique_ptr<GasSpecies>>& speciesMap);

std::unique_ptr<GasNode> deserializeGasNode(
    const nlohmann::json& js,
    const std::map<std::string, std::unique_ptr<GasBasis>>& basisMap);
std::unique_ptr<GasNode> deserializeGasSourceSink(
    const nlohmann::json& js,
    const std::map<std::string, std::unique_ptr<GasBasis>>& basisMap);
std::unique_ptr<GasNode> deserializeIsochoricCell(
    const nlohmann::json& js,
    const std::map<std::string, std::unique_ptr<GasBasis>>& basisMap);
std::unique_ptr<GasNode> deserializeIsobaricCell(
    const nlohmann::json& js,
    const std::map<std::string, std::unique_ptr<GasBasis>>& basisMap);

std::unique_ptr<GasEdge> deserializeGasEdge(
    const nlohmann::json& js,
    const std::map<std::string, std::unique_ptr<GasNode>>& nodeMap);
std::unique_ptr<GasEdge> deserializeVelocityHeadLossPipe(
    const nlohmann::json& js,
    const std::map<std::string, std::unique_ptr<GasNode>>& nodeMap);

}  // namespace factories