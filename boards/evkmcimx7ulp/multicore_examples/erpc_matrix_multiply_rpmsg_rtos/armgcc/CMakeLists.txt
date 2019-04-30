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

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -DNO_CRP")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -g")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -Wall")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -fno-common")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -ffunction-sections")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -fdata-sections")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -ffreestanding")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -fno-builtin")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -mthumb")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -mapcs")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -std=gnu99")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -mcpu=cortex-m4")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -mfloat-abi=hard")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -mfpu=fpv4-sp-d16")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -DNDEBUG")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -D__STARTUP_CLEAR_BSS")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -DNO_CRP")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -Wall")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -fno-common")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -ffunction-sections")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -fdata-sections")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -ffreestanding")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -fno-builtin")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -mthumb")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -mapcs")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -std=gnu99")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -mcpu=cortex-m4")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -mfloat-abi=hard")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -mfpu=fpv4-sp-d16")

SET(CMAKE_ASM_FLAGS_FLASH_DEBUG "${CMAKE_ASM_FLAGS_FLASH_DEBUG} -DDEBUG")

SET(CMAKE_ASM_FLAGS_FLASH_DEBUG "${CMAKE_ASM_FLAGS_FLASH_DEBUG} -DFSL_SDK_DRIVER_QUICK_ACCESS_ENABLE=1")

SET(CMAKE_ASM_FLAGS_FLASH_DEBUG "${CMAKE_ASM_FLAGS_FLASH_DEBUG} -DENABLE_RAM_VECTOR_TABLE")

SET(CMAKE_ASM_FLAGS_FLASH_DEBUG "${CMAKE_ASM_FLAGS_FLASH_DEBUG} -D__STARTUP_CLEAR_BSS")

SET(CMAKE_ASM_FLAGS_FLASH_DEBUG "${CMAKE_ASM_FLAGS_FLASH_DEBUG} -DNO_CRP")

SET(CMAKE_ASM_FLAGS_FLASH_DEBUG "${CMAKE_ASM_FLAGS_FLASH_DEBUG} -g")

SET(CMAKE_ASM_FLAGS_FLASH_DEBUG "${CMAKE_ASM_FLAGS_FLASH_DEBUG} -Wall")

SET(CMAKE_ASM_FLAGS_FLASH_DEBUG "${CMAKE_ASM_FLAGS_FLASH_DEBUG} -fno-common")

SET(CMAKE_ASM_FLAGS_FLASH_DEBUG "${CMAKE_ASM_FLAGS_FLASH_DEBUG} -ffunction-sections")

SET(CMAKE_ASM_FLAGS_FLASH_DEBUG "${CMAKE_ASM_FLAGS_FLASH_DEBUG} -fdata-sections")

SET(CMAKE_ASM_FLAGS_FLASH_DEBUG "${CMAKE_ASM_FLAGS_FLASH_DEBUG} -ffreestanding")

SET(CMAKE_ASM_FLAGS_FLASH_DEBUG "${CMAKE_ASM_FLAGS_FLASH_DEBUG} -fno-builtin")

SET(CMAKE_ASM_FLAGS_FLASH_DEBUG "${CMAKE_ASM_FLAGS_FLASH_DEBUG} -mthumb")

SET(CMAKE_ASM_FLAGS_FLASH_DEBUG "${CMAKE_ASM_FLAGS_FLASH_DEBUG} -mapcs")

SET(CMAKE_ASM_FLAGS_FLASH_DEBUG "${CMAKE_ASM_FLAGS_FLASH_DEBUG} -std=gnu99")

SET(CMAKE_ASM_FLAGS_FLASH_DEBUG "${CMAKE_ASM_FLAGS_FLASH_DEBUG} -mcpu=cortex-m4")

SET(CMAKE_ASM_FLAGS_FLASH_DEBUG "${CMAKE_ASM_FLAGS_FLASH_DEBUG} -mfloat-abi=hard")

SET(CMAKE_ASM_FLAGS_FLASH_DEBUG "${CMAKE_ASM_FLAGS_FLASH_DEBUG} -mfpu=fpv4-sp-d16")

SET(CMAKE_ASM_FLAGS_FLASH_RELEASE "${CMAKE_ASM_FLAGS_FLASH_RELEASE} -DNDEBUG")

SET(CMAKE_ASM_FLAGS_FLASH_RELEASE "${CMAKE_ASM_FLAGS_FLASH_RELEASE} -DFSL_SDK_DRIVER_QUICK_ACCESS_ENABLE=1")

SET(CMAKE_ASM_FLAGS_FLASH_RELEASE "${CMAKE_ASM_FLAGS_FLASH_RELEASE} -DENABLE_RAM_VECTOR_TABLE")

SET(CMAKE_ASM_FLAGS_FLASH_RELEASE "${CMAKE_ASM_FLAGS_FLASH_RELEASE} -D__STARTUP_CLEAR_BSS")

SET(CMAKE_ASM_FLAGS_FLASH_RELEASE "${CMAKE_ASM_FLAGS_FLASH_RELEASE} -DNO_CRP")

SET(CMAKE_ASM_FLAGS_FLASH_RELEASE "${CMAKE_ASM_FLAGS_FLASH_RELEASE} -Wall")

SET(CMAKE_ASM_FLAGS_FLASH_RELEASE "${CMAKE_ASM_FLAGS_FLASH_RELEASE} -fno-common")

SET(CMAKE_ASM_FLAGS_FLASH_RELEASE "${CMAKE_ASM_FLAGS_FLASH_RELEASE} -ffunction-sections")

SET(CMAKE_ASM_FLAGS_FLASH_RELEASE "${CMAKE_ASM_FLAGS_FLASH_RELEASE} -fdata-sections")

SET(CMAKE_ASM_FLAGS_FLASH_RELEASE "${CMAKE_ASM_FLAGS_FLASH_RELEASE} -ffreestanding")

SET(CMAKE_ASM_FLAGS_FLASH_RELEASE "${CMAKE_ASM_FLAGS_FLASH_RELEASE} -fno-builtin")

SET(CMAKE_ASM_FLAGS_FLASH_RELEASE "${CMAKE_ASM_FLAGS_FLASH_RELEASE} -mthumb")

SET(CMAKE_ASM_FLAGS_FLASH_RELEASE "${CMAKE_ASM_FLAGS_FLASH_RELEASE} -mapcs")

SET(CMAKE_ASM_FLAGS_FLASH_RELEASE "${CMAKE_ASM_FLAGS_FLASH_RELEASE} -std=gnu99")

SET(CMAKE_ASM_FLAGS_FLASH_RELEASE "${CMAKE_ASM_FLAGS_FLASH_RELEASE} -mcpu=cortex-m4")

SET(CMAKE_ASM_FLAGS_FLASH_RELEASE "${CMAKE_ASM_FLAGS_FLASH_RELEASE} -mfloat-abi=hard")

SET(CMAKE_ASM_FLAGS_FLASH_RELEASE "${CMAKE_ASM_FLAGS_FLASH_RELEASE} -mfpu=fpv4-sp-d16")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DCPU_MCIMX7U5DVP08")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DFSL_RTOS_FREE_RTOS")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DSDK_I2C_BASED_COMPONENT_USED=1")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DBOARD_USE_CODEC=1")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wall")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -fno-common")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -ffunction-sections")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -fdata-sections")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -ffreestanding")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -fno-builtin")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -mthumb")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -mapcs")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -std=gnu99")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -mcpu=cortex-m4")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -mfloat-abi=hard")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -mfpu=fpv4-sp-d16")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -MMD")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -MP")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DNDEBUG")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DCPU_MCIMX7U5DVP08")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DFSL_RTOS_FREE_RTOS")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DSDK_I2C_BASED_COMPONENT_USED=1")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DBOARD_USE_CODEC=1")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Os")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Wall")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -fno-common")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -ffunction-sections")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -fdata-sections")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -ffreestanding")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -fno-builtin")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -mthumb")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -mapcs")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -std=gnu99")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -mcpu=cortex-m4")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -mfloat-abi=hard")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -mfpu=fpv4-sp-d16")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -MMD")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -MP")

SET(CMAKE_C_FLAGS_FLASH_DEBUG "${CMAKE_C_FLAGS_FLASH_DEBUG} -DDEBUG")

SET(CMAKE_C_FLAGS_FLASH_DEBUG "${CMAKE_C_FLAGS_FLASH_DEBUG} -DFSL_SDK_DRIVER_QUICK_ACCESS_ENABLE=1")

SET(CMAKE_C_FLAGS_FLASH_DEBUG "${CMAKE_C_FLAGS_FLASH_DEBUG} -DENABLE_RAM_VECTOR_TABLE")

SET(CMAKE_C_FLAGS_FLASH_DEBUG "${CMAKE_C_FLAGS_FLASH_DEBUG} -DCPU_MCIMX7U5DVP08")

SET(CMAKE_C_FLAGS_FLASH_DEBUG "${CMAKE_C_FLAGS_FLASH_DEBUG} -DFSL_RTOS_FREE_RTOS")

SET(CMAKE_C_FLAGS_FLASH_DEBUG "${CMAKE_C_FLAGS_FLASH_DEBUG} -DSDK_I2C_BASED_COMPONENT_USED=1")

SET(CMAKE_C_FLAGS_FLASH_DEBUG "${CMAKE_C_FLAGS_FLASH_DEBUG} -DBOARD_USE_CODEC=1")

SET(CMAKE_C_FLAGS_FLASH_DEBUG "${CMAKE_C_FLAGS_FLASH_DEBUG} -g")

SET(CMAKE_C_FLAGS_FLASH_DEBUG "${CMAKE_C_FLAGS_FLASH_DEBUG} -O0")

SET(CMAKE_C_FLAGS_FLASH_DEBUG "${CMAKE_C_FLAGS_FLASH_DEBUG} -Wall")

SET(CMAKE_C_FLAGS_FLASH_DEBUG "${CMAKE_C_FLAGS_FLASH_DEBUG} -fno-common")

SET(CMAKE_C_FLAGS_FLASH_DEBUG "${CMAKE_C_FLAGS_FLASH_DEBUG} -ffunction-sections")

SET(CMAKE_C_FLAGS_FLASH_DEBUG "${CMAKE_C_FLAGS_FLASH_DEBUG} -fdata-sections")

SET(CMAKE_C_FLAGS_FLASH_DEBUG "${CMAKE_C_FLAGS_FLASH_DEBUG} -ffreestanding")

SET(CMAKE_C_FLAGS_FLASH_DEBUG "${CMAKE_C_FLAGS_FLASH_DEBUG} -fno-builtin")

SET(CMAKE_C_FLAGS_FLASH_DEBUG "${CMAKE_C_FLAGS_FLASH_DEBUG} -mthumb")

SET(CMAKE_C_FLAGS_FLASH_DEBUG "${CMAKE_C_FLAGS_FLASH_DEBUG} -mapcs")

SET(CMAKE_C_FLAGS_FLASH_DEBUG "${CMAKE_C_FLAGS_FLASH_DEBUG} -std=gnu99")

SET(CMAKE_C_FLAGS_FLASH_DEBUG "${CMAKE_C_FLAGS_FLASH_DEBUG} -mcpu=cortex-m4")

SET(CMAKE_C_FLAGS_FLASH_DEBUG "${CMAKE_C_FLAGS_FLASH_DEBUG} -mfloat-abi=hard")

SET(CMAKE_C_FLAGS_FLASH_DEBUG "${CMAKE_C_FLAGS_FLASH_DEBUG} -mfpu=fpv4-sp-d16")

SET(CMAKE_C_FLAGS_FLASH_DEBUG "${CMAKE_C_FLAGS_FLASH_DEBUG} -MMD")

SET(CMAKE_C_FLAGS_FLASH_DEBUG "${CMAKE_C_FLAGS_FLASH_DEBUG} -MP")

SET(CMAKE_C_FLAGS_FLASH_RELEASE "${CMAKE_C_FLAGS_FLASH_RELEASE} -DNDEBUG")

SET(CMAKE_C_FLAGS_FLASH_RELEASE "${CMAKE_C_FLAGS_FLASH_RELEASE} -DFSL_SDK_DRIVER_QUICK_ACCESS_ENABLE=1")

SET(CMAKE_C_FLAGS_FLASH_RELEASE "${CMAKE_C_FLAGS_FLASH_RELEASE} -DENABLE_RAM_VECTOR_TABLE")

SET(CMAKE_C_FLAGS_FLASH_RELEASE "${CMAKE_C_FLAGS_FLASH_RELEASE} -DCPU_MCIMX7U5DVP08")

SET(CMAKE_C_FLAGS_FLASH_RELEASE "${CMAKE_C_FLAGS_FLASH_RELEASE} -DFSL_RTOS_FREE_RTOS")

SET(CMAKE_C_FLAGS_FLASH_RELEASE "${CMAKE_C_FLAGS_FLASH_RELEASE} -DSDK_I2C_BASED_COMPONENT_USED=1")

SET(CMAKE_C_FLAGS_FLASH_RELEASE "${CMAKE_C_FLAGS_FLASH_RELEASE} -DBOARD_USE_CODEC=1")

SET(CMAKE_C_FLAGS_FLASH_RELEASE "${CMAKE_C_FLAGS_FLASH_RELEASE} -Os")

SET(CMAKE_C_FLAGS_FLASH_RELEASE "${CMAKE_C_FLAGS_FLASH_RELEASE} -Wall")

SET(CMAKE_C_FLAGS_FLASH_RELEASE "${CMAKE_C_FLAGS_FLASH_RELEASE} -fno-common")

SET(CMAKE_C_FLAGS_FLASH_RELEASE "${CMAKE_C_FLAGS_FLASH_RELEASE} -ffunction-sections")

SET(CMAKE_C_FLAGS_FLASH_RELEASE "${CMAKE_C_FLAGS_FLASH_RELEASE} -fdata-sections")

SET(CMAKE_C_FLAGS_FLASH_RELEASE "${CMAKE_C_FLAGS_FLASH_RELEASE} -ffreestanding")

SET(CMAKE_C_FLAGS_FLASH_RELEASE "${CMAKE_C_FLAGS_FLASH_RELEASE} -fno-builtin")

SET(CMAKE_C_FLAGS_FLASH_RELEASE "${CMAKE_C_FLAGS_FLASH_RELEASE} -mthumb")

SET(CMAKE_C_FLAGS_FLASH_RELEASE "${CMAKE_C_FLAGS_FLASH_RELEASE} -mapcs")

SET(CMAKE_C_FLAGS_FLASH_RELEASE "${CMAKE_C_FLAGS_FLASH_RELEASE} -std=gnu99")

SET(CMAKE_C_FLAGS_FLASH_RELEASE "${CMAKE_C_FLAGS_FLASH_RELEASE} -mcpu=cortex-m4")

SET(CMAKE_C_FLAGS_FLASH_RELEASE "${CMAKE_C_FLAGS_FLASH_RELEASE} -mfloat-abi=hard")

SET(CMAKE_C_FLAGS_FLASH_RELEASE "${CMAKE_C_FLAGS_FLASH_RELEASE} -mfpu=fpv4-sp-d16")

SET(CMAKE_C_FLAGS_FLASH_RELEASE "${CMAKE_C_FLAGS_FLASH_RELEASE} -MMD")

SET(CMAKE_C_FLAGS_FLASH_RELEASE "${CMAKE_C_FLAGS_FLASH_RELEASE} -MP")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DCPU_MCIMX7U5DVP08")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-common")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -ffunction-sections")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fdata-sections")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -ffreestanding")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-builtin")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -mthumb")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -mapcs")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-rtti")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-exceptions")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -mcpu=cortex-m4")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -mfloat-abi=hard")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -mfpu=fpv4-sp-d16")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -MMD")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -MP")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DNDEBUG")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DCPU_MCIMX7U5DVP08")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Os")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fno-common")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -ffunction-sections")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fdata-sections")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -ffreestanding")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fno-builtin")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -mthumb")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -mapcs")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fno-rtti")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fno-exceptions")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -mcpu=cortex-m4")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -mfloat-abi=hard")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -mfpu=fpv4-sp-d16")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -MMD")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -MP")

SET(CMAKE_CXX_FLAGS_FLASH_DEBUG "${CMAKE_CXX_FLAGS_FLASH_DEBUG} -DDEBUG")

SET(CMAKE_CXX_FLAGS_FLASH_DEBUG "${CMAKE_CXX_FLAGS_FLASH_DEBUG} -DFSL_SDK_DRIVER_QUICK_ACCESS_ENABLE=1")

SET(CMAKE_CXX_FLAGS_FLASH_DEBUG "${CMAKE_CXX_FLAGS_FLASH_DEBUG} -DENABLE_RAM_VECTOR_TABLE")

SET(CMAKE_CXX_FLAGS_FLASH_DEBUG "${CMAKE_CXX_FLAGS_FLASH_DEBUG} -DCPU_MCIMX7U5DVP08")

SET(CMAKE_CXX_FLAGS_FLASH_DEBUG "${CMAKE_CXX_FLAGS_FLASH_DEBUG} -g")

SET(CMAKE_CXX_FLAGS_FLASH_DEBUG "${CMAKE_CXX_FLAGS_FLASH_DEBUG} -O0")

SET(CMAKE_CXX_FLAGS_FLASH_DEBUG "${CMAKE_CXX_FLAGS_FLASH_DEBUG} -Wall")

SET(CMAKE_CXX_FLAGS_FLASH_DEBUG "${CMAKE_CXX_FLAGS_FLASH_DEBUG} -fno-common")

SET(CMAKE_CXX_FLAGS_FLASH_DEBUG "${CMAKE_CXX_FLAGS_FLASH_DEBUG} -ffunction-sections")

SET(CMAKE_CXX_FLAGS_FLASH_DEBUG "${CMAKE_CXX_FLAGS_FLASH_DEBUG} -fdata-sections")

SET(CMAKE_CXX_FLAGS_FLASH_DEBUG "${CMAKE_CXX_FLAGS_FLASH_DEBUG} -ffreestanding")

SET(CMAKE_CXX_FLAGS_FLASH_DEBUG "${CMAKE_CXX_FLAGS_FLASH_DEBUG} -fno-builtin")

SET(CMAKE_CXX_FLAGS_FLASH_DEBUG "${CMAKE_CXX_FLAGS_FLASH_DEBUG} -mthumb")

SET(CMAKE_CXX_FLAGS_FLASH_DEBUG "${CMAKE_CXX_FLAGS_FLASH_DEBUG} -mapcs")

SET(CMAKE_CXX_FLAGS_FLASH_DEBUG "${CMAKE_CXX_FLAGS_FLASH_DEBUG} -fno-rtti")

SET(CMAKE_CXX_FLAGS_FLASH_DEBUG "${CMAKE_CXX_FLAGS_FLASH_DEBUG} -fno-exceptions")

SET(CMAKE_CXX_FLAGS_FLASH_DEBUG "${CMAKE_CXX_FLAGS_FLASH_DEBUG} -mcpu=cortex-m4")

SET(CMAKE_CXX_FLAGS_FLASH_DEBUG "${CMAKE_CXX_FLAGS_FLASH_DEBUG} -mfloat-abi=hard")

SET(CMAKE_CXX_FLAGS_FLASH_DEBUG "${CMAKE_CXX_FLAGS_FLASH_DEBUG} -mfpu=fpv4-sp-d16")

SET(CMAKE_CXX_FLAGS_FLASH_DEBUG "${CMAKE_CXX_FLAGS_FLASH_DEBUG} -MMD")

SET(CMAKE_CXX_FLAGS_FLASH_DEBUG "${CMAKE_CXX_FLAGS_FLASH_DEBUG} -MP")

SET(CMAKE_CXX_FLAGS_FLASH_RELEASE "${CMAKE_CXX_FLAGS_FLASH_RELEASE} -DNDEBUG")

SET(CMAKE_CXX_FLAGS_FLASH_RELEASE "${CMAKE_CXX_FLAGS_FLASH_RELEASE} -DFSL_SDK_DRIVER_QUICK_ACCESS_ENABLE=1")

SET(CMAKE_CXX_FLAGS_FLASH_RELEASE "${CMAKE_CXX_FLAGS_FLASH_RELEASE} -DENABLE_RAM_VECTOR_TABLE")

SET(CMAKE_CXX_FLAGS_FLASH_RELEASE "${CMAKE_CXX_FLAGS_FLASH_RELEASE} -DCPU_MCIMX7U5DVP08")

SET(CMAKE_CXX_FLAGS_FLASH_RELEASE "${CMAKE_CXX_FLAGS_FLASH_RELEASE} -Os")

SET(CMAKE_CXX_FLAGS_FLASH_RELEASE "${CMAKE_CXX_FLAGS_FLASH_RELEASE} -Wall")

SET(CMAKE_CXX_FLAGS_FLASH_RELEASE "${CMAKE_CXX_FLAGS_FLASH_RELEASE} -fno-common")

SET(CMAKE_CXX_FLAGS_FLASH_RELEASE "${CMAKE_CXX_FLAGS_FLASH_RELEASE} -ffunction-sections")

SET(CMAKE_CXX_FLAGS_FLASH_RELEASE "${CMAKE_CXX_FLAGS_FLASH_RELEASE} -fdata-sections")

SET(CMAKE_CXX_FLAGS_FLASH_RELEASE "${CMAKE_CXX_FLAGS_FLASH_RELEASE} -ffreestanding")

SET(CMAKE_CXX_FLAGS_FLASH_RELEASE "${CMAKE_CXX_FLAGS_FLASH_RELEASE} -fno-builtin")

SET(CMAKE_CXX_FLAGS_FLASH_RELEASE "${CMAKE_CXX_FLAGS_FLASH_RELEASE} -mthumb")

SET(CMAKE_CXX_FLAGS_FLASH_RELEASE "${CMAKE_CXX_FLAGS_FLASH_RELEASE} -mapcs")

SET(CMAKE_CXX_FLAGS_FLASH_RELEASE "${CMAKE_CXX_FLAGS_FLASH_RELEASE} -fno-rtti")

SET(CMAKE_CXX_FLAGS_FLASH_RELEASE "${CMAKE_CXX_FLAGS_FLASH_RELEASE} -fno-exceptions")

SET(CMAKE_CXX_FLAGS_FLASH_RELEASE "${CMAKE_CXX_FLAGS_FLASH_RELEASE} -mcpu=cortex-m4")

SET(CMAKE_CXX_FLAGS_FLASH_RELEASE "${CMAKE_CXX_FLAGS_FLASH_RELEASE} -mfloat-abi=hard")

SET(CMAKE_CXX_FLAGS_FLASH_RELEASE "${CMAKE_CXX_FLAGS_FLASH_RELEASE} -mfpu=fpv4-sp-d16")

SET(CMAKE_CXX_FLAGS_FLASH_RELEASE "${CMAKE_CXX_FLAGS_FLASH_RELEASE} -MMD")

SET(CMAKE_CXX_FLAGS_FLASH_RELEASE "${CMAKE_CXX_FLAGS_FLASH_RELEASE} -MP")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -g")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} --specs=nano.specs")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} --specs=nosys.specs")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -Wall")

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

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -mcpu=cortex-m4")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -mfloat-abi=hard")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -mfpu=fpv4-sp-d16")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} --defsym=__stack_size__=0x400")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} --defsym=__heap_size__=0x400")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} --specs=nano.specs")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} --specs=nosys.specs")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -Wall")

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

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -mcpu=cortex-m4")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -mfloat-abi=hard")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -mfpu=fpv4-sp-d16")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} --defsym=__stack_size__=0x400")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} --defsym=__heap_size__=0x400")

SET(CMAKE_EXE_LINKER_FLAGS_FLASH_DEBUG "${CMAKE_EXE_LINKER_FLAGS_FLASH_DEBUG} -g")

SET(CMAKE_EXE_LINKER_FLAGS_FLASH_DEBUG "${CMAKE_EXE_LINKER_FLAGS_FLASH_DEBUG} --specs=nano.specs")

SET(CMAKE_EXE_LINKER_FLAGS_FLASH_DEBUG "${CMAKE_EXE_LINKER_FLAGS_FLASH_DEBUG} --specs=nosys.specs")

SET(CMAKE_EXE_LINKER_FLAGS_FLASH_DEBUG "${CMAKE_EXE_LINKER_FLAGS_FLASH_DEBUG} -Wall")

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

SET(CMAKE_EXE_LINKER_FLAGS_FLASH_DEBUG "${CMAKE_EXE_LINKER_FLAGS_FLASH_DEBUG} -mcpu=cortex-m4")

SET(CMAKE_EXE_LINKER_FLAGS_FLASH_DEBUG "${CMAKE_EXE_LINKER_FLAGS_FLASH_DEBUG} -mfloat-abi=hard")

SET(CMAKE_EXE_LINKER_FLAGS_FLASH_DEBUG "${CMAKE_EXE_LINKER_FLAGS_FLASH_DEBUG} -mfpu=fpv4-sp-d16")

SET(CMAKE_EXE_LINKER_FLAGS_FLASH_DEBUG "${CMAKE_EXE_LINKER_FLAGS_FLASH_DEBUG} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_FLASH_DEBUG "${CMAKE_EXE_LINKER_FLAGS_FLASH_DEBUG} --defsym=__stack_size__=0x400")

SET(CMAKE_EXE_LINKER_FLAGS_FLASH_DEBUG "${CMAKE_EXE_LINKER_FLAGS_FLASH_DEBUG} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_FLASH_DEBUG "${CMAKE_EXE_LINKER_FLAGS_FLASH_DEBUG} --defsym=__heap_size__=0x400")

SET(CMAKE_EXE_LINKER_FLAGS_FLASH_RELEASE "${CMAKE_EXE_LINKER_FLAGS_FLASH_RELEASE} --specs=nano.specs")

SET(CMAKE_EXE_LINKER_FLAGS_FLASH_RELEASE "${CMAKE_EXE_LINKER_FLAGS_FLASH_RELEASE} --specs=nosys.specs")

SET(CMAKE_EXE_LINKER_FLAGS_FLASH_RELEASE "${CMAKE_EXE_LINKER_FLAGS_FLASH_RELEASE} -Wall")

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

SET(CMAKE_EXE_LINKER_FLAGS_FLASH_RELEASE "${CMAKE_EXE_LINKER_FLAGS_FLASH_RELEASE} -mcpu=cortex-m4")

SET(CMAKE_EXE_LINKER_FLAGS_FLASH_RELEASE "${CMAKE_EXE_LINKER_FLAGS_FLASH_RELEASE} -mfloat-abi=hard")

SET(CMAKE_EXE_LINKER_FLAGS_FLASH_RELEASE "${CMAKE_EXE_LINKER_FLAGS_FLASH_RELEASE} -mfpu=fpv4-sp-d16")

SET(CMAKE_EXE_LINKER_FLAGS_FLASH_RELEASE "${CMAKE_EXE_LINKER_FLAGS_FLASH_RELEASE} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_FLASH_RELEASE "${CMAKE_EXE_LINKER_FLAGS_FLASH_RELEASE} --defsym=__stack_size__=0x400")

SET(CMAKE_EXE_LINKER_FLAGS_FLASH_RELEASE "${CMAKE_EXE_LINKER_FLAGS_FLASH_RELEASE} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_FLASH_RELEASE "${CMAKE_EXE_LINKER_FLAGS_FLASH_RELEASE} --defsym=__heap_size__=0x400")

include_directories(${ProjDirPath}/..)

include_directories(${ProjDirPath}/../../..)

include_directories(${ProjDirPath}/..)

include_directories(${ProjDirPath}/../../..)

include_directories(${ProjDirPath}/..)

include_directories(${ProjDirPath}/../../..)

include_directories(${ProjDirPath}/..)

include_directories(${ProjDirPath}/../../..)

include_directories(${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/FreeRTOS/portable/GCC/ARM_CM4F)

include_directories(${ProjDirPath}/..)

include_directories(${ProjDirPath}/../../erpc_common/erpc_matrix_multiply/service)

include_directories(${ProjDirPath}/../../..)

include_directories(${ProjDirPath}/../../erpc_common)

include_directories(${ProjDirPath}/../../../../../CMSIS/Include)

include_directories(${ProjDirPath}/../../../../../devices)

include_directories(${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include)

include_directories(${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include/private)

include_directories(${ProjDirPath}/../srtm/channels)

include_directories(${ProjDirPath}/../srtm/services)

include_directories(${ProjDirPath}/../../../../../devices/MCIMX7U5/drivers)

include_directories(${ProjDirPath}/../../../../../devices/MCIMX7U5)

include_directories(${ProjDirPath}/../../../../../devices/MCIMX7U5/utilities/str)

include_directories(${ProjDirPath}/../../../../../devices/MCIMX7U5/utilities/debug_console)

include_directories(${ProjDirPath}/../../../../../components/uart)

include_directories(${ProjDirPath}/../../../../../components/serial_manager)

include_directories(${ProjDirPath}/../../../../../components/lists)

include_directories(${ProjDirPath}/../../../../../components/pf1550)

include_directories(${ProjDirPath}/../../../../../components/codec/wm8960)

include_directories(${ProjDirPath}/../../../../../components/codec)

include_directories(${ProjDirPath}/../../../../../components/srtm/include)

include_directories(${ProjDirPath}/../../../../../components/srtm/srtm)

include_directories(${ProjDirPath}/../../../../../middleware/multicore/rpmsg_lite/lib/include/platform/imx7ulp_m4)

include_directories(${ProjDirPath}/../../../../../middleware/multicore/rpmsg_lite/lib/include)

include_directories(${ProjDirPath}/../../../../../middleware/issdk/sensors)

include_directories(${ProjDirPath}/../../../../../CMSIS/Driver/Include)

include_directories(${ProjDirPath}/../../../../../middleware/issdk/algorithms/pedometer/source)

include_directories(${ProjDirPath}/../../../../../middleware/issdk/algorithms/pedometer/include)

include_directories(${ProjDirPath}/../../../../../middleware/issdk/algorithms/pedometer/include/libinclude)

include_directories(${ProjDirPath}/../../../../../middleware/multicore/erpc/erpc_c/infra)

include_directories(${ProjDirPath}/../../../../../middleware/multicore/erpc/erpc_c/port)

include_directories(${ProjDirPath}/../../../../../middleware/multicore/erpc/erpc_c/setup)

include_directories(${ProjDirPath}/../../../../../middleware/multicore/erpc/erpc_c/transports)

include_directories(${ProjDirPath}/../../../../../middleware/multicore/erpc/erpc_c)

add_executable(erpc_matrix_multiply_rpmsg_rtos_imxcm4.elf 
"${ProjDirPath}/../main_core1.c"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../rpmsg_config.h"
"${ProjDirPath}/../erpc_config.h"
"${ProjDirPath}/../python/erpc_matrix_multiply_python.md"
"${ProjDirPath}/../python/example.py"
"${ProjDirPath}/../python/service/__init__.py"
"${ProjDirPath}/../python/service/erpc_matrix_multiply.erpc"
"${ProjDirPath}/../python/service/erpc_matrix_multiply/__init__.py"
"${ProjDirPath}/../python/service/erpc_matrix_multiply/client.py"
"${ProjDirPath}/../python/service/erpc_matrix_multiply/common.py"
"${ProjDirPath}/../python/service/erpc_matrix_multiply/interface.py"
"${ProjDirPath}/../python/service/erpc_matrix_multiply/server.py"
"${ProjDirPath}/../../erpc_common/erpc_error_handler.h"
"${ProjDirPath}/../../erpc_common/erpc_error_handler.cpp"
"${ProjDirPath}/../../erpc_common/erpc_matrix_multiply/service/erpc_matrix_multiply.erpc"
"${ProjDirPath}/../../erpc_common/erpc_matrix_multiply/service/erpc_matrix_multiply.h"
"${ProjDirPath}/../../erpc_common/erpc_matrix_multiply/service/erpc_matrix_multiply_server.h"
"${ProjDirPath}/../../erpc_common/erpc_matrix_multiply/service/erpc_matrix_multiply_server.cpp"
"${ProjDirPath}/../FreeRTOSConfig.h"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
"${ProjDirPath}/../fsl_iomuxc.h"
"${ProjDirPath}/../app_srtm.c"
"${ProjDirPath}/../app_srtm.h"
"${ProjDirPath}/../srtm/channels/srtm_rpmsg_endpoint.h"
"${ProjDirPath}/../srtm/channels/srtm_rpmsg_endpoint.c"
"${ProjDirPath}/../srtm/services/srtm_lfcl_service.h"
"${ProjDirPath}/../srtm/services/srtm_lfcl_service.c"
"${ProjDirPath}/../srtm/services/srtm_pmic_service.h"
"${ProjDirPath}/../srtm/services/srtm_pmic_service.c"
"${ProjDirPath}/../srtm/services/srtm_pf1550_adapter.h"
"${ProjDirPath}/../srtm/services/srtm_pf1550_adapter.c"
"${ProjDirPath}/../srtm/services/srtm_audio_service.h"
"${ProjDirPath}/../srtm/services/srtm_audio_service.c"
"${ProjDirPath}/../srtm/services/srtm_sai_edma_adapter.h"
"${ProjDirPath}/../srtm/services/srtm_sai_edma_adapter.c"
"${ProjDirPath}/../srtm/services/srtm_i2c_codec_adapter.h"
"${ProjDirPath}/../srtm/services/srtm_i2c_codec_adapter.c"
"${ProjDirPath}/../srtm/services/srtm_io_service.h"
"${ProjDirPath}/../srtm/services/srtm_io_service.c"
"${ProjDirPath}/../srtm/services/srtm_keypad_service.h"
"${ProjDirPath}/../srtm/services/srtm_keypad_service.c"
"${ProjDirPath}/../srtm/services/srtm_rtc_service.h"
"${ProjDirPath}/../srtm/services/srtm_rtc_service.c"
"${ProjDirPath}/../srtm/services/srtm_snvs_lp_rtc_adapter.h"
"${ProjDirPath}/../srtm/services/srtm_snvs_lp_rtc_adapter.c"
"${ProjDirPath}/../srtm/services/srtm_sensor_service.h"
"${ProjDirPath}/../srtm/services/srtm_sensor_service.c"
"${ProjDirPath}/../../../../../devices/MCIMX7U5/drivers/fsl_gpio.h"
"${ProjDirPath}/../../../../../devices/MCIMX7U5/drivers/fsl_gpio.c"
"${ProjDirPath}/../../../../../devices/MCIMX7U5/drivers/fsl_port.h"
"${ProjDirPath}/../../../../../devices/MCIMX7U5/drivers/fsl_clock.h"
"${ProjDirPath}/../../../../../devices/MCIMX7U5/drivers/fsl_clock.c"
"${ProjDirPath}/../../../../../devices/MCIMX7U5/drivers/fsl_mu.h"
"${ProjDirPath}/../../../../../devices/MCIMX7U5/drivers/fsl_mu.c"
"${ProjDirPath}/../../../../../middleware/multicore/erpc/erpc_c/infra/erpc_simple_server.h"
"${ProjDirPath}/../../../../../middleware/multicore/erpc/erpc_c/infra/erpc_simple_server.cpp"
"${ProjDirPath}/../../../../../middleware/multicore/erpc/erpc_c/infra/erpc_server.h"
"${ProjDirPath}/../../../../../middleware/multicore/erpc/erpc_c/infra/erpc_server.cpp"
"${ProjDirPath}/../../../../../middleware/multicore/erpc/erpc_c/setup/erpc_server_setup.h"
"${ProjDirPath}/../../../../../middleware/multicore/erpc/erpc_c/setup/erpc_server_setup.cpp"
"${ProjDirPath}/../../../../../middleware/multicore/erpc/erpc_c/setup/erpc_setup_rpmsg_tty_rtos_remote.cpp"
"${ProjDirPath}/../../../../../middleware/multicore/erpc/erpc_c/transports/erpc_rpmsg_lite_base_transport.h"
"${ProjDirPath}/../../../../../middleware/multicore/erpc/erpc_c/transports/erpc_rpmsg_tty_rtos_transport.h"
"${ProjDirPath}/../../../../../middleware/multicore/erpc/erpc_c/transports/erpc_rpmsg_tty_rtos_transport.cpp"
"${ProjDirPath}/../../../../../middleware/multicore/erpc/erpc_c/setup/erpc_setup_mbf_rpmsg_tty.cpp"
"${ProjDirPath}/../../../../../middleware/multicore/erpc/erpc_c/port/erpc_threading.h"
"${ProjDirPath}/../../../../../middleware/multicore/erpc/erpc_c/port/erpc_threading_freertos.cpp"
"${ProjDirPath}/../../../../../middleware/multicore/erpc/erpc_c/port/erpc_port.h"
"${ProjDirPath}/../../../../../middleware/multicore/erpc/erpc_c/port/erpc_port_freertos.cpp"
"${ProjDirPath}/../../../../../middleware/multicore/erpc/erpc_c/port/erpc_config_internal.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/LICENSE"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include/event_groups.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include/FreeRTOS.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include/message_buffer.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include/queue.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include/semphr.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include/stream_buffer.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include/task.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include/timers.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include/freertos_tasks_c_additions.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include/private/deprecated_definitions.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include/private/list.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include/private/mpu_wrappers.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include/private/mpu_prototypes.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include/private/portable.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include/private/projdefs.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include/private/stack_macros.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include/private/threading_alt.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/FreeRTOS/event_groups.c"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/FreeRTOS/list.c"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/FreeRTOS/queue.c"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/FreeRTOS/stream_buffer.c"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/FreeRTOS/tasks.c"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/FreeRTOS/timers.c"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/FreeRTOS/portable/GCC/ARM_CM4F/fsl_tickless_generic.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/FreeRTOS/portable/GCC/ARM_CM4F/fsl_tickless_systick.c"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/FreeRTOS/portable/GCC/ARM_CM4F/port.c"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/FreeRTOS/portable/GCC/ARM_CM4F/portmacro.h"
"${ProjDirPath}/../../../../../devices/MCIMX7U5/drivers/fsl_common.h"
"${ProjDirPath}/../../../../../devices/MCIMX7U5/drivers/fsl_common.c"
"${ProjDirPath}/../../../../../devices/MCIMX7U5/fsl_device_registers.h"
"${ProjDirPath}/../../../../../devices/MCIMX7U5/MCIMX7U5_cm4.h"
"${ProjDirPath}/../../../../../devices/MCIMX7U5/MCIMX7U5_cm4_features.h"
"${ProjDirPath}/../../../../../devices/MCIMX7U5/system_MCIMX7U5_cm4.c"
"${ProjDirPath}/../../../../../devices/MCIMX7U5/system_MCIMX7U5_cm4.h"
"${ProjDirPath}/../../../../../devices/MCIMX7U5/utilities/str/fsl_str.c"
"${ProjDirPath}/../../../../../devices/MCIMX7U5/utilities/str/fsl_str.h"
"${ProjDirPath}/../../../../../devices/MCIMX7U5/utilities/debug_console/fsl_debug_console.c"
"${ProjDirPath}/../../../../../devices/MCIMX7U5/utilities/debug_console/fsl_debug_console.h"
"${ProjDirPath}/../../../../../devices/MCIMX7U5/utilities/debug_console/fsl_debug_console_conf.h"
"${ProjDirPath}/../../../../../components/uart/uart.h"
"${ProjDirPath}/../../../../../components/uart/lpuart_adapter.c"
"${ProjDirPath}/../../../../../components/serial_manager/serial_manager.h"
"${ProjDirPath}/../../../../../components/serial_manager/serial_manager.c"
"${ProjDirPath}/../../../../../components/serial_manager/serial_port_uart.h"
"${ProjDirPath}/../../../../../components/serial_manager/serial_port_uart.c"
"${ProjDirPath}/../../../../../devices/MCIMX7U5/drivers/fsl_lpuart.h"
"${ProjDirPath}/../../../../../devices/MCIMX7U5/drivers/fsl_lpuart.c"
"${ProjDirPath}/../../../../../components/lists/generic_list.h"
"${ProjDirPath}/../../../../../components/lists/generic_list.c"
"${ProjDirPath}/../../../../../devices/MCIMX7U5/gcc/startup_MCIMX7U5_cm4.S"
"${ProjDirPath}/../../../../../devices/MCIMX7U5/drivers/fsl_msmc.h"
"${ProjDirPath}/../../../../../devices/MCIMX7U5/drivers/fsl_msmc.c"
"${ProjDirPath}/../../../../../devices/MCIMX7U5/utilities/fsl_assert.c"
"${ProjDirPath}/../../../../../devices/MCIMX7U5/drivers/fsl_dmamux.h"
"${ProjDirPath}/../../../../../devices/MCIMX7U5/drivers/fsl_dmamux.c"
"${ProjDirPath}/../../../../../CMSIS/Include/core_cm4.h"
"${ProjDirPath}/../../../../../CMSIS/Include/cmsis_armcc.h"
"${ProjDirPath}/../../../../../CMSIS/Include/cmsis_armclang.h"
"${ProjDirPath}/../../../../../CMSIS/Include/cmsis_gcc.h"
"${ProjDirPath}/../../../../../CMSIS/Include/cmsis_compiler.h"
"${ProjDirPath}/../../../../../CMSIS/Include/cmsis_version.h"
"${ProjDirPath}/../../../../../CMSIS/Include/mpu_armv7.h"
"${ProjDirPath}/../../../../../CMSIS/Include/core_armv8mbl.h"
"${ProjDirPath}/../../../../../CMSIS/Include/core_armv8mml.h"
"${ProjDirPath}/../../../../../CMSIS/Include/arm_common_tables.h"
"${ProjDirPath}/../../../../../CMSIS/Include/arm_const_structs.h"
"${ProjDirPath}/../../../../../CMSIS/Include/arm_math.h"
"${ProjDirPath}/../../../../../CMSIS/Include/cmsis_iccarm.h"
"${ProjDirPath}/../../../../../CMSIS/Include/mpu_armv8.h"
"${ProjDirPath}/../../../../../devices/MCIMX7U5/drivers/fsl_edma.h"
"${ProjDirPath}/../../../../../devices/MCIMX7U5/drivers/fsl_edma.c"
"${ProjDirPath}/../../../../../devices/MCIMX7U5/drivers/fsl_lpi2c.c"
"${ProjDirPath}/../../../../../devices/MCIMX7U5/drivers/fsl_lpi2c.h"
"${ProjDirPath}/../../../../../devices/MCIMX7U5/drivers/fsl_lpi2c_freertos.h"
"${ProjDirPath}/../../../../../devices/MCIMX7U5/drivers/fsl_lpi2c_freertos.c"
"${ProjDirPath}/../../../../../devices/MCIMX7U5/drivers/fsl_pmc0.h"
"${ProjDirPath}/../../../../../devices/MCIMX7U5/drivers/fsl_llwu.h"
"${ProjDirPath}/../../../../../devices/MCIMX7U5/drivers/fsl_llwu.c"
"${ProjDirPath}/../../../../../devices/MCIMX7U5/drivers/fsl_snvs_lp.c"
"${ProjDirPath}/../../../../../devices/MCIMX7U5/drivers/fsl_snvs_lp.h"
"${ProjDirPath}/../../../../../components/pf1550/fsl_pf1550.h"
"${ProjDirPath}/../../../../../components/pf1550/fsl_pf1550.c"
"${ProjDirPath}/../../../../../components/pf1550/fsl_pf1550_charger.h"
"${ProjDirPath}/../../../../../components/pf1550/fsl_pf1550_charger.c"
"${ProjDirPath}/../../../../../devices/MCIMX7U5/drivers/fsl_sai.h"
"${ProjDirPath}/../../../../../devices/MCIMX7U5/drivers/fsl_sai.c"
"${ProjDirPath}/../../../../../devices/MCIMX7U5/drivers/fsl_sai_edma.h"
"${ProjDirPath}/../../../../../devices/MCIMX7U5/drivers/fsl_sai_edma.c"
"${ProjDirPath}/../../../../../components/codec/wm8960/fsl_wm8960.h"
"${ProjDirPath}/../../../../../components/codec/wm8960/fsl_wm8960.c"
"${ProjDirPath}/../../../../../components/codec/fsl_codec_common.h"
"${ProjDirPath}/../../../../../components/codec/fsl_codec_common.c"
"${ProjDirPath}/../../../../../components/srtm/port/srtm_heap_freertos.c"
"${ProjDirPath}/../../../../../components/srtm/port/srtm_mutex_freertos.c"
"${ProjDirPath}/../../../../../components/srtm/port/srtm_sem_freertos.c"
"${ProjDirPath}/../../../../../components/srtm/include/srtm_channel.h"
"${ProjDirPath}/../../../../../components/srtm/include/srtm_defs.h"
"${ProjDirPath}/../../../../../components/srtm/include/srtm_dispatcher.h"
"${ProjDirPath}/../../../../../components/srtm/include/srtm_message.h"
"${ProjDirPath}/../../../../../components/srtm/include/srtm_peercore.h"
"${ProjDirPath}/../../../../../components/srtm/include/srtm_service.h"
"${ProjDirPath}/../../../../../components/srtm/srtm/srtm_channel.c"
"${ProjDirPath}/../../../../../components/srtm/srtm/srtm_channel_struct.h"
"${ProjDirPath}/../../../../../components/srtm/srtm/srtm_dispatcher.c"
"${ProjDirPath}/../../../../../components/srtm/srtm/srtm_dispatcher_struct.h"
"${ProjDirPath}/../../../../../components/srtm/srtm/srtm_message.c"
"${ProjDirPath}/../../../../../components/srtm/srtm/srtm_message_struct.h"
"${ProjDirPath}/../../../../../components/srtm/srtm/srtm_peercore.c"
"${ProjDirPath}/../../../../../components/srtm/srtm/srtm_peercore_struct.h"
"${ProjDirPath}/../../../../../components/srtm/srtm/srtm_service.c"
"${ProjDirPath}/../../../../../components/srtm/srtm/srtm_service_struct.h"
"${ProjDirPath}/../../../../../components/srtm/srtm/srtm_heap.h"
"${ProjDirPath}/../../../../../components/srtm/srtm/srtm_message_pool.h"
"${ProjDirPath}/../../../../../components/srtm/srtm/srtm_list.h"
"${ProjDirPath}/../../../../../components/srtm/srtm/srtm_mutex.h"
"${ProjDirPath}/../../../../../components/srtm/srtm/srtm_sem.h"
"${ProjDirPath}/../../../../../components/srtm/port/srtm_message_pool.c"
"${ProjDirPath}/../../../../../middleware/multicore/rpmsg_lite/lib/include/platform/imx7ulp_m4/rpmsg_platform.h"
"${ProjDirPath}/../../../../../middleware/multicore/rpmsg_lite/lib/rpmsg_lite/porting/platform/imx7ulp_m4/rpmsg_platform.c"
"${ProjDirPath}/../../../../../middleware/multicore/rpmsg_lite/lib/rpmsg_lite/porting/environment/rpmsg_env_freertos.c"
"${ProjDirPath}/../../../../../middleware/multicore/rpmsg_lite/lib/include/rpmsg_env.h"
"${ProjDirPath}/../../../../../middleware/multicore/rpmsg_lite/lib/include/rpmsg_queue.h"
"${ProjDirPath}/../../../../../middleware/multicore/rpmsg_lite/lib/rpmsg_lite/rpmsg_queue.c"
"${ProjDirPath}/../../../../../middleware/multicore/rpmsg_lite/lib/include/rpmsg_lite.h"
"${ProjDirPath}/../../../../../middleware/multicore/rpmsg_lite/lib/include/rpmsg_ns.h"
"${ProjDirPath}/../../../../../middleware/multicore/rpmsg_lite/lib/include/llist.h"
"${ProjDirPath}/../../../../../middleware/multicore/rpmsg_lite/lib/include/rpmsg_default_config.h"
"${ProjDirPath}/../../../../../middleware/multicore/rpmsg_lite/lib/include/virtio_ring.h"
"${ProjDirPath}/../../../../../middleware/multicore/rpmsg_lite/lib/include/virtqueue.h"
"${ProjDirPath}/../../../../../middleware/multicore/rpmsg_lite/lib/include/rpmsg_compiler.h"
"${ProjDirPath}/../../../../../middleware/multicore/rpmsg_lite/lib/common/llist.c"
"${ProjDirPath}/../../../../../middleware/multicore/rpmsg_lite/lib/rpmsg_lite/rpmsg_lite.c"
"${ProjDirPath}/../../../../../middleware/multicore/rpmsg_lite/lib/rpmsg_lite/rpmsg_ns.c"
"${ProjDirPath}/../../../../../middleware/multicore/rpmsg_lite/lib/virtio/virtqueue.c"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/FreeRTOS/portable/MemMang/heap_4.c"
"${ProjDirPath}/../../../../../middleware/issdk/sensors/fxas21002.h"
"${ProjDirPath}/../../../../../middleware/issdk/sensors/fxos8700.h"
"${ProjDirPath}/../../../../../middleware/issdk/sensors/mpl3115.h"
"${ProjDirPath}/../../../../../middleware/issdk/sensors/fxlc95000.h"
"${ProjDirPath}/../../../../../middleware/issdk/sensors/fxls8471q.h"
"${ProjDirPath}/../../../../../middleware/issdk/sensors/fxls8962.h"
"${ProjDirPath}/../../../../../middleware/issdk/sensors/fxpq3115.h"
"${ProjDirPath}/../../../../../middleware/issdk/sensors/mag3110.h"
"${ProjDirPath}/../../../../../middleware/issdk/sensors/mma845x.h"
"${ProjDirPath}/../../../../../middleware/issdk/sensors/mma865x.h"
"${ProjDirPath}/../../../../../middleware/issdk/sensors/mma8491q.h"
"${ProjDirPath}/../../../../../middleware/issdk/sensors/mma9553.h"
"${ProjDirPath}/../../../../../middleware/issdk/sensors/diff_p.h"
"${ProjDirPath}/../../../../../CMSIS/Driver/Include/Driver_I2C.h"
"${ProjDirPath}/../../../../../CMSIS/Driver/Include/Driver_Common.h"
"${ProjDirPath}/../../../../../CMSIS/Driver/Include/Driver_SPI.h"
"${ProjDirPath}/../../../../../middleware/issdk/algorithms/pedometer/source/pedometer.c"
"${ProjDirPath}/../../../../../middleware/issdk/algorithms/pedometer/include/pedometer.h"
"${ProjDirPath}/../../../../../middleware/issdk/algorithms/pedometer/include/libinclude/KeynetikPedometer.h"
"${ProjDirPath}/../../../../../middleware/multicore/erpc/erpc_c/infra/erpc_version.h"
"${ProjDirPath}/../../../../../middleware/multicore/erpc/erpc_c/infra/erpc_codec.h"
"${ProjDirPath}/../../../../../middleware/multicore/erpc/erpc_c/infra/erpc_basic_codec.h"
"${ProjDirPath}/../../../../../middleware/multicore/erpc/erpc_c/infra/erpc_transport.h"
"${ProjDirPath}/../../../../../middleware/multicore/erpc/erpc_c/infra/erpc_message_buffer.h"
"${ProjDirPath}/../../../../../middleware/multicore/erpc/erpc_c/infra/erpc_message_loggers.h"
"${ProjDirPath}/../../../../../middleware/multicore/erpc/erpc_c/infra/erpc_common.h"
"${ProjDirPath}/../../../../../middleware/multicore/erpc/erpc_c/setup/erpc_transport_setup.h"
"${ProjDirPath}/../../../../../middleware/multicore/erpc/erpc_c/setup/erpc_mbf_setup.h"
"${ProjDirPath}/../../../../../middleware/multicore/erpc/erpc_c/infra/erpc_manually_constructed.h"
"${ProjDirPath}/../../../../../middleware/multicore/erpc/erpc_c/infra/erpc_crc16.h"
"${ProjDirPath}/../../../../../middleware/multicore/erpc/erpc_c/infra/erpc_basic_codec.cpp"
"${ProjDirPath}/../../../../../middleware/multicore/erpc/erpc_c/infra/erpc_message_buffer.cpp"
"${ProjDirPath}/../../../../../middleware/multicore/erpc/erpc_c/infra/erpc_message_loggers.cpp"
"${ProjDirPath}/../../../../../middleware/multicore/erpc/erpc_c/infra/erpc_crc16.cpp"
)


set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -T${ProjDirPath}/MCIMX7U5xxxxx_cm4_ram.ld -static")

set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -T${ProjDirPath}/MCIMX7U5xxxxx_cm4_ram.ld -static")

set(CMAKE_EXE_LINKER_FLAGS_FLASH_DEBUG "${CMAKE_EXE_LINKER_FLAGS_FLASH_DEBUG} -T${ProjDirPath}/MCIMX7U5xxxxx_cm4_flash.ld -static")

set(CMAKE_EXE_LINKER_FLAGS_FLASH_RELEASE "${CMAKE_EXE_LINKER_FLAGS_FLASH_RELEASE} -T${ProjDirPath}/MCIMX7U5xxxxx_cm4_flash.ld -static")

TARGET_LINK_LIBRARIES(erpc_matrix_multiply_rpmsg_rtos_imxcm4.elf -Wl,--start-group)
target_link_libraries(erpc_matrix_multiply_rpmsg_rtos_imxcm4.elf debug g)

target_link_libraries(erpc_matrix_multiply_rpmsg_rtos_imxcm4.elf debug stdc++)

target_link_libraries(erpc_matrix_multiply_rpmsg_rtos_imxcm4.elf debug m)

target_link_libraries(erpc_matrix_multiply_rpmsg_rtos_imxcm4.elf debug c)

target_link_libraries(erpc_matrix_multiply_rpmsg_rtos_imxcm4.elf debug gcc)

target_link_libraries(erpc_matrix_multiply_rpmsg_rtos_imxcm4.elf debug nosys)

target_link_libraries(erpc_matrix_multiply_rpmsg_rtos_imxcm4.elf optimized g)

target_link_libraries(erpc_matrix_multiply_rpmsg_rtos_imxcm4.elf optimized stdc++)

target_link_libraries(erpc_matrix_multiply_rpmsg_rtos_imxcm4.elf optimized m)

target_link_libraries(erpc_matrix_multiply_rpmsg_rtos_imxcm4.elf optimized c)

target_link_libraries(erpc_matrix_multiply_rpmsg_rtos_imxcm4.elf optimized gcc)

target_link_libraries(erpc_matrix_multiply_rpmsg_rtos_imxcm4.elf optimized nosys)

target_link_libraries(erpc_matrix_multiply_rpmsg_rtos_imxcm4.elf optimized g)

target_link_libraries(erpc_matrix_multiply_rpmsg_rtos_imxcm4.elf optimized stdc++)

target_link_libraries(erpc_matrix_multiply_rpmsg_rtos_imxcm4.elf optimized m)

target_link_libraries(erpc_matrix_multiply_rpmsg_rtos_imxcm4.elf optimized c)

target_link_libraries(erpc_matrix_multiply_rpmsg_rtos_imxcm4.elf optimized gcc)

target_link_libraries(erpc_matrix_multiply_rpmsg_rtos_imxcm4.elf optimized nosys)

target_link_libraries(erpc_matrix_multiply_rpmsg_rtos_imxcm4.elf optimized g)

target_link_libraries(erpc_matrix_multiply_rpmsg_rtos_imxcm4.elf optimized stdc++)

target_link_libraries(erpc_matrix_multiply_rpmsg_rtos_imxcm4.elf optimized m)

target_link_libraries(erpc_matrix_multiply_rpmsg_rtos_imxcm4.elf optimized c)

target_link_libraries(erpc_matrix_multiply_rpmsg_rtos_imxcm4.elf optimized gcc)

target_link_libraries(erpc_matrix_multiply_rpmsg_rtos_imxcm4.elf optimized nosys)

link_directories(${ProjDirPath}/../../../../../middleware/issdk/algorithms/pedometer/lib)

target_link_libraries(erpc_matrix_multiply_rpmsg_rtos_imxcm4.elf debug ${ProjDirPath}/../../../../../middleware/issdk/algorithms/pedometer/lib/libpedometerm4.a)

link_directories(${ProjDirPath}/../../../../../middleware/issdk/algorithms/pedometer/lib)

target_link_libraries(erpc_matrix_multiply_rpmsg_rtos_imxcm4.elf optimized ${ProjDirPath}/../../../../../middleware/issdk/algorithms/pedometer/lib/libpedometerm4.a)

link_directories(${ProjDirPath}/../../../../../middleware/issdk/algorithms/pedometer/lib)

target_link_libraries(erpc_matrix_multiply_rpmsg_rtos_imxcm4.elf optimized ${ProjDirPath}/../../../../../middleware/issdk/algorithms/pedometer/lib/libpedometerm4.a)

link_directories(${ProjDirPath}/../../../../../middleware/issdk/algorithms/pedometer/lib)

target_link_libraries(erpc_matrix_multiply_rpmsg_rtos_imxcm4.elf optimized ${ProjDirPath}/../../../../../middleware/issdk/algorithms/pedometer/lib/libpedometerm4.a)

TARGET_LINK_LIBRARIES(erpc_matrix_multiply_rpmsg_rtos_imxcm4.elf -Wl,--end-group)

ADD_CUSTOM_COMMAND(TARGET erpc_matrix_multiply_rpmsg_rtos_imxcm4.elf POST_BUILD COMMAND ${CMAKE_OBJCOPY}
-Obinary ${EXECUTABLE_OUTPUT_PATH}/erpc_matrix_multiply_rpmsg_rtos_imxcm4.elf ${EXECUTABLE_OUTPUT_PATH}/sdk20-app.bin)
