IF(NOT DEFINED FPU)  
    SET(FPU "-mfloat-abi=hard -mfpu=fpv5-d16")  
ENDIF()  

IF(NOT DEFINED SPECS)  
    SET(SPECS "--specs=nano.specs --specs=nosys.specs")  
ENDIF()  

IF(NOT DEFINED DEBUG_CONSOLE_CONFIG)  
    SET(DEBUG_CONSOLE_CONFIG "-DSDK_DEBUGCONSOLE_UART")  
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
    -DXIP_EXTERNAL_FLASH=1 \
    -DXIP_BOOT_HEADER_ENABLE=1 \
    -DDEBUG \
    -DCPU_MIMXRT1176DVMAA_cm7 \
    -DIMX_RT \
    -DSIMW_DEMO_ENABLE__DEMO_SE05X_MINIMAL \
    -DPRINTF_ADVANCED_ENABLE=1 \
    -DCRYPTO_USE_DRIVER_CAAM \
    -DCACHE_MODE_WRITE_THROUGH=1 \
    -DMBEDTLS \
    -DSCP_MODE=C_MAC_C_ENC_R_MAC_R_ENC \
    -DSERIAL_PORT_TYPE_UART=1 \
    -DT1oI2C \
    -DT1oI2C_UM11225 \
    -DSSS_USE_FTR_FILE \
    -DNO_SECURE_CHANNEL_SUPPORT \
    -DMCUXPRESSO_SDK \
    -g \
    -O0 \
    -fomit-frame-pointer \
    -Wno-unused-function \
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
    -DXIP_EXTERNAL_FLASH=1 \
    -DXIP_BOOT_HEADER_ENABLE=1 \
    -DNDEBUG \
    -DCPU_MIMXRT1176DVMAA_cm7 \
    -DIMX_RT \
    -DSIMW_DEMO_ENABLE__DEMO_SE05X_MINIMAL \
    -DPRINTF_ADVANCED_ENABLE=1 \
    -DCRYPTO_USE_DRIVER_CAAM \
    -DCACHE_MODE_WRITE_THROUGH=1 \
    -DMBEDTLS \
    -DSCP_MODE=C_MAC_C_ENC_R_MAC_R_ENC \
    -DSERIAL_PORT_TYPE_UART=1 \
    -DT1oI2C \
    -DT1oI2C_UM11225 \
    -DSSS_USE_FTR_FILE \
    -DNO_SECURE_CHANNEL_SUPPORT \
    -DMCUXPRESSO_SDK \
    -Os \
    -fomit-frame-pointer \
    -Wno-unused-function \
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
    -DCPU_MIMXRT1176DVMAA_cm7 \
    -DSERIAL_PORT_TYPE_UART=1 \
    -DT1oI2C \
    -DT1oI2C_UM11225 \
    -DMCUXPRESSO_SDK \
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
    -DCPU_MIMXRT1176DVMAA_cm7 \
    -DSERIAL_PORT_TYPE_UART=1 \
    -DT1oI2C \
    -DT1oI2C_UM11225 \
    -DMCUXPRESSO_SDK \
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
    -Xlinker \
    --defsym=__stack_size__=0x2000 \
    -Xlinker \
    --defsym=__heap_size__=0x8000 \
    ${FPU} \
    ${SPECS} \
    -T${ProjDirPath}/MIMXRT1176xxxxx_cm7_flexspi_nor.ld -static \
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
    -Xlinker \
    --defsym=__stack_size__=0x2000 \
    -Xlinker \
    --defsym=__heap_size__=0x8000 \
    ${FPU} \
    ${SPECS} \
    -T${ProjDirPath}/MIMXRT1176xxxxx_cm7_flexspi_nor.ld -static \
")
