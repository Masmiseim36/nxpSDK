IF(NOT DEFINED FPU)  
    SET(FPU "-mfloat-abi=hard -mfpu=fpv5-d16")  
ENDIF()  

IF(NOT DEFINED SPECS)  
    SET(SPECS "--specs=nano.specs --specs=nosys.specs")  
ENDIF()  

IF(NOT DEFINED DEBUG_CONSOLE_CONFIG)  
    SET(DEBUG_CONSOLE_CONFIG "-DSDK_DEBUGCONSOLE=1")  
ENDIF()  

SET(CMAKE_ASM_FLAGS_FLEXSPI_NOR_DEBUG " \
    ${CMAKE_ASM_FLAGS_FLEXSPI_NOR_DEBUG} \
    -D__STARTUP_CLEAR_BSS \
    -DDEBUG \
    -D__STARTUP_INITIALIZE_NONCACHEDATA \
    -mcpu=cortex-m7 \
    -mthumb \
    ${FPU} \
")
SET(CMAKE_ASM_FLAGS_FLEXSPI_NOR_RELEASE " \
    ${CMAKE_ASM_FLAGS_FLEXSPI_NOR_RELEASE} \
    -D__STARTUP_CLEAR_BSS \
    -DNDEBUG \
    -D__STARTUP_INITIALIZE_NONCACHEDATA \
    -mcpu=cortex-m7 \
    -mthumb \
    ${FPU} \
")
SET(CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG " \
    ${CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG} \
    -include ${ProjDirPath}/../app_config.h \
    -include ${ProjDirPath}/../wifi_config.h \
    -DXIP_EXTERNAL_FLASH=1 \
    -DXIP_BOOT_HEADER_ENABLE=1 \
    -DDEBUG \
    -DCPU_MIMXRT1062DVL6B \
    -DFSL_SDK_ENABLE_DRIVER_CACHE_CONTROL=1 \
    -DAPPL_CONFIG_DISABLE \
    -DPXM \
    -DHTS \
    -DAPPL_MENU_OPS \
    -DFSL_FEATURE_PHYKSZ8081_USE_RMII50M_MODE \
    -DSAI_XFER_QUEUE_SIZE=6 \
    -DSDK_COMPONENT_INTEGRATION=1 \
    -DgNvStorageIncluded_d=1 \
    -DgNvTableKeptInRam_d=1 \
    -DgAppUseNvm_d=1 \
    -DIPSP_HAVE_6LO_NIFACE \
    -DLWIP_IPV6 \
    -DSUPPORT_HTTP_CLIENT \
    -DFSL_OSA_MAIN_FUNC_ENABLE=0 \
    -DAPPL_GAP_CENTRAL \
    -DAPPL_GAP_PERIPHERAL \
    -DHAL_UART_ADAPTER_FIFO=1 \
    -DHAL_UART_ISR_PRIORITY=configLIBRARY_LOWEST_INTERRUPT_PRIORITY \
    -DSTORAGE_IDLE_TASK_SYNC_ENABLE=1 \
    -DNVRAM_WORKAROUND \
    -DBT_SNOOP_WRITE_TRUNCATE \
    -DOOB_WAKEUP \
    -DHAL_AUDIO_DMA_INIT_ENABLE=0 \
    -DHAL_UART_DMA_ENABLE=1 \
    -DUSE_RTOS=1 \
    -DPRINTF_ADVANCED_ENABLE=1 \
    -DSDK_OS_FREE_RTOS \
    -DFSL_OSA_TASK_ENABLE=1 \
    -DDEBUG_CONSOLE_TRANSFER_NON_BLOCKING \
    -DGATT_DB \
    -DGATT_CLIENT \
    -DCONFIG_BT_SNOOP=1 \
    -DDEBUG_CONSOLE_SCANF_MAX_LOG_LEN=64 \
    -DCONFIG_BT_SETTINGS \
    -DAPPL_LIMIT_LOGS \
    -DNXP_CODE \
    -DNVM_NO_COMPONNET=1 \
    -DSDIO_ENABLED=1 \
    -DCPU_MIMXRT1062DVL6A \
    -DCPU_MIMXRT1062DVL6A_cm7 \
    -DMCUXPRESSO_SDK \
    -DSDK_I2C_BASED_COMPONENT_USED=1 \
    -DSERIAL_PORT_TYPE_UART=1 \
    -DBOARD_USE_CODEC=1 \
    -DCODEC_WM8960_ENABLE \
    -DLWIP_DISABLE_PBUF_POOL_SIZE_SANITY_CHECKS=1 \
    -DLWIP_TIMEVAL_PRIVATE=0 \
    -DLWIP_DNS=1 \
    -DLWIP_NETIF_HOSTNAME=1 \
    -DLWIP_NETIF_STATUS_CALLBACK=1 \
    -DLWIP_IGMP=1 \
    -g \
    -O0 \
    -mcpu=cortex-m7 \
    -Wall \
    -mthumb \
    -MMD \
    -MP \
    -fno-common \
    -ffunction-sections \
    -fdata-sections \
    -ffreestanding \
    -fno-builtin \
    -mapcs \
    -std=gnu99 \
    ${FPU} \
    ${DEBUG_CONSOLE_CONFIG} \
")
SET(CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE " \
    ${CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE} \
    -include ${ProjDirPath}/../app_config.h \
    -include ${ProjDirPath}/../wifi_config.h \
    -DXIP_EXTERNAL_FLASH=1 \
    -DXIP_BOOT_HEADER_ENABLE=1 \
    -DNDEBUG \
    -DCPU_MIMXRT1062DVL6B \
    -DFSL_SDK_ENABLE_DRIVER_CACHE_CONTROL=1 \
    -DAPPL_CONFIG_DISABLE \
    -DPXM \
    -DHTS \
    -DAPPL_MENU_OPS \
    -DFSL_FEATURE_PHYKSZ8081_USE_RMII50M_MODE \
    -DSAI_XFER_QUEUE_SIZE=6 \
    -DSDK_COMPONENT_INTEGRATION=1 \
    -DgNvStorageIncluded_d=1 \
    -DgNvTableKeptInRam_d=1 \
    -DgAppUseNvm_d=1 \
    -DIPSP_HAVE_6LO_NIFACE \
    -DLWIP_IPV6 \
    -DSUPPORT_HTTP_CLIENT \
    -DFSL_OSA_MAIN_FUNC_ENABLE=0 \
    -DAPPL_GAP_CENTRAL \
    -DAPPL_GAP_PERIPHERAL \
    -DHAL_UART_ADAPTER_FIFO=1 \
    -DHAL_UART_ISR_PRIORITY=configLIBRARY_LOWEST_INTERRUPT_PRIORITY \
    -DSTORAGE_IDLE_TASK_SYNC_ENABLE=1 \
    -DNVRAM_WORKAROUND \
    -DBT_SNOOP_WRITE_TRUNCATE \
    -DOOB_WAKEUP \
    -DHAL_AUDIO_DMA_INIT_ENABLE=0 \
    -DHAL_UART_DMA_ENABLE=1 \
    -DUSE_RTOS=1 \
    -DPRINTF_ADVANCED_ENABLE=1 \
    -DSDK_OS_FREE_RTOS \
    -DFSL_OSA_TASK_ENABLE=1 \
    -DDEBUG_CONSOLE_TRANSFER_NON_BLOCKING \
    -DGATT_DB \
    -DGATT_CLIENT \
    -DCONFIG_BT_SNOOP=1 \
    -DDEBUG_CONSOLE_SCANF_MAX_LOG_LEN=64 \
    -DCONFIG_BT_SETTINGS \
    -DAPPL_LIMIT_LOGS \
    -DNXP_CODE \
    -DNVM_NO_COMPONNET=1 \
    -DSDIO_ENABLED=1 \
    -DCPU_MIMXRT1062DVL6A \
    -DCPU_MIMXRT1062DVL6A_cm7 \
    -DMCUXPRESSO_SDK \
    -DSDK_I2C_BASED_COMPONENT_USED=1 \
    -DSERIAL_PORT_TYPE_UART=1 \
    -DBOARD_USE_CODEC=1 \
    -DCODEC_WM8960_ENABLE \
    -DLWIP_DISABLE_PBUF_POOL_SIZE_SANITY_CHECKS=1 \
    -DLWIP_TIMEVAL_PRIVATE=0 \
    -DLWIP_DNS=1 \
    -DLWIP_NETIF_HOSTNAME=1 \
    -DLWIP_NETIF_STATUS_CALLBACK=1 \
    -DLWIP_IGMP=1 \
    -Os \
    -mcpu=cortex-m7 \
    -Wall \
    -mthumb \
    -MMD \
    -MP \
    -fno-common \
    -ffunction-sections \
    -fdata-sections \
    -ffreestanding \
    -fno-builtin \
    -mapcs \
    -std=gnu99 \
    ${FPU} \
    ${DEBUG_CONSOLE_CONFIG} \
")
SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_DEBUG " \
    ${CMAKE_CXX_FLAGS_FLEXSPI_NOR_DEBUG} \
    -DDEBUG \
    -DCPU_MIMXRT1062DVL6B \
    -DMCUXPRESSO_SDK \
    -DSERIAL_PORT_TYPE_UART=1 \
    -g \
    -O0 \
    -mcpu=cortex-m7 \
    -Wall \
    -mthumb \
    -MMD \
    -MP \
    -fno-common \
    -ffunction-sections \
    -fdata-sections \
    -ffreestanding \
    -fno-builtin \
    -mapcs \
    -fno-rtti \
    -fno-exceptions \
    ${FPU} \
    ${DEBUG_CONSOLE_CONFIG} \
")
SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_RELEASE " \
    ${CMAKE_CXX_FLAGS_FLEXSPI_NOR_RELEASE} \
    -DNDEBUG \
    -DCPU_MIMXRT1062DVL6B \
    -DMCUXPRESSO_SDK \
    -DSERIAL_PORT_TYPE_UART=1 \
    -Os \
    -mcpu=cortex-m7 \
    -Wall \
    -mthumb \
    -MMD \
    -MP \
    -fno-common \
    -ffunction-sections \
    -fdata-sections \
    -ffreestanding \
    -fno-builtin \
    -mapcs \
    -fno-rtti \
    -fno-exceptions \
    ${FPU} \
    ${DEBUG_CONSOLE_CONFIG} \
")
SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_DEBUG " \
    ${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_DEBUG} \
    -g \
    -mcpu=cortex-m7 \
    -Wall \
    -fno-common \
    -ffunction-sections \
    -fdata-sections \
    -ffreestanding \
    -fno-builtin \
    -mthumb \
    -mapcs \
    -Xlinker \
    --gc-sections \
    -Xlinker \
    -static \
    -Xlinker \
    -z \
    -Xlinker \
    muldefs \
    -Xlinker \
    -Map=output.map \
    -Wl,--print-memory-usage \
    ${FPU} \
    ${SPECS} \
    -T${ProjDirPath}/MIMXRT1062_cm7_flexspi_nor.ld -static \
")
SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_RELEASE " \
    ${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_RELEASE} \
    -mcpu=cortex-m7 \
    -Wall \
    -fno-common \
    -ffunction-sections \
    -fdata-sections \
    -ffreestanding \
    -fno-builtin \
    -mthumb \
    -mapcs \
    -Xlinker \
    --gc-sections \
    -Xlinker \
    -static \
    -Xlinker \
    -z \
    -Xlinker \
    muldefs \
    -Xlinker \
    -Map=output.map \
    -Wl,--print-memory-usage \
    ${FPU} \
    ${SPECS} \
    -T${ProjDirPath}/MIMXRT1062_cm7_flexspi_nor.ld -static \
")
