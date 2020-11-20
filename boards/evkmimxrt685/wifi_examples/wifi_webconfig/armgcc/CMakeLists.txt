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

SET(CMAKE_C_FLAGS_FLASH_DEBUG "${CMAKE_C_FLAGS_FLASH_DEBUG} -include ${ProjDirPath}/../../../../../boards/evkmimxrt685/wifi_examples/wifi_webconfig/wifi_config.h")

SET(CMAKE_C_FLAGS_FLASH_DEBUG "${CMAKE_C_FLAGS_FLASH_DEBUG} -DXIP_EXTERNAL_FLASH")

SET(CMAKE_C_FLAGS_FLASH_DEBUG "${CMAKE_C_FLAGS_FLASH_DEBUG} -DDEBUG")

SET(CMAKE_C_FLAGS_FLASH_DEBUG "${CMAKE_C_FLAGS_FLASH_DEBUG} -DFSL_SDK_DRIVER_QUICK_ACCESS_ENABLE=1")

SET(CMAKE_C_FLAGS_FLASH_DEBUG "${CMAKE_C_FLAGS_FLASH_DEBUG} -DLWIP_TIMEVAL_PRIVATE=0")

SET(CMAKE_C_FLAGS_FLASH_DEBUG "${CMAKE_C_FLAGS_FLASH_DEBUG} -DCPU_MIMXRT685SFVKB_cm33")

SET(CMAKE_C_FLAGS_FLASH_DEBUG "${CMAKE_C_FLAGS_FLASH_DEBUG} -DBOOT_HEADER_ENABLE=1")

SET(CMAKE_C_FLAGS_FLASH_DEBUG "${CMAKE_C_FLAGS_FLASH_DEBUG} -DUSE_RTOS=1")

SET(CMAKE_C_FLAGS_FLASH_DEBUG "${CMAKE_C_FLAGS_FLASH_DEBUG} -DWIFI_BOARD_PAN9026_SDIO")

SET(CMAKE_C_FLAGS_FLASH_DEBUG "${CMAKE_C_FLAGS_FLASH_DEBUG} -DPRINTF_ADVANCED_ENABLE=1")

SET(CMAKE_C_FLAGS_FLASH_DEBUG "${CMAKE_C_FLAGS_FLASH_DEBUG} -DLWIP_NETIF_API=1")

SET(CMAKE_C_FLAGS_FLASH_DEBUG "${CMAKE_C_FLAGS_FLASH_DEBUG} -DHTTPSRV_CFG_WEBSOCKET_ENABLED=1")

SET(CMAKE_C_FLAGS_FLASH_DEBUG "${CMAKE_C_FLAGS_FLASH_DEBUG} -DHTTPSRV_CFG_DEFAULT_SES_CNT=8")

SET(CMAKE_C_FLAGS_FLASH_DEBUG "${CMAKE_C_FLAGS_FLASH_DEBUG} -DHTTPSRV_CFG_HTTP_SESSION_STACK_SIZE=2048")

SET(CMAKE_C_FLAGS_FLASH_DEBUG "${CMAKE_C_FLAGS_FLASH_DEBUG} -DSDK_DEBUGCONSOLE_UART")

SET(CMAKE_C_FLAGS_FLASH_DEBUG "${CMAKE_C_FLAGS_FLASH_DEBUG} -DSDK_I2C_BASED_COMPONENT_USED=1")

SET(CMAKE_C_FLAGS_FLASH_DEBUG "${CMAKE_C_FLAGS_FLASH_DEBUG} -DSDIO_ENABLED")

SET(CMAKE_C_FLAGS_FLASH_DEBUG "${CMAKE_C_FLAGS_FLASH_DEBUG} -DSERIAL_PORT_TYPE_UART=1")

SET(CMAKE_C_FLAGS_FLASH_DEBUG "${CMAKE_C_FLAGS_FLASH_DEBUG} -DFSL_RTOS_FREE_RTOS")

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

SET(CMAKE_C_FLAGS_FLASH_RELEASE "${CMAKE_C_FLAGS_FLASH_RELEASE} -include ${ProjDirPath}/../../../../../boards/evkmimxrt685/wifi_examples/wifi_webconfig/wifi_config.h")

SET(CMAKE_C_FLAGS_FLASH_RELEASE "${CMAKE_C_FLAGS_FLASH_RELEASE} -DXIP_EXTERNAL_FLASH")

SET(CMAKE_C_FLAGS_FLASH_RELEASE "${CMAKE_C_FLAGS_FLASH_RELEASE} -DNDEBUG")

SET(CMAKE_C_FLAGS_FLASH_RELEASE "${CMAKE_C_FLAGS_FLASH_RELEASE} -DFSL_SDK_DRIVER_QUICK_ACCESS_ENABLE=1")

SET(CMAKE_C_FLAGS_FLASH_RELEASE "${CMAKE_C_FLAGS_FLASH_RELEASE} -DLWIP_TIMEVAL_PRIVATE=0")

SET(CMAKE_C_FLAGS_FLASH_RELEASE "${CMAKE_C_FLAGS_FLASH_RELEASE} -DCPU_MIMXRT685SFVKB_cm33")

SET(CMAKE_C_FLAGS_FLASH_RELEASE "${CMAKE_C_FLAGS_FLASH_RELEASE} -DBOOT_HEADER_ENABLE=1")

SET(CMAKE_C_FLAGS_FLASH_RELEASE "${CMAKE_C_FLAGS_FLASH_RELEASE} -DUSE_RTOS=1")

SET(CMAKE_C_FLAGS_FLASH_RELEASE "${CMAKE_C_FLAGS_FLASH_RELEASE} -DWIFI_BOARD_PAN9026_SDIO")

SET(CMAKE_C_FLAGS_FLASH_RELEASE "${CMAKE_C_FLAGS_FLASH_RELEASE} -DPRINTF_ADVANCED_ENABLE=1")

SET(CMAKE_C_FLAGS_FLASH_RELEASE "${CMAKE_C_FLAGS_FLASH_RELEASE} -DLWIP_NETIF_API=1")

SET(CMAKE_C_FLAGS_FLASH_RELEASE "${CMAKE_C_FLAGS_FLASH_RELEASE} -DHTTPSRV_CFG_WEBSOCKET_ENABLED=1")

SET(CMAKE_C_FLAGS_FLASH_RELEASE "${CMAKE_C_FLAGS_FLASH_RELEASE} -DHTTPSRV_CFG_DEFAULT_SES_CNT=8")

SET(CMAKE_C_FLAGS_FLASH_RELEASE "${CMAKE_C_FLAGS_FLASH_RELEASE} -DHTTPSRV_CFG_HTTP_SESSION_STACK_SIZE=2048")

SET(CMAKE_C_FLAGS_FLASH_RELEASE "${CMAKE_C_FLAGS_FLASH_RELEASE} -DSDK_DEBUGCONSOLE_UART")

SET(CMAKE_C_FLAGS_FLASH_RELEASE "${CMAKE_C_FLAGS_FLASH_RELEASE} -DSDK_I2C_BASED_COMPONENT_USED=1")

SET(CMAKE_C_FLAGS_FLASH_RELEASE "${CMAKE_C_FLAGS_FLASH_RELEASE} -DSDIO_ENABLED")

SET(CMAKE_C_FLAGS_FLASH_RELEASE "${CMAKE_C_FLAGS_FLASH_RELEASE} -DSERIAL_PORT_TYPE_UART=1")

SET(CMAKE_C_FLAGS_FLASH_RELEASE "${CMAKE_C_FLAGS_FLASH_RELEASE} -DFSL_RTOS_FREE_RTOS")

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

SET(CMAKE_EXE_LINKER_FLAGS_FLASH_DEBUG "${CMAKE_EXE_LINKER_FLAGS_FLASH_DEBUG} --defsym=__stack_size__=2048")

SET(CMAKE_EXE_LINKER_FLAGS_FLASH_DEBUG "${CMAKE_EXE_LINKER_FLAGS_FLASH_DEBUG} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_FLASH_DEBUG "${CMAKE_EXE_LINKER_FLAGS_FLASH_DEBUG} --defsym=__heap_size__=90000")

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

SET(CMAKE_EXE_LINKER_FLAGS_FLASH_RELEASE "${CMAKE_EXE_LINKER_FLAGS_FLASH_RELEASE} --defsym=__stack_size__=2048")

SET(CMAKE_EXE_LINKER_FLAGS_FLASH_RELEASE "${CMAKE_EXE_LINKER_FLAGS_FLASH_RELEASE} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_FLASH_RELEASE "${CMAKE_EXE_LINKER_FLAGS_FLASH_RELEASE} --defsym=__heap_size__=90000")

include_directories(${ProjDirPath}/../../../../src/wifi_common/webconfig/nxp)

include_directories(${ProjDirPath}/../../../../src/wifi_common/webconfig)

include_directories(${ProjDirPath}/..)

include_directories(${ProjDirPath}/../../..)

include_directories(${ProjDirPath}/../../../../src/wifi_common/webconfig/nxp)

include_directories(${ProjDirPath}/../../../../src/wifi_common/webconfig)

include_directories(${ProjDirPath}/..)

include_directories(${ProjDirPath}/../../..)

include_directories(${ProjDirPath}/../../../../../rtos/freertos/freertos_kernel/portable/GCC/ARM_CM33_NTZ/non_secure)

include_directories(${ProjDirPath}/../../../../src/wifi_common/webconfig/nxp)

include_directories(${ProjDirPath}/..)

include_directories(${ProjDirPath}/../../../../../middleware/lwip/src/apps/httpsrv)

include_directories(${ProjDirPath}/../../..)

include_directories(${ProjDirPath}/../../../../../CMSIS/Include)

include_directories(${ProjDirPath}/../../../../../devices)

include_directories(${ProjDirPath}/../../../../../rtos/freertos/freertos_kernel/include)

include_directories(${ProjDirPath}/../../../../../devices/MIMXRT685S/drivers)

include_directories(${ProjDirPath}/../../../../../components/pca9420)

include_directories(${ProjDirPath}/../../../../../middleware/mflash/mimxrt685)

include_directories(${ProjDirPath}/../../../../../middleware/lwip/port)

include_directories(${ProjDirPath}/../../../../../middleware/lwip/src)

include_directories(${ProjDirPath}/../../../../../middleware/lwip/src/include)

include_directories(${ProjDirPath}/../../../../../devices/MIMXRT685S/utilities/str)

include_directories(${ProjDirPath}/../../../../../devices/MIMXRT685S/utilities/debug_console)

include_directories(${ProjDirPath}/../../../../../middleware/wifi/dhcpd)

include_directories(${ProjDirPath}/../../../../../middleware/wifi/incl)

include_directories(${ProjDirPath}/../../../../../middleware/wifi/incl/port/lwip)

include_directories(${ProjDirPath}/../../../../../middleware/wifi/incl/port/os)

include_directories(${ProjDirPath}/../../../../../middleware/wifi/incl/wifidriver)

include_directories(${ProjDirPath}/../../../../../middleware/wifi/incl/wlcmgr)

include_directories(${ProjDirPath}/../../../../../middleware/wifi/port/lwip)

include_directories(${ProjDirPath}/../../../../../middleware/wifi/wifidriver)

include_directories(${ProjDirPath}/../../../../../middleware/wifi/wifidriver/incl)

include_directories(${ProjDirPath}/../../../../../middleware/wifi/wifi_bt_firmware)

include_directories(${ProjDirPath}/../../../../../middleware/sdmmc/sdio)

include_directories(${ProjDirPath}/../../../../../middleware/sdmmc/common)

include_directories(${ProjDirPath}/../../../../../middleware/sdmmc/host/usdhc)

include_directories(${ProjDirPath}/../../../../../devices/MIMXRT685S)

include_directories(${ProjDirPath}/../../../../../components/uart)

include_directories(${ProjDirPath}/../../../flash_config)

include_directories(${ProjDirPath}/../../../../../components/serial_manager)

include_directories(${ProjDirPath}/../../../../../components/lists)

include_directories(${ProjDirPath}/../../../../../middleware/sdmmc/osa)

include_directories(${ProjDirPath}/../../../../../components/osa)

include_directories(${ProjDirPath}/../../../../../devices/MIMXRT685S/utilities)

add_executable(wifi_webconfig.elf 
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../lwipopts.h"
"${ProjDirPath}/../lwippools.h"
"${ProjDirPath}/../wifi_config.h"
"${ProjDirPath}/../webconfig.c"
"${ProjDirPath}/../webconfig.h"
"${ProjDirPath}/../httpsrv_fs_data.c"
"${ProjDirPath}/../cred_flash_storage.c"
"${ProjDirPath}/../cred_flash_storage.h"
"${ProjDirPath}/../http_server.c"
"${ProjDirPath}/../http_server.h"
"${ProjDirPath}/../wpl.h"
"${ProjDirPath}/../wpl_nxp.c"
"${ProjDirPath}/../FreeRTOSConfig.h"
"${ProjDirPath}/../../../sdmmc_config.c"
"${ProjDirPath}/../../../sdmmc_config.h"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
"${ProjDirPath}/../../../../../rtos/freertos/freertos_kernel/portable/GCC/ARM_CM33_NTZ/non_secure/port.c"
"${ProjDirPath}/../../../../../rtos/freertos/freertos_kernel/portable/GCC/ARM_CM33_NTZ/non_secure/portasm.c"
"${ProjDirPath}/../../../../../rtos/freertos/freertos_kernel/portable/GCC/ARM_CM33_NTZ/non_secure/portasm.h"
"${ProjDirPath}/../../../../../rtos/freertos/freertos_kernel/portable/GCC/ARM_CM33_NTZ/non_secure/portmacro.h"
"${ProjDirPath}/../../../../../devices/MIMXRT685S/drivers/fsl_i2c.c"
"${ProjDirPath}/../../../../../devices/MIMXRT685S/drivers/fsl_i2c.h"
"${ProjDirPath}/../../../../../components/pca9420/fsl_pca9420.c"
"${ProjDirPath}/../../../../../components/pca9420/fsl_pca9420.h"
"${ProjDirPath}/../../../../../middleware/mflash/mimxrt685/mflash_drv.c"
"${ProjDirPath}/../../../../../middleware/mflash/mimxrt685/mflash_drv.h"
"${ProjDirPath}/../../../../../middleware/mflash/mimxrt685/mflash_file.c"
"${ProjDirPath}/../../../../../middleware/mflash/mimxrt685/mflash_file.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/apps/httpsrv/httpsrv.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/apps/httpsrv/httpsrv.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/apps/httpsrv/httpsrv_base64.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/apps/httpsrv/httpsrv_base64.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/apps/httpsrv/httpsrv_config.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/apps/httpsrv/httpsrv_fs.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/apps/httpsrv/httpsrv_fs.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/apps/httpsrv/httpsrv_port.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/apps/httpsrv/httpsrv_prv.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/apps/httpsrv/httpsrv_script.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/apps/httpsrv/httpsrv_script.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/apps/httpsrv/httpsrv_sha1.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/apps/httpsrv/httpsrv_sha1.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/apps/httpsrv/httpsrv_supp.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/apps/httpsrv/httpsrv_supp.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/apps/httpsrv/httpsrv_task.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/apps/httpsrv/httpsrv_tls.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/apps/httpsrv/httpsrv_tls.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/apps/httpsrv/httpsrv_utf8.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/apps/httpsrv/httpsrv_utf8.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/apps/httpsrv/httpsrv_ws.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/apps/httpsrv/httpsrv_ws.h"
"${ProjDirPath}/../../../../../middleware/lwip/src/apps/httpsrv/httpsrv_ws_api.c"
"${ProjDirPath}/../../../../../middleware/lwip/src/apps/httpsrv/httpsrv_ws_prv.h"
"${ProjDirPath}/../../../../../devices/MIMXRT685S/drivers/fsl_clock.c"
"${ProjDirPath}/../../../../../devices/MIMXRT685S/drivers/fsl_clock.h"
"${ProjDirPath}/../../../../../rtos/freertos/freertos_kernel/portable/MemMang/heap_3.c"
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
"${ProjDirPath}/../../../../../middleware/lwip/src/core/ipv4/acd.c"
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
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/acd.h"
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
"${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip/prot/acd.h"
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
"${ProjDirPath}/../../../../../devices/MIMXRT685S/utilities/debug_console/fsl_debug_console.c"
"${ProjDirPath}/../../../../../devices/MIMXRT685S/utilities/debug_console/fsl_debug_console.h"
"${ProjDirPath}/../../../../../devices/MIMXRT685S/utilities/debug_console/fsl_debug_console_conf.h"
"${ProjDirPath}/../../../../../devices/MIMXRT685S/utilities/str/fsl_str.c"
"${ProjDirPath}/../../../../../devices/MIMXRT685S/utilities/str/fsl_str.h"
"${ProjDirPath}/../../../../../middleware/wifi/dhcpd/dhcp-bootp.h"
"${ProjDirPath}/../../../../../middleware/wifi/dhcpd/dhcp-priv.h"
"${ProjDirPath}/../../../../../middleware/wifi/dhcpd/dhcp-server-cli.c"
"${ProjDirPath}/../../../../../middleware/wifi/dhcpd/dhcp-server-main.c"
"${ProjDirPath}/../../../../../middleware/wifi/dhcpd/dhcp-server.c"
"${ProjDirPath}/../../../../../middleware/wifi/dhcpd/dns-server.c"
"${ProjDirPath}/../../../../../middleware/wifi/dhcpd/dns.h"
"${ProjDirPath}/../../../../../middleware/wifi/incl/dhcp-server.h"
"${ProjDirPath}/../../../../../middleware/wifi/incl/port/lwip/wm_net.h"
"${ProjDirPath}/../../../../../middleware/wifi/incl/port/os/wm_os.h"
"${ProjDirPath}/../../../../../middleware/wifi/incl/wifidriver/mdev.h"
"${ProjDirPath}/../../../../../middleware/wifi/incl/wifidriver/mdev_sdio.h"
"${ProjDirPath}/../../../../../middleware/wifi/incl/wifidriver/wifi-decl.h"
"${ProjDirPath}/../../../../../middleware/wifi/incl/wifidriver/wifi.h"
"${ProjDirPath}/../../../../../middleware/wifi/incl/wifidriver/wifi_events.h"
"${ProjDirPath}/../../../../../middleware/wifi/incl/wlcmgr/wlan.h"
"${ProjDirPath}/../../../../../middleware/wifi/incl/wlcmgr/wlan_11d.h"
"${ProjDirPath}/../../../../../middleware/wifi/incl/wm_utils.h"
"${ProjDirPath}/../../../../../middleware/wifi/incl/wmerrno.h"
"${ProjDirPath}/../../../../../middleware/wifi/incl/wmlog.h"
"${ProjDirPath}/../../../../../middleware/wifi/incl/wmtypes.h"
"${ProjDirPath}/../../../../../middleware/wifi/port/lwip/net.c"
"${ProjDirPath}/../../../../../middleware/wifi/port/lwip/netif_decl.h"
"${ProjDirPath}/../../../../../middleware/wifi/port/lwip/wifi_netif.c"
"${ProjDirPath}/../../../../../middleware/wifi/port/os/os.c"
"${ProjDirPath}/../../../../../middleware/wifi/wifi_bt_firmware/sd8801_wlan.h"
"${ProjDirPath}/../../../../../middleware/wifi/wifi_bt_firmware/sduart8977_wlan_bt.h"
"${ProjDirPath}/../../../../../middleware/wifi/wifidriver/incl/mlan.h"
"${ProjDirPath}/../../../../../middleware/wifi/wifidriver/incl/mlan_11ac.h"
"${ProjDirPath}/../../../../../middleware/wifi/wifidriver/incl/mlan_11h.h"
"${ProjDirPath}/../../../../../middleware/wifi/wifidriver/incl/mlan_11n.h"
"${ProjDirPath}/../../../../../middleware/wifi/wifidriver/incl/mlan_11n_aggr.h"
"${ProjDirPath}/../../../../../middleware/wifi/wifidriver/incl/mlan_11n_rxreorder.h"
"${ProjDirPath}/../../../../../middleware/wifi/wifidriver/incl/mlan_decl.h"
"${ProjDirPath}/../../../../../middleware/wifi/wifidriver/incl/mlan_fw.h"
"${ProjDirPath}/../../../../../middleware/wifi/wifidriver/incl/mlan_ieee.h"
"${ProjDirPath}/../../../../../middleware/wifi/wifidriver/incl/mlan_init.h"
"${ProjDirPath}/../../../../../middleware/wifi/wifidriver/incl/mlan_ioctl.h"
"${ProjDirPath}/../../../../../middleware/wifi/wifidriver/incl/mlan_join.h"
"${ProjDirPath}/../../../../../middleware/wifi/wifidriver/incl/mlan_main.h"
"${ProjDirPath}/../../../../../middleware/wifi/wifidriver/incl/mlan_meas.h"
"${ProjDirPath}/../../../../../middleware/wifi/wifidriver/incl/mlan_remap_mem_operations.h"
"${ProjDirPath}/../../../../../middleware/wifi/wifidriver/incl/mlan_sdio.h"
"${ProjDirPath}/../../../../../middleware/wifi/wifidriver/incl/mlan_uap.h"
"${ProjDirPath}/../../../../../middleware/wifi/wifidriver/incl/mlan_util.h"
"${ProjDirPath}/../../../../../middleware/wifi/wifidriver/incl/mlan_wmm.h"
"${ProjDirPath}/../../../../../middleware/wifi/wifidriver/incl/mlan_wmsdk.h"
"${ProjDirPath}/../../../../../middleware/wifi/wifidriver/mdev_sdio.c"
"${ProjDirPath}/../../../../../middleware/wifi/wifidriver/mlan_11ac.c"
"${ProjDirPath}/../../../../../middleware/wifi/wifidriver/mlan_11d.c"
"${ProjDirPath}/../../../../../middleware/wifi/wifidriver/mlan_11h.c"
"${ProjDirPath}/../../../../../middleware/wifi/wifidriver/mlan_11n.c"
"${ProjDirPath}/../../../../../middleware/wifi/wifidriver/mlan_11n_aggr.c"
"${ProjDirPath}/../../../../../middleware/wifi/wifidriver/mlan_11n_rxreorder.c"
"${ProjDirPath}/../../../../../middleware/wifi/wifidriver/mlan_cfp.c"
"${ProjDirPath}/../../../../../middleware/wifi/wifidriver/mlan_cmdevt.c"
"${ProjDirPath}/../../../../../middleware/wifi/wifidriver/mlan_glue.c"
"${ProjDirPath}/../../../../../middleware/wifi/wifidriver/mlan_init.c"
"${ProjDirPath}/../../../../../middleware/wifi/wifidriver/mlan_join.c"
"${ProjDirPath}/../../../../../middleware/wifi/wifidriver/mlan_misc.c"
"${ProjDirPath}/../../../../../middleware/wifi/wifidriver/mlan_scan.c"
"${ProjDirPath}/../../../../../middleware/wifi/wifidriver/mlan_shim.c"
"${ProjDirPath}/../../../../../middleware/wifi/wifidriver/mlan_sta_cmd.c"
"${ProjDirPath}/../../../../../middleware/wifi/wifidriver/mlan_sta_cmdresp.c"
"${ProjDirPath}/../../../../../middleware/wifi/wifidriver/mlan_sta_event.c"
"${ProjDirPath}/../../../../../middleware/wifi/wifidriver/mlan_sta_ioctl.c"
"${ProjDirPath}/../../../../../middleware/wifi/wifidriver/mlan_sta_rx.c"
"${ProjDirPath}/../../../../../middleware/wifi/wifidriver/mlan_txrx.c"
"${ProjDirPath}/../../../../../middleware/wifi/wifidriver/mlan_uap_cmdevent.c"
"${ProjDirPath}/../../../../../middleware/wifi/wifidriver/mlan_uap_ioctl.c"
"${ProjDirPath}/../../../../../middleware/wifi/wifidriver/mlan_wmm.c"
"${ProjDirPath}/../../../../../middleware/wifi/wifidriver/mlan_wmsdk.c"
"${ProjDirPath}/../../../../../middleware/wifi/wifidriver/wifi-debug.c"
"${ProjDirPath}/../../../../../middleware/wifi/wifidriver/wifi-debug.h"
"${ProjDirPath}/../../../../../middleware/wifi/wifidriver/wifi-internal.h"
"${ProjDirPath}/../../../../../middleware/wifi/wifidriver/wifi-mem.c"
"${ProjDirPath}/../../../../../middleware/wifi/wifidriver/wifi-sdio.c"
"${ProjDirPath}/../../../../../middleware/wifi/wifidriver/wifi-sdio.h"
"${ProjDirPath}/../../../../../middleware/wifi/wifidriver/wifi-uap.c"
"${ProjDirPath}/../../../../../middleware/wifi/wifidriver/wifi.c"
"${ProjDirPath}/../../../../../middleware/wifi/wifidriver/wifi_common.h"
"${ProjDirPath}/../../../../../middleware/wifi/wifidriver/wifi_pwrmgr.c"
"${ProjDirPath}/../../../../../middleware/wifi/wlcmgr/wlan.c"
"${ProjDirPath}/../../../../../middleware/wifi/wlcmgr/wlan_txpwrlimit_cfg.c"
"${ProjDirPath}/../../../../../devices/MIMXRT685S/drivers/fsl_usdhc.c"
"${ProjDirPath}/../../../../../devices/MIMXRT685S/drivers/fsl_usdhc.h"
"${ProjDirPath}/../../../../../middleware/sdmmc/sdio/fsl_sdio.c"
"${ProjDirPath}/../../../../../middleware/sdmmc/sdio/fsl_sdio.h"
"${ProjDirPath}/../../../../../middleware/sdmmc/common/fsl_sdmmc_common.c"
"${ProjDirPath}/../../../../../middleware/sdmmc/common/fsl_sdmmc_common.h"
"${ProjDirPath}/../../../../../middleware/sdmmc/common/fsl_sdmmc_spec.h"
"${ProjDirPath}/../../../../../middleware/sdmmc/host/usdhc/fsl_sdmmc_host.h"
"${ProjDirPath}/../../../../../middleware/sdmmc/host/usdhc/fsl_sdmmc_host.h"
"${ProjDirPath}/../../../../../middleware/sdmmc/host/usdhc/non_blocking/fsl_sdmmc_host.c"
"${ProjDirPath}/../../../../../devices/MIMXRT685S/drivers/fsl_common.c"
"${ProjDirPath}/../../../../../devices/MIMXRT685S/drivers/fsl_common.h"
"${ProjDirPath}/../../../../../devices/MIMXRT685S/drivers/fsl_power.c"
"${ProjDirPath}/../../../../../devices/MIMXRT685S/drivers/fsl_power.h"
"${ProjDirPath}/../../../../../devices/MIMXRT685S/drivers/fsl_reset.c"
"${ProjDirPath}/../../../../../devices/MIMXRT685S/drivers/fsl_reset.h"
"${ProjDirPath}/../../../../../devices/MIMXRT685S/MIMXRT685S_cm33.h"
"${ProjDirPath}/../../../../../devices/MIMXRT685S/MIMXRT685S_cm33_features.h"
"${ProjDirPath}/../../../../../devices/MIMXRT685S/fsl_device_registers.h"
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
"${ProjDirPath}/../../../../../rtos/freertos/freertos_kernel/croutine.c"
"${ProjDirPath}/../../../../../rtos/freertos/freertos_kernel/event_groups.c"
"${ProjDirPath}/../../../../../rtos/freertos/freertos_kernel/include/FreeRTOS.h"
"${ProjDirPath}/../../../../../rtos/freertos/freertos_kernel/include/StackMacros.h"
"${ProjDirPath}/../../../../../rtos/freertos/freertos_kernel/include/atomic.h"
"${ProjDirPath}/../../../../../rtos/freertos/freertos_kernel/include/croutine.h"
"${ProjDirPath}/../../../../../rtos/freertos/freertos_kernel/include/deprecated_definitions.h"
"${ProjDirPath}/../../../../../rtos/freertos/freertos_kernel/include/event_groups.h"
"${ProjDirPath}/../../../../../rtos/freertos/freertos_kernel/include/list.h"
"${ProjDirPath}/../../../../../rtos/freertos/freertos_kernel/include/message_buffer.h"
"${ProjDirPath}/../../../../../rtos/freertos/freertos_kernel/include/mpu_prototypes.h"
"${ProjDirPath}/../../../../../rtos/freertos/freertos_kernel/include/mpu_wrappers.h"
"${ProjDirPath}/../../../../../rtos/freertos/freertos_kernel/include/portable.h"
"${ProjDirPath}/../../../../../rtos/freertos/freertos_kernel/include/projdefs.h"
"${ProjDirPath}/../../../../../rtos/freertos/freertos_kernel/include/queue.h"
"${ProjDirPath}/../../../../../rtos/freertos/freertos_kernel/include/semphr.h"
"${ProjDirPath}/../../../../../rtos/freertos/freertos_kernel/include/stack_macros.h"
"${ProjDirPath}/../../../../../rtos/freertos/freertos_kernel/include/stream_buffer.h"
"${ProjDirPath}/../../../../../rtos/freertos/freertos_kernel/include/task.h"
"${ProjDirPath}/../../../../../rtos/freertos/freertos_kernel/include/timers.h"
"${ProjDirPath}/../../../../../rtos/freertos/freertos_kernel/list.c"
"${ProjDirPath}/../../../../../rtos/freertos/freertos_kernel/portable/MemMang/ReadMe.url"
"${ProjDirPath}/../../../../../rtos/freertos/freertos_kernel/queue.c"
"${ProjDirPath}/../../../../../rtos/freertos/freertos_kernel/stream_buffer.c"
"${ProjDirPath}/../../../../../rtos/freertos/freertos_kernel/tasks.c"
"${ProjDirPath}/../../../../../rtos/freertos/freertos_kernel/timers.c"
"${ProjDirPath}/../../../../../rtos/freertos/freertos_kernel/include/freertos_tasks_c_additions.h"
"${ProjDirPath}/../../../../../CMSIS/Include/core_cm33.h"
"${ProjDirPath}/../../../../../CMSIS/Include/mpu_armv8.h"
"${ProjDirPath}/../../../../../CMSIS/Include/tz_context.h"
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
"${ProjDirPath}/../../../../../middleware/sdmmc/osa/fsl_sdmmc_osa.c"
"${ProjDirPath}/../../../../../middleware/sdmmc/osa/fsl_sdmmc_osa.h"
"${ProjDirPath}/../../../../../components/osa/fsl_os_abstraction.h"
"${ProjDirPath}/../../../../../components/osa/fsl_os_abstraction_config.h"
"${ProjDirPath}/../../../../../components/osa/fsl_os_abstraction_free_rtos.c"
"${ProjDirPath}/../../../../../components/osa/fsl_os_abstraction_free_rtos.h"
"${ProjDirPath}/../../../../../devices/MIMXRT685S/utilities/fsl_sbrk.c"
)


set(CMAKE_EXE_LINKER_FLAGS_FLASH_DEBUG "${CMAKE_EXE_LINKER_FLAGS_FLASH_DEBUG} -T${ProjDirPath}/linker/MIMXRT685Sxxxx_cm33_flash.ld -static")

set(CMAKE_EXE_LINKER_FLAGS_FLASH_RELEASE "${CMAKE_EXE_LINKER_FLAGS_FLASH_RELEASE} -T${ProjDirPath}/linker/MIMXRT685Sxxxx_cm33_flash.ld -static")

TARGET_LINK_LIBRARIES(wifi_webconfig.elf -Wl,--start-group)
target_link_libraries(wifi_webconfig.elf optimized m)

target_link_libraries(wifi_webconfig.elf optimized c)

target_link_libraries(wifi_webconfig.elf optimized gcc)

target_link_libraries(wifi_webconfig.elf optimized nosys)

target_link_libraries(wifi_webconfig.elf optimized m)

target_link_libraries(wifi_webconfig.elf optimized c)

target_link_libraries(wifi_webconfig.elf optimized gcc)

target_link_libraries(wifi_webconfig.elf optimized nosys)

TARGET_LINK_LIBRARIES(wifi_webconfig.elf -Wl,--end-group)

ADD_CUSTOM_COMMAND(TARGET wifi_webconfig.elf POST_BUILD COMMAND ${CMAKE_OBJCOPY}
-Obinary ${EXECUTABLE_OUTPUT_PATH}/wifi_webconfig.elf ${EXECUTABLE_OUTPUT_PATH}/sdk20-app.bin)

