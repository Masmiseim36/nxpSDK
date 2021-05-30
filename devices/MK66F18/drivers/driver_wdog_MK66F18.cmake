if(NOT DRIVER_WDOG_MK66F18_INCLUDED)
    
    set(DRIVER_WDOG_MK66F18_INCLUDED true CACHE BOOL "driver_wdog component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_wdog.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_common_MK66F18)

endif()
