IF(NOT DEFINED FPU)  
    SET(FPU "-mfloat-abi=hard -mfpu=fpv5-sp-d16")  
ENDIF()  

IF(NOT DEFINED SPECS)  
    SET(SPECS "--specs=nano.specs --specs=nosys.specs")  
ENDIF()  

IF(NOT DEFINED DEBUG_CONSOLE_CONFIG)  
    SET(DEBUG_CONSOLE_CONFIG "-DSDK_DEBUGCONSOLE=1")  
ENDIF()  

SET(CMAKE_ASM_FLAGS_FLEXSPI_NOR_RELEASE " \
    ${CMAKE_ASM_FLAGS_FLEXSPI_NOR_RELEASE} \
    -D__STARTUP_INITIALIZE_RAMFUNCTION \
    -D__STARTUP_INITIALIZE_NONCACHEDATA \
    -D__STARTUP_CLEAR_BSS \
    -DMCUXPRESSO_SDK \
    -DCPU_MIMXRT1189CVM8B_cm33 \
    -DDEBUG_CONSOLE_TRANSFER_NON_BLOCKING \
    -DOSA_USED \
    -mthumb \
    -mcpu=cortex-m33 \
    ${FPU} \
")
SET(CMAKE_ASM_FLAGS_FLEXSPI_NOR_DEBUG " \
    ${CMAKE_ASM_FLAGS_FLEXSPI_NOR_DEBUG} \
    -D__STARTUP_INITIALIZE_RAMFUNCTION \
    -D__STARTUP_INITIALIZE_NONCACHEDATA \
    -D__STARTUP_CLEAR_BSS \
    -DMCUXPRESSO_SDK \
    -DCPU_MIMXRT1189CVM8B_cm33 \
    -DDEBUG_CONSOLE_TRANSFER_NON_BLOCKING \
    -DOSA_USED \
    -g \
    -mthumb \
    -mcpu=cortex-m33 \
    ${FPU} \
")
SET(CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE " \
    ${CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE} \
    -include ${ProjDirPath}/../edgefast_bluetooth_app.h \
    -include ${ProjDirPath}/../mcux_config.h \
    -DLFS_NO_ASSERT \
    -DNDEBUG \
    -DXIP_EXTERNAL_FLASH=1 \
    -DXIP_BOOT_HEADER_ENABLE=0 \
    -DLPUART_RING_BUFFER_SIZE=1024U \
    -DPRINTF_ADVANCED_ENABLE=1 \
    -DSDK_OS_FREE_RTOS \
    -DSHELL_TASK_STACK_SIZE=2900 \
    -DUSE_RTOS=1 \
    -DAPPL_USE_STANDARD_IO \
    -DCONFIG_ARM=1 \
    -DEDGEFAST_BT_LITTLEFS_MFLASH \
    -DFSL_DRIVER_TRANSFER_DOUBLE_WEAK_IRQ=0 \
    -DFSL_FEATURE_FLASH_PAGE_SIZE_BYTES=4096 \
    -DFSL_SDK_ENABLE_DRIVER_CACHE_CONTROL=1 \
    -DGATT_CLIENT \
    -DGATT_DB \
    -DHAL_UART_ADAPTER_FIFO=1 \
    -DHAL_UART_DMA_ENABLE=1 \
    -DIOT_WIFI_ENABLE_SAVE_NETWORK=1 \
    -DLFS_NO_ERROR=1 \
    -DLFS_NO_INTRINSICS=1 \
    -DMBEDTLS_MCUX_ELE_S400_API \
    -DSDIO_ENABLED=1 \
    -DMCUX_META_BUILD \
    -DMCUXPRESSO_SDK \
    -DCPU_MIMXRT1189CVM8B_cm33 \
    -DDEBUG_CONSOLE_TRANSFER_NON_BLOCKING=1 \
    -DOSA_USED=1 \
    -DSERIAL_PORT_TYPE_UART=1 \
    -DMFLASH_FILE_BASEADDR=14221312 \
    -DCACHE_MODE_WRITE_THROUGH=1 \
    -DLFS_THREADSAFE=1 \
    -DLOG_ENABLE_ASYNC_MODE=1 \
    -DLOG_MAX_ARGUMENT_COUNT=10 \
    -DLOG_ENABLE_OVERWRITE=0 \
    -DSHELL_ADVANCE=1 \
    -DDEBUG_CONSOLE_RX_ENABLE=0 \
    -DSHELL_USE_COMMON_TASK=0 \
    -Os \
    -Wall \
    -fno-common \
    -ffunction-sections \
    -fdata-sections \
    -fno-builtin \
    -mthumb \
    -mapcs \
    -std=gnu99 \
    -mcpu=cortex-m33 \
    -fomit-frame-pointer \
    -Wno-unused-function \
    ${FPU} \
    ${DEBUG_CONSOLE_CONFIG} \
")
SET(CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG " \
    ${CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG} \
    -include ${ProjDirPath}/../edgefast_bluetooth_app.h \
    -include ${ProjDirPath}/../mcux_config.h \
    -DDEBUG \
    -DXIP_EXTERNAL_FLASH=1 \
    -DXIP_BOOT_HEADER_ENABLE=0 \
    -DLPUART_RING_BUFFER_SIZE=1024U \
    -DPRINTF_ADVANCED_ENABLE=1 \
    -DSDK_OS_FREE_RTOS \
    -DSHELL_TASK_STACK_SIZE=2900 \
    -DUSE_RTOS=1 \
    -DAPPL_USE_STANDARD_IO \
    -DCONFIG_ARM=1 \
    -DEDGEFAST_BT_LITTLEFS_MFLASH \
    -DFSL_DRIVER_TRANSFER_DOUBLE_WEAK_IRQ=0 \
    -DFSL_FEATURE_FLASH_PAGE_SIZE_BYTES=4096 \
    -DFSL_SDK_ENABLE_DRIVER_CACHE_CONTROL=1 \
    -DGATT_CLIENT \
    -DGATT_DB \
    -DHAL_UART_ADAPTER_FIFO=1 \
    -DHAL_UART_DMA_ENABLE=1 \
    -DIOT_WIFI_ENABLE_SAVE_NETWORK=1 \
    -DLFS_NO_ERROR=1 \
    -DLFS_NO_INTRINSICS=1 \
    -DMBEDTLS_MCUX_ELE_S400_API \
    -DSDIO_ENABLED=1 \
    -DMCUX_META_BUILD \
    -DMCUXPRESSO_SDK \
    -DCPU_MIMXRT1189CVM8B_cm33 \
    -DDEBUG_CONSOLE_TRANSFER_NON_BLOCKING=1 \
    -DOSA_USED=1 \
    -DSERIAL_PORT_TYPE_UART=1 \
    -DMFLASH_FILE_BASEADDR=14221312 \
    -DCACHE_MODE_WRITE_THROUGH=1 \
    -DLFS_THREADSAFE=1 \
    -DLOG_ENABLE_ASYNC_MODE=1 \
    -DLOG_MAX_ARGUMENT_COUNT=10 \
    -DLOG_ENABLE_OVERWRITE=0 \
    -DSHELL_ADVANCE=1 \
    -DDEBUG_CONSOLE_RX_ENABLE=0 \
    -DSHELL_USE_COMMON_TASK=0 \
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
    -mcpu=cortex-m33 \
    -fomit-frame-pointer \
    -Wno-unused-function \
    ${FPU} \
    ${DEBUG_CONSOLE_CONFIG} \
")
SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_RELEASE " \
    ${CMAKE_CXX_FLAGS_FLEXSPI_NOR_RELEASE} \
    -DNDEBUG \
    -DXIP_EXTERNAL_FLASH=1 \
    -DMCUX_META_BUILD \
    -DMCUXPRESSO_SDK \
    -DCPU_MIMXRT1189CVM8B_cm33 \
    -DDEBUG_CONSOLE_TRANSFER_NON_BLOCKING=1 \
    -DOSA_USED=1 \
    -DSERIAL_PORT_TYPE_UART=1 \
    -DMFLASH_FILE_BASEADDR=14221312 \
    -DUSE_RTOS=1 \
    -DMBEDTLS_MCUX_ELE_S400_API \
    -DLFS_NO_INTRINSICS=1 \
    -DLFS_NO_ERROR=1 \
    -DLFS_THREADSAFE=1 \
    -DLOG_ENABLE_ASYNC_MODE=1 \
    -DLOG_MAX_ARGUMENT_COUNT=10 \
    -DLOG_ENABLE_OVERWRITE=0 \
    -DCONFIG_ARM=1 \
    -DSHELL_ADVANCE=1 \
    -DDEBUG_CONSOLE_RX_ENABLE=0 \
    -DSHELL_USE_COMMON_TASK=0 \
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
    -mcpu=cortex-m33 \
    ${FPU} \
    ${DEBUG_CONSOLE_CONFIG} \
")
SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_DEBUG " \
    ${CMAKE_CXX_FLAGS_FLEXSPI_NOR_DEBUG} \
    -DDEBUG \
    -DXIP_EXTERNAL_FLASH=1 \
    -DMCUX_META_BUILD \
    -DMCUXPRESSO_SDK \
    -DCPU_MIMXRT1189CVM8B_cm33 \
    -DDEBUG_CONSOLE_TRANSFER_NON_BLOCKING=1 \
    -DOSA_USED=1 \
    -DSERIAL_PORT_TYPE_UART=1 \
    -DMFLASH_FILE_BASEADDR=14221312 \
    -DUSE_RTOS=1 \
    -DMBEDTLS_MCUX_ELE_S400_API \
    -DLFS_NO_INTRINSICS=1 \
    -DLFS_NO_ERROR=1 \
    -DLFS_THREADSAFE=1 \
    -DLOG_ENABLE_ASYNC_MODE=1 \
    -DLOG_MAX_ARGUMENT_COUNT=10 \
    -DLOG_ENABLE_OVERWRITE=0 \
    -DCONFIG_ARM=1 \
    -DSHELL_ADVANCE=1 \
    -DDEBUG_CONSOLE_RX_ENABLE=0 \
    -DSHELL_USE_COMMON_TASK=0 \
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
    -mcpu=cortex-m33 \
    ${FPU} \
    ${DEBUG_CONSOLE_CONFIG} \
")
SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_RELEASE " \
    ${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_RELEASE} \
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
    -mcpu=cortex-m33 \
    ${FPU} \
    ${SPECS} \
    -T\"${ProjDirPath}/MIMXRT1189xxxxx_cm33_flexspi_nor.ld\" -static \
")
SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_DEBUG " \
    ${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_DEBUG} \
    -g \
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
    -mcpu=cortex-m33 \
    ${FPU} \
    ${SPECS} \
    -T\"${ProjDirPath}/MIMXRT1189xxxxx_cm33_flexspi_nor.ld\" -static \
")
