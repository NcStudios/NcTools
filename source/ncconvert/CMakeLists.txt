# Assimp Options
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ZLIB ON CACHE BOOL "" FORCE)
set(ASSIMP_INSTALL OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "" FORCE)
set(ASSIMP_NO_EXPORT OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ALL_EXPORTERS_BY_DEFAULT OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_FBX_IMPORTER ON CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_OBJ_IMPORTER ON CACHE BOOL "" FORCE)
set(ASSIMP_INSTALL_PDB OFF CACHE BOOL "" FORCE)

FetchContent_Declare(assimp
                     GIT_REPOSITORY https://github.com/NcStudios/assimp.git
                     GIT_TAG        70eddc418e2bae64e78a41cf3cf4e7b95d36bc64 # origin/master
                     GIT_SHALLOW    TRUE
)

FetchContent_MakeAvailable(assimp)

add_executable(nc-convert)

target_compile_options(nc-convert
    PRIVATE
        ${NC_TOOLS_COMPILE_OPTIONS}
)

target_include_directories(nc-convert
    PRIVATE
        ${PROJECT_SOURCE_DIR}/source/ncconvert
)

target_include_directories(nc-convert
    SYSTEM PRIVATE
        ${PROJECT_SOURCE_DIR}/source/external
)

target_sources(nc-convert
    PRIVATE
        ${PROJECT_SOURCE_DIR}/source/ncconvert/Main.cpp
)

target_link_libraries(nc-convert
    PRIVATE
        NcAsset
        NcMath
        NcUtility
        assimp::assimp
)

install(
    TARGETS
        nc-convert
    DESTINATION
        bin
)

add_subdirectory(analysis)
add_subdirectory(builder)
add_subdirectory(converters)
add_subdirectory(utility)
