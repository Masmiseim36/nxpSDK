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


SET(CMAKE_ASM_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_ASM_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -D__STARTUP_CLEAR_BSS")

SET(CMAKE_ASM_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_ASM_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -DDEBUG")

SET(CMAKE_ASM_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_ASM_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -D__STARTUP_INITIALIZE_NONCACHEDATA")

SET(CMAKE_ASM_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_ASM_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -g")

SET(CMAKE_ASM_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_ASM_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -Wall")

SET(CMAKE_ASM_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_ASM_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -fno-common")

SET(CMAKE_ASM_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_ASM_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -ffunction-sections")

SET(CMAKE_ASM_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_ASM_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -fdata-sections")

SET(CMAKE_ASM_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_ASM_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -ffreestanding")

SET(CMAKE_ASM_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_ASM_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -fno-builtin")

SET(CMAKE_ASM_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_ASM_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -mthumb")

SET(CMAKE_ASM_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_ASM_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -mapcs")

SET(CMAKE_ASM_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_ASM_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -std=gnu99")

SET(CMAKE_ASM_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_ASM_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -mcpu=cortex-m7")

SET(CMAKE_ASM_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_ASM_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -mfloat-abi=hard")

SET(CMAKE_ASM_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_ASM_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -mfpu=fpv5-d16")

SET(CMAKE_ASM_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_ASM_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -D__STARTUP_CLEAR_BSS")

SET(CMAKE_ASM_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_ASM_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -DNDEBUG")

SET(CMAKE_ASM_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_ASM_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -D__STARTUP_INITIALIZE_NONCACHEDATA")

SET(CMAKE_ASM_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_ASM_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -Wall")

SET(CMAKE_ASM_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_ASM_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -fno-common")

SET(CMAKE_ASM_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_ASM_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -ffunction-sections")

SET(CMAKE_ASM_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_ASM_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -fdata-sections")

SET(CMAKE_ASM_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_ASM_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -ffreestanding")

SET(CMAKE_ASM_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_ASM_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -fno-builtin")

SET(CMAKE_ASM_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_ASM_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -mthumb")

SET(CMAKE_ASM_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_ASM_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -mapcs")

SET(CMAKE_ASM_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_ASM_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -std=gnu99")

SET(CMAKE_ASM_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_ASM_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -mcpu=cortex-m7")

SET(CMAKE_ASM_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_ASM_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -mfloat-abi=hard")

SET(CMAKE_ASM_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_ASM_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -mfpu=fpv5-d16")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -DXIP_EXTERNAL_FLASH=1")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -DXIP_BOOT_HEADER_ENABLE=1")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -DXIP_BOOT_HEADER_DCD_ENABLE=1")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -DSKIP_SYSCLK_INIT")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -DDEBUG")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -DCPU_MIMXRT1052DVL6B")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -DSDK_DEBUGCONSOLE=0")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -DSDK_DEBUGCONSOLE_UART")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -DFLATBUFFERS_LITTLEENDIAN")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -DTFLITE_MCU")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -g")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -O0")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -Wall")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -fno-common")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -ffunction-sections")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -fdata-sections")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -ffreestanding")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -fno-builtin")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -mthumb")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -mapcs")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -std=gnu99")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -mcpu=cortex-m7")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -mfloat-abi=hard")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -mfpu=fpv5-d16")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -MMD")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -MP")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -DXIP_EXTERNAL_FLASH=1")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -DXIP_BOOT_HEADER_ENABLE=1")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -DXIP_BOOT_HEADER_DCD_ENABLE=1")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -DSKIP_SYSCLK_INIT")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -DNDEBUG")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -DCPU_MIMXRT1052DVL6B")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -DSDK_DEBUGCONSOLE=0")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -DSDK_DEBUGCONSOLE_UART")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -DFLATBUFFERS_LITTLEENDIAN")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -DTFLITE_MCU")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -Os")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -Wall")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -fno-common")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -ffunction-sections")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -fdata-sections")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -ffreestanding")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -fno-builtin")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -mthumb")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -mapcs")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -std=gnu99")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -mcpu=cortex-m7")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -mfloat-abi=hard")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -mfpu=fpv5-d16")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -MMD")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -MP")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -DDEBUG")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -DSDK_DEBUGCONSOLE=0")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -DSDK_DEBUGCONSOLE_UART")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -DFLATBUFFERS_LITTLEENDIAN")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -DTFLITE_MCU")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -DCPU_MIMXRT1052DVL6B")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -g")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -O0")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -Wno-sign-compare")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -Wall")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -fno-common")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -ffunction-sections")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -fdata-sections")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -ffreestanding")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -fno-builtin")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -mthumb")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -mapcs")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -fno-rtti")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -fno-exceptions")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -mcpu=cortex-m7")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -mfloat-abi=hard")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -mfpu=fpv5-d16")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -MMD")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -MP")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -DNDEBUG")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -DSDK_DEBUGCONSOLE=0")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -DSDK_DEBUGCONSOLE_UART")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -DFLATBUFFERS_LITTLEENDIAN")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -DTFLITE_MCU")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -DCPU_MIMXRT1052DVL6B")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -Os")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -Wno-sign-compare")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -Wall")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -fno-common")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -ffunction-sections")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -fdata-sections")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -ffreestanding")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -fno-builtin")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -mthumb")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -mapcs")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -fno-rtti")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -fno-exceptions")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -mcpu=cortex-m7")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -mfloat-abi=hard")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -mfpu=fpv5-d16")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -MMD")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -MP")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -g")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} --specs=nosys.specs")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -Wall")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -fno-common")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -ffunction-sections")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -fdata-sections")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -ffreestanding")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -fno-builtin")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -mthumb")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -mapcs")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} --gc-sections")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -static")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -z")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} muldefs")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -Map=output.map")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -mcpu=cortex-m7")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -mfloat-abi=hard")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -mfpu=fpv5-d16")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} --defsym=__stack_size__=0x10000")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} --defsym=__heap_size__=0x800000")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} --specs=nosys.specs")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -Wall")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -fno-common")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -ffunction-sections")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -fdata-sections")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -ffreestanding")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -fno-builtin")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -mthumb")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -mapcs")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} --gc-sections")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -static")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -z")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} muldefs")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -Map=output.map")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -mcpu=cortex-m7")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -mfloat-abi=hard")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -mfpu=fpv5-d16")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} --defsym=__stack_size__=0x10000")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} --defsym=__heap_size__=0x800000")

include_directories(${ProjDirPath}/../../../../../CMSIS/Include)

include_directories(${ProjDirPath}/../../../../../devices)

include_directories(${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/examples/label_image)

include_directories(${ProjDirPath}/..)

include_directories(${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite)

include_directories(${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/third_party/flatbuffers/include)

include_directories(${ProjDirPath}/../../../../../components/lists)

include_directories(${ProjDirPath}/../../../../../components/uart)

include_directories(${ProjDirPath}/../../../../../components/serial_manager)

include_directories(${ProjDirPath}/../../../../../devices/MIMXRT1052/drivers)

include_directories(${ProjDirPath}/../../../../../devices/MIMXRT1052)

include_directories(${ProjDirPath}/../../../../../devices/MIMXRT1052/xip)

include_directories(${ProjDirPath}/../../../xip)

include_directories(${ProjDirPath}/../../../../../devices/MIMXRT1052/utilities/str)

include_directories(${ProjDirPath}/../../../../../devices/MIMXRT1052/utilities/debug_console)

add_executable(tensorflow_lite_label_image.elf 
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/examples/label_image/bitmap_helpers.cpp"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/examples/label_image/bitmap_helpers.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/examples/label_image/bitmap_helpers_impl.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/examples/label_image/get_top_n.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/examples/label_image/get_top_n_impl.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/examples/label_image/labels.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/examples/label_image/label_image.cpp"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/examples/label_image/label_image.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/examples/label_image/mobilenet_v1_0.25_128_quant_model.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/examples/label_image/stopwatch_image.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/examples/label_image/timer.c"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/examples/label_image/timer.h"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/tensorflow/contrib/lite/_string.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/tensorflow/contrib/lite/allocation.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/tensorflow/contrib/lite/arena_planner.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/tensorflow/contrib/lite/builtin_op_data.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/tensorflow/contrib/lite/builtin_ops.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/tensorflow/contrib/lite/context.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/tensorflow/contrib/lite/context_util.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/tensorflow/contrib/lite/error_reporter.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/tensorflow/contrib/lite/graph_info.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/tensorflow/contrib/lite/interpreter.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/tensorflow/contrib/lite/kernels/register.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/tensorflow/contrib/lite/memory_planner.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/tensorflow/contrib/lite/model.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/tensorflow/contrib/lite/nnapi_delegate.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/tensorflow/contrib/lite/op_resolver.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/tensorflow/contrib/lite/optional_debug_tools.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/tensorflow/contrib/lite/profiling/_time.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/tensorflow/contrib/lite/profiling/profile_buffer.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/tensorflow/contrib/lite/profiling/profile_summarizer.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/tensorflow/contrib/lite/profiling/profiler.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/tensorflow/contrib/lite/schema/schema_generated.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/tensorflow/contrib/lite/simple_memory_arena.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/tensorflow/contrib/lite/string_util.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/tensorflow/contrib/lite/util.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/tensorflow/contrib/lite/version.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/third_party/flatbuffers/include/flatbuffers/base.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/third_party/flatbuffers/include/flatbuffers/code_generators.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/third_party/flatbuffers/include/flatbuffers/flatbuffers.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/third_party/flatbuffers/include/flatbuffers/flexbuffers.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/third_party/flatbuffers/include/flatbuffers/hash.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/third_party/flatbuffers/include/flatbuffers/idl.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/third_party/flatbuffers/include/flatbuffers/minireflect.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/third_party/flatbuffers/include/flatbuffers/reflection.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/third_party/flatbuffers/include/flatbuffers/reflection_generated.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/third_party/flatbuffers/include/flatbuffers/registry.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/third_party/flatbuffers/include/flatbuffers/stl_emulation.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/third_party/flatbuffers/include/flatbuffers/util.h"
"${ProjDirPath}/../../../../../components/lists/generic_list.c"
"${ProjDirPath}/../../../../../components/lists/generic_list.h"
"${ProjDirPath}/../../../../../components/uart/lpuart_adapter.c"
"${ProjDirPath}/../../../../../components/uart/uart.h"
"${ProjDirPath}/../../../../../components/serial_manager/serial_manager.c"
"${ProjDirPath}/../../../../../components/serial_manager/serial_manager.h"
"${ProjDirPath}/../../../../../components/serial_manager/serial_port_internal.h"
"${ProjDirPath}/../../../../../components/serial_manager/serial_port_uart.c"
"${ProjDirPath}/../../../../../components/serial_manager/serial_port_uart.h"
"${ProjDirPath}/../../../../../devices/MIMXRT1052/drivers/fsl_lpuart.c"
"${ProjDirPath}/../../../../../devices/MIMXRT1052/drivers/fsl_lpuart.h"
"${ProjDirPath}/../../../../../devices/MIMXRT1052/MIMXRT1052.h"
"${ProjDirPath}/../../../../../devices/MIMXRT1052/MIMXRT1052_features.h"
"${ProjDirPath}/../../../../../devices/MIMXRT1052/fsl_device_registers.h"
"${ProjDirPath}/../../../../../devices/MIMXRT1052/system_MIMXRT1052.c"
"${ProjDirPath}/../../../../../devices/MIMXRT1052/system_MIMXRT1052.h"
"${ProjDirPath}/../../../../../devices/MIMXRT1052/gcc/startup_MIMXRT1052.S"
"${ProjDirPath}/../../../../../devices/MIMXRT1052/drivers/fsl_clock.c"
"${ProjDirPath}/../../../../../devices/MIMXRT1052/drivers/fsl_clock.h"
"${ProjDirPath}/../../../../../devices/MIMXRT1052/drivers/fsl_common.c"
"${ProjDirPath}/../../../../../devices/MIMXRT1052/drivers/fsl_common.h"
"${ProjDirPath}/../../../../../devices/MIMXRT1052/drivers/fsl_gpio.c"
"${ProjDirPath}/../../../../../devices/MIMXRT1052/drivers/fsl_gpio.h"
"${ProjDirPath}/../../../../../devices/MIMXRT1052/drivers/fsl_iomuxc.h"
"${ProjDirPath}/../../../../../devices/MIMXRT1052/xip/fsl_flexspi_nor_boot.c"
"${ProjDirPath}/../../../../../devices/MIMXRT1052/xip/fsl_flexspi_nor_boot.h"
"${ProjDirPath}/../../../xip/evkbimxrt1050_flexspi_nor_config.c"
"${ProjDirPath}/../../../xip/evkbimxrt1050_flexspi_nor_config.h"
"${ProjDirPath}/../../../xip/evkbimxrt1050_sdram_ini_dcd.c"
"${ProjDirPath}/../../../xip/evkbimxrt1050_sdram_ini_dcd.h"
"${ProjDirPath}/../../../../../devices/MIMXRT1052/utilities/fsl_assert.c"
"${ProjDirPath}/../../../../../devices/MIMXRT1052/utilities/debug_console/fsl_debug_console.c"
"${ProjDirPath}/../../../../../devices/MIMXRT1052/utilities/debug_console/fsl_debug_console.h"
"${ProjDirPath}/../../../../../devices/MIMXRT1052/utilities/debug_console/fsl_debug_console_conf.h"
"${ProjDirPath}/../../../../../devices/MIMXRT1052/utilities/str/fsl_str.c"
"${ProjDirPath}/../../../../../devices/MIMXRT1052/utilities/str/fsl_str.h"
"${ProjDirPath}/../../../../../CMSIS/Include/core_cm7.h"
"${ProjDirPath}/../../../../../CMSIS/Include/arm_common_tables.h"
"${ProjDirPath}/../../../../../CMSIS/Include/arm_const_structs.h"
"${ProjDirPath}/../../../../../CMSIS/Include/arm_math.h"
"${ProjDirPath}/../../../../../CMSIS/Include/cmsis_armcc.h"
"${ProjDirPath}/../../../../../CMSIS/Include/cmsis_armclang.h"
"${ProjDirPath}/../../../../../CMSIS/Include/cmsis_compiler.h"
"${ProjDirPath}/../../../../../CMSIS/Include/cmsis_gcc.h"
"${ProjDirPath}/../../../../../CMSIS/Include/cmsis_iccarm.h"
"${ProjDirPath}/../../../../../CMSIS/Include/cmsis_version.h"
"${ProjDirPath}/../../../../../CMSIS/Include/core_armv8mbl.h"
"${ProjDirPath}/../../../../../CMSIS/Include/core_armv8mml.h"
"${ProjDirPath}/../../../../../CMSIS/Include/mpu_armv7.h"
"${ProjDirPath}/../../../../../CMSIS/Include/mpu_armv8.h"
)


set(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -T${ProjDirPath}/MIMXRT1052xxxxx_flexspi_nor_sdram.ld -static")

set(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -T${ProjDirPath}/MIMXRT1052xxxxx_flexspi_nor_sdram.ld -static")

TARGET_LINK_LIBRARIES(tensorflow_lite_label_image.elf -Wl,--start-group)
target_link_libraries(tensorflow_lite_label_image.elf optimized m)

target_link_libraries(tensorflow_lite_label_image.elf optimized c)

target_link_libraries(tensorflow_lite_label_image.elf optimized gcc)

target_link_libraries(tensorflow_lite_label_image.elf optimized nosys)

target_link_libraries(tensorflow_lite_label_image.elf optimized m)

target_link_libraries(tensorflow_lite_label_image.elf optimized c)

target_link_libraries(tensorflow_lite_label_image.elf optimized gcc)

target_link_libraries(tensorflow_lite_label_image.elf optimized nosys)

link_directories(${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/lib/cm7/gcc)

target_link_libraries(tensorflow_lite_label_image.elf optimized ${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/lib/cm7/gcc/libtensorflow-lite.a)

link_directories(${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/lib/cm7/gcc)

target_link_libraries(tensorflow_lite_label_image.elf optimized ${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/lib/cm7/gcc/libtensorflow-lite.a)

TARGET_LINK_LIBRARIES(tensorflow_lite_label_image.elf -Wl,--end-group)

