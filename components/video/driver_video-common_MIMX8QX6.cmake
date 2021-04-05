if(NOT DRIVER_VIDEO-COMMON_MIMX8QX6_INCLUDED)
    
    set(DRIVER_VIDEO-COMMON_MIMX8QX6_INCLUDED true CACHE BOOL "driver_video-common component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_video_common.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_common_MIMX8QX6)

endif()
