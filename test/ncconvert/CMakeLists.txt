set(NC_CONVERT_TEST_COLLATERAL_DIR ${CMAKE_CURRENT_LIST_DIR}/../collateral)

## FbxConverter Tests ###
add_executable(FbxConverter_unit_tests
    FbxConverter_unit_tests.cpp
)

target_compile_options(FbxConverter_unit_tests
    PUBLIC
        ${WARNING_FLAGS}
)

target_compile_definitions(FbxConverter_unit_tests
    PRIVATE
        TEST_COLLATERAL_DIR="${NC_CONVERT_TEST_COLLATERAL_DIR}"
)

target_include_directories(FbxConverter_unit_tests
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/source/ncconvert
)

target_sources(FbxConverter_unit_tests
    PRIVATE
        ${PROJECT_SOURCE_DIR}/source/ncconvert/analysis/GeometryAnalysis.cpp
        ${PROJECT_SOURCE_DIR}/source/ncconvert/analysis/Sanitize.cpp
        ${PROJECT_SOURCE_DIR}/source/ncconvert/converters/FbxConverter.cpp
)

target_link_libraries(FbxConverter_unit_tests
    PRIVATE
        gtest_main
        NcMath
        assimp::assimp
)

add_test(FbxConverter_unit_tests FbxConverter_unit_tests)

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