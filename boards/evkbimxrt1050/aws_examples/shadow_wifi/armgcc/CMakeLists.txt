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

SET(CMAKE_ASM_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_ASM_FLAGS_FLEXSPI_NOR_DEBUG} -mfpu=fpv5-d16")

SET(CMAKE_ASM_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_ASM_FLAGS_FLEXSPI_NOR_RELEASE} -D__STARTUP_CLEAR_BSS")

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

SET(CMAKE_ASM_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_ASM_FLAGS_FLEXSPI_NOR_RELEASE} -mfpu=fpv5-d16")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG} -DXIP_EXTERNAL_FLASH=1")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG} -DXIP_BOOT_HEADER_ENABLE=1")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG} -DXIP_BOOT_HEADER_DCD_ENABLE=1")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG} -DCPU_MIMXRT1052DVL6B")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG} -DUSE_RTOS=1")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG} -DMBEDTLS_CONFIG_FILE='\"aws_mbedtls_config.h\"'")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG} -DFSL_RTOS_FREE_RTOS")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG} -DA_LITTLE_ENDIAN")

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

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG} -mfpu=fpv5-d16")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG} -MMD")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG} -MP")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG} -fomit-frame-pointer")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE} -DXIP_EXTERNAL_FLASH=1")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE} -DXIP_BOOT_HEADER_ENABLE=1")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE} -DXIP_BOOT_HEADER_DCD_ENABLE=1")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE} -DCPU_MIMXRT1052DVL6B")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE} -DUSE_RTOS=1")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE} -DMBEDTLS_CONFIG_FILE='\"aws_mbedtls_config.h\"'")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE} -DFSL_RTOS_FREE_RTOS")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE} -DA_LITTLE_ENDIAN")

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

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE} -mfpu=fpv5-d16")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE} -MMD")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE} -MP")

SET(CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE} -fomit-frame-pointer")

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

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_DEBUG} -mfpu=fpv5-d16")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_DEBUG} -MMD")

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_DEBUG} -MP")

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

SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_CXX_FLAGS_FLEXSPI_NOR_RELEASE} -mfpu=fpv5-d16")

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

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_DEBUG} -mcpu=cortex-m7")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_DEBUG} -mfloat-abi=hard")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_DEBUG} -mfpu=fpv5-d16")

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

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_RELEASE} -mcpu=cortex-m7")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_RELEASE} -mfloat-abi=hard")

SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_RELEASE} -mfpu=fpv5-d16")

include_directories(${ProjDirPath}/../../..)

include_directories(${ProjDirPath}/..)

include_directories(${ProjDirPath}/../../remote_control_wifi/config_files)

include_directories(${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/FreeRTOS/portable/GCC/ARM_CM4F)

include_directories(${ProjDirPath}/../../../../../CMSIS/Include)

include_directories(${ProjDirPath}/../../../../../devices)

include_directories(${ProjDirPath}/../../../../src/aws_examples/common)

include_directories(${ProjDirPath}/../config_files)

include_directories(${ProjDirPath}/../../../../../platform/utilities/virtual_com)

include_directories(${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include)

include_directories(${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include/private)

include_directories(${ProjDirPath}/../../../../../middleware/mbedtls/include)

include_directories(${ProjDirPath}/../../../../../middleware/mbedtls/port/ksdk)

include_directories(${ProjDirPath}/../../../../../devices/MIMXRT1052/drivers)

include_directories(${ProjDirPath}/../../../../../middleware/wifi_qca/port/boards/evkbimxrt1050/freertos/gt202)

include_directories(${ProjDirPath}/../../../../../middleware/wifi_qca/port/boards/evkbimxrt1050/freertos)

include_directories(${ProjDirPath}/../../../../../middleware/wifi_qca/port/env/freertos)

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

include_directories(${ProjDirPath}/../../../../../middleware/wifi_qca/port/drivers/lpspi_edma_freertos)

include_directories(${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/third_party/mcu_vendor/nxp/MIMXRT1052/drivers)

include_directories(${ProjDirPath}/../../../../../rtos/amazon-freertos/demos/common/include)

include_directories(${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/third_party/pkcs11)

include_directories(${ProjDirPath}/../../../../../devices/MIMXRT1052)

include_directories(${ProjDirPath}/../../../../../devices/MIMXRT1052/utilities/io)

include_directories(${ProjDirPath}/../../../../../devices/MIMXRT1052/utilities/str)

include_directories(${ProjDirPath}/../../../../../devices/MIMXRT1052/utilities/log)

include_directories(${ProjDirPath}/../../../../../devices/MIMXRT1052/utilities)

include_directories(${ProjDirPath}/../../../../../devices/MIMXRT1052/xip)

include_directories(${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/third_party/jsmn)

add_executable(aws_shadow_wifi.elf 
"${ProjDirPath}/../aws_shadow_lightbulb_on_off.c"
"${ProjDirPath}/../main.c"
"${ProjDirPath}/../aws_mbedtls_config.h"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../FreeRTOSConfig.h"
"${ProjDirPath}/../config_files/aws_bufferpool_config.h"
"${ProjDirPath}/../config_files/aws_demo_config.h"
"${ProjDirPath}/../config_files/aws_mqtt_agent_config.h"
"${ProjDirPath}/../config_files/aws_mqtt_config.h"
"${ProjDirPath}/../config_files/aws_secure_sockets_config.h"
"${ProjDirPath}/../config_files/aws_shadow_config.h"
"${ProjDirPath}/../config_files/aws_wifi_config.h"
"${ProjDirPath}/../config_files/FreeRTOSIPConfig.h"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
"${ProjDirPath}/../../../../../devices/MIMXRT1052/drivers/fsl_trng.c"
"${ProjDirPath}/../../../../../devices/MIMXRT1052/drivers/fsl_trng.h"
"${ProjDirPath}/../../../../../devices/MIMXRT1052/drivers/fsl_common.h"
"${ProjDirPath}/../../../../../devices/MIMXRT1052/drivers/fsl_common.c"
"${ProjDirPath}/../../../../../devices/MIMXRT1052/drivers/fsl_clock.h"
"${ProjDirPath}/../../../../../devices/MIMXRT1052/drivers/fsl_clock.c"
"${ProjDirPath}/../../../../../devices/MIMXRT1052/drivers/fsl_dcp.c"
"${ProjDirPath}/../../../../../devices/MIMXRT1052/drivers/fsl_dcp.h"
"${ProjDirPath}/../../../../../devices/MIMXRT1052/drivers/fsl_cache.c"
"${ProjDirPath}/../../../../../devices/MIMXRT1052/drivers/fsl_cache.h"
"${ProjDirPath}/../../../../../devices/MIMXRT1052/drivers/fsl_flexspi.c"
"${ProjDirPath}/../../../../../devices/MIMXRT1052/drivers/fsl_flexspi.h"
"${ProjDirPath}/../../../../../middleware/wifi_qca/port/boards/evkbimxrt1050/freertos/gt202/wifi_shield_gt202.h"
"${ProjDirPath}/../../../../../middleware/wifi_qca/port/boards/evkbimxrt1050/freertos/wifi_shield.h"
"${ProjDirPath}/../../../../../middleware/wifi_qca/port/boards/evkbimxrt1050/freertos/wifi_shield.c"
"${ProjDirPath}/../../../../../middleware/wifi_qca/port/env/freertos/wifi_port.h"
"${ProjDirPath}/../../../../../middleware/wifi_qca/port/env/freertos/wifi_env_port.h"
"${ProjDirPath}/../../../../../middleware/wifi_qca/port/env/freertos/wifi_env.c"
"${ProjDirPath}/../../../../../middleware/wifi_qca/port/env/freertos/wifi_env.h"
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
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include/private/portable.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include/private/projdefs.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include/private/stack_macros.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/FreeRTOS/event_groups.c"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/FreeRTOS/list.c"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/FreeRTOS/queue.c"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/FreeRTOS/stream_buffer.c"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/FreeRTOS/tasks.c"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/FreeRTOS/timers.c"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/FreeRTOS/portable/readme.txt"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/FreeRTOS/portable/GCC/ARM_CM4F/fsl_tickless_generic.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/FreeRTOS/portable/GCC/ARM_CM4F/fsl_tickless_systick.c"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/FreeRTOS/portable/GCC/ARM_CM4F/port.c"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/FreeRTOS/portable/GCC/ARM_CM4F/portmacro.h"
"${ProjDirPath}/../../../../../middleware/wifi_qca/common_src/wmi/wmi.c"
"${ProjDirPath}/../../../../../middleware/wifi_qca/common_src/storerecall/dset_api.c"
"${ProjDirPath}/../../../../../middleware/wifi_qca/common_src/stack_common/common_stack_offload.h"
"${ProjDirPath}/../../../../../middleware/wifi_qca/common_src/stack_common/api_stack_offload.c"
"${ProjDirPath}/../../../../../middleware/wifi_qca/common_src/qapi/qcom_api.c"
"${ProjDirPath}/../../../../../middleware/wifi_qca/common_src/qapi/qcom_dns.c"
"${ProjDirPath}/../../../../../middleware/wifi_qca/common_src/include/dset_api.h"
"${ProjDirPath}/../../../../../middleware/wifi_qca/common_src/include/driver_cxt.h"
"${ProjDirPath}/../../../../../middleware/wifi_qca/common_src/include/a_drv_api.h"
"${ProjDirPath}/../../../../../middleware/wifi_qca/common_src/htc/htc.c"
"${ProjDirPath}/../../../../../middleware/wifi_qca/common_src/hcd/spi_hcd.c"
"${ProjDirPath}/../../../../../middleware/wifi_qca/common_src/driver/driver_main.c"
"${ProjDirPath}/../../../../../middleware/wifi_qca/common_src/api_interface/api_wmi_rx.c"
"${ProjDirPath}/../../../../../middleware/wifi_qca/common_src/api_interface/api_ioctl.c"
"${ProjDirPath}/../../../../../middleware/wifi_qca/common_src/include/wlan_api.h"
"${ProjDirPath}/../../../../../middleware/wifi_qca/common_src/hw_interface/hw_api.c"
"${ProjDirPath}/../../../../../middleware/wifi_qca/common_src/storerecall/dset.c"
"${ProjDirPath}/../../../../../middleware/wifi_qca/common_src/include/wmi_api.h"
"${ProjDirPath}/../../../../../middleware/wifi_qca/common_src/include/hif_internal.h"
"${ProjDirPath}/../../../../../middleware/wifi_qca/common_src/include/dset.h"
"${ProjDirPath}/../../../../../middleware/wifi_qca/common_src/include/dbglog_api.h"
"${ProjDirPath}/../../../../../middleware/wifi_qca/common_src/include/common_api.h"
"${ProjDirPath}/../../../../../middleware/wifi_qca/common_src/include/bmi.h"
"${ProjDirPath}/../../../../../middleware/wifi_qca/common_src/include/aggr_recv_api.h"
"${ProjDirPath}/../../../../../middleware/wifi_qca/common_src/include/a_drv.h"
"${ProjDirPath}/../../../../../middleware/wifi_qca/common_src/include/a_debug.h"
"${ProjDirPath}/../../../../../middleware/wifi_qca/common_src/hcd/hcd_api.h"
"${ProjDirPath}/../../../../../middleware/wifi_qca/common_src/driver/driver_txrx.c"
"${ProjDirPath}/../../../../../middleware/wifi_qca/common_src/driver/driver_netbuf.c"
"${ProjDirPath}/../../../../../middleware/wifi_qca/common_src/driver/driver_init.c"
"${ProjDirPath}/../../../../../middleware/wifi_qca/common_src/driver/driver_diag.c"
"${ProjDirPath}/../../../../../middleware/wifi_qca/common_src/bmi/bmi.c"
"${ProjDirPath}/../../../../../middleware/wifi_qca/common_src/api_interface/api_txrx.c"
"${ProjDirPath}/../../../../../middleware/wifi_qca/common_src/api_interface/api_init.c"
"${ProjDirPath}/../../../../../middleware/wifi_qca/common_src/include/spi_hcd_if.h"
"${ProjDirPath}/../../../../../middleware/wifi_qca/common_src/wmi/wmi_host.h"
"${ProjDirPath}/../../../../../middleware/wifi_qca/common_src/util/util.c"
"${ProjDirPath}/../../../../../middleware/wifi_qca/common_src/storerecall/storerecall.c"
"${ProjDirPath}/../../../../../middleware/wifi_qca/common_src/reorder/rcv_aggr.c"
"${ProjDirPath}/../../../../../middleware/wifi_qca/common_src/qapi/qcom_legacy.c"
"${ProjDirPath}/../../../../../middleware/wifi_qca/common_src/include/netbuf.h"
"${ProjDirPath}/../../../../../middleware/wifi_qca/common_src/include/ieee80211.h"
"${ProjDirPath}/../../../../../middleware/wifi_qca/common_src/include/htc_api.h"
"${ProjDirPath}/../../../../../middleware/wifi_qca/custom_src/stack_custom/cust_api_stack_offload.c"
"${ProjDirPath}/../../../../../middleware/wifi_qca/custom_src/qapi/custom_qcom_api.c"
"${ProjDirPath}/../../../../../middleware/wifi_qca/custom_src/include/cust_netbuf.h"
"${ProjDirPath}/../../../../../middleware/wifi_qca/custom_src/include/a_osapi.h"
"${ProjDirPath}/../../../../../middleware/wifi_qca/custom_src/include/a_config.h"
"${ProjDirPath}/../../../../../middleware/wifi_qca/custom_src/api_interface/cust_api_wmi_rx.c"
"${ProjDirPath}/../../../../../middleware/wifi_qca/custom_src/api_interface/cust_api_ioctl.c"
"${ProjDirPath}/../../../../../middleware/wifi_qca/custom_src/driver/cust_driver_netbuf.c"
"${ProjDirPath}/../../../../../middleware/wifi_qca/custom_src/include/wlan_config.h"
"${ProjDirPath}/../../../../../middleware/wifi_qca/custom_src/stack_custom/qca_structs.h"
"${ProjDirPath}/../../../../../middleware/wifi_qca/custom_src/stack_custom/custom_stack_offload.h"
"${ProjDirPath}/../../../../../middleware/wifi_qca/custom_src/stack_custom/cust_api_stack_txrx.c"
"${ProjDirPath}/../../../../../middleware/wifi_qca/custom_src/include/custom_wlan_api.h"
"${ProjDirPath}/../../../../../middleware/wifi_qca/custom_src/include/a_types.h"
"${ProjDirPath}/../../../../../middleware/wifi_qca/custom_src/hw_interface/cust_spi_hcd.c"
"${ProjDirPath}/../../../../../middleware/wifi_qca/custom_src/driver/cust_driver_main.c"
"${ProjDirPath}/../../../../../middleware/wifi_qca/custom_src/api_interface/cust_api_txrx.c"
"${ProjDirPath}/../../../../../middleware/wifi_qca/custom_src/api_interface/cust_api_init.c"
"${ProjDirPath}/../../../../../middleware/wifi_qca/include/wmi.h"
"${ProjDirPath}/../../../../../middleware/wifi_qca/include/dbglog.h"
"${ProjDirPath}/../../../../../middleware/wifi_qca/include/athdefs.h"
"${ProjDirPath}/../../../../../middleware/wifi_qca/include/dbglog_id.h"
"${ProjDirPath}/../../../../../middleware/wifi_qca/include/bmi_msg.h"
"${ProjDirPath}/../../../../../middleware/wifi_qca/include/AR6002/hw2.0/hw/hw20_gpio_reg.h"
"${ProjDirPath}/../../../../../middleware/wifi_qca/include/AR6002/hw2.0/hw/hw20_apb_map.h"
"${ProjDirPath}/../../../../../middleware/wifi_qca/include/AR6002/AR6K_version.h"
"${ProjDirPath}/../../../../../middleware/wifi_qca/include/AR6002/hw4.0/hw/hw40_rtc_wlan_reg.h"
"${ProjDirPath}/../../../../../middleware/wifi_qca/include/AR6002/hw4.0/hw/hw40_rtc_reg.h"
"${ProjDirPath}/../../../../../middleware/wifi_qca/include/wlan_defs.h"
"${ProjDirPath}/../../../../../middleware/wifi_qca/include/targaddrs.h"
"${ProjDirPath}/../../../../../middleware/wifi_qca/include/pmu.h"
"${ProjDirPath}/../../../../../middleware/wifi_qca/include/pkt_log.h"
"${ProjDirPath}/../../../../../middleware/wifi_qca/include/p2p.h"
"${ProjDirPath}/../../../../../middleware/wifi_qca/include/htc_services.h"
"${ProjDirPath}/../../../../../middleware/wifi_qca/include/htc.h"
"${ProjDirPath}/../../../../../middleware/wifi_qca/include/AR6002/hw2.0/hw/hw20_vmc_reg.h"
"${ProjDirPath}/../../../../../middleware/wifi_qca/include/AR6002/hw2.0/hw/hw20_uart_reg.h"
"${ProjDirPath}/../../../../../middleware/wifi_qca/include/AR6002/hw2.0/hw/hw20_si_reg.h"
"${ProjDirPath}/../../../../../middleware/wifi_qca/include/AR6002/hw2.0/hw/hw20_rtc_reg.h"
"${ProjDirPath}/../../../../../middleware/wifi_qca/include/AR6002/hw2.0/hw/hw20_mbox_reg.h"
"${ProjDirPath}/../../../../../middleware/wifi_qca/include/AR6002/hw2.0/hw/hw20_mbox_host_reg.h"
"${ProjDirPath}/../../../../../middleware/wifi_qca/port/wifi_common.h"
"${ProjDirPath}/../../../../../middleware/wifi_qca/qcom_api.h"
"${ProjDirPath}/../../../../../middleware/wifi_qca/atheros_stack_offload.h"
"${ProjDirPath}/../../../../../middleware/wifi_qca/atheros_wifi_internal.h"
"${ProjDirPath}/../../../../../middleware/wifi_qca/atheros_wifi_api.h"
"${ProjDirPath}/../../../../../middleware/wifi_qca/atheros_wifi.h"
"${ProjDirPath}/../../../../../middleware/wifi_qca/port/drivers/lpspi_edma_freertos/wifi_spi.h"
"${ProjDirPath}/../../../../../middleware/wifi_qca/port/drivers/lpspi_edma_freertos/wifi_spi.c"
"${ProjDirPath}/../../../../../devices/MIMXRT1052/drivers/fsl_lpspi.c"
"${ProjDirPath}/../../../../../devices/MIMXRT1052/drivers/fsl_lpspi.h"
"${ProjDirPath}/../../../../../devices/MIMXRT1052/drivers/fsl_lpspi_edma.h"
"${ProjDirPath}/../../../../../devices/MIMXRT1052/drivers/fsl_lpspi_edma.c"
"${ProjDirPath}/../../../../../devices/MIMXRT1052/drivers/fsl_edma.h"
"${ProjDirPath}/../../../../../devices/MIMXRT1052/drivers/fsl_edma.c"
"${ProjDirPath}/../../../../../devices/MIMXRT1052/drivers/fsl_dmamux.h"
"${ProjDirPath}/../../../../../devices/MIMXRT1052/drivers/fsl_dmamux.c"
"${ProjDirPath}/../../../../../devices/MIMXRT1052/drivers/fsl_gpio.h"
"${ProjDirPath}/../../../../../devices/MIMXRT1052/drivers/fsl_gpio.c"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/third_party/mcu_vendor/nxp/MIMXRT1052/drivers/mflash_drv.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/third_party/mcu_vendor/nxp/MIMXRT1052/drivers/mflash_drv.c"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/third_party/mcu_vendor/nxp/MIMXRT1052/drivers/mflash_file.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/third_party/mcu_vendor/nxp/MIMXRT1052/drivers/mflash_file.c"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/pkcs11/portable/nxp/common/pkcs11.c"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include/FreeRTOS.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include/aws_crypto.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include/aws_greengrass_discovery.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include/aws_mqtt_agent.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include/aws_mqtt_lib.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include/aws_pkcs11.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include/aws_secure_sockets.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include/aws_shadow.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include/aws_system_init.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include/aws_tls.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include/aws_wifi.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include/event_groups.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include/message_buffer.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include/private/aws_bufferpool.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include/private/aws_default_root_certificates.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include/private/aws_doubly_linked_list.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include/private/aws_ggd_config_defaults.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include/private/aws_greengrass_lib_private.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include/private/aws_helper_secure_connect.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include/private/aws_lib_init.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include/private/aws_mqtt_agent_config_defaults.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include/private/aws_mqtt_buffer.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include/private/aws_mqtt_config_defaults.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include/private/aws_mqtt_lib_private.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include/private/aws_rsprintf.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include/private/aws_secure_sockets_config_defaults.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include/private/aws_shadow_config_defaults.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include/private/aws_shadow_json.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include/private/deprecated_definitions.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include/private/list.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include/private/mpu_wrappers.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include/private/portable.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include/private/projdefs.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include/private/stack_macros.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include/queue.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include/semphr.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include/stream_buffer.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include/task.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include/timers.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/mqtt/aws_mqtt_agent.c"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/mqtt/aws_mqtt_lib.c"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/tls/aws_tls.c"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/bufferpool/aws_bufferpool_static_thread_safe.c"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/demos/common/include/aws_clientcredential.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/demos/common/include/aws_clientcredential_keys.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/demos/common/include/aws_demo.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/demos/common/include/aws_dev_mode_key_provisioning.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/demos/common/include/aws_logging_task.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/demos/common/include/aws_shadow_lightbulb_on_off.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/demos/common/include/aws_greengrass_discovery_demo.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/demos/common/logging/aws_logging_task_dynamic_buffers.c"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/crypto/aws_crypto.c"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/third_party/pkcs11/pkcs11.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/third_party/pkcs11/pkcs11f.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/third_party/pkcs11/pkcs11t.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/utils/aws_system_init.c"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/demos/common/devmode_key_provisioning/aws_dev_mode_key_provisioning.c"
"${ProjDirPath}/../../../../../devices/MIMXRT1052/system_MIMXRT1052.c"
"${ProjDirPath}/../../../../../devices/MIMXRT1052/system_MIMXRT1052.h"
"${ProjDirPath}/../../../../../devices/MIMXRT1052/gcc/startup_MIMXRT1052.S"
"${ProjDirPath}/../../../../../devices/MIMXRT1052/drivers/fsl_lpuart.h"
"${ProjDirPath}/../../../../../devices/MIMXRT1052/drivers/fsl_lpuart.c"
"${ProjDirPath}/../../../../../devices/MIMXRT1052/utilities/io/fsl_io.c"
"${ProjDirPath}/../../../../../devices/MIMXRT1052/utilities/io/fsl_io.h"
"${ProjDirPath}/../../../../../devices/MIMXRT1052/utilities/log/fsl_log.c"
"${ProjDirPath}/../../../../../devices/MIMXRT1052/utilities/log/fsl_log.h"
"${ProjDirPath}/../../../../../devices/MIMXRT1052/utilities/str/fsl_str.c"
"${ProjDirPath}/../../../../../devices/MIMXRT1052/utilities/str/fsl_str.h"
"${ProjDirPath}/../../../../../devices/MIMXRT1052/utilities/fsl_debug_console.c"
"${ProjDirPath}/../../../../../devices/MIMXRT1052/utilities/fsl_debug_console.h"
"${ProjDirPath}/../../../../../devices/MIMXRT1052/utilities/fsl_debug_console_conf.h"
"${ProjDirPath}/../../../../../devices/MIMXRT1052/drivers/fsl_iomuxc.h"
"${ProjDirPath}/../../../../../devices/MIMXRT1052/utilities/fsl_assert.c"
"${ProjDirPath}/../../../../../devices/MIMXRT1052/xip/fsl_flexspi_nor_boot.h"
"${ProjDirPath}/../../../../../devices/MIMXRT1052/xip/fsl_flexspi_nor_boot.c"
"${ProjDirPath}/../../../../../devices/MIMXRT1052/xip/evkbimxrt1050_flexspi_nor_config.c"
"${ProjDirPath}/../../../../../devices/MIMXRT1052/xip/evkbimxrt1050_flexspi_nor_config.h"
"${ProjDirPath}/../../../../../devices/MIMXRT1052/xip/evkbimxrt1050_sdram_ini_dcd.c"
"${ProjDirPath}/../../../../../devices/MIMXRT1052/xip/evkbimxrt1050_sdram_ini_dcd.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/FreeRTOS/portable/MemMang/heap_4.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/aes.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/aesni.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/arc4.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/asn1.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/asn1write.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/base64.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/bignum.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/blowfish.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/bn_mul.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/camellia.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/ccm.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/certs.h"
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
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/hmac_drbg.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/md.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/md_internal.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/md2.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/md4.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/md5.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/memory_buffer_alloc.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/net.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/net_sockets.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/oid.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/padlock.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/pem.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/pk.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/pk_internal.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/pkcs5.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/pkcs11.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/pkcs12.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/platform.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/platform_time.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/ripemd160.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/rsa.h"
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
"${ProjDirPath}/../../../../../middleware/mbedtls/library/asn1parse.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/asn1write.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/base64.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/bignum.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/blowfish.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/camellia.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/ccm.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/certs.c"
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
"${ProjDirPath}/../../../../../middleware/mbedtls/library/hmac_drbg.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/md.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/md_wrap.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/md2.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/md4.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/md5.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/memory_buffer_alloc.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/net_sockets.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/oid.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/padlock.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/pem.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/pk.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/pk_wrap.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/pkcs5.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/pkcs12.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/pkparse.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/pkwrite.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/platform.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/ripemd160.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/rsa.c"
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
"${ProjDirPath}/../../../../../middleware/mbedtls/port/ksdk/ksdk_mbedtls.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/port/ksdk/ksdk_mbedtls.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/port/ksdk/ksdk_mbedtls_config.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/port/ksdk/sha1_alt.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/port/ksdk/sha256_alt.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/port/ksdk/des_alt.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/port/ksdk/des_alt.c"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/third_party/jsmn/jsmn.c"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/third_party/jsmn/jsmn.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/shadow/aws_shadow.c"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/shadow/aws_shadow_json.c"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/secure_sockets/portable/nxp/wifi_qca/aws_secure_sockets.c"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/wifi/portable/nxp/common/aws_wifi.c"
)


set(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_DEBUG "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_DEBUG} -T${ProjDirPath}/MIMXRT1052xxxxx_flexspi_nor.ld -static")

set(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_RELEASE "${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_RELEASE} -T${ProjDirPath}/MIMXRT1052xxxxx_flexspi_nor.ld -static")

TARGET_LINK_LIBRARIES(aws_shadow_wifi.elf -Wl,--start-group)
target_link_libraries(aws_shadow_wifi.elf optimized m)

target_link_libraries(aws_shadow_wifi.elf optimized c)

target_link_libraries(aws_shadow_wifi.elf optimized gcc)

target_link_libraries(aws_shadow_wifi.elf optimized nosys)

target_link_libraries(aws_shadow_wifi.elf optimized m)

target_link_libraries(aws_shadow_wifi.elf optimized c)

target_link_libraries(aws_shadow_wifi.elf optimized gcc)

target_link_libraries(aws_shadow_wifi.elf optimized nosys)

TARGET_LINK_LIBRARIES(aws_shadow_wifi.elf -Wl,--end-group)

