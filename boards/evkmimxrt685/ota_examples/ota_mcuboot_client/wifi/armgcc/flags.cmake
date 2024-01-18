IF(NOT DEFINED FPU)  
    SET(FPU "-mfloat-abi=hard -mfpu=fpv5-sp-d16")  
ENDIF()  

IF(NOT DEFINED SPECS)  
    SET(SPECS "--specs=nano.specs --specs=nosys.specs")  
ENDIF()  

IF(NOT DEFINED DEBUG_CONSOLE_CONFIG)  
    SET(DEBUG_CONSOLE_CONFIG "-DSDK_DEBUGCONSOLE=1")  
ENDIF()  

SET(CMAKE_ASM_FLAGS_FLASH_DEBUG " \
    ${CMAKE_ASM_FLAGS_FLASH_DEBUG} \
    -DDEBUG \
    -D__STARTUP_CLEAR_BSS \
    -mcpu=cortex-m33 \
    -mthumb \
    ${FPU} \
")
SET(CMAKE_ASM_FLAGS_FLASH_RELEASE " \
    ${CMAKE_ASM_FLAGS_FLASH_RELEASE} \
    -DNDEBUG \
    -D__STARTUP_CLEAR_BSS \
    -mcpu=cortex-m33 \
    -mthumb \
    ${FPU} \
")
SET(CMAKE_C_FLAGS_FLASH_DEBUG " \
    ${CMAKE_C_FLAGS_FLASH_DEBUG} \
    -include ${ProjDirPath}/../app_config.h \
    -DDEBUG \
    -DFSL_SDK_DRIVER_QUICK_ACCESS_ENABLE=1 \
    -D_POSIX_SOURCE \
    -DCPU_MIMXRT685SFVKB_cm33 \
    -DBOOT_HEADER_ENABLE=1 \
    -DXIP_IMAGE \
    -DXIP_EXTERNAL_FLASH \
    -DHIGH_SPEED_SDIO_CLOCK \
    -DMBEDTLS_CCM_ALT \
    -DPRINTF_ADVANCED_ENABLE=1 \
    -DMCUBOOT_APPLICATION=1 \
    -DUSE_RTOS=1 \
    -DLWIP_DNS=1 \
    -DLWIP_DHCP=1 \
    -DENET_RXBUFF_NUM=14 \
    -DSHELL_BUFFER_SIZE=128U \
    -DMCUXPRESSO_SDK \
    -DMFLASH_FILE_BASEADDR=7340032 \
    -DSDK_I2C_BASED_COMPONENT_USED=1 \
    -DSDIO_ENABLED \
    -DLWIP_TIMEVAL_PRIVATE=0 \
    -DDEBUG_CONSOLE_RX_ENABLE=0 \
    -DSERIAL_PORT_TYPE_UART=1 \
    -DSDK_OS_FREE_RTOS \
    -DLWIP_NETIF_HOSTNAME=1 \
    -DLWIP_NETIF_STATUS_CALLBACK=1 \
    -DLWIP_IGMP=1 \
    -g \
    -O0 \
    -mcpu=cortex-m33 \
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
    -fomit-frame-pointer \
    -Wno-unused-function \
    ${FPU} \
    ${DEBUG_CONSOLE_CONFIG} \
")
SET(CMAKE_C_FLAGS_FLASH_RELEASE " \
    ${CMAKE_C_FLAGS_FLASH_RELEASE} \
    -include ${ProjDirPath}/../app_config.h \
    -DNDEBUG \
    -DFSL_SDK_DRIVER_QUICK_ACCESS_ENABLE=1 \
    -D_POSIX_SOURCE \
    -DCPU_MIMXRT685SFVKB_cm33 \
    -DBOOT_HEADER_ENABLE=1 \
    -DXIP_IMAGE \
    -DXIP_EXTERNAL_FLASH \
    -DHIGH_SPEED_SDIO_CLOCK \
    -DMBEDTLS_CCM_ALT \
    -DPRINTF_ADVANCED_ENABLE=1 \
    -DMCUBOOT_APPLICATION=1 \
    -DUSE_RTOS=1 \
    -DLWIP_DNS=1 \
    -DLWIP_DHCP=1 \
    -DENET_RXBUFF_NUM=14 \
    -DSHELL_BUFFER_SIZE=128U \
    -DMCUXPRESSO_SDK \
    -DMFLASH_FILE_BASEADDR=7340032 \
    -DSDK_I2C_BASED_COMPONENT_USED=1 \
    -DSDIO_ENABLED \
    -DLWIP_TIMEVAL_PRIVATE=0 \
    -DDEBUG_CONSOLE_RX_ENABLE=0 \
    -DSERIAL_PORT_TYPE_UART=1 \
    -DSDK_OS_FREE_RTOS \
    -DLWIP_NETIF_HOSTNAME=1 \
    -DLWIP_NETIF_STATUS_CALLBACK=1 \
    -DLWIP_IGMP=1 \
    -Os \
    -mcpu=cortex-m33 \
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
    -fomit-frame-pointer \
    -Wno-unused-function \
    ${FPU} \
    ${DEBUG_CONSOLE_CONFIG} \
")
SET(CMAKE_CXX_FLAGS_FLASH_DEBUG " \
    ${CMAKE_CXX_FLAGS_FLASH_DEBUG} \
    -DDEBUG \
    -DCPU_MIMXRT685SFVKB_cm33 \
    -DBOOT_HEADER_ENABLE=1 \
    -DMCUXPRESSO_SDK \
    -DSERIAL_PORT_TYPE_UART=1 \
    -g \
    -O0 \
    -mcpu=cortex-m33 \
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
SET(CMAKE_CXX_FLAGS_FLASH_RELEASE " \
    ${CMAKE_CXX_FLAGS_FLASH_RELEASE} \
    -DNDEBUG \
    -DCPU_MIMXRT685SFVKB_cm33 \
    -DBOOT_HEADER_ENABLE=1 \
    -DMCUXPRESSO_SDK \
    -DSERIAL_PORT_TYPE_UART=1 \
    -Os \
    -mcpu=cortex-m33 \
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
SET(CMAKE_EXE_LINKER_FLAGS_FLASH_DEBUG " \
    ${CMAKE_EXE_LINKER_FLAGS_FLASH_DEBUG} \
    -g \
    -mcpu=cortex-m33 \
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
    --defsym=__stack_size__=0x200 \
    -Xlinker \
    --defsym=__heap_size__=0x200 \
    ${FPU} \
    ${SPECS} \
    -T\"${ProjDirPath}/linker/MIMXRT685Sxxxx_cm33_flash_mcuboot.ld\" -static \
")
SET(CMAKE_EXE_LINKER_FLAGS_FLASH_RELEASE " \
    ${CMAKE_EXE_LINKER_FLAGS_FLASH_RELEASE} \
    -mcpu=cortex-m33 \
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
    --defsym=__stack_size__=0x200 \
    -Xlinker \
    --defsym=__heap_size__=0x200 \
    ${FPU} \
    ${SPECS} \
    -T\"${ProjDirPath}/linker/MIMXRT685Sxxxx_cm33_flash_mcuboot.ld\" -static \
")
