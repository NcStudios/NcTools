add_executable(nc-convert)

target_compile_options(nc-convert
    PRIVATE
        ${WARNING_FLAGS}
)

target_include_directories(nc-convert
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/source
        ${PROJECT_SOURCE_DIR}/source/ncconvert
)

set(nc-convert-common-source ${PROJECT_SOURCE_DIR}/source/common/NcaHeader.cpp
                             ${PROJECT_SOURCE_DIR}/source/common/Serialize.cpp
)

target_sources(nc-convert
    PRIVATE
        ${PROJECT_SOURCE_DIR}/source/ncconvert/Main.cpp
        ${nc-convert-common-source}
)

target_link_libraries(nc-convert
    PRIVATE
        NcMath
        assimp::assimp
)

install(
    TARGETS
        nc-convert
    DESTINATION
        bin
)

add_subdirectory(analysis)
add_subdirectory(builder)
add_subdirectory(converters)
add_subdirectory(utility)
