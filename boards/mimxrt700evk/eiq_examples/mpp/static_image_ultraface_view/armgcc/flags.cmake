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
    -D__STARTUP_INITIALIZE_NONCACHEDATA \
    -D__STARTUP_CLEAR_BSS \
    -DMCUXPRESSO_SDK \
    -DCPU_MIMXRT798SGFOA_cm33_core0 \
    -g \
    -mthumb \
    -mcpu=cortex-m33 \
    ${FPU} \
")
SET(CMAKE_ASM_FLAGS_FLASH_RELEASE " \
    ${CMAKE_ASM_FLAGS_FLASH_RELEASE} \
    -D__STARTUP_INITIALIZE_NONCACHEDATA \
    -D__STARTUP_CLEAR_BSS \
    -DMCUXPRESSO_SDK \
    -DCPU_MIMXRT798SGFOA_cm33_core0 \
    -mthumb \
    -mcpu=cortex-m33 \
    ${FPU} \
")
SET(CMAKE_C_FLAGS_FLASH_DEBUG " \
    ${CMAKE_C_FLAGS_FLASH_DEBUG} \
    -include ${ProjDirPath}/../mcux_config.h \
    -DDEBUG \
    -DGCID_REV_CID=gc555/0x423_ECO \
    -DMCUX_DBI_LEGACY=0 \
    -DDBI_USE_MIPI_PANEL=1 \
    -DSDK_I2C_BASED_COMPONENT_USED=1 \
    -DSSD1963_DATA_WITDH=8 \
    -DFLEXIO_MCULCD_DATA_BUS_WIDTH=8 \
    -DCUSTOM_VGLITE_MEMORY_CONFIG=1 \
    -D__FPU_PRESENT=1 \
    -DARM_MATH_CM33 \
    -DRTOS_HEAP_SIZE=2048 \
    -DconfigGENERATE_RUN_TIME_STATS=1 \
    -DMCUX_META_BUILD \
    -DMCUXPRESSO_SDK \
    -DBOARD_ENABLE_PSRAM_CACHE=0 \
    -DCPU_MIMXRT798SGFOA_cm33_core0 \
    -DBOOT_HEADER_ENABLE=1 \
    -DSDK_OS_FREE_RTOS \
    -DTF_LITE_STATIC_MEMORY \
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
    -Wno-strict-aliasing \
    ${FPU} \
    ${DEBUG_CONSOLE_CONFIG} \
")
SET(CMAKE_C_FLAGS_FLASH_RELEASE " \
    ${CMAKE_C_FLAGS_FLASH_RELEASE} \
    -include ${ProjDirPath}/../mcux_config.h \
    -DNDEBUG \
    -DGCID_REV_CID=gc555/0x423_ECO \
    -DMCUX_DBI_LEGACY=0 \
    -DDBI_USE_MIPI_PANEL=1 \
    -DSDK_I2C_BASED_COMPONENT_USED=1 \
    -DSSD1963_DATA_WITDH=8 \
    -DFLEXIO_MCULCD_DATA_BUS_WIDTH=8 \
    -DCUSTOM_VGLITE_MEMORY_CONFIG=1 \
    -D__FPU_PRESENT=1 \
    -DARM_MATH_CM33 \
    -DRTOS_HEAP_SIZE=2048 \
    -DconfigGENERATE_RUN_TIME_STATS=1 \
    -DMCUX_META_BUILD \
    -DMCUXPRESSO_SDK \
    -DBOARD_ENABLE_PSRAM_CACHE=0 \
    -DCPU_MIMXRT798SGFOA_cm33_core0 \
    -DBOOT_HEADER_ENABLE=1 \
    -DSDK_OS_FREE_RTOS \
    -DTF_LITE_STATIC_MEMORY \
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
    -Wno-strict-aliasing \
    ${FPU} \
    ${DEBUG_CONSOLE_CONFIG} \
")
SET(CMAKE_CXX_FLAGS_FLASH_DEBUG " \
    ${CMAKE_CXX_FLAGS_FLASH_DEBUG} \
    -DDEBUG \
    -DARM_MATH_CM33 \
    -D__FPU_PRESENT=1 \
    -DDconfigGENERATE_RUN_TIME_STATS=1 \
    -DMCUX_META_BUILD \
    -DMCUXPRESSO_SDK \
    -DCPU_MIMXRT798SGFOA_cm33_core0 \
    -DBOOT_HEADER_ENABLE=1 \
    -DTF_LITE_STATIC_MEMORY \
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
    -Wno-sign-compare \
    -Wno-strict-aliasing \
    ${FPU} \
    ${DEBUG_CONSOLE_CONFIG} \
")
SET(CMAKE_CXX_FLAGS_FLASH_RELEASE " \
    ${CMAKE_CXX_FLAGS_FLASH_RELEASE} \
    -DNDEBUG \
    -DARM_MATH_CM33 \
    -D__FPU_PRESENT=1 \
    -DDconfigGENERATE_RUN_TIME_STATS=1 \
    -DMCUX_META_BUILD \
    -DMCUXPRESSO_SDK \
    -DCPU_MIMXRT798SGFOA_cm33_core0 \
    -DBOOT_HEADER_ENABLE=1 \
    -DTF_LITE_STATIC_MEMORY \
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
    -Wno-sign-compare \
    -Wno-strict-aliasing \
    ${FPU} \
    ${DEBUG_CONSOLE_CONFIG} \
")
SET(CMAKE_EXE_LINKER_FLAGS_FLASH_DEBUG " \
    ${CMAKE_EXE_LINKER_FLAGS_FLASH_DEBUG} \
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
    -T\"${ProjDirPath}/MIMXRT798Sxxxx_cm33_core0_flash.ld\" -static \
")
SET(CMAKE_EXE_LINKER_FLAGS_FLASH_RELEASE " \
    ${CMAKE_EXE_LINKER_FLAGS_FLASH_RELEASE} \
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
    -T\"${ProjDirPath}/MIMXRT798Sxxxx_cm33_core0_flash.ld\" -static \
")
