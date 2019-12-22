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

SET(CMAKE_ASM_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_ASM_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -mcpu=cortex-m7")

SET(CMAKE_ASM_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_ASM_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -Wall")

SET(CMAKE_ASM_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_ASM_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -mfloat-abi=hard")

SET(CMAKE_ASM_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_ASM_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -mfpu=fpv5-d16")

SET(CMAKE_ASM_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_ASM_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -mthumb")

SET(CMAKE_ASM_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_ASM_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -fno-common")

SET(CMAKE_ASM_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_ASM_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -ffunction-sections")

SET(CMAKE_ASM_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_ASM_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -fdata-sections")

SET(CMAKE_ASM_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_ASM_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -ffreestanding")

SET(CMAKE_ASM_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_ASM_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -fno-builtin")

SET(CMAKE_ASM_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_ASM_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -mapcs")

SET(CMAKE_ASM_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_ASM_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -std=gnu99")

SET(CMAKE_ASM_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_ASM_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -D__STARTUP_CLEAR_BSS")

SET(CMAKE_ASM_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_ASM_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -DNDEBUG")

SET(CMAKE_ASM_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_ASM_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -D__STARTUP_INITIALIZE_NONCACHEDATA")

SET(CMAKE_ASM_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_ASM_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -mcpu=cortex-m7")

SET(CMAKE_ASM_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_ASM_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -Wall")

SET(CMAKE_ASM_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_ASM_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -mfloat-abi=hard")

SET(CMAKE_ASM_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_ASM_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -mfpu=fpv5-d16")

SET(CMAKE_ASM_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_ASM_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -mthumb")

SET(CMAKE_ASM_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_ASM_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -fno-common")

SET(CMAKE_ASM_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_ASM_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -ffunction-sections")

SET(CMAKE_ASM_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_ASM_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -fdata-sections")

SET(CMAKE_ASM_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_ASM_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -ffreestanding")

SET(CMAKE_ASM_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_ASM_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -fno-builtin")

SET(CMAKE_ASM_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_ASM_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -mapcs")

SET(CMAKE_ASM_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_ASM_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -std=gnu99")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -DXIP_EXTERNAL_FLASH=1")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -DXIP_BOOT_HEADER_ENABLE=1")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -DXIP_BOOT_HEADER_DCD_ENABLE=1")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -DSKIP_SYSCLK_INIT")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -DDEBUG")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -DCPU_MIMXRT1052DVL6B")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -DSDK_DEBUGCONSOLE_UART")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -DFLATBUFFERS_LITTLEENDIAN")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -DTFLITE_MCU")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -DARM_MATH_CM7")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -D__FPU_PRESENT=1")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -DSDK_I2C_BASED_COMPONENT_USED=1")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -DSERIAL_PORT_TYPE_UART=1")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -g")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -O0")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -mcpu=cortex-m7")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -Wall")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -mfloat-abi=hard")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -mfpu=fpv5-d16")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -mthumb")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -MMD")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -MP")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -fno-common")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -ffunction-sections")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -fdata-sections")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -ffreestanding")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -fno-builtin")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -mapcs")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -std=gnu99")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -DXIP_EXTERNAL_FLASH=1")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -DXIP_BOOT_HEADER_ENABLE=1")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -DXIP_BOOT_HEADER_DCD_ENABLE=1")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -DSKIP_SYSCLK_INIT")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -DNDEBUG")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -DCPU_MIMXRT1052DVL6B")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -DSDK_DEBUGCONSOLE_UART")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -DFLATBUFFERS_LITTLEENDIAN")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -DTFLITE_MCU")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -DARM_MATH_CM7")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -D__FPU_PRESENT=1")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -DSDK_I2C_BASED_COMPONENT_USED=1")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -DSERIAL_PORT_TYPE_UART=1")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -Os")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -mcpu=cortex-m7")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -Wall")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -mfloat-abi=hard")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -mfpu=fpv5-d16")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -mthumb")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -MMD")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -MP")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -fno-common")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -ffunction-sections")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -fdata-sections")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -ffreestanding")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -fno-builtin")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -mapcs")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -std=gnu99")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -DDEBUG")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -DSDK_DEBUGCONSOLE_UART")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -DFLATBUFFERS_LITTLEENDIAN")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -DTFLITE_MCU")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -DARM_MATH_CM7")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -D__FPU_PRESENT=1")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -DCPU_MIMXRT1052DVL6B")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -DSDK_I2C_BASED_COMPONENT_USED=1")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -DSERIAL_PORT_TYPE_UART=1")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -g")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -O0")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -Wno-sign-compare")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -mcpu=cortex-m7")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -Wall")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -mfloat-abi=hard")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -mfpu=fpv5-d16")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -mthumb")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -MMD")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -MP")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -fno-common")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -ffunction-sections")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -fdata-sections")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -ffreestanding")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -fno-builtin")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -mapcs")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -fno-rtti")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -fno-exceptions")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -DNDEBUG")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -DSDK_DEBUGCONSOLE_UART")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -DFLATBUFFERS_LITTLEENDIAN")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -DTFLITE_MCU")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -DARM_MATH_CM7")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -D__FPU_PRESENT=1")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -DCPU_MIMXRT1052DVL6B")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -DSDK_I2C_BASED_COMPONENT_USED=1")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -DSERIAL_PORT_TYPE_UART=1")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -Os")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -Wno-sign-compare")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -mcpu=cortex-m7")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -Wall")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -mfloat-abi=hard")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -mfpu=fpv5-d16")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -mthumb")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -MMD")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -MP")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -fno-common")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -ffunction-sections")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -fdata-sections")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -ffreestanding")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -fno-builtin")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -mapcs")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -fno-rtti")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -fno-exceptions")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -g")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -mcpu=cortex-m7")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -Wall")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -mfloat-abi=hard")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -mfpu=fpv5-d16")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} --specs=nosys.specs")

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

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} --defsym=__stack_size__=0x10000")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} --defsym=__heap_size__=0x800000")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -mcpu=cortex-m7")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -Wall")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -mfloat-abi=hard")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -mfpu=fpv5-d16")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} --specs=nosys.specs")

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

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} --defsym=__stack_size__=0x10000")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} --defsym=__heap_size__=0x800000")

include_directories(${ProjDirPath}/../../../../../CMSIS/Include)

include_directories(${ProjDirPath}/../../../../../devices)

include_directories(${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/examples/cifar10)

include_directories(${ProjDirPath}/../../..)

include_directories(${ProjDirPath}/..)

include_directories(${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/examples/common/source)

include_directories(${ProjDirPath}/../../../../../components/video/camera/device)

include_directories(${ProjDirPath}/../../../../../components/video/camera/receiver)

include_directories(${ProjDirPath}/../../../../../components/video/camera/device/sccb)

include_directories(${ProjDirPath}/../../../../../components/video/camera/device/ov7725)

include_directories(${ProjDirPath}/../../../../../components/video/camera/device/mt9m114)

include_directories(${ProjDirPath}/../../../../../components/video/camera)

include_directories(${ProjDirPath}/../../../../../components/video/camera/receiver/csi)

include_directories(${ProjDirPath}/../../../../../devices/MIMXRT1052/drivers)

include_directories(${ProjDirPath}/../../../../../components/video)

include_directories(${ProjDirPath}/../../../../../components/video/i2c)

include_directories(${ProjDirPath}/../../../../../components/video/display/dc)

include_directories(${ProjDirPath}/../../../../../components/video/display/dc/elcdif)

include_directories(${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite)

include_directories(${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/third_party/flatbuffers/include)

include_directories(${ProjDirPath}/../../../../../components/lists)

include_directories(${ProjDirPath}/../../../../../components/uart)

include_directories(${ProjDirPath}/../../../../../components/serial_manager)

include_directories(${ProjDirPath}/../../../../../devices/MIMXRT1052)

include_directories(${ProjDirPath}/../../../../../devices/MIMXRT1052/xip)

include_directories(${ProjDirPath}/../../../xip)

include_directories(${ProjDirPath}/../../../../../devices/MIMXRT1052/utilities/str)

include_directories(${ProjDirPath}/../../../../../devices/MIMXRT1052/utilities/debug_console)

include_directories(${ProjDirPath}/../../../../../devices/MIMXRT1052/utilities)

add_executable(tensorflow_lite_cifar10.elf 
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/examples/cifar10/cifar10.cpp"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/examples/common/source/image.c"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/examples/common/source/image.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/examples/cifar10/ship.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/examples/cifar10/cifar10_model.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/examples/common/source/get_top_n.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/examples/common/source/get_top_n_impl.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/examples/common/source/timer.c"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/examples/common/source/timer.h"
"${ProjDirPath}/../../../display_support.h"
"${ProjDirPath}/../../../display_support.c"
"${ProjDirPath}/../../../camera_support.h"
"${ProjDirPath}/../../../camera_support.c"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
"${ProjDirPath}/../../../../../components/video/camera/device/fsl_camera_device.h"
"${ProjDirPath}/../../../../../components/video/camera/receiver/fsl_camera_receiver.h"
"${ProjDirPath}/../../../../../components/video/camera/device/sccb/fsl_sccb.c"
"${ProjDirPath}/../../../../../components/video/camera/device/sccb/fsl_sccb.h"
"${ProjDirPath}/../../../../../components/video/camera/device/ov7725/fsl_ov7725.c"
"${ProjDirPath}/../../../../../components/video/camera/device/ov7725/fsl_ov7725.h"
"${ProjDirPath}/../../../../../components/video/camera/device/mt9m114/fsl_mt9m114.c"
"${ProjDirPath}/../../../../../components/video/camera/device/mt9m114/fsl_mt9m114.h"
"${ProjDirPath}/../../../../../components/video/camera/fsl_camera.h"
"${ProjDirPath}/../../../../../components/video/camera/receiver/csi/fsl_csi_camera_adapter.c"
"${ProjDirPath}/../../../../../components/video/camera/receiver/csi/fsl_csi_camera_adapter.h"
"${ProjDirPath}/../../../../../devices/MIMXRT1052/drivers/fsl_csi.c"
"${ProjDirPath}/../../../../../devices/MIMXRT1052/drivers/fsl_csi.h"
"${ProjDirPath}/../../../../../components/video/fsl_video_common.c"
"${ProjDirPath}/../../../../../components/video/fsl_video_common.h"
"${ProjDirPath}/../../../../../components/video/i2c/fsl_video_i2c.c"
"${ProjDirPath}/../../../../../components/video/i2c/fsl_video_i2c.h"
"${ProjDirPath}/../../../../../devices/MIMXRT1052/drivers/fsl_elcdif.c"
"${ProjDirPath}/../../../../../devices/MIMXRT1052/drivers/fsl_elcdif.h"
"${ProjDirPath}/../../../../../devices/MIMXRT1052/drivers/fsl_pxp.c"
"${ProjDirPath}/../../../../../devices/MIMXRT1052/drivers/fsl_pxp.h"
"${ProjDirPath}/../../../../../components/video/display/dc/fsl_dc_fb.h"
"${ProjDirPath}/../../../../../components/video/display/dc/elcdif/fsl_dc_fb_elcdif.c"
"${ProjDirPath}/../../../../../components/video/display/dc/elcdif/fsl_dc_fb_elcdif.h"
"${ProjDirPath}/../../../../../devices/MIMXRT1052/drivers/fsl_gpio.c"
"${ProjDirPath}/../../../../../devices/MIMXRT1052/drivers/fsl_gpio.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/tensorflow/core/public/session.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/tensorflow/core/public/version.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/allocation.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/arena_planner.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/builtin_op_data.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/builtin_ops.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/c/builtin_op_data.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/c/c_api_internal.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/context.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/context_util.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/core/api/error_reporter.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/core/api/flatbuffer_conversions.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/core/api/op_resolver.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/core/api/profiler.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/core/subgraph.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/error_reporter.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/graph_info.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/interpreter.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/activation_functor.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/cpu_backend_context.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/cpu_backend_gemm.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/cpu_backend_gemm_custom_gemv.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/cpu_backend_gemm_eigen.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/cpu_backend_gemm_gemmlowp.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/cpu_backend_gemm_params.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/cpu_backend_gemm_ruy.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/cpu_backend_support.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/cpu_backend_threadpool.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/eigen_support.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/gemmlowp_support.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/reference/integer_ops/add.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/reference/integer_ops/conv.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/reference/integer_ops/depthwise_conv.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/reference/integer_ops/dequantize.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/reference/integer_ops/fully_connected.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/reference/integer_ops/l2normalization.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/reference/integer_ops/log_softmax.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/reference/integer_ops/logistic.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/reference/integer_ops/mean.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/reference/integer_ops/mul.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/reference/integer_ops/pooling.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/reference/integer_ops/softmax.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/internal/reference/integer_ops/tanh.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/kernel_util.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/lstm_eval.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/op_macros.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/padding.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/register.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/kernels/register_ref.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/memory_planner.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/minimal_logging.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/model.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/mutable_op_resolver.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/op_resolver.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/optional_debug_tools.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/profiling/profile_buffer.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/profiling/profile_summarizer.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/profiling/profiler.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/profiling/time.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/schema/schema_generated.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/simple_memory_arena.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/stderr_reporter.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/string.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/string_util.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/util.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/tensorflow/lite/version.h"
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
"${ProjDirPath}/../../../../../devices/MIMXRT1052/drivers/fsl_common.c"
"${ProjDirPath}/../../../../../devices/MIMXRT1052/drivers/fsl_common.h"
"${ProjDirPath}/../../../../../components/serial_manager/serial_port_uart.c"
"${ProjDirPath}/../../../../../components/serial_manager/serial_port_uart.h"
"${ProjDirPath}/../../../../../devices/MIMXRT1052/drivers/fsl_lpuart.c"
"${ProjDirPath}/../../../../../devices/MIMXRT1052/drivers/fsl_lpuart.h"
"${ProjDirPath}/../../../../../devices/MIMXRT1052/MIMXRT1052.h"
"${ProjDirPath}/../../../../../devices/MIMXRT1052/MIMXRT1052_features.h"
"${ProjDirPath}/../../../../../devices/MIMXRT1052/fsl_device_registers.h"
"${ProjDirPath}/../../../../../devices/MIMXRT1052/gcc/startup_MIMXRT1052.S"
"${ProjDirPath}/../../../../../devices/MIMXRT1052/system_MIMXRT1052.c"
"${ProjDirPath}/../../../../../devices/MIMXRT1052/system_MIMXRT1052.h"
"${ProjDirPath}/../../../../../devices/MIMXRT1052/drivers/fsl_clock.c"
"${ProjDirPath}/../../../../../devices/MIMXRT1052/drivers/fsl_clock.h"
"${ProjDirPath}/../../../../../devices/MIMXRT1052/drivers/fsl_iomuxc.h"
"${ProjDirPath}/../../../../../devices/MIMXRT1052/drivers/fsl_lpi2c.c"
"${ProjDirPath}/../../../../../devices/MIMXRT1052/drivers/fsl_lpi2c.h"
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
"${ProjDirPath}/../../../../../devices/MIMXRT1052/utilities/fsl_sbrk.c"
)


set(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -T${ProjDirPath}/MIMXRT1052xxxxx_flexspi_nor_sdram.ld -static")

set(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -T${ProjDirPath}/MIMXRT1052xxxxx_flexspi_nor_sdram.ld -static")

TARGET_LINK_LIBRARIES(tensorflow_lite_cifar10.elf -Wl,--start-group)
target_link_libraries(tensorflow_lite_cifar10.elf optimized m)

target_link_libraries(tensorflow_lite_cifar10.elf optimized c)

target_link_libraries(tensorflow_lite_cifar10.elf optimized gcc)

target_link_libraries(tensorflow_lite_cifar10.elf optimized nosys)

target_link_libraries(tensorflow_lite_cifar10.elf optimized m)

target_link_libraries(tensorflow_lite_cifar10.elf optimized c)

target_link_libraries(tensorflow_lite_cifar10.elf optimized gcc)

target_link_libraries(tensorflow_lite_cifar10.elf optimized nosys)

link_directories(${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/lib/cm7/gcc)

target_link_libraries(tensorflow_lite_cifar10.elf optimized ${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/lib/cm7/gcc/libtensorflow-lite.a)

link_directories(${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/lib/cm7/gcc)

target_link_libraries(tensorflow_lite_cifar10.elf optimized ${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/lib/cm7/gcc/libtensorflow-lite.a)

TARGET_LINK_LIBRARIES(tensorflow_lite_cifar10.elf -Wl,--end-group)


