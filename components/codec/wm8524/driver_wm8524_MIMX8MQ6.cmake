if(NOT DRIVER_WM8524_MIMX8MQ6_INCLUDED)
    
    set(DRIVER_WM8524_MIMX8MQ6_INCLUDED true CACHE BOOL "driver_wm8524 component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_wm8524.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_common_MIMX8MQ6)

endif()
