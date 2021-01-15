if(NOT DRIVER_COMMON_MIMXRT1021_INCLUDED)
    
    set(DRIVER_COMMON_MIMXRT1021_INCLUDED true CACHE BOOL "driver_common component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_common.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_clock_MIMXRT1021)

    include(device_MIMXRT1021_CMSIS_MIMXRT1021)

endif()
