INCLUDE(CMakeForceCompiler)

# CROSS COMPILER SETTING
SET(CMAKE_SYSTEM_NAME Generic)
CMAKE_MINIMUM_REQUIRED (VERSION 2.6)

# THE VERSION NUMBER
SET (Tutorial_VERSION_MAJOR 1)
SET (Tutorial_VERSION_MINOR 0)

# ENABLE ASM
ENABLE_LANGUAGE(ASM)

SET(CMAKE_STATIC_LIBRARY_PREFIX)
SET(CMAKE_STATIC_LIBRARY_SUFFIX)

SET(CMAKE_EXECUTABLE_LIBRARY_PREFIX)
SET(CMAKE_EXECUTABLE_LIBRARY_SUFFIX)

 
# CURRENT DIRECTORY
SET(ProjDirPath ${CMAKE_CURRENT_SOURCE_DIR})


SET(CMAKE_ASM_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_ASM_FLAGS_FLEXSPI_NOR_DEBUG} -D__STARTUP_CLEAR_BSS")

SET(CMAKE_ASM_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_ASM_FLAGS_FLEXSPI_NOR_DEBUG} -DDEBUG")

SET(CMAKE_ASM_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_ASM_FLAGS_FLEXSPI_NOR_DEBUG} -D__STARTUP_INITIALIZE_NONCACHEDATA")

SET(CMAKE_ASM_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_ASM_FLAGS_FLEXSPI_NOR_DEBUG} -g")

SET(CMAKE_ASM_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_ASM_FLAGS_FLEXSPI_NOR_DEBUG} -mcpu=cortex-m7")

SET(CMAKE_ASM_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_ASM_FLAGS_FLEXSPI_NOR_DEBUG} -Wall")

SET(CMAKE_ASM_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_ASM_FLAGS_FLEXSPI_NOR_DEBUG} -mfloat-abi=hard")

SET(CMAKE_ASM_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_ASM_FLAGS_FLEXSPI_NOR_DEBUG} -mfpu=fpv5-d16")

SET(CMAKE_ASM_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_ASM_FLAGS_FLEXSPI_NOR_DEBUG} -mthumb")

SET(CMAKE_ASM_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_ASM_FLAGS_FLEXSPI_NOR_DEBUG} -fno-common")

SET(CMAKE_ASM_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_ASM_FLAGS_FLEXSPI_NOR_DEBUG} -ffunction-sections")

SET(CMAKE_ASM_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_ASM_FLAGS_FLEXSPI_NOR_DEBUG} -fdata-sections")

SET(CMAKE_ASM_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_ASM_FLAGS_FLEXSPI_NOR_DEBUG} -ffreestanding")

SET(CMAKE_ASM_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_ASM_FLAGS_FLEXSPI_NOR_DEBUG} -fno-builtin")

SET(CMAKE_ASM_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_ASM_FLAGS_FLEXSPI_NOR_DEBUG} -mapcs")

SET(CMAKE_ASM_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_ASM_FLAGS_FLEXSPI_NOR_DEBUG} -std=gnu99")

SET(CMAKE_ASM_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_ASM_FLAGS_FLEXSPI_NOR_RELEASE} -D__STARTUP_CLEAR_BSS")

SET(CMAKE_ASM_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_ASM_FLAGS_FLEXSPI_NOR_RELEASE} -DNDEBUG")

SET(CMAKE_ASM_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_ASM_FLAGS_FLEXSPI_NOR_RELEASE} -D__STARTUP_INITIALIZE_NONCACHEDATA")

SET(CMAKE_ASM_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_ASM_FLAGS_FLEXSPI_NOR_RELEASE} -mcpu=cortex-m7")

SET(CMAKE_ASM_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_ASM_FLAGS_FLEXSPI_NOR_RELEASE} -Wall")

SET(CMAKE_ASM_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_ASM_FLAGS_FLEXSPI_NOR_RELEASE} -mfloat-abi=hard")

SET(CMAKE_ASM_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_ASM_FLAGS_FLEXSPI_NOR_RELEASE} -mfpu=fpv5-d16")

SET(CMAKE_ASM_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_ASM_FLAGS_FLEXSPI_NOR_RELEASE} -mthumb")

SET(CMAKE_ASM_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_ASM_FLAGS_FLEXSPI_NOR_RELEASE} -fno-common")

SET(CMAKE_ASM_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_ASM_FLAGS_FLEXSPI_NOR_RELEASE} -ffunction-sections")

SET(CMAKE_ASM_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_ASM_FLAGS_FLEXSPI_NOR_RELEASE} -fdata-sections")

SET(CMAKE_ASM_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_ASM_FLAGS_FLEXSPI_NOR_RELEASE} -ffreestanding")

SET(CMAKE_ASM_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_ASM_FLAGS_FLEXSPI_NOR_RELEASE} -fno-builtin")

SET(CMAKE_ASM_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_ASM_FLAGS_FLEXSPI_NOR_RELEASE} -mapcs")

SET(CMAKE_ASM_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_ASM_FLAGS_FLEXSPI_NOR_RELEASE} -std=gnu99")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG} -DXIP_EXTERNAL_FLASH=1")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG} -DXIP_BOOT_HEADER_ENABLE=1")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG} -DDEBUG")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG} -DCPU_MIMXRT1052DVL6B")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG} -DPRINTF_ADVANCED_ENABLE=1")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG} -DUSE_RTOS=1")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG} -DMBEDTLS_CONFIG_FILE='\"google_mbedtls_config.h\"'")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG} -DIOTC_CROSS_TARGET")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG} -DIOTC_FS_DUMMY")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG} -DHIGH_SPEED_SDIO_CLOCK")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG} -DSDK_DEBUGCONSOLE_UART")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG} -DSERIAL_PORT_TYPE_UART=1")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG} -DWIFI_WICED")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG} -DFSL_RTOS_FREE_RTOS")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG} -g")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG} -O0")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG} -fomit-frame-pointer")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG} -mcpu=cortex-m7")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG} -Wall")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG} -mfloat-abi=hard")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG} -mfpu=fpv5-d16")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG} -mthumb")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG} -MMD")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG} -MP")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG} -fno-common")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG} -ffunction-sections")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG} -fdata-sections")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG} -ffreestanding")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG} -fno-builtin")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG} -mapcs")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG} -std=gnu99")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE} -DXIP_EXTERNAL_FLASH=1")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE} -DXIP_BOOT_HEADER_ENABLE=1")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE} -DNDEBUG")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE} -DCPU_MIMXRT1052DVL6B")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE} -DPRINTF_ADVANCED_ENABLE=1")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE} -DUSE_RTOS=1")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE} -DMBEDTLS_CONFIG_FILE='\"google_mbedtls_config.h\"'")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE} -DIOTC_CROSS_TARGET")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE} -DIOTC_FS_DUMMY")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE} -DHIGH_SPEED_SDIO_CLOCK")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE} -DSDK_DEBUGCONSOLE_UART")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE} -DSERIAL_PORT_TYPE_UART=1")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE} -DWIFI_WICED")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE} -DFSL_RTOS_FREE_RTOS")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE} -Os")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE} -fomit-frame-pointer")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE} -mcpu=cortex-m7")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE} -Wall")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE} -mfloat-abi=hard")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE} -mfpu=fpv5-d16")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE} -mthumb")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE} -MMD")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE} -MP")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE} -fno-common")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE} -ffunction-sections")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE} -fdata-sections")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE} -ffreestanding")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE} -fno-builtin")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE} -mapcs")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE} -std=gnu99")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_DEBUG} -DDEBUG")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_DEBUG} -DSERIAL_PORT_TYPE_UART=1")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_DEBUG} -g")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_DEBUG} -O0")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_DEBUG} -mcpu=cortex-m7")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_DEBUG} -Wall")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_DEBUG} -mfloat-abi=hard")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_DEBUG} -mfpu=fpv5-d16")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_DEBUG} -mthumb")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_DEBUG} -MMD")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_DEBUG} -MP")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_DEBUG} -fno-common")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_DEBUG} -ffunction-sections")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_DEBUG} -fdata-sections")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_DEBUG} -ffreestanding")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_DEBUG} -fno-builtin")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_DEBUG} -mapcs")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_DEBUG} -fno-rtti")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_DEBUG} -fno-exceptions")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_RELEASE} -DNDEBUG")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_RELEASE} -DSERIAL_PORT_TYPE_UART=1")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_RELEASE} -Os")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_RELEASE} -mcpu=cortex-m7")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_RELEASE} -Wall")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_RELEASE} -mfloat-abi=hard")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_RELEASE} -mfpu=fpv5-d16")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_RELEASE} -mthumb")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_RELEASE} -MMD")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_RELEASE} -MP")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_RELEASE} -fno-common")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_RELEASE} -ffunction-sections")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_RELEASE} -fdata-sections")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_RELEASE} -ffreestanding")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_RELEASE} -fno-builtin")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_RELEASE} -mapcs")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_RELEASE} -fno-rtti")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_RELEASE} -fno-exceptions")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_DEBUG} -g")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_DEBUG} -mcpu=cortex-m7")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_DEBUG} -Wall")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_DEBUG} -mfloat-abi=hard")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_DEBUG} -mfpu=fpv5-d16")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_DEBUG} --specs=nano.specs")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_DEBUG} --specs=nosys.specs")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_DEBUG} -fno-common")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_DEBUG} -ffunction-sections")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_DEBUG} -fdata-sections")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_DEBUG} -ffreestanding")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_DEBUG} -fno-builtin")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_DEBUG} -mthumb")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_DEBUG} -mapcs")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_DEBUG} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_DEBUG} --gc-sections")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_DEBUG} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_DEBUG} -static")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_DEBUG} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_DEBUG} -z")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_DEBUG} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_DEBUG} muldefs")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_DEBUG} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_DEBUG} -Map=output.map")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_DEBUG} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_DEBUG} --defsym=__stack_size__=0x200")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_DEBUG} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_DEBUG} --defsym=__heap_size__=0x1000")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_RELEASE} -mcpu=cortex-m7")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_RELEASE} -Wall")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_RELEASE} -mfloat-abi=hard")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_RELEASE} -mfpu=fpv5-d16")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_RELEASE} --specs=nano.specs")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_RELEASE} --specs=nosys.specs")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_RELEASE} -fno-common")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_RELEASE} -ffunction-sections")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_RELEASE} -fdata-sections")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_RELEASE} -ffreestanding")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_RELEASE} -fno-builtin")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_RELEASE} -mthumb")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_RELEASE} -mapcs")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_RELEASE} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_RELEASE} --gc-sections")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_RELEASE} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_RELEASE} -static")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_RELEASE} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_RELEASE} -z")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_RELEASE} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_RELEASE} muldefs")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_RELEASE} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_RELEASE} -Map=output.map")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_RELEASE} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_RELEASE} --defsym=__stack_size__=0x200")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_RELEASE} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_RELEASE} --defsym=__heap_size__=0x1000")

include_directories(${ProjDirPath}/..)

include_directories(${ProjDirPath}/../../..)

include_directories(${ProjDirPath}/..)

include_directories(${ProjDirPath}/../../..)

include_directories(${ProjDirPath}/../../../../../rtos/amazon-freertos/freertos_kernel/portable/GCC/ARM_CM4F)

include_directories(${ProjDirPath}/..)

include_directories(${ProjDirPath}/../../..)

include_directories(${ProjDirPath}/../../../../../CMSIS/Include)

include_directories(${ProjDirPath}/../../../../../devices)

include_directories(${ProjDirPath}/../../../../../middleware/mbedtls/include)

include_directories(${ProjDirPath}/../../../../../middleware/mbedtls/port/ksdk)

include_directories(${ProjDirPath}/../../../../../middleware/google_iot/include)

include_directories(${ProjDirPath}/../../../../../middleware/google_iot/include/bsp)

include_directories(${ProjDirPath}/../../../../../middleware/google_iot/src/bsp)

include_directories(${ProjDirPath}/../../../../../middleware/google_iot/src/libiotc)

include_directories(${ProjDirPath}/../../../../../middleware/google_iot/src/libiotc/control_topic)

include_directories(${ProjDirPath}/../../../../../middleware/google_iot/src/libiotc/datastructures)

include_directories(${ProjDirPath}/../../../../../middleware/google_iot/src/libiotc/event_dispatcher)

include_directories(${ProjDirPath}/../../../../../middleware/google_iot/src/libiotc/event_loop)

include_directories(${ProjDirPath}/../../../../../middleware/google_iot/src/libiotc/io)

include_directories(${ProjDirPath}/../../../../../middleware/google_iot/src/libiotc/io/fs)

include_directories(${ProjDirPath}/../../../../../middleware/google_iot/src/libiotc/io/net)

include_directories(${ProjDirPath}/../../../../../middleware/google_iot/src/libiotc/memory)

include_directories(${ProjDirPath}/../../../../../middleware/google_iot/src/libiotc/mqtt/codec)

include_directories(${ProjDirPath}/../../../../../middleware/google_iot/src/libiotc/mqtt/logic)

include_directories(${ProjDirPath}/../../../../../middleware/google_iot/src/libiotc/platform/iotc_thread)

include_directories(${ProjDirPath}/../../../../../middleware/google_iot/src/libiotc/platform/dummy/iotc_thread)

include_directories(${ProjDirPath}/../../../../../middleware/google_iot/src/libiotc/tls)

include_directories(${ProjDirPath}/../../../../../middleware/google_iot/src/libiotc/tls/certs)

include_directories(${ProjDirPath}/../../../../../middleware/google_iot/third_party/mqtt-protocol-c)

include_directories(${ProjDirPath}/../../../../../rtos/amazon-freertos/freertos_kernel/include)

include_directories(${ProjDirPath}/../../../../../devices/MIMXRT1052/drivers)

include_directories(${ProjDirPath}/../../../../../middleware/lwip/port)

include_directories(${ProjDirPath}/../../../../../middleware/lwip/src)

include_directories(${ProjDirPath}/../../../../../middleware/lwip/src/include)

include_directories(${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/platforms/MURATA_TYPE1DX)

include_directories(${ProjDirPath}/../../../../../middleware/sdmmc/port)

include_directories(${ProjDirPath}/../../../../../devices/MIMXRT1052)

include_directories(${ProjDirPath}/../../../../../devices/MIMXRT1052/utilities/str)

include_directories(${ProjDirPath}/../../../../../devices/MIMXRT1052/utilities/debug_console)

include_directories(${ProjDirPath}/../../../../../components/uart)

include_directories(${ProjDirPath}/../../../../../components/serial_manager)

include_directories(${ProjDirPath}/../../../../../components/lists)

include_directories(${ProjDirPath}/../../../../../devices/MIMXRT1052/xip)

include_directories(${ProjDirPath}/../../../xip)

include_directories(${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD/internal/chips/4343W)

include_directories(${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/include)

include_directories(${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/network/LwIP/WWD)

include_directories(${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/platform)

include_directories(${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/platform/include)

include_directories(${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/platform/MCU)

include_directories(${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/platform/MCU/LPC)

include_directories(${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/platform/MCU/LPC/WWD)

include_directories(${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/RTOS/FreeRTOS/WWD)

include_directories(${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD)

include_directories(${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD/include)

include_directories(${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD/include/network)

include_directories(${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD/include/platform)

include_directories(${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD/include/RTOS)

include_directories(${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD/internal)

include_directories(${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD/internal/bus_protocols/SDIO)

include_directories(${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip)

include_directories(${ProjDirPath}/../../../../../middleware/sdmmc/inc)

include_directories(${ProjDirPath}/../../../../../middleware/sdmmc/src)

include_directories(${ProjDirPath}/../../../../../devices/MIMXRT1052/utilities)

add_executable(google_iot_hello_wifi.elf 
"${ProjDirPath}/../main.c"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/app/test_scan.c"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/app/wlanif_add.c"
"${ProjDirPath}/../commandline.c"
"${ProjDirPath}/../commandline.h"
"${ProjDirPath}/../example_utils.c"
"${ProjDirPath}/../example_utils.h"
"${ProjDirPath}/../lwipopts.h"
"${ProjDirPath}/../google_mbedtls_config.h"
"${ProjDirPath}/../google_iot_config.h"
"${ProjDirPath}/../FreeRTOSConfig.h"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
"${ProjDirPath}/../../../../../devices/MIMXRT1052/drivers/fsl_trng.c"
"${ProjDirPath}/../../../../../devices/MIMXRT1052/drivers/fsl_trng.h"
"${ProjDirPath}/../../../../../devices/MIMXRT1052/drivers/fsl_dcp.c"
"${ProjDirPath}/../../../../../devices/MIMXRT1052/drivers/fsl_dcp.h"
"${ProjDirPath}/../../../../../devices/MIMXRT1052/drivers/fsl_cache.c"
"${ProjDirPath}/../../../../../devices/MIMXRT1052/drivers/fsl_cache.h"
"${ProjDirPath}/../../../../../devices/MIMXRT1052/drivers/fsl_flexspi.c"
"${ProjDirPath}/../../../../../devices/MIMXRT1052/drivers/fsl_flexspi.h"
"${ProjDirPath}/../../../../../middleware/lwip/port/arch/cc.h"
"${ProjDirPath}/../../../../../middleware/lwip/port/arch/perf.h"
"${ProjDirPath}/../../../../../middleware/lwip/port/arch/sys_arch.h"
"${ProjDirPath}/../../../../../middleware/lwip/port/sys_arch.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/api/api_lib.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/api/api_msg.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/api/err.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/api/if_api.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/api/netbuf.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/api/netdb.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/api/netifapi.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/api/sockets.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/api/tcpip.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/core/altcp.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/core/altcp_alloc.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/core/altcp_tcp.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/core/def.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/core/dns.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/core/inet_chksum.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/core/init.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/core/ip.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/core/ipv4/autoip.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/core/ipv4/dhcp.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/core/ipv4/etharp.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/core/ipv4/icmp.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/core/ipv4/igmp.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/core/ipv4/ip4.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/core/ipv4/ip4_addr.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/core/ipv4/ip4_frag.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/core/ipv6/dhcp6.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/core/ipv6/ethip6.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/core/ipv6/icmp6.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/core/ipv6/inet6.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/core/ipv6/ip6.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/core/ipv6/ip6_addr.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/core/ipv6/ip6_frag.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/core/ipv6/mld6.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/core/ipv6/nd6.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/core/mem.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/core/memp.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/core/netif.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/core/pbuf.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/core/raw.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/core/stats.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/core/sys.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/core/tcp.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/core/tcp_in.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/core/tcp_out.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/core/timeouts.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/core/udp.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/compat/posix/arpa/inet.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/compat/posix/net/if.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/compat/posix/netdb.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/compat/posix/sys/socket.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/compat/stdc/errno.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/altcp.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/altcp_tcp.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/altcp_tls.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/api.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/arch.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/autoip.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/debug.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/def.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/dhcp.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/dhcp6.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/dns.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/err.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/errno.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/etharp.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/ethip6.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/icmp.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/icmp6.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/if_api.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/igmp.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/inet.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/inet_chksum.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/init.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/ip.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/ip4.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/ip4_addr.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/ip4_frag.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/ip6.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/ip6_addr.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/ip6_frag.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/ip6_zone.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/ip_addr.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/mem.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/memp.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/mld6.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/nd6.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/netbuf.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/netdb.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/netif.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/netifapi.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/opt.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/pbuf.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/priv/altcp_priv.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/priv/api_msg.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/priv/mem_priv.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/priv/memp_priv.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/priv/memp_std.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/priv/nd6_priv.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/priv/raw_priv.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/priv/sockets_priv.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/priv/tcp_priv.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/priv/tcpip_priv.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/prot/autoip.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/prot/dhcp.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/prot/dhcp6.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/prot/dns.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/prot/etharp.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/prot/ethernet.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/prot/iana.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/prot/icmp.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/prot/icmp6.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/prot/ieee.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/prot/igmp.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/prot/ip.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/prot/ip4.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/prot/ip6.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/prot/mld6.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/prot/nd6.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/prot/tcp.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/prot/udp.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/raw.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/sio.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/snmp.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/sockets.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/stats.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/sys.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/tcp.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/tcpbase.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/tcpip.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/timeouts.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/udp.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/netif/bridgeif.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/netif/bridgeif_opts.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/netif/etharp.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/netif/ethernet.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/netif/ieee802154.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/netif/lowpan6.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/netif/lowpan6_ble.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/netif/lowpan6_common.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/netif/lowpan6_opts.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/netif/ppp/ccp.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/netif/ppp/chap-md5.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/netif/ppp/chap-new.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/netif/ppp/chap_ms.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/netif/ppp/eap.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/netif/ppp/ecp.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/netif/ppp/eui64.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/netif/ppp/fsm.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/netif/ppp/ipcp.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/netif/ppp/ipv6cp.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/netif/ppp/lcp.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/netif/ppp/magic.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/netif/ppp/mppe.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/netif/ppp/polarssl/arc4.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/netif/ppp/polarssl/des.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/netif/ppp/polarssl/md4.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/netif/ppp/polarssl/md5.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/netif/ppp/polarssl/sha1.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/netif/ppp/ppp.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/netif/ppp/ppp_impl.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/netif/ppp/ppp_opts.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/netif/ppp/pppapi.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/netif/ppp/pppcrypt.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/netif/ppp/pppdebug.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/netif/ppp/pppoe.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/netif/ppp/pppol2tp.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/netif/ppp/pppos.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/netif/ppp/upap.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/netif/ppp/vj.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/netif/slipif.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/netif/zepif.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/netif/bridgeif.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/netif/bridgeif_fdb.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/netif/ethernet.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/netif/lowpan6.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/netif/lowpan6_ble.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/netif/lowpan6_common.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/netif/ppp/auth.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/netif/ppp/ccp.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/netif/ppp/chap-md5.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/netif/ppp/chap-new.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/netif/ppp/chap_ms.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/netif/ppp/demand.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/netif/ppp/eap.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/netif/ppp/eui64.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/netif/ppp/fsm.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/netif/ppp/ipcp.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/netif/ppp/ipv6cp.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/netif/ppp/lcp.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/netif/ppp/lwip_ecp.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/netif/ppp/magic.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/netif/ppp/mppe.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/netif/ppp/multilink.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/netif/ppp/ppp.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/netif/ppp/pppapi.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/netif/ppp/pppcrypt.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/netif/ppp/pppoe.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/netif/ppp/pppol2tp.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/netif/ppp/pppos.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/netif/ppp/upap.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/netif/ppp/utils.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/netif/ppp/vj.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/netif/slipif.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/netif/zepif.c"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/platforms/MURATA_TYPE1DX/wifi_nvram_image.h"
"${ProjDirPath}/../../../../../middleware/sdmmc/inc/fsl_sdio.h"
"${ProjDirPath}/../../../../../middleware/sdmmc/inc/fsl_sdmmc_common.h"
"${ProjDirPath}/../../../../../middleware/sdmmc/inc/fsl_sdmmc_host.h"
"${ProjDirPath}/../../../../../middleware/sdmmc/src/fsl_sdio.c"
"${ProjDirPath}/../../../../../middleware/sdmmc/src/fsl_sdmmc_common.c"
"${ProjDirPath}/../../../../../middleware/sdmmc/port/fsl_sdmmc_event.h"
"${ProjDirPath}/../../../../../middleware/sdmmc/port/usdhc/freertos/fsl_sdmmc_event.c"
"${ProjDirPath}/../../../../../middleware/sdmmc/port/usdhc/freertos/fsl_sdmmc_host.c"
"${ProjDirPath}/../../../../../devices/MIMXRT1052/drivers/fsl_usdhc.c"
"${ProjDirPath}/../../../../../devices/MIMXRT1052/drivers/fsl_usdhc.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/freertos_kernel/portable/MemMang/heap_4.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/aes.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/aesni.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/arc4.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/aria.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/asn1.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/asn1write.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/base64.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/bignum.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/blowfish.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/bn_mul.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/camellia.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/ccm.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/certs.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/chacha20.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/chachapoly.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/check_config.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/cipher.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/cipher_internal.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/cmac.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/compat-1.3.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/config.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/ctr_drbg.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/debug.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/des.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/dhm.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/ecdh.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/ecdsa.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/ecjpake.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/ecp.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/ecp_internal.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/entropy.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/entropy_poll.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/error.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/gcm.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/havege.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/hkdf.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/hmac_drbg.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/md.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/md2.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/md4.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/md5.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/md_internal.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/memory_buffer_alloc.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/net.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/net_sockets.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/nist_kw.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/oid.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/padlock.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/pem.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/pk.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/pk_internal.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/pkcs11.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/pkcs12.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/pkcs5.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/platform.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/platform_time.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/platform_util.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/poly1305.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/ripemd160.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/rsa.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/rsa_internal.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/sha1.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/sha256.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/sha512.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/ssl.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/ssl_cache.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/ssl_ciphersuites.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/ssl_cookie.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/ssl_internal.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/ssl_ticket.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/threading.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/timing.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/version.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/x509.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/x509_crl.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/x509_crt.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/x509_csr.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/xtea.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/aes.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/aesni.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/arc4.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/aria.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/asn1parse.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/asn1write.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/base64.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/bignum.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/blowfish.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/camellia.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/ccm.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/certs.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/chacha20.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/chachapoly.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/cipher.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/cipher_wrap.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/cmac.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/ctr_drbg.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/debug.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/des.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/dhm.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/ecdh.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/ecdsa.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/ecjpake.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/ecp.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/ecp_curves.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/entropy.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/entropy_poll.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/error.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/gcm.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/havege.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/hkdf.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/hmac_drbg.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/md.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/md2.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/md4.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/md5.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/md_wrap.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/memory_buffer_alloc.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/net_sockets.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/nist_kw.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/oid.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/padlock.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/pem.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/pk.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/pk_wrap.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/pkcs11.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/pkcs12.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/pkcs5.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/pkparse.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/pkwrite.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/platform.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/platform_util.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/poly1305.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/ripemd160.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/rsa.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/rsa_internal.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/sha1.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/sha256.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/sha512.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/ssl_cache.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/ssl_ciphersuites.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/ssl_cli.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/ssl_cookie.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/ssl_srv.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/ssl_ticket.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/ssl_tls.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/threading.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/timing.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/version.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/version_features.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/x509.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/x509_create.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/x509_crl.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/x509_crt.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/x509_csr.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/x509write_crt.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/x509write_csr.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/xtea.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/port/ksdk/aes_alt.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/port/ksdk/aes_alt.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/port/ksdk/des_alt.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/port/ksdk/des_alt.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/port/ksdk/ecp_alt.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/port/ksdk/ecp_alt.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/port/ksdk/ecp_alt_ksdk.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/port/ksdk/ecp_curves_alt.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/port/ksdk/ksdk_mbedtls.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/port/ksdk/ksdk_mbedtls.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/port/ksdk/ksdk_mbedtls_config.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/port/ksdk/sha1_alt.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/port/ksdk/sha256_alt.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/apps/sntp/sntp.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/apps/sntp.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/apps/sntp_opts.h"
"${ProjDirPath}/../../../../../middleware/google_iot/include/bsp/iotc_bsp_crypto.h"
"${ProjDirPath}/../../../../../middleware/google_iot/include/bsp/iotc_bsp_io_fs.h"
"${ProjDirPath}/../../../../../middleware/google_iot/include/bsp/iotc_bsp_io_net.h"
"${ProjDirPath}/../../../../../middleware/google_iot/include/bsp/iotc_bsp_mem.h"
"${ProjDirPath}/../../../../../middleware/google_iot/include/bsp/iotc_bsp_rng.h"
"${ProjDirPath}/../../../../../middleware/google_iot/include/bsp/iotc_bsp_time.h"
"${ProjDirPath}/../../../../../middleware/google_iot/include/bsp/iotc_bsp_tls.h"
"${ProjDirPath}/../../../../../middleware/google_iot/include/iotc.h"
"${ProjDirPath}/../../../../../middleware/google_iot/include/iotc_connection_data.h"
"${ProjDirPath}/../../../../../middleware/google_iot/include/iotc_error.h"
"${ProjDirPath}/../../../../../middleware/google_iot/include/iotc_jwt.h"
"${ProjDirPath}/../../../../../middleware/google_iot/include/iotc_mqtt.h"
"${ProjDirPath}/../../../../../middleware/google_iot/include/iotc_time.h"
"${ProjDirPath}/../../../../../middleware/google_iot/include/iotc_types.h"
"${ProjDirPath}/../../../../../middleware/google_iot/src/bsp/crypto/mbedtls/iotc_bsp_crypto_mbedtls.c"
"${ProjDirPath}/../../../../../middleware/google_iot/src/bsp/iotc_bsp_debug.h"
"${ProjDirPath}/../../../../../middleware/google_iot/src/bsp/platform/freertos/iotc_bsp_io_fs_freertos.c"
"${ProjDirPath}/../../../../../middleware/google_iot/src/bsp/platform/freertos/iotc_bsp_io_net_freertos.c"
"${ProjDirPath}/../../../../../middleware/google_iot/src/bsp/platform/freertos/iotc_bsp_mem_freertos.c"
"${ProjDirPath}/../../../../../middleware/google_iot/src/bsp/platform/freertos/iotc_bsp_rng_freertos.c"
"${ProjDirPath}/../../../../../middleware/google_iot/src/bsp/platform/freertos/iotc_bsp_time_freertos.c"
"${ProjDirPath}/../../../../../middleware/google_iot/src/bsp/tls/mbedtls/iotc_bsp_tls_mbedtls.c"
"${ProjDirPath}/../../../../../middleware/google_iot/src/libiotc/control_topic/iotc_control_topic_layer.c"
"${ProjDirPath}/../../../../../middleware/google_iot/src/libiotc/control_topic/iotc_control_topic_layer.h"
"${ProjDirPath}/../../../../../middleware/google_iot/src/libiotc/datastructures/iotc_list.h"
"${ProjDirPath}/../../../../../middleware/google_iot/src/libiotc/datastructures/iotc_memory_type.h"
"${ProjDirPath}/../../../../../middleware/google_iot/src/libiotc/datastructures/iotc_vector.c"
"${ProjDirPath}/../../../../../middleware/google_iot/src/libiotc/datastructures/iotc_vector.h"
"${ProjDirPath}/../../../../../middleware/google_iot/src/libiotc/event_dispatcher/iotc_event_dispatcher.c"
"${ProjDirPath}/../../../../../middleware/google_iot/src/libiotc/event_dispatcher/iotc_event_dispatcher_api.h"
"${ProjDirPath}/../../../../../middleware/google_iot/src/libiotc/event_dispatcher/iotc_event_dispatcher_macros.h"
"${ProjDirPath}/../../../../../middleware/google_iot/src/libiotc/event_dispatcher/iotc_event_handle.c"
"${ProjDirPath}/../../../../../middleware/google_iot/src/libiotc/event_dispatcher/iotc_event_handle.h"
"${ProjDirPath}/../../../../../middleware/google_iot/src/libiotc/event_dispatcher/iotc_event_handle_queue.h"
"${ProjDirPath}/../../../../../middleware/google_iot/src/libiotc/event_dispatcher/iotc_event_handle_typedef.h"
"${ProjDirPath}/../../../../../middleware/google_iot/src/libiotc/event_dispatcher/iotc_event_thread_dispatcher.c"
"${ProjDirPath}/../../../../../middleware/google_iot/src/libiotc/event_dispatcher/iotc_event_thread_dispatcher.h"
"${ProjDirPath}/../../../../../middleware/google_iot/src/libiotc/event_dispatcher/iotc_time_event.c"
"${ProjDirPath}/../../../../../middleware/google_iot/src/libiotc/event_dispatcher/iotc_time_event.h"
"${ProjDirPath}/../../../../../middleware/google_iot/src/libiotc/event_loop/iotc_event_loop.c"
"${ProjDirPath}/../../../../../middleware/google_iot/src/libiotc/event_loop/iotc_event_loop.h"
"${ProjDirPath}/../../../../../middleware/google_iot/src/libiotc/io/fs/dummy/iotc_fs_dummy.c"
"${ProjDirPath}/../../../../../middleware/google_iot/src/libiotc/io/fs/iotc_fs_api.h"
"${ProjDirPath}/../../../../../middleware/google_iot/src/libiotc/io/fs/iotc_fs_bsp_to_iotc_mapping.c"
"${ProjDirPath}/../../../../../middleware/google_iot/src/libiotc/io/fs/iotc_fs_bsp_to_iotc_mapping.h"
"${ProjDirPath}/../../../../../middleware/google_iot/src/libiotc/io/fs/iotc_fs_filename_defs.h"
"${ProjDirPath}/../../../../../middleware/google_iot/src/libiotc/io/fs/iotc_fs_filenames.c"
"${ProjDirPath}/../../../../../middleware/google_iot/src/libiotc/io/fs/iotc_fs_filenames.h"
"${ProjDirPath}/../../../../../middleware/google_iot/src/libiotc/io/fs/iotc_fs_header.h"
"${ProjDirPath}/../../../../../middleware/google_iot/src/libiotc/io/fs/iotc_resource_manager.c"
"${ProjDirPath}/../../../../../middleware/google_iot/src/libiotc/io/fs/iotc_resource_manager.h"
"${ProjDirPath}/../../../../../middleware/google_iot/src/libiotc/io/net/iotc_io_net_layer.c"
"${ProjDirPath}/../../../../../middleware/google_iot/src/libiotc/io/net/iotc_io_net_layer.h"
"${ProjDirPath}/../../../../../middleware/google_iot/src/libiotc/io/net/iotc_io_net_layer_state.h"
"${ProjDirPath}/../../../../../middleware/google_iot/src/libiotc/iotc.c"
"${ProjDirPath}/../../../../../middleware/google_iot/src/libiotc/iotc_backoff_lut_config.h"
"${ProjDirPath}/../../../../../middleware/google_iot/src/libiotc/iotc_backoff_status_api.c"
"${ProjDirPath}/../../../../../middleware/google_iot/src/libiotc/iotc_backoff_status_api.h"
"${ProjDirPath}/../../../../../middleware/google_iot/src/libiotc/iotc_common.h"
"${ProjDirPath}/../../../../../middleware/google_iot/src/libiotc/iotc_config.h"
"${ProjDirPath}/../../../../../middleware/google_iot/src/libiotc/iotc_connection_data.c"
"${ProjDirPath}/../../../../../middleware/google_iot/src/libiotc/iotc_connection_data_internal.h"
"${ProjDirPath}/../../../../../middleware/google_iot/src/libiotc/iotc_coroutine.h"
"${ProjDirPath}/../../../../../middleware/google_iot/src/libiotc/iotc_data_desc.c"
"${ProjDirPath}/../../../../../middleware/google_iot/src/libiotc/iotc_data_desc.h"
"${ProjDirPath}/../../../../../middleware/google_iot/src/libiotc/iotc_debug.c"
"${ProjDirPath}/../../../../../middleware/google_iot/src/libiotc/iotc_debug.h"
"${ProjDirPath}/../../../../../middleware/google_iot/src/libiotc/iotc_error.c"
"${ProjDirPath}/../../../../../middleware/google_iot/src/libiotc/iotc_globals.c"
"${ProjDirPath}/../../../../../middleware/google_iot/src/libiotc/iotc_globals.h"
"${ProjDirPath}/../../../../../middleware/google_iot/src/libiotc/iotc_handle.c"
"${ProjDirPath}/../../../../../middleware/google_iot/src/libiotc/iotc_handle.h"
"${ProjDirPath}/../../../../../middleware/google_iot/src/libiotc/iotc_helpers.c"
"${ProjDirPath}/../../../../../middleware/google_iot/src/libiotc/iotc_helpers.h"
"${ProjDirPath}/../../../../../middleware/google_iot/src/libiotc/iotc_internals.c"
"${ProjDirPath}/../../../../../middleware/google_iot/src/libiotc/iotc_internals.h"
"${ProjDirPath}/../../../../../middleware/google_iot/src/libiotc/iotc_io_timeouts.c"
"${ProjDirPath}/../../../../../middleware/google_iot/src/libiotc/iotc_io_timeouts.h"
"${ProjDirPath}/../../../../../middleware/google_iot/src/libiotc/iotc_jwt.c"
"${ProjDirPath}/../../../../../middleware/google_iot/src/libiotc/iotc_layer.h"
"${ProjDirPath}/../../../../../middleware/google_iot/src/libiotc/iotc_layer_api.c"
"${ProjDirPath}/../../../../../middleware/google_iot/src/libiotc/iotc_layer_api.h"
"${ProjDirPath}/../../../../../middleware/google_iot/src/libiotc/iotc_layer_chain.h"
"${ProjDirPath}/../../../../../middleware/google_iot/src/libiotc/iotc_layer_default_allocators.c"
"${ProjDirPath}/../../../../../middleware/google_iot/src/libiotc/iotc_layer_default_allocators.h"
"${ProjDirPath}/../../../../../middleware/google_iot/src/libiotc/iotc_layer_default_functions.c"
"${ProjDirPath}/../../../../../middleware/google_iot/src/libiotc/iotc_layer_default_functions.h"
"${ProjDirPath}/../../../../../middleware/google_iot/src/libiotc/iotc_layer_factory.h"
"${ProjDirPath}/../../../../../middleware/google_iot/src/libiotc/iotc_layer_macros.h"
"${ProjDirPath}/../../../../../middleware/google_iot/src/libiotc/iotc_layer_stack.h"
"${ProjDirPath}/../../../../../middleware/google_iot/src/libiotc/iotc_layers_ids.h"
"${ProjDirPath}/../../../../../middleware/google_iot/src/libiotc/iotc_macros.h"
"${ProjDirPath}/../../../../../middleware/google_iot/src/libiotc/iotc_timed_task.c"
"${ProjDirPath}/../../../../../middleware/google_iot/src/libiotc/iotc_timed_task.h"
"${ProjDirPath}/../../../../../middleware/google_iot/src/libiotc/iotc_tuple.h"
"${ProjDirPath}/../../../../../middleware/google_iot/src/libiotc/iotc_tuples.c"
"${ProjDirPath}/../../../../../middleware/google_iot/src/libiotc/iotc_tuples.h"
"${ProjDirPath}/../../../../../middleware/google_iot/src/libiotc/iotc_types_internal.h"
"${ProjDirPath}/../../../../../middleware/google_iot/src/libiotc/iotc_user_sub_call_wrapper.c"
"${ProjDirPath}/../../../../../middleware/google_iot/src/libiotc/iotc_user_sub_call_wrapper.h"
"${ProjDirPath}/../../../../../middleware/google_iot/src/libiotc/iotc_version.h"
"${ProjDirPath}/../../../../../middleware/google_iot/src/libiotc/memory/iotc_allocator.c"
"${ProjDirPath}/../../../../../middleware/google_iot/src/libiotc/memory/iotc_allocator.h"
"${ProjDirPath}/../../../../../middleware/google_iot/src/libiotc/mqtt/codec/iotc_mqtt_codec_layer.c"
"${ProjDirPath}/../../../../../middleware/google_iot/src/libiotc/mqtt/codec/iotc_mqtt_codec_layer.h"
"${ProjDirPath}/../../../../../middleware/google_iot/src/libiotc/mqtt/codec/iotc_mqtt_codec_layer_data.c"
"${ProjDirPath}/../../../../../middleware/google_iot/src/libiotc/mqtt/codec/iotc_mqtt_codec_layer_data.h"
"${ProjDirPath}/../../../../../middleware/google_iot/src/libiotc/mqtt/logic/iotc_mqtt_logic_layer.c"
"${ProjDirPath}/../../../../../middleware/google_iot/src/libiotc/mqtt/logic/iotc_mqtt_logic_layer.h"
"${ProjDirPath}/../../../../../middleware/google_iot/src/libiotc/mqtt/logic/iotc_mqtt_logic_layer_commands.h"
"${ProjDirPath}/../../../../../middleware/google_iot/src/libiotc/mqtt/logic/iotc_mqtt_logic_layer_connect_command.h"
"${ProjDirPath}/../../../../../middleware/google_iot/src/libiotc/mqtt/logic/iotc_mqtt_logic_layer_connection_helpers.h"
"${ProjDirPath}/../../../../../middleware/google_iot/src/libiotc/mqtt/logic/iotc_mqtt_logic_layer_data.c"
"${ProjDirPath}/../../../../../middleware/google_iot/src/libiotc/mqtt/logic/iotc_mqtt_logic_layer_data.h"
"${ProjDirPath}/../../../../../middleware/google_iot/src/libiotc/mqtt/logic/iotc_mqtt_logic_layer_data_helpers.h"
"${ProjDirPath}/../../../../../middleware/google_iot/src/libiotc/mqtt/logic/iotc_mqtt_logic_layer_handlers.h"
"${ProjDirPath}/../../../../../middleware/google_iot/src/libiotc/mqtt/logic/iotc_mqtt_logic_layer_helpers.h"
"${ProjDirPath}/../../../../../middleware/google_iot/src/libiotc/mqtt/logic/iotc_mqtt_logic_layer_keepalive_handler.c"
"${ProjDirPath}/../../../../../middleware/google_iot/src/libiotc/mqtt/logic/iotc_mqtt_logic_layer_keepalive_handler.h"
"${ProjDirPath}/../../../../../middleware/google_iot/src/libiotc/mqtt/logic/iotc_mqtt_logic_layer_publish_handler.h"
"${ProjDirPath}/../../../../../middleware/google_iot/src/libiotc/mqtt/logic/iotc_mqtt_logic_layer_publish_q0_command.h"
"${ProjDirPath}/../../../../../middleware/google_iot/src/libiotc/mqtt/logic/iotc_mqtt_logic_layer_publish_q1_command.h"
"${ProjDirPath}/../../../../../middleware/google_iot/src/libiotc/mqtt/logic/iotc_mqtt_logic_layer_subscribe_command.h"
"${ProjDirPath}/../../../../../middleware/google_iot/src/libiotc/mqtt/logic/iotc_mqtt_logic_layer_task_helpers.c"
"${ProjDirPath}/../../../../../middleware/google_iot/src/libiotc/mqtt/logic/iotc_mqtt_logic_layer_task_helpers.h"
"${ProjDirPath}/../../../../../middleware/google_iot/src/libiotc/platform/dummy/iotc_thread/iotc_critical_section_def.h"
"${ProjDirPath}/../../../../../middleware/google_iot/src/libiotc/platform/iotc_thread/iotc_critical_section.h"
"${ProjDirPath}/../../../../../middleware/google_iot/src/libiotc/platform/iotc_thread/iotc_thread_ids.h"
"${ProjDirPath}/../../../../../middleware/google_iot/src/libiotc/platform/iotc_thread/iotc_thread_threadpool.h"
"${ProjDirPath}/../../../../../middleware/google_iot/src/libiotc/platform/iotc_thread/iotc_thread_workerthread.h"
"${ProjDirPath}/../../../../../middleware/google_iot/src/libiotc/tls/certs/iotc_RootCA_list.c"
"${ProjDirPath}/../../../../../middleware/google_iot/src/libiotc/tls/certs/iotc_RootCA_list.h"
"${ProjDirPath}/../../../../../middleware/google_iot/src/libiotc/tls/iotc_tls_layer.c"
"${ProjDirPath}/../../../../../middleware/google_iot/src/libiotc/tls/iotc_tls_layer.h"
"${ProjDirPath}/../../../../../middleware/google_iot/src/libiotc/tls/iotc_tls_layer_state.h"
"${ProjDirPath}/../../../../../middleware/google_iot/third_party/mqtt-protocol-c/iotc_debug_data_desc_dump.c"
"${ProjDirPath}/../../../../../middleware/google_iot/third_party/mqtt-protocol-c/iotc_debug_data_desc_dump.h"
"${ProjDirPath}/../../../../../middleware/google_iot/third_party/mqtt-protocol-c/iotc_mqtt_errors.h"
"${ProjDirPath}/../../../../../middleware/google_iot/third_party/mqtt-protocol-c/iotc_mqtt_message.c"
"${ProjDirPath}/../../../../../middleware/google_iot/third_party/mqtt-protocol-c/iotc_mqtt_message.h"
"${ProjDirPath}/../../../../../middleware/google_iot/third_party/mqtt-protocol-c/iotc_mqtt_parser.c"
"${ProjDirPath}/../../../../../middleware/google_iot/third_party/mqtt-protocol-c/iotc_mqtt_parser.h"
"${ProjDirPath}/../../../../../middleware/google_iot/third_party/mqtt-protocol-c/iotc_mqtt_serialiser.c"
"${ProjDirPath}/../../../../../middleware/google_iot/third_party/mqtt-protocol-c/iotc_mqtt_serialiser.h"
"${ProjDirPath}/../../../../../devices/MIMXRT1052/drivers/fsl_clock.c"
"${ProjDirPath}/../../../../../devices/MIMXRT1052/drivers/fsl_clock.h"
"${ProjDirPath}/../../../../../devices/MIMXRT1052/drivers/fsl_common.c"
"${ProjDirPath}/../../../../../devices/MIMXRT1052/drivers/fsl_common.h"
"${ProjDirPath}/../../../../../devices/MIMXRT1052/MIMXRT1052.h"
"${ProjDirPath}/../../../../../devices/MIMXRT1052/MIMXRT1052_features.h"
"${ProjDirPath}/../../../../../devices/MIMXRT1052/fsl_device_registers.h"
"${ProjDirPath}/../../../../../devices/MIMXRT1052/utilities/debug_console/fsl_debug_console.c"
"${ProjDirPath}/../../../../../devices/MIMXRT1052/utilities/debug_console/fsl_debug_console.h"
"${ProjDirPath}/../../../../../devices/MIMXRT1052/utilities/debug_console/fsl_debug_console_conf.h"
"${ProjDirPath}/../../../../../devices/MIMXRT1052/utilities/str/fsl_str.c"
"${ProjDirPath}/../../../../../devices/MIMXRT1052/utilities/str/fsl_str.h"
"${ProjDirPath}/../../../../../components/uart/lpuart_adapter.c"
"${ProjDirPath}/../../../../../components/uart/uart.h"
"${ProjDirPath}/../../../../../components/serial_manager/serial_manager.c"
"${ProjDirPath}/../../../../../components/serial_manager/serial_manager.h"
"${ProjDirPath}/../../../../../components/serial_manager/serial_port_internal.h"
"${ProjDirPath}/../../../../../components/lists/generic_list.c"
"${ProjDirPath}/../../../../../components/lists/generic_list.h"
"${ProjDirPath}/../../../../../components/serial_manager/serial_port_uart.c"
"${ProjDirPath}/../../../../../components/serial_manager/serial_port_uart.h"
"${ProjDirPath}/../../../../../devices/MIMXRT1052/drivers/fsl_lpuart.c"
"${ProjDirPath}/../../../../../devices/MIMXRT1052/drivers/fsl_lpuart.h"
"${ProjDirPath}/../../../../../devices/MIMXRT1052/gcc/startup_MIMXRT1052.S"
"${ProjDirPath}/../../../../../devices/MIMXRT1052/system_MIMXRT1052.c"
"${ProjDirPath}/../../../../../devices/MIMXRT1052/system_MIMXRT1052.h"
"${ProjDirPath}/../../../../../devices/MIMXRT1052/drivers/fsl_iomuxc.h"
"${ProjDirPath}/../../../../../devices/MIMXRT1052/utilities/fsl_assert.c"
"${ProjDirPath}/../../../../../devices/MIMXRT1052/drivers/fsl_gpio.c"
"${ProjDirPath}/../../../../../devices/MIMXRT1052/drivers/fsl_gpio.h"
"${ProjDirPath}/../../../../../devices/MIMXRT1052/xip/fsl_flexspi_nor_boot.c"
"${ProjDirPath}/../../../../../devices/MIMXRT1052/xip/fsl_flexspi_nor_boot.h"
"${ProjDirPath}/../../../xip/evkbimxrt1050_flexspi_nor_config.c"
"${ProjDirPath}/../../../xip/evkbimxrt1050_flexspi_nor_config.h"
"${ProjDirPath}/../../../xip/evkbimxrt1050_sdram_ini_dcd.c"
"${ProjDirPath}/../../../xip/evkbimxrt1050_sdram_ini_dcd.h"
"${ProjDirPath}/../../../../../CMSIS/Include/core_cm7.h"
"${ProjDirPath}/../../../../../CMSIS/Include/cmsis_armcc.h"
"${ProjDirPath}/../../../../../CMSIS/Include/cmsis_armclang.h"
"${ProjDirPath}/../../../../../CMSIS/Include/cmsis_armclang_ltm.h"
"${ProjDirPath}/../../../../../CMSIS/Include/cmsis_compiler.h"
"${ProjDirPath}/../../../../../CMSIS/Include/cmsis_gcc.h"
"${ProjDirPath}/../../../../../CMSIS/Include/cmsis_iccarm.h"
"${ProjDirPath}/../../../../../CMSIS/Include/cmsis_version.h"
"${ProjDirPath}/../../../../../CMSIS/Include/core_armv81mml.h"
"${ProjDirPath}/../../../../../CMSIS/Include/core_armv8mbl.h"
"${ProjDirPath}/../../../../../CMSIS/Include/core_armv8mml.h"
"${ProjDirPath}/../../../../../CMSIS/Include/mpu_armv7.h"
"${ProjDirPath}/../../../../../CMSIS/Include/mpu_armv8.h"
"${ProjDirPath}/../../../../../CMSIS/Include/arm_common_tables.h"
"${ProjDirPath}/../../../../../CMSIS/Include/arm_const_structs.h"
"${ProjDirPath}/../../../../../CMSIS/Include/arm_math.h"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD/internal/chips/4343W/4343WA1.h"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD/internal/chips/4343W/4343WA1_clm.h"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD/internal/chips/4343W/4343WA1_clm_mfg.h"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD/internal/chips/4343W/4343WA1_mfg.h"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD/internal/chips/4343W/chip_constants.h"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD/internal/chips/4343W/resources.c"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD/internal/chips/4343W/resources.h"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD/internal/chips/4343W/wwd_ap.c"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD/internal/chips/4343W/wwd_chip_specific_functions.c"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/RTOS/FreeRTOS/WWD/wwd_FreeRTOS_systick.h"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/RTOS/FreeRTOS/WWD/wwd_rtos.c"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/RTOS/FreeRTOS/WWD/wwd_rtos.h"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD/include/RTOS/wwd_rtos_interface.h"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD/include/network/wwd_buffer_interface.h"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD/include/network/wwd_network_constants.h"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD/include/network/wwd_network_interface.h"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD/include/platform/wwd_bus_interface.h"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD/include/platform/wwd_platform_interface.h"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD/include/platform/wwd_resource_interface.h"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD/include/platform/wwd_sdio_interface.h"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD/include/platform/wwd_spi_interface.h"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD/include/wwd_assert.h"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD/include/wwd_constants.h"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD/include/wwd_debug.h"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD/include/wwd_eapol.h"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD/include/wwd_events.h"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD/include/wwd_management.h"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD/include/wwd_poll.h"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD/include/wwd_structures.h"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD/include/wwd_wifi.h"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD/include/wwd_wifi_sleep.h"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD/include/wwd_wlioctl.h"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD/internal/bus_protocols/SDIO/wwd_bus_protocol.c"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD/internal/bus_protocols/SDIO/wwd_bus_protocol.h"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD/internal/bus_protocols/wwd_bus_common.c"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD/internal/bus_protocols/wwd_bus_protocol_interface.h"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD/internal/tlv.c"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD/internal/tlv.h"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD/internal/wwd_ap.h"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD/internal/wwd_ap_common.c"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD/internal/wwd_ap_common.h"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD/internal/wwd_bcmendian.h"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD/internal/wwd_clm.c"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD/internal/wwd_clm.h"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD/internal/wwd_debug.c"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD/internal/wwd_eapol.c"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD/internal/wwd_internal.c"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD/internal/wwd_internal.h"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD/internal/wwd_logging.h"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD/internal/wwd_management.c"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD/internal/wwd_rtos_interface.c"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD/internal/wwd_sdpcm.c"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD/internal/wwd_sdpcm.h"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD/internal/wwd_thread.c"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD/internal/wwd_thread.h"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD/internal/wwd_thread_internal.c"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD/internal/wwd_thread_internal.h"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD/internal/wwd_wifi.c"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD/internal/wwd_wifi_chip_common.c"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD/internal/wwd_wifi_chip_common.h"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD/internal/wwd_wifi_sleep.c"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD/wwd.h"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD/wwd_wiced.h"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/internal/wifi.c"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/network/LwIP/WWD/wwd_buffer.c"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/network/LwIP/WWD/wwd_buffer.h"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/network/LwIP/WWD/wwd_network.c"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/network/LwIP/WWD/wwd_network.h"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/platform/MCU/LPC/WWD/wwd_SDIO.c"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/platform/MCU/LPC/WWD/wwd_platform.c"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/platform/MCU/LPC/platform_mcu_peripheral.h"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/platform/MCU/wwd_platform_common.h"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/platform/MCU/wwd_platform_separate_mcu.c"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/platform/MCU/wwd_resources.c"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/platform/include/platform_cache_def.h"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/platform/include/platform_peripheral.h"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/platform/platform_toolchain.h"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/generated_mac_address.txt"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/include/wiced_resource.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/freertos_kernel/event_groups.c"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/freertos_kernel/include/FreeRTOS.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/freertos_kernel/include/atomic.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/freertos_kernel/include/deprecated_definitions.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/freertos_kernel/include/event_groups.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/freertos_kernel/include/list.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/freertos_kernel/include/message_buffer.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/freertos_kernel/include/mpu_prototypes.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/freertos_kernel/include/mpu_wrappers.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/freertos_kernel/include/portable.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/freertos_kernel/include/projdefs.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/freertos_kernel/include/queue.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/freertos_kernel/include/semphr.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/freertos_kernel/include/stack_macros.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/freertos_kernel/include/stream_buffer.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/freertos_kernel/include/task.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/freertos_kernel/include/timers.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/freertos_kernel/list.c"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/freertos_kernel/portable/GCC/ARM_CM4F/port.c"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/freertos_kernel/portable/GCC/ARM_CM4F/portmacro.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/freertos_kernel/portable/MemMang/ReadMe.url"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/freertos_kernel/queue.c"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/freertos_kernel/stream_buffer.c"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/freertos_kernel/tasks.c"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/freertos_kernel/timers.c"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/freertos_kernel/include/freertos_tasks_c_additions.h"
"${ProjDirPath}/../../../../../middleware/sdmmc/inc/fsl_sdmmc_spec.h"
"${ProjDirPath}/../../../../../devices/MIMXRT1052/utilities/fsl_sbrk.c"
)


set(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_DEBUG} -T${ProjDirPath}/MIMXRT1052xxxxx_flexspi_nor.ld -static")

set(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_RELEASE} -T${ProjDirPath}/MIMXRT1052xxxxx_flexspi_nor.ld -static")

TARGET_LINK_LIBRARIES(google_iot_hello_wifi.elf -Wl,--start-group)
target_link_libraries(google_iot_hello_wifi.elf optimized m)

target_link_libraries(google_iot_hello_wifi.elf optimized c)

target_link_libraries(google_iot_hello_wifi.elf optimized gcc)

target_link_libraries(google_iot_hello_wifi.elf optimized nosys)

target_link_libraries(google_iot_hello_wifi.elf optimized m)

target_link_libraries(google_iot_hello_wifi.elf optimized c)

target_link_libraries(google_iot_hello_wifi.elf optimized gcc)

target_link_libraries(google_iot_hello_wifi.elf optimized nosys)

TARGET_LINK_LIBRARIES(google_iot_hello_wifi.elf -Wl,--end-group)


