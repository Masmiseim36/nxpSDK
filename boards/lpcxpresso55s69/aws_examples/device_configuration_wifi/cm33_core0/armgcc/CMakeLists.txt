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

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DFNET_CFG_COMP_GNUC=1")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DCPU_LPC55S69JBD100_cm33_core0")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DUSE_RTOS=1")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DMBEDTLS_CONFIG_FILE='\"aws_mbedtls_config.h\"'")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DFNET_PORT_WIFI_QCA=1")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEMO_DEVICE_CONFIGURATION_WIFI")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DA_LITTLE_ENDIAN")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DSDK_DEBUGCONSOLE_UART")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DFSL_RTOS_FREE_RTOS")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DSERIAL_PORT_TYPE_UART=1")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -fomit-frame-pointer")

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

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DFNET_CFG_COMP_GNUC=1")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DCPU_LPC55S69JBD100_cm33_core0")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DUSE_RTOS=1")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DMBEDTLS_CONFIG_FILE='\"aws_mbedtls_config.h\"'")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DFNET_PORT_WIFI_QCA=1")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DDEMO_DEVICE_CONFIGURATION_WIFI")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DA_LITTLE_ENDIAN")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DSDK_DEBUGCONSOLE_UART")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DFSL_RTOS_FREE_RTOS")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DSERIAL_PORT_TYPE_UART=1")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Os")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -fomit-frame-pointer")

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

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DCPU_LPC55S69JBD100_cm33_core0")

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

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DCPU_LPC55S69JBD100_cm33_core0")

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

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -g")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -mcpu=cortex-m33")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -Wall")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -mfloat-abi=hard")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -mfpu=fpv5-sp-d16")

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

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} --defsym=__stack_size__=0x200")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} --defsym=__heap_size__=0x200")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -mcpu=cortex-m33")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -Wall")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -mfloat-abi=hard")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -mfpu=fpv5-sp-d16")

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

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} --defsym=__stack_size__=0x200")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} --defsym=__heap_size__=0x200")

include_directories(${ProjDirPath}/../config_files)

include_directories(${ProjDirPath}/../config_files/cm33_core0)

include_directories(${ProjDirPath}/../../../..)

include_directories(${ProjDirPath}/../..)

include_directories(${ProjDirPath}/..)

include_directories(${ProjDirPath}/../config_files)

include_directories(${ProjDirPath}/../config_files/cm33_core0)

include_directories(${ProjDirPath}/../../../..)

include_directories(${ProjDirPath}/../..)

include_directories(${ProjDirPath}/..)

include_directories(${ProjDirPath}/../../../../../../rtos/freertos/freertos_kernel/portable/GCC/ARM_CM33_NTZ/non_secure)

include_directories(${ProjDirPath}/..)

include_directories(${ProjDirPath}/../config_files)

include_directories(${ProjDirPath}/../config_files/cm33_core0)

include_directories(${ProjDirPath}/../cm33_core0)

include_directories(${ProjDirPath}/../ssl_server/cm33_core0)

include_directories(${ProjDirPath}/../fnet_mdns/cm33_core0)

include_directories(${ProjDirPath}/../../../../cm33_core0)

include_directories(${ProjDirPath}/../ssl_server)

include_directories(${ProjDirPath}/../fnet_mdns)

include_directories(${ProjDirPath}/../..)

include_directories(${ProjDirPath}/../../../..)

include_directories(${ProjDirPath}/../../../../../../CMSIS/Include)

include_directories(${ProjDirPath}/../../../../../../devices)

include_directories(${ProjDirPath}/../../../../../../devices/LPC55S69)

include_directories(${ProjDirPath}/../../../../../../rtos/freertos/demos/include)

include_directories(${ProjDirPath}/../../../../../../rtos/freertos/demos/network_manager)

include_directories(${ProjDirPath}/../../../../../../rtos/freertos/libraries/abstractions/wifi/include)

include_directories(${ProjDirPath}/../../../../../../rtos/freertos/freertos_kernel/include)

include_directories(${ProjDirPath}/../../../../../../rtos/freertos/libraries/abstractions/platform/freertos/include)

include_directories(${ProjDirPath}/../../../../../../rtos/freertos/libraries/abstractions/platform/include)

include_directories(${ProjDirPath}/../../../../../../rtos/freertos/libraries/c_sdk/aws/shadow/include)

include_directories(${ProjDirPath}/../../../../../../rtos/freertos/libraries/c_sdk/aws/shadow/src)

include_directories(${ProjDirPath}/../../../../../../rtos/freertos/libraries/c_sdk/standard/common/include)

include_directories(${ProjDirPath}/../../../../../../rtos/freertos/libraries/c_sdk/standard/common/include/private)

include_directories(${ProjDirPath}/../../../../../../rtos/freertos/libraries/c_sdk/standard/mqtt/include)

include_directories(${ProjDirPath}/../../../../../../rtos/freertos/libraries/c_sdk/standard/mqtt/src)

include_directories(${ProjDirPath}/../../../../../../rtos/freertos/libraries/c_sdk/standard/serializer/include)

include_directories(${ProjDirPath}/../../../../../../rtos/freertos/libraries/abstractions/secure_sockets/include)

include_directories(${ProjDirPath}/../../../../../../rtos/freertos/libraries/freertos_plus/standard/crypto/include)

include_directories(${ProjDirPath}/../../../../../../rtos/freertos/libraries/freertos_plus/standard/pkcs11/include)

include_directories(${ProjDirPath}/../../../../../../rtos/freertos/libraries/freertos_plus/standard/utils/include)

include_directories(${ProjDirPath}/../../../../../../rtos/freertos/libraries/freertos_plus/standard/tls/include)

include_directories(${ProjDirPath}/../../../../../../rtos/freertos/libraries/abstractions/pkcs11/include)

include_directories(${ProjDirPath}/../../../../../../rtos/freertos/libraries/abstractions/pkcs11/mbedtls)

include_directories(${ProjDirPath}/../../../../../../rtos/freertos/libraries/3rdparty/pkcs11)

include_directories(${ProjDirPath}/../../../../../../rtos/freertos/libraries/3rdparty/jsmn)

include_directories(${ProjDirPath}/../../../../../../middleware/mbedtls/include)

include_directories(${ProjDirPath}/../../../../../../middleware/mbedtls/port/ksdk)

include_directories(${ProjDirPath}/../../../../../../middleware/wifi_qca/port/boards/lpcxpresso55s69/freertos/gt202)

include_directories(${ProjDirPath}/../../../../../../middleware/wifi_qca/port/boards/lpcxpresso55s69/freertos/wifi10click)

include_directories(${ProjDirPath}/../../../../../../middleware/wifi_qca/port/boards/lpcxpresso55s69/freertos/silex2401)

include_directories(${ProjDirPath}/../../../../../../devices/LPC55S69/drivers)

include_directories(${ProjDirPath}/../../../../../../middleware/mflash/lpc55xxx)

include_directories(${ProjDirPath}/../../../../../../middleware/wifi_qca)

include_directories(${ProjDirPath}/../../../../../../middleware/wifi_qca/include)

include_directories(${ProjDirPath}/../../../../../../middleware/wifi_qca/include/AR6002)

include_directories(${ProjDirPath}/../../../../../../middleware/wifi_qca/include/AR6002/hw2.0/hw)

include_directories(${ProjDirPath}/../../../../../../middleware/wifi_qca/include/AR6002/hw4.0/hw)

include_directories(${ProjDirPath}/../../../../../../middleware/wifi_qca/custom_src/stack_custom)

include_directories(${ProjDirPath}/../../../../../../middleware/wifi_qca/custom_src/include)

include_directories(${ProjDirPath}/../../../../../../middleware/wifi_qca/common_src/include)

include_directories(${ProjDirPath}/../../../../../../middleware/wifi_qca/common_src/stack_common)

include_directories(${ProjDirPath}/../../../../../../middleware/wifi_qca/common_src/hcd)

include_directories(${ProjDirPath}/../../../../../../middleware/wifi_qca/common_src/wmi)

include_directories(${ProjDirPath}/../../../../../../middleware/wifi_qca/port)

include_directories(${ProjDirPath}/../../../../../../devices/LPC55S69/utilities/str)

include_directories(${ProjDirPath}/../../../../../../devices/LPC55S69/utilities/debug_console)

include_directories(${ProjDirPath}/../../../../../../components/uart)

include_directories(${ProjDirPath}/../../../../../../components/serial_manager)

include_directories(${ProjDirPath}/../../../../../../components/lists)

include_directories(${ProjDirPath}/../../../../../../middleware/wifi_qca/port/boards/lpcxpresso55s69/freertos)

include_directories(${ProjDirPath}/../../../../../../middleware/wifi_qca/port/env/freertos)

include_directories(${ProjDirPath}/../../../../../../middleware/wifi_qca/port/drivers/flexcomm_freertos)

include_directories(${ProjDirPath}/../../../../../../devices/LPC55S69/utilities)

add_executable(aws_device_configuration_wifi.elf 
"${ProjDirPath}/../aws_mbedtls_config.h"
"${ProjDirPath}/../config_files/FreeRTOSConfig.h"
"${ProjDirPath}/../config_files/aws_bufferpool_config.h"
"${ProjDirPath}/../config_files/aws_demo_config.h"
"${ProjDirPath}/../config_files/aws_mqtt_config.h"
"${ProjDirPath}/../config_files/aws_secure_sockets_config.h"
"${ProjDirPath}/../config_files/aws_shadow_config.h"
"${ProjDirPath}/../config_files/aws_wifi_config.h"
"${ProjDirPath}/../config_files/FreeRTOSIPConfig.h"
"${ProjDirPath}/../config_files/aws_iot_network_config.h"
"${ProjDirPath}/../config_files/iot_config.h"
"${ProjDirPath}/../config_files/iot_mqtt_agent_config.h"
"${ProjDirPath}/../config_files/iot_pkcs11_config.h"
"${ProjDirPath}/../ssl_server/sslsrv.c"
"${ProjDirPath}/../ssl_server/sslsrv.h"
"${ProjDirPath}/../ssl_server/sslsrv_config.h"
"${ProjDirPath}/../ssl_server/sslsrv_keys.h"
"${ProjDirPath}/../ssl_server/sslsrv_port.h"
"${ProjDirPath}/../ssl_server/sslsrv_prv.h"
"${ProjDirPath}/../ssl_server/sslsrv_supp.c"
"${ProjDirPath}/../ssl_server/sslsrv_supp.h"
"${ProjDirPath}/../ssl_server/sslsrv_task.c"
"${ProjDirPath}/../ssl_server/sslsrv_tls.c"
"${ProjDirPath}/../ssl_server/sslsrv_tls.h"
"${ProjDirPath}/../ssl_server/sslsrv_port_wifi.c"
"${ProjDirPath}/../fnet_mdns/port_wifi.c"
"${ProjDirPath}/../fnet_mdns/fnet_comp.h"
"${ProjDirPath}/../fnet_mdns/fnet_mdns.c"
"${ProjDirPath}/../fnet_mdns/fnet_mdns.h"
"${ProjDirPath}/../fnet_mdns/fnet_mdns_config.h"
"${ProjDirPath}/../fnet_mdns/fnet_mdns_port.h"
"${ProjDirPath}/../fnet_mdns/fnet_types.h"
"${ProjDirPath}/../fnet_mdns/fnet_user_config.h"
"${ProjDirPath}/../aws_clientcredential.h"
"${ProjDirPath}/../device_configuration.c"
"${ProjDirPath}/../device_configuration.h"
"${ProjDirPath}/../aws_shadow_lightbulb_on_off.c"
"${ProjDirPath}/../main_wifi.c"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
"${ProjDirPath}/../../../../../../middleware/wifi_qca/port/boards/lpcxpresso55s69/freertos/gt202/wifi_shield_gt202.h"
"${ProjDirPath}/../../../../../../middleware/wifi_qca/port/boards/lpcxpresso55s69/freertos/wifi10click/wifi_shield_wifi10click.h"
"${ProjDirPath}/../../../../../../middleware/wifi_qca/port/boards/lpcxpresso55s69/freertos/silex2401/wifi_shield_silex2401.h"
"${ProjDirPath}/../../../../../../devices/LPC55S69/drivers/fsl_casper.c"
"${ProjDirPath}/../../../../../../devices/LPC55S69/drivers/fsl_casper.h"
"${ProjDirPath}/../../../../../../devices/LPC55S69/drivers/fsl_hashcrypt.c"
"${ProjDirPath}/../../../../../../devices/LPC55S69/drivers/fsl_hashcrypt.h"
"${ProjDirPath}/../../../../../../devices/LPC55S69/drivers/fsl_rng.c"
"${ProjDirPath}/../../../../../../devices/LPC55S69/drivers/fsl_rng.h"
"${ProjDirPath}/../../../../../../middleware/mflash/lpc55xxx/mflash_drv.c"
"${ProjDirPath}/../../../../../../middleware/mflash/lpc55xxx/mflash_drv.h"
"${ProjDirPath}/../../../../../../middleware/mflash/lpc55xxx/mflash_file.c"
"${ProjDirPath}/../../../../../../middleware/mflash/lpc55xxx/mflash_file.h"
"${ProjDirPath}/../../../../../../devices/LPC55S69/drivers/fsl_iap.c"
"${ProjDirPath}/../../../../../../devices/LPC55S69/drivers/fsl_iap.h"
"${ProjDirPath}/../../../../../../devices/LPC55S69/drivers/fsl_iap_ffr.h"
"${ProjDirPath}/../../../../../../devices/LPC55S69/drivers/fsl_iap_kbp.h"
"${ProjDirPath}/../../../../../../devices/LPC55S69/drivers/fsl_iap_skboot_authenticate.h"
"${ProjDirPath}/../../../../../../rtos/freertos/freertos_kernel/portable/GCC/ARM_CM33_NTZ/non_secure/port.c"
"${ProjDirPath}/../../../../../../rtos/freertos/freertos_kernel/portable/GCC/ARM_CM33_NTZ/non_secure/portasm.c"
"${ProjDirPath}/../../../../../../rtos/freertos/freertos_kernel/portable/GCC/ARM_CM33_NTZ/non_secure/portasm.h"
"${ProjDirPath}/../../../../../../rtos/freertos/freertos_kernel/portable/GCC/ARM_CM33_NTZ/non_secure/portmacro.h"
"${ProjDirPath}/../../../../../../rtos/freertos/vendors/nxp/secure_sockets/wifi_qca/iot_secure_sockets.c"
"${ProjDirPath}/../../../../../../rtos/freertos/vendors/nxp/wifi/qca_400x/iot_wifi.c"
"${ProjDirPath}/../../../../../../middleware/wifi_qca/atheros_stack_offload.h"
"${ProjDirPath}/../../../../../../middleware/wifi_qca/atheros_wifi.h"
"${ProjDirPath}/../../../../../../middleware/wifi_qca/atheros_wifi_api.h"
"${ProjDirPath}/../../../../../../middleware/wifi_qca/atheros_wifi_internal.h"
"${ProjDirPath}/../../../../../../middleware/wifi_qca/common_src/api_interface/api_init.c"
"${ProjDirPath}/../../../../../../middleware/wifi_qca/common_src/api_interface/api_ioctl.c"
"${ProjDirPath}/../../../../../../middleware/wifi_qca/common_src/api_interface/api_txrx.c"
"${ProjDirPath}/../../../../../../middleware/wifi_qca/common_src/api_interface/api_wmi_rx.c"
"${ProjDirPath}/../../../../../../middleware/wifi_qca/common_src/bmi/bmi.c"
"${ProjDirPath}/../../../../../../middleware/wifi_qca/common_src/driver/driver_diag.c"
"${ProjDirPath}/../../../../../../middleware/wifi_qca/common_src/driver/driver_init.c"
"${ProjDirPath}/../../../../../../middleware/wifi_qca/common_src/driver/driver_main.c"
"${ProjDirPath}/../../../../../../middleware/wifi_qca/common_src/driver/driver_netbuf.c"
"${ProjDirPath}/../../../../../../middleware/wifi_qca/common_src/driver/driver_txrx.c"
"${ProjDirPath}/../../../../../../middleware/wifi_qca/common_src/hcd/hcd_api.h"
"${ProjDirPath}/../../../../../../middleware/wifi_qca/common_src/hcd/spi_hcd.c"
"${ProjDirPath}/../../../../../../middleware/wifi_qca/common_src/htc/htc.c"
"${ProjDirPath}/../../../../../../middleware/wifi_qca/common_src/hw_interface/hw_api.c"
"${ProjDirPath}/../../../../../../middleware/wifi_qca/common_src/include/a_debug.h"
"${ProjDirPath}/../../../../../../middleware/wifi_qca/common_src/include/a_drv.h"
"${ProjDirPath}/../../../../../../middleware/wifi_qca/common_src/include/a_drv_api.h"
"${ProjDirPath}/../../../../../../middleware/wifi_qca/common_src/include/aggr_recv_api.h"
"${ProjDirPath}/../../../../../../middleware/wifi_qca/common_src/include/bmi.h"
"${ProjDirPath}/../../../../../../middleware/wifi_qca/common_src/include/common_api.h"
"${ProjDirPath}/../../../../../../middleware/wifi_qca/common_src/include/dbglog_api.h"
"${ProjDirPath}/../../../../../../middleware/wifi_qca/common_src/include/driver_cxt.h"
"${ProjDirPath}/../../../../../../middleware/wifi_qca/common_src/include/dset.h"
"${ProjDirPath}/../../../../../../middleware/wifi_qca/common_src/include/dset_api.h"
"${ProjDirPath}/../../../../../../middleware/wifi_qca/common_src/include/hif_internal.h"
"${ProjDirPath}/../../../../../../middleware/wifi_qca/common_src/include/htc_api.h"
"${ProjDirPath}/../../../../../../middleware/wifi_qca/common_src/include/ieee80211.h"
"${ProjDirPath}/../../../../../../middleware/wifi_qca/common_src/include/netbuf.h"
"${ProjDirPath}/../../../../../../middleware/wifi_qca/common_src/include/spi_hcd_if.h"
"${ProjDirPath}/../../../../../../middleware/wifi_qca/common_src/include/wlan_api.h"
"${ProjDirPath}/../../../../../../middleware/wifi_qca/common_src/include/wmi_api.h"
"${ProjDirPath}/../../../../../../middleware/wifi_qca/common_src/qapi/qcom_api.c"
"${ProjDirPath}/../../../../../../middleware/wifi_qca/common_src/qapi/qcom_dns.c"
"${ProjDirPath}/../../../../../../middleware/wifi_qca/common_src/qapi/qcom_legacy.c"
"${ProjDirPath}/../../../../../../middleware/wifi_qca/common_src/reorder/rcv_aggr.c"
"${ProjDirPath}/../../../../../../middleware/wifi_qca/common_src/stack_common/api_stack_offload.c"
"${ProjDirPath}/../../../../../../middleware/wifi_qca/common_src/stack_common/common_stack_offload.h"
"${ProjDirPath}/../../../../../../middleware/wifi_qca/common_src/storerecall/dset.c"
"${ProjDirPath}/../../../../../../middleware/wifi_qca/common_src/storerecall/dset_api.c"
"${ProjDirPath}/../../../../../../middleware/wifi_qca/common_src/storerecall/storerecall.c"
"${ProjDirPath}/../../../../../../middleware/wifi_qca/common_src/util/util.c"
"${ProjDirPath}/../../../../../../middleware/wifi_qca/common_src/wmi/wmi.c"
"${ProjDirPath}/../../../../../../middleware/wifi_qca/common_src/wmi/wmi_host.h"
"${ProjDirPath}/../../../../../../middleware/wifi_qca/custom_src/api_interface/cust_api_init.c"
"${ProjDirPath}/../../../../../../middleware/wifi_qca/custom_src/api_interface/cust_api_ioctl.c"
"${ProjDirPath}/../../../../../../middleware/wifi_qca/custom_src/api_interface/cust_api_txrx.c"
"${ProjDirPath}/../../../../../../middleware/wifi_qca/custom_src/api_interface/cust_api_wmi_rx.c"
"${ProjDirPath}/../../../../../../middleware/wifi_qca/custom_src/driver/cust_driver_main.c"
"${ProjDirPath}/../../../../../../middleware/wifi_qca/custom_src/driver/cust_driver_netbuf.c"
"${ProjDirPath}/../../../../../../middleware/wifi_qca/custom_src/hw_interface/cust_spi_hcd.c"
"${ProjDirPath}/../../../../../../middleware/wifi_qca/custom_src/include/a_config.h"
"${ProjDirPath}/../../../../../../middleware/wifi_qca/custom_src/include/a_osapi.h"
"${ProjDirPath}/../../../../../../middleware/wifi_qca/custom_src/include/a_types.h"
"${ProjDirPath}/../../../../../../middleware/wifi_qca/custom_src/include/cust_netbuf.h"
"${ProjDirPath}/../../../../../../middleware/wifi_qca/custom_src/include/custom_wlan_api.h"
"${ProjDirPath}/../../../../../../middleware/wifi_qca/custom_src/include/wlan_config.h"
"${ProjDirPath}/../../../../../../middleware/wifi_qca/custom_src/qapi/custom_qcom_api.c"
"${ProjDirPath}/../../../../../../middleware/wifi_qca/custom_src/stack_custom/cust_api_stack_offload.c"
"${ProjDirPath}/../../../../../../middleware/wifi_qca/custom_src/stack_custom/cust_api_stack_txrx.c"
"${ProjDirPath}/../../../../../../middleware/wifi_qca/custom_src/stack_custom/custom_stack_offload.h"
"${ProjDirPath}/../../../../../../middleware/wifi_qca/custom_src/stack_custom/qca_structs.h"
"${ProjDirPath}/../../../../../../middleware/wifi_qca/include/AR6002/AR6K_version.h"
"${ProjDirPath}/../../../../../../middleware/wifi_qca/include/AR6002/hw2.0/hw/hw20_apb_map.h"
"${ProjDirPath}/../../../../../../middleware/wifi_qca/include/AR6002/hw2.0/hw/hw20_gpio_reg.h"
"${ProjDirPath}/../../../../../../middleware/wifi_qca/include/AR6002/hw2.0/hw/hw20_mbox_host_reg.h"
"${ProjDirPath}/../../../../../../middleware/wifi_qca/include/AR6002/hw2.0/hw/hw20_mbox_reg.h"
"${ProjDirPath}/../../../../../../middleware/wifi_qca/include/AR6002/hw2.0/hw/hw20_rtc_reg.h"
"${ProjDirPath}/../../../../../../middleware/wifi_qca/include/AR6002/hw2.0/hw/hw20_si_reg.h"
"${ProjDirPath}/../../../../../../middleware/wifi_qca/include/AR6002/hw2.0/hw/hw20_uart_reg.h"
"${ProjDirPath}/../../../../../../middleware/wifi_qca/include/AR6002/hw2.0/hw/hw20_vmc_reg.h"
"${ProjDirPath}/../../../../../../middleware/wifi_qca/include/AR6002/hw4.0/hw/hw40_rtc_reg.h"
"${ProjDirPath}/../../../../../../middleware/wifi_qca/include/AR6002/hw4.0/hw/hw40_rtc_wlan_reg.h"
"${ProjDirPath}/../../../../../../middleware/wifi_qca/include/athdefs.h"
"${ProjDirPath}/../../../../../../middleware/wifi_qca/include/bmi_msg.h"
"${ProjDirPath}/../../../../../../middleware/wifi_qca/include/dbglog.h"
"${ProjDirPath}/../../../../../../middleware/wifi_qca/include/dbglog_id.h"
"${ProjDirPath}/../../../../../../middleware/wifi_qca/include/htc.h"
"${ProjDirPath}/../../../../../../middleware/wifi_qca/include/htc_services.h"
"${ProjDirPath}/../../../../../../middleware/wifi_qca/include/p2p.h"
"${ProjDirPath}/../../../../../../middleware/wifi_qca/include/pkt_log.h"
"${ProjDirPath}/../../../../../../middleware/wifi_qca/include/pmu.h"
"${ProjDirPath}/../../../../../../middleware/wifi_qca/include/targaddrs.h"
"${ProjDirPath}/../../../../../../middleware/wifi_qca/include/wlan_defs.h"
"${ProjDirPath}/../../../../../../middleware/wifi_qca/include/wmi.h"
"${ProjDirPath}/../../../../../../middleware/wifi_qca/port/wifi_common.h"
"${ProjDirPath}/../../../../../../middleware/wifi_qca/port/wifi_shield_common.h"
"${ProjDirPath}/../../../../../../middleware/wifi_qca/qcom_api.h"
"${ProjDirPath}/../../../../../../rtos/freertos/demos/demo_runner/aws_demo.c"
"${ProjDirPath}/../../../../../../rtos/freertos/demos/demo_runner/iot_demo_freertos.c"
"${ProjDirPath}/../../../../../../rtos/freertos/demos/include/aws_clientcredential.h"
"${ProjDirPath}/../../../../../../rtos/freertos/demos/include/aws_clientcredential_keys.h"
"${ProjDirPath}/../../../../../../rtos/freertos/demos/include/aws_demo.h"
"${ProjDirPath}/../../../../../../rtos/freertos/demos/include/aws_iot_demo_network.h"
"${ProjDirPath}/../../../../../../rtos/freertos/demos/include/aws_ota_codesigner_certificate.h"
"${ProjDirPath}/../../../../../../rtos/freertos/demos/include/aws_wifi_connect_task.h"
"${ProjDirPath}/../../../../../../rtos/freertos/demos/include/iot_ble_numericComparison.h"
"${ProjDirPath}/../../../../../../rtos/freertos/demos/include/iot_config_common.h"
"${ProjDirPath}/../../../../../../rtos/freertos/demos/include/iot_demo_logging.h"
"${ProjDirPath}/../../../../../../rtos/freertos/demos/include/iot_demo_runner.h"
"${ProjDirPath}/../../../../../../rtos/freertos/demos/network_manager/aws_iot_demo_network.c"
"${ProjDirPath}/../../../../../../rtos/freertos/demos/network_manager/aws_iot_network_manager.c"
"${ProjDirPath}/../../../../../../rtos/freertos/demos/network_manager/iot_network_manager_private.h"
"${ProjDirPath}/../../../../../../rtos/freertos/libraries/abstractions/wifi/include/iot_wifi.h"
"${ProjDirPath}/../../../../../../rtos/freertos/freertos_kernel/croutine.c"
"${ProjDirPath}/../../../../../../rtos/freertos/freertos_kernel/event_groups.c"
"${ProjDirPath}/../../../../../../rtos/freertos/freertos_kernel/include/FreeRTOS.h"
"${ProjDirPath}/../../../../../../rtos/freertos/freertos_kernel/include/StackMacros.h"
"${ProjDirPath}/../../../../../../rtos/freertos/freertos_kernel/include/atomic.h"
"${ProjDirPath}/../../../../../../rtos/freertos/freertos_kernel/include/croutine.h"
"${ProjDirPath}/../../../../../../rtos/freertos/freertos_kernel/include/deprecated_definitions.h"
"${ProjDirPath}/../../../../../../rtos/freertos/freertos_kernel/include/event_groups.h"
"${ProjDirPath}/../../../../../../rtos/freertos/freertos_kernel/include/list.h"
"${ProjDirPath}/../../../../../../rtos/freertos/freertos_kernel/include/message_buffer.h"
"${ProjDirPath}/../../../../../../rtos/freertos/freertos_kernel/include/mpu_prototypes.h"
"${ProjDirPath}/../../../../../../rtos/freertos/freertos_kernel/include/mpu_wrappers.h"
"${ProjDirPath}/../../../../../../rtos/freertos/freertos_kernel/include/portable.h"
"${ProjDirPath}/../../../../../../rtos/freertos/freertos_kernel/include/projdefs.h"
"${ProjDirPath}/../../../../../../rtos/freertos/freertos_kernel/include/queue.h"
"${ProjDirPath}/../../../../../../rtos/freertos/freertos_kernel/include/semphr.h"
"${ProjDirPath}/../../../../../../rtos/freertos/freertos_kernel/include/stack_macros.h"
"${ProjDirPath}/../../../../../../rtos/freertos/freertos_kernel/include/stream_buffer.h"
"${ProjDirPath}/../../../../../../rtos/freertos/freertos_kernel/include/task.h"
"${ProjDirPath}/../../../../../../rtos/freertos/freertos_kernel/include/timers.h"
"${ProjDirPath}/../../../../../../rtos/freertos/freertos_kernel/list.c"
"${ProjDirPath}/../../../../../../rtos/freertos/freertos_kernel/portable/MemMang/ReadMe.url"
"${ProjDirPath}/../../../../../../rtos/freertos/freertos_kernel/queue.c"
"${ProjDirPath}/../../../../../../rtos/freertos/freertos_kernel/stream_buffer.c"
"${ProjDirPath}/../../../../../../rtos/freertos/freertos_kernel/tasks.c"
"${ProjDirPath}/../../../../../../rtos/freertos/freertos_kernel/timers.c"
"${ProjDirPath}/../../../../../../rtos/freertos/freertos_kernel/portable/MemMang/heap_4.c"
"${ProjDirPath}/../../../../../../rtos/freertos/libraries/abstractions/platform/freertos/include/platform/iot_network_ble.h"
"${ProjDirPath}/../../../../../../rtos/freertos/libraries/abstractions/platform/freertos/include/platform/iot_network_freertos.h"
"${ProjDirPath}/../../../../../../rtos/freertos/libraries/abstractions/platform/freertos/include/platform/iot_platform_types_freertos.h"
"${ProjDirPath}/../../../../../../rtos/freertos/libraries/abstractions/platform/freertos/iot_clock_freertos.c"
"${ProjDirPath}/../../../../../../rtos/freertos/libraries/abstractions/platform/freertos/iot_metrics.c"
"${ProjDirPath}/../../../../../../rtos/freertos/libraries/abstractions/platform/freertos/iot_network_freertos.c"
"${ProjDirPath}/../../../../../../rtos/freertos/libraries/abstractions/platform/freertos/iot_threads_freertos.c"
"${ProjDirPath}/../../../../../../rtos/freertos/libraries/abstractions/platform/include/platform/iot_clock.h"
"${ProjDirPath}/../../../../../../rtos/freertos/libraries/abstractions/platform/include/platform/iot_metrics.h"
"${ProjDirPath}/../../../../../../rtos/freertos/libraries/abstractions/platform/include/platform/iot_network.h"
"${ProjDirPath}/../../../../../../rtos/freertos/libraries/abstractions/platform/include/platform/iot_threads.h"
"${ProjDirPath}/../../../../../../rtos/freertos/libraries/abstractions/platform/include/types/iot_platform_types.h"
"${ProjDirPath}/../../../../../../rtos/freertos/libraries/c_sdk/aws/shadow/include/aws_iot_shadow.h"
"${ProjDirPath}/../../../../../../rtos/freertos/libraries/c_sdk/aws/shadow/include/aws_shadow.h"
"${ProjDirPath}/../../../../../../rtos/freertos/libraries/c_sdk/aws/shadow/include/types/aws_iot_shadow_types.h"
"${ProjDirPath}/../../../../../../rtos/freertos/libraries/c_sdk/aws/shadow/src/aws_iot_shadow_api.c"
"${ProjDirPath}/../../../../../../rtos/freertos/libraries/c_sdk/aws/shadow/src/aws_iot_shadow_operation.c"
"${ProjDirPath}/../../../../../../rtos/freertos/libraries/c_sdk/aws/shadow/src/aws_iot_shadow_parser.c"
"${ProjDirPath}/../../../../../../rtos/freertos/libraries/c_sdk/aws/shadow/src/aws_iot_shadow_static_memory.c"
"${ProjDirPath}/../../../../../../rtos/freertos/libraries/c_sdk/aws/shadow/src/aws_iot_shadow_subscription.c"
"${ProjDirPath}/../../../../../../rtos/freertos/libraries/c_sdk/aws/shadow/src/aws_shadow.c"
"${ProjDirPath}/../../../../../../rtos/freertos/libraries/c_sdk/aws/shadow/src/aws_shadow_config_defaults.h"
"${ProjDirPath}/../../../../../../rtos/freertos/libraries/c_sdk/aws/shadow/src/private/aws_iot_shadow_internal.h"
"${ProjDirPath}/../../../../../../rtos/freertos/libraries/c_sdk/standard/common/include/iot_appversion32.h"
"${ProjDirPath}/../../../../../../rtos/freertos/libraries/c_sdk/standard/common/include/iot_atomic.h"
"${ProjDirPath}/../../../../../../rtos/freertos/libraries/c_sdk/standard/common/include/iot_init.h"
"${ProjDirPath}/../../../../../../rtos/freertos/libraries/c_sdk/standard/common/include/iot_linear_containers.h"
"${ProjDirPath}/../../../../../../rtos/freertos/libraries/c_sdk/standard/common/include/iot_logging_setup.h"
"${ProjDirPath}/../../../../../../rtos/freertos/libraries/c_sdk/standard/common/include/iot_logging_task.h"
"${ProjDirPath}/../../../../../../rtos/freertos/libraries/c_sdk/standard/common/include/iot_taskpool.h"
"${ProjDirPath}/../../../../../../rtos/freertos/libraries/c_sdk/standard/common/include/private/iot_default_root_certificates.h"
"${ProjDirPath}/../../../../../../rtos/freertos/libraries/c_sdk/standard/common/include/private/iot_doubly_linked_list.h"
"${ProjDirPath}/../../../../../../rtos/freertos/libraries/c_sdk/standard/common/include/private/iot_error.h"
"${ProjDirPath}/../../../../../../rtos/freertos/libraries/c_sdk/standard/common/include/private/iot_lib_init.h"
"${ProjDirPath}/../../../../../../rtos/freertos/libraries/c_sdk/standard/common/include/private/iot_logging.h"
"${ProjDirPath}/../../../../../../rtos/freertos/libraries/c_sdk/standard/common/include/private/iot_static_memory.h"
"${ProjDirPath}/../../../../../../rtos/freertos/libraries/c_sdk/standard/common/include/private/iot_taskpool_internal.h"
"${ProjDirPath}/../../../../../../rtos/freertos/libraries/c_sdk/standard/common/include/types/iot_network_types.h"
"${ProjDirPath}/../../../../../../rtos/freertos/libraries/c_sdk/standard/common/include/types/iot_taskpool_types.h"
"${ProjDirPath}/../../../../../../rtos/freertos/libraries/c_sdk/standard/common/iot_device_metrics.c"
"${ProjDirPath}/../../../../../../rtos/freertos/libraries/c_sdk/standard/common/iot_init.c"
"${ProjDirPath}/../../../../../../rtos/freertos/libraries/c_sdk/standard/common/iot_static_memory_common.c"
"${ProjDirPath}/../../../../../../rtos/freertos/libraries/c_sdk/standard/common/logging/iot_logging.c"
"${ProjDirPath}/../../../../../../rtos/freertos/libraries/c_sdk/standard/common/logging/iot_logging_task_dynamic_buffers.c"
"${ProjDirPath}/../../../../../../rtos/freertos/libraries/c_sdk/standard/common/taskpool/iot_taskpool.c"
"${ProjDirPath}/../../../../../../rtos/freertos/libraries/c_sdk/standard/common/taskpool/iot_taskpool_static_memory.c"
"${ProjDirPath}/../../../../../../rtos/freertos/libraries/c_sdk/standard/mqtt/include/iot_mqtt.h"
"${ProjDirPath}/../../../../../../rtos/freertos/libraries/c_sdk/standard/mqtt/include/iot_mqtt_agent.h"
"${ProjDirPath}/../../../../../../rtos/freertos/libraries/c_sdk/standard/mqtt/include/iot_mqtt_agent_config_defaults.h"
"${ProjDirPath}/../../../../../../rtos/freertos/libraries/c_sdk/standard/mqtt/include/iot_mqtt_config_defaults.h"
"${ProjDirPath}/../../../../../../rtos/freertos/libraries/c_sdk/standard/mqtt/include/iot_mqtt_lib.h"
"${ProjDirPath}/../../../../../../rtos/freertos/libraries/c_sdk/standard/mqtt/include/types/iot_mqtt_types.h"
"${ProjDirPath}/../../../../../../rtos/freertos/libraries/c_sdk/standard/mqtt/src/iot_mqtt_agent.c"
"${ProjDirPath}/../../../../../../rtos/freertos/libraries/c_sdk/standard/mqtt/src/iot_mqtt_api.c"
"${ProjDirPath}/../../../../../../rtos/freertos/libraries/c_sdk/standard/mqtt/src/iot_mqtt_network.c"
"${ProjDirPath}/../../../../../../rtos/freertos/libraries/c_sdk/standard/mqtt/src/iot_mqtt_operation.c"
"${ProjDirPath}/../../../../../../rtos/freertos/libraries/c_sdk/standard/mqtt/src/iot_mqtt_serialize.c"
"${ProjDirPath}/../../../../../../rtos/freertos/libraries/c_sdk/standard/mqtt/src/iot_mqtt_static_memory.c"
"${ProjDirPath}/../../../../../../rtos/freertos/libraries/c_sdk/standard/mqtt/src/iot_mqtt_subscription.c"
"${ProjDirPath}/../../../../../../rtos/freertos/libraries/c_sdk/standard/mqtt/src/iot_mqtt_validate.c"
"${ProjDirPath}/../../../../../../rtos/freertos/libraries/c_sdk/standard/mqtt/src/private/iot_mqtt_internal.h"
"${ProjDirPath}/../../../../../../rtos/freertos/libraries/c_sdk/standard/serializer/include/iot_json_utils.h"
"${ProjDirPath}/../../../../../../rtos/freertos/libraries/c_sdk/standard/serializer/include/iot_serializer.h"
"${ProjDirPath}/../../../../../../rtos/freertos/libraries/c_sdk/standard/serializer/src/iot_json_utils.c"
"${ProjDirPath}/../../../../../../rtos/freertos/libraries/c_sdk/standard/serializer/src/iot_serializer_static_memory.c"
"${ProjDirPath}/../../../../../../rtos/freertos/libraries/c_sdk/standard/serializer/src/json/iot_serializer_json_decoder.c"
"${ProjDirPath}/../../../../../../rtos/freertos/libraries/c_sdk/standard/serializer/src/json/iot_serializer_json_encoder.c"
"${ProjDirPath}/../../../../../../rtos/freertos/libraries/abstractions/secure_sockets/include/iot_secure_sockets.h"
"${ProjDirPath}/../../../../../../rtos/freertos/libraries/abstractions/secure_sockets/include/iot_secure_sockets_config_defaults.h"
"${ProjDirPath}/../../../../../../rtos/freertos/libraries/abstractions/secure_sockets/include/iot_secure_sockets_wrapper_metrics.h"
"${ProjDirPath}/../../../../../../rtos/freertos/libraries/freertos_plus/standard/crypto/include/iot_crypto.h"
"${ProjDirPath}/../../../../../../rtos/freertos/libraries/freertos_plus/standard/crypto/src/iot_crypto.c"
"${ProjDirPath}/../../../../../../rtos/freertos/libraries/freertos_plus/standard/pkcs11/include/iot_pkcs11.h"
"${ProjDirPath}/../../../../../../rtos/freertos/libraries/freertos_plus/standard/pkcs11/src/iot_pkcs11.c"
"${ProjDirPath}/../../../../../../rtos/freertos/libraries/freertos_plus/standard/utils/include/iot_pki_utils.h"
"${ProjDirPath}/../../../../../../rtos/freertos/libraries/freertos_plus/standard/utils/include/iot_system_init.h"
"${ProjDirPath}/../../../../../../rtos/freertos/libraries/freertos_plus/standard/utils/src/iot_pki_utils.c"
"${ProjDirPath}/../../../../../../rtos/freertos/libraries/freertos_plus/standard/utils/src/iot_system_init.c"
"${ProjDirPath}/../../../../../../rtos/freertos/libraries/freertos_plus/standard/tls/include/iot_tls.h"
"${ProjDirPath}/../../../../../../rtos/freertos/libraries/freertos_plus/standard/tls/src/iot_tls.c"
"${ProjDirPath}/../../../../../../rtos/freertos/libraries/abstractions/pkcs11/include/iot_pkcs11_pal.h"
"${ProjDirPath}/../../../../../../rtos/freertos/libraries/abstractions/pkcs11/mbedtls/iot_pkcs11_mbedtls.c"
"${ProjDirPath}/../../../../../../rtos/freertos/libraries/abstractions/pkcs11/mbedtls/threading_alt.h"
"${ProjDirPath}/../../../../../../rtos/freertos/libraries/3rdparty/pkcs11/CONTRIBUTING.md"
"${ProjDirPath}/../../../../../../rtos/freertos/libraries/3rdparty/pkcs11/LICENSE.md"
"${ProjDirPath}/../../../../../../rtos/freertos/libraries/3rdparty/pkcs11/README.md"
"${ProjDirPath}/../../../../../../rtos/freertos/libraries/3rdparty/pkcs11/pkcs11.h"
"${ProjDirPath}/../../../../../../rtos/freertos/libraries/3rdparty/pkcs11/pkcs11f.h"
"${ProjDirPath}/../../../../../../rtos/freertos/libraries/3rdparty/pkcs11/pkcs11t.h"
"${ProjDirPath}/../../../../../../rtos/freertos/libraries/3rdparty/jsmn/jsmn.c"
"${ProjDirPath}/../../../../../../rtos/freertos/libraries/3rdparty/jsmn/jsmn.h"
"${ProjDirPath}/../../../../../../rtos/freertos/vendors/nxp/pkcs11/common/iot_pkcs11_pal.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls/include/mbedtls/aes.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls/include/mbedtls/aesni.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls/include/mbedtls/arc4.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls/include/mbedtls/aria.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls/include/mbedtls/asn1.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls/include/mbedtls/asn1write.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls/include/mbedtls/base64.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls/include/mbedtls/bignum.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls/include/mbedtls/blowfish.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls/include/mbedtls/bn_mul.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls/include/mbedtls/camellia.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls/include/mbedtls/ccm.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls/include/mbedtls/certs.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls/include/mbedtls/chacha20.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls/include/mbedtls/chachapoly.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls/include/mbedtls/check_config.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls/include/mbedtls/cipher.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls/include/mbedtls/cipher_internal.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls/include/mbedtls/cmac.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls/include/mbedtls/compat-1.3.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls/include/mbedtls/config.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls/include/mbedtls/ctr_drbg.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls/include/mbedtls/debug.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls/include/mbedtls/des.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls/include/mbedtls/dhm.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls/include/mbedtls/ecdh.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls/include/mbedtls/ecdsa.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls/include/mbedtls/ecjpake.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls/include/mbedtls/ecp.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls/include/mbedtls/ecp_internal.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls/include/mbedtls/entropy.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls/include/mbedtls/entropy_poll.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls/include/mbedtls/error.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls/include/mbedtls/gcm.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls/include/mbedtls/havege.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls/include/mbedtls/hkdf.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls/include/mbedtls/hmac_drbg.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls/include/mbedtls/md.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls/include/mbedtls/md2.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls/include/mbedtls/md4.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls/include/mbedtls/md5.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls/include/mbedtls/md_internal.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls/include/mbedtls/memory_buffer_alloc.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls/include/mbedtls/net.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls/include/mbedtls/net_sockets.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls/include/mbedtls/nist_kw.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls/include/mbedtls/oid.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls/include/mbedtls/padlock.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls/include/mbedtls/pem.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls/include/mbedtls/pk.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls/include/mbedtls/pk_internal.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls/include/mbedtls/pkcs11.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls/include/mbedtls/pkcs12.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls/include/mbedtls/pkcs5.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls/include/mbedtls/platform.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls/include/mbedtls/platform_time.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls/include/mbedtls/platform_util.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls/include/mbedtls/poly1305.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls/include/mbedtls/ripemd160.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls/include/mbedtls/rsa.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls/include/mbedtls/rsa_internal.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls/include/mbedtls/sha1.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls/include/mbedtls/sha256.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls/include/mbedtls/sha512.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls/include/mbedtls/ssl.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls/include/mbedtls/ssl_cache.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls/include/mbedtls/ssl_ciphersuites.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls/include/mbedtls/ssl_cookie.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls/include/mbedtls/ssl_internal.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls/include/mbedtls/ssl_ticket.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls/include/mbedtls/threading.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls/include/mbedtls/timing.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls/include/mbedtls/version.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls/include/mbedtls/x509.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls/include/mbedtls/x509_crl.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls/include/mbedtls/x509_crt.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls/include/mbedtls/x509_csr.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls/include/mbedtls/xtea.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls/library/aes.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls/library/aesni.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls/library/arc4.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls/library/aria.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls/library/asn1parse.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls/library/asn1write.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls/library/base64.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls/library/bignum.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls/library/blowfish.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls/library/camellia.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls/library/ccm.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls/library/certs.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls/library/chacha20.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls/library/chachapoly.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls/library/cipher.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls/library/cipher_wrap.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls/library/cmac.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls/library/ctr_drbg.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls/library/debug.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls/library/des.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls/library/dhm.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls/library/ecdh.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls/library/ecdsa.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls/library/ecjpake.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls/library/ecp.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls/library/ecp_curves.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls/library/entropy.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls/library/entropy_poll.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls/library/error.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls/library/gcm.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls/library/havege.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls/library/hkdf.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls/library/hmac_drbg.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls/library/md.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls/library/md2.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls/library/md4.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls/library/md5.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls/library/md_wrap.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls/library/memory_buffer_alloc.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls/library/net_sockets.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls/library/nist_kw.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls/library/oid.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls/library/padlock.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls/library/pem.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls/library/pk.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls/library/pk_wrap.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls/library/pkcs11.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls/library/pkcs12.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls/library/pkcs5.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls/library/pkparse.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls/library/pkwrite.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls/library/platform.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls/library/platform_util.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls/library/poly1305.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls/library/ripemd160.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls/library/rsa.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls/library/rsa_internal.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls/library/sha1.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls/library/sha256.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls/library/sha512.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls/library/ssl_cache.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls/library/ssl_ciphersuites.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls/library/ssl_cli.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls/library/ssl_cookie.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls/library/ssl_srv.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls/library/ssl_ticket.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls/library/ssl_tls.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls/library/threading.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls/library/timing.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls/library/version.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls/library/version_features.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls/library/x509.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls/library/x509_create.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls/library/x509_crl.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls/library/x509_crt.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls/library/x509_csr.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls/library/x509write_crt.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls/library/x509write_csr.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls/library/xtea.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls/port/ksdk/aes_alt.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls/port/ksdk/aes_alt.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls/port/ksdk/des_alt.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls/port/ksdk/des_alt.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls/port/ksdk/ecp_alt.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls/port/ksdk/ecp_alt.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls/port/ksdk/ecp_alt_ksdk.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls/port/ksdk/ecp_curves_alt.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls/port/ksdk/ksdk_mbedtls.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls/port/ksdk/ksdk_mbedtls.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls/port/ksdk/ksdk_mbedtls_config.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls/port/ksdk/sha1_alt.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls/port/ksdk/sha256_alt.h"
"${ProjDirPath}/../../../../../../devices/LPC55S69/drivers/fsl_clock.c"
"${ProjDirPath}/../../../../../../devices/LPC55S69/drivers/fsl_clock.h"
"${ProjDirPath}/../../../../../../devices/LPC55S69/drivers/fsl_common.c"
"${ProjDirPath}/../../../../../../devices/LPC55S69/drivers/fsl_common.h"
"${ProjDirPath}/../../../../../../devices/LPC55S69/LPC55S69_cm33_core0.h"
"${ProjDirPath}/../../../../../../devices/LPC55S69/LPC55S69_cm33_core0_features.h"
"${ProjDirPath}/../../../../../../devices/LPC55S69/fsl_device_registers.h"
"${ProjDirPath}/../../../../../../devices/LPC55S69/system_LPC55S69_cm33_core0.c"
"${ProjDirPath}/../../../../../../devices/LPC55S69/system_LPC55S69_cm33_core0.h"
"${ProjDirPath}/../../../../../../devices/LPC55S69/gcc/startup_LPC55S69_cm33_core0.S"
"${ProjDirPath}/../../../../../../devices/LPC55S69/drivers/fsl_usart.c"
"${ProjDirPath}/../../../../../../devices/LPC55S69/drivers/fsl_usart.h"
"${ProjDirPath}/../../../../../../devices/LPC55S69/drivers/fsl_flexcomm.c"
"${ProjDirPath}/../../../../../../devices/LPC55S69/drivers/fsl_flexcomm.h"
"${ProjDirPath}/../../../../../../devices/LPC55S69/drivers/fsl_iocon.h"
"${ProjDirPath}/../../../../../../devices/LPC55S69/drivers/fsl_gpio.c"
"${ProjDirPath}/../../../../../../devices/LPC55S69/drivers/fsl_gpio.h"
"${ProjDirPath}/../../../../../../devices/LPC55S69/drivers/fsl_power.c"
"${ProjDirPath}/../../../../../../devices/LPC55S69/drivers/fsl_power.h"
"${ProjDirPath}/../../../../../../devices/LPC55S69/utilities/fsl_assert.c"
"${ProjDirPath}/../../../../../../devices/LPC55S69/utilities/debug_console/fsl_debug_console.c"
"${ProjDirPath}/../../../../../../devices/LPC55S69/utilities/debug_console/fsl_debug_console.h"
"${ProjDirPath}/../../../../../../devices/LPC55S69/utilities/debug_console/fsl_debug_console_conf.h"
"${ProjDirPath}/../../../../../../devices/LPC55S69/utilities/str/fsl_str.c"
"${ProjDirPath}/../../../../../../devices/LPC55S69/utilities/str/fsl_str.h"
"${ProjDirPath}/../../../../../../components/uart/uart.h"
"${ProjDirPath}/../../../../../../components/uart/usart_adapter.c"
"${ProjDirPath}/../../../../../../components/serial_manager/serial_manager.c"
"${ProjDirPath}/../../../../../../components/serial_manager/serial_manager.h"
"${ProjDirPath}/../../../../../../components/serial_manager/serial_port_internal.h"
"${ProjDirPath}/../../../../../../components/lists/generic_list.c"
"${ProjDirPath}/../../../../../../components/lists/generic_list.h"
"${ProjDirPath}/../../../../../../components/serial_manager/serial_port_uart.c"
"${ProjDirPath}/../../../../../../components/serial_manager/serial_port_uart.h"
"${ProjDirPath}/../../../../../../middleware/wifi_qca/port/boards/lpcxpresso55s69/freertos/wifi_shield.c"
"${ProjDirPath}/../../../../../../middleware/wifi_qca/port/boards/lpcxpresso55s69/freertos/wifi_shield.h"
"${ProjDirPath}/../../../../../../middleware/wifi_qca/port/env/freertos/wifi_env.c"
"${ProjDirPath}/../../../../../../middleware/wifi_qca/port/env/freertos/wifi_env.h"
"${ProjDirPath}/../../../../../../middleware/wifi_qca/port/env/freertos/wifi_env_port.h"
"${ProjDirPath}/../../../../../../middleware/wifi_qca/port/env/freertos/wifi_port.h"
"${ProjDirPath}/../../../../../../rtos/freertos/freertos_kernel/include/freertos_tasks_c_additions.h"
"${ProjDirPath}/../../../../../../middleware/wifi_qca/port/drivers/flexcomm_freertos/wifi_spi.c"
"${ProjDirPath}/../../../../../../middleware/wifi_qca/port/drivers/flexcomm_freertos/wifi_spi.h"
"${ProjDirPath}/../../../../../../devices/LPC55S69/drivers/fsl_spi.c"
"${ProjDirPath}/../../../../../../devices/LPC55S69/drivers/fsl_spi.h"
"${ProjDirPath}/../../../../../../CMSIS/Include/core_cm33.h"
"${ProjDirPath}/../../../../../../CMSIS/Include/mpu_armv8.h"
"${ProjDirPath}/../../../../../../CMSIS/Include/tz_context.h"
"${ProjDirPath}/../../../../../../CMSIS/Include/cmsis_armcc.h"
"${ProjDirPath}/../../../../../../CMSIS/Include/cmsis_armclang.h"
"${ProjDirPath}/../../../../../../CMSIS/Include/cmsis_armclang_ltm.h"
"${ProjDirPath}/../../../../../../CMSIS/Include/cmsis_compiler.h"
"${ProjDirPath}/../../../../../../CMSIS/Include/cmsis_gcc.h"
"${ProjDirPath}/../../../../../../CMSIS/Include/cmsis_iccarm.h"
"${ProjDirPath}/../../../../../../CMSIS/Include/cmsis_version.h"
"${ProjDirPath}/../../../../../../CMSIS/Include/arm_common_tables.h"
"${ProjDirPath}/../../../../../../CMSIS/Include/arm_const_structs.h"
"${ProjDirPath}/../../../../../../CMSIS/Include/arm_math.h"
"${ProjDirPath}/../../../../../../devices/LPC55S69/drivers/fsl_reset.c"
"${ProjDirPath}/../../../../../../devices/LPC55S69/drivers/fsl_reset.h"
"${ProjDirPath}/../../../../../../devices/LPC55S69/drivers/fsl_spi_dma.c"
"${ProjDirPath}/../../../../../../devices/LPC55S69/drivers/fsl_spi_dma.h"
"${ProjDirPath}/../../../../../../devices/LPC55S69/drivers/fsl_dma.c"
"${ProjDirPath}/../../../../../../devices/LPC55S69/drivers/fsl_dma.h"
"${ProjDirPath}/../../../../../../devices/LPC55S69/drivers/fsl_pint.c"
"${ProjDirPath}/../../../../../../devices/LPC55S69/drivers/fsl_pint.h"
"${ProjDirPath}/../../../../../../devices/LPC55S69/drivers/fsl_inputmux.c"
"${ProjDirPath}/../../../../../../devices/LPC55S69/drivers/fsl_inputmux.h"
"${ProjDirPath}/../../../../../../devices/LPC55S69/drivers/fsl_inputmux_connections.h"
"${ProjDirPath}/../../../../../../devices/LPC55S69/utilities/fsl_sbrk.c"
)


set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -T${ProjDirPath}/LPC55S69_cm33_core0_flash.ld -static")

set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -T${ProjDirPath}/LPC55S69_cm33_core0_flash.ld -static")

TARGET_LINK_LIBRARIES(aws_device_configuration_wifi.elf -Wl,--start-group)
target_link_libraries(aws_device_configuration_wifi.elf debug m)

target_link_libraries(aws_device_configuration_wifi.elf debug c)

target_link_libraries(aws_device_configuration_wifi.elf debug gcc)

target_link_libraries(aws_device_configuration_wifi.elf debug nosys)

target_link_libraries(aws_device_configuration_wifi.elf optimized m)

target_link_libraries(aws_device_configuration_wifi.elf optimized c)

target_link_libraries(aws_device_configuration_wifi.elf optimized gcc)

target_link_libraries(aws_device_configuration_wifi.elf optimized nosys)

link_directories(${ProjDirPath}/../../../../../../devices/LPC55S69/gcc)

target_link_libraries(aws_device_configuration_wifi.elf debug ${ProjDirPath}/../../../../../../devices/LPC55S69/gcc/libpower_hardabi.a)

link_directories(${ProjDirPath}/../../../../../../devices/LPC55S69/gcc)

target_link_libraries(aws_device_configuration_wifi.elf optimized ${ProjDirPath}/../../../../../../devices/LPC55S69/gcc/libpower_hardabi.a)

TARGET_LINK_LIBRARIES(aws_device_configuration_wifi.elf -Wl,--end-group)


