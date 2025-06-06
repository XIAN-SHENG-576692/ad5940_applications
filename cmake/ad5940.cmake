function(import_ad5940 TARGET_NAME AD5940_DIR)
    # Check if the given AD5940_DIR is a valid directory
    if(NOT IS_DIRECTORY ${AD5940_DIR})
        message(FATAL_ERROR "Given AD5940_DIR path '${AD5940_DIR}' is not valid")
    endif()

    # Search for source files
    file(GLOB_RECURSE AD5940_SOURCES
        ${AD5940_DIR}/*.c
    )
    target_sources(${TARGET_NAME} PRIVATE ${AD5940_SOURCES})

    # Add all subdirectories as include paths
    file(GLOB_RECURSE AD5940_RECURSE_DIRS LIST_DIRECTORIES true ${AD5940_DIR})
    foreach(_dir IN LISTS AD5940_RECURSE_DIRS)
        if(IS_DIRECTORY ${_dir})
            target_include_directories(${TARGET_NAME} PRIVATE ${_dir})
        endif()
    endforeach()
endfunction()
