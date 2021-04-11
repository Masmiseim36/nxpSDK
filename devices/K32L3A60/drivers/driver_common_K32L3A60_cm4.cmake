if(NOT DRIVER_COMMON_K32L3A60_cm4_INCLUDED)
    
    set(DRIVER_COMMON_K32L3A60_cm4_INCLUDED true CACHE BOOL "driver_common component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_common.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_clock_K32L3A60_cm4)

    include(device_K32L3A60_CMSIS_K32L3A60_cm4)

endif()
