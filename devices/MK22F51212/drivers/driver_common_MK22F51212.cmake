if(NOT DRIVER_COMMON_MK22F51212_INCLUDED)
    
    set(DRIVER_COMMON_MK22F51212_INCLUDED true CACHE BOOL "driver_common component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_common.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_clock_MK22F51212)

    include(device_MK22F51212_CMSIS_MK22F51212)

endif()
