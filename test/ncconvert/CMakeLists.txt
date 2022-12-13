set(NC_CONVERT_TEST_COLLATERAL_DIR ${CMAKE_CURRENT_LIST_DIR}/../collateral)

## GeometryConverter Tests ###
add_executable(GeometryConverter_unit_tests
    GeometryConverter_unit_tests.cpp
)

target_compile_options(GeometryConverter_unit_tests
    PUBLIC
        ${WARNING_FLAGS}
)

target_compile_definitions(GeometryConverter_unit_tests
    PRIVATE
        TEST_COLLATERAL_DIR="${NC_CONVERT_TEST_COLLATERAL_DIR}"
)

target_include_directories(GeometryConverter_unit_tests
    PRIVATE
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

## GeometryAnalysis Tests ###
add_executable(GeometryAnalysis_unit_tests
    GeometryAnalysis_tests.cpp
)

target_compile_options(GeometryAnalysis_unit_tests
    PUBLIC
        ${WARNING_FLAGS}
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

## TextureConverter Tests ###
add_executable(TextureConverter_unit_tests
    TextureConverter_unit_tests.cpp
)

target_compile_options(TextureConverter_unit_tests
    PUBLIC
        ${WARNING_FLAGS}
)

target_compile_definitions(TextureConverter_unit_tests
    PRIVATE
        TEST_COLLATERAL_DIR="${NC_CONVERT_TEST_COLLATERAL_DIR}"
)

target_include_directories(TextureConverter_unit_tests
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/source/ncconvert
        ${PROJECT_SOURCE_DIR}/source/external
)

target_sources(TextureConverter_unit_tests
    PRIVATE
        ${PROJECT_SOURCE_DIR}/source/ncconvert/converters/TextureConverter.cpp
        ${PROJECT_SOURCE_DIR}/source/ncconvert/utility/Path.cpp
)

target_link_libraries(TextureConverter_unit_tests
    PRIVATE
        gtest_main
        NcMath
)

add_test(TextureConverter_unit_tests TextureConverter_unit_tests)
