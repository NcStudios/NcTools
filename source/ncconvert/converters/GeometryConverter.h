#pragma once

#include "ncasset/AssetsFwd.h"

#include <filesystem>
#include <memory>

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

        /** Proecess an fbx file as geometry for a mesh renderer. */
        auto ImportMesh(const std::filesystem::path& path) -> asset::Mesh;

    private:
        class impl;
        std::unique_ptr<impl> m_impl;
};
} // namespace nc::convert
