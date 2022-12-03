target_sources(nc-tools-2
    PRIVATE
        Builders.cpp
        Main.cpp
        MeshAnalysis.cpp
        NcaFile.cpp
        Serialize.cpp
        Validation.cpp
)

if(NC_TOOLS_BUILD_IMPORTER_LIB)
    add_subdirectory(ncasset)
endif()

# if(NC_TOOLS_BUILD_EXPORTER_CLI)
#     add_subdirectory(nctools)
# endif()
