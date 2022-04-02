#pragma once

#include "Common.h"

/**
 * NcEngine Asset File
 * Version: str
 * Count: int
 * 
 * id(path): byte offset
 * ...
 * 
 * subheader
 * data
 * ...
 */

class NcaFile
{
    public:
        NcaFile(const std::filesystem::path& outPath);

        void AddMesh(aiMesh* mesh);
        void AddConcaveCollider(aiMesh* mesh);
        void AddConvexHull(aiMesh* mesh);
    
    private:
        std::ofstream m_file;
        std::filesystem::path m_ncaPath;
        AssetType m_type;
};