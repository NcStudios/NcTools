#pragma once

#include "ncasset/AssetsFwd.h"

#include <filesystem>
#include <memory>
#include <optional>
#include <vector>

namespace nc::convert
{
class GeometryConverter
{
    public:
        GeometryConverter();
        ~GeometryConverter() noexcept;

        /** Process an Fbx file as geometry for a concave collider. */
        auto ImportConcaveCollider(const std::filesystem::path& path) -> asset::ConcaveCollider;

        /** Process an fbx file as geometry for a hull collider. */
        auto ImportHullCollider(const std::filesystem::path& path) -> asset::HullCollider;

        /** Process an fbx file as geometry for a mesh renderer. Supply an internalName of the mesh to extract if there are multiple meshes in the fbx file. */
        auto ImportMesh(const std::filesystem::path& path, const std::optional<std::string>& internalName = std::nullopt) -> asset::Mesh;

    private:
        class impl;
        std::unique_ptr<impl> m_impl;
};
} // namespace nc::convert
