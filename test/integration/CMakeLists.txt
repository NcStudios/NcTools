### Serialization Tests ###
add_executable(Serialize_unit_tests
    Serialize_unit_tests.cpp
)

target_compile_options(Serialize_unit_tests
    PUBLIC
        ${NC_TOOLS_COMPILE_OPTIONS}
)

target_include_directories(Serialize_unit_tests
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/source/ncasset
        ${PROJECT_SOURCE_DIR}/source/ncconvert
)

target_sources(Serialize_unit_tests
    PRIVATE
        ${PROJECT_SOURCE_DIR}/source/ncasset/Deserialize.cpp
        ${PROJECT_SOURCE_DIR}/source/ncasset/RawNcaBuffer.cpp
        ${PROJECT_SOURCE_DIR}/source/ncconvert/builder/Serialize.cpp
        ${PROJECT_SOURCE_DIR}/source/ncconvert/utility/BlobSize.cpp
)

target_link_libraries(Serialize_unit_tests
    PRIVATE
        gtest_main
        NcMath
)

add_test(Serialize_unit_tests Serialize_unit_tests)
