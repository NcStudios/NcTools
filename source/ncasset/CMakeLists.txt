add_library(NcAsset STATIC)

target_compile_options(NcAsset
    PRIVATE
        ${NC_TOOLS_COMPILE_OPTIONS}
)

target_include_directories(NcAsset
    PUBLIC
        $<INSTALL_INTERFACE:include/>
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    PRIVATE
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/ncasset>
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/source/ncasset>
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/source>
)

set(NcAsset-common-source ${PROJECT_SOURCE_DIR}/source/common/Deserialize.cpp
                          ${PROJECT_SOURCE_DIR}/source/common/NcaHeader.cpp
                          ${PROJECT_SOURCE_DIR}/source/common/RawNcaBuffer.cpp
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

install(
    DIRECTORY
        ${PROJECT_SOURCE_DIR}/include/ncasset
    DESTINATION
        include
)
