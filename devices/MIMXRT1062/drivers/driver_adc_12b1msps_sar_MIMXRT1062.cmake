if(NOT DRIVER_ADC_12B1MSPS_SAR_MIMXRT1062_INCLUDED)
    
    set(DRIVER_ADC_12B1MSPS_SAR_MIMXRT1062_INCLUDED true CACHE BOOL "driver_adc_12b1msps_sar component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_adc.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_common_MIMXRT1062)

endif()
