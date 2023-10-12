set(NC_CONVERT_TEST_COLLATERAL_DIR ${PROJECT_SOURCE_DIR}/test/collateral)

## Build and Import Tests ###
if(NC_TOOLS_BUILD_CONVERTER)
    add_executable(BuildAndImport_integration_tests
        BuildAndImport_integration_tests.cpp
    )

    target_compile_options(BuildAndImport_integration_tests
        PUBLIC
            ${NC_TOOLS_COMPILE_OPTIONS}
    )

    target_compile_definitions(BuildAndImport_integration_tests
        PRIVATE
            TEST_COLLATERAL_DIR="${NC_CONVERT_TEST_COLLATERAL_DIR}"
    )

    target_include_directories(BuildAndImport_integration_tests
        PRIVATE
            ${PROJECT_SOURCE_DIR}/test/collateral
            ${PROJECT_SOURCE_DIR}/test/common
            ${PROJECT_SOURCE_DIR}/include
            ${PROJECT_SOURCE_DIR}/include/ncasset
            ${PROJECT_SOURCE_DIR}/source
            ${PROJECT_SOURCE_DIR}/source/common
            ${PROJECT_SOURCE_DIR}/source/ncconvert
            ${PROJECT_SOURCE_DIR}/source/ncconvert/analysis
            ${PROJECT_SOURCE_DIR}/source/ncconvert/builder
            ${PROJECT_SOURCE_DIR}/source/ncconvert/converters
            ${PROJECT_SOURCE_DIR}/source/ncconvert/utility
    )

    target_include_directories(BuildAndImport_integration_tests
        SYSTEM PRIVATE
            ${PROJECT_SOURCE_DIR}/source/external
    )

    target_sources(BuildAndImport_integration_tests
        PRIVATE
            ${PROJECT_SOURCE_DIR}/source/ncasset/BonesReader.cpp
            ${PROJECT_SOURCE_DIR}/source/ncasset/Deserialize.cpp
            ${PROJECT_SOURCE_DIR}/source/ncasset/Import.cpp
            ${PROJECT_SOURCE_DIR}/source/ncasset/RawNcaBuffer.cpp
            ${PROJECT_SOURCE_DIR}/source/ncconvert/analysis/GeometryAnalysis.cpp
            ${PROJECT_SOURCE_DIR}/source/ncconvert/analysis/Sanitize.cpp
            ${PROJECT_SOURCE_DIR}/source/ncconvert/analysis/TextureAnalysis.cpp
            ${PROJECT_SOURCE_DIR}/source/ncconvert/builder/BonesWriter.cpp
            ${PROJECT_SOURCE_DIR}/source/ncconvert/builder/Builder.cpp
            ${PROJECT_SOURCE_DIR}/source/ncconvert/builder/Serialize.cpp
            ${PROJECT_SOURCE_DIR}/source/ncconvert/builder/SkeletalAnimationWriter.cpp
            ${PROJECT_SOURCE_DIR}/source/ncconvert/converters/AudioConverter.cpp
            ${PROJECT_SOURCE_DIR}/source/ncconvert/converters/GeometryConverter.cpp
            ${PROJECT_SOURCE_DIR}/source/ncconvert/converters/TextureConverter.cpp
            ${PROJECT_SOURCE_DIR}/source/ncconvert/utility/BlobSize.cpp
            ${PROJECT_SOURCE_DIR}/source/ncconvert/utility/EnumConversion.cpp
            ${PROJECT_SOURCE_DIR}/source/ncconvert/utility/Path.cpp
    )

    target_link_libraries(BuildAndImport_integration_tests
        PRIVATE
            gtest_main
            NcMath
            assimp::assimp
    )

    add_test(BuildAndImport_integration_tests BuildAndImport_integration_tests)
endif()

## NcConvert Integration Tests ##
if(NC_TOOLS_BUILD_CONVERTER)
    add_executable(NcConvert_integration_tests
        NcConvert_integration_tests.cpp
    )

    add_dependencies(NcConvert_integration_tests nc-convert)

    target_compile_options(NcConvert_integration_tests
        PUBLIC
            ${NC_TOOLS_COMPILE_OPTIONS}
    )

    target_compile_definitions(NcConvert_integration_tests
        PRIVATE
            TEST_COLLATERAL_DIR="${NC_CONVERT_TEST_COLLATERAL_DIR}"
            NC_CONVERT_EXECUTABLE_PATH="$<TARGET_FILE:nc-convert>"
    )

    target_include_directories(NcConvert_integration_tests
        PRIVATE
            ${PROJECT_SOURCE_DIR}/test/collateral
            ${PROJECT_SOURCE_DIR}/source/ncconvert
    )

    target_include_directories(NcConvert_integration_tests
        SYSTEM PRIVATE
            ${PROJECT_SOURCE_DIR}/source/external
    )

    target_link_libraries(NcConvert_integration_tests
        PRIVATE
            gtest_main
            NcUtility
    )

    add_test(NcConvert_integration_tests NcConvert_integration_tests)
endif()
### Serialization Tests ###
add_executable(Serialize_integration_tests
    Serialize_integration_tests.cpp
)

target_compile_options(Serialize_integration_tests
    PUBLIC
        ${NC_TOOLS_COMPILE_OPTIONS}
)

target_include_directories(Serialize_integration_tests
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/source/ncasset
        ${PROJECT_SOURCE_DIR}/source/common
        ${PROJECT_SOURCE_DIR}/source/ncconvert
        ${PROJECT_SOURCE_DIR}/source/ncconvert/builder
        ${PROJECT_SOURCE_DIR}/source/ncconvert/utility
)

target_sources(Serialize_integration_tests
    PRIVATE
        ${PROJECT_SOURCE_DIR}/source/ncasset/BonesReader.cpp
        ${PROJECT_SOURCE_DIR}/source/ncasset/Deserialize.cpp
        ${PROJECT_SOURCE_DIR}/source/ncasset/RawNcaBuffer.cpp
        ${PROJECT_SOURCE_DIR}/source/ncconvert/builder/BonesWriter.cpp
        ${PROJECT_SOURCE_DIR}/source/ncconvert/builder/Serialize.cpp
        ${PROJECT_SOURCE_DIR}/source/ncconvert/builder/SkeletalAnimationWriter.cpp
        ${PROJECT_SOURCE_DIR}/source/ncconvert/utility/BlobSize.cpp
)

target_link_libraries(Serialize_integration_tests
    PRIVATE
        gtest_main
        NcMath
)

add_test(Serialize_integration_tests Serialize_integration_tests)
