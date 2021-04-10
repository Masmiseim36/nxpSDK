if(NOT COMPONENT_FLEXSPI_NOR_FLASH_ADAPTER_RT1060EVK_MIMXRT1062_INCLUDED)
    
    set(COMPONENT_FLEXSPI_NOR_FLASH_ADAPTER_RT1060EVK_MIMXRT1062_INCLUDED true CACHE BOOL "component_flexspi_nor_flash_adapter_rt1060evk component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_adapter_flexspi_nor_flash.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_common_MIMXRT1062)

    include(driver_flexspi_MIMXRT1062)

endif()
