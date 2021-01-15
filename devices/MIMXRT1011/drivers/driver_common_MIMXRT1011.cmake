if(NOT DRIVER_COMMON_MIMXRT1011_INCLUDED)
    
    set(DRIVER_COMMON_MIMXRT1011_INCLUDED true CACHE BOOL "driver_common component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_common.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_clock_MIMXRT1011)

    include(device_MIMXRT1011_CMSIS_MIMXRT1011)

endif()
