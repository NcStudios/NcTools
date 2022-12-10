#pragma once

#include <filesystem>
#include <memory>

namespace nc::asset
{
struct ConcaveCollider;
struct HullCollider;
struct Mesh;
}

namespace nc::convert
{
class FbxConverter
{
    public:
        FbxConverter();
        ~FbxConverter() noexcept;

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
