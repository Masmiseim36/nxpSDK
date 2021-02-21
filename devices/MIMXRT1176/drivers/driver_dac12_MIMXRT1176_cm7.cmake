if(NOT DRIVER_DAC12_MIMXRT1176_cm7_INCLUDED)
    
    set(DRIVER_DAC12_MIMXRT1176_cm7_INCLUDED true CACHE BOOL "driver_dac12 component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_dac12.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_common_MIMXRT1176_cm7)

endif()
