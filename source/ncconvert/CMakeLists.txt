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

FetchContent_Declare(assimp
                     GIT_REPOSITORY https://github.com/NcStudios/assimp.git
                     GIT_TAG        origin/master
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
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/source
        ${PROJECT_SOURCE_DIR}/source/ncconvert
)

target_include_directories(nc-convert
    SYSTEM PRIVATE
        ${PROJECT_SOURCE_DIR}/source/external
)

set(nc-convert-common-source ${PROJECT_SOURCE_DIR}/source/common/NcaHeader.cpp
                             ${PROJECT_SOURCE_DIR}/source/common/Serialize.cpp
)

target_sources(nc-convert
    PRIVATE
        ${PROJECT_SOURCE_DIR}/source/ncconvert/Main.cpp
        ${nc-convert-common-source}
)

target_link_libraries(nc-convert
    PRIVATE
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
