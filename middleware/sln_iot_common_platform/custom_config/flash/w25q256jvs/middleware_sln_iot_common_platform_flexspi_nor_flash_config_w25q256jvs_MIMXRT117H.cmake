if(NOT MIDDLEWARE_SLN_IOT_COMMON_PLATFORM_FLEXSPI_NOR_FLASH_CONFIG_W25Q256JVS_MIMXRT117H_INCLUDED)
    
    set(MIDDLEWARE_SLN_IOT_COMMON_PLATFORM_FLEXSPI_NOR_FLASH_CONFIG_W25Q256JVS_MIMXRT117H_INCLUDED true CACHE BOOL "middleware_sln_iot_common_platform_flexspi_nor_flash_config_w25q256jvs component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/sln_flash_config.c
        ${CMAKE_CURRENT_LIST_DIR}/flexspi_nor_flash_ops.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_flexspi_MIMXRT117H)

endif()
