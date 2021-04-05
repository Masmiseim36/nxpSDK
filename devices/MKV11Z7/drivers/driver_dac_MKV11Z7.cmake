if(NOT DRIVER_DAC_MKV11Z7_INCLUDED)
    
    set(DRIVER_DAC_MKV11Z7_INCLUDED true CACHE BOOL "driver_dac component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_dac.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_common_MKV11Z7)

endif()
