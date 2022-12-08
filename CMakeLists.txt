cmake_minimum_required(VERSION 3.20)
project("nc-tools" VERSION "0.1.1" LANGUAGES CXX)

if(${PROJECT_SOURCE_DIR} EQUAL ${PROJECT_BINARY_DIR})
    message(FATAL_ERROR "Don't be a fool, out-of-source build with your tool.")
endif()

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(WARNING_FLAGS -Werror -Wall -Wextra -Wfatal-errors -Wno-cast-function-type)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(WARNING_FLAGS /W3 /WX)
endif()

option(NC_TOOLS_BUILD_IMPORTER_LIB "Build NcAsset" ON)
option(NC_TOOLS_BUILD_CONVERTER "Build nc-convert executable" ON)
option(NC_TOOLS_BUILD_TESTS "Include tests in build" ON)

add_definitions(-DNC_ASSERT_ENABLED)

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

include(FetchContent)

FetchContent_Declare(NcCommon
                     GIT_REPOSITORY https://github.com/NcStudios/NcCommon.git
                     GIT_TAG        39ff59b4027854ad236a046446ecfbb5d91aec5f
)

FetchContent_Declare(assimp
                     GIT_REPOSITORY https://github.com/NcStudios/assimp.git
                     GIT_TAG        origin/master
)

FetchContent_MakeAvailable(NcCommon assimp)

# nc-tools V1
# TODO: We still need to build the old version until v2
# covers all asset types and can be consumed properly.
add_executable(nc-tools)

configure_file(${PROJECT_SOURCE_DIR}/source/Version.h.in
               ${PROJECT_SOURCE_DIR}/source/Version.h
)

set_target_properties(nc-tools
    PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}
        RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}
)

target_compile_options(nc-tools
    PRIVATE
        ${WARNING_FLAGS}
)

target_include_directories(nc-tools
    PRIVATE
        ${PROJECT_SOURCE_DIR}/source
)

target_sources(nc-tools
    PRIVATE
        ${PROJECT_SOURCE_DIR}/source/NcTools_v1.cpp
)

target_link_libraries(nc-tools
    PRIVATE
        assimp::assimp
)

add_subdirectory(source)

if(NC_TOOLS_BUILD_TESTS)
    add_subdirectory(test)
endif()
