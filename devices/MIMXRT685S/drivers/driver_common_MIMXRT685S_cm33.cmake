if(NOT DRIVER_COMMON_MIMXRT685S_cm33_INCLUDED)
    
    set(DRIVER_COMMON_MIMXRT685S_cm33_INCLUDED true CACHE BOOL "driver_common component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_common.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_reset_MIMXRT685S_cm33)

    include(driver_clock_MIMXRT685S_cm33)

    include(device_MIMXRT685S_CMSIS_MIMXRT685S_cm33)

endif()
