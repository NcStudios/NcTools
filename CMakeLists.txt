cmake_minimum_required(VERSION 3.20)
project("nc-tools" VERSION "1.0.0" LANGUAGES CXX)

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

FetchContent_MakeAvailable(NcCommon)

add_subdirectory(source)

if(NC_TOOLS_BUILD_TESTS)
    add_subdirectory(test)
endif()
