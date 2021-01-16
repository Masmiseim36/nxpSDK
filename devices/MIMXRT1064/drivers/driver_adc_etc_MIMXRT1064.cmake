if(NOT DRIVER_ADC_ETC_MIMXRT1064_INCLUDED)
    
    set(DRIVER_ADC_ETC_MIMXRT1064_INCLUDED true CACHE BOOL "driver_adc_etc component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_adc_etc.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_common_MIMXRT1064)

endif()
