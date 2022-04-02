#include "Builders.h"
#include "NcaFile.h"
#include "Serialize.h"
#include "Validation.h"

namespace
{
    bool IsValidMeshExtension(const std::filesystem::path& extension) 
    {
        return extension.compare(".fbx") == 0 || extension.compare(".obj") == 0;
    }

    auto ReadScene(Assimp::Importer* importer, const std::filesystem::path& inPath, unsigned flags) -> const aiScene*
    {
        auto scene = importer->ReadFile(inPath.string(), flags);
        if(!scene)
            throw std::runtime_error("AssImp failure");

        return scene;
    }

    auto Import(Assimp::Importer* importer, const std::filesystem::path& inPath, unsigned flags)
    {
        auto* mesh = ReadScene(importer, inPath, flags)->mMeshes[0];
        if(!mesh->mNormals || !mesh->mTextureCoords || !mesh->mTangents || !mesh->mBitangents)
        {
            throw std::runtime_error("File does not contain all required data");
        }

        if(SanitizeMesh(mesh))
        {
            std::cerr << "    Warning: Bad value detected in mesh data. Some values have been set to 0.\n";
        }

        return mesh;
    }

    auto ImportWithoutAttributes(Assimp::Importer* importer, const std::filesystem::path& inPath, unsigned flags)
    {
        auto* mesh = ReadScene(importer, inPath, flags)->mMeshes[0];
        const auto count = mesh->mNumVertices;
        if(SanitizeVectorArray(mesh->mVertices, count))
        {
            std::cerr << "    Warning: Bad value detected in mesh data. Some values have been set to 0.\n";
        }

        return mesh;
    }

    void BuildMeshAsset(Assimp::Importer* importer, const std::filesystem::path& inPath, const std::filesystem::path& outPath)
    {
        std::cout << "Creating Mesh: " << outPath << '\n';

        if (!IsValidMeshExtension(inPath.extension()))
            throw std::runtime_error("Invalid mesh file extension: " + inPath.string());

        auto* mesh = Import(importer, inPath, MeshFlags);
        auto nca = NcaFile{outPath};
        nca.AddMesh(mesh);
    }

    void BuildConvexHullAsset(Assimp::Importer* importer, const std::filesystem::path& inPath, const std::filesystem::path& outPath)
    {
        std::cout << "Creating Hull Collider: " << outPath << '\n';

        if (!IsValidMeshExtension(inPath.extension()))
            throw std::runtime_error("Invalid hull file extension: " + inPath.string());

        const auto mesh = ImportWithoutAttributes(importer, inPath, ConvexHullFlags);
        auto nca = NcaFile{outPath};
        nca.AddConvexHull(mesh);
    }

    void BuildConcaveColliderAsset(Assimp::Importer* importer, const std::filesystem::path& inPath, const std::filesystem::path& outPath)
    {
        std::cout << "Creating Concave Collider: " << outPath << '\n';

        if (!IsValidMeshExtension(inPath.extension()))
            throw std::runtime_error("Invalid mesh file extension: " + inPath.string());

        const auto mesh = ImportWithoutAttributes(importer, inPath, ConcaveColliderFlags);
        auto nca = NcaFile{outPath};
        nca.AddConcaveCollider(mesh);
    }

    // void BuildSkyboxAsset(const std::filesystem::path& inPath, const Config& config)
    // {
    //     if (!std::filesystem::is_directory(inPath))
    //     {
    //         throw std::runtime_error("Must specify a directory.");
    //     }

    //     const auto assetPath = ToAssetPath(inPath, config);
    //     std::ofstream outFile{assetPath};
    //     if(!outFile)
    //         throw std::runtime_error("Failure opening asset file");

    //     struct Entry
    //     {
    //         std::string face;
    //         bool found = false;
    //     };

    //     std::array<Entry, 6> skyboxFacePaths
    //     {
    //         Entry{"front"},
    //         Entry{"back"},
    //         Entry{"up"},
    //         Entry{"down"},
    //         Entry{"right"},
    //         Entry{"left"}
    //     };

    //     for (const auto& facePathIt : std::filesystem::directory_iterator{inPath})
    //     {
    //         const auto& facePath = facePathIt.path();
    //         auto facePathName = facePath.stem().string();
    //         auto facePathExt = facePath.extension().string();
    //         std::ranges::for_each(facePathName, [](auto& character){character = std::tolower(character);});

    //         auto pos = std::ranges::find_if(skyboxFacePaths, [facePathName](const auto& entry)
    //         {
    //             return entry.face == facePathName;
    //         });

    //         if (pos == skyboxFacePaths.end())
    //         {
    //             continue;
    //         }

    //         pos->found = true;
    //         auto outputDir = config.OutputDirectory / facePathName;
    //         outputDir.replace_extension(facePathExt);
    //         auto outputDirStr = outputDir.string();
    //         try
    //         {
    //             std::filesystem::copy_file(facePath, outputDir, std::filesystem::copy_options::overwrite_existing);
    //         }
    //         catch(const std::exception&)
    //         {
    //         }

    //         outputDirStr.erase(std::remove(outputDirStr.begin(), outputDirStr.end(), '\"'), outputDirStr.end());
    //         outFile << facePathName << facePathExt << '\n';
    //     }

    //     if (std::any_of(skyboxFacePaths.begin(), skyboxFacePaths.end(), [](const auto& entry) { return entry.found == false; }))
    //     {
    //         throw std::runtime_error("One or more skybox faces did not get created correctly.");
    //     }
    // }
} // end anonymouse namespace

//namespace nc::asset_builder
//{
    void BuildAsset(Assimp::Importer* importer, const Target& target, const std::filesystem::path& outPath)
    {
        switch(target.type)
        {
            case AssetType::Mesh:
            {
                BuildMeshAsset(importer, target.path, outPath);
                break;
            }
            case AssetType::ConvexHull:
            {
                BuildConvexHullAsset(importer, target.path, outPath);
                break;
            }
            case AssetType::ConcaveCollider:
            {
                BuildConcaveColliderAsset(importer, target.path, outPath);
                break;
            }
            case AssetType::Skybox:
            {
                //BuildSkyboxAsset(target.path, config);
                break;
            }
        }
    }
//} // end namespace nc::asset_builder