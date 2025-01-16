IF(NOT DEFINED FPU)  
    SET(FPU "-mfloat-abi=hard -mfpu=fpv5-d16")  
ENDIF()  

IF(NOT DEFINED SPECS)  
    SET(SPECS "--specs=nano.specs --specs=nosys.specs")  
ENDIF()  

IF(NOT DEFINED DEBUG_CONSOLE_CONFIG)  
    SET(DEBUG_CONSOLE_CONFIG "-DSDK_DEBUGCONSOLE=1")  
ENDIF()  

SET(CMAKE_ASM_FLAGS_SDRAM_RELEASE " \
    ${CMAKE_ASM_FLAGS_SDRAM_RELEASE} \
    -D__STARTUP_INITIALIZE_QADATA \
    -D__STARTUP_INITIALIZE_NONCACHEDATA \
    -D__STARTUP_CLEAR_BSS \
    -DMCUXPRESSO_SDK \
    -DCPU_MIMXRT1024DAG5A \
    -DOSA_USED \
    -mthumb \
    -mcpu=cortex-m7 \
    ${FPU} \
")
SET(CMAKE_C_FLAGS_SDRAM_RELEASE " \
    ${CMAKE_C_FLAGS_SDRAM_RELEASE} \
    -include ${ProjDirPath}/../mcux_config.h \
    -DFSL_SDK_ENABLE_DRIVER_CACHE_CONTROL=1 \
    -DUSB_DEVICE_CONFIG_BUFFER_PROPERTY_CACHEABLE=1 \
    -DNDEBUG \
    -DUSE_SDRAM=1 \
    -DSKIP_SYSCLK_INIT=1 \
    -DUSB_STACK_FREERTOS \
    -DMCUX_META_BUILD \
    -DMCUXPRESSO_SDK \
    -DCPU_MIMXRT1024DAG5A \
    -DOSA_USED \
    -DUSE_RTOS=1 \
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
SET(CMAKE_CXX_FLAGS_SDRAM_RELEASE " \
    ${CMAKE_CXX_FLAGS_SDRAM_RELEASE} \
    -DNDEBUG \
    -DUSE_SDRAM=1 \
    -DMCUX_META_BUILD \
    -DMCUXPRESSO_SDK \
    -DCPU_MIMXRT1024DAG5A \
    -DOSA_USED \
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
SET(CMAKE_EXE_LINKER_FLAGS_SDRAM_RELEASE " \
    ${CMAKE_EXE_LINKER_FLAGS_SDRAM_RELEASE} \
    -Xlinker \
    --defsym=__heap_size__=0x2000 \
    -Xlinker \
    --defsym=__stack_size__=0x2000 \
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
    -T\"${ProjDirPath}/MIMXRT1024xxxxx_sdram.ld\" -static \
")
