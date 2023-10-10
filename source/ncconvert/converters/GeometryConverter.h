#pragma once

#include "ncasset/AssetsFwd.h"

#include <filesystem>
#include <memory>
#include <vector>

namespace nc::convert
{
class GeometryConverter
{
    public:
        GeometryConverter();
        ~GeometryConverter() noexcept;

        /** Process an fbx file as geometry for a concave collider. */
        auto ImportConcaveCollider(const std::filesystem::path& path) -> asset::ConcaveCollider;

        /** Process an fbx file as geometry for a hull collider. */
        auto ImportHullCollider(const std::filesystem::path& path) -> asset::HullCollider;

        /** Process an fbx file as geometry for a mesh renderer. */
        auto ImportMesh(const std::filesystem::path& path) -> asset::Mesh;

        /** Process an fbx file into skeletal animation clip(s). */
        auto ImportSkeletalAnimations(const std::filesystem::path& path) -> std::vector<asset::SkeletalAnimationClip>;

    private:
        class impl;
        std::unique_ptr<impl> m_impl;
};
} // namespace nc::convert
