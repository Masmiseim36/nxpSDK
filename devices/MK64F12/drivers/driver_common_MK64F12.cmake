if(NOT DRIVER_COMMON_MK64F12_INCLUDED)
    
    set(DRIVER_COMMON_MK64F12_INCLUDED true CACHE BOOL "driver_common component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_common.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_clock_MK64F12)

    include(device_MK64F12_CMSIS_MK64F12)

endif()
