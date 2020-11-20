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

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -DDATA_SECTION_IS_CACHEABLE=1")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -DDEBUG")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -DCPU_MIMXRT1064DVL6A")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -DLV_CONF_INCLUDE_SIMPLE=1")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -DDEBUG_CONSOLE_TRANSFER_NON_BLOCKING")

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

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -DDATA_SECTION_IS_CACHEABLE=1")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -DNDEBUG")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -DCPU_MIMXRT1064DVL6A")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -DLV_CONF_INCLUDE_SIMPLE=1")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_C_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -DDEBUG_CONSOLE_TRANSFER_NON_BLOCKING")

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

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -DSERIAL_PORT_TYPE_UART=1")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -g")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -O0")

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

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -DSERIAL_PORT_TYPE_UART=1")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -Os")

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

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} --defsym=__stack_size__=0x1000")

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

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} --defsym=__stack_size__=0x1000")

include_directories(${ProjDirPath}/..)

include_directories(${ProjDirPath}/../../..)

include_directories(${ProjDirPath}/..)

include_directories(${ProjDirPath}/../../..)

include_directories(${ProjDirPath}/..)

include_directories(${ProjDirPath}/../..)

include_directories(${ProjDirPath}/../../..)

include_directories(${ProjDirPath}/../../../../../middleware/littlevgl)

include_directories(${ProjDirPath}/../../../../../CMSIS/Include)

include_directories(${ProjDirPath}/../../../../../devices)

include_directories(${ProjDirPath}/../../../../../devices/MIMXRT1064/drivers)

include_directories(${ProjDirPath}/../../../../../components/ft5406_rt)

include_directories(${ProjDirPath}/../../../../../middleware/littlevgl/lvgl)

include_directories(${ProjDirPath}/../../../../../devices/MIMXRT1064)

include_directories(${ProjDirPath}/../../../../../devices/MIMXRT1064/utilities/str)

include_directories(${ProjDirPath}/../../../../../devices/MIMXRT1064/utilities/debug_console)

include_directories(${ProjDirPath}/../../../../../components/uart)

include_directories(${ProjDirPath}/../../../../../components/serial_manager)

include_directories(${ProjDirPath}/../../../../../components/lists)

include_directories(${ProjDirPath}/../../../../../devices/MIMXRT1064/xip)

include_directories(${ProjDirPath}/../../../xip)

include_directories(${ProjDirPath}/../../../../../devices/MIMXRT1064/utilities)

add_executable(littlevgl_terminal_bm.elf 
"${ProjDirPath}/../littlevgl_terminal_bm.c"
"${ProjDirPath}/../lv_conf.h"
"${ProjDirPath}/../lv_ex_conf.h"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_examples/lv_apps/terminal/terminal.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lv_examples/lv_apps/terminal/terminal.h"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
"${ProjDirPath}/../../littlevgl_support.h"
"${ProjDirPath}/../../littlevgl_support.c"
"${ProjDirPath}/../dcd.c"
"${ProjDirPath}/../dcd.h"
"${ProjDirPath}/../../../../../devices/MIMXRT1064/drivers/fsl_elcdif.c"
"${ProjDirPath}/../../../../../devices/MIMXRT1064/drivers/fsl_elcdif.h"
"${ProjDirPath}/../../../../../devices/MIMXRT1064/drivers/fsl_lpi2c.c"
"${ProjDirPath}/../../../../../devices/MIMXRT1064/drivers/fsl_lpi2c.h"
"${ProjDirPath}/../../../../../components/ft5406_rt/fsl_ft5406_rt.c"
"${ProjDirPath}/../../../../../components/ft5406_rt/fsl_ft5406_rt.h"
"${ProjDirPath}/../../../../../devices/MIMXRT1064/drivers/fsl_pxp.c"
"${ProjDirPath}/../../../../../devices/MIMXRT1064/drivers/fsl_pxp.h"
"${ProjDirPath}/../../../../../devices/MIMXRT1064/drivers/fsl_common.c"
"${ProjDirPath}/../../../../../devices/MIMXRT1064/drivers/fsl_common.h"
"${ProjDirPath}/../../../../../devices/MIMXRT1064/drivers/fsl_clock.c"
"${ProjDirPath}/../../../../../devices/MIMXRT1064/drivers/fsl_clock.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/lvgl.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_conf_checker.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_core/lv_debug.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_core/lv_debug.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_core/lv_disp.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_core/lv_disp.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_core/lv_group.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_core/lv_group.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_core/lv_indev.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_core/lv_indev.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_core/lv_obj.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_core/lv_obj.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_core/lv_refr.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_core/lv_refr.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_core/lv_style.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_core/lv_style.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_draw/lv_draw.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_draw/lv_draw.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_draw/lv_draw_arc.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_draw/lv_draw_arc.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_draw/lv_draw_basic.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_draw/lv_draw_basic.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_draw/lv_draw_img.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_draw/lv_draw_img.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_draw/lv_draw_label.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_draw/lv_draw_label.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_draw/lv_draw_line.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_draw/lv_draw_line.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_draw/lv_draw_rect.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_draw/lv_draw_rect.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_draw/lv_draw_triangle.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_draw/lv_draw_triangle.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_draw/lv_img_cache.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_draw/lv_img_cache.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_draw/lv_img_decoder.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_draw/lv_img_decoder.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_font/lv_font.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_font/lv_font.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_font/lv_font_fmt_txt.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_font/lv_font_fmt_txt.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_font/lv_font_roboto_12.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_font/lv_font_roboto_12_subpx.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_font/lv_font_roboto_16.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_font/lv_font_roboto_22.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_font/lv_font_roboto_28.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_font/lv_font_roboto_28_compressed.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_font/lv_font_unscii_8.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_font/lv_symbol_def.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_hal/lv_hal.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_hal/lv_hal_disp.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_hal/lv_hal_disp.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_hal/lv_hal_indev.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_hal/lv_hal_indev.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_hal/lv_hal_tick.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_hal/lv_hal_tick.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_misc/lv_anim.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_misc/lv_anim.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_misc/lv_area.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_misc/lv_area.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_misc/lv_async.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_misc/lv_async.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_misc/lv_bidi.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_misc/lv_bidi.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_misc/lv_circ.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_misc/lv_circ.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_misc/lv_color.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_misc/lv_color.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_misc/lv_fs.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_misc/lv_fs.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_misc/lv_gc.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_misc/lv_gc.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_misc/lv_ll.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_misc/lv_ll.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_misc/lv_log.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_misc/lv_log.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_misc/lv_math.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_misc/lv_math.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_misc/lv_mem.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_misc/lv_mem.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_misc/lv_printf.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_misc/lv_printf.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_misc/lv_task.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_misc/lv_task.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_misc/lv_templ.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_misc/lv_templ.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_misc/lv_txt.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_misc/lv_txt.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_misc/lv_types.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_misc/lv_utils.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_misc/lv_utils.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_objx/lv_arc.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_objx/lv_arc.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_objx/lv_bar.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_objx/lv_bar.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_objx/lv_btn.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_objx/lv_btn.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_objx/lv_btnm.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_objx/lv_btnm.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_objx/lv_calendar.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_objx/lv_calendar.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_objx/lv_canvas.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_objx/lv_canvas.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_objx/lv_cb.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_objx/lv_cb.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_objx/lv_chart.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_objx/lv_chart.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_objx/lv_cont.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_objx/lv_cont.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_objx/lv_cpicker.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_objx/lv_cpicker.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_objx/lv_ddlist.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_objx/lv_ddlist.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_objx/lv_gauge.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_objx/lv_gauge.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_objx/lv_img.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_objx/lv_img.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_objx/lv_imgbtn.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_objx/lv_imgbtn.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_objx/lv_kb.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_objx/lv_kb.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_objx/lv_label.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_objx/lv_label.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_objx/lv_led.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_objx/lv_led.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_objx/lv_line.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_objx/lv_line.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_objx/lv_list.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_objx/lv_list.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_objx/lv_lmeter.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_objx/lv_lmeter.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_objx/lv_mbox.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_objx/lv_mbox.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_objx/lv_objx_templ.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_objx/lv_objx_templ.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_objx/lv_page.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_objx/lv_page.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_objx/lv_preload.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_objx/lv_preload.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_objx/lv_roller.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_objx/lv_roller.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_objx/lv_slider.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_objx/lv_slider.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_objx/lv_spinbox.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_objx/lv_spinbox.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_objx/lv_sw.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_objx/lv_sw.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_objx/lv_ta.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_objx/lv_ta.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_objx/lv_table.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_objx/lv_table.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_objx/lv_tabview.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_objx/lv_tabview.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_objx/lv_tileview.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_objx/lv_tileview.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_objx/lv_win.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_objx/lv_win.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_themes/lv_theme.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_themes/lv_theme.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_themes/lv_theme_alien.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_themes/lv_theme_alien.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_themes/lv_theme_default.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_themes/lv_theme_default.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_themes/lv_theme_material.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_themes/lv_theme_material.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_themes/lv_theme_mono.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_themes/lv_theme_mono.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_themes/lv_theme_nemo.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_themes/lv_theme_nemo.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_themes/lv_theme_night.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_themes/lv_theme_night.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_themes/lv_theme_templ.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_themes/lv_theme_templ.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_themes/lv_theme_zen.c"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_themes/lv_theme_zen.h"
"${ProjDirPath}/../../../../../middleware/littlevgl/lvgl/src/lv_version.h"
"${ProjDirPath}/../../../../../devices/MIMXRT1064/MIMXRT1064.h"
"${ProjDirPath}/../../../../../devices/MIMXRT1064/MIMXRT1064_features.h"
"${ProjDirPath}/../../../../../devices/MIMXRT1064/fsl_device_registers.h"
"${ProjDirPath}/../../../../../devices/MIMXRT1064/utilities/debug_console/fsl_debug_console.c"
"${ProjDirPath}/../../../../../devices/MIMXRT1064/utilities/debug_console/fsl_debug_console.h"
"${ProjDirPath}/../../../../../devices/MIMXRT1064/utilities/debug_console/fsl_debug_console_conf.h"
"${ProjDirPath}/../../../../../devices/MIMXRT1064/utilities/str/fsl_str.c"
"${ProjDirPath}/../../../../../devices/MIMXRT1064/utilities/str/fsl_str.h"
"${ProjDirPath}/../../../../../components/uart/lpuart_adapter.c"
"${ProjDirPath}/../../../../../components/uart/uart.h"
"${ProjDirPath}/../../../../../components/serial_manager/serial_manager.c"
"${ProjDirPath}/../../../../../components/serial_manager/serial_manager.h"
"${ProjDirPath}/../../../../../components/serial_manager/serial_port_internal.h"
"${ProjDirPath}/../../../../../components/lists/generic_list.c"
"${ProjDirPath}/../../../../../components/lists/generic_list.h"
"${ProjDirPath}/../../../../../components/serial_manager/serial_port_uart.c"
"${ProjDirPath}/../../../../../components/serial_manager/serial_port_uart.h"
"${ProjDirPath}/../../../../../devices/MIMXRT1064/drivers/fsl_lpuart.c"
"${ProjDirPath}/../../../../../devices/MIMXRT1064/drivers/fsl_lpuart.h"
"${ProjDirPath}/../../../../../devices/MIMXRT1064/gcc/startup_MIMXRT1064.S"
"${ProjDirPath}/../../../../../devices/MIMXRT1064/system_MIMXRT1064.c"
"${ProjDirPath}/../../../../../devices/MIMXRT1064/system_MIMXRT1064.h"
"${ProjDirPath}/../../../../../devices/MIMXRT1064/drivers/fsl_iomuxc.h"
"${ProjDirPath}/../../../../../devices/MIMXRT1064/utilities/fsl_assert.c"
"${ProjDirPath}/../../../../../devices/MIMXRT1064/drivers/fsl_gpio.c"
"${ProjDirPath}/../../../../../devices/MIMXRT1064/drivers/fsl_gpio.h"
"${ProjDirPath}/../../../../../devices/MIMXRT1064/xip/fsl_flexspi_nor_boot.c"
"${ProjDirPath}/../../../../../devices/MIMXRT1064/xip/fsl_flexspi_nor_boot.h"
"${ProjDirPath}/../../../xip/evkmimxrt1064_flexspi_nor_config.c"
"${ProjDirPath}/../../../xip/evkmimxrt1064_flexspi_nor_config.h"
"${ProjDirPath}/../../../../../CMSIS/Include/core_cm7.h"
"${ProjDirPath}/../../../../../CMSIS/Include/mpu_armv7.h"
"${ProjDirPath}/../../../../../CMSIS/Include/cmsis_armcc.h"
"${ProjDirPath}/../../../../../CMSIS/Include/cmsis_armclang.h"
"${ProjDirPath}/../../../../../CMSIS/Include/cmsis_armclang_ltm.h"
"${ProjDirPath}/../../../../../CMSIS/Include/cmsis_compiler.h"
"${ProjDirPath}/../../../../../CMSIS/Include/cmsis_gcc.h"
"${ProjDirPath}/../../../../../CMSIS/Include/cmsis_iccarm.h"
"${ProjDirPath}/../../../../../CMSIS/Include/cmsis_version.h"
"${ProjDirPath}/../../../../../CMSIS/Include/arm_common_tables.h"
"${ProjDirPath}/../../../../../CMSIS/Include/arm_const_structs.h"
"${ProjDirPath}/../../../../../CMSIS/Include/arm_math.h"
"${ProjDirPath}/../../../../../devices/MIMXRT1064/utilities/fsl_sbrk.c"
)


set(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_DEBUG} -T${ProjDirPath}/MIMXRT1064xxxxx_flexspi_nor_sdram.ld -static")

set(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_SDRAM_RELEASE} -T${ProjDirPath}/MIMXRT1064xxxxx_flexspi_nor_sdram.ld -static")

TARGET_LINK_LIBRARIES(littlevgl_terminal_bm.elf -Wl,--start-group)
target_link_libraries(littlevgl_terminal_bm.elf optimized m)

target_link_libraries(littlevgl_terminal_bm.elf optimized c)

target_link_libraries(littlevgl_terminal_bm.elf optimized gcc)

target_link_libraries(littlevgl_terminal_bm.elf optimized nosys)

target_link_libraries(littlevgl_terminal_bm.elf optimized m)

target_link_libraries(littlevgl_terminal_bm.elf optimized c)

target_link_libraries(littlevgl_terminal_bm.elf optimized gcc)

target_link_libraries(littlevgl_terminal_bm.elf optimized nosys)

TARGET_LINK_LIBRARIES(littlevgl_terminal_bm.elf -Wl,--end-group)


