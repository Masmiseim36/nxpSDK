if(NOT DRIVER_COMMON_MCIMX7U5_INCLUDED)
    
    set(DRIVER_COMMON_MCIMX7U5_INCLUDED true CACHE BOOL "driver_common component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_common.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_clock_MCIMX7U5)

    include(device_MCIMX7U5_CMSIS_MCIMX7U5)

endif()
