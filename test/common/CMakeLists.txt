### Serialization Tests ###
add_executable(Serialize_unit_tests
    Serialize_unit_tests.cpp
)

target_compile_options(Serialize_unit_tests
    PUBLIC
        ${WARNING_FLAGS}
)

target_include_directories(Serialize_unit_tests
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/source/common
        ${PROJECT_SOURCE_DIR}/source/ncasset
)

target_sources(Serialize_unit_tests
    PRIVATE
        ${PROJECT_SOURCE_DIR}/source/common/Deserialize.cpp
        ${PROJECT_SOURCE_DIR}/source/common/NcaHeader.cpp
        ${PROJECT_SOURCE_DIR}/source/common/RawNcaBuffer.cpp
        ${PROJECT_SOURCE_DIR}/source/common/Serialize.cpp
)

target_link_libraries(Serialize_unit_tests
    PRIVATE
        gtest_main
        NcMath
)

add_test(Serialize_unit_tests Serialize_unit_tests)
