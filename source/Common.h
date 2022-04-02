#pragma once

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include <array>
#include <algorithm>
#include <cctype>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

const auto AssetExtension = std::string{".nca"};
const auto DefaultAssetTargetFilename = std::string{"targets.txt"};
constexpr auto MeshFlags = aiProcess_Triangulate |
                           aiProcess_JoinIdenticalVertices |
                           aiProcess_ConvertToLeftHanded |
                           aiProcess_GenNormals |
                           aiProcess_CalcTangentSpace;

constexpr auto ConvexHullFlags = aiProcess_Triangulate |
                                 aiProcess_JoinIdenticalVertices |
                                 aiProcess_ConvertToLeftHanded;

constexpr auto ConcaveColliderFlags = aiProcess_Triangulate |
                                      aiProcess_ConvertToLeftHanded;

enum class AssetType
{
    Mesh,
    ConvexHull,
    ConcaveCollider,
    Skybox
};

struct Target
{
    std::filesystem::path path;
    AssetType type;
};

struct NcaHeader
{
    int version;
    AssetType assetType;
};

struct MeshExtents
{
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
    float max = 0.0f;
};