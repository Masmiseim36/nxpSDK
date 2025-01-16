IF(NOT DEFINED FPU)  
    SET(FPU "-mfloat-abi=hard -mfpu=fpv5-sp-d16")  
ENDIF()  

IF(NOT DEFINED SPECS)  
    SET(SPECS "--specs=nano.specs --specs=nosys.specs")  
ENDIF()  

IF(NOT DEFINED DEBUG_CONSOLE_CONFIG)  
    SET(DEBUG_CONSOLE_CONFIG "-DSDK_DEBUGCONSOLE=1")  
ENDIF()  

SET(CMAKE_ASM_FLAGS_DEBUG " \
    ${CMAKE_ASM_FLAGS_DEBUG} \
    -D__STARTUP_INITIALIZE_NONCACHEDATA \
    -D__STARTUP_CLEAR_BSS \
    -DMCUXPRESSO_SDK \
    -DCPU_MIMXRT1189CVM8B_cm33 \
    -DDEBUG_CONSOLE_TRANSFER_NON_BLOCKING \
    -g \
    -mthumb \
    -mcpu=cortex-m33 \
    ${FPU} \
")
SET(CMAKE_ASM_FLAGS_HYPERRAM_RELEASE " \
    ${CMAKE_ASM_FLAGS_HYPERRAM_RELEASE} \
    -D__STARTUP_INITIALIZE_QADATA \
    -D__STARTUP_INITIALIZE_NONCACHEDATA \
    -D__STARTUP_CLEAR_BSS \
    -DMCUXPRESSO_SDK \
    -DCPU_MIMXRT1189CVM8B_cm33 \
    -DDEBUG_CONSOLE_TRANSFER_NON_BLOCKING \
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
    -g \
    -mthumb \
    -mcpu=cortex-m33 \
    ${FPU} \
")
SET(CMAKE_ASM_FLAGS_FLEXSPI_NOR_RELEASE " \
    ${CMAKE_ASM_FLAGS_FLEXSPI_NOR_RELEASE} \
    -D__STARTUP_INITIALIZE_RAMFUNCTION \
    -D__STARTUP_INITIALIZE_NONCACHEDATA \
    -D__STARTUP_CLEAR_BSS \
    -DMCUXPRESSO_SDK \
    -DCPU_MIMXRT1189CVM8B_cm33 \
    -DDEBUG_CONSOLE_TRANSFER_NON_BLOCKING \
    -mthumb \
    -mcpu=cortex-m33 \
    ${FPU} \
")
SET(CMAKE_ASM_FLAGS_RELEASE " \
    ${CMAKE_ASM_FLAGS_RELEASE} \
    -D__STARTUP_INITIALIZE_NONCACHEDATA \
    -D__STARTUP_CLEAR_BSS \
    -DMCUXPRESSO_SDK \
    -DCPU_MIMXRT1189CVM8B_cm33 \
    -DDEBUG_CONSOLE_TRANSFER_NON_BLOCKING \
    -mthumb \
    -mcpu=cortex-m33 \
    ${FPU} \
")
SET(CMAKE_C_FLAGS_DEBUG " \
    ${CMAKE_C_FLAGS_DEBUG} \
    -include ${ProjDirPath}/../mcux_config.h \
    -DDEBUG \
    -DPRINTF_ADVANCED_ENABLE=1 \
    -DDEBUG_CONSOLE_TRANSFER_NON_BLOCKING \
    -DFSL_ETH_ENABLE_CACHE_CONTROL \
    -DETH_MAX_RX_PKTS_AT_ONCE=8 \
    -DNETC_RXBD_NUM=8 \
    -DNETC_RXBUFF_NUM=16 \
    -DTCP_WND=5840 \
    -DENET_RXBUFF_NUM=14 \
    -DENET_RXBD_NUM=9 \
    -DMCUX_META_BUILD \
    -DMCUXPRESSO_SDK \
    -DCPU_MIMXRT1189CVM8B_cm33 \
    -DSERIAL_PORT_TYPE_UART=1 \
    -DSO_REUSE=1 \
    -DPRINTF_FLOAT_ENABLE=1 \
    -DLWIP_DISABLE_PBUF_POOL_SIZE_SANITY_CHECKS=1 \
    -DLWIP_SUPPORT_CUSTOM_PBUF=1 \
    -DCHECKSUM_GEN_IP=1 \
    -DCHECKSUM_GEN_UDP=1 \
    -DCHECKSUM_GEN_TCP=1 \
    -DCHECKSUM_GEN_ICMP=1 \
    -DCHECKSUM_GEN_ICMP6=1 \
    -DCHECKSUM_CHECK_UDP=1 \
    -DCHECKSUM_CHECK_TCP=1 \
    -DCHECKSUM_CHECK_ICMP=1 \
    -DCHECKSUM_CHECK_ICMP6=1 \
    -Og \
    -g \
    -Wall \
    -fno-common \
    -ffunction-sections \
    -fdata-sections \
    -fno-builtin \
    -mthumb \
    -mapcs \
    -std=gnu99 \
    -mcpu=cortex-m33 \
    ${FPU} \
    ${DEBUG_CONSOLE_CONFIG} \
")
SET(CMAKE_C_FLAGS_HYPERRAM_RELEASE " \
    ${CMAKE_C_FLAGS_HYPERRAM_RELEASE} \
    -include ${ProjDirPath}/../mcux_config.h \
    -DNDEBUG \
    -DUSE_HYPERRAM=1 \
    -DPRINTF_ADVANCED_ENABLE=1 \
    -DDEBUG_CONSOLE_TRANSFER_NON_BLOCKING \
    -DFSL_ETH_ENABLE_CACHE_CONTROL \
    -DETH_MAX_RX_PKTS_AT_ONCE=8 \
    -DNETC_RXBD_NUM=8 \
    -DNETC_RXBUFF_NUM=16 \
    -DTCP_WND=5840 \
    -DENET_RXBUFF_NUM=14 \
    -DENET_RXBD_NUM=9 \
    -DMCUX_META_BUILD \
    -DMCUXPRESSO_SDK \
    -DCPU_MIMXRT1189CVM8B_cm33 \
    -DSERIAL_PORT_TYPE_UART=1 \
    -DSO_REUSE=1 \
    -DPRINTF_FLOAT_ENABLE=1 \
    -DLWIP_DISABLE_PBUF_POOL_SIZE_SANITY_CHECKS=1 \
    -DLWIP_SUPPORT_CUSTOM_PBUF=1 \
    -DCHECKSUM_GEN_IP=1 \
    -DCHECKSUM_GEN_UDP=1 \
    -DCHECKSUM_GEN_TCP=1 \
    -DCHECKSUM_GEN_ICMP=1 \
    -DCHECKSUM_GEN_ICMP6=1 \
    -DCHECKSUM_CHECK_UDP=1 \
    -DCHECKSUM_CHECK_TCP=1 \
    -DCHECKSUM_CHECK_ICMP=1 \
    -DCHECKSUM_CHECK_ICMP6=1 \
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
    ${FPU} \
    ${DEBUG_CONSOLE_CONFIG} \
")
SET(CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG " \
    ${CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG} \
    -include ${ProjDirPath}/../mcux_config.h \
    -DDEBUG \
    -DXIP_EXTERNAL_FLASH=1 \
    -DXIP_BOOT_HEADER_ENABLE=0 \
    -DPRINTF_ADVANCED_ENABLE=1 \
    -DDEBUG_CONSOLE_TRANSFER_NON_BLOCKING \
    -DFSL_ETH_ENABLE_CACHE_CONTROL \
    -DETH_MAX_RX_PKTS_AT_ONCE=8 \
    -DNETC_RXBD_NUM=8 \
    -DNETC_RXBUFF_NUM=16 \
    -DTCP_WND=5840 \
    -DENET_RXBUFF_NUM=14 \
    -DENET_RXBD_NUM=9 \
    -DMCUX_META_BUILD \
    -DMCUXPRESSO_SDK \
    -DCPU_MIMXRT1189CVM8B_cm33 \
    -DSERIAL_PORT_TYPE_UART=1 \
    -DSO_REUSE=1 \
    -DPRINTF_FLOAT_ENABLE=1 \
    -DLWIP_DISABLE_PBUF_POOL_SIZE_SANITY_CHECKS=1 \
    -DLWIP_SUPPORT_CUSTOM_PBUF=1 \
    -DCHECKSUM_GEN_IP=1 \
    -DCHECKSUM_GEN_UDP=1 \
    -DCHECKSUM_GEN_TCP=1 \
    -DCHECKSUM_GEN_ICMP=1 \
    -DCHECKSUM_GEN_ICMP6=1 \
    -DCHECKSUM_CHECK_UDP=1 \
    -DCHECKSUM_CHECK_TCP=1 \
    -DCHECKSUM_CHECK_ICMP=1 \
    -DCHECKSUM_CHECK_ICMP6=1 \
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
    ${FPU} \
    ${DEBUG_CONSOLE_CONFIG} \
")
SET(CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE " \
    ${CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE} \
    -include ${ProjDirPath}/../mcux_config.h \
    -DNDEBUG \
    -DXIP_EXTERNAL_FLASH=1 \
    -DXIP_BOOT_HEADER_ENABLE=0 \
    -DPRINTF_ADVANCED_ENABLE=1 \
    -DDEBUG_CONSOLE_TRANSFER_NON_BLOCKING \
    -DFSL_ETH_ENABLE_CACHE_CONTROL \
    -DETH_MAX_RX_PKTS_AT_ONCE=8 \
    -DNETC_RXBD_NUM=8 \
    -DNETC_RXBUFF_NUM=16 \
    -DTCP_WND=5840 \
    -DENET_RXBUFF_NUM=14 \
    -DENET_RXBD_NUM=9 \
    -DMCUX_META_BUILD \
    -DMCUXPRESSO_SDK \
    -DCPU_MIMXRT1189CVM8B_cm33 \
    -DSERIAL_PORT_TYPE_UART=1 \
    -DSO_REUSE=1 \
    -DPRINTF_FLOAT_ENABLE=1 \
    -DLWIP_DISABLE_PBUF_POOL_SIZE_SANITY_CHECKS=1 \
    -DLWIP_SUPPORT_CUSTOM_PBUF=1 \
    -DCHECKSUM_GEN_IP=1 \
    -DCHECKSUM_GEN_UDP=1 \
    -DCHECKSUM_GEN_TCP=1 \
    -DCHECKSUM_GEN_ICMP=1 \
    -DCHECKSUM_GEN_ICMP6=1 \
    -DCHECKSUM_CHECK_UDP=1 \
    -DCHECKSUM_CHECK_TCP=1 \
    -DCHECKSUM_CHECK_ICMP=1 \
    -DCHECKSUM_CHECK_ICMP6=1 \
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
    ${FPU} \
    ${DEBUG_CONSOLE_CONFIG} \
")
SET(CMAKE_C_FLAGS_RELEASE " \
    ${CMAKE_C_FLAGS_RELEASE} \
    -include ${ProjDirPath}/../mcux_config.h \
    -DNDEBUG \
    -DPRINTF_ADVANCED_ENABLE=1 \
    -DDEBUG_CONSOLE_TRANSFER_NON_BLOCKING \
    -DFSL_ETH_ENABLE_CACHE_CONTROL \
    -DETH_MAX_RX_PKTS_AT_ONCE=8 \
    -DNETC_RXBD_NUM=8 \
    -DNETC_RXBUFF_NUM=16 \
    -DTCP_WND=5840 \
    -DENET_RXBUFF_NUM=14 \
    -DENET_RXBD_NUM=9 \
    -DMCUX_META_BUILD \
    -DMCUXPRESSO_SDK \
    -DCPU_MIMXRT1189CVM8B_cm33 \
    -DSERIAL_PORT_TYPE_UART=1 \
    -DSO_REUSE=1 \
    -DPRINTF_FLOAT_ENABLE=1 \
    -DLWIP_DISABLE_PBUF_POOL_SIZE_SANITY_CHECKS=1 \
    -DLWIP_SUPPORT_CUSTOM_PBUF=1 \
    -DCHECKSUM_GEN_IP=1 \
    -DCHECKSUM_GEN_UDP=1 \
    -DCHECKSUM_GEN_TCP=1 \
    -DCHECKSUM_GEN_ICMP=1 \
    -DCHECKSUM_GEN_ICMP6=1 \
    -DCHECKSUM_CHECK_UDP=1 \
    -DCHECKSUM_CHECK_TCP=1 \
    -DCHECKSUM_CHECK_ICMP=1 \
    -DCHECKSUM_CHECK_ICMP6=1 \
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
    ${FPU} \
    ${DEBUG_CONSOLE_CONFIG} \
")
SET(CMAKE_CXX_FLAGS_DEBUG " \
    ${CMAKE_CXX_FLAGS_DEBUG} \
    -DDEBUG \
    -DMCUX_META_BUILD \
    -DMCUXPRESSO_SDK \
    -DCPU_MIMXRT1189CVM8B_cm33 \
    -DDEBUG_CONSOLE_TRANSFER_NON_BLOCKING \
    -DSERIAL_PORT_TYPE_UART=1 \
    -DSO_REUSE=1 \
    -DPRINTF_FLOAT_ENABLE=1 \
    -DLWIP_DISABLE_PBUF_POOL_SIZE_SANITY_CHECKS=1 \
    -DLWIP_SUPPORT_CUSTOM_PBUF=1 \
    -DCHECKSUM_GEN_IP=1 \
    -DCHECKSUM_GEN_UDP=1 \
    -DCHECKSUM_GEN_TCP=1 \
    -DCHECKSUM_GEN_ICMP=1 \
    -DCHECKSUM_GEN_ICMP6=1 \
    -DCHECKSUM_CHECK_UDP=1 \
    -DCHECKSUM_CHECK_TCP=1 \
    -DCHECKSUM_CHECK_ICMP=1 \
    -DCHECKSUM_CHECK_ICMP6=1 \
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
SET(CMAKE_CXX_FLAGS_HYPERRAM_RELEASE " \
    ${CMAKE_CXX_FLAGS_HYPERRAM_RELEASE} \
    -DNDEBUG \
    -DUSE_HYPERRAM=1 \
    -DMCUX_META_BUILD \
    -DMCUXPRESSO_SDK \
    -DCPU_MIMXRT1189CVM8B_cm33 \
    -DDEBUG_CONSOLE_TRANSFER_NON_BLOCKING \
    -DSERIAL_PORT_TYPE_UART=1 \
    -DSO_REUSE=1 \
    -DPRINTF_FLOAT_ENABLE=1 \
    -DLWIP_DISABLE_PBUF_POOL_SIZE_SANITY_CHECKS=1 \
    -DLWIP_SUPPORT_CUSTOM_PBUF=1 \
    -DCHECKSUM_GEN_IP=1 \
    -DCHECKSUM_GEN_UDP=1 \
    -DCHECKSUM_GEN_TCP=1 \
    -DCHECKSUM_GEN_ICMP=1 \
    -DCHECKSUM_GEN_ICMP6=1 \
    -DCHECKSUM_CHECK_UDP=1 \
    -DCHECKSUM_CHECK_TCP=1 \
    -DCHECKSUM_CHECK_ICMP=1 \
    -DCHECKSUM_CHECK_ICMP6=1 \
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
    -DDEBUG_CONSOLE_TRANSFER_NON_BLOCKING \
    -DSERIAL_PORT_TYPE_UART=1 \
    -DSO_REUSE=1 \
    -DPRINTF_FLOAT_ENABLE=1 \
    -DLWIP_DISABLE_PBUF_POOL_SIZE_SANITY_CHECKS=1 \
    -DLWIP_SUPPORT_CUSTOM_PBUF=1 \
    -DCHECKSUM_GEN_IP=1 \
    -DCHECKSUM_GEN_UDP=1 \
    -DCHECKSUM_GEN_TCP=1 \
    -DCHECKSUM_GEN_ICMP=1 \
    -DCHECKSUM_GEN_ICMP6=1 \
    -DCHECKSUM_CHECK_UDP=1 \
    -DCHECKSUM_CHECK_TCP=1 \
    -DCHECKSUM_CHECK_ICMP=1 \
    -DCHECKSUM_CHECK_ICMP6=1 \
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
SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_RELEASE " \
    ${CMAKE_CXX_FLAGS_FLEXSPI_NOR_RELEASE} \
    -DNDEBUG \
    -DXIP_EXTERNAL_FLASH=1 \
    -DMCUX_META_BUILD \
    -DMCUXPRESSO_SDK \
    -DCPU_MIMXRT1189CVM8B_cm33 \
    -DDEBUG_CONSOLE_TRANSFER_NON_BLOCKING \
    -DSERIAL_PORT_TYPE_UART=1 \
    -DSO_REUSE=1 \
    -DPRINTF_FLOAT_ENABLE=1 \
    -DLWIP_DISABLE_PBUF_POOL_SIZE_SANITY_CHECKS=1 \
    -DLWIP_SUPPORT_CUSTOM_PBUF=1 \
    -DCHECKSUM_GEN_IP=1 \
    -DCHECKSUM_GEN_UDP=1 \
    -DCHECKSUM_GEN_TCP=1 \
    -DCHECKSUM_GEN_ICMP=1 \
    -DCHECKSUM_GEN_ICMP6=1 \
    -DCHECKSUM_CHECK_UDP=1 \
    -DCHECKSUM_CHECK_TCP=1 \
    -DCHECKSUM_CHECK_ICMP=1 \
    -DCHECKSUM_CHECK_ICMP6=1 \
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
SET(CMAKE_CXX_FLAGS_RELEASE " \
    ${CMAKE_CXX_FLAGS_RELEASE} \
    -DNDEBUG \
    -DMCUX_META_BUILD \
    -DMCUXPRESSO_SDK \
    -DCPU_MIMXRT1189CVM8B_cm33 \
    -DDEBUG_CONSOLE_TRANSFER_NON_BLOCKING \
    -DSERIAL_PORT_TYPE_UART=1 \
    -DSO_REUSE=1 \
    -DPRINTF_FLOAT_ENABLE=1 \
    -DLWIP_DISABLE_PBUF_POOL_SIZE_SANITY_CHECKS=1 \
    -DLWIP_SUPPORT_CUSTOM_PBUF=1 \
    -DCHECKSUM_GEN_IP=1 \
    -DCHECKSUM_GEN_UDP=1 \
    -DCHECKSUM_GEN_TCP=1 \
    -DCHECKSUM_GEN_ICMP=1 \
    -DCHECKSUM_GEN_ICMP6=1 \
    -DCHECKSUM_CHECK_UDP=1 \
    -DCHECKSUM_CHECK_TCP=1 \
    -DCHECKSUM_CHECK_ICMP=1 \
    -DCHECKSUM_CHECK_ICMP6=1 \
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
SET(CMAKE_EXE_LINKER_FLAGS_DEBUG " \
    ${CMAKE_EXE_LINKER_FLAGS_DEBUG} \
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
    -mcpu=cortex-m33 \
    ${FPU} \
    ${SPECS} \
    -T\"${ProjDirPath}/MIMXRT1189xxxxx_cm33_ram.ld\" -static \
")
SET(CMAKE_EXE_LINKER_FLAGS_HYPERRAM_RELEASE " \
    ${CMAKE_EXE_LINKER_FLAGS_HYPERRAM_RELEASE} \
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
    -mcpu=cortex-m33 \
    ${FPU} \
    ${SPECS} \
    -T\"${ProjDirPath}/MIMXRT1189xxxxx_cm33_hyperram.ld\" -static \
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
    -mcpu=cortex-m33 \
    ${FPU} \
    ${SPECS} \
    -T\"${ProjDirPath}/MIMXRT1189xxxxx_cm33_flexspi_nor.ld\" -static \
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
    -mcpu=cortex-m33 \
    ${FPU} \
    ${SPECS} \
    -T\"${ProjDirPath}/MIMXRT1189xxxxx_cm33_flexspi_nor.ld\" -static \
")
SET(CMAKE_EXE_LINKER_FLAGS_RELEASE " \
    ${CMAKE_EXE_LINKER_FLAGS_RELEASE} \
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
    -mcpu=cortex-m33 \
    ${FPU} \
    ${SPECS} \
    -T\"${ProjDirPath}/MIMXRT1189xxxxx_cm33_ram.ld\" -static \
")
