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

SET(CMAKE_ASM_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_ASM_FLAGS_FLEXSPI_NOR_DEBUG} -Wall")

SET(CMAKE_ASM_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_ASM_FLAGS_FLEXSPI_NOR_DEBUG} -fno-common")

SET(CMAKE_ASM_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_ASM_FLAGS_FLEXSPI_NOR_DEBUG} -ffunction-sections")

SET(CMAKE_ASM_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_ASM_FLAGS_FLEXSPI_NOR_DEBUG} -fdata-sections")

SET(CMAKE_ASM_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_ASM_FLAGS_FLEXSPI_NOR_DEBUG} -ffreestanding")

SET(CMAKE_ASM_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_ASM_FLAGS_FLEXSPI_NOR_DEBUG} -fno-builtin")

SET(CMAKE_ASM_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_ASM_FLAGS_FLEXSPI_NOR_DEBUG} -mthumb")

SET(CMAKE_ASM_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_ASM_FLAGS_FLEXSPI_NOR_DEBUG} -mapcs")

SET(CMAKE_ASM_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_ASM_FLAGS_FLEXSPI_NOR_DEBUG} -std=gnu99")

SET(CMAKE_ASM_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_ASM_FLAGS_FLEXSPI_NOR_DEBUG} -mcpu=cortex-m7")

SET(CMAKE_ASM_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_ASM_FLAGS_FLEXSPI_NOR_DEBUG} -mfloat-abi=hard")

SET(CMAKE_ASM_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_ASM_FLAGS_FLEXSPI_NOR_DEBUG} -mfpu=fpv5-sp-d16")

SET(CMAKE_ASM_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_ASM_FLAGS_FLEXSPI_NOR_RELEASE} -D__STARTUP_CLEAR_BSS")

SET(CMAKE_ASM_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_ASM_FLAGS_FLEXSPI_NOR_RELEASE} -DNDEBUG")

SET(CMAKE_ASM_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_ASM_FLAGS_FLEXSPI_NOR_RELEASE} -D__STARTUP_INITIALIZE_NONCACHEDATA")

SET(CMAKE_ASM_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_ASM_FLAGS_FLEXSPI_NOR_RELEASE} -Wall")

SET(CMAKE_ASM_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_ASM_FLAGS_FLEXSPI_NOR_RELEASE} -fno-common")

SET(CMAKE_ASM_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_ASM_FLAGS_FLEXSPI_NOR_RELEASE} -ffunction-sections")

SET(CMAKE_ASM_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_ASM_FLAGS_FLEXSPI_NOR_RELEASE} -fdata-sections")

SET(CMAKE_ASM_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_ASM_FLAGS_FLEXSPI_NOR_RELEASE} -ffreestanding")

SET(CMAKE_ASM_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_ASM_FLAGS_FLEXSPI_NOR_RELEASE} -fno-builtin")

SET(CMAKE_ASM_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_ASM_FLAGS_FLEXSPI_NOR_RELEASE} -mthumb")

SET(CMAKE_ASM_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_ASM_FLAGS_FLEXSPI_NOR_RELEASE} -mapcs")

SET(CMAKE_ASM_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_ASM_FLAGS_FLEXSPI_NOR_RELEASE} -std=gnu99")

SET(CMAKE_ASM_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_ASM_FLAGS_FLEXSPI_NOR_RELEASE} -mcpu=cortex-m7")

SET(CMAKE_ASM_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_ASM_FLAGS_FLEXSPI_NOR_RELEASE} -mfloat-abi=hard")

SET(CMAKE_ASM_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_ASM_FLAGS_FLEXSPI_NOR_RELEASE} -mfpu=fpv5-sp-d16")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG} -DXIP_EXTERNAL_FLASH=1")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG} -DXIP_BOOT_HEADER_ENABLE=1")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG} -DDEBUG")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG} -DCPU_MIMXRT1011DAE5A")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG} -DA_LITTLE_ENDIAN")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG} -DFSL_RTOS_FREE_RTOS")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG} -g")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG} -O0")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG} -Wall")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG} -fno-common")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG} -ffunction-sections")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG} -fdata-sections")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG} -ffreestanding")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG} -fno-builtin")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG} -mthumb")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG} -mapcs")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG} -std=gnu99")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG} -mcpu=cortex-m7")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG} -mfloat-abi=hard")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG} -mfpu=fpv5-sp-d16")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG} -MMD")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG} -MP")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE} -DXIP_EXTERNAL_FLASH=1")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE} -DXIP_BOOT_HEADER_ENABLE=1")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE} -DNDEBUG")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE} -DCPU_MIMXRT1011DAE5A")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE} -DA_LITTLE_ENDIAN")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE} -DFSL_RTOS_FREE_RTOS")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE} -Os")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE} -Wall")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE} -fno-common")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE} -ffunction-sections")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE} -fdata-sections")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE} -ffreestanding")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE} -fno-builtin")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE} -mthumb")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE} -mapcs")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE} -std=gnu99")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE} -mcpu=cortex-m7")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE} -mfloat-abi=hard")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE} -mfpu=fpv5-sp-d16")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE} -MMD")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE} -MP")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_DEBUG} -DDEBUG")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_DEBUG} -g")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_DEBUG} -O0")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_DEBUG} -Wall")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_DEBUG} -fno-common")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_DEBUG} -ffunction-sections")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_DEBUG} -fdata-sections")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_DEBUG} -ffreestanding")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_DEBUG} -fno-builtin")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_DEBUG} -mthumb")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_DEBUG} -mapcs")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_DEBUG} -fno-rtti")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_DEBUG} -fno-exceptions")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_DEBUG} -mcpu=cortex-m7")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_DEBUG} -mfloat-abi=hard")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_DEBUG} -mfpu=fpv5-sp-d16")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_DEBUG} -MMD")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_DEBUG} -MP")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_RELEASE} -DNDEBUG")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_RELEASE} -Os")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_RELEASE} -Wall")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_RELEASE} -fno-common")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_RELEASE} -ffunction-sections")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_RELEASE} -fdata-sections")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_RELEASE} -ffreestanding")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_RELEASE} -fno-builtin")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_RELEASE} -mthumb")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_RELEASE} -mapcs")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_RELEASE} -fno-rtti")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_RELEASE} -fno-exceptions")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_RELEASE} -mcpu=cortex-m7")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_RELEASE} -mfloat-abi=hard")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_RELEASE} -mfpu=fpv5-sp-d16")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_RELEASE} -MMD")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_RELEASE} -MP")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_DEBUG} -g")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_DEBUG} --specs=nano.specs")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_DEBUG} --specs=nosys.specs")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_DEBUG} -Wall")

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

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_DEBUG} -mcpu=cortex-m7")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_DEBUG} -mfloat-abi=hard")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_DEBUG} -mfpu=fpv5-sp-d16")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_RELEASE} --specs=nano.specs")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_RELEASE} --specs=nosys.specs")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_RELEASE} -Wall")

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

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_RELEASE} -mcpu=cortex-m7")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_RELEASE} -mfloat-abi=hard")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_RELEASE} -mfpu=fpv5-sp-d16")

include_directories(${ProjDirPath}/../../..)

include_directories(${ProjDirPath}/..)

include_directories(${ProjDirPath}/../../..)

include_directories(${ProjDirPath}/..)

include_directories(${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/FreeRTOS/portable/GCC/ARM_CM4F)

include_directories(${ProjDirPath}/../../..)

include_directories(${ProjDirPath}/..)

include_directories(${ProjDirPath}/../../../../../CMSIS/Include)

include_directories(${ProjDirPath}/../../../../../devices)

include_directories(${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include)

include_directories(${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include/private)

include_directories(${ProjDirPath}/../../../../../middleware/wifi_qca/port/boards/evkmimxrt1010/freertos/silex2401)

include_directories(${ProjDirPath}/../../../../../middleware/wifi_qca)

include_directories(${ProjDirPath}/../../../../../middleware/wifi_qca/include)

include_directories(${ProjDirPath}/../../../../../middleware/wifi_qca/include/AR6002)

include_directories(${ProjDirPath}/../../../../../middleware/wifi_qca/include/AR6002/hw2.0/hw)

include_directories(${ProjDirPath}/../../../../../middleware/wifi_qca/include/AR6002/hw4.0/hw)

include_directories(${ProjDirPath}/../../../../../middleware/wifi_qca/custom_src/stack_custom)

include_directories(${ProjDirPath}/../../../../../middleware/wifi_qca/custom_src/include)

include_directories(${ProjDirPath}/../../../../../middleware/wifi_qca/common_src/include)

include_directories(${ProjDirPath}/../../../../../middleware/wifi_qca/common_src/stack_common)

include_directories(${ProjDirPath}/../../../../../middleware/wifi_qca/common_src/hcd)

include_directories(${ProjDirPath}/../../../../../middleware/wifi_qca/common_src/wmi)

include_directories(${ProjDirPath}/../../../../../middleware/wifi_qca/port)

include_directories(${ProjDirPath}/../../../../../devices/MIMXRT1011/drivers)

include_directories(${ProjDirPath}/../../../../../devices/MIMXRT1011)

include_directories(${ProjDirPath}/../../../../../devices/MIMXRT1011/utilities/str)

include_directories(${ProjDirPath}/../../../../../devices/MIMXRT1011/utilities/debug_console)

include_directories(${ProjDirPath}/../../../../../components/uart)

include_directories(${ProjDirPath}/../../../../../components/serial_manager)

include_directories(${ProjDirPath}/../../../../../components/lists)

include_directories(${ProjDirPath}/../../../../../devices/MIMXRT1011/xip)

include_directories(${ProjDirPath}/../../../xip)

include_directories(${ProjDirPath}/../../../../../middleware/wifi_qca/port/boards/evkmimxrt1010/freertos)

include_directories(${ProjDirPath}/../../../../../middleware/wifi_qca/port/env/freertos)

include_directories(${ProjDirPath}/../../../../../middleware/wifi_qca/port/drivers/lpspi_edma_freertos)

add_executable(qca_demo.elf 
"${ProjDirPath}/../FreeRTOSConfig.h"
"${ProjDirPath}/../main.c"
"${ProjDirPath}/../wlan_qca400x.c"
"${ProjDirPath}/../wlan_qcom.c"
"${ProjDirPath}/../wlan_qcom.h"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
"${ProjDirPath}/../../../../../middleware/wifi_qca/port/boards/evkmimxrt1010/freertos/silex2401/wifi_shield_silex2401.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/FreeRTOS/portable/MemMang/heap_4.c"
"${ProjDirPath}/../../../../../middleware/wifi_qca/atheros_stack_offload.h"
"${ProjDirPath}/../../../../../middleware/wifi_qca/atheros_wifi.h"
"${ProjDirPath}/../../../../../middleware/wifi_qca/atheros_wifi_api.h"
"${ProjDirPath}/../../../../../middleware/wifi_qca/atheros_wifi_internal.h"
"${ProjDirPath}/../../../../../middleware/wifi_qca/common_src/api_interface/api_init.c"
"${ProjDirPath}/../../../../../middleware/wifi_qca/common_src/api_interface/api_ioctl.c"
"${ProjDirPath}/../../../../../middleware/wifi_qca/common_src/api_interface/api_txrx.c"
"${ProjDirPath}/../../../../../middleware/wifi_qca/common_src/api_interface/api_wmi_rx.c"
"${ProjDirPath}/../../../../../middleware/wifi_qca/common_src/bmi/bmi.c"
"${ProjDirPath}/../../../../../middleware/wifi_qca/common_src/driver/driver_diag.c"
"${ProjDirPath}/../../../../../middleware/wifi_qca/common_src/driver/driver_init.c"
"${ProjDirPath}/../../../../../middleware/wifi_qca/common_src/driver/driver_main.c"
"${ProjDirPath}/../../../../../middleware/wifi_qca/common_src/driver/driver_netbuf.c"
"${ProjDirPath}/../../../../../middleware/wifi_qca/common_src/driver/driver_txrx.c"
"${ProjDirPath}/../../../../../middleware/wifi_qca/common_src/hcd/hcd_api.h"
"${ProjDirPath}/../../../../../middleware/wifi_qca/common_src/hcd/spi_hcd.c"
"${ProjDirPath}/../../../../../middleware/wifi_qca/common_src/htc/htc.c"
"${ProjDirPath}/../../../../../middleware/wifi_qca/common_src/hw_interface/hw_api.c"
"${ProjDirPath}/../../../../../middleware/wifi_qca/common_src/include/a_debug.h"
"${ProjDirPath}/../../../../../middleware/wifi_qca/common_src/include/a_drv.h"
"${ProjDirPath}/../../../../../middleware/wifi_qca/common_src/include/a_drv_api.h"
"${ProjDirPath}/../../../../../middleware/wifi_qca/common_src/include/aggr_recv_api.h"
"${ProjDirPath}/../../../../../middleware/wifi_qca/common_src/include/bmi.h"
"${ProjDirPath}/../../../../../middleware/wifi_qca/common_src/include/common_api.h"
"${ProjDirPath}/../../../../../middleware/wifi_qca/common_src/include/dbglog_api.h"
"${ProjDirPath}/../../../../../middleware/wifi_qca/common_src/include/driver_cxt.h"
"${ProjDirPath}/../../../../../middleware/wifi_qca/common_src/include/dset.h"
"${ProjDirPath}/../../../../../middleware/wifi_qca/common_src/include/dset_api.h"
"${ProjDirPath}/../../../../../middleware/wifi_qca/common_src/include/hif_internal.h"
"${ProjDirPath}/../../../../../middleware/wifi_qca/common_src/include/htc_api.h"
"${ProjDirPath}/../../../../../middleware/wifi_qca/common_src/include/ieee80211.h"
"${ProjDirPath}/../../../../../middleware/wifi_qca/common_src/include/netbuf.h"
"${ProjDirPath}/../../../../../middleware/wifi_qca/common_src/include/spi_hcd_if.h"
"${ProjDirPath}/../../../../../middleware/wifi_qca/common_src/include/wlan_api.h"
"${ProjDirPath}/../../../../../middleware/wifi_qca/common_src/include/wmi_api.h"
"${ProjDirPath}/../../../../../middleware/wifi_qca/common_src/qapi/qcom_api.c"
"${ProjDirPath}/../../../../../middleware/wifi_qca/common_src/qapi/qcom_dns.c"
"${ProjDirPath}/../../../../../middleware/wifi_qca/common_src/qapi/qcom_legacy.c"
"${ProjDirPath}/../../../../../middleware/wifi_qca/common_src/reorder/rcv_aggr.c"
"${ProjDirPath}/../../../../../middleware/wifi_qca/common_src/stack_common/api_stack_offload.c"
"${ProjDirPath}/../../../../../middleware/wifi_qca/common_src/stack_common/common_stack_offload.h"
"${ProjDirPath}/../../../../../middleware/wifi_qca/common_src/storerecall/dset.c"
"${ProjDirPath}/../../../../../middleware/wifi_qca/common_src/storerecall/dset_api.c"
"${ProjDirPath}/../../../../../middleware/wifi_qca/common_src/storerecall/storerecall.c"
"${ProjDirPath}/../../../../../middleware/wifi_qca/common_src/util/util.c"
"${ProjDirPath}/../../../../../middleware/wifi_qca/common_src/wmi/wmi.c"
"${ProjDirPath}/../../../../../middleware/wifi_qca/common_src/wmi/wmi_host.h"
"${ProjDirPath}/../../../../../middleware/wifi_qca/custom_src/api_interface/cust_api_init.c"
"${ProjDirPath}/../../../../../middleware/wifi_qca/custom_src/api_interface/cust_api_ioctl.c"
"${ProjDirPath}/../../../../../middleware/wifi_qca/custom_src/api_interface/cust_api_txrx.c"
"${ProjDirPath}/../../../../../middleware/wifi_qca/custom_src/api_interface/cust_api_wmi_rx.c"
"${ProjDirPath}/../../../../../middleware/wifi_qca/custom_src/driver/cust_driver_main.c"
"${ProjDirPath}/../../../../../middleware/wifi_qca/custom_src/driver/cust_driver_netbuf.c"
"${ProjDirPath}/../../../../../middleware/wifi_qca/custom_src/hw_interface/cust_spi_hcd.c"
"${ProjDirPath}/../../../../../middleware/wifi_qca/custom_src/include/a_config.h"
"${ProjDirPath}/../../../../../middleware/wifi_qca/custom_src/include/a_osapi.h"
"${ProjDirPath}/../../../../../middleware/wifi_qca/custom_src/include/a_types.h"
"${ProjDirPath}/../../../../../middleware/wifi_qca/custom_src/include/cust_netbuf.h"
"${ProjDirPath}/../../../../../middleware/wifi_qca/custom_src/include/custom_wlan_api.h"
"${ProjDirPath}/../../../../../middleware/wifi_qca/custom_src/include/wlan_config.h"
"${ProjDirPath}/../../../../../middleware/wifi_qca/custom_src/qapi/custom_qcom_api.c"
"${ProjDirPath}/../../../../../middleware/wifi_qca/custom_src/stack_custom/cust_api_stack_offload.c"
"${ProjDirPath}/../../../../../middleware/wifi_qca/custom_src/stack_custom/cust_api_stack_txrx.c"
"${ProjDirPath}/../../../../../middleware/wifi_qca/custom_src/stack_custom/custom_stack_offload.h"
"${ProjDirPath}/../../../../../middleware/wifi_qca/custom_src/stack_custom/qca_structs.h"
"${ProjDirPath}/../../../../../middleware/wifi_qca/include/AR6002/AR6K_version.h"
"${ProjDirPath}/../../../../../middleware/wifi_qca/include/AR6002/hw2.0/hw/hw20_apb_map.h"
"${ProjDirPath}/../../../../../middleware/wifi_qca/include/AR6002/hw2.0/hw/hw20_gpio_reg.h"
"${ProjDirPath}/../../../../../middleware/wifi_qca/include/AR6002/hw2.0/hw/hw20_mbox_host_reg.h"
"${ProjDirPath}/../../../../../middleware/wifi_qca/include/AR6002/hw2.0/hw/hw20_mbox_reg.h"
"${ProjDirPath}/../../../../../middleware/wifi_qca/include/AR6002/hw2.0/hw/hw20_rtc_reg.h"
"${ProjDirPath}/../../../../../middleware/wifi_qca/include/AR6002/hw2.0/hw/hw20_si_reg.h"
"${ProjDirPath}/../../../../../middleware/wifi_qca/include/AR6002/hw2.0/hw/hw20_uart_reg.h"
"${ProjDirPath}/../../../../../middleware/wifi_qca/include/AR6002/hw2.0/hw/hw20_vmc_reg.h"
"${ProjDirPath}/../../../../../middleware/wifi_qca/include/AR6002/hw4.0/hw/hw40_rtc_reg.h"
"${ProjDirPath}/../../../../../middleware/wifi_qca/include/AR6002/hw4.0/hw/hw40_rtc_wlan_reg.h"
"${ProjDirPath}/../../../../../middleware/wifi_qca/include/athdefs.h"
"${ProjDirPath}/../../../../../middleware/wifi_qca/include/bmi_msg.h"
"${ProjDirPath}/../../../../../middleware/wifi_qca/include/dbglog.h"
"${ProjDirPath}/../../../../../middleware/wifi_qca/include/dbglog_id.h"
"${ProjDirPath}/../../../../../middleware/wifi_qca/include/htc.h"
"${ProjDirPath}/../../../../../middleware/wifi_qca/include/htc_services.h"
"${ProjDirPath}/../../../../../middleware/wifi_qca/include/p2p.h"
"${ProjDirPath}/../../../../../middleware/wifi_qca/include/pkt_log.h"
"${ProjDirPath}/../../../../../middleware/wifi_qca/include/pmu.h"
"${ProjDirPath}/../../../../../middleware/wifi_qca/include/targaddrs.h"
"${ProjDirPath}/../../../../../middleware/wifi_qca/include/wlan_defs.h"
"${ProjDirPath}/../../../../../middleware/wifi_qca/include/wmi.h"
"${ProjDirPath}/../../../../../middleware/wifi_qca/port/wifi_common.h"
"${ProjDirPath}/../../../../../middleware/wifi_qca/port/wifi_shield_common.h"
"${ProjDirPath}/../../../../../middleware/wifi_qca/qcom_api.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/FreeRTOS/event_groups.c"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/FreeRTOS/list.c"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/FreeRTOS/portable/GCC/ARM_CM4F/port.c"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/FreeRTOS/portable/GCC/ARM_CM4F/portmacro.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/FreeRTOS/queue.c"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/FreeRTOS/stream_buffer.c"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/FreeRTOS/tasks.c"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/FreeRTOS/timers.c"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include/FreeRTOS.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include/event_groups.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include/freertos_tasks_c_additions.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include/message_buffer.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include/private/deprecated_definitions.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include/private/list.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include/private/mpu_prototypes.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include/private/mpu_wrappers.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include/private/portable.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include/private/projdefs.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include/private/stack_macros.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include/private/threading_alt.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include/queue.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include/semphr.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include/stream_buffer.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include/task.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include/timers.h"
"${ProjDirPath}/../../../../../devices/MIMXRT1011/drivers/fsl_clock.c"
"${ProjDirPath}/../../../../../devices/MIMXRT1011/drivers/fsl_clock.h"
"${ProjDirPath}/../../../../../devices/MIMXRT1011/drivers/fsl_common.c"
"${ProjDirPath}/../../../../../devices/MIMXRT1011/drivers/fsl_common.h"
"${ProjDirPath}/../../../../../devices/MIMXRT1011/MIMXRT1011.h"
"${ProjDirPath}/../../../../../devices/MIMXRT1011/MIMXRT1011_features.h"
"${ProjDirPath}/../../../../../devices/MIMXRT1011/fsl_device_registers.h"
"${ProjDirPath}/../../../../../devices/MIMXRT1011/system_MIMXRT1011.c"
"${ProjDirPath}/../../../../../devices/MIMXRT1011/system_MIMXRT1011.h"
"${ProjDirPath}/../../../../../devices/MIMXRT1011/utilities/debug_console/fsl_debug_console.c"
"${ProjDirPath}/../../../../../devices/MIMXRT1011/utilities/debug_console/fsl_debug_console.h"
"${ProjDirPath}/../../../../../devices/MIMXRT1011/utilities/debug_console/fsl_debug_console_conf.h"
"${ProjDirPath}/../../../../../devices/MIMXRT1011/utilities/str/fsl_str.c"
"${ProjDirPath}/../../../../../devices/MIMXRT1011/utilities/str/fsl_str.h"
"${ProjDirPath}/../../../../../components/uart/lpuart_adapter.c"
"${ProjDirPath}/../../../../../components/uart/uart.h"
"${ProjDirPath}/../../../../../components/serial_manager/serial_manager.c"
"${ProjDirPath}/../../../../../components/serial_manager/serial_manager.h"
"${ProjDirPath}/../../../../../components/serial_manager/serial_port_internal.h"
"${ProjDirPath}/../../../../../components/serial_manager/serial_port_uart.c"
"${ProjDirPath}/../../../../../components/serial_manager/serial_port_uart.h"
"${ProjDirPath}/../../../../../devices/MIMXRT1011/drivers/fsl_lpuart.c"
"${ProjDirPath}/../../../../../devices/MIMXRT1011/drivers/fsl_lpuart.h"
"${ProjDirPath}/../../../../../components/lists/generic_list.c"
"${ProjDirPath}/../../../../../components/lists/generic_list.h"
"${ProjDirPath}/../../../../../devices/MIMXRT1011/gcc/startup_MIMXRT1011.S"
"${ProjDirPath}/../../../../../devices/MIMXRT1011/drivers/fsl_iomuxc.h"
"${ProjDirPath}/../../../../../devices/MIMXRT1011/utilities/fsl_assert.c"
"${ProjDirPath}/../../../../../devices/MIMXRT1011/drivers/fsl_gpio.c"
"${ProjDirPath}/../../../../../devices/MIMXRT1011/drivers/fsl_gpio.h"
"${ProjDirPath}/../../../../../devices/MIMXRT1011/xip/fsl_flexspi_nor_boot.c"
"${ProjDirPath}/../../../../../devices/MIMXRT1011/xip/fsl_flexspi_nor_boot.h"
"${ProjDirPath}/../../../xip/evkmimxrt1010_flexspi_nor_config.c"
"${ProjDirPath}/../../../xip/evkmimxrt1010_flexspi_nor_config.h"
"${ProjDirPath}/../../../../../middleware/wifi_qca/port/boards/evkmimxrt1010/freertos/wifi_shield.c"
"${ProjDirPath}/../../../../../middleware/wifi_qca/port/boards/evkmimxrt1010/freertos/wifi_shield.h"
"${ProjDirPath}/../../../../../middleware/wifi_qca/port/env/freertos/wifi_env.c"
"${ProjDirPath}/../../../../../middleware/wifi_qca/port/env/freertos/wifi_env.h"
"${ProjDirPath}/../../../../../middleware/wifi_qca/port/env/freertos/wifi_env_port.h"
"${ProjDirPath}/../../../../../middleware/wifi_qca/port/env/freertos/wifi_port.h"
"${ProjDirPath}/../../../../../middleware/wifi_qca/port/drivers/lpspi_edma_freertos/wifi_spi.c"
"${ProjDirPath}/../../../../../middleware/wifi_qca/port/drivers/lpspi_edma_freertos/wifi_spi.h"
"${ProjDirPath}/../../../../../devices/MIMXRT1011/drivers/fsl_lpspi.c"
"${ProjDirPath}/../../../../../devices/MIMXRT1011/drivers/fsl_lpspi.h"
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
"${ProjDirPath}/../../../../../devices/MIMXRT1011/drivers/fsl_lpspi_edma.c"
"${ProjDirPath}/../../../../../devices/MIMXRT1011/drivers/fsl_lpspi_edma.h"
"${ProjDirPath}/../../../../../devices/MIMXRT1011/drivers/fsl_edma.c"
"${ProjDirPath}/../../../../../devices/MIMXRT1011/drivers/fsl_edma.h"
"${ProjDirPath}/../../../../../devices/MIMXRT1011/drivers/fsl_dmamux.c"
"${ProjDirPath}/../../../../../devices/MIMXRT1011/drivers/fsl_dmamux.h"
)


set(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_DEBUG} -T${ProjDirPath}/MIMXRT1011xxxxx_flexspi_nor.ld -static")

set(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_RELEASE} -T${ProjDirPath}/MIMXRT1011xxxxx_flexspi_nor.ld -static")

TARGET_LINK_LIBRARIES(qca_demo.elf -Wl,--start-group)
target_link_libraries(qca_demo.elf optimized m)

target_link_libraries(qca_demo.elf optimized c)

target_link_libraries(qca_demo.elf optimized gcc)

target_link_libraries(qca_demo.elf optimized nosys)

target_link_libraries(qca_demo.elf optimized m)

target_link_libraries(qca_demo.elf optimized c)

target_link_libraries(qca_demo.elf optimized gcc)

target_link_libraries(qca_demo.elf optimized nosys)

TARGET_LINK_LIBRARIES(qca_demo.elf -Wl,--end-group)

