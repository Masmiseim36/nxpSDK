IF(NOT DEFINED FPU)  
    SET(FPU "-mfloat-abi=hard -mfpu=fpv5-d16")  
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
    -DCPU_MIMXRT1062DVL6B \
    -DOSA_USED \
    -g \
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
    -DCPU_MIMXRT1062DVL6B \
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
    -DOSA_USED \
    -mthumb \
    -mcpu=cortex-m7 \
    ${FPU} \
")
SET(CMAKE_ASM_FLAGS_RELEASE " \
    ${CMAKE_ASM_FLAGS_RELEASE} \
    -D__STARTUP_INITIALIZE_NONCACHEDATA \
    -D__STARTUP_CLEAR_BSS \
    -DMCUXPRESSO_SDK \
    -DCPU_MIMXRT1062DVL6B \
    -DOSA_USED \
    -mthumb \
    -mcpu=cortex-m7 \
    ${FPU} \
")
SET(CMAKE_ASM_FLAGS_SDRAM_DEBUG " \
    ${CMAKE_ASM_FLAGS_SDRAM_DEBUG} \
    -D__STARTUP_INITIALIZE_QADATA \
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
SET(CMAKE_ASM_FLAGS_SDRAM_RELEASE " \
    ${CMAKE_ASM_FLAGS_SDRAM_RELEASE} \
    -D__STARTUP_INITIALIZE_QADATA \
    -D__STARTUP_INITIALIZE_NONCACHEDATA \
    -D__STARTUP_CLEAR_BSS \
    -DMCUXPRESSO_SDK \
    -DCPU_MIMXRT1062DVL6B \
    -DOSA_USED \
    -mthumb \
    -mcpu=cortex-m7 \
    ${FPU} \
")
SET(CMAKE_C_FLAGS_DEBUG " \
    ${CMAKE_C_FLAGS_DEBUG} \
    -include ${ProjDirPath}/../mcux_config.h \
    -DDEBUG \
    -DUSB_STACK_BM \
    -DMCUX_META_BUILD \
    -DMCUXPRESSO_SDK \
    -DCPU_MIMXRT1062DVL6B \
    -DOSA_USED \
    -DSERIAL_PORT_TYPE_UART=1 \
    -DUSE_RTOS=0 \
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
SET(CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG " \
    ${CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG} \
    -include ${ProjDirPath}/../mcux_config.h \
    -DDEBUG \
    -DXIP_BOOT_HEADER_ENABLE=1 \
    -DXIP_EXTERNAL_FLASH=1 \
    -DUSB_STACK_BM \
    -DMCUX_META_BUILD \
    -DMCUXPRESSO_SDK \
    -DCPU_MIMXRT1062DVL6B \
    -DOSA_USED \
    -DSERIAL_PORT_TYPE_UART=1 \
    -DUSE_RTOS=0 \
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
    -DUSB_STACK_BM \
    -DMCUX_META_BUILD \
    -DMCUXPRESSO_SDK \
    -DCPU_MIMXRT1062DVL6B \
    -DOSA_USED \
    -DSERIAL_PORT_TYPE_UART=1 \
    -DUSE_RTOS=0 \
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
SET(CMAKE_C_FLAGS_RELEASE " \
    ${CMAKE_C_FLAGS_RELEASE} \
    -include ${ProjDirPath}/../mcux_config.h \
    -DNDEBUG \
    -DUSB_STACK_BM \
    -DMCUX_META_BUILD \
    -DMCUXPRESSO_SDK \
    -DCPU_MIMXRT1062DVL6B \
    -DOSA_USED \
    -DSERIAL_PORT_TYPE_UART=1 \
    -DUSE_RTOS=0 \
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
SET(CMAKE_C_FLAGS_SDRAM_DEBUG " \
    ${CMAKE_C_FLAGS_SDRAM_DEBUG} \
    -include ${ProjDirPath}/../mcux_config.h \
    -DUSB_HOST_CONFIG_BUFFER_PROPERTY_CACHEABLE=1 \
    -DDEBUG \
    -DUSE_SDRAM=1 \
    -DSKIP_SYSCLK_INIT=1 \
    -DUSB_STACK_BM \
    -DMCUX_META_BUILD \
    -DMCUXPRESSO_SDK \
    -DCPU_MIMXRT1062DVL6B \
    -DOSA_USED \
    -DSERIAL_PORT_TYPE_UART=1 \
    -DUSE_RTOS=0 \
    -O1 \
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
SET(CMAKE_C_FLAGS_SDRAM_RELEASE " \
    ${CMAKE_C_FLAGS_SDRAM_RELEASE} \
    -include ${ProjDirPath}/../mcux_config.h \
    -DUSB_HOST_CONFIG_BUFFER_PROPERTY_CACHEABLE=1 \
    -DNDEBUG \
    -DUSE_SDRAM=1 \
    -DSKIP_SYSCLK_INIT=1 \
    -DUSB_STACK_BM \
    -DMCUX_META_BUILD \
    -DMCUXPRESSO_SDK \
    -DCPU_MIMXRT1062DVL6B \
    -DOSA_USED \
    -DSERIAL_PORT_TYPE_UART=1 \
    -DUSE_RTOS=0 \
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
SET(CMAKE_CXX_FLAGS_DEBUG " \
    ${CMAKE_CXX_FLAGS_DEBUG} \
    -DDEBUG \
    -DMCUX_META_BUILD \
    -DMCUXPRESSO_SDK \
    -DCPU_MIMXRT1062DVL6B \
    -DOSA_USED \
    -DSERIAL_PORT_TYPE_UART=1 \
    -DUSE_RTOS=0 \
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
SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_DEBUG " \
    ${CMAKE_CXX_FLAGS_FLEXSPI_NOR_DEBUG} \
    -DDEBUG \
    -DXIP_EXTERNAL_FLASH=1 \
    -DMCUX_META_BUILD \
    -DMCUXPRESSO_SDK \
    -DCPU_MIMXRT1062DVL6B \
    -DOSA_USED \
    -DSERIAL_PORT_TYPE_UART=1 \
    -DUSE_RTOS=0 \
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
    -DUSE_RTOS=0 \
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
SET(CMAKE_CXX_FLAGS_RELEASE " \
    ${CMAKE_CXX_FLAGS_RELEASE} \
    -DNDEBUG \
    -DMCUX_META_BUILD \
    -DMCUXPRESSO_SDK \
    -DCPU_MIMXRT1062DVL6B \
    -DOSA_USED \
    -DSERIAL_PORT_TYPE_UART=1 \
    -DUSE_RTOS=0 \
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
SET(CMAKE_CXX_FLAGS_SDRAM_DEBUG " \
    ${CMAKE_CXX_FLAGS_SDRAM_DEBUG} \
    -DDEBUG \
    -DUSE_SDRAM=1 \
    -DMCUX_META_BUILD \
    -DMCUXPRESSO_SDK \
    -DCPU_MIMXRT1062DVL6B \
    -DOSA_USED \
    -DSERIAL_PORT_TYPE_UART=1 \
    -DUSE_RTOS=0 \
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
SET(CMAKE_CXX_FLAGS_SDRAM_RELEASE " \
    ${CMAKE_CXX_FLAGS_SDRAM_RELEASE} \
    -DNDEBUG \
    -DUSE_SDRAM=1 \
    -DMCUX_META_BUILD \
    -DMCUXPRESSO_SDK \
    -DCPU_MIMXRT1062DVL6B \
    -DOSA_USED \
    -DSERIAL_PORT_TYPE_UART=1 \
    -DUSE_RTOS=0 \
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
    -mcpu=cortex-m7 \
    ${FPU} \
    ${SPECS} \
    -T\"${ProjDirPath}/MIMXRT1062xxxxx_ram.ld\" -static \
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
    -mcpu=cortex-m7 \
    ${FPU} \
    ${SPECS} \
    -T\"${ProjDirPath}/MIMXRT1062xxxxx_ram.ld\" -static \
")
SET(CMAKE_EXE_LINKER_FLAGS_SDRAM_DEBUG " \
    ${CMAKE_EXE_LINKER_FLAGS_SDRAM_DEBUG} \
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
    -T\"${ProjDirPath}/MIMXRT1062xxxxx_sdram.ld\" -static \
")
SET(CMAKE_EXE_LINKER_FLAGS_SDRAM_RELEASE " \
    ${CMAKE_EXE_LINKER_FLAGS_SDRAM_RELEASE} \
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
    -T\"${ProjDirPath}/MIMXRT1062xxxxx_sdram.ld\" -static \
")
