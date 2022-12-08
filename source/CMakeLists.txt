if(NC_TOOLS_BUILD_IMPORTER_LIB)
    add_subdirectory(ncasset)
endif()

if(NC_TOOLS_BUILD_CONVERTER)
    add_subdirectory(ncconvert)
endif()
