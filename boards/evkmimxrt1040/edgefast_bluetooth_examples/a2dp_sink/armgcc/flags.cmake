IF(NOT DEFINED FPU)  
    SET(FPU "-mfloat-abi=hard -mfpu=fpv5-d16")  
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
    -DCPU_MIMXRT1042XJM5B \
    -DDEBUG_CONSOLE_TRANSFER_NON_BLOCKING \
    -DOSA_USED \
    -mthumb \
    -mcpu=cortex-m7 \
    ${FPU} \
")
SET(CMAKE_ASM_FLAGS_FLEXSPI_NOR_DEBUG " \
    ${CMAKE_ASM_FLAGS_FLEXSPI_NOR_DEBUG} \
    -D__STARTUP_INITIALIZE_RAMFUNCTION \
    -D__STARTUP_INITIALIZE_NONCACHEDATA \
    -D__STARTUP_CLEAR_BSS \
    -DMCUXPRESSO_SDK \
    -DCPU_MIMXRT1042XJM5B \
    -DDEBUG_CONSOLE_TRANSFER_NON_BLOCKING \
    -DOSA_USED \
    -g \
    -mthumb \
    -mcpu=cortex-m7 \
    ${FPU} \
")
SET(CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE " \
    ${CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE} \
    -include ${ProjDirPath}/../edgefast_bluetooth_app.h \
    -include ${ProjDirPath}/../mcux_config.h \
    -DLFS_NO_ASSERT \
    -DNDEBUG \
    -DXIP_BOOT_HEADER_ENABLE=1 \
    -DXIP_EXTERNAL_FLASH=1 \
    -DLPUART_RING_BUFFER_SIZE=1024U \
    -DPRINTF_ADVANCED_ENABLE=1 \
    -DSDK_OS_FREE_RTOS \
    -DUSE_RTOS=1 \
    -DAPPL_USE_STANDARD_IO \
    -DCONFIG_ARM=1 \
    -DEDGEFAST_BT_LITTLEFS_MFLASH \
    -DFSL_DRIVER_TRANSFER_DOUBLE_WEAK_IRQ=0 \
    -DFSL_FEATURE_FLASH_PAGE_SIZE_BYTES=4096 \
    -DFSL_SDK_ENABLE_DRIVER_CACHE_CONTROL=1 \
    -DGATT_CLIENT \
    -DGATT_DB \
    -DHAL_AUDIO_DMA_INIT_ENABLE=0 \
    -DHAL_UART_ADAPTER_FIFO=1 \
    -DHAL_UART_DMA_ENABLE=1 \
    -DIOT_WIFI_ENABLE_SAVE_NETWORK=1 \
    -DLFS_NO_ERROR=1 \
    -DLFS_NO_INTRINSICS=1 \
    -DSAI_XFER_QUEUE_SIZE=6 \
    -DSDIO_ENABLED=1 \
    -DUSB_HOST_CONFIG_BUFFER_PROPERTY_CACHEABLE=1 \
    -DgMemManagerLight=0 \
    -DMCUX_META_BUILD \
    -DMCUXPRESSO_SDK \
    -DCPU_MIMXRT1042XJM5B \
    -DDEBUG_CONSOLE_TRANSFER_NON_BLOCKING=1 \
    -DOSA_USED \
    -DSERIAL_PORT_TYPE_UART=1 \
    -DSDK_I2C_BASED_COMPONENT_USED=1 \
    -DCODEC_MULTI_ADAPTERS=1 \
    -DCODEC_WM8960_ENABLE \
    -DMFLASH_FILE_BASEADDR=7340032 \
    -DLFS_THREADSAFE=1 \
    -DCFG_CLASSIC \
    -DLOG_ENABLE_ASYNC_MODE=1 \
    -DLOG_MAX_ARGUMENT_COUNT=10 \
    -DLOG_ENABLE_OVERWRITE=0 \
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
    -DXIP_BOOT_HEADER_ENABLE=1 \
    -DXIP_EXTERNAL_FLASH=1 \
    -DLPUART_RING_BUFFER_SIZE=1024U \
    -DPRINTF_ADVANCED_ENABLE=1 \
    -DSDK_OS_FREE_RTOS \
    -DUSE_RTOS=1 \
    -DAPPL_USE_STANDARD_IO \
    -DCONFIG_ARM=1 \
    -DEDGEFAST_BT_LITTLEFS_MFLASH \
    -DFSL_DRIVER_TRANSFER_DOUBLE_WEAK_IRQ=0 \
    -DFSL_FEATURE_FLASH_PAGE_SIZE_BYTES=4096 \
    -DFSL_SDK_ENABLE_DRIVER_CACHE_CONTROL=1 \
    -DGATT_CLIENT \
    -DGATT_DB \
    -DHAL_AUDIO_DMA_INIT_ENABLE=0 \
    -DHAL_UART_ADAPTER_FIFO=1 \
    -DHAL_UART_DMA_ENABLE=1 \
    -DIOT_WIFI_ENABLE_SAVE_NETWORK=1 \
    -DLFS_NO_ERROR=1 \
    -DLFS_NO_INTRINSICS=1 \
    -DSAI_XFER_QUEUE_SIZE=6 \
    -DSDIO_ENABLED=1 \
    -DUSB_HOST_CONFIG_BUFFER_PROPERTY_CACHEABLE=1 \
    -DgMemManagerLight=0 \
    -DMCUX_META_BUILD \
    -DMCUXPRESSO_SDK \
    -DCPU_MIMXRT1042XJM5B \
    -DDEBUG_CONSOLE_TRANSFER_NON_BLOCKING=1 \
    -DOSA_USED \
    -DSERIAL_PORT_TYPE_UART=1 \
    -DSDK_I2C_BASED_COMPONENT_USED=1 \
    -DCODEC_MULTI_ADAPTERS=1 \
    -DCODEC_WM8960_ENABLE \
    -DMFLASH_FILE_BASEADDR=7340032 \
    -DLFS_THREADSAFE=1 \
    -DCFG_CLASSIC \
    -DLOG_ENABLE_ASYNC_MODE=1 \
    -DLOG_MAX_ARGUMENT_COUNT=10 \
    -DLOG_ENABLE_OVERWRITE=0 \
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
    -DCPU_MIMXRT1042XJM5B \
    -DDEBUG_CONSOLE_TRANSFER_NON_BLOCKING=1 \
    -DOSA_USED \
    -DSERIAL_PORT_TYPE_UART=1 \
    -DMFLASH_FILE_BASEADDR=7340032 \
    -DUSE_RTOS=1 \
    -DLFS_NO_INTRINSICS=1 \
    -DLFS_NO_ERROR=1 \
    -DLFS_THREADSAFE=1 \
    -DCFG_CLASSIC \
    -DLOG_ENABLE_ASYNC_MODE=1 \
    -DLOG_MAX_ARGUMENT_COUNT=10 \
    -DLOG_ENABLE_OVERWRITE=0 \
    -DCONFIG_ARM=1 \
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
SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_DEBUG " \
    ${CMAKE_CXX_FLAGS_FLEXSPI_NOR_DEBUG} \
    -DDEBUG \
    -DXIP_EXTERNAL_FLASH=1 \
    -DMCUX_META_BUILD \
    -DMCUXPRESSO_SDK \
    -DCPU_MIMXRT1042XJM5B \
    -DDEBUG_CONSOLE_TRANSFER_NON_BLOCKING=1 \
    -DOSA_USED \
    -DSERIAL_PORT_TYPE_UART=1 \
    -DMFLASH_FILE_BASEADDR=7340032 \
    -DUSE_RTOS=1 \
    -DLFS_NO_INTRINSICS=1 \
    -DLFS_NO_ERROR=1 \
    -DLFS_THREADSAFE=1 \
    -DCFG_CLASSIC \
    -DLOG_ENABLE_ASYNC_MODE=1 \
    -DLOG_MAX_ARGUMENT_COUNT=10 \
    -DLOG_ENABLE_OVERWRITE=0 \
    -DCONFIG_ARM=1 \
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
    -mcpu=cortex-m7 \
    ${FPU} \
    ${SPECS} \
    -T\"${ProjDirPath}/MIMXRT1042xxxxx_flexspi_nor.ld\" -static \
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
    -mcpu=cortex-m7 \
    ${FPU} \
    ${SPECS} \
    -T\"${ProjDirPath}/MIMXRT1042xxxxx_flexspi_nor.ld\" -static \
")
