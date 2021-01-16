if(NOT DRIVER_DCDC_1_MIMXRT1052_INCLUDED)
    
    set(DRIVER_DCDC_1_MIMXRT1052_INCLUDED true CACHE BOOL "driver_dcdc_1 component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_dcdc.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_common_MIMXRT1052)

endif()
