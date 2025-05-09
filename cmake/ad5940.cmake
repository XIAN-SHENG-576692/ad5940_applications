function(import_ad5940 TARGET_NAME AD5940_DIR)
    if(NOT IS_DIRECTORY ${AD5940_DIR})
        message(FATAL_ERROR "Given AD5940_DIR path '${AD5940_DIR}' is not valid")
    endif()

    # 搜尋 source files
    file(GLOB_RECURSE AD5940_SOURCES
        ${AD5940_DIR}/*.c
    )
    target_sources(${TARGET_NAME} PRIVATE ${AD5940_SOURCES})

    # 加入所有子資料夾當 include path
    file(GLOB_RECURSE AD5940_RECURSE_DIRS LIST_DIRECTORIES true ${AD5940_DIR})
    foreach(_dir IN LISTS AD5940_RECURSE_DIRS)
        if(IS_DIRECTORY ${_dir})
            target_include_directories(${TARGET_NAME} PRIVATE ${_dir})
        endif()
    endforeach()
endfunction()
