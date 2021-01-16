if(NOT DRIVER_RTWATCHDOG_MIMXRT1064_INCLUDED)
    
    set(DRIVER_RTWATCHDOG_MIMXRT1064_INCLUDED true CACHE BOOL "driver_rtwatchdog component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_rtwdog.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_common_MIMXRT1064)

endif()
