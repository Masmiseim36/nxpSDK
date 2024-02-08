IF(NOT DEFINED FPU)  
    SET(FPU "-mfloat-abi=hard -mfpu=fpv5-d16")  
ENDIF()  

IF(NOT DEFINED SPECS)  
    SET(SPECS "--specs=nano.specs --specs=nosys.specs")  
ENDIF()  

IF(NOT DEFINED DEBUG_CONSOLE_CONFIG)  
    SET(DEBUG_CONSOLE_CONFIG "-DSDK_DEBUGCONSOLE=1")  
ENDIF()  

SET(CMAKE_ASM_FLAGS_HYPERRAM_DEBUG " \
    ${CMAKE_ASM_FLAGS_HYPERRAM_DEBUG} \
    -D__STARTUP_INITIALIZE_QADATA=1 \
    -DDEBUG \
    -D__STARTUP_CLEAR_BSS=1 \
    -D__STARTUP_INITIALIZE_NONCACHEDATA=1 \
    -g \
    -mcpu=cortex-m7 \
    -Wall \
    -mthumb \
    ${FPU} \
")
SET(CMAKE_ASM_FLAGS_HYPERRAM_RELEASE " \
    ${CMAKE_ASM_FLAGS_HYPERRAM_RELEASE} \
    -D__STARTUP_INITIALIZE_QADATA=1 \
    -DNDEBUG \
    -D__STARTUP_CLEAR_BSS=1 \
    -D__STARTUP_INITIALIZE_NONCACHEDATA=1 \
    -mcpu=cortex-m7 \
    -Wall \
    -mthumb \
    ${FPU} \
")
SET(CMAKE_C_FLAGS_HYPERRAM_DEBUG " \
    ${CMAKE_C_FLAGS_HYPERRAM_DEBUG} \
    -DDATA_SECTION_IS_CACHEABLE=1 \
    -DUSE_HYPERRAM \
    -DDEBUG \
    -DCPU_MIMXRT1189CVM8B_cm7 \
    -DUSE_RTOS=1 \
    -DPHY_RTL8201=1 \
    -DFSL_SDK_ENABLE_DRIVER_CACHE_CONTROL=1 \
    -DFSL_ETH_ENABLE_CACHE_CONTROL=1 \
    -DMCUXPRESSO_SDK \
    -DLWIP_DISABLE_PBUF_POOL_SIZE_SANITY_CHECKS=1 \
    -DCHECKSUM_GEN_IP=1 \
    -DCHECKSUM_GEN_UDP=1 \
    -DCHECKSUM_GEN_TCP=1 \
    -DCHECKSUM_GEN_ICMP=1 \
    -DCHECKSUM_GEN_ICMP6=1 \
    -DCHECKSUM_CHECK_ICMP=1 \
    -DCHECKSUM_CHECK_ICMP6=1 \
    -DLWIP_TIMEVAL_PRIVATE=0 \
    -DSERIAL_PORT_TYPE_UART=1 \
    -DSDK_OS_FREE_RTOS \
    -g \
    -O0 \
    -O1 \
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
SET(CMAKE_C_FLAGS_HYPERRAM_RELEASE " \
    ${CMAKE_C_FLAGS_HYPERRAM_RELEASE} \
    -DDATA_SECTION_IS_CACHEABLE=1 \
    -DUSE_HYPERRAM \
    -DNDEBUG \
    -DCPU_MIMXRT1189CVM8B_cm7 \
    -DUSE_RTOS=1 \
    -DPHY_RTL8201=1 \
    -DFSL_SDK_ENABLE_DRIVER_CACHE_CONTROL=1 \
    -DFSL_ETH_ENABLE_CACHE_CONTROL=1 \
    -DMCUXPRESSO_SDK \
    -DLWIP_DISABLE_PBUF_POOL_SIZE_SANITY_CHECKS=1 \
    -DCHECKSUM_GEN_IP=1 \
    -DCHECKSUM_GEN_UDP=1 \
    -DCHECKSUM_GEN_TCP=1 \
    -DCHECKSUM_GEN_ICMP=1 \
    -DCHECKSUM_GEN_ICMP6=1 \
    -DCHECKSUM_CHECK_ICMP=1 \
    -DCHECKSUM_CHECK_ICMP6=1 \
    -DLWIP_TIMEVAL_PRIVATE=0 \
    -DSERIAL_PORT_TYPE_UART=1 \
    -DSDK_OS_FREE_RTOS \
    -Os \
    -O1 \
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
SET(CMAKE_CXX_FLAGS_HYPERRAM_DEBUG " \
    ${CMAKE_CXX_FLAGS_HYPERRAM_DEBUG} \
    -DDEBUG \
    -DCPU_MIMXRT1189CVM8B_cm7 \
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
SET(CMAKE_CXX_FLAGS_HYPERRAM_RELEASE " \
    ${CMAKE_CXX_FLAGS_HYPERRAM_RELEASE} \
    -DNDEBUG \
    -DCPU_MIMXRT1189CVM8B_cm7 \
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
SET(CMAKE_EXE_LINKER_FLAGS_HYPERRAM_DEBUG " \
    ${CMAKE_EXE_LINKER_FLAGS_HYPERRAM_DEBUG} \
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
    -Xlinker \
    --defsym=__stack_size__=0x800 \
    -Xlinker \
    --defsym=__heap_size__=0x7000 \
    ${FPU} \
    ${SPECS} \
    -T\"${ProjDirPath}/MIMXRT1189xxxxx_cm7_hyperram.ld\" -static \
")
SET(CMAKE_EXE_LINKER_FLAGS_HYPERRAM_RELEASE " \
    ${CMAKE_EXE_LINKER_FLAGS_HYPERRAM_RELEASE} \
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
    -Xlinker \
    --defsym=__stack_size__=0x800 \
    -Xlinker \
    --defsym=__heap_size__=0x7000 \
    ${FPU} \
    ${SPECS} \
    -T\"${ProjDirPath}/MIMXRT1189xxxxx_cm7_hyperram.ld\" -static \
")
