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


SET(CMAKE_ASM_FLAGS_DDR_DEBUG "${CMAKE_ASM_FLAGS_DDR_DEBUG} -DDEBUG")

SET(CMAKE_ASM_FLAGS_DDR_DEBUG "${CMAKE_ASM_FLAGS_DDR_DEBUG} -D__STARTUP_CLEAR_BSS")

SET(CMAKE_ASM_FLAGS_DDR_DEBUG "${CMAKE_ASM_FLAGS_DDR_DEBUG} -D__STARTUP_INITIALIZE_NONCACHEDATA")

SET(CMAKE_ASM_FLAGS_DDR_DEBUG "${CMAKE_ASM_FLAGS_DDR_DEBUG} -g")

SET(CMAKE_ASM_FLAGS_DDR_DEBUG "${CMAKE_ASM_FLAGS_DDR_DEBUG} -march=armv7-a")

SET(CMAKE_ASM_FLAGS_DDR_DEBUG "${CMAKE_ASM_FLAGS_DDR_DEBUG} -Wall")

SET(CMAKE_ASM_FLAGS_DDR_DEBUG "${CMAKE_ASM_FLAGS_DDR_DEBUG} -mfloat-abi=hard")

SET(CMAKE_ASM_FLAGS_DDR_DEBUG "${CMAKE_ASM_FLAGS_DDR_DEBUG} -mfpu=vfpv4")

SET(CMAKE_ASM_FLAGS_DDR_DEBUG "${CMAKE_ASM_FLAGS_DDR_DEBUG} -fno-common")

SET(CMAKE_ASM_FLAGS_DDR_DEBUG "${CMAKE_ASM_FLAGS_DDR_DEBUG} -ffunction-sections")

SET(CMAKE_ASM_FLAGS_DDR_DEBUG "${CMAKE_ASM_FLAGS_DDR_DEBUG} -fdata-sections")

SET(CMAKE_ASM_FLAGS_DDR_DEBUG "${CMAKE_ASM_FLAGS_DDR_DEBUG} -ffreestanding")

SET(CMAKE_ASM_FLAGS_DDR_DEBUG "${CMAKE_ASM_FLAGS_DDR_DEBUG} -fno-builtin")

SET(CMAKE_ASM_FLAGS_DDR_DEBUG "${CMAKE_ASM_FLAGS_DDR_DEBUG} -mthumb")

SET(CMAKE_ASM_FLAGS_DDR_DEBUG "${CMAKE_ASM_FLAGS_DDR_DEBUG} -mapcs")

SET(CMAKE_ASM_FLAGS_DDR_DEBUG "${CMAKE_ASM_FLAGS_DDR_DEBUG} -std=gnu99")

SET(CMAKE_ASM_FLAGS_DDR_RELEASE "${CMAKE_ASM_FLAGS_DDR_RELEASE} -D__STARTUP_CLEAR_BSS")

SET(CMAKE_ASM_FLAGS_DDR_RELEASE "${CMAKE_ASM_FLAGS_DDR_RELEASE} -D__STARTUP_INITIALIZE_NONCACHEDATA")

SET(CMAKE_ASM_FLAGS_DDR_RELEASE "${CMAKE_ASM_FLAGS_DDR_RELEASE} -march=armv7-a")

SET(CMAKE_ASM_FLAGS_DDR_RELEASE "${CMAKE_ASM_FLAGS_DDR_RELEASE} -Wall")

SET(CMAKE_ASM_FLAGS_DDR_RELEASE "${CMAKE_ASM_FLAGS_DDR_RELEASE} -mfloat-abi=hard")

SET(CMAKE_ASM_FLAGS_DDR_RELEASE "${CMAKE_ASM_FLAGS_DDR_RELEASE} -mfpu=vfpv4")

SET(CMAKE_ASM_FLAGS_DDR_RELEASE "${CMAKE_ASM_FLAGS_DDR_RELEASE} -fno-common")

SET(CMAKE_ASM_FLAGS_DDR_RELEASE "${CMAKE_ASM_FLAGS_DDR_RELEASE} -ffunction-sections")

SET(CMAKE_ASM_FLAGS_DDR_RELEASE "${CMAKE_ASM_FLAGS_DDR_RELEASE} -fdata-sections")

SET(CMAKE_ASM_FLAGS_DDR_RELEASE "${CMAKE_ASM_FLAGS_DDR_RELEASE} -ffreestanding")

SET(CMAKE_ASM_FLAGS_DDR_RELEASE "${CMAKE_ASM_FLAGS_DDR_RELEASE} -fno-builtin")

SET(CMAKE_ASM_FLAGS_DDR_RELEASE "${CMAKE_ASM_FLAGS_DDR_RELEASE} -mthumb")

SET(CMAKE_ASM_FLAGS_DDR_RELEASE "${CMAKE_ASM_FLAGS_DDR_RELEASE} -mapcs")

SET(CMAKE_ASM_FLAGS_DDR_RELEASE "${CMAKE_ASM_FLAGS_DDR_RELEASE} -std=gnu99")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -DDEBUG")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -D__STARTUP_CLEAR_BSS")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -D__STARTUP_INITIALIZE_NONCACHEDATA")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -g")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -march=armv7-a")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -Wall")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -mfloat-abi=hard")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -mfpu=vfpv4")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -fno-common")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -ffunction-sections")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -fdata-sections")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -ffreestanding")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -fno-builtin")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -mthumb")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -mapcs")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -std=gnu99")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -D__STARTUP_CLEAR_BSS")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -D__STARTUP_INITIALIZE_NONCACHEDATA")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -march=armv7-a")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -Wall")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -mfloat-abi=hard")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -mfpu=vfpv4")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -fno-common")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -ffunction-sections")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -fdata-sections")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -ffreestanding")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -fno-builtin")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -mthumb")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -mapcs")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -std=gnu99")

SET(CMAKE_C_FLAGS_DDR_DEBUG "${CMAKE_C_FLAGS_DDR_DEBUG} -DDEBUG")

SET(CMAKE_C_FLAGS_DDR_DEBUG "${CMAKE_C_FLAGS_DDR_DEBUG} -DCPU_MCIMX6G3DVM05")

SET(CMAKE_C_FLAGS_DDR_DEBUG "${CMAKE_C_FLAGS_DDR_DEBUG} -DFSL_FEATURE_PHYKSZ8081_USE_RMII50M_MODE")

SET(CMAKE_C_FLAGS_DDR_DEBUG "${CMAKE_C_FLAGS_DDR_DEBUG} -DFSL_SDK_ENABLE_DRIVER_CACHE_CONTROL=1")

SET(CMAKE_C_FLAGS_DDR_DEBUG "${CMAKE_C_FLAGS_DDR_DEBUG} -DUSE_RTOS=1")

SET(CMAKE_C_FLAGS_DDR_DEBUG "${CMAKE_C_FLAGS_DDR_DEBUG} -DPRINTF_ADVANCED_ENABLE=1")

SET(CMAKE_C_FLAGS_DDR_DEBUG "${CMAKE_C_FLAGS_DDR_DEBUG} -DFSL_RTOS_FREE_RTOS")

SET(CMAKE_C_FLAGS_DDR_DEBUG "${CMAKE_C_FLAGS_DDR_DEBUG} -DEVK_MCIMX6UL")

SET(CMAKE_C_FLAGS_DDR_DEBUG "${CMAKE_C_FLAGS_DDR_DEBUG} -g")

SET(CMAKE_C_FLAGS_DDR_DEBUG "${CMAKE_C_FLAGS_DDR_DEBUG} -O0")

SET(CMAKE_C_FLAGS_DDR_DEBUG "${CMAKE_C_FLAGS_DDR_DEBUG} -mno-unaligned-access")

SET(CMAKE_C_FLAGS_DDR_DEBUG "${CMAKE_C_FLAGS_DDR_DEBUG} -march=armv7-a")

SET(CMAKE_C_FLAGS_DDR_DEBUG "${CMAKE_C_FLAGS_DDR_DEBUG} -Wall")

SET(CMAKE_C_FLAGS_DDR_DEBUG "${CMAKE_C_FLAGS_DDR_DEBUG} -mfloat-abi=hard")

SET(CMAKE_C_FLAGS_DDR_DEBUG "${CMAKE_C_FLAGS_DDR_DEBUG} -mfpu=vfpv4")

SET(CMAKE_C_FLAGS_DDR_DEBUG "${CMAKE_C_FLAGS_DDR_DEBUG} -MMD")

SET(CMAKE_C_FLAGS_DDR_DEBUG "${CMAKE_C_FLAGS_DDR_DEBUG} -MP")

SET(CMAKE_C_FLAGS_DDR_DEBUG "${CMAKE_C_FLAGS_DDR_DEBUG} -fno-common")

SET(CMAKE_C_FLAGS_DDR_DEBUG "${CMAKE_C_FLAGS_DDR_DEBUG} -ffunction-sections")

SET(CMAKE_C_FLAGS_DDR_DEBUG "${CMAKE_C_FLAGS_DDR_DEBUG} -fdata-sections")

SET(CMAKE_C_FLAGS_DDR_DEBUG "${CMAKE_C_FLAGS_DDR_DEBUG} -ffreestanding")

SET(CMAKE_C_FLAGS_DDR_DEBUG "${CMAKE_C_FLAGS_DDR_DEBUG} -fno-builtin")

SET(CMAKE_C_FLAGS_DDR_DEBUG "${CMAKE_C_FLAGS_DDR_DEBUG} -mthumb")

SET(CMAKE_C_FLAGS_DDR_DEBUG "${CMAKE_C_FLAGS_DDR_DEBUG} -mapcs")

SET(CMAKE_C_FLAGS_DDR_DEBUG "${CMAKE_C_FLAGS_DDR_DEBUG} -std=gnu99")

SET(CMAKE_C_FLAGS_DDR_RELEASE "${CMAKE_C_FLAGS_DDR_RELEASE} -DNDEBUG")

SET(CMAKE_C_FLAGS_DDR_RELEASE "${CMAKE_C_FLAGS_DDR_RELEASE} -DCPU_MCIMX6G3DVM05")

SET(CMAKE_C_FLAGS_DDR_RELEASE "${CMAKE_C_FLAGS_DDR_RELEASE} -DFSL_FEATURE_PHYKSZ8081_USE_RMII50M_MODE")

SET(CMAKE_C_FLAGS_DDR_RELEASE "${CMAKE_C_FLAGS_DDR_RELEASE} -DFSL_SDK_ENABLE_DRIVER_CACHE_CONTROL=1")

SET(CMAKE_C_FLAGS_DDR_RELEASE "${CMAKE_C_FLAGS_DDR_RELEASE} -DUSE_RTOS=1")

SET(CMAKE_C_FLAGS_DDR_RELEASE "${CMAKE_C_FLAGS_DDR_RELEASE} -DPRINTF_ADVANCED_ENABLE=1")

SET(CMAKE_C_FLAGS_DDR_RELEASE "${CMAKE_C_FLAGS_DDR_RELEASE} -DFSL_RTOS_FREE_RTOS")

SET(CMAKE_C_FLAGS_DDR_RELEASE "${CMAKE_C_FLAGS_DDR_RELEASE} -DEVK_MCIMX6UL")

SET(CMAKE_C_FLAGS_DDR_RELEASE "${CMAKE_C_FLAGS_DDR_RELEASE} -Os")

SET(CMAKE_C_FLAGS_DDR_RELEASE "${CMAKE_C_FLAGS_DDR_RELEASE} -mno-unaligned-access")

SET(CMAKE_C_FLAGS_DDR_RELEASE "${CMAKE_C_FLAGS_DDR_RELEASE} -march=armv7-a")

SET(CMAKE_C_FLAGS_DDR_RELEASE "${CMAKE_C_FLAGS_DDR_RELEASE} -Wall")

SET(CMAKE_C_FLAGS_DDR_RELEASE "${CMAKE_C_FLAGS_DDR_RELEASE} -mfloat-abi=hard")

SET(CMAKE_C_FLAGS_DDR_RELEASE "${CMAKE_C_FLAGS_DDR_RELEASE} -mfpu=vfpv4")

SET(CMAKE_C_FLAGS_DDR_RELEASE "${CMAKE_C_FLAGS_DDR_RELEASE} -MMD")

SET(CMAKE_C_FLAGS_DDR_RELEASE "${CMAKE_C_FLAGS_DDR_RELEASE} -MP")

SET(CMAKE_C_FLAGS_DDR_RELEASE "${CMAKE_C_FLAGS_DDR_RELEASE} -fno-common")

SET(CMAKE_C_FLAGS_DDR_RELEASE "${CMAKE_C_FLAGS_DDR_RELEASE} -ffunction-sections")

SET(CMAKE_C_FLAGS_DDR_RELEASE "${CMAKE_C_FLAGS_DDR_RELEASE} -fdata-sections")

SET(CMAKE_C_FLAGS_DDR_RELEASE "${CMAKE_C_FLAGS_DDR_RELEASE} -ffreestanding")

SET(CMAKE_C_FLAGS_DDR_RELEASE "${CMAKE_C_FLAGS_DDR_RELEASE} -fno-builtin")

SET(CMAKE_C_FLAGS_DDR_RELEASE "${CMAKE_C_FLAGS_DDR_RELEASE} -mthumb")

SET(CMAKE_C_FLAGS_DDR_RELEASE "${CMAKE_C_FLAGS_DDR_RELEASE} -mapcs")

SET(CMAKE_C_FLAGS_DDR_RELEASE "${CMAKE_C_FLAGS_DDR_RELEASE} -std=gnu99")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DCPU_MCIMX6G3DVM05")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DFSL_FEATURE_PHYKSZ8081_USE_RMII50M_MODE")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DFSL_SDK_ENABLE_DRIVER_CACHE_CONTROL=1")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DUSE_RTOS=1")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DPRINTF_ADVANCED_ENABLE=1")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DFSL_RTOS_FREE_RTOS")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DEVK_MCIMX6UL")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -mno-unaligned-access")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -march=armv7-a")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wall")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -mfloat-abi=hard")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -mfpu=vfpv4")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -MMD")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -MP")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -fno-common")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -ffunction-sections")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -fdata-sections")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -ffreestanding")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -fno-builtin")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -mthumb")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -mapcs")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -std=gnu99")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DNDEBUG")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DCPU_MCIMX6G3DVM05")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DFSL_FEATURE_PHYKSZ8081_USE_RMII50M_MODE")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DFSL_SDK_ENABLE_DRIVER_CACHE_CONTROL=1")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DUSE_RTOS=1")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DPRINTF_ADVANCED_ENABLE=1")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DFSL_RTOS_FREE_RTOS")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DEVK_MCIMX6UL")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Os")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -mno-unaligned-access")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -march=armv7-a")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Wall")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -mfloat-abi=hard")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -mfpu=vfpv4")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -MMD")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -MP")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -fno-common")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -ffunction-sections")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -fdata-sections")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -ffreestanding")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -fno-builtin")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -mthumb")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -mapcs")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -std=gnu99")

SET(CMAKE_CXX_FLAGS_DDR_DEBUG "${CMAKE_CXX_FLAGS_DDR_DEBUG} -DDEBUG")

SET(CMAKE_CXX_FLAGS_DDR_DEBUG "${CMAKE_CXX_FLAGS_DDR_DEBUG} -g")

SET(CMAKE_CXX_FLAGS_DDR_DEBUG "${CMAKE_CXX_FLAGS_DDR_DEBUG} -O0")

SET(CMAKE_CXX_FLAGS_DDR_DEBUG "${CMAKE_CXX_FLAGS_DDR_DEBUG} -march=armv7-a")

SET(CMAKE_CXX_FLAGS_DDR_DEBUG "${CMAKE_CXX_FLAGS_DDR_DEBUG} -Wall")

SET(CMAKE_CXX_FLAGS_DDR_DEBUG "${CMAKE_CXX_FLAGS_DDR_DEBUG} -mfloat-abi=hard")

SET(CMAKE_CXX_FLAGS_DDR_DEBUG "${CMAKE_CXX_FLAGS_DDR_DEBUG} -mfpu=vfpv4")

SET(CMAKE_CXX_FLAGS_DDR_DEBUG "${CMAKE_CXX_FLAGS_DDR_DEBUG} -MMD")

SET(CMAKE_CXX_FLAGS_DDR_DEBUG "${CMAKE_CXX_FLAGS_DDR_DEBUG} -MP")

SET(CMAKE_CXX_FLAGS_DDR_DEBUG "${CMAKE_CXX_FLAGS_DDR_DEBUG} -fno-common")

SET(CMAKE_CXX_FLAGS_DDR_DEBUG "${CMAKE_CXX_FLAGS_DDR_DEBUG} -ffunction-sections")

SET(CMAKE_CXX_FLAGS_DDR_DEBUG "${CMAKE_CXX_FLAGS_DDR_DEBUG} -fdata-sections")

SET(CMAKE_CXX_FLAGS_DDR_DEBUG "${CMAKE_CXX_FLAGS_DDR_DEBUG} -ffreestanding")

SET(CMAKE_CXX_FLAGS_DDR_DEBUG "${CMAKE_CXX_FLAGS_DDR_DEBUG} -fno-builtin")

SET(CMAKE_CXX_FLAGS_DDR_DEBUG "${CMAKE_CXX_FLAGS_DDR_DEBUG} -mthumb")

SET(CMAKE_CXX_FLAGS_DDR_DEBUG "${CMAKE_CXX_FLAGS_DDR_DEBUG} -mapcs")

SET(CMAKE_CXX_FLAGS_DDR_DEBUG "${CMAKE_CXX_FLAGS_DDR_DEBUG} -fno-rtti")

SET(CMAKE_CXX_FLAGS_DDR_DEBUG "${CMAKE_CXX_FLAGS_DDR_DEBUG} -fno-exceptions")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -march=armv7-a")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -mfloat-abi=hard")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -mfpu=vfpv4")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -MMD")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -MP")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-common")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -ffunction-sections")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fdata-sections")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -ffreestanding")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-builtin")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -mthumb")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -mapcs")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-rtti")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-exceptions")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Os")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -march=armv7-a")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -mfloat-abi=hard")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -mfpu=vfpv4")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -MMD")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -MP")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fno-common")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -ffunction-sections")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fdata-sections")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -ffreestanding")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fno-builtin")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -mthumb")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -mapcs")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fno-rtti")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fno-exceptions")

SET(CMAKE_CXX_FLAGS_DDR_RELEASE "${CMAKE_CXX_FLAGS_DDR_RELEASE} -Os")

SET(CMAKE_CXX_FLAGS_DDR_RELEASE "${CMAKE_CXX_FLAGS_DDR_RELEASE} -march=armv7-a")

SET(CMAKE_CXX_FLAGS_DDR_RELEASE "${CMAKE_CXX_FLAGS_DDR_RELEASE} -Wall")

SET(CMAKE_CXX_FLAGS_DDR_RELEASE "${CMAKE_CXX_FLAGS_DDR_RELEASE} -mfloat-abi=hard")

SET(CMAKE_CXX_FLAGS_DDR_RELEASE "${CMAKE_CXX_FLAGS_DDR_RELEASE} -mfpu=vfpv4")

SET(CMAKE_CXX_FLAGS_DDR_RELEASE "${CMAKE_CXX_FLAGS_DDR_RELEASE} -MMD")

SET(CMAKE_CXX_FLAGS_DDR_RELEASE "${CMAKE_CXX_FLAGS_DDR_RELEASE} -MP")

SET(CMAKE_CXX_FLAGS_DDR_RELEASE "${CMAKE_CXX_FLAGS_DDR_RELEASE} -fno-common")

SET(CMAKE_CXX_FLAGS_DDR_RELEASE "${CMAKE_CXX_FLAGS_DDR_RELEASE} -ffunction-sections")

SET(CMAKE_CXX_FLAGS_DDR_RELEASE "${CMAKE_CXX_FLAGS_DDR_RELEASE} -fdata-sections")

SET(CMAKE_CXX_FLAGS_DDR_RELEASE "${CMAKE_CXX_FLAGS_DDR_RELEASE} -ffreestanding")

SET(CMAKE_CXX_FLAGS_DDR_RELEASE "${CMAKE_CXX_FLAGS_DDR_RELEASE} -fno-builtin")

SET(CMAKE_CXX_FLAGS_DDR_RELEASE "${CMAKE_CXX_FLAGS_DDR_RELEASE} -mthumb")

SET(CMAKE_CXX_FLAGS_DDR_RELEASE "${CMAKE_CXX_FLAGS_DDR_RELEASE} -mapcs")

SET(CMAKE_CXX_FLAGS_DDR_RELEASE "${CMAKE_CXX_FLAGS_DDR_RELEASE} -fno-rtti")

SET(CMAKE_CXX_FLAGS_DDR_RELEASE "${CMAKE_CXX_FLAGS_DDR_RELEASE} -fno-exceptions")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -g")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} --defsym=IRQ_Handler=FreeRTOS_IRQ_Handler")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} --defsym=SVC_Handler=FreeRTOS_SWI_Handler")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -march=armv7-a")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -Wall")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -mfloat-abi=hard")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -mfpu=vfpv4")

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

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} --defsym=IRQ_Handler=FreeRTOS_IRQ_Handler")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} --defsym=SVC_Handler=FreeRTOS_SWI_Handler")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -march=armv7-a")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -Wall")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -mfloat-abi=hard")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -mfpu=vfpv4")

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

SET(CMAKE_EXE_LINKER_FLAGS_DDR_RELEASE "${CMAKE_EXE_LINKER_FLAGS_DDR_RELEASE} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_DDR_RELEASE "${CMAKE_EXE_LINKER_FLAGS_DDR_RELEASE} --defsym=IRQ_Handler=FreeRTOS_IRQ_Handler")

SET(CMAKE_EXE_LINKER_FLAGS_DDR_RELEASE "${CMAKE_EXE_LINKER_FLAGS_DDR_RELEASE} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_DDR_RELEASE "${CMAKE_EXE_LINKER_FLAGS_DDR_RELEASE} --defsym=SVC_Handler=FreeRTOS_SWI_Handler")

SET(CMAKE_EXE_LINKER_FLAGS_DDR_RELEASE "${CMAKE_EXE_LINKER_FLAGS_DDR_RELEASE} -march=armv7-a")

SET(CMAKE_EXE_LINKER_FLAGS_DDR_RELEASE "${CMAKE_EXE_LINKER_FLAGS_DDR_RELEASE} -Wall")

SET(CMAKE_EXE_LINKER_FLAGS_DDR_RELEASE "${CMAKE_EXE_LINKER_FLAGS_DDR_RELEASE} -mfloat-abi=hard")

SET(CMAKE_EXE_LINKER_FLAGS_DDR_RELEASE "${CMAKE_EXE_LINKER_FLAGS_DDR_RELEASE} -mfpu=vfpv4")

SET(CMAKE_EXE_LINKER_FLAGS_DDR_RELEASE "${CMAKE_EXE_LINKER_FLAGS_DDR_RELEASE} --specs=nano.specs")

SET(CMAKE_EXE_LINKER_FLAGS_DDR_RELEASE "${CMAKE_EXE_LINKER_FLAGS_DDR_RELEASE} --specs=nosys.specs")

SET(CMAKE_EXE_LINKER_FLAGS_DDR_RELEASE "${CMAKE_EXE_LINKER_FLAGS_DDR_RELEASE} -fno-common")

SET(CMAKE_EXE_LINKER_FLAGS_DDR_RELEASE "${CMAKE_EXE_LINKER_FLAGS_DDR_RELEASE} -ffunction-sections")

SET(CMAKE_EXE_LINKER_FLAGS_DDR_RELEASE "${CMAKE_EXE_LINKER_FLAGS_DDR_RELEASE} -fdata-sections")

SET(CMAKE_EXE_LINKER_FLAGS_DDR_RELEASE "${CMAKE_EXE_LINKER_FLAGS_DDR_RELEASE} -ffreestanding")

SET(CMAKE_EXE_LINKER_FLAGS_DDR_RELEASE "${CMAKE_EXE_LINKER_FLAGS_DDR_RELEASE} -fno-builtin")

SET(CMAKE_EXE_LINKER_FLAGS_DDR_RELEASE "${CMAKE_EXE_LINKER_FLAGS_DDR_RELEASE} -mthumb")

SET(CMAKE_EXE_LINKER_FLAGS_DDR_RELEASE "${CMAKE_EXE_LINKER_FLAGS_DDR_RELEASE} -mapcs")

SET(CMAKE_EXE_LINKER_FLAGS_DDR_RELEASE "${CMAKE_EXE_LINKER_FLAGS_DDR_RELEASE} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_DDR_RELEASE "${CMAKE_EXE_LINKER_FLAGS_DDR_RELEASE} --gc-sections")

SET(CMAKE_EXE_LINKER_FLAGS_DDR_RELEASE "${CMAKE_EXE_LINKER_FLAGS_DDR_RELEASE} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_DDR_RELEASE "${CMAKE_EXE_LINKER_FLAGS_DDR_RELEASE} -static")

SET(CMAKE_EXE_LINKER_FLAGS_DDR_RELEASE "${CMAKE_EXE_LINKER_FLAGS_DDR_RELEASE} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_DDR_RELEASE "${CMAKE_EXE_LINKER_FLAGS_DDR_RELEASE} -z")

SET(CMAKE_EXE_LINKER_FLAGS_DDR_RELEASE "${CMAKE_EXE_LINKER_FLAGS_DDR_RELEASE} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_DDR_RELEASE "${CMAKE_EXE_LINKER_FLAGS_DDR_RELEASE} muldefs")

SET(CMAKE_EXE_LINKER_FLAGS_DDR_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DDR_DEBUG} -g")

SET(CMAKE_EXE_LINKER_FLAGS_DDR_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DDR_DEBUG} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_DDR_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DDR_DEBUG} --defsym=IRQ_Handler=FreeRTOS_IRQ_Handler")

SET(CMAKE_EXE_LINKER_FLAGS_DDR_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DDR_DEBUG} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_DDR_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DDR_DEBUG} --defsym=SVC_Handler=FreeRTOS_SWI_Handler")

SET(CMAKE_EXE_LINKER_FLAGS_DDR_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DDR_DEBUG} -march=armv7-a")

SET(CMAKE_EXE_LINKER_FLAGS_DDR_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DDR_DEBUG} -Wall")

SET(CMAKE_EXE_LINKER_FLAGS_DDR_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DDR_DEBUG} -mfloat-abi=hard")

SET(CMAKE_EXE_LINKER_FLAGS_DDR_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DDR_DEBUG} -mfpu=vfpv4")

SET(CMAKE_EXE_LINKER_FLAGS_DDR_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DDR_DEBUG} --specs=nano.specs")

SET(CMAKE_EXE_LINKER_FLAGS_DDR_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DDR_DEBUG} --specs=nosys.specs")

SET(CMAKE_EXE_LINKER_FLAGS_DDR_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DDR_DEBUG} -fno-common")

SET(CMAKE_EXE_LINKER_FLAGS_DDR_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DDR_DEBUG} -ffunction-sections")

SET(CMAKE_EXE_LINKER_FLAGS_DDR_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DDR_DEBUG} -fdata-sections")

SET(CMAKE_EXE_LINKER_FLAGS_DDR_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DDR_DEBUG} -ffreestanding")

SET(CMAKE_EXE_LINKER_FLAGS_DDR_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DDR_DEBUG} -fno-builtin")

SET(CMAKE_EXE_LINKER_FLAGS_DDR_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DDR_DEBUG} -mthumb")

SET(CMAKE_EXE_LINKER_FLAGS_DDR_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DDR_DEBUG} -mapcs")

SET(CMAKE_EXE_LINKER_FLAGS_DDR_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DDR_DEBUG} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_DDR_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DDR_DEBUG} --gc-sections")

SET(CMAKE_EXE_LINKER_FLAGS_DDR_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DDR_DEBUG} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_DDR_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DDR_DEBUG} -static")

SET(CMAKE_EXE_LINKER_FLAGS_DDR_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DDR_DEBUG} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_DDR_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DDR_DEBUG} -z")

SET(CMAKE_EXE_LINKER_FLAGS_DDR_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DDR_DEBUG} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_DDR_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DDR_DEBUG} muldefs")

include_directories(${ProjDirPath}/..)

include_directories(${ProjDirPath}/../../../../..)

include_directories(${ProjDirPath}/../../../../../../../CMSIS/Include)

include_directories(${ProjDirPath}/../../../../../../../rtos/freertos_9.0.0/Source/portable/GCC/ARM_CA9)

include_directories(${ProjDirPath}/../../../../../../../rtos/freertos_9.0.0/Source/include)

include_directories(${ProjDirPath}/../../../../../../../platform/components/phyksz8081)

include_directories(${ProjDirPath}/../../../../../../../CORTEXA/Include)

include_directories(${ProjDirPath}/../../../../../../../devices)

include_directories(${ProjDirPath}/../../../../../../../rtos/freertos_9.0.0/Source)

include_directories(${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/port)

include_directories(${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src)

include_directories(${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/include)

include_directories(${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/contrib/apps)

include_directories(${ProjDirPath}/../../../../../../../devices/MCIMX6G3/utilities)

include_directories(${ProjDirPath}/../../../../../../../devices/MCIMX6G3/drivers)

include_directories(${ProjDirPath}/../../../../../../../platform/utilities/debug_console)

include_directories(${ProjDirPath}/../../../../../../../platform/utilities/debug_console/io)

include_directories(${ProjDirPath}/../../../../../../../platform/utilities/debug_console/str)

include_directories(${ProjDirPath}/../../../../../../../devices/MCIMX6G3)

add_executable(lwip_tcpecho_freertos.elf 
"${ProjDirPath}/../../../../../../../devices/MCIMX6G3/utilities/fsl_sbrk.c"
"${ProjDirPath}/../../../../../../../rtos/freertos_9.0.0/Source/portable/GCC/ARM_CA9/port.c"
"${ProjDirPath}/../../../../../../../rtos/freertos_9.0.0/Source/portable/GCC/ARM_CA9/portASM.S"
"${ProjDirPath}/../../../../../../../rtos/freertos_9.0.0/Source/portable/GCC/ARM_CA9/portmacro.h"
"${ProjDirPath}/../../../../../../../devices/MCIMX6G3/gcc/startup_MCIMX6G3.S"
"${ProjDirPath}/../fsl_phy.h"
"${ProjDirPath}/../fsl_phy.c"
"${ProjDirPath}/../../../../../../../devices/MCIMX6G3/drivers/fsl_cache.c"
"${ProjDirPath}/../../../../../../../devices/MCIMX6G3/drivers/fsl_cache.h"
"${ProjDirPath}/../../../../../../../devices/MCIMX6G3/drivers/fsl_uart.h"
"${ProjDirPath}/../../../../../../../devices/MCIMX6G3/drivers/fsl_uart.c"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/contrib/apps/tcpecho/tcpecho.h"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/contrib/apps/tcpecho/tcpecho.c"
"${ProjDirPath}/../../../../../../../devices/MCIMX6G3/drivers/fsl_clock.h"
"${ProjDirPath}/../../../../../../../devices/MCIMX6G3/drivers/fsl_clock.c"
"${ProjDirPath}/../../../../../../../devices/MCIMX6G3/drivers/fsl_enet.h"
"${ProjDirPath}/../../../../../../../devices/MCIMX6G3/drivers/fsl_enet.c"
"${ProjDirPath}/../../../../../../../rtos/freertos_9.0.0/Source/portable/MemMang/heap_3.c"
"${ProjDirPath}/../lwip_tcpecho_freertos.c"
"${ProjDirPath}/../FreeRTOSConfig.h"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../../../../../../../devices/MCIMX6G3/drivers/fsl_common.h"
"${ProjDirPath}/../../../../../../../devices/MCIMX6G3/drivers/fsl_common.c"
"${ProjDirPath}/../../../../../../../rtos/freertos_9.0.0/Source/include/croutine.h"
"${ProjDirPath}/../../../../../../../rtos/freertos_9.0.0/Source/include/event_groups.h"
"${ProjDirPath}/../../../../../../../rtos/freertos_9.0.0/Source/include/FreeRTOS.h"
"${ProjDirPath}/../../../../../../../rtos/freertos_9.0.0/Source/include/list.h"
"${ProjDirPath}/../../../../../../../rtos/freertos_9.0.0/Source/include/mpu_wrappers.h"
"${ProjDirPath}/../../../../../../../rtos/freertos_9.0.0/Source/include/portable.h"
"${ProjDirPath}/../../../../../../../rtos/freertos_9.0.0/Source/include/projdefs.h"
"${ProjDirPath}/../../../../../../../rtos/freertos_9.0.0/Source/include/queue.h"
"${ProjDirPath}/../../../../../../../rtos/freertos_9.0.0/Source/include/semphr.h"
"${ProjDirPath}/../../../../../../../rtos/freertos_9.0.0/Source/include/StackMacros.h"
"${ProjDirPath}/../../../../../../../rtos/freertos_9.0.0/Source/include/task.h"
"${ProjDirPath}/../../../../../../../rtos/freertos_9.0.0/Source/include/timers.h"
"${ProjDirPath}/../../../../../../../rtos/freertos_9.0.0/Source/include/deprecated_definitions.h"
"${ProjDirPath}/../../../../../../../rtos/freertos_9.0.0/Source/croutine.c"
"${ProjDirPath}/../../../../../../../rtos/freertos_9.0.0/Source/list.c"
"${ProjDirPath}/../../../../../../../rtos/freertos_9.0.0/Source/queue.c"
"${ProjDirPath}/../../../../../../../rtos/freertos_9.0.0/Source/tasks.c"
"${ProjDirPath}/../../../../../../../rtos/freertos_9.0.0/Source/timers.c"
"${ProjDirPath}/../../../../../../../rtos/freertos_9.0.0/Source/event_groups.c"
"${ProjDirPath}/../../../../../../../devices/MCIMX6G3/utilities/fsl_debug_console.c"
"${ProjDirPath}/../../../../../../../devices/MCIMX6G3/utilities/fsl_debug_console.h"
"${ProjDirPath}/../../../../../../../devices/MCIMX6G3/utilities/fsl_debug_console_conf.h"
"${ProjDirPath}/../../../../../../../devices/MCIMX6G3/utilities/fsl_io.c"
"${ProjDirPath}/../../../../../../../devices/MCIMX6G3/utilities/fsl_io.h"
"${ProjDirPath}/../../../../../../../devices/MCIMX6G3/utilities/fsl_log.c"
"${ProjDirPath}/../../../../../../../devices/MCIMX6G3/utilities/fsl_log.h"
"${ProjDirPath}/../../../../../../../devices/MCIMX6G3/utilities/fsl_str.c"
"${ProjDirPath}/../../../../../../../devices/MCIMX6G3/utilities/fsl_str.h"
"${ProjDirPath}/../../../../../../../devices/MCIMX6G3/system_MCIMX6G3.c"
"${ProjDirPath}/../../../../../../../devices/MCIMX6G3/system_MCIMX6G3.h"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/port/ethernetif.c"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/port/ethernetif.h"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/port/lwipopts.h"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/port/sys_arch.c"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/port/arch/cc.h"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/port/arch/perf.h"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/port/arch/sys_arch.h"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/api/api_lib.c"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/api/api_msg.c"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/api/err.c"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/api/netbuf.c"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/api/netdb.c"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/api/netifapi.c"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/api/sockets.c"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/api/tcpip.c"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/core/def.c"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/core/dns.c"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/core/inet_chksum.c"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/core/init.c"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/core/ip.c"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/core/mem.c"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/core/memp.c"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/core/netif.c"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/core/pbuf.c"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/core/raw.c"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/core/stats.c"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/core/sys.c"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/core/tcp.c"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/core/tcp_in.c"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/core/tcp_out.c"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/core/timeouts.c"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/core/udp.c"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/core/ipv4/autoip.c"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/core/ipv4/dhcp.c"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/core/ipv4/etharp.c"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/core/ipv4/icmp.c"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/core/ipv4/igmp.c"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/core/ipv4/ip4.c"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/core/ipv4/ip4_addr.c"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/core/ipv4/ip4_frag.c"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/core/ipv6/dhcp6.c"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/core/ipv6/ethip6.c"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/core/ipv6/icmp6.c"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/core/ipv6/inet6.c"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/core/ipv6/ip6.c"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/core/ipv6/ip6_addr.c"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/core/ipv6/ip6_frag.c"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/core/ipv6/mld6.c"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/core/ipv6/nd6.c"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/include/lwip/api.h"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/include/lwip/arch.h"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/include/lwip/autoip.h"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/include/lwip/debug.h"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/include/lwip/def.h"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/include/lwip/dhcp.h"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/include/lwip/dhcp6.h"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/include/lwip/dns.h"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/include/lwip/err.h"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/include/lwip/errno.h"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/include/lwip/etharp.h"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/include/lwip/ethip6.h"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/include/lwip/icmp.h"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/include/lwip/icmp6.h"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/include/lwip/igmp.h"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/include/lwip/inet.h"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/include/lwip/inet_chksum.h"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/include/lwip/init.h"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/include/lwip/ip.h"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/include/lwip/ip_addr.h"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/include/lwip/ip4.h"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/include/lwip/ip4_addr.h"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/include/lwip/ip4_frag.h"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/include/lwip/ip6.h"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/include/lwip/ip6_addr.h"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/include/lwip/ip6_frag.h"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/include/lwip/mem.h"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/include/lwip/memp.h"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/include/lwip/mld6.h"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/include/lwip/nd6.h"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/include/lwip/netbuf.h"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/include/lwip/netdb.h"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/include/lwip/netif.h"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/include/lwip/netifapi.h"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/include/lwip/opt.h"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/include/lwip/pbuf.h"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/include/lwip/raw.h"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/include/lwip/sio.h"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/include/lwip/snmp.h"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/include/lwip/sockets.h"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/include/lwip/stats.h"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/include/lwip/sys.h"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/include/lwip/tcp.h"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/include/lwip/tcpip.h"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/include/lwip/timeouts.h"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/include/lwip/udp.h"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/include/lwip/priv/api_msg.h"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/include/lwip/priv/memp_priv.h"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/include/lwip/priv/memp_std.h"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/include/lwip/priv/nd6_priv.h"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/include/lwip/priv/tcp_priv.h"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/include/lwip/priv/tcpip_priv.h"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/include/lwip/prot/autoip.h"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/include/lwip/prot/dhcp.h"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/include/lwip/prot/dns.h"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/include/lwip/prot/etharp.h"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/include/lwip/prot/ethernet.h"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/include/lwip/prot/icmp.h"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/include/lwip/prot/icmp6.h"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/include/lwip/prot/igmp.h"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/include/lwip/prot/ip.h"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/include/lwip/prot/ip4.h"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/include/lwip/prot/ip6.h"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/include/lwip/prot/mld6.h"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/include/lwip/prot/nd6.h"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/include/lwip/prot/tcp.h"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/include/lwip/prot/udp.h"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/include/netif/etharp.h"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/include/netif/ethernet.h"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/include/netif/lowpan6.h"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/include/netif/lowpan6_opts.h"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/include/netif/slipif.h"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/include/netif/ppp/pppoe.h"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/include/posix/errno.h"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/include/posix/netdb.h"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/include/posix/sys/socket.h"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/netif/ethernet.c"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/netif/lowpan6.c"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/netif/slipif.c"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/netif/ppp/auth.c"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/netif/ppp/ccp.c"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/netif/ppp/chap-md5.c"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/netif/ppp/chap-new.c"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/netif/ppp/chap_ms.c"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/netif/ppp/demand.c"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/netif/ppp/eap.c"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/netif/ppp/eui64.c"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/netif/ppp/fsm.c"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/netif/ppp/ipcp.c"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/netif/ppp/ipv6cp.c"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/netif/ppp/lcp.c"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/netif/ppp/lwip_ecp.c"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/netif/ppp/magic.c"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/netif/ppp/mppe.c"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/netif/ppp/multilink.c"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/netif/ppp/ppp.c"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/netif/ppp/pppapi.c"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/netif/ppp/pppcrypt.c"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/netif/ppp/pppoe.c"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/netif/ppp/pppol2tp.c"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/netif/ppp/pppos.c"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/netif/ppp/upap.c"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/netif/ppp/utils.c"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/netif/ppp/vj.c"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/include/netif/ppp/ccp.h"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/include/netif/ppp/chap-md5.h"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/include/netif/ppp/chap-new.h"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/include/netif/ppp/chap_ms.h"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/include/netif/ppp/eap.h"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/include/netif/ppp/ecp.h"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/include/netif/ppp/eui64.h"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/include/netif/ppp/fsm.h"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/include/netif/ppp/ipcp.h"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/include/netif/ppp/ipv6cp.h"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/include/netif/ppp/lcp.h"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/include/netif/ppp/magic.h"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/include/netif/ppp/mppe.h"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/include/netif/ppp/ppp.h"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/include/netif/ppp/ppp_impl.h"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/include/netif/ppp/ppp_opts.h"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/include/netif/ppp/pppapi.h"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/include/netif/ppp/pppcrypt.h"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/include/netif/ppp/pppdebug.h"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/include/netif/ppp/pppoe.h"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/include/netif/ppp/pppol2tp.h"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/include/netif/ppp/pppos.h"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/include/netif/ppp/upap.h"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/include/netif/ppp/vj.h"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/include/netif/ppp/polarssl/arc4.h"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/include/netif/ppp/polarssl/des.h"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/include/netif/ppp/polarssl/md4.h"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/include/netif/ppp/polarssl/md5.h"
"${ProjDirPath}/../../../../../../../middleware/lwip_2.0.2/src/include/netif/ppp/polarssl/sha1.h"
"${ProjDirPath}/../clock_config_ocram.c"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
"${ProjDirPath}/../../../../../../../devices/MCIMX6G3/drivers/fsl_iomuxc.h"
)


set(CMAKE_EXE_LINKER_FLAGS_DDR_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DDR_DEBUG} -T${ProjDirPath}/MCIMX6G3xxx05_ram.ld -static")

set(CMAKE_EXE_LINKER_FLAGS_DDR_RELEASE "${CMAKE_EXE_LINKER_FLAGS_DDR_RELEASE} -T${ProjDirPath}/MCIMX6G3xxx05_ram.ld -static")

set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -T${ProjDirPath}/MCIMX6G3xxx05_flash.ld -static")

set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -T${ProjDirPath}/MCIMX6G3xxx05_flash.ld -static")

TARGET_LINK_LIBRARIES(lwip_tcpecho_freertos.elf -Wl,--start-group)
target_link_libraries(lwip_tcpecho_freertos.elf optimized m)

target_link_libraries(lwip_tcpecho_freertos.elf optimized c)

target_link_libraries(lwip_tcpecho_freertos.elf optimized gcc)

target_link_libraries(lwip_tcpecho_freertos.elf optimized nosys)

target_link_libraries(lwip_tcpecho_freertos.elf optimized m)

target_link_libraries(lwip_tcpecho_freertos.elf optimized c)

target_link_libraries(lwip_tcpecho_freertos.elf optimized gcc)

target_link_libraries(lwip_tcpecho_freertos.elf optimized nosys)

target_link_libraries(lwip_tcpecho_freertos.elf debug m)

target_link_libraries(lwip_tcpecho_freertos.elf debug c)

target_link_libraries(lwip_tcpecho_freertos.elf debug gcc)

target_link_libraries(lwip_tcpecho_freertos.elf debug nosys)

target_link_libraries(lwip_tcpecho_freertos.elf optimized m)

target_link_libraries(lwip_tcpecho_freertos.elf optimized c)

target_link_libraries(lwip_tcpecho_freertos.elf optimized gcc)

target_link_libraries(lwip_tcpecho_freertos.elf optimized nosys)

TARGET_LINK_LIBRARIES(lwip_tcpecho_freertos.elf -Wl,--end-group)

ADD_CUSTOM_COMMAND(TARGET lwip_tcpecho_freertos.elf POST_BUILD COMMAND ${CMAKE_OBJCOPY}
-Obinary ${EXECUTABLE_OUTPUT_PATH}/lwip_tcpecho_freertos.elf ${EXECUTABLE_OUTPUT_PATH}/sdk20-app.bin)
