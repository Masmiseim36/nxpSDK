IF(NOT DEFINED FPU)  
    SET(FPU "-mfloat-abi=hard -mfpu=fpv5-d16")  
ENDIF()  

IF(NOT DEFINED SPECS)  
    SET(SPECS "--specs=nano.specs --specs=nosys.specs")  
ENDIF()  

IF(NOT DEFINED DEBUG_CONSOLE_CONFIG)  
    SET(DEBUG_CONSOLE_CONFIG "-DSDK_DEBUGCONSOLE=1")  
ENDIF()  

SET(CMAKE_ASM_FLAGS_RELEASE " \
    ${CMAKE_ASM_FLAGS_RELEASE} \
    -D__STARTUP_CLEAR_BSS \
    -DNDEBUG \
    -DCPU_MIMXRT1062DVL6B \
    -DMIMXRT1062_SERIES \
    -DOSA_USED \
    -DDEBUG \
    -mthumb \
    -mcpu=cortex-m7 \
    ${FPU} \
")
SET(CMAKE_C_FLAGS_RELEASE " \
    ${CMAKE_C_FLAGS_RELEASE} \
    -include ${ProjDirPath}/../mcux_config.h \
    -DNDEBUG \
    -D__STARTUP_CLEAR_BSS \
    -DCPU_MIMXRT1062DVL6A \
    -DCPU_IS_ARM_CORTEX_M7=1 \
    -DBL_TARGET_RAM \
    -DMIMXRT1062 \
    -DUSB_STACK_BM \
    -DPINT_USE_SIMPLE_CALLBACK=1 \
    -DCPU_MIMXRT1062DVL6B \
    -DMIMXRT1062_SERIES \
    -DOSA_USED \
    -DDEBUG \
    -DUSE_RTOS=0 \
    -Wall \
    -fno-common \
    -ffunction-sections \
    -fdata-sections \
    -fno-builtin \
    -mthumb \
    -mapcs \
    -std=gnu99 \
    -mcpu=cortex-m7 \
    -ffreestanding \
    -g \
    -O0 \
    -Os \
    -Wno-unused-variable \
    -Wno-unused-function \
    -Wno-maybe-uninitialized \
    -Wno-missing-braces \
    -MMD \
    -MP \
    ${FPU} \
    ${DEBUG_CONSOLE_CONFIG} \
")
SET(CMAKE_CXX_FLAGS_RELEASE " \
    ${CMAKE_CXX_FLAGS_RELEASE} \
    -DNDEBUG \
    -DCPU_MIMXRT1062DVL6B \
    -DMIMXRT1062_SERIES \
    -DOSA_USED \
    -DDEBUG \
    -DUSE_RTOS=0 \
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
    -ffreestanding \
    -g \
    -O0 \
    -Os \
    -MMD \
    -MP \
    ${FPU} \
    ${DEBUG_CONSOLE_CONFIG} \
")
SET(CMAKE_EXE_LINKER_FLAGS_RELEASE " \
    ${CMAKE_EXE_LINKER_FLAGS_RELEASE} \
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
    -Wl,--print-memory-usage \
    -mcpu=cortex-m7 \
    ${FPU} \
    ${SPECS} \
    -T\"${ProjDirPath}/MIMXRT1062_ram_flashloader.ld\" -static \
")
