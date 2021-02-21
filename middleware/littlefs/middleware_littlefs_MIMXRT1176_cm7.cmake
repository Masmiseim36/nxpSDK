if(NOT MIDDLEWARE_LITTLEFS_MIMXRT1176_cm7_INCLUDED)
    
    set(MIDDLEWARE_LITTLEFS_MIMXRT1176_cm7_INCLUDED true CACHE BOOL "middleware_littlefs component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/lfs.c
        ${CMAKE_CURRENT_LIST_DIR}/lfs_util.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


endif()
