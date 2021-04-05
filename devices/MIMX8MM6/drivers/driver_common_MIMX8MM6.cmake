if(NOT DRIVER_COMMON_MIMX8MM6_INCLUDED)
    
    set(DRIVER_COMMON_MIMX8MM6_INCLUDED true CACHE BOOL "driver_common component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_common.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_clock_MIMX8MM6)

    include(device_MIMX8MM6_CMSIS_MIMX8MM6)

endif()
