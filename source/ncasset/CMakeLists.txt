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

target_include_directories(NcAsset
    PUBLIC
        $<INSTALL_INTERFACE:include/>
    PRIVATE
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/ncasset>
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/source/ncasset>
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/source>
)

set(NcAsset-common-source ${PROJECT_SOURCE_DIR}/source/common/NcaHeader.cpp
                          ${PROJECT_SOURCE_DIR}/source/common/RawNcaBuffer.cpp
                          ${PROJECT_SOURCE_DIR}/source/common/Serialize.cpp
)

target_sources(NcAsset
    PRIVATE
        ${NcAsset-common-source}
        ${PROJECT_SOURCE_DIR}/source/ncasset/Import.cpp
)

target_link_libraries(NcAsset
    PUBLIC
        NcMath
        NcUtility
)

install(DIRECTORY   ${PROJECT_SOURCE_DIR}/include/ncasset
        DESTINATION include
)
