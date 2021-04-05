if(NOT DRIVER_WM8524_MIMX8MM6_INCLUDED)
    
    set(DRIVER_WM8524_MIMX8MM6_INCLUDED true CACHE BOOL "driver_wm8524 component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_wm8524.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_common_MIMX8MM6)

endif()
