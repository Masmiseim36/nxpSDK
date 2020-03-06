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


SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -DDEBUG")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -D__STARTUP_CLEAR_BSS")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -g")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -mcpu=cortex-m33")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -Wall")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -mfloat-abi=hard")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -mfpu=fpv5-sp-d16")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -mthumb")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -fno-common")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -ffunction-sections")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -fdata-sections")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -ffreestanding")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -fno-builtin")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -mapcs")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -std=gnu99")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -DNDEBUG")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -D__STARTUP_CLEAR_BSS")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -mcpu=cortex-m33")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -Wall")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -mfloat-abi=hard")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -mfpu=fpv5-sp-d16")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -mthumb")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -fno-common")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -ffunction-sections")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -fdata-sections")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -ffreestanding")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -fno-builtin")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -mapcs")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -std=gnu99")

SET(CMAKE_ASM_FLAGS_FLASH_DEBUG "${CMAKE_ASM_FLAGS_FLASH_DEBUG} -DDEBUG")

SET(CMAKE_ASM_FLAGS_FLASH_DEBUG "${CMAKE_ASM_FLAGS_FLASH_DEBUG} -D__STARTUP_CLEAR_BSS")

SET(CMAKE_ASM_FLAGS_FLASH_DEBUG "${CMAKE_ASM_FLAGS_FLASH_DEBUG} -g")

SET(CMAKE_ASM_FLAGS_FLASH_DEBUG "${CMAKE_ASM_FLAGS_FLASH_DEBUG} -mcpu=cortex-m33")

SET(CMAKE_ASM_FLAGS_FLASH_DEBUG "${CMAKE_ASM_FLAGS_FLASH_DEBUG} -Wall")

SET(CMAKE_ASM_FLAGS_FLASH_DEBUG "${CMAKE_ASM_FLAGS_FLASH_DEBUG} -mfloat-abi=hard")

SET(CMAKE_ASM_FLAGS_FLASH_DEBUG "${CMAKE_ASM_FLAGS_FLASH_DEBUG} -mfpu=fpv5-sp-d16")

SET(CMAKE_ASM_FLAGS_FLASH_DEBUG "${CMAKE_ASM_FLAGS_FLASH_DEBUG} -mthumb")

SET(CMAKE_ASM_FLAGS_FLASH_DEBUG "${CMAKE_ASM_FLAGS_FLASH_DEBUG} -fno-common")

SET(CMAKE_ASM_FLAGS_FLASH_DEBUG "${CMAKE_ASM_FLAGS_FLASH_DEBUG} -ffunction-sections")

SET(CMAKE_ASM_FLAGS_FLASH_DEBUG "${CMAKE_ASM_FLAGS_FLASH_DEBUG} -fdata-sections")

SET(CMAKE_ASM_FLAGS_FLASH_DEBUG "${CMAKE_ASM_FLAGS_FLASH_DEBUG} -ffreestanding")

SET(CMAKE_ASM_FLAGS_FLASH_DEBUG "${CMAKE_ASM_FLAGS_FLASH_DEBUG} -fno-builtin")

SET(CMAKE_ASM_FLAGS_FLASH_DEBUG "${CMAKE_ASM_FLAGS_FLASH_DEBUG} -mapcs")

SET(CMAKE_ASM_FLAGS_FLASH_DEBUG "${CMAKE_ASM_FLAGS_FLASH_DEBUG} -std=gnu99")

SET(CMAKE_ASM_FLAGS_FLASH_RELEASE "${CMAKE_ASM_FLAGS_FLASH_RELEASE} -DNDEBUG")

SET(CMAKE_ASM_FLAGS_FLASH_RELEASE "${CMAKE_ASM_FLAGS_FLASH_RELEASE} -D__STARTUP_CLEAR_BSS")

SET(CMAKE_ASM_FLAGS_FLASH_RELEASE "${CMAKE_ASM_FLAGS_FLASH_RELEASE} -mcpu=cortex-m33")

SET(CMAKE_ASM_FLAGS_FLASH_RELEASE "${CMAKE_ASM_FLAGS_FLASH_RELEASE} -Wall")

SET(CMAKE_ASM_FLAGS_FLASH_RELEASE "${CMAKE_ASM_FLAGS_FLASH_RELEASE} -mfloat-abi=hard")

SET(CMAKE_ASM_FLAGS_FLASH_RELEASE "${CMAKE_ASM_FLAGS_FLASH_RELEASE} -mfpu=fpv5-sp-d16")

SET(CMAKE_ASM_FLAGS_FLASH_RELEASE "${CMAKE_ASM_FLAGS_FLASH_RELEASE} -mthumb")

SET(CMAKE_ASM_FLAGS_FLASH_RELEASE "${CMAKE_ASM_FLAGS_FLASH_RELEASE} -fno-common")

SET(CMAKE_ASM_FLAGS_FLASH_RELEASE "${CMAKE_ASM_FLAGS_FLASH_RELEASE} -ffunction-sections")

SET(CMAKE_ASM_FLAGS_FLASH_RELEASE "${CMAKE_ASM_FLAGS_FLASH_RELEASE} -fdata-sections")

SET(CMAKE_ASM_FLAGS_FLASH_RELEASE "${CMAKE_ASM_FLAGS_FLASH_RELEASE} -ffreestanding")

SET(CMAKE_ASM_FLAGS_FLASH_RELEASE "${CMAKE_ASM_FLAGS_FLASH_RELEASE} -fno-builtin")

SET(CMAKE_ASM_FLAGS_FLASH_RELEASE "${CMAKE_ASM_FLAGS_FLASH_RELEASE} -mapcs")

SET(CMAKE_ASM_FLAGS_FLASH_RELEASE "${CMAKE_ASM_FLAGS_FLASH_RELEASE} -std=gnu99")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DCPU_MIMXRT685SFVKB_cm33")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DBOOT_HEADER_ENABLE=1")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DARM_MATH_CM33")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DSERIAL_PORT_TYPE_UART=1")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -mcpu=cortex-m33")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wall")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -mfloat-abi=hard")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -mfpu=fpv5-sp-d16")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -mthumb")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -MMD")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -MP")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -fno-common")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -ffunction-sections")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -fdata-sections")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -ffreestanding")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -fno-builtin")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -mapcs")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -std=gnu99")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DNDEBUG")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DCPU_MIMXRT685SFVKB_cm33")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DBOOT_HEADER_ENABLE=1")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DARM_MATH_CM33")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DSERIAL_PORT_TYPE_UART=1")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Os")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -mcpu=cortex-m33")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Wall")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -mfloat-abi=hard")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -mfpu=fpv5-sp-d16")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -mthumb")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -MMD")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -MP")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -fno-common")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -ffunction-sections")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -fdata-sections")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -ffreestanding")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -fno-builtin")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -mapcs")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -std=gnu99")

SET(CMAKE_C_FLAGS_FLASH_DEBUG "${CMAKE_C_FLAGS_FLASH_DEBUG} -DDEBUG")

SET(CMAKE_C_FLAGS_FLASH_DEBUG "${CMAKE_C_FLAGS_FLASH_DEBUG} -DFSL_SDK_DRIVER_QUICK_ACCESS_ENABLE=1")

SET(CMAKE_C_FLAGS_FLASH_DEBUG "${CMAKE_C_FLAGS_FLASH_DEBUG} -DCPU_MIMXRT685SFVKB_cm33")

SET(CMAKE_C_FLAGS_FLASH_DEBUG "${CMAKE_C_FLAGS_FLASH_DEBUG} -DBOOT_HEADER_ENABLE=1")

SET(CMAKE_C_FLAGS_FLASH_DEBUG "${CMAKE_C_FLAGS_FLASH_DEBUG} -DARM_MATH_CM33")

SET(CMAKE_C_FLAGS_FLASH_DEBUG "${CMAKE_C_FLAGS_FLASH_DEBUG} -DSERIAL_PORT_TYPE_UART=1")

SET(CMAKE_C_FLAGS_FLASH_DEBUG "${CMAKE_C_FLAGS_FLASH_DEBUG} -g")

SET(CMAKE_C_FLAGS_FLASH_DEBUG "${CMAKE_C_FLAGS_FLASH_DEBUG} -O0")

SET(CMAKE_C_FLAGS_FLASH_DEBUG "${CMAKE_C_FLAGS_FLASH_DEBUG} -mcpu=cortex-m33")

SET(CMAKE_C_FLAGS_FLASH_DEBUG "${CMAKE_C_FLAGS_FLASH_DEBUG} -Wall")

SET(CMAKE_C_FLAGS_FLASH_DEBUG "${CMAKE_C_FLAGS_FLASH_DEBUG} -mfloat-abi=hard")

SET(CMAKE_C_FLAGS_FLASH_DEBUG "${CMAKE_C_FLAGS_FLASH_DEBUG} -mfpu=fpv5-sp-d16")

SET(CMAKE_C_FLAGS_FLASH_DEBUG "${CMAKE_C_FLAGS_FLASH_DEBUG} -mthumb")

SET(CMAKE_C_FLAGS_FLASH_DEBUG "${CMAKE_C_FLAGS_FLASH_DEBUG} -MMD")

SET(CMAKE_C_FLAGS_FLASH_DEBUG "${CMAKE_C_FLAGS_FLASH_DEBUG} -MP")

SET(CMAKE_C_FLAGS_FLASH_DEBUG "${CMAKE_C_FLAGS_FLASH_DEBUG} -fno-common")

SET(CMAKE_C_FLAGS_FLASH_DEBUG "${CMAKE_C_FLAGS_FLASH_DEBUG} -ffunction-sections")

SET(CMAKE_C_FLAGS_FLASH_DEBUG "${CMAKE_C_FLAGS_FLASH_DEBUG} -fdata-sections")

SET(CMAKE_C_FLAGS_FLASH_DEBUG "${CMAKE_C_FLAGS_FLASH_DEBUG} -ffreestanding")

SET(CMAKE_C_FLAGS_FLASH_DEBUG "${CMAKE_C_FLAGS_FLASH_DEBUG} -fno-builtin")

SET(CMAKE_C_FLAGS_FLASH_DEBUG "${CMAKE_C_FLAGS_FLASH_DEBUG} -mapcs")

SET(CMAKE_C_FLAGS_FLASH_DEBUG "${CMAKE_C_FLAGS_FLASH_DEBUG} -std=gnu99")

SET(CMAKE_C_FLAGS_FLASH_RELEASE "${CMAKE_C_FLAGS_FLASH_RELEASE} -DNDEBUG")

SET(CMAKE_C_FLAGS_FLASH_RELEASE "${CMAKE_C_FLAGS_FLASH_RELEASE} -DFSL_SDK_DRIVER_QUICK_ACCESS_ENABLE=1")

SET(CMAKE_C_FLAGS_FLASH_RELEASE "${CMAKE_C_FLAGS_FLASH_RELEASE} -DCPU_MIMXRT685SFVKB_cm33")

SET(CMAKE_C_FLAGS_FLASH_RELEASE "${CMAKE_C_FLAGS_FLASH_RELEASE} -DBOOT_HEADER_ENABLE=1")

SET(CMAKE_C_FLAGS_FLASH_RELEASE "${CMAKE_C_FLAGS_FLASH_RELEASE} -DARM_MATH_CM33")

SET(CMAKE_C_FLAGS_FLASH_RELEASE "${CMAKE_C_FLAGS_FLASH_RELEASE} -DSERIAL_PORT_TYPE_UART=1")

SET(CMAKE_C_FLAGS_FLASH_RELEASE "${CMAKE_C_FLAGS_FLASH_RELEASE} -Os")

SET(CMAKE_C_FLAGS_FLASH_RELEASE "${CMAKE_C_FLAGS_FLASH_RELEASE} -mcpu=cortex-m33")

SET(CMAKE_C_FLAGS_FLASH_RELEASE "${CMAKE_C_FLAGS_FLASH_RELEASE} -Wall")

SET(CMAKE_C_FLAGS_FLASH_RELEASE "${CMAKE_C_FLAGS_FLASH_RELEASE} -mfloat-abi=hard")

SET(CMAKE_C_FLAGS_FLASH_RELEASE "${CMAKE_C_FLAGS_FLASH_RELEASE} -mfpu=fpv5-sp-d16")

SET(CMAKE_C_FLAGS_FLASH_RELEASE "${CMAKE_C_FLAGS_FLASH_RELEASE} -mthumb")

SET(CMAKE_C_FLAGS_FLASH_RELEASE "${CMAKE_C_FLAGS_FLASH_RELEASE} -MMD")

SET(CMAKE_C_FLAGS_FLASH_RELEASE "${CMAKE_C_FLAGS_FLASH_RELEASE} -MP")

SET(CMAKE_C_FLAGS_FLASH_RELEASE "${CMAKE_C_FLAGS_FLASH_RELEASE} -fno-common")

SET(CMAKE_C_FLAGS_FLASH_RELEASE "${CMAKE_C_FLAGS_FLASH_RELEASE} -ffunction-sections")

SET(CMAKE_C_FLAGS_FLASH_RELEASE "${CMAKE_C_FLAGS_FLASH_RELEASE} -fdata-sections")

SET(CMAKE_C_FLAGS_FLASH_RELEASE "${CMAKE_C_FLAGS_FLASH_RELEASE} -ffreestanding")

SET(CMAKE_C_FLAGS_FLASH_RELEASE "${CMAKE_C_FLAGS_FLASH_RELEASE} -fno-builtin")

SET(CMAKE_C_FLAGS_FLASH_RELEASE "${CMAKE_C_FLAGS_FLASH_RELEASE} -mapcs")

SET(CMAKE_C_FLAGS_FLASH_RELEASE "${CMAKE_C_FLAGS_FLASH_RELEASE} -std=gnu99")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DSERIAL_PORT_TYPE_UART=1")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -mcpu=cortex-m33")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -mfloat-abi=hard")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -mfpu=fpv5-sp-d16")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -mthumb")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -MMD")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -MP")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-common")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -ffunction-sections")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fdata-sections")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -ffreestanding")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-builtin")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -mapcs")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-rtti")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-exceptions")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DNDEBUG")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DSERIAL_PORT_TYPE_UART=1")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Os")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -mcpu=cortex-m33")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -mfloat-abi=hard")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -mfpu=fpv5-sp-d16")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -mthumb")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -MMD")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -MP")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fno-common")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -ffunction-sections")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fdata-sections")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -ffreestanding")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fno-builtin")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -mapcs")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fno-rtti")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fno-exceptions")

SET(CMAKE_CXX_FLAGS_FLASH_DEBUG "${CMAKE_CXX_FLAGS_FLASH_DEBUG} -DDEBUG")

SET(CMAKE_CXX_FLAGS_FLASH_DEBUG "${CMAKE_CXX_FLAGS_FLASH_DEBUG} -DSERIAL_PORT_TYPE_UART=1")

SET(CMAKE_CXX_FLAGS_FLASH_DEBUG "${CMAKE_CXX_FLAGS_FLASH_DEBUG} -g")

SET(CMAKE_CXX_FLAGS_FLASH_DEBUG "${CMAKE_CXX_FLAGS_FLASH_DEBUG} -O0")

SET(CMAKE_CXX_FLAGS_FLASH_DEBUG "${CMAKE_CXX_FLAGS_FLASH_DEBUG} -mcpu=cortex-m33")

SET(CMAKE_CXX_FLAGS_FLASH_DEBUG "${CMAKE_CXX_FLAGS_FLASH_DEBUG} -Wall")

SET(CMAKE_CXX_FLAGS_FLASH_DEBUG "${CMAKE_CXX_FLAGS_FLASH_DEBUG} -mfloat-abi=hard")

SET(CMAKE_CXX_FLAGS_FLASH_DEBUG "${CMAKE_CXX_FLAGS_FLASH_DEBUG} -mfpu=fpv5-sp-d16")

SET(CMAKE_CXX_FLAGS_FLASH_DEBUG "${CMAKE_CXX_FLAGS_FLASH_DEBUG} -mthumb")

SET(CMAKE_CXX_FLAGS_FLASH_DEBUG "${CMAKE_CXX_FLAGS_FLASH_DEBUG} -MMD")

SET(CMAKE_CXX_FLAGS_FLASH_DEBUG "${CMAKE_CXX_FLAGS_FLASH_DEBUG} -MP")

SET(CMAKE_CXX_FLAGS_FLASH_DEBUG "${CMAKE_CXX_FLAGS_FLASH_DEBUG} -fno-common")

SET(CMAKE_CXX_FLAGS_FLASH_DEBUG "${CMAKE_CXX_FLAGS_FLASH_DEBUG} -ffunction-sections")

SET(CMAKE_CXX_FLAGS_FLASH_DEBUG "${CMAKE_CXX_FLAGS_FLASH_DEBUG} -fdata-sections")

SET(CMAKE_CXX_FLAGS_FLASH_DEBUG "${CMAKE_CXX_FLAGS_FLASH_DEBUG} -ffreestanding")

SET(CMAKE_CXX_FLAGS_FLASH_DEBUG "${CMAKE_CXX_FLAGS_FLASH_DEBUG} -fno-builtin")

SET(CMAKE_CXX_FLAGS_FLASH_DEBUG "${CMAKE_CXX_FLAGS_FLASH_DEBUG} -mapcs")

SET(CMAKE_CXX_FLAGS_FLASH_DEBUG "${CMAKE_CXX_FLAGS_FLASH_DEBUG} -fno-rtti")

SET(CMAKE_CXX_FLAGS_FLASH_DEBUG "${CMAKE_CXX_FLAGS_FLASH_DEBUG} -fno-exceptions")

SET(CMAKE_CXX_FLAGS_FLASH_RELEASE "${CMAKE_CXX_FLAGS_FLASH_RELEASE} -DNDEBUG")

SET(CMAKE_CXX_FLAGS_FLASH_RELEASE "${CMAKE_CXX_FLAGS_FLASH_RELEASE} -DSERIAL_PORT_TYPE_UART=1")

SET(CMAKE_CXX_FLAGS_FLASH_RELEASE "${CMAKE_CXX_FLAGS_FLASH_RELEASE} -Os")

SET(CMAKE_CXX_FLAGS_FLASH_RELEASE "${CMAKE_CXX_FLAGS_FLASH_RELEASE} -mcpu=cortex-m33")

SET(CMAKE_CXX_FLAGS_FLASH_RELEASE "${CMAKE_CXX_FLAGS_FLASH_RELEASE} -Wall")

SET(CMAKE_CXX_FLAGS_FLASH_RELEASE "${CMAKE_CXX_FLAGS_FLASH_RELEASE} -mfloat-abi=hard")

SET(CMAKE_CXX_FLAGS_FLASH_RELEASE "${CMAKE_CXX_FLAGS_FLASH_RELEASE} -mfpu=fpv5-sp-d16")

SET(CMAKE_CXX_FLAGS_FLASH_RELEASE "${CMAKE_CXX_FLAGS_FLASH_RELEASE} -mthumb")

SET(CMAKE_CXX_FLAGS_FLASH_RELEASE "${CMAKE_CXX_FLAGS_FLASH_RELEASE} -MMD")

SET(CMAKE_CXX_FLAGS_FLASH_RELEASE "${CMAKE_CXX_FLAGS_FLASH_RELEASE} -MP")

SET(CMAKE_CXX_FLAGS_FLASH_RELEASE "${CMAKE_CXX_FLAGS_FLASH_RELEASE} -fno-common")

SET(CMAKE_CXX_FLAGS_FLASH_RELEASE "${CMAKE_CXX_FLAGS_FLASH_RELEASE} -ffunction-sections")

SET(CMAKE_CXX_FLAGS_FLASH_RELEASE "${CMAKE_CXX_FLAGS_FLASH_RELEASE} -fdata-sections")

SET(CMAKE_CXX_FLAGS_FLASH_RELEASE "${CMAKE_CXX_FLAGS_FLASH_RELEASE} -ffreestanding")

SET(CMAKE_CXX_FLAGS_FLASH_RELEASE "${CMAKE_CXX_FLAGS_FLASH_RELEASE} -fno-builtin")

SET(CMAKE_CXX_FLAGS_FLASH_RELEASE "${CMAKE_CXX_FLAGS_FLASH_RELEASE} -mapcs")

SET(CMAKE_CXX_FLAGS_FLASH_RELEASE "${CMAKE_CXX_FLAGS_FLASH_RELEASE} -fno-rtti")

SET(CMAKE_CXX_FLAGS_FLASH_RELEASE "${CMAKE_CXX_FLAGS_FLASH_RELEASE} -fno-exceptions")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -g")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -mcpu=cortex-m33")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -Wall")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -mfloat-abi=hard")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -mfpu=fpv5-sp-d16")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} --specs=nano.specs")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} --specs=nosys.specs")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -fno-common")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -ffunction-sections")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -fdata-sections")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -ffreestanding")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -fno-builtin")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -mthumb")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -mapcs")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} --gc-sections")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -static")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -z")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} muldefs")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -Map=output.map")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} --defsym=__stack_size__=0x800")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -mcpu=cortex-m33")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -Wall")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -mfloat-abi=hard")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -mfpu=fpv5-sp-d16")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} --specs=nano.specs")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} --specs=nosys.specs")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -fno-common")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -ffunction-sections")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -fdata-sections")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -ffreestanding")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -fno-builtin")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -mthumb")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -mapcs")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} --gc-sections")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -static")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -z")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} muldefs")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -Map=output.map")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} --defsym=__stack_size__=0x800")

SET(CMAKE_EXE_LINKER_FLAGS_FLASH_DEBUG "${CMAKE_EXE_LINKER_FLAGS_FLASH_DEBUG} -g")

SET(CMAKE_EXE_LINKER_FLAGS_FLASH_DEBUG "${CMAKE_EXE_LINKER_FLAGS_FLASH_DEBUG} -mcpu=cortex-m33")

SET(CMAKE_EXE_LINKER_FLAGS_FLASH_DEBUG "${CMAKE_EXE_LINKER_FLAGS_FLASH_DEBUG} -Wall")

SET(CMAKE_EXE_LINKER_FLAGS_FLASH_DEBUG "${CMAKE_EXE_LINKER_FLAGS_FLASH_DEBUG} -mfloat-abi=hard")

SET(CMAKE_EXE_LINKER_FLAGS_FLASH_DEBUG "${CMAKE_EXE_LINKER_FLAGS_FLASH_DEBUG} -mfpu=fpv5-sp-d16")

SET(CMAKE_EXE_LINKER_FLAGS_FLASH_DEBUG "${CMAKE_EXE_LINKER_FLAGS_FLASH_DEBUG} --specs=nano.specs")

SET(CMAKE_EXE_LINKER_FLAGS_FLASH_DEBUG "${CMAKE_EXE_LINKER_FLAGS_FLASH_DEBUG} --specs=nosys.specs")

SET(CMAKE_EXE_LINKER_FLAGS_FLASH_DEBUG "${CMAKE_EXE_LINKER_FLAGS_FLASH_DEBUG} -fno-common")

SET(CMAKE_EXE_LINKER_FLAGS_FLASH_DEBUG "${CMAKE_EXE_LINKER_FLAGS_FLASH_DEBUG} -ffunction-sections")

SET(CMAKE_EXE_LINKER_FLAGS_FLASH_DEBUG "${CMAKE_EXE_LINKER_FLAGS_FLASH_DEBUG} -fdata-sections")

SET(CMAKE_EXE_LINKER_FLAGS_FLASH_DEBUG "${CMAKE_EXE_LINKER_FLAGS_FLASH_DEBUG} -ffreestanding")

SET(CMAKE_EXE_LINKER_FLAGS_FLASH_DEBUG "${CMAKE_EXE_LINKER_FLAGS_FLASH_DEBUG} -fno-builtin")

SET(CMAKE_EXE_LINKER_FLAGS_FLASH_DEBUG "${CMAKE_EXE_LINKER_FLAGS_FLASH_DEBUG} -mthumb")

SET(CMAKE_EXE_LINKER_FLAGS_FLASH_DEBUG "${CMAKE_EXE_LINKER_FLAGS_FLASH_DEBUG} -mapcs")

SET(CMAKE_EXE_LINKER_FLAGS_FLASH_DEBUG "${CMAKE_EXE_LINKER_FLAGS_FLASH_DEBUG} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_FLASH_DEBUG "${CMAKE_EXE_LINKER_FLAGS_FLASH_DEBUG} --gc-sections")

SET(CMAKE_EXE_LINKER_FLAGS_FLASH_DEBUG "${CMAKE_EXE_LINKER_FLAGS_FLASH_DEBUG} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_FLASH_DEBUG "${CMAKE_EXE_LINKER_FLAGS_FLASH_DEBUG} -static")

SET(CMAKE_EXE_LINKER_FLAGS_FLASH_DEBUG "${CMAKE_EXE_LINKER_FLAGS_FLASH_DEBUG} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_FLASH_DEBUG "${CMAKE_EXE_LINKER_FLAGS_FLASH_DEBUG} -z")

SET(CMAKE_EXE_LINKER_FLAGS_FLASH_DEBUG "${CMAKE_EXE_LINKER_FLAGS_FLASH_DEBUG} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_FLASH_DEBUG "${CMAKE_EXE_LINKER_FLAGS_FLASH_DEBUG} muldefs")

SET(CMAKE_EXE_LINKER_FLAGS_FLASH_DEBUG "${CMAKE_EXE_LINKER_FLAGS_FLASH_DEBUG} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_FLASH_DEBUG "${CMAKE_EXE_LINKER_FLAGS_FLASH_DEBUG} -Map=output.map")

SET(CMAKE_EXE_LINKER_FLAGS_FLASH_DEBUG "${CMAKE_EXE_LINKER_FLAGS_FLASH_DEBUG} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_FLASH_DEBUG "${CMAKE_EXE_LINKER_FLAGS_FLASH_DEBUG} --defsym=__stack_size__=0x800")

SET(CMAKE_EXE_LINKER_FLAGS_FLASH_RELEASE "${CMAKE_EXE_LINKER_FLAGS_FLASH_RELEASE} -mcpu=cortex-m33")

SET(CMAKE_EXE_LINKER_FLAGS_FLASH_RELEASE "${CMAKE_EXE_LINKER_FLAGS_FLASH_RELEASE} -Wall")

SET(CMAKE_EXE_LINKER_FLAGS_FLASH_RELEASE "${CMAKE_EXE_LINKER_FLAGS_FLASH_RELEASE} -mfloat-abi=hard")

SET(CMAKE_EXE_LINKER_FLAGS_FLASH_RELEASE "${CMAKE_EXE_LINKER_FLAGS_FLASH_RELEASE} -mfpu=fpv5-sp-d16")

SET(CMAKE_EXE_LINKER_FLAGS_FLASH_RELEASE "${CMAKE_EXE_LINKER_FLAGS_FLASH_RELEASE} --specs=nano.specs")

SET(CMAKE_EXE_LINKER_FLAGS_FLASH_RELEASE "${CMAKE_EXE_LINKER_FLAGS_FLASH_RELEASE} --specs=nosys.specs")

SET(CMAKE_EXE_LINKER_FLAGS_FLASH_RELEASE "${CMAKE_EXE_LINKER_FLAGS_FLASH_RELEASE} -fno-common")

SET(CMAKE_EXE_LINKER_FLAGS_FLASH_RELEASE "${CMAKE_EXE_LINKER_FLAGS_FLASH_RELEASE} -ffunction-sections")

SET(CMAKE_EXE_LINKER_FLAGS_FLASH_RELEASE "${CMAKE_EXE_LINKER_FLAGS_FLASH_RELEASE} -fdata-sections")

SET(CMAKE_EXE_LINKER_FLAGS_FLASH_RELEASE "${CMAKE_EXE_LINKER_FLAGS_FLASH_RELEASE} -ffreestanding")

SET(CMAKE_EXE_LINKER_FLAGS_FLASH_RELEASE "${CMAKE_EXE_LINKER_FLAGS_FLASH_RELEASE} -fno-builtin")

SET(CMAKE_EXE_LINKER_FLAGS_FLASH_RELEASE "${CMAKE_EXE_LINKER_FLAGS_FLASH_RELEASE} -mthumb")

SET(CMAKE_EXE_LINKER_FLAGS_FLASH_RELEASE "${CMAKE_EXE_LINKER_FLAGS_FLASH_RELEASE} -mapcs")

SET(CMAKE_EXE_LINKER_FLAGS_FLASH_RELEASE "${CMAKE_EXE_LINKER_FLAGS_FLASH_RELEASE} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_FLASH_RELEASE "${CMAKE_EXE_LINKER_FLAGS_FLASH_RELEASE} --gc-sections")

SET(CMAKE_EXE_LINKER_FLAGS_FLASH_RELEASE "${CMAKE_EXE_LINKER_FLAGS_FLASH_RELEASE} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_FLASH_RELEASE "${CMAKE_EXE_LINKER_FLAGS_FLASH_RELEASE} -static")

SET(CMAKE_EXE_LINKER_FLAGS_FLASH_RELEASE "${CMAKE_EXE_LINKER_FLAGS_FLASH_RELEASE} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_FLASH_RELEASE "${CMAKE_EXE_LINKER_FLAGS_FLASH_RELEASE} -z")

SET(CMAKE_EXE_LINKER_FLAGS_FLASH_RELEASE "${CMAKE_EXE_LINKER_FLAGS_FLASH_RELEASE} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_FLASH_RELEASE "${CMAKE_EXE_LINKER_FLAGS_FLASH_RELEASE} muldefs")

SET(CMAKE_EXE_LINKER_FLAGS_FLASH_RELEASE "${CMAKE_EXE_LINKER_FLAGS_FLASH_RELEASE} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_FLASH_RELEASE "${CMAKE_EXE_LINKER_FLAGS_FLASH_RELEASE} -Map=output.map")

SET(CMAKE_EXE_LINKER_FLAGS_FLASH_RELEASE "${CMAKE_EXE_LINKER_FLAGS_FLASH_RELEASE} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_FLASH_RELEASE "${CMAKE_EXE_LINKER_FLAGS_FLASH_RELEASE} --defsym=__stack_size__=0x800")

include_directories(${ProjDirPath}/..)

include_directories(${ProjDirPath}/../../../../../CMSIS/Driver/Include)

include_directories(${ProjDirPath}/../../..)

include_directories(${ProjDirPath}/../../../../../CMSIS/Include)

include_directories(${ProjDirPath}/../../../../../devices)

include_directories(${ProjDirPath}/../../../../../middleware/emwin/emWin_Config)

include_directories(${ProjDirPath}/../../../../../middleware/emwin/emWin_header)

include_directories(${ProjDirPath}/../../../../../components/ili9341)

include_directories(${ProjDirPath}/../../../../../components/ft6x06)

include_directories(${ProjDirPath}/../../../../../devices/MIMXRT685S/drivers)

include_directories(${ProjDirPath}/../../../../../devices/MIMXRT685S/cmsis_drivers)

include_directories(${ProjDirPath}/../../../../../devices/MIMXRT685S)

include_directories(${ProjDirPath}/../../../../../devices/MIMXRT685S/utilities/str)

include_directories(${ProjDirPath}/../../../../../devices/MIMXRT685S/utilities/debug_console)

include_directories(${ProjDirPath}/../../../../../components/uart)

include_directories(${ProjDirPath}/../../../flash_config)

include_directories(${ProjDirPath}/../../../../../components/serial_manager)

include_directories(${ProjDirPath}/../../../../../components/lists)

include_directories(${ProjDirPath}/../../../../../devices/MIMXRT685S/utilities)

add_executable(emwin_gui_demo.elf 
"${ProjDirPath}/../emwin_gui_demo.c"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../emwin_support.c"
"${ProjDirPath}/../emwin_support.h"
"${ProjDirPath}/../RTE_Device.h"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
"${ProjDirPath}/../../../../../components/ili9341/fsl_ili9341.c"
"${ProjDirPath}/../../../../../components/ili9341/fsl_ili9341.h"
"${ProjDirPath}/../../../../../components/ft6x06/fsl_ft6x06.c"
"${ProjDirPath}/../../../../../components/ft6x06/fsl_ft6x06.h"
"${ProjDirPath}/../../../../../devices/MIMXRT685S/drivers/fsl_clock.c"
"${ProjDirPath}/../../../../../devices/MIMXRT685S/drivers/fsl_clock.h"
"${ProjDirPath}/../../../../../devices/MIMXRT685S/drivers/fsl_spi_dma.c"
"${ProjDirPath}/../../../../../devices/MIMXRT685S/drivers/fsl_spi_dma.h"
"${ProjDirPath}/../../../../../devices/MIMXRT685S/drivers/fsl_i2c.c"
"${ProjDirPath}/../../../../../devices/MIMXRT685S/drivers/fsl_i2c.h"
"${ProjDirPath}/../../../../../devices/MIMXRT685S/drivers/fsl_spi.c"
"${ProjDirPath}/../../../../../devices/MIMXRT685S/drivers/fsl_spi.h"
"${ProjDirPath}/../../../../../devices/MIMXRT685S/cmsis_drivers/fsl_spi_cmsis.c"
"${ProjDirPath}/../../../../../devices/MIMXRT685S/cmsis_drivers/fsl_spi_cmsis.h"
"${ProjDirPath}/../../../../../devices/MIMXRT685S/drivers/fsl_inputmux.c"
"${ProjDirPath}/../../../../../devices/MIMXRT685S/drivers/fsl_inputmux.h"
"${ProjDirPath}/../../../../../devices/MIMXRT685S/cmsis_drivers/fsl_i2c_cmsis.c"
"${ProjDirPath}/../../../../../devices/MIMXRT685S/cmsis_drivers/fsl_i2c_cmsis.h"
"${ProjDirPath}/../../../../../devices/MIMXRT685S/drivers/fsl_inputmux_connections.h"
"${ProjDirPath}/../../../../../devices/MIMXRT685S/drivers/fsl_i2c_dma.c"
"${ProjDirPath}/../../../../../devices/MIMXRT685S/drivers/fsl_i2c_dma.h"
"${ProjDirPath}/../../../../../devices/MIMXRT685S/drivers/fsl_dma.c"
"${ProjDirPath}/../../../../../devices/MIMXRT685S/drivers/fsl_dma.h"
"${ProjDirPath}/../../../../../middleware/emwin/emWin_Config/GUIConf.h"
"${ProjDirPath}/../../../../../middleware/emwin/emWin_Config/LCDConf.h"
"${ProjDirPath}/../../../../../middleware/emwin/emWin_header/BUTTON.h"
"${ProjDirPath}/../../../../../middleware/emwin/emWin_header/BUTTON_Private.h"
"${ProjDirPath}/../../../../../middleware/emwin/emWin_header/CALENDAR.h"
"${ProjDirPath}/../../../../../middleware/emwin/emWin_header/CHECKBOX.h"
"${ProjDirPath}/../../../../../middleware/emwin/emWin_header/CHECKBOX_Private.h"
"${ProjDirPath}/../../../../../middleware/emwin/emWin_header/CHOOSECOLOR.h"
"${ProjDirPath}/../../../../../middleware/emwin/emWin_header/CHOOSEFILE.h"
"${ProjDirPath}/../../../../../middleware/emwin/emWin_header/DIALOG.h"
"${ProjDirPath}/../../../../../middleware/emwin/emWin_header/DIALOG_Type.h"
"${ProjDirPath}/../../../../../middleware/emwin/emWin_header/DROPDOWN.h"
"${ProjDirPath}/../../../../../middleware/emwin/emWin_header/DROPDOWN_Private.h"
"${ProjDirPath}/../../../../../middleware/emwin/emWin_header/EDIT.h"
"${ProjDirPath}/../../../../../middleware/emwin/emWin_header/EDIT_Private.h"
"${ProjDirPath}/../../../../../middleware/emwin/emWin_header/FRAMEWIN.h"
"${ProjDirPath}/../../../../../middleware/emwin/emWin_header/FRAMEWIN_Private.h"
"${ProjDirPath}/../../../../../middleware/emwin/emWin_header/GRAPH.h"
"${ProjDirPath}/../../../../../middleware/emwin/emWin_header/GRAPH_Private.h"
"${ProjDirPath}/../../../../../middleware/emwin/emWin_header/GUI.h"
"${ProjDirPath}/../../../../../middleware/emwin/emWin_header/GUIDRV_BitPlains.h"
"${ProjDirPath}/../../../../../middleware/emwin/emWin_header/GUIDRV_DCache.h"
"${ProjDirPath}/../../../../../middleware/emwin/emWin_header/GUIDRV_DCache_Private.h"
"${ProjDirPath}/../../../../../middleware/emwin/emWin_header/GUIDRV_Dist.h"
"${ProjDirPath}/../../../../../middleware/emwin/emWin_header/GUIDRV_FlexColor.h"
"${ProjDirPath}/../../../../../middleware/emwin/emWin_header/GUIDRV_FlexColor_Private.h"
"${ProjDirPath}/../../../../../middleware/emwin/emWin_header/GUIDRV_IST3088.h"
"${ProjDirPath}/../../../../../middleware/emwin/emWin_header/GUIDRV_IST3088_Private.h"
"${ProjDirPath}/../../../../../middleware/emwin/emWin_header/GUIDRV_Lin.h"
"${ProjDirPath}/../../../../../middleware/emwin/emWin_header/GUIDRV_Lin_Opt_16.h"
"${ProjDirPath}/../../../../../middleware/emwin/emWin_header/GUIDRV_Lin_Opt_24.h"
"${ProjDirPath}/../../../../../middleware/emwin/emWin_header/GUIDRV_Lin_Opt_32.h"
"${ProjDirPath}/../../../../../middleware/emwin/emWin_header/GUIDRV_Lin_Opt_8.h"
"${ProjDirPath}/../../../../../middleware/emwin/emWin_header/GUIDRV_Lin_Private.h"
"${ProjDirPath}/../../../../../middleware/emwin/emWin_header/GUIDRV_NoOpt_1_8.h"
"${ProjDirPath}/../../../../../middleware/emwin/emWin_header/GUIDRV_S1D13748.h"
"${ProjDirPath}/../../../../../middleware/emwin/emWin_header/GUIDRV_S1D13748_Private.h"
"${ProjDirPath}/../../../../../middleware/emwin/emWin_header/GUIDRV_S1D13781.h"
"${ProjDirPath}/../../../../../middleware/emwin/emWin_header/GUIDRV_S1D13781_Private.h"
"${ProjDirPath}/../../../../../middleware/emwin/emWin_header/GUIDRV_S1D15G00.h"
"${ProjDirPath}/../../../../../middleware/emwin/emWin_header/GUIDRV_SH_MEM.h"
"${ProjDirPath}/../../../../../middleware/emwin/emWin_header/GUIDRV_SH_MEM_Private.h"
"${ProjDirPath}/../../../../../middleware/emwin/emWin_header/GUIDRV_SLin.h"
"${ProjDirPath}/../../../../../middleware/emwin/emWin_header/GUIDRV_SLin_Private.h"
"${ProjDirPath}/../../../../../middleware/emwin/emWin_header/GUIDRV_SPage.h"
"${ProjDirPath}/../../../../../middleware/emwin/emWin_header/GUIDRV_SPage_Private.h"
"${ProjDirPath}/../../../../../middleware/emwin/emWin_header/GUIDRV_SSD1926.h"
"${ProjDirPath}/../../../../../middleware/emwin/emWin_header/GUIDRV_SSD1926_Private.h"
"${ProjDirPath}/../../../../../middleware/emwin/emWin_header/GUIDRV_Template.h"
"${ProjDirPath}/../../../../../middleware/emwin/emWin_header/GUIDRV_TemplateI.h"
"${ProjDirPath}/../../../../../middleware/emwin/emWin_header/GUIDRV_TemplateI_Private.h"
"${ProjDirPath}/../../../../../middleware/emwin/emWin_header/GUITDRV_ADS7846.h"
"${ProjDirPath}/../../../../../middleware/emwin/emWin_header/GUI_ARRAY.h"
"${ProjDirPath}/../../../../../middleware/emwin/emWin_header/GUI_ARRAY_Private.h"
"${ProjDirPath}/../../../../../middleware/emwin/emWin_header/GUI_BMP_Private.h"
"${ProjDirPath}/../../../../../middleware/emwin/emWin_header/GUI_ConfDefaults.h"
"${ProjDirPath}/../../../../../middleware/emwin/emWin_header/GUI_Debug.h"
"${ProjDirPath}/../../../../../middleware/emwin/emWin_header/GUI_FontIntern.h"
"${ProjDirPath}/../../../../../middleware/emwin/emWin_header/GUI_GCache_Private.h"
"${ProjDirPath}/../../../../../middleware/emwin/emWin_header/GUI_GIF_Private.h"
"${ProjDirPath}/../../../../../middleware/emwin/emWin_header/GUI_HOOK.h"
"${ProjDirPath}/../../../../../middleware/emwin/emWin_header/GUI_JPEG_Private.h"
"${ProjDirPath}/../../../../../middleware/emwin/emWin_header/GUI_Private.h"
"${ProjDirPath}/../../../../../middleware/emwin/emWin_header/GUI_SPRITE_Private.h"
"${ProjDirPath}/../../../../../middleware/emwin/emWin_header/GUI_SetOrientation.h"
"${ProjDirPath}/../../../../../middleware/emwin/emWin_header/GUI_SetOrientationCX.h"
"${ProjDirPath}/../../../../../middleware/emwin/emWin_header/GUI_Type.h"
"${ProjDirPath}/../../../../../middleware/emwin/emWin_header/GUI_VNC.h"
"${ProjDirPath}/../../../../../middleware/emwin/emWin_header/GUI_Version.h"
"${ProjDirPath}/../../../../../middleware/emwin/emWin_header/GUI__BiDi2_Brackets_800.h"
"${ProjDirPath}/../../../../../middleware/emwin/emWin_header/GUI__BiDi2_Types_800.h"
"${ProjDirPath}/../../../../../middleware/emwin/emWin_header/Global.h"
"${ProjDirPath}/../../../../../middleware/emwin/emWin_header/HEADER.h"
"${ProjDirPath}/../../../../../middleware/emwin/emWin_header/HEADER_Private.h"
"${ProjDirPath}/../../../../../middleware/emwin/emWin_header/ICONVIEW.h"
"${ProjDirPath}/../../../../../middleware/emwin/emWin_header/ICONVIEW_Private.h"
"${ProjDirPath}/../../../../../middleware/emwin/emWin_header/IMAGE.h"
"${ProjDirPath}/../../../../../middleware/emwin/emWin_header/IMAGE_Private.h"
"${ProjDirPath}/../../../../../middleware/emwin/emWin_header/IP_FS.h"
"${ProjDirPath}/../../../../../middleware/emwin/emWin_header/KNOB.h"
"${ProjDirPath}/../../../../../middleware/emwin/emWin_header/KNOB_Private.h"
"${ProjDirPath}/../../../../../middleware/emwin/emWin_header/LCD.h"
"${ProjDirPath}/../../../../../middleware/emwin/emWin_header/LCD_ConfDefaults.h"
"${ProjDirPath}/../../../../../middleware/emwin/emWin_header/LCD_Private.h"
"${ProjDirPath}/../../../../../middleware/emwin/emWin_header/LCD_Protected.h"
"${ProjDirPath}/../../../../../middleware/emwin/emWin_header/LCD_SIM.h"
"${ProjDirPath}/../../../../../middleware/emwin/emWin_header/LISTBOX.h"
"${ProjDirPath}/../../../../../middleware/emwin/emWin_header/LISTBOX_Private.h"
"${ProjDirPath}/../../../../../middleware/emwin/emWin_header/LISTVIEW.h"
"${ProjDirPath}/../../../../../middleware/emwin/emWin_header/LISTVIEW_Private.h"
"${ProjDirPath}/../../../../../middleware/emwin/emWin_header/LISTWHEEL.h"
"${ProjDirPath}/../../../../../middleware/emwin/emWin_header/LISTWHEEL_Private.h"
"${ProjDirPath}/../../../../../middleware/emwin/emWin_header/MENU.h"
"${ProjDirPath}/../../../../../middleware/emwin/emWin_header/MENU_Private.h"
"${ProjDirPath}/../../../../../middleware/emwin/emWin_header/MESSAGEBOX.h"
"${ProjDirPath}/../../../../../middleware/emwin/emWin_header/MULTIEDIT.h"
"${ProjDirPath}/../../../../../middleware/emwin/emWin_header/MULTIPAGE.h"
"${ProjDirPath}/../../../../../middleware/emwin/emWin_header/MULTIPAGE_Private.h"
"${ProjDirPath}/../../../../../middleware/emwin/emWin_header/PROGBAR.h"
"${ProjDirPath}/../../../../../middleware/emwin/emWin_header/PROGBAR_Private.h"
"${ProjDirPath}/../../../../../middleware/emwin/emWin_header/RADIO.h"
"${ProjDirPath}/../../../../../middleware/emwin/emWin_header/RADIO_Private.h"
"${ProjDirPath}/../../../../../middleware/emwin/emWin_header/ROTARY.h"
"${ProjDirPath}/../../../../../middleware/emwin/emWin_header/ROTARY_Private.h"
"${ProjDirPath}/../../../../../middleware/emwin/emWin_header/SCROLLBAR.h"
"${ProjDirPath}/../../../../../middleware/emwin/emWin_header/SCROLLBAR_Private.h"
"${ProjDirPath}/../../../../../middleware/emwin/emWin_header/SEGGER.h"
"${ProjDirPath}/../../../../../middleware/emwin/emWin_header/SLIDER.h"
"${ProjDirPath}/../../../../../middleware/emwin/emWin_header/SLIDER_Private.h"
"${ProjDirPath}/../../../../../middleware/emwin/emWin_header/SPINBOX.h"
"${ProjDirPath}/../../../../../middleware/emwin/emWin_header/SPINBOX_Private.h"
"${ProjDirPath}/../../../../../middleware/emwin/emWin_header/SWIPELIST.h"
"${ProjDirPath}/../../../../../middleware/emwin/emWin_header/SWIPELIST_Private.h"
"${ProjDirPath}/../../../../../middleware/emwin/emWin_header/TEXT.h"
"${ProjDirPath}/../../../../../middleware/emwin/emWin_header/TEXT_Private.h"
"${ProjDirPath}/../../../../../middleware/emwin/emWin_header/TREEVIEW.h"
"${ProjDirPath}/../../../../../middleware/emwin/emWin_header/TREEVIEW_Private.h"
"${ProjDirPath}/../../../../../middleware/emwin/emWin_header/WIDGET.h"
"${ProjDirPath}/../../../../../middleware/emwin/emWin_header/WINDOW_Private.h"
"${ProjDirPath}/../../../../../middleware/emwin/emWin_header/WM.h"
"${ProjDirPath}/../../../../../middleware/emwin/emWin_header/WM_GUI.h"
"${ProjDirPath}/../../../../../middleware/emwin/emWin_header/WM_Intern.h"
"${ProjDirPath}/../../../../../devices/MIMXRT685S/drivers/fsl_common.c"
"${ProjDirPath}/../../../../../devices/MIMXRT685S/drivers/fsl_common.h"
"${ProjDirPath}/../../../../../devices/MIMXRT685S/drivers/fsl_power.c"
"${ProjDirPath}/../../../../../devices/MIMXRT685S/drivers/fsl_power.h"
"${ProjDirPath}/../../../../../devices/MIMXRT685S/MIMXRT685S_cm33.h"
"${ProjDirPath}/../../../../../devices/MIMXRT685S/MIMXRT685S_cm33_features.h"
"${ProjDirPath}/../../../../../devices/MIMXRT685S/fsl_device_registers.h"
"${ProjDirPath}/../../../../../devices/MIMXRT685S/utilities/debug_console/fsl_debug_console.c"
"${ProjDirPath}/../../../../../devices/MIMXRT685S/utilities/debug_console/fsl_debug_console.h"
"${ProjDirPath}/../../../../../devices/MIMXRT685S/utilities/debug_console/fsl_debug_console_conf.h"
"${ProjDirPath}/../../../../../devices/MIMXRT685S/utilities/str/fsl_str.c"
"${ProjDirPath}/../../../../../devices/MIMXRT685S/utilities/str/fsl_str.h"
"${ProjDirPath}/../../../../../devices/MIMXRT685S/utilities/fsl_assert.c"
"${ProjDirPath}/../../../../../components/uart/uart.h"
"${ProjDirPath}/../../../../../components/uart/usart_adapter.c"
"${ProjDirPath}/../../../flash_config/flash_config.c"
"${ProjDirPath}/../../../flash_config/flash_config.h"
"${ProjDirPath}/../../../../../devices/MIMXRT685S/drivers/fsl_flexspi.c"
"${ProjDirPath}/../../../../../devices/MIMXRT685S/drivers/fsl_flexspi.h"
"${ProjDirPath}/../../../../../devices/MIMXRT685S/drivers/fsl_cache.c"
"${ProjDirPath}/../../../../../devices/MIMXRT685S/drivers/fsl_cache.h"
"${ProjDirPath}/../../../../../components/serial_manager/serial_manager.c"
"${ProjDirPath}/../../../../../components/serial_manager/serial_manager.h"
"${ProjDirPath}/../../../../../components/serial_manager/serial_port_internal.h"
"${ProjDirPath}/../../../../../components/lists/generic_list.c"
"${ProjDirPath}/../../../../../components/lists/generic_list.h"
"${ProjDirPath}/../../../../../components/serial_manager/serial_port_uart.c"
"${ProjDirPath}/../../../../../components/serial_manager/serial_port_uart.h"
"${ProjDirPath}/../../../../../devices/MIMXRT685S/drivers/fsl_usart.c"
"${ProjDirPath}/../../../../../devices/MIMXRT685S/drivers/fsl_usart.h"
"${ProjDirPath}/../../../../../devices/MIMXRT685S/gcc/startup_MIMXRT685S_cm33.S"
"${ProjDirPath}/../../../../../devices/MIMXRT685S/system_MIMXRT685S_cm33.c"
"${ProjDirPath}/../../../../../devices/MIMXRT685S/system_MIMXRT685S_cm33.h"
"${ProjDirPath}/../../../../../devices/MIMXRT685S/drivers/fsl_flexcomm.c"
"${ProjDirPath}/../../../../../devices/MIMXRT685S/drivers/fsl_flexcomm.h"
"${ProjDirPath}/../../../../../devices/MIMXRT685S/drivers/fsl_iopctl.h"
"${ProjDirPath}/../../../../../devices/MIMXRT685S/drivers/fsl_gpio.c"
"${ProjDirPath}/../../../../../devices/MIMXRT685S/drivers/fsl_gpio.h"
"${ProjDirPath}/../../../../../devices/MIMXRT685S/drivers/fsl_reset.c"
"${ProjDirPath}/../../../../../devices/MIMXRT685S/drivers/fsl_reset.h"
"${ProjDirPath}/../../../../../CMSIS/Include/core_cm33.h"
"${ProjDirPath}/../../../../../CMSIS/Include/tz_context.h"
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
"${ProjDirPath}/../../../../../CMSIS/Driver/Include/Driver_I2C.h"
"${ProjDirPath}/../../../../../CMSIS/Driver/Include/Driver_Common.h"
"${ProjDirPath}/../../../../../CMSIS/Driver/Include/Driver_SPI.h"
"${ProjDirPath}/../../../../../devices/MIMXRT685S/utilities/fsl_sbrk.c"
)


set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -T${ProjDirPath}/MIMXRT685Sxxxx_cm33_ram.ld -static")

set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -T${ProjDirPath}/MIMXRT685Sxxxx_cm33_ram.ld -static")

set(CMAKE_EXE_LINKER_FLAGS_FLASH_DEBUG "${CMAKE_EXE_LINKER_FLAGS_FLASH_DEBUG} -T${ProjDirPath}/MIMXRT685Sxxxx_cm33_flash.ld -static")

set(CMAKE_EXE_LINKER_FLAGS_FLASH_RELEASE "${CMAKE_EXE_LINKER_FLAGS_FLASH_RELEASE} -T${ProjDirPath}/MIMXRT685Sxxxx_cm33_flash.ld -static")

TARGET_LINK_LIBRARIES(emwin_gui_demo.elf -Wl,--start-group)
target_link_libraries(emwin_gui_demo.elf debug m)

target_link_libraries(emwin_gui_demo.elf debug c)

target_link_libraries(emwin_gui_demo.elf debug gcc)

target_link_libraries(emwin_gui_demo.elf debug nosys)

target_link_libraries(emwin_gui_demo.elf optimized m)

target_link_libraries(emwin_gui_demo.elf optimized c)

target_link_libraries(emwin_gui_demo.elf optimized gcc)

target_link_libraries(emwin_gui_demo.elf optimized nosys)

target_link_libraries(emwin_gui_demo.elf optimized m)

target_link_libraries(emwin_gui_demo.elf optimized c)

target_link_libraries(emwin_gui_demo.elf optimized gcc)

target_link_libraries(emwin_gui_demo.elf optimized nosys)

target_link_libraries(emwin_gui_demo.elf optimized m)

target_link_libraries(emwin_gui_demo.elf optimized c)

target_link_libraries(emwin_gui_demo.elf optimized gcc)

target_link_libraries(emwin_gui_demo.elf optimized nosys)

link_directories(${ProjDirPath}/../../../../../middleware/emwin/emWin_library/ARMGCC)

target_link_libraries(emwin_gui_demo.elf debug ${ProjDirPath}/../../../../../middleware/emwin/emWin_library/ARMGCC/libemWin_M33.a)

link_directories(${ProjDirPath}/../../../../../devices/MIMXRT685S/gcc)

target_link_libraries(emwin_gui_demo.elf debug ${ProjDirPath}/../../../../../devices/MIMXRT685S/gcc/libpower_hardabi.a)

link_directories(${ProjDirPath}/../../../../../middleware/emwin/emWin_library/ARMGCC)

target_link_libraries(emwin_gui_demo.elf optimized ${ProjDirPath}/../../../../../middleware/emwin/emWin_library/ARMGCC/libemWin_M33.a)

link_directories(${ProjDirPath}/../../../../../devices/MIMXRT685S/gcc)

target_link_libraries(emwin_gui_demo.elf optimized ${ProjDirPath}/../../../../../devices/MIMXRT685S/gcc/libpower_hardabi.a)

link_directories(${ProjDirPath}/../../../../../middleware/emwin/emWin_library/ARMGCC)

target_link_libraries(emwin_gui_demo.elf optimized ${ProjDirPath}/../../../../../middleware/emwin/emWin_library/ARMGCC/libemWin_M33.a)

link_directories(${ProjDirPath}/../../../../../devices/MIMXRT685S/gcc)

target_link_libraries(emwin_gui_demo.elf optimized ${ProjDirPath}/../../../../../devices/MIMXRT685S/gcc/libpower_hardabi.a)

link_directories(${ProjDirPath}/../../../../../middleware/emwin/emWin_library/ARMGCC)

target_link_libraries(emwin_gui_demo.elf optimized ${ProjDirPath}/../../../../../middleware/emwin/emWin_library/ARMGCC/libemWin_M33.a)

link_directories(${ProjDirPath}/../../../../../devices/MIMXRT685S/gcc)

target_link_libraries(emwin_gui_demo.elf optimized ${ProjDirPath}/../../../../../devices/MIMXRT685S/gcc/libpower_hardabi.a)

TARGET_LINK_LIBRARIES(emwin_gui_demo.elf -Wl,--end-group)

ADD_CUSTOM_COMMAND(TARGET emwin_gui_demo.elf POST_BUILD COMMAND ${CMAKE_OBJCOPY}
-Obinary ${EXECUTABLE_OUTPUT_PATH}/emwin_gui_demo.elf ${EXECUTABLE_OUTPUT_PATH}/sdk20-app.bin)

