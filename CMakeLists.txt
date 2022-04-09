cmake_minimum_required(VERSION 3.20)
project("nc-tools" VERSION "0.1.0" LANGUAGES CXX)

if(${PROJECT_SOURCE_DIR} EQUAL ${PROJECT_BINARY_DIR})
    message(FATAL_ERROR "Don't be a fool, out-of-source build with your tool.")
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(WARNING_FLAGS -Werror -Wall -Wextra -Wfatal-errors -Wno-cast-function-type)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(WARNING_FLAGS /W3 /WX)
endif()

option(NC_BUILD_TESTS "Include tests in build" ON)

add_executable(nc-tools)

configure_file(${PROJECT_SOURCE_DIR}/source/Version.h.in
               ${PROJECT_SOURCE_DIR}/source/Version.h
)

set_target_properties(nc-tools
    PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}
        RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}
)

target_compile_options(nc-tools
    PRIVATE
        ${WARNING_FLAGS}
)

target_include_directories(nc-tools
    PRIVATE
        ${PROJECT_SOURCE_DIR}/source
        ${PROJECT_SOURCE_DIR}/external/assimp/include
)

target_link_libraries(nc-tools
    PRIVATE
        ${PROJECT_SOURCE_DIR}/external/assimp/lib/assimp-vc143-mt.lib
)

add_subdirectory(source)

if(NC_BUILD_TESTS)
    add_subdirectory(test)
endif()