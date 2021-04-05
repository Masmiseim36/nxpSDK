if(NOT DRIVER_COMMON_MKV11Z7_INCLUDED)
    
    set(DRIVER_COMMON_MKV11Z7_INCLUDED true CACHE BOOL "driver_common component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_common.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_clock_MKV11Z7)

    include(device_MKV11Z7_CMSIS_MKV11Z7)

endif()
