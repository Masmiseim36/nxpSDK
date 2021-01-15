if(NOT DRIVER_COMMON_MIMXRT1052_INCLUDED)
    
    set(DRIVER_COMMON_MIMXRT1052_INCLUDED true CACHE BOOL "driver_common component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_common.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_clock_MIMXRT1052)

    include(device_MIMXRT1052_CMSIS_MIMXRT1052)

endif()
