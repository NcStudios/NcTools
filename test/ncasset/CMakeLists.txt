### GetAssetType Tests ###
add_executable(GetAssetType_unit_tests
    GetAssetType_unit_tests.cpp
)

target_compile_options(GetAssetType_unit_tests
    PUBLIC
        ${NC_TOOLS_COMPILE_OPTIONS}
)

target_include_directories(GetAssetType_unit_tests
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include/ncasset
)

target_sources(GetAssetType_unit_tests
    PRIVATE
        ${PROJECT_SOURCE_DIR}/source/ncasset/NcaHeader.cpp
)

target_link_libraries(GetAssetType_unit_tests
    PRIVATE
        gtest_main
        NcUtility
)

add_test(GetAssetType_unit_tests GetAssetType_unit_tests)
