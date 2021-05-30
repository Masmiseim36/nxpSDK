if(NOT DRIVER_COMMON_MK66F18_INCLUDED)
    
    set(DRIVER_COMMON_MK66F18_INCLUDED true CACHE BOOL "driver_common component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_common.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_clock_MK66F18)

    include(device_MK66F18_CMSIS_MK66F18)

endif()
