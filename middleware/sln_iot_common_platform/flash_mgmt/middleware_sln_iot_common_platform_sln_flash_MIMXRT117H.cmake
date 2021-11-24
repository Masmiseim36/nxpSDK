if(NOT MIDDLEWARE_SLN_IOT_COMMON_PLATFORM_SLN_FLASH_MIMXRT117H_INCLUDED)
    
    set(MIDDLEWARE_SLN_IOT_COMMON_PLATFORM_SLN_FLASH_MIMXRT117H_INCLUDED true CACHE BOOL "middleware_sln_iot_common_platform_sln_flash component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/sln_flash.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    #OR Logic component
    if(CONFIG_USE_middleware_sln_iot_common_platform_flexspi_hyperflash_config_is26kl256s_MIMXRT117H)
         include(middleware_sln_iot_common_platform_flexspi_hyperflash_config_is26kl256s_MIMXRT117H)
    endif()
    if(CONFIG_USE_middleware_sln_iot_common_platform_flexspi_nor_flash_config_mt25ql256aba_MIMXRT117H)
         include(middleware_sln_iot_common_platform_flexspi_nor_flash_config_mt25ql256aba_MIMXRT117H)
    endif()
    if(CONFIG_USE_middleware_sln_iot_common_platform_flexspi_nor_flash_config_mt25ql128aba_MIMXRT117H)
         include(middleware_sln_iot_common_platform_flexspi_nor_flash_config_mt25ql128aba_MIMXRT117H)
    endif()
    if(CONFIG_USE_middleware_sln_iot_common_platform_flexspi_nor_flash_config_w25q256jvs_MIMXRT117H)
         include(middleware_sln_iot_common_platform_flexspi_nor_flash_config_w25q256jvs_MIMXRT117H)
    endif()
    if(NOT (CONFIG_USE_middleware_sln_iot_common_platform_flexspi_hyperflash_config_is26kl256s_MIMXRT117H OR CONFIG_USE_middleware_sln_iot_common_platform_flexspi_nor_flash_config_mt25ql256aba_MIMXRT117H OR CONFIG_USE_middleware_sln_iot_common_platform_flexspi_nor_flash_config_mt25ql128aba_MIMXRT117H OR CONFIG_USE_middleware_sln_iot_common_platform_flexspi_nor_flash_config_w25q256jvs_MIMXRT117H))
        message(WARNING "Since middleware_sln_iot_common_platform_flexspi_hyperflash_config_is26kl256s_MIMXRT117H/middleware_sln_iot_common_platform_flexspi_nor_flash_config_mt25ql256aba_MIMXRT117H/middleware_sln_iot_common_platform_flexspi_nor_flash_config_mt25ql128aba_MIMXRT117H/middleware_sln_iot_common_platform_flexspi_nor_flash_config_w25q256jvs_MIMXRT117H is not included at first or config in config.cmake file, use middleware_sln_iot_common_platform_flexspi_hyperflash_config_is26kl256s_MIMXRT117H by default.")
        include(middleware_sln_iot_common_platform_flexspi_hyperflash_config_is26kl256s_MIMXRT117H)
    endif()

    include(driver_common_MIMXRT117H)

endif()
