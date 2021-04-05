if(NOT DRIVER_DISPLAY-IT6263_MIMX8QX6_INCLUDED)
    
    set(DRIVER_DISPLAY-IT6263_MIMX8QX6_INCLUDED true CACHE BOOL "driver_display-it6263 component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_it6263.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_video-common_MIMX8QX6)

    include(driver_video-i2c_MIMX8QX6)

    include(driver_display-common_MIMX8QX6)

endif()
