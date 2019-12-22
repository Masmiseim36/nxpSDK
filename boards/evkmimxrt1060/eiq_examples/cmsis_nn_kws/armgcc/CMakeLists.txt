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

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -DXIP_EXTERNAL_FLASH=1")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -DXIP_BOOT_HEADER_ENABLE=1")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -DXIP_BOOT_HEADER_DCD_ENABLE=1")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -DSKIP_SYSCLK_INIT")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -DNDEBUG")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -DCPU_MIMXRT1062DVL6A")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -DBOARD_USE_CODEC=1")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -DARM_MATH_CM7")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -D__FPU_PRESENT=1")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -DSDK_I2C_BASED_COMPONENT_USED=1")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -DCODEC_WM8960_ENABLE")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -DSERIAL_PORT_TYPE_UART=1")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -O2")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -fno-strict-aliasing")

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

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -DXIP_EXTERNAL_FLASH=1")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -DXIP_BOOT_HEADER_ENABLE=1")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -DXIP_BOOT_HEADER_DCD_ENABLE=1")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -DSKIP_SYSCLK_INIT")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -DDEBUG")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -DCPU_MIMXRT1062DVL6A")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -DBOARD_USE_CODEC=1")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -DARM_MATH_CM7")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -D__FPU_PRESENT=1")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -DSDK_I2C_BASED_COMPONENT_USED=1")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -DCODEC_WM8960_ENABLE")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -DSERIAL_PORT_TYPE_UART=1")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -g")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -O0")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -fno-strict-aliasing")

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

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -DNDEBUG")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -DBOARD_USE_CODEC=1")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -DARM_MATH_CM7")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -D__FPU_PRESENT=1")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -DCPU_MIMXRT1062DVL6A")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -DSDK_I2C_BASED_COMPONENT_USED=1")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -DSERIAL_PORT_TYPE_UART=1")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -O2")

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

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -DDEBUG")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -DBOARD_USE_CODEC=1")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -DARM_MATH_CM7")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -D__FPU_PRESENT=1")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -DCPU_MIMXRT1062DVL6A")

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

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -mcpu=cortex-m7")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -Wall")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -mfloat-abi=hard")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -mfpu=fpv5-d16")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} --specs=nano.specs")

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

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -g")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -mcpu=cortex-m7")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -Wall")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -mfloat-abi=hard")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -mfpu=fpv5-d16")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} --specs=nano.specs")

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

include_directories(${ProjDirPath}/../../../../../CMSIS/Include)

include_directories(${ProjDirPath}/../../../../../devices)

include_directories(${ProjDirPath}/../../../../../middleware/eiq/cmsis-nn/Examples/kws)

include_directories(${ProjDirPath}/../../../../../middleware/eiq/cmsis-nn/Examples/kws/KWS)

include_directories(${ProjDirPath}/../../../../../middleware/eiq/cmsis-nn/Examples/kws/KWS_NN)

include_directories(${ProjDirPath}/../../../../../middleware/eiq/cmsis-nn/Examples/kws/local_NN)

include_directories(${ProjDirPath}/../../../../../middleware/eiq/cmsis-nn/Examples/kws/MFCC)

include_directories(${ProjDirPath}/..)

include_directories(${ProjDirPath}/../../../../../middleware/eiq/cmsis-nn/Examples/common/source)

include_directories(${ProjDirPath}/../../../../../components/codec/wm8960)

include_directories(${ProjDirPath}/../../../../../components/codec)

include_directories(${ProjDirPath}/../../../../../components/codec/port)

include_directories(${ProjDirPath}/../../../../../components/i2c)

include_directories(${ProjDirPath}/../../../../../devices/MIMXRT1062/drivers)

include_directories(${ProjDirPath}/../../../../../middleware/eiq/cmsis-nn/Include)

include_directories(${ProjDirPath}/../../../../../components/lists)

include_directories(${ProjDirPath}/../../../../../components/uart)

include_directories(${ProjDirPath}/../../../../../components/serial_manager)

include_directories(${ProjDirPath}/../../../../../devices/MIMXRT1062)

include_directories(${ProjDirPath}/../../../../../devices/MIMXRT1062/xip)

include_directories(${ProjDirPath}/../../../xip)

include_directories(${ProjDirPath}/../../../../../devices/MIMXRT1062/utilities/str)

include_directories(${ProjDirPath}/../../../../../devices/MIMXRT1062/utilities/debug_console)

include_directories(${ProjDirPath}/../../../../../components/codec/i2c)

include_directories(${ProjDirPath}/../../../../../devices/MIMXRT1062/utilities)

add_executable(cmsis_nn_kws.elf 
"${ProjDirPath}/../../../../../middleware/eiq/cmsis-nn/Examples/kws/KWS/kws.cpp"
"${ProjDirPath}/../../../../../middleware/eiq/cmsis-nn/Examples/kws/KWS/kws.h"
"${ProjDirPath}/../../../../../middleware/eiq/cmsis-nn/Examples/kws/KWS/kws_dnn.cpp"
"${ProjDirPath}/../../../../../middleware/eiq/cmsis-nn/Examples/kws/KWS/kws_dnn.h"
"${ProjDirPath}/../../../../../middleware/eiq/cmsis-nn/Examples/kws/KWS/kws_ds_cnn.cpp"
"${ProjDirPath}/../../../../../middleware/eiq/cmsis-nn/Examples/kws/KWS/kws_ds_cnn.h"
"${ProjDirPath}/../../../../../middleware/eiq/cmsis-nn/Examples/kws/KWS_NN/dnn.cpp"
"${ProjDirPath}/../../../../../middleware/eiq/cmsis-nn/Examples/kws/KWS_NN/dnn.h"
"${ProjDirPath}/../../../../../middleware/eiq/cmsis-nn/Examples/kws/KWS_NN/dnn_weights.h"
"${ProjDirPath}/../../../../../middleware/eiq/cmsis-nn/Examples/kws/KWS_NN/ds_cnn.cpp"
"${ProjDirPath}/../../../../../middleware/eiq/cmsis-nn/Examples/kws/KWS_NN/ds_cnn.h"
"${ProjDirPath}/../../../../../middleware/eiq/cmsis-nn/Examples/kws/KWS_NN/ds_cnn_weights.h"
"${ProjDirPath}/../../../../../middleware/eiq/cmsis-nn/Examples/kws/KWS_NN/nn.cpp"
"${ProjDirPath}/../../../../../middleware/eiq/cmsis-nn/Examples/kws/KWS_NN/nn.h"
"${ProjDirPath}/../../../../../middleware/eiq/cmsis-nn/Examples/kws/local_NN/arm_avepool_q7_HWC_nonsquare.c"
"${ProjDirPath}/../../../../../middleware/eiq/cmsis-nn/Examples/kws/local_NN/local_NN.h"
"${ProjDirPath}/../../../../../middleware/eiq/cmsis-nn/Examples/kws/MFCC/mfcc.cpp"
"${ProjDirPath}/../../../../../middleware/eiq/cmsis-nn/Examples/kws/MFCC/mfcc.h"
"${ProjDirPath}/../../../../../middleware/eiq/cmsis-nn/Examples/kws/commands.h"
"${ProjDirPath}/../../../../../middleware/eiq/cmsis-nn/Examples/common/source/timer.h"
"${ProjDirPath}/../../../../../middleware/eiq/cmsis-nn/Examples/common/source/timer.c"
"${ProjDirPath}/../../../../../middleware/eiq/cmsis-nn/Examples/kws/main.cpp"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
"${ProjDirPath}/../../../../../components/codec/wm8960/fsl_wm8960.c"
"${ProjDirPath}/../../../../../components/codec/wm8960/fsl_wm8960.h"
"${ProjDirPath}/../../../../../components/codec/fsl_codec_common.c"
"${ProjDirPath}/../../../../../components/codec/fsl_codec_common.h"
"${ProjDirPath}/../../../../../components/codec/port/fsl_codec_adapter.h"
"${ProjDirPath}/../../../../../components/codec/port/wm8960/fsl_codec_adapter.c"
"${ProjDirPath}/../../../../../components/i2c/i2c.h"
"${ProjDirPath}/../../../../../components/i2c/lpi2c_adapter.c"
"${ProjDirPath}/../../../../../devices/MIMXRT1062/drivers/fsl_sai.c"
"${ProjDirPath}/../../../../../devices/MIMXRT1062/drivers/fsl_sai.h"
"${ProjDirPath}/../../../../../middleware/eiq/cmsis-nn/Include/arm_nn_tables.h"
"${ProjDirPath}/../../../../../middleware/eiq/cmsis-nn/Include/arm_nnfunctions.h"
"${ProjDirPath}/../../../../../middleware/eiq/cmsis-nn/Include/arm_nnsupportfunctions.h"
"${ProjDirPath}/../../../../../middleware/eiq/cmsis-nn/Source/ActivationFunctions/arm_nn_activations_q15.c"
"${ProjDirPath}/../../../../../middleware/eiq/cmsis-nn/Source/ActivationFunctions/arm_nn_activations_q7.c"
"${ProjDirPath}/../../../../../middleware/eiq/cmsis-nn/Source/ActivationFunctions/arm_relu_q15.c"
"${ProjDirPath}/../../../../../middleware/eiq/cmsis-nn/Source/ActivationFunctions/arm_relu_q7.c"
"${ProjDirPath}/../../../../../middleware/eiq/cmsis-nn/Source/ConvolutionFunctions/arm_convolve_1x1_HWC_q7_fast_nonsquare.c"
"${ProjDirPath}/../../../../../middleware/eiq/cmsis-nn/Source/ConvolutionFunctions/arm_convolve_HWC_q15_basic.c"
"${ProjDirPath}/../../../../../middleware/eiq/cmsis-nn/Source/ConvolutionFunctions/arm_convolve_HWC_q15_fast.c"
"${ProjDirPath}/../../../../../middleware/eiq/cmsis-nn/Source/ConvolutionFunctions/arm_convolve_HWC_q15_fast_nonsquare.c"
"${ProjDirPath}/../../../../../middleware/eiq/cmsis-nn/Source/ConvolutionFunctions/arm_convolve_HWC_q7_RGB.c"
"${ProjDirPath}/../../../../../middleware/eiq/cmsis-nn/Source/ConvolutionFunctions/arm_convolve_HWC_q7_basic.c"
"${ProjDirPath}/../../../../../middleware/eiq/cmsis-nn/Source/ConvolutionFunctions/arm_convolve_HWC_q7_basic_nonsquare.c"
"${ProjDirPath}/../../../../../middleware/eiq/cmsis-nn/Source/ConvolutionFunctions/arm_convolve_HWC_q7_fast.c"
"${ProjDirPath}/../../../../../middleware/eiq/cmsis-nn/Source/ConvolutionFunctions/arm_convolve_HWC_q7_fast_nonsquare.c"
"${ProjDirPath}/../../../../../middleware/eiq/cmsis-nn/Source/ConvolutionFunctions/arm_depthwise_conv_u8_basic_ver1.c"
"${ProjDirPath}/../../../../../middleware/eiq/cmsis-nn/Source/ConvolutionFunctions/arm_depthwise_separable_conv_HWC_q7.c"
"${ProjDirPath}/../../../../../middleware/eiq/cmsis-nn/Source/ConvolutionFunctions/arm_depthwise_separable_conv_HWC_q7_nonsquare.c"
"${ProjDirPath}/../../../../../middleware/eiq/cmsis-nn/Source/ConvolutionFunctions/arm_nn_mat_mult_kernel_q7_q15.c"
"${ProjDirPath}/../../../../../middleware/eiq/cmsis-nn/Source/ConvolutionFunctions/arm_nn_mat_mult_kernel_q7_q15_reordered.c"
"${ProjDirPath}/../../../../../middleware/eiq/cmsis-nn/Source/FullyConnectedFunctions/arm_fully_connected_mat_q7_vec_q15.c"
"${ProjDirPath}/../../../../../middleware/eiq/cmsis-nn/Source/FullyConnectedFunctions/arm_fully_connected_mat_q7_vec_q15_opt.c"
"${ProjDirPath}/../../../../../middleware/eiq/cmsis-nn/Source/FullyConnectedFunctions/arm_fully_connected_q15.c"
"${ProjDirPath}/../../../../../middleware/eiq/cmsis-nn/Source/FullyConnectedFunctions/arm_fully_connected_q15_opt.c"
"${ProjDirPath}/../../../../../middleware/eiq/cmsis-nn/Source/FullyConnectedFunctions/arm_fully_connected_q7.c"
"${ProjDirPath}/../../../../../middleware/eiq/cmsis-nn/Source/FullyConnectedFunctions/arm_fully_connected_q7_opt.c"
"${ProjDirPath}/../../../../../middleware/eiq/cmsis-nn/Source/NNSupportFunctions/arm_nn_mult_q15.c"
"${ProjDirPath}/../../../../../middleware/eiq/cmsis-nn/Source/NNSupportFunctions/arm_nn_mult_q7.c"
"${ProjDirPath}/../../../../../middleware/eiq/cmsis-nn/Source/NNSupportFunctions/arm_nntables.c"
"${ProjDirPath}/../../../../../middleware/eiq/cmsis-nn/Source/NNSupportFunctions/arm_q7_to_q15_no_shift.c"
"${ProjDirPath}/../../../../../middleware/eiq/cmsis-nn/Source/NNSupportFunctions/arm_q7_to_q15_reordered_no_shift.c"
"${ProjDirPath}/../../../../../middleware/eiq/cmsis-nn/Source/PoolingFunctions/arm_pool_q7_HWC.c"
"${ProjDirPath}/../../../../../middleware/eiq/cmsis-nn/Source/SoftmaxFunctions/arm_softmax_q15.c"
"${ProjDirPath}/../../../../../middleware/eiq/cmsis-nn/Source/SoftmaxFunctions/arm_softmax_q7.c"
"${ProjDirPath}/../../../../../components/lists/generic_list.c"
"${ProjDirPath}/../../../../../components/lists/generic_list.h"
"${ProjDirPath}/../../../../../components/uart/lpuart_adapter.c"
"${ProjDirPath}/../../../../../components/uart/uart.h"
"${ProjDirPath}/../../../../../components/serial_manager/serial_manager.c"
"${ProjDirPath}/../../../../../components/serial_manager/serial_manager.h"
"${ProjDirPath}/../../../../../components/serial_manager/serial_port_internal.h"
"${ProjDirPath}/../../../../../devices/MIMXRT1062/drivers/fsl_common.c"
"${ProjDirPath}/../../../../../devices/MIMXRT1062/drivers/fsl_common.h"
"${ProjDirPath}/../../../../../components/serial_manager/serial_port_uart.c"
"${ProjDirPath}/../../../../../components/serial_manager/serial_port_uart.h"
"${ProjDirPath}/../../../../../devices/MIMXRT1062/drivers/fsl_lpuart.c"
"${ProjDirPath}/../../../../../devices/MIMXRT1062/drivers/fsl_lpuart.h"
"${ProjDirPath}/../../../../../devices/MIMXRT1062/MIMXRT1062.h"
"${ProjDirPath}/../../../../../devices/MIMXRT1062/MIMXRT1062_features.h"
"${ProjDirPath}/../../../../../devices/MIMXRT1062/fsl_device_registers.h"
"${ProjDirPath}/../../../../../devices/MIMXRT1062/gcc/startup_MIMXRT1062.S"
"${ProjDirPath}/../../../../../devices/MIMXRT1062/system_MIMXRT1062.c"
"${ProjDirPath}/../../../../../devices/MIMXRT1062/system_MIMXRT1062.h"
"${ProjDirPath}/../../../../../devices/MIMXRT1062/drivers/fsl_clock.c"
"${ProjDirPath}/../../../../../devices/MIMXRT1062/drivers/fsl_clock.h"
"${ProjDirPath}/../../../../../devices/MIMXRT1062/drivers/fsl_gpio.c"
"${ProjDirPath}/../../../../../devices/MIMXRT1062/drivers/fsl_gpio.h"
"${ProjDirPath}/../../../../../devices/MIMXRT1062/drivers/fsl_iomuxc.h"
"${ProjDirPath}/../../../../../devices/MIMXRT1062/drivers/fsl_lpi2c.c"
"${ProjDirPath}/../../../../../devices/MIMXRT1062/drivers/fsl_lpi2c.h"
"${ProjDirPath}/../../../../../devices/MIMXRT1062/xip/fsl_flexspi_nor_boot.c"
"${ProjDirPath}/../../../../../devices/MIMXRT1062/xip/fsl_flexspi_nor_boot.h"
"${ProjDirPath}/../../../xip/evkmimxrt1060_flexspi_nor_config.c"
"${ProjDirPath}/../../../xip/evkmimxrt1060_flexspi_nor_config.h"
"${ProjDirPath}/../../../xip/evkmimxrt1060_sdram_ini_dcd.c"
"${ProjDirPath}/../../../xip/evkmimxrt1060_sdram_ini_dcd.h"
"${ProjDirPath}/../../../../../devices/MIMXRT1062/utilities/fsl_assert.c"
"${ProjDirPath}/../../../../../devices/MIMXRT1062/utilities/debug_console/fsl_debug_console.c"
"${ProjDirPath}/../../../../../devices/MIMXRT1062/utilities/debug_console/fsl_debug_console.h"
"${ProjDirPath}/../../../../../devices/MIMXRT1062/utilities/debug_console/fsl_debug_console_conf.h"
"${ProjDirPath}/../../../../../devices/MIMXRT1062/utilities/str/fsl_str.c"
"${ProjDirPath}/../../../../../devices/MIMXRT1062/utilities/str/fsl_str.h"
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
"${ProjDirPath}/../../../../../components/codec/i2c/fsl_codec_i2c.c"
"${ProjDirPath}/../../../../../components/codec/i2c/fsl_codec_i2c.h"
"${ProjDirPath}/../../../../../devices/MIMXRT1062/utilities/fsl_sbrk.c"
)


set(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -T${ProjDirPath}/MIMXRT1062xxxxx_flexspi_nor_sdram.ld -static")

set(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -T${ProjDirPath}/MIMXRT1062xxxxx_flexspi_nor_sdram.ld -static")

TARGET_LINK_LIBRARIES(cmsis_nn_kws.elf -Wl,--start-group)
target_link_libraries(cmsis_nn_kws.elf optimized m)

target_link_libraries(cmsis_nn_kws.elf optimized c)

target_link_libraries(cmsis_nn_kws.elf optimized gcc)

target_link_libraries(cmsis_nn_kws.elf optimized nosys)

target_link_libraries(cmsis_nn_kws.elf optimized m)

target_link_libraries(cmsis_nn_kws.elf optimized c)

target_link_libraries(cmsis_nn_kws.elf optimized gcc)

target_link_libraries(cmsis_nn_kws.elf optimized nosys)

link_directories(${ProjDirPath}/../../../../../CMSIS/DSP/Lib/GCC)

target_link_libraries(cmsis_nn_kws.elf optimized ${ProjDirPath}/../../../../../CMSIS/DSP/Lib/GCC/libarm_cortexM7lfdp_math.a)

link_directories(${ProjDirPath}/../../../../../CMSIS/DSP/Lib/GCC)

target_link_libraries(cmsis_nn_kws.elf optimized ${ProjDirPath}/../../../../../CMSIS/DSP/Lib/GCC/libarm_cortexM7lfdp_math.a)

TARGET_LINK_LIBRARIES(cmsis_nn_kws.elf -Wl,--end-group)


