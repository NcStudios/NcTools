add_executable(nc-convert)

set_target_properties(nc-convert
    PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}
        RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}
)

target_compile_options(nc-convert
    PRIVATE
        ${WARNING_FLAGS}
)

target_include_directories(nc-convert
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/source
        ${PROJECT_SOURCE_DIR}/source/ncconvert
)

set(nc-convert-common-source ${PROJECT_SOURCE_DIR}/source/common/NcaHeader.cpp
                             ${PROJECT_SOURCE_DIR}/source/common/Serialize.cpp
)

target_sources(nc-convert
    PRIVATE
        ${PROJECT_SOURCE_DIR}/source/ncconvert/Builder.cpp
        ${PROJECT_SOURCE_DIR}/source/ncconvert/Main.cpp
        ${nc-convert-common-source}
)

target_link_libraries(nc-convert
    PRIVATE
        NcMath
        assimp::assimp
)

add_subdirectory(analysis)
add_subdirectory(converters)