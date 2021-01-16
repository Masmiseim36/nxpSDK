if(NOT DRIVER_COMMON_MIMXRT1064_INCLUDED)
    
    set(DRIVER_COMMON_MIMXRT1064_INCLUDED true CACHE BOOL "driver_common component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_common.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_clock_MIMXRT1064)

    include(device_MIMXRT1064_CMSIS_MIMXRT1064)

endif()
