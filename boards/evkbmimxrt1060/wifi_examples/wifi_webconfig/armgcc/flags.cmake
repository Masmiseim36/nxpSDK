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
    -D__STARTUP_INITIALIZE_NONCACHEDATA \
    -D__STARTUP_CLEAR_BSS \
    -DMCUXPRESSO_SDK \
    -DCPU_MIMXRT1062DVL6B \
    -DOSA_USED \
    -g \
    -mthumb \
    -mcpu=cortex-m7 \
    ${FPU} \
")
SET(CMAKE_ASM_FLAGS_FLEXSPI_NOR_RELEASE " \
    ${CMAKE_ASM_FLAGS_FLEXSPI_NOR_RELEASE} \
    -D__STARTUP_INITIALIZE_NONCACHEDATA \
    -D__STARTUP_CLEAR_BSS \
    -DMCUXPRESSO_SDK \
    -DCPU_MIMXRT1062DVL6B \
    -DOSA_USED \
    -mthumb \
    -mcpu=cortex-m7 \
    ${FPU} \
")
SET(CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG " \
    ${CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG} \
    -include ${ProjDirPath}/../mcux_config.h \
    -DDEBUG \
    -DXIP_BOOT_HEADER_ENABLE=1 \
    -DXIP_EXTERNAL_FLASH=1 \
    -DUSE_RTOS=1 \
    -DPRINTF_ADVANCED_ENABLE=1 \
    -DLWIP_NETIF_API=1 \
    -DHTTPSRV_CFG_WEBSOCKET_ENABLED=1 \
    -DHTTPSRV_CFG_DEFAULT_SES_CNT=8 \
    -DMCUX_META_BUILD \
    -DMCUXPRESSO_SDK \
    -DCPU_MIMXRT1062DVL6B \
    -DOSA_USED \
    -DSERIAL_PORT_TYPE_UART=1 \
    -DMFLASH_FILE_BASEADDR=7340032 \
    -DCONFIG_NXP_WIFI_SOFTAP_SUPPORT=1 \
    -DSDIO_ENABLED=1 \
    -DSDK_OS_FREE_RTOS \
    -g \
    -O0 \
    -Wall \
    -fno-common \
    -ffunction-sections \
    -fdata-sections \
    -fno-builtin \
    -mthumb \
    -mapcs \
    -std=gnu99 \
    -mcpu=cortex-m7 \
    ${FPU} \
    ${DEBUG_CONSOLE_CONFIG} \
")
SET(CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE " \
    ${CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE} \
    -include ${ProjDirPath}/../mcux_config.h \
    -DNDEBUG \
    -DXIP_BOOT_HEADER_ENABLE=1 \
    -DXIP_EXTERNAL_FLASH=1 \
    -DUSE_RTOS=1 \
    -DPRINTF_ADVANCED_ENABLE=1 \
    -DLWIP_NETIF_API=1 \
    -DHTTPSRV_CFG_WEBSOCKET_ENABLED=1 \
    -DHTTPSRV_CFG_DEFAULT_SES_CNT=8 \
    -DMCUX_META_BUILD \
    -DMCUXPRESSO_SDK \
    -DCPU_MIMXRT1062DVL6B \
    -DOSA_USED \
    -DSERIAL_PORT_TYPE_UART=1 \
    -DMFLASH_FILE_BASEADDR=7340032 \
    -DCONFIG_NXP_WIFI_SOFTAP_SUPPORT=1 \
    -DSDIO_ENABLED=1 \
    -DSDK_OS_FREE_RTOS \
    -Os \
    -Wall \
    -fno-common \
    -ffunction-sections \
    -fdata-sections \
    -fno-builtin \
    -mthumb \
    -mapcs \
    -std=gnu99 \
    -mcpu=cortex-m7 \
    ${FPU} \
    ${DEBUG_CONSOLE_CONFIG} \
")
SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_DEBUG " \
    ${CMAKE_CXX_FLAGS_FLEXSPI_NOR_DEBUG} \
    -DDEBUG \
    -DXIP_EXTERNAL_FLASH=1 \
    -DMCUX_META_BUILD \
    -DMCUXPRESSO_SDK \
    -DCPU_MIMXRT1062DVL6B \
    -DOSA_USED \
    -DSERIAL_PORT_TYPE_UART=1 \
    -DMFLASH_FILE_BASEADDR=7340032 \
    -DUSE_RTOS=1 \
    -DSDK_OS_FREE_RTOS \
    -g \
    -O0 \
    -Wall \
    -fno-common \
    -ffunction-sections \
    -fdata-sections \
    -fno-builtin \
    -mthumb \
    -mapcs \
    -fno-rtti \
    -fno-exceptions \
    -mcpu=cortex-m7 \
    ${FPU} \
    ${DEBUG_CONSOLE_CONFIG} \
")
SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_RELEASE " \
    ${CMAKE_CXX_FLAGS_FLEXSPI_NOR_RELEASE} \
    -DNDEBUG \
    -DXIP_EXTERNAL_FLASH=1 \
    -DMCUX_META_BUILD \
    -DMCUXPRESSO_SDK \
    -DCPU_MIMXRT1062DVL6B \
    -DOSA_USED \
    -DSERIAL_PORT_TYPE_UART=1 \
    -DMFLASH_FILE_BASEADDR=7340032 \
    -DUSE_RTOS=1 \
    -DSDK_OS_FREE_RTOS \
    -Os \
    -Wall \
    -fno-common \
    -ffunction-sections \
    -fdata-sections \
    -fno-builtin \
    -mthumb \
    -mapcs \
    -fno-rtti \
    -fno-exceptions \
    -mcpu=cortex-m7 \
    ${FPU} \
    ${DEBUG_CONSOLE_CONFIG} \
")
SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_DEBUG " \
    ${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_DEBUG} \
    -g \
    -Xlinker \
    --defsym=__stack_size__=2048 \
    -Xlinker \
    --defsym=__heap_size__=90000 \
    -Xlinker \
    -Map=output.map \
    -Wall \
    -fno-common \
    -ffunction-sections \
    -fdata-sections \
    -fno-builtin \
    -mthumb \
    -mapcs \
    -Wl,--gc-sections \
    -Wl,-static \
    -Wl,-z \
    -Wl,muldefs \
    -Wl,-Map=output.map \
    -Wl,--print-memory-usage \
    -mcpu=cortex-m7 \
    ${FPU} \
    ${SPECS} \
    -T\"${ProjDirPath}/MIMXRT1062xxxxx_webconfig.ld\" -static \
")
SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_RELEASE " \
    ${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_RELEASE} \
    -Xlinker \
    --defsym=__stack_size__=2048 \
    -Xlinker \
    --defsym=__heap_size__=90000 \
    -Xlinker \
    -Map=output.map \
    -Wall \
    -fno-common \
    -ffunction-sections \
    -fdata-sections \
    -fno-builtin \
    -mthumb \
    -mapcs \
    -Wl,--gc-sections \
    -Wl,-static \
    -Wl,-z \
    -Wl,muldefs \
    -Wl,-Map=output.map \
    -Wl,--print-memory-usage \
    -mcpu=cortex-m7 \
    ${FPU} \
    ${SPECS} \
    -T\"${ProjDirPath}/MIMXRT1062xxxxx_webconfig.ld\" -static \
")
