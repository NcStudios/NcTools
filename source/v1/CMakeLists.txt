FetchContent_Declare(assimp
                     GIT_REPOSITORY https://github.com/NcStudios/assimp.git
                     GIT_TAG        origin/master
)

FetchContent_MakeAvailable(assimp)

add_executable(nc-tools
    ${PROJECT_SOURCE_DIR}/source/v1/NcTools_v1.cpp
)

configure_file(${PROJECT_SOURCE_DIR}/source/v1/Version.h.in
               ${PROJECT_SOURCE_DIR}/source/v1/Version.h
)

target_compile_options(nc-tools
    PRIVATE
        ${WARNING_FLAGS}
)

target_link_libraries(nc-tools
    PRIVATE
        assimp::assimp
)

install(
    TARGETS
        nc-tools
    DESTINATION
        bin
)