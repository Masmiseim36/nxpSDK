if(NOT COMPONENT_LPI2C_ADAPTER_MIMXRT1052_INCLUDED)
    
    set(COMPONENT_LPI2C_ADAPTER_MIMXRT1052_INCLUDED true CACHE BOOL "component_lpi2c_adapter component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_adapter_lpi2c.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_common_MIMXRT1052)

    include(driver_lpi2c_MIMXRT1052)

endif()
