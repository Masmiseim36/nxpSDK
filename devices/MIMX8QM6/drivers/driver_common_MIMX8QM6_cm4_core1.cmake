if(NOT DRIVER_COMMON_MIMX8QM6_cm4_core1_INCLUDED)
    
    set(DRIVER_COMMON_MIMX8QM6_cm4_core1_INCLUDED true CACHE BOOL "driver_common component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_common.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_clock_MIMX8QM6_cm4_core1)

    include(device_MIMX8QM6_CMSIS_MIMX8QM6_cm4_core1)

endif()
