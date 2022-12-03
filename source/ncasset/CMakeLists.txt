add_library(NcAsset STATIC)

set_target_properties(NcAsset
    PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}
        RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}
)

target_compile_options(NcAsset
    PRIVATE
        ${WARNING_FLAGS}
)

# maybe disable in release
target_compile_definitions(NcAsset
    PRIVATE
        NC_ASSERT_ENABLED
)

target_include_directories(NcAsset
    PUBLIC
        $<INSTALL_INTERFACE:include/>
    PRIVATE
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/ncasset>
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/source/ncasset>
)

target_sources(NcAsset
    PRIVATE
        ${PROJECT_SOURCE_DIR}/source/ncasset/Deserialize.cpp
        ${PROJECT_SOURCE_DIR}/source/ncasset/Import.cpp
        ${PROJECT_SOURCE_DIR}/source/ncasset/NcaHeader.cpp
        ${PROJECT_SOURCE_DIR}/source/ncasset/RawNcaBuffer.cpp
        ${PROJECT_SOURCE_DIR}/source/ncasset/Serialize.cpp
)

target_link_libraries(NcAsset
    PUBLIC
        NcMath
        NcUtility
)

install(DIRECTORY   ${PROJECT_SOURCE_DIR}/include/ncasset
        DESTINATION include
)
