# Fetch gtest
set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)

enable_testing()

FetchContent_Declare(googletest
                     GIT_REPOSITORY https://github.com/NcStudios/googletest.git
                     GIT_TAG        origin/main
)

FetchContent_MakeAvailable(googletest)

include(GoogleTest)

add_executable(nc-tools_tests)

set_target_properties(nc-tools_tests
    PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}
        RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}
)

target_compile_options(nc-tools_tests
    PRIVATE
        ${WARNING_FLAGS}
)

target_sources(nc-tools_tests
    PRIVATE
        ${PROJECT_SOURCE_DIR}/test/Test.cpp
)

add_subdirectory(ncasset)