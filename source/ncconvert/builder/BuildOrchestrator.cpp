#include "BuildOrchestrator.h"
#include "Builder.h"
#include "BuildInstructions.h"
#include "Target.h"
#include "ncasset/AssetTypes.h"
#include "utility/EnumConversion.h"

#include "ncutility/NcError.h"

#include <fstream>
#include <iostream>

namespace
{
constexpr auto assetTypes = std::array<nc::asset::AssetType, 4>{
    nc::asset::AssetType::Cubemap,
    nc::asset::AssetType::ConcaveCollider,
    nc::asset::AssetType::HullCollider,
    nc::asset::AssetType::Mesh
};
}

namespace nc::convert
{
BuildOrchestrator::BuildOrchestrator(Config config)
    : m_config{std::move(config)},
      m_builder{std::make_unique<Builder>()}
{
}

BuildOrchestrator::~BuildOrchestrator() = default;

void BuildOrchestrator::RunBuild()
{
    if(!std::filesystem::exists(m_config.outputDirectory))
    {
        std::cout << "Creating directory: " << m_config.outputDirectory << '\n';
        if(!std::filesystem::create_directories(m_config.outputDirectory))
        {
            throw NcError("Failed to create output directory: ", m_config.outputDirectory.string());
        }
    }

    const auto instructions = BuildInstructions{m_config};

    for (auto type : assetTypes)
    {
        for (const auto& target : instructions.GetTargetsForType(type))
        {
            std::cout << "Building asset: " << target.destinationPath << '\n';

            if (!m_builder->Build(type, target))
            {
                std::cerr << "Failed building: " << target.destinationPath << '\n';

            }
        }
    }
}
} // namespace nc::convert
