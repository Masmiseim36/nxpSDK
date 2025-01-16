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
    -D__STARTUP_INITIALIZE_RAMFUNCTION \
    -D__STARTUP_INITIALIZE_NONCACHEDATA \
    -D__STARTUP_CLEAR_BSS \
    -DMCUXPRESSO_SDK \
    -DCPU_MIMXRT1062DVL6B \
    -DDEBUG_CONSOLE_TRANSFER_NON_BLOCKING \
    -DOSA_USED \
    -g \
    -mthumb \
    -mcpu=cortex-m7 \
    ${FPU} \
")
SET(CMAKE_ASM_FLAGS_FLEXSPI_NOR_RELEASE " \
    ${CMAKE_ASM_FLAGS_FLEXSPI_NOR_RELEASE} \
    -D__STARTUP_INITIALIZE_RAMFUNCTION \
    -D__STARTUP_INITIALIZE_NONCACHEDATA \
    -D__STARTUP_CLEAR_BSS \
    -DMCUXPRESSO_SDK \
    -DCPU_MIMXRT1062DVL6B \
    -DDEBUG_CONSOLE_TRANSFER_NON_BLOCKING \
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
    -DVIT_MODEL_EN \
    -DVIT_MODEL_CN \
    -DVIT_MODEL_DE \
    -DVIT_MODEL_ES \
    -DVIT_MODEL_FR \
    -DVIT_MODEL_IT \
    -DVIT_MODEL_JA \
    -DVIT_MODEL_KO \
    -DVIT_MODEL_PT \
    -DVIT_MODEL_TR \
    -DVIT_PROC=1 \
    -DVOICE_SEEKER_PROC=1 \
    -DFILE_FS_TABLE_SIZE=3 \
    -DPLATFORM_RT1060 \
    -DFSL_SDK_ENABLE_DRIVER_CACHE_CONTROL=1 \
    -DSAI_XFER_QUEUE_SIZE=4 \
    -DDEBUG_CONSOLE_TRANSFER_NON_BLOCKING \
    -DOSA_USED \
    -DSHELL_TASK_STACK_SIZE=4000 \
    -DMCUX_META_BUILD \
    -DMCUXPRESSO_SDK \
    -DCPU_MIMXRT1062DVL6B \
    -DDISABLEFLOAT16 \
    -DSERIAL_PORT_TYPE_UART=1 \
    -DSDK_I2C_BASED_COMPONENT_USED=1 \
    -DCODEC_MULTI_ADAPTERS=1 \
    -DCODEC_WM8962_ENABLE \
    -DUSE_RTOS=1 \
    -DSTREAMER_ENABLE_AUDIO_PROC \
    -DSTREAMER_ENABLE_AUDIO_SINK \
    -DSTREAMER_ENABLE_AUDIO_SRC \
    -DSTREAMER_ENABLE_FILE_SINK \
    -DSTREAMER_ENABLE_MEM_SRC \
    -DSTREAMER_ENABLE_PROCESS_SINK \
    -DSD_ENABLED=1 \
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
    -DVIT_MODEL_EN \
    -DVIT_MODEL_CN \
    -DVIT_MODEL_DE \
    -DVIT_MODEL_ES \
    -DVIT_MODEL_FR \
    -DVIT_MODEL_IT \
    -DVIT_MODEL_JA \
    -DVIT_MODEL_KO \
    -DVIT_MODEL_PT \
    -DVIT_MODEL_TR \
    -DVIT_PROC=1 \
    -DVOICE_SEEKER_PROC=1 \
    -DFILE_FS_TABLE_SIZE=3 \
    -DPLATFORM_RT1060 \
    -DFSL_SDK_ENABLE_DRIVER_CACHE_CONTROL=1 \
    -DSAI_XFER_QUEUE_SIZE=4 \
    -DDEBUG_CONSOLE_TRANSFER_NON_BLOCKING \
    -DOSA_USED \
    -DSHELL_TASK_STACK_SIZE=4000 \
    -DMCUX_META_BUILD \
    -DMCUXPRESSO_SDK \
    -DCPU_MIMXRT1062DVL6B \
    -DDISABLEFLOAT16 \
    -DSERIAL_PORT_TYPE_UART=1 \
    -DSDK_I2C_BASED_COMPONENT_USED=1 \
    -DCODEC_MULTI_ADAPTERS=1 \
    -DCODEC_WM8962_ENABLE \
    -DUSE_RTOS=1 \
    -DSTREAMER_ENABLE_AUDIO_PROC \
    -DSTREAMER_ENABLE_AUDIO_SINK \
    -DSTREAMER_ENABLE_AUDIO_SRC \
    -DSTREAMER_ENABLE_FILE_SINK \
    -DSTREAMER_ENABLE_MEM_SRC \
    -DSTREAMER_ENABLE_PROCESS_SINK \
    -DSD_ENABLED=1 \
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
    -DDEBUG_CONSOLE_TRANSFER_NON_BLOCKING \
    -DOSA_USED \
    -DSERIAL_PORT_TYPE_UART=1 \
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
    -DDEBUG_CONSOLE_TRANSFER_NON_BLOCKING \
    -DOSA_USED \
    -DSERIAL_PORT_TYPE_UART=1 \
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
    --defsym=__stack_size__=0x1000 \
    -Xlinker \
    --defsym=__heap_size__=0x1000 \
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
    -T\"${ProjDirPath}/MIMXRT1062xxxxx_flexspi_nor.ld\" -static \
")
SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_RELEASE " \
    ${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_RELEASE} \
    -Xlinker \
    --defsym=__stack_size__=0x1000 \
    -Xlinker \
    --defsym=__heap_size__=0x1000 \
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
    -T\"${ProjDirPath}/MIMXRT1062xxxxx_flexspi_nor.ld\" -static \
")
