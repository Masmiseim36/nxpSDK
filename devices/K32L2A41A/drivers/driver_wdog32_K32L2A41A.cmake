if(NOT DRIVER_WDOG32_K32L2A41A_INCLUDED)
    
    set(DRIVER_WDOG32_K32L2A41A_INCLUDED true CACHE BOOL "driver_wdog32 component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_wdog32.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_common_K32L2A41A)

endif()
