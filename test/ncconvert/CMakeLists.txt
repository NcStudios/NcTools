set(NC_CONVERT_TEST_COLLATERAL_DIR ${CMAKE_CURRENT_LIST_DIR}/../collateral)

## AudioConverter Tests ###
add_executable(AudioConverter_unit_tests
    AudioConverter_unit_tests.cpp
)

target_compile_options(AudioConverter_unit_tests
    PUBLIC
        ${NC_TOOLS_COMPILE_OPTIONS}
)

target_compile_definitions(AudioConverter_unit_tests
    PRIVATE
        TEST_COLLATERAL_DIR="${NC_CONVERT_TEST_COLLATERAL_DIR}"
)

target_include_directories(AudioConverter_unit_tests
    PRIVATE
        ${PROJECT_SOURCE_DIR}/test/collateral
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/source/ncconvert
)

target_include_directories(AudioConverter_unit_tests
    SYSTEM PRIVATE
        ${PROJECT_SOURCE_DIR}/source/external
)

target_sources(AudioConverter_unit_tests
    PRIVATE
        ${PROJECT_SOURCE_DIR}/source/ncconvert/converters/AudioConverter.cpp
        ${PROJECT_SOURCE_DIR}/source/ncconvert/utility/Path.cpp
)

target_link_libraries(AudioConverter_unit_tests
    PRIVATE
        gtest_main
        NcMath
)

add_test(AudioConverter_unit_tests AudioConverter_unit_tests)

## EnumExtensions Tests ###
add_executable(EnumExtensions_unit_tests
    EnumExtensions_unit_tests.cpp
)

target_compile_options(EnumExtensions_unit_tests
    PUBLIC
        ${NC_TOOLS_COMPILE_OPTIONS}
)

target_include_directories(EnumExtensions_unit_tests
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/source/ncconvert
)

target_sources(EnumExtensions_unit_tests
    PRIVATE
        ${PROJECT_SOURCE_DIR}/source/ncconvert/utility/EnumExtensions.cpp
)

target_link_libraries(EnumExtensions_unit_tests
    PRIVATE
        gtest_main
        NcMath
)

add_test(EnumExtensions_unit_tests EnumExtensions_unit_tests)

## GeometryConverter Tests ###
if(NC_TOOLS_BUILD_CONVERTER)
    add_executable(GeometryConverter_unit_tests
        GeometryConverter_unit_tests.cpp
    )

    target_compile_options(GeometryConverter_unit_tests
        PUBLIC
            ${NC_TOOLS_COMPILE_OPTIONS}
    )

    target_compile_definitions(GeometryConverter_unit_tests
        PRIVATE
            TEST_COLLATERAL_DIR="${NC_CONVERT_TEST_COLLATERAL_DIR}"
    )

    target_include_directories(GeometryConverter_unit_tests
        PRIVATE
            ${PROJECT_SOURCE_DIR}/test/collateral
            ${PROJECT_SOURCE_DIR}/test/common
            ${PROJECT_SOURCE_DIR}/include
            ${PROJECT_SOURCE_DIR}/source/ncconvert
    )

    target_sources(GeometryConverter_unit_tests
        PRIVATE
            ${PROJECT_SOURCE_DIR}/source/ncconvert/analysis/GeometryAnalysis.cpp
            ${PROJECT_SOURCE_DIR}/source/ncconvert/analysis/Sanitize.cpp
            ${PROJECT_SOURCE_DIR}/source/ncconvert/converters/GeometryConverter.cpp
            ${PROJECT_SOURCE_DIR}/source/ncconvert/utility/Path.cpp
    )

    target_link_libraries(GeometryConverter_unit_tests
        PRIVATE
            gtest_main
            NcMath
            assimp::assimp
    )

    add_test(GeometryConverter_unit_tests GeometryConverter_unit_tests)
endif()

## GeometryAnalysis Tests ###
add_executable(GeometryAnalysis_unit_tests
    GeometryAnalysis_tests.cpp
)

target_compile_options(GeometryAnalysis_unit_tests
    PUBLIC
        ${NC_TOOLS_COMPILE_OPTIONS}
)

target_include_directories(GeometryAnalysis_unit_tests
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/source/ncconvert
)

target_sources(GeometryAnalysis_unit_tests
    PRIVATE
        ${PROJECT_SOURCE_DIR}/source/ncconvert/analysis/GeometryAnalysis.cpp
)

target_link_libraries(GeometryAnalysis_unit_tests
    PRIVATE
        gtest_main
        NcMath
)

add_test(GeometryAnalysis_unit_tests GeometryAnalysis_unit_tests)

## TextureAnalysis Tests ###
add_executable(TextureAnalysis_unit_tests
    TextureAnalysis_unit_tests.cpp
)

target_compile_options(TextureAnalysis_unit_tests
    PUBLIC
        ${NC_TOOLS_COMPILE_OPTIONS}
)

target_include_directories(TextureAnalysis_unit_tests
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/source/ncconvert
)

target_sources(TextureAnalysis_unit_tests
    PRIVATE
        ${PROJECT_SOURCE_DIR}/source/ncconvert/analysis/TextureAnalysis.cpp
)

target_link_libraries(TextureAnalysis_unit_tests
    PRIVATE
        gtest_main
        NcMath
)

add_test(TextureAnalysis_unit_tests TextureAnalysis_unit_tests)

## TextureConverter Tests ###
add_executable(TextureConverter_unit_tests
    TextureConverter_unit_tests.cpp
)

target_compile_options(TextureConverter_unit_tests
    PUBLIC
        ${NC_TOOLS_COMPILE_OPTIONS}
)

target_compile_definitions(TextureConverter_unit_tests
    PRIVATE
        TEST_COLLATERAL_DIR="${NC_CONVERT_TEST_COLLATERAL_DIR}"
)

target_include_directories(TextureConverter_unit_tests
    PRIVATE
        ${PROJECT_SOURCE_DIR}/test/collateral
        ${PROJECT_SOURCE_DIR}/test/common
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/source/ncconvert

)

target_include_directories(TextureConverter_unit_tests
    SYSTEM PRIVATE
        ${PROJECT_SOURCE_DIR}/source/external
)

target_sources(TextureConverter_unit_tests
    PRIVATE
        ${PROJECT_SOURCE_DIR}/source/ncconvert/analysis/TextureAnalysis.cpp
        ${PROJECT_SOURCE_DIR}/source/ncconvert/converters/TextureConverter.cpp
        ${PROJECT_SOURCE_DIR}/source/ncconvert/utility/Path.cpp
)

target_link_libraries(TextureConverter_unit_tests
    PRIVATE
        gtest_main
        NcMath
)

add_test(TextureConverter_unit_tests TextureConverter_unit_tests)
