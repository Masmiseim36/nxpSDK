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

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -D__STARTUP_INITIALIZE_NONCACHEDATA")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -g")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -mcpu=cortex-m7")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -Wall")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -mfloat-abi=hard")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -mfpu=fpv5-d16")

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

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -D__STARTUP_INITIALIZE_NONCACHEDATA")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -mcpu=cortex-m7")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -Wall")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -mfloat-abi=hard")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -mfpu=fpv5-d16")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -mthumb")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -fno-common")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -ffunction-sections")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -fdata-sections")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -ffreestanding")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -fno-builtin")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -mapcs")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -std=gnu99")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DCPU_MIMXRT106ADVL6A")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DCPU_MIMXRT106ADVL6A_cm7")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DPRINTF_FLOAT_ENABLE=0")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DSCANF_FLOAT_ENABLE=0")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DPRINTF_ADVANCED_ENABLE=0")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DSCANF_ADVANCED_ENABLE=0")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DXIP_EXTERNAL_FLASH=1")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DXIP_BOOT_HEADER_ENABLE=1")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DXIP_BOOT_HEADER_DCD_ENABLE=1")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -D__USE_CMSIS")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDISABLE_IMAGE_VERIFICATION=0")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DUSE_WIFI_CONNECTION=1")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DBOOTLOADER_AWS_IOT_OTA_ENABLED=1")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DBOOTLOADER_WIFI_ENABLED=1")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DLWIP_DNS=1")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DLWIP_DHCP=1")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG_CONSOLE_TRANSMIT_BUFFER_LEN=4096")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG_CONSOLE_PRINTF_MAX_LOG_LEN=256")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DENABLE_UNSIGNED_USB_MSD=1")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DBOOTLOADER_MSD_ENABLED=1")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DUSB_COMPOSITE=0")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DRELOCATE_VECTOR_TABLE=1")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DAPP_MAJ_VER=0x01")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DAPP_MIN_VER=0x00")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DAPP_BLD_VER=0x0006")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -D__SEMIHOST_HARDFAULT_DISABLE=1")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG_CONSOLE_TRANSFER_NON_BLOCKING=1")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DUSB_STACK_FREERTOS_HEAP_SIZE=65536")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DUSB_STACK_FREERTOS")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DARM_MATH_CM7")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DSDK_DEBUGCONSOLE=1")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DCR_INTEGER_PRINTF")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DUSE_RTOS=1")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DFSL_RTOS_FREE_RTOS")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DMBEDTLS_CONFIG_FILE='\"aws_mbedtls_config.h\"'")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DAWS_LOGGING_INCLUDE='\"iot_logging_task.h\"'")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DAWS_SYS_INIT_INCLUDE='\"iot_system_init.h\"'")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DAWS_MQTT_AGENT_INCLUDE='\"iot_mqtt_agent.h\"'")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DAWS_VERSION_INCLUDE='\"iot_appversion32.h\"'")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEMO_NETWORK_TYPE=AWSIOT_NETWORK_TYPE_WIFI")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DSDK_2_8")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DCONFIG_FLEXRAM_AT_STARTUP=1")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DSDIO_ENABLED")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DSERIAL_PORT_TYPE_UART=1")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DWIFI_WICED")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DSDK_DEBUGCONSOLE_UART")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -fomit-frame-pointer")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -mcpu=cortex-m7")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wall")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -mfloat-abi=hard")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -mfpu=fpv5-d16")

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

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DCPU_MIMXRT106ADVL6A")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DCPU_MIMXRT106ADVL6A_cm7")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DPRINTF_FLOAT_ENABLE=0")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DSCANF_FLOAT_ENABLE=0")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DPRINTF_ADVANCED_ENABLE=0")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DSCANF_ADVANCED_ENABLE=0")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DXIP_EXTERNAL_FLASH=1")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DXIP_BOOT_HEADER_ENABLE=1")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DXIP_BOOT_HEADER_DCD_ENABLE=1")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -D__USE_CMSIS")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DDISABLE_IMAGE_VERIFICATION=0")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DUSE_WIFI_CONNECTION=1")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DBOOTLOADER_AWS_IOT_OTA_ENABLED=1")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DBOOTLOADER_WIFI_ENABLED=1")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DLWIP_DNS=1")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DLWIP_DHCP=1")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DDEBUG_CONSOLE_TRANSMIT_BUFFER_LEN=4096")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DDEBUG_CONSOLE_PRINTF_MAX_LOG_LEN=256")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DENABLE_UNSIGNED_USB_MSD=1")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DBOOTLOADER_MSD_ENABLED=1")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DUSB_COMPOSITE=0")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DRELOCATE_VECTOR_TABLE=1")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DAPP_MAJ_VER=0x01")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DAPP_MIN_VER=0x00")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DAPP_BLD_VER=0x0006")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -D__SEMIHOST_HARDFAULT_DISABLE=1")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DDEBUG_CONSOLE_TRANSFER_NON_BLOCKING=1")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DUSB_STACK_FREERTOS_HEAP_SIZE=65536")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DUSB_STACK_FREERTOS")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DARM_MATH_CM7")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DSDK_DEBUGCONSOLE=1")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DCR_INTEGER_PRINTF")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DUSE_RTOS=1")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DFSL_RTOS_FREE_RTOS")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DMBEDTLS_CONFIG_FILE='\"aws_mbedtls_config.h\"'")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DAWS_LOGGING_INCLUDE='\"iot_logging_task.h\"'")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DAWS_SYS_INIT_INCLUDE='\"iot_system_init.h\"'")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DAWS_MQTT_AGENT_INCLUDE='\"iot_mqtt_agent.h\"'")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DAWS_VERSION_INCLUDE='\"iot_appversion32.h\"'")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DDEMO_NETWORK_TYPE=AWSIOT_NETWORK_TYPE_WIFI")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DSDK_2_8")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DCONFIG_FLEXRAM_AT_STARTUP=1")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DSDIO_ENABLED")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DSERIAL_PORT_TYPE_UART=1")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DWIFI_WICED")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DSDK_DEBUGCONSOLE_UART")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Os")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O0")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -g")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -fomit-frame-pointer")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -mcpu=cortex-m7")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Wall")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -mfloat-abi=hard")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -mfpu=fpv5-d16")

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

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DSERIAL_PORT_TYPE_UART=1")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -mcpu=cortex-m7")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -mfloat-abi=hard")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -mfpu=fpv5-d16")

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

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -mcpu=cortex-m7")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -mfloat-abi=hard")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -mfpu=fpv5-d16")

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

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -mcpu=cortex-m7")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -Wall")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -mfloat-abi=hard")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -mfpu=fpv5-d16")

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

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} --defsym=__stack_size__=0x1000")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} --defsym=__heap_size__=0x1000")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} --gc-sections")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -print-memory-usage")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -mcpu=cortex-m7")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -Wall")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -mfloat-abi=hard")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -mfpu=fpv5-d16")

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

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} --defsym=__stack_size__=0x1000")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} --defsym=__heap_size__=0x1000")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} --gc-sections")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -print-memory-usage")

include_directories(${ProjDirPath}/../../../../../rtos/freertos/freertos_kernel/portable/GCC/ARM_CM4F)

include_directories(${ProjDirPath}/../../../../../middleware/sln_iot_common_platform/security)

include_directories(${ProjDirPath}/../config_files)

include_directories(${ProjDirPath}/../../../common)

include_directories(${ProjDirPath}/../../../../../rtos/freertos/demos/include)

include_directories(${ProjDirPath}/../../../../../rtos/freertos/libraries/abstractions/pkcs11/mbedtls)

include_directories(${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/app)

include_directories(${ProjDirPath}/../../../../../rtos/freertos/libraries/abstractions/pkcs11/include)

include_directories(${ProjDirPath}/../../../../../rtos/freertos/libraries/c_sdk/standard/common/include/types)

include_directories(${ProjDirPath}/../../../../../rtos/freertos/libraries/c_sdk/standard/mqtt/include/types)

include_directories(${ProjDirPath}/../../bootloader)

include_directories(${ProjDirPath}/../../../../../middleware/sln_iot_common_platform/boot_code/bootloader)

include_directories(${ProjDirPath}/../../../../../middleware/sln_iot_common_platform/boot_code/bootloader/config_files)

include_directories(${ProjDirPath}/../../../../../middleware/sln_iot_common_platform/boot_code/bootloader/bl_drv)

include_directories(${ProjDirPath}/../../../../../devices/MIMXRT106A/drivers)

include_directories(${ProjDirPath}/../../../../../platform/utilities/debug_console)

include_directories(${ProjDirPath}/../../../../../platform/utilities/debug_console/str)

include_directories(${ProjDirPath}/../../..)

include_directories(${ProjDirPath}/../../../../../CMSIS/Include)

include_directories(${ProjDirPath}/../../../../../devices)

include_directories(${ProjDirPath}/../../../../../rtos/freertos/libraries/freertos_plus/aws/ota/src)

include_directories(${ProjDirPath}/../../../../../rtos/freertos/libraries/freertos_plus/aws/ota/include)

include_directories(${ProjDirPath}/../../../../../rtos/freertos/libraries/3rdparty/jsmn)

include_directories(${ProjDirPath}/../../../../../rtos/freertos/libraries/freertos_plus/standard/utils/include)

include_directories(${ProjDirPath}/../../../../../rtos/freertos/libraries/c_sdk/standard/https/include)

include_directories(${ProjDirPath}/../../../../../rtos/freertos/libraries/freertos_plus/standard/crypto/include)

include_directories(${ProjDirPath}/../../../../../rtos/freertos/libraries/abstractions/secure_sockets/include)

include_directories(${ProjDirPath}/../../../../../rtos/freertos/libraries/abstractions/platform/freertos/include)

include_directories(${ProjDirPath}/../../../../../rtos/freertos/libraries/abstractions/platform/include)

include_directories(${ProjDirPath}/../../../../../rtos/freertos/libraries/freertos_plus/standard/tls/include)

include_directories(${ProjDirPath}/../../../../../rtos/freertos/libraries/freertos_plus/standard/pkcs11/include)

include_directories(${ProjDirPath}/../../../../../rtos/freertos/libraries/3rdparty/pkcs11)

include_directories(${ProjDirPath}/../../../../../rtos/freertos/libraries/c_sdk/aws/shadow/include)

include_directories(${ProjDirPath}/../../../../../rtos/freertos/libraries/c_sdk/aws/shadow/src)

include_directories(${ProjDirPath}/../../../../../rtos/freertos/libraries/c_sdk/standard/common/include)

include_directories(${ProjDirPath}/../../../../../rtos/freertos/libraries/c_sdk/standard/common/include/private)

include_directories(${ProjDirPath}/../../../../../rtos/freertos/libraries/c_sdk/standard/mqtt/include)

include_directories(${ProjDirPath}/../../../../../rtos/freertos/libraries/c_sdk/standard/mqtt/src)

include_directories(${ProjDirPath}/../../../../../rtos/freertos/libraries/c_sdk/standard/serializer/include)

include_directories(${ProjDirPath}/../../../../../rtos/freertos/libraries/3rdparty/http_parser)

include_directories(${ProjDirPath}/../../../../../rtos/freertos/freertos_kernel/include)

include_directories(${ProjDirPath}/../../../../../middleware/mbedtls/include)

include_directories(${ProjDirPath}/../../../../../middleware/mbedtls/port/ksdk)

include_directories(${ProjDirPath}/../../../../../rtos/freertos/demos/network_manager)

include_directories(${ProjDirPath}/../../../../../rtos/freertos/libraries/abstractions/wifi/include)

include_directories(${ProjDirPath}/../../../../../rtos/freertos/libraries/3rdparty/tinycbor)

include_directories(${ProjDirPath}/../../../../../middleware/sln_iot_common_platform/boot_code/bootloader/config_files/wifi)

include_directories(${ProjDirPath}/../../../../../devices/MIMXRT106A/utilities/str)

include_directories(${ProjDirPath}/../../../../../devices/MIMXRT106A/utilities/debug_console)

include_directories(${ProjDirPath}/../../../../../middleware/lwip/port)

include_directories(${ProjDirPath}/../../../../../middleware/lwip/src)

include_directories(${ProjDirPath}/../../../../../middleware/lwip/src/include)

include_directories(${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/platforms/AW_NM372SM)

include_directories(${ProjDirPath}/../../../../../middleware/sln_iot_common_platform/network)

include_directories(${ProjDirPath}/../../../../../components/osa)

include_directories(${ProjDirPath}/../../../../../middleware/usb/device)

include_directories(${ProjDirPath}/../../../../../middleware/usb/include)

include_directories(${ProjDirPath}/../../../../../middleware/usb/output/source/device/class)

include_directories(${ProjDirPath}/../../../../../middleware/usb/output/source/device)

include_directories(${ProjDirPath}/..)

include_directories(${ProjDirPath}/../../../../../middleware/usb/phy)

include_directories(${ProjDirPath}/../../../../../middleware/sdmmc/sdio)

include_directories(${ProjDirPath}/../../../../../middleware/sdmmc/common)

include_directories(${ProjDirPath}/../../../../../middleware/sdmmc/host/usdhc)

include_directories(${ProjDirPath}/../../../../../middleware/sln_iot_common_platform/custom_config/flash/is26kl256s)

include_directories(${ProjDirPath}/../../../../../middleware/sln_iot_common_platform/drivers)

include_directories(${ProjDirPath}/../../../../../middleware/sln_iot_common_platform/flexspi)

include_directories(${ProjDirPath}/../../../../../middleware/sln_iot_common_platform/boot_code/common)

include_directories(${ProjDirPath}/../../../../../middleware/sln_iot_common_platform/utils)

include_directories(${ProjDirPath}/../../../../../devices/MIMXRT106A)

include_directories(${ProjDirPath}/../../../../../components/uart)

include_directories(${ProjDirPath}/../../../../../components/serial_manager)

include_directories(${ProjDirPath}/../../../../../components/lists)

include_directories(${ProjDirPath}/../../../../../devices/MIMXRT106A/xip)

include_directories(${ProjDirPath}/../../../xip)

include_directories(${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD/internal/chips/4343W)

include_directories(${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/include)

include_directories(${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/network/LwIP/WWD)

include_directories(${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/platform)

include_directories(${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/platform/include)

include_directories(${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/platform/MCU)

include_directories(${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/platform/MCU/LPC)

include_directories(${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/platform/MCU/LPC/WWD)

include_directories(${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/RTOS/FreeRTOS/WWD)

include_directories(${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD)

include_directories(${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD/include)

include_directories(${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD/include/network)

include_directories(${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD/include/platform)

include_directories(${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD/include/RTOS)

include_directories(${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD/internal)

include_directories(${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD/internal/bus_protocols/SDIO)

include_directories(${ProjDirPath}/../../../../../middleware/lwip/src/include/lwip)

include_directories(${ProjDirPath}/../../../../../middleware/sln_iot_common_platform/flash_mgmt)

include_directories(${ProjDirPath}/../../../../../middleware/sdmmc/osa)

include_directories(${ProjDirPath}/../../../../../devices/MIMXRT106A/utilities)

add_executable(bootloader_43438.elf 
"${ProjDirPath}/../../../source/dhcp_server.c"
"${ProjDirPath}/../../../source/dhcp_server.h"
"${ProjDirPath}/../../../source/iot_wifi.c"
"${ProjDirPath}/../../../common/sln_file_table.h"
"${ProjDirPath}/../../../common/sln_cfg_file.h"
"${ProjDirPath}/../aws_ota_pal.c"
"${ProjDirPath}/../sln_ota.c"
"${ProjDirPath}/../sln_ota.h"
"${ProjDirPath}/../iot_pkcs11_pal.h"
"${ProjDirPath}/../threading_alt.h"
"${ProjDirPath}/../../../sdmmc_config.c"
"${ProjDirPath}/../../../sdmmc_config.h"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../main.c"
"${ProjDirPath}/../config_files/aws_iot_network_config.h"
"${ProjDirPath}/../config_files/iot_config.h"
"${ProjDirPath}/../config_files/iot_mqtt_agent_config.h"
"${ProjDirPath}/../config_files/iot_pkcs11_config.h"
"${ProjDirPath}/../config_files/iot_config_common.h"
"${ProjDirPath}/../bootloader.c"
"${ProjDirPath}/../aws_application_version.h"
"${ProjDirPath}/../bootloader.h"
"${ProjDirPath}/../nor_encrypt_bee.c"
"${ProjDirPath}/../nor_encrypt_bee.h"
"${ProjDirPath}/../fusemap.h"
"${ProjDirPath}/../bl_drv/fsl_dcp_ext.c"
"${ProjDirPath}/../bl_drv/fsl_dcp_ext.h"
"${ProjDirPath}/../aws_mbedtls_config.h"
"${ProjDirPath}/../disk.c"
"${ProjDirPath}/../disk.h"
"${ProjDirPath}/../sln_msc_vfs.c"
"${ProjDirPath}/../sln_msc_vfs.h"
"${ProjDirPath}/../usb_device_config.h"
"${ProjDirPath}/../usb_device_descriptor.c"
"${ProjDirPath}/../usb_device_descriptor.h"
"${ProjDirPath}/../usb_device_board_config.h"
"${ProjDirPath}/../../../../../devices/MIMXRT106A/utilities/str/fsl_str.c"
"${ProjDirPath}/../../../../../devices/MIMXRT106A/utilities/str/fsl_str.h"
"${ProjDirPath}/../../../../../devices/MIMXRT106A/utilities/fsl_debug_console_conf.h"
"${ProjDirPath}/../iot_demo_logging.h"
"${ProjDirPath}/../aws_demo.h"
"${ProjDirPath}/../iot_freertos_init.c"
"${ProjDirPath}/../config_files/aws_mbedtls_config.h"
"${ProjDirPath}/../config_files/FreeRTOSConfigBoard.h"
"${ProjDirPath}/../config_files/FreeRTOSConfig.h"
"${ProjDirPath}/../config_files/aws_bufferpool_config.h"
"${ProjDirPath}/../config_files/aws_demo_config.h"
"${ProjDirPath}/../config_files/aws_mqtt_config.h"
"${ProjDirPath}/../config_files/aws_ota_agent_config.h"
"${ProjDirPath}/../config_files/aws_secure_sockets_config.h"
"${ProjDirPath}/../config_files/aws_shadow_config.h"
"${ProjDirPath}/../config_files/aws_wifi_config.h"
"${ProjDirPath}/../config_files/FreeRTOSIPConfig.h"
"${ProjDirPath}/../config_files/lwipopts.h"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
"${ProjDirPath}/../dcd.c"
"${ProjDirPath}/../dcd.h"
"${ProjDirPath}/../../../../../devices/MIMXRT106A/utilities/debug_console/fsl_debug_console.c"
"${ProjDirPath}/../../../../../devices/MIMXRT106A/utilities/debug_console/fsl_debug_console.h"
"${ProjDirPath}/../../../../../devices/MIMXRT106A/utilities/debug_console/fsl_debug_console_conf.h"
"${ProjDirPath}/../../../../../devices/MIMXRT106A/utilities/str/fsl_str.c"
"${ProjDirPath}/../../../../../devices/MIMXRT106A/utilities/str/fsl_str.h"
"${ProjDirPath}/../../../../../rtos/freertos/libraries/freertos_plus/aws/ota/README.md"
"${ProjDirPath}/../../../../../rtos/freertos/libraries/freertos_plus/aws/ota/include/aws_iot_ota_agent.h"
"${ProjDirPath}/../../../../../rtos/freertos/libraries/freertos_plus/aws/ota/include/aws_iot_ota_types.h"
"${ProjDirPath}/../../../../../rtos/freertos/libraries/freertos_plus/aws/ota/src/aws_iot_ota_agent.c"
"${ProjDirPath}/../../../../../rtos/freertos/libraries/freertos_plus/aws/ota/src/aws_iot_ota_agent_internal.h"
"${ProjDirPath}/../../../../../rtos/freertos/libraries/freertos_plus/aws/ota/src/aws_iot_ota_interface.c"
"${ProjDirPath}/../../../../../rtos/freertos/libraries/freertos_plus/aws/ota/src/aws_iot_ota_interface.h"
"${ProjDirPath}/../../../../../rtos/freertos/libraries/freertos_plus/aws/ota/src/aws_iot_ota_pal.h"
"${ProjDirPath}/../../../../../rtos/freertos/libraries/freertos_plus/aws/ota/src/http/aws_iot_ota_http.c"
"${ProjDirPath}/../../../../../rtos/freertos/libraries/freertos_plus/aws/ota/src/http/aws_iot_ota_http.h"
"${ProjDirPath}/../../../../../rtos/freertos/libraries/freertos_plus/aws/ota/src/mqtt/aws_iot_ota_cbor.c"
"${ProjDirPath}/../../../../../rtos/freertos/libraries/freertos_plus/aws/ota/src/mqtt/aws_iot_ota_cbor.h"
"${ProjDirPath}/../../../../../rtos/freertos/libraries/freertos_plus/aws/ota/src/mqtt/aws_iot_ota_cbor_internal.h"
"${ProjDirPath}/../../../../../rtos/freertos/libraries/freertos_plus/aws/ota/src/mqtt/aws_iot_ota_mqtt.c"
"${ProjDirPath}/../../../../../rtos/freertos/libraries/freertos_plus/aws/ota/src/mqtt/aws_iot_ota_mqtt.h"
"${ProjDirPath}/../../../../../rtos/freertos/libraries/3rdparty/jsmn/jsmn.c"
"${ProjDirPath}/../../../../../rtos/freertos/libraries/3rdparty/jsmn/jsmn.h"
"${ProjDirPath}/../../../../../rtos/freertos/libraries/freertos_plus/standard/utils/include/iot_pki_utils.h"
"${ProjDirPath}/../../../../../rtos/freertos/libraries/freertos_plus/standard/utils/include/iot_system_init.h"
"${ProjDirPath}/../../../../../rtos/freertos/libraries/freertos_plus/standard/utils/src/iot_pki_utils.c"
"${ProjDirPath}/../../../../../rtos/freertos/libraries/freertos_plus/standard/utils/src/iot_system_init.c"
"${ProjDirPath}/../../../../../rtos/freertos/libraries/c_sdk/standard/https/include/iot_https_client.h"
"${ProjDirPath}/../../../../../rtos/freertos/libraries/c_sdk/standard/https/include/iot_https_utils.h"
"${ProjDirPath}/../../../../../rtos/freertos/libraries/c_sdk/standard/https/include/types/iot_https_types.h"
"${ProjDirPath}/../../../../../rtos/freertos/libraries/c_sdk/standard/https/src/iot_https_client.c"
"${ProjDirPath}/../../../../../rtos/freertos/libraries/c_sdk/standard/https/src/iot_https_utils.c"
"${ProjDirPath}/../../../../../rtos/freertos/libraries/c_sdk/standard/https/src/private/iot_https_internal.h"
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
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/platforms/AW_NM372SM/wifi_nvram_image.h"
"${ProjDirPath}/../../../../../rtos/freertos/vendors/nxp/secure_sockets/lwip/iot_secure_sockets.c"
"${ProjDirPath}/../../../../../rtos/freertos/libraries/freertos_plus/standard/crypto/include/iot_crypto.h"
"${ProjDirPath}/../../../../../rtos/freertos/libraries/freertos_plus/standard/crypto/src/iot_crypto.c"
"${ProjDirPath}/../../../../../rtos/freertos/libraries/abstractions/secure_sockets/include/iot_secure_sockets.h"
"${ProjDirPath}/../../../../../rtos/freertos/libraries/abstractions/secure_sockets/include/iot_secure_sockets_config_defaults.h"
"${ProjDirPath}/../../../../../rtos/freertos/libraries/abstractions/secure_sockets/include/iot_secure_sockets_wrapper_metrics.h"
"${ProjDirPath}/../../../../../rtos/freertos/libraries/abstractions/platform/freertos/include/platform/iot_network_ble.h"
"${ProjDirPath}/../../../../../rtos/freertos/libraries/abstractions/platform/freertos/include/platform/iot_network_freertos.h"
"${ProjDirPath}/../../../../../rtos/freertos/libraries/abstractions/platform/freertos/include/platform/iot_platform_types_freertos.h"
"${ProjDirPath}/../../../../../rtos/freertos/libraries/abstractions/platform/freertos/iot_clock_freertos.c"
"${ProjDirPath}/../../../../../rtos/freertos/libraries/abstractions/platform/freertos/iot_metrics.c"
"${ProjDirPath}/../../../../../rtos/freertos/libraries/abstractions/platform/freertos/iot_network_freertos.c"
"${ProjDirPath}/../../../../../rtos/freertos/libraries/abstractions/platform/freertos/iot_threads_freertos.c"
"${ProjDirPath}/../../../../../rtos/freertos/libraries/abstractions/platform/include/platform/iot_clock.h"
"${ProjDirPath}/../../../../../rtos/freertos/libraries/abstractions/platform/include/platform/iot_metrics.h"
"${ProjDirPath}/../../../../../rtos/freertos/libraries/abstractions/platform/include/platform/iot_network.h"
"${ProjDirPath}/../../../../../rtos/freertos/libraries/abstractions/platform/include/platform/iot_threads.h"
"${ProjDirPath}/../../../../../rtos/freertos/libraries/abstractions/platform/include/types/iot_platform_types.h"
"${ProjDirPath}/../../../../../rtos/freertos/libraries/freertos_plus/standard/tls/include/iot_tls.h"
"${ProjDirPath}/../../../../../rtos/freertos/libraries/freertos_plus/standard/tls/src/iot_tls.c"
"${ProjDirPath}/../../../../../rtos/freertos/libraries/freertos_plus/standard/pkcs11/include/iot_pkcs11.h"
"${ProjDirPath}/../../../../../rtos/freertos/libraries/freertos_plus/standard/pkcs11/src/iot_pkcs11.c"
"${ProjDirPath}/../../../../../rtos/freertos/libraries/3rdparty/pkcs11/CONTRIBUTING.md"
"${ProjDirPath}/../../../../../rtos/freertos/libraries/3rdparty/pkcs11/LICENSE.md"
"${ProjDirPath}/../../../../../rtos/freertos/libraries/3rdparty/pkcs11/README.md"
"${ProjDirPath}/../../../../../rtos/freertos/libraries/3rdparty/pkcs11/pkcs11.h"
"${ProjDirPath}/../../../../../rtos/freertos/libraries/3rdparty/pkcs11/pkcs11f.h"
"${ProjDirPath}/../../../../../rtos/freertos/libraries/3rdparty/pkcs11/pkcs11t.h"
"${ProjDirPath}/../../../../../middleware/sln_iot_common_platform/security/aws_clientcredential.h"
"${ProjDirPath}/../../../../../middleware/sln_iot_common_platform/security/aws_clientcredential_keys.h"
"${ProjDirPath}/../../../../../middleware/sln_iot_common_platform/network/network_connection.c"
"${ProjDirPath}/../../../../../middleware/sln_iot_common_platform/network/network_connection.h"
"${ProjDirPath}/../../../../../middleware/sln_iot_common_platform/security/sln_iot_pkcs11_mbedtls.c"
"${ProjDirPath}/../../../../../middleware/sln_iot_common_platform/security/sln_iot_pkcs11_pal.c"
"${ProjDirPath}/../../../../../middleware/sln_iot_common_platform/network/wifi_credentials.c"
"${ProjDirPath}/../../../../../middleware/sln_iot_common_platform/network/wifi_credentials.h"
"${ProjDirPath}/../../../../../middleware/usb/device/usb_device.h"
"${ProjDirPath}/../../../../../middleware/usb/device/usb_device_dci.c"
"${ProjDirPath}/../../../../../middleware/usb/device/usb_device_dci.h"
"${ProjDirPath}/../../../../../components/osa/fsl_os_abstraction.h"
"${ProjDirPath}/../../../../../components/osa/fsl_os_abstraction_config.h"
"${ProjDirPath}/../../../../../components/osa/fsl_os_abstraction_free_rtos.c"
"${ProjDirPath}/../../../../../components/osa/fsl_os_abstraction_free_rtos.h"
"${ProjDirPath}/../../../../../middleware/usb/device/usb_device_ehci.c"
"${ProjDirPath}/../../../../../middleware/usb/device/usb_device_ehci.h"
"${ProjDirPath}/../../../../../middleware/usb/output/source/device/class/usb_device_class.c"
"${ProjDirPath}/../../../../../middleware/usb/output/source/device/class/usb_device_class.h"
"${ProjDirPath}/../../../../../middleware/usb/output/source/device/usb_device_ch9.c"
"${ProjDirPath}/../../../../../middleware/usb/output/source/device/usb_device_ch9.h"
"${ProjDirPath}/../usb_device_msc.c"
"${ProjDirPath}/../usb_device_msc.h"
"${ProjDirPath}/../usb_device_msc_ufi.c"
"${ProjDirPath}/../usb_device_msc_ufi.h"
"${ProjDirPath}/../usb_device_cdc_acm.c"
"${ProjDirPath}/../usb_device_cdc_acm.h"
"${ProjDirPath}/../../../../../middleware/usb/phy/usb_phy.c"
"${ProjDirPath}/../../../../../middleware/usb/phy/usb_phy.h"
"${ProjDirPath}/../../../../../rtos/freertos/libraries/c_sdk/aws/shadow/include/aws_iot_shadow.h"
"${ProjDirPath}/../../../../../rtos/freertos/libraries/c_sdk/aws/shadow/include/aws_shadow.h"
"${ProjDirPath}/../../../../../rtos/freertos/libraries/c_sdk/aws/shadow/include/types/aws_iot_shadow_types.h"
"${ProjDirPath}/../../../../../rtos/freertos/libraries/c_sdk/aws/shadow/src/aws_iot_shadow_api.c"
"${ProjDirPath}/../../../../../rtos/freertos/libraries/c_sdk/aws/shadow/src/aws_iot_shadow_operation.c"
"${ProjDirPath}/../../../../../rtos/freertos/libraries/c_sdk/aws/shadow/src/aws_iot_shadow_parser.c"
"${ProjDirPath}/../../../../../rtos/freertos/libraries/c_sdk/aws/shadow/src/aws_iot_shadow_static_memory.c"
"${ProjDirPath}/../../../../../rtos/freertos/libraries/c_sdk/aws/shadow/src/aws_iot_shadow_subscription.c"
"${ProjDirPath}/../../../../../rtos/freertos/libraries/c_sdk/aws/shadow/src/aws_shadow.c"
"${ProjDirPath}/../../../../../rtos/freertos/libraries/c_sdk/aws/shadow/src/aws_shadow_config_defaults.h"
"${ProjDirPath}/../../../../../rtos/freertos/libraries/c_sdk/aws/shadow/src/private/aws_iot_shadow_internal.h"
"${ProjDirPath}/../../../../../rtos/freertos/libraries/c_sdk/standard/common/include/iot_appversion32.h"
"${ProjDirPath}/../../../../../rtos/freertos/libraries/c_sdk/standard/common/include/iot_atomic.h"
"${ProjDirPath}/../../../../../rtos/freertos/libraries/c_sdk/standard/common/include/iot_init.h"
"${ProjDirPath}/../../../../../rtos/freertos/libraries/c_sdk/standard/common/include/iot_linear_containers.h"
"${ProjDirPath}/../../../../../rtos/freertos/libraries/c_sdk/standard/common/include/iot_logging_setup.h"
"${ProjDirPath}/../../../../../rtos/freertos/libraries/c_sdk/standard/common/include/iot_logging_task.h"
"${ProjDirPath}/../../../../../rtos/freertos/libraries/c_sdk/standard/common/include/iot_taskpool.h"
"${ProjDirPath}/../../../../../rtos/freertos/libraries/c_sdk/standard/common/include/private/iot_default_root_certificates.h"
"${ProjDirPath}/../../../../../rtos/freertos/libraries/c_sdk/standard/common/include/private/iot_doubly_linked_list.h"
"${ProjDirPath}/../../../../../rtos/freertos/libraries/c_sdk/standard/common/include/private/iot_error.h"
"${ProjDirPath}/../../../../../rtos/freertos/libraries/c_sdk/standard/common/include/private/iot_lib_init.h"
"${ProjDirPath}/../../../../../rtos/freertos/libraries/c_sdk/standard/common/include/private/iot_logging.h"
"${ProjDirPath}/../../../../../rtos/freertos/libraries/c_sdk/standard/common/include/private/iot_static_memory.h"
"${ProjDirPath}/../../../../../rtos/freertos/libraries/c_sdk/standard/common/include/private/iot_taskpool_internal.h"
"${ProjDirPath}/../../../../../rtos/freertos/libraries/c_sdk/standard/common/include/types/iot_network_types.h"
"${ProjDirPath}/../../../../../rtos/freertos/libraries/c_sdk/standard/common/include/types/iot_taskpool_types.h"
"${ProjDirPath}/../../../../../rtos/freertos/libraries/c_sdk/standard/common/iot_device_metrics.c"
"${ProjDirPath}/../../../../../rtos/freertos/libraries/c_sdk/standard/common/iot_init.c"
"${ProjDirPath}/../../../../../rtos/freertos/libraries/c_sdk/standard/common/iot_static_memory_common.c"
"${ProjDirPath}/../../../../../rtos/freertos/libraries/c_sdk/standard/common/logging/iot_logging.c"
"${ProjDirPath}/../../../../../rtos/freertos/libraries/c_sdk/standard/common/logging/iot_logging_task_dynamic_buffers.c"
"${ProjDirPath}/../../../../../rtos/freertos/libraries/c_sdk/standard/common/taskpool/iot_taskpool.c"
"${ProjDirPath}/../../../../../rtos/freertos/libraries/c_sdk/standard/common/taskpool/iot_taskpool_static_memory.c"
"${ProjDirPath}/../../../../../rtos/freertos/libraries/c_sdk/standard/mqtt/include/iot_mqtt.h"
"${ProjDirPath}/../../../../../rtos/freertos/libraries/c_sdk/standard/mqtt/include/iot_mqtt_agent.h"
"${ProjDirPath}/../../../../../rtos/freertos/libraries/c_sdk/standard/mqtt/include/iot_mqtt_agent_config_defaults.h"
"${ProjDirPath}/../../../../../rtos/freertos/libraries/c_sdk/standard/mqtt/include/iot_mqtt_config_defaults.h"
"${ProjDirPath}/../../../../../rtos/freertos/libraries/c_sdk/standard/mqtt/include/iot_mqtt_lib.h"
"${ProjDirPath}/../../../../../rtos/freertos/libraries/c_sdk/standard/mqtt/include/types/iot_mqtt_types.h"
"${ProjDirPath}/../../../../../rtos/freertos/libraries/c_sdk/standard/mqtt/src/iot_mqtt_agent.c"
"${ProjDirPath}/../../../../../rtos/freertos/libraries/c_sdk/standard/mqtt/src/iot_mqtt_api.c"
"${ProjDirPath}/../../../../../rtos/freertos/libraries/c_sdk/standard/mqtt/src/iot_mqtt_network.c"
"${ProjDirPath}/../../../../../rtos/freertos/libraries/c_sdk/standard/mqtt/src/iot_mqtt_operation.c"
"${ProjDirPath}/../../../../../rtos/freertos/libraries/c_sdk/standard/mqtt/src/iot_mqtt_serialize.c"
"${ProjDirPath}/../../../../../rtos/freertos/libraries/c_sdk/standard/mqtt/src/iot_mqtt_static_memory.c"
"${ProjDirPath}/../../../../../rtos/freertos/libraries/c_sdk/standard/mqtt/src/iot_mqtt_subscription.c"
"${ProjDirPath}/../../../../../rtos/freertos/libraries/c_sdk/standard/mqtt/src/iot_mqtt_validate.c"
"${ProjDirPath}/../../../../../rtos/freertos/libraries/c_sdk/standard/mqtt/src/private/iot_mqtt_internal.h"
"${ProjDirPath}/../../../../../rtos/freertos/libraries/c_sdk/standard/serializer/include/iot_json_utils.h"
"${ProjDirPath}/../../../../../rtos/freertos/libraries/c_sdk/standard/serializer/include/iot_serializer.h"
"${ProjDirPath}/../../../../../rtos/freertos/libraries/c_sdk/standard/serializer/src/iot_json_utils.c"
"${ProjDirPath}/../../../../../rtos/freertos/libraries/c_sdk/standard/serializer/src/iot_serializer_static_memory.c"
"${ProjDirPath}/../../../../../rtos/freertos/libraries/c_sdk/standard/serializer/src/json/iot_serializer_json_decoder.c"
"${ProjDirPath}/../../../../../rtos/freertos/libraries/c_sdk/standard/serializer/src/json/iot_serializer_json_encoder.c"
"${ProjDirPath}/../../../../../rtos/freertos/libraries/3rdparty/http_parser/http_parser.c"
"${ProjDirPath}/../../../../../rtos/freertos/libraries/3rdparty/http_parser/http_parser.h"
"${ProjDirPath}/../../../../../middleware/sdmmc/sdio/fsl_sdio.c"
"${ProjDirPath}/../../../../../middleware/sdmmc/sdio/fsl_sdio.h"
"${ProjDirPath}/../../../../../middleware/sdmmc/common/fsl_sdmmc_common.c"
"${ProjDirPath}/../../../../../middleware/sdmmc/common/fsl_sdmmc_common.h"
"${ProjDirPath}/../../../../../middleware/sdmmc/common/fsl_sdmmc_spec.h"
"${ProjDirPath}/../../../../../middleware/sdmmc/host/usdhc/fsl_sdmmc_host.h"
"${ProjDirPath}/../../../../../middleware/sdmmc/host/usdhc/fsl_sdmmc_host.h"
"${ProjDirPath}/../../../../../middleware/sdmmc/host/usdhc/non_blocking/fsl_sdmmc_host.c"
"${ProjDirPath}/../../../../../middleware/sln_iot_common_platform/custom_config/flash/is26kl256s/sln_flash_config.c"
"${ProjDirPath}/../../../../../middleware/sln_iot_common_platform/custom_config/flash/is26kl256s/sln_flash_config.h"
"${ProjDirPath}/../../../../../middleware/sln_iot_common_platform/drivers/sln_RT10xx_RGB_LED_driver_pwm.c"
"${ProjDirPath}/../../../../../devices/MIMXRT106A/drivers/fsl_lpi2c.c"
"${ProjDirPath}/../../../../../devices/MIMXRT106A/drivers/fsl_lpi2c.h"
"${ProjDirPath}/../../../../../devices/MIMXRT106A/drivers/fsl_clock.c"
"${ProjDirPath}/../../../../../devices/MIMXRT106A/drivers/fsl_clock.h"
"${ProjDirPath}/../../../../../devices/MIMXRT106A/drivers/fsl_enet.c"
"${ProjDirPath}/../../../../../devices/MIMXRT106A/drivers/fsl_enet.h"
"${ProjDirPath}/../../../../../devices/MIMXRT106A/drivers/fsl_usdhc.c"
"${ProjDirPath}/../../../../../devices/MIMXRT106A/drivers/fsl_usdhc.h"
"${ProjDirPath}/../../../../../devices/MIMXRT106A/drivers/fsl_xbara.c"
"${ProjDirPath}/../../../../../devices/MIMXRT106A/drivers/fsl_xbara.h"
"${ProjDirPath}/../../../../../devices/MIMXRT106A/drivers/fsl_pwm.c"
"${ProjDirPath}/../../../../../devices/MIMXRT106A/drivers/fsl_pwm.h"
"${ProjDirPath}/../../../../../devices/MIMXRT106A/drivers/fsl_pit.c"
"${ProjDirPath}/../../../../../devices/MIMXRT106A/drivers/fsl_pit.h"
"${ProjDirPath}/../../../../../devices/MIMXRT106A/drivers/fsl_gpt.c"
"${ProjDirPath}/../../../../../devices/MIMXRT106A/drivers/fsl_gpt.h"
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
"${ProjDirPath}/../../../../../rtos/freertos/freertos_kernel/portable/GCC/ARM_CM4F/port.c"
"${ProjDirPath}/../../../../../rtos/freertos/freertos_kernel/portable/GCC/ARM_CM4F/portmacro.h"
"${ProjDirPath}/../../../../../rtos/freertos/freertos_kernel/portable/MemMang/ReadMe.url"
"${ProjDirPath}/../../../../../rtos/freertos/freertos_kernel/queue.c"
"${ProjDirPath}/../../../../../rtos/freertos/freertos_kernel/stream_buffer.c"
"${ProjDirPath}/../../../../../rtos/freertos/freertos_kernel/tasks.c"
"${ProjDirPath}/../../../../../rtos/freertos/freertos_kernel/timers.c"
"${ProjDirPath}/../../../../../rtos/freertos/freertos_kernel/portable/MemMang/heap_4.c"
"${ProjDirPath}/../../../../../middleware/sln_iot_common_platform/flexspi/flexspi_hyper_flash_ops.c"
"${ProjDirPath}/../../../../../middleware/sln_iot_common_platform/flexspi/flexspi_hyper_flash_ops.h"
"${ProjDirPath}/../../../../../middleware/sln_iot_common_platform/security/sln_encrypt.c"
"${ProjDirPath}/../../../../../middleware/sln_iot_common_platform/security/sln_encrypt.h"
"${ProjDirPath}/../../../../../middleware/sln_iot_common_platform/security/sln_auth.c"
"${ProjDirPath}/../../../../../middleware/sln_iot_common_platform/security/sln_auth.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/aes.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/aesni.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/arc4.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/aria.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/asn1.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/asn1write.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/base64.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/bignum.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/blowfish.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/bn_mul.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/camellia.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/ccm.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/certs.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/chacha20.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/chachapoly.h"
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
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/hkdf.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/hmac_drbg.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/md.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/md2.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/md4.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/md5.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/md_internal.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/memory_buffer_alloc.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/net.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/net_sockets.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/nist_kw.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/oid.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/padlock.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/pem.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/pk.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/pk_internal.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/pkcs11.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/pkcs12.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/pkcs5.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/platform.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/platform_time.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/platform_util.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/poly1305.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/ripemd160.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/rsa.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/include/mbedtls/rsa_internal.h"
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
"${ProjDirPath}/../../../../../middleware/mbedtls/library/aria.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/asn1parse.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/asn1write.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/base64.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/bignum.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/blowfish.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/camellia.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/ccm.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/certs.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/chacha20.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/chachapoly.c"
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
"${ProjDirPath}/../../../../../middleware/mbedtls/library/hkdf.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/hmac_drbg.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/md.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/md2.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/md4.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/md5.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/md_wrap.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/memory_buffer_alloc.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/net_sockets.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/nist_kw.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/oid.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/padlock.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/pem.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/pk.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/pk_wrap.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/pkcs11.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/pkcs12.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/pkcs5.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/pkparse.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/pkwrite.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/platform.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/platform_util.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/poly1305.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/ripemd160.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/rsa.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/library/rsa_internal.c"
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
"${ProjDirPath}/../../../../../middleware/mbedtls/port/ksdk/aes_alt.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/port/ksdk/aes_alt.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/port/ksdk/des_alt.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/port/ksdk/des_alt.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/port/ksdk/ecp_alt.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/port/ksdk/ecp_alt.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/port/ksdk/ecp_alt_ksdk.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/port/ksdk/ecp_curves_alt.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/port/ksdk/ksdk_mbedtls.c"
"${ProjDirPath}/../../../../../middleware/mbedtls/port/ksdk/ksdk_mbedtls.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/port/ksdk/ksdk_mbedtls_config.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/port/ksdk/sha1_alt.h"
"${ProjDirPath}/../../../../../middleware/mbedtls/port/ksdk/sha256_alt.h"
"${ProjDirPath}/../../../../../middleware/sln_iot_common_platform/boot_code/common/fica_definition.h"
"${ProjDirPath}/../../../../../middleware/sln_iot_common_platform/boot_code/common/flash_ica_driver.c"
"${ProjDirPath}/../../../../../middleware/sln_iot_common_platform/boot_code/common/flash_ica_driver.h"
"${ProjDirPath}/../../../../../middleware/sln_iot_common_platform/drivers/sln_RT10xx_RGB_LED_driver.c"
"${ProjDirPath}/../../../../../middleware/sln_iot_common_platform/drivers/sln_RT10xx_RGB_LED_driver.h"
"${ProjDirPath}/../../../../../middleware/sln_iot_common_platform/utils/device_utils.c"
"${ProjDirPath}/../../../../../middleware/sln_iot_common_platform/utils/device_utils.h"
"${ProjDirPath}/../../../../../middleware/sln_iot_common_platform/network/mqtt_connection.c"
"${ProjDirPath}/../../../../../middleware/sln_iot_common_platform/network/mqtt_connection.h"
"${ProjDirPath}/../../../../../rtos/freertos/demos/include/aws_wifi_connect_task.h"
"${ProjDirPath}/../../../../../rtos/freertos/demos/include/iot_ble_numericComparison.h"
"${ProjDirPath}/../../../../../rtos/freertos/demos/network_manager/aws_iot_network_manager.c"
"${ProjDirPath}/../../../../../rtos/freertos/demos/network_manager/iot_network_manager_private.h"
"${ProjDirPath}/../../../../../rtos/freertos/libraries/abstractions/wifi/include/iot_wifi.h"
"${ProjDirPath}/../../../../../devices/MIMXRT106A/drivers/fsl_common.c"
"${ProjDirPath}/../../../../../devices/MIMXRT106A/drivers/fsl_common.h"
"${ProjDirPath}/../../../../../devices/MIMXRT106A/MIMXRT106A.h"
"${ProjDirPath}/../../../../../devices/MIMXRT106A/MIMXRT106A_features.h"
"${ProjDirPath}/../../../../../devices/MIMXRT106A/fsl_device_registers.h"
"${ProjDirPath}/../../../../../components/uart/lpuart_adapter.c"
"${ProjDirPath}/../../../../../components/uart/uart.h"
"${ProjDirPath}/../../../../../components/serial_manager/serial_manager.c"
"${ProjDirPath}/../../../../../components/serial_manager/serial_manager.h"
"${ProjDirPath}/../../../../../components/serial_manager/serial_port_internal.h"
"${ProjDirPath}/../../../../../components/lists/generic_list.c"
"${ProjDirPath}/../../../../../components/lists/generic_list.h"
"${ProjDirPath}/../../../../../components/serial_manager/serial_port_uart.c"
"${ProjDirPath}/../../../../../components/serial_manager/serial_port_uart.h"
"${ProjDirPath}/../../../../../devices/MIMXRT106A/drivers/fsl_lpuart.c"
"${ProjDirPath}/../../../../../devices/MIMXRT106A/drivers/fsl_lpuart.h"
"${ProjDirPath}/../../../../../devices/MIMXRT106A/gcc/startup_MIMXRT106A.c"
"${ProjDirPath}/../../../../../devices/MIMXRT106A/system_MIMXRT106A.c"
"${ProjDirPath}/../../../../../devices/MIMXRT106A/system_MIMXRT106A.h"
"${ProjDirPath}/../../../../../devices/MIMXRT106A/drivers/fsl_iomuxc.h"
"${ProjDirPath}/../../../../../devices/MIMXRT106A/utilities/fsl_assert.c"
"${ProjDirPath}/../../../../../devices/MIMXRT106A/drivers/fsl_gpio.c"
"${ProjDirPath}/../../../../../devices/MIMXRT106A/drivers/fsl_gpio.h"
"${ProjDirPath}/../../../../../devices/MIMXRT106A/xip/fsl_flexspi_nor_boot.c"
"${ProjDirPath}/../../../../../devices/MIMXRT106A/xip/fsl_flexspi_nor_boot.h"
"${ProjDirPath}/../../../xip/sln_alexa_iot_flexspi_nor_config.c"
"${ProjDirPath}/../../../xip/sln_alexa_iot_flexspi_nor_config.h"
"${ProjDirPath}/../../../../../devices/MIMXRT106A/drivers/fsl_dcp.c"
"${ProjDirPath}/../../../../../devices/MIMXRT106A/drivers/fsl_dcp.h"
"${ProjDirPath}/../../../../../devices/MIMXRT106A/drivers/fsl_trng.c"
"${ProjDirPath}/../../../../../devices/MIMXRT106A/drivers/fsl_trng.h"
"${ProjDirPath}/../../../../../devices/MIMXRT106A/drivers/fsl_flexram_allocate.c"
"${ProjDirPath}/../../../../../devices/MIMXRT106A/drivers/fsl_flexram_allocate.h"
"${ProjDirPath}/../../../../../devices/MIMXRT106A/drivers/fsl_flexram.c"
"${ProjDirPath}/../../../../../devices/MIMXRT106A/drivers/fsl_flexram.h"
"${ProjDirPath}/../../../../../devices/MIMXRT106A/drivers/fsl_flexspi.c"
"${ProjDirPath}/../../../../../devices/MIMXRT106A/drivers/fsl_flexspi.h"
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
"${ProjDirPath}/../../../../../rtos/freertos/libraries/3rdparty/tinycbor/LICENSE"
"${ProjDirPath}/../../../../../rtos/freertos/libraries/3rdparty/tinycbor/README.md"
"${ProjDirPath}/../../../../../rtos/freertos/libraries/3rdparty/tinycbor/cbor.h"
"${ProjDirPath}/../../../../../rtos/freertos/libraries/3rdparty/tinycbor/cborencoder.c"
"${ProjDirPath}/../../../../../rtos/freertos/libraries/3rdparty/tinycbor/cborencoder_close_container_checked.c"
"${ProjDirPath}/../../../../../rtos/freertos/libraries/3rdparty/tinycbor/cborerrorstrings.c"
"${ProjDirPath}/../../../../../rtos/freertos/libraries/3rdparty/tinycbor/cborinternal_p.h"
"${ProjDirPath}/../../../../../rtos/freertos/libraries/3rdparty/tinycbor/cborparser.c"
"${ProjDirPath}/../../../../../rtos/freertos/libraries/3rdparty/tinycbor/cborparser_dup_string.c"
"${ProjDirPath}/../../../../../rtos/freertos/libraries/3rdparty/tinycbor/cborvalidation.c"
"${ProjDirPath}/../../../../../rtos/freertos/libraries/3rdparty/tinycbor/compilersupport_p.h"
"${ProjDirPath}/../../../../../rtos/freertos/libraries/3rdparty/tinycbor/tinycbor-version.h"
"${ProjDirPath}/../../../../../rtos/freertos/libraries/3rdparty/tinycbor/utf8_p.h"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD/internal/chips/4343W/4343WA1.h"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD/internal/chips/4343W/4343WA1_clm.h"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD/internal/chips/4343W/4343WA1_clm_mfg.h"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD/internal/chips/4343W/4343WA1_mfg.h"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD/internal/chips/4343W/chip_constants.h"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD/internal/chips/4343W/resources.c"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD/internal/chips/4343W/resources.h"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD/internal/chips/4343W/wwd_ap.c"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD/internal/chips/4343W/wwd_chip_specific_functions.c"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/RTOS/FreeRTOS/WWD/wwd_FreeRTOS_systick.h"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/RTOS/FreeRTOS/WWD/wwd_rtos.c"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/RTOS/FreeRTOS/WWD/wwd_rtos.h"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD/include/RTOS/wwd_rtos_interface.h"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD/include/network/wwd_buffer_interface.h"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD/include/network/wwd_network_constants.h"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD/include/network/wwd_network_interface.h"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD/include/platform/wwd_bus_interface.h"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD/include/platform/wwd_platform_interface.h"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD/include/platform/wwd_resource_interface.h"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD/include/platform/wwd_sdio_interface.h"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD/include/platform/wwd_spi_interface.h"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD/include/wwd_assert.h"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD/include/wwd_constants.h"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD/include/wwd_debug.h"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD/include/wwd_eapol.h"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD/include/wwd_events.h"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD/include/wwd_management.h"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD/include/wwd_poll.h"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD/include/wwd_structures.h"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD/include/wwd_wifi.h"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD/include/wwd_wifi_sleep.h"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD/include/wwd_wlioctl.h"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD/internal/bus_protocols/SDIO/wwd_bus_protocol.c"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD/internal/bus_protocols/SDIO/wwd_bus_protocol.h"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD/internal/bus_protocols/wwd_bus_common.c"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD/internal/bus_protocols/wwd_bus_protocol_interface.h"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD/internal/tlv.c"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD/internal/tlv.h"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD/internal/wwd_ap.h"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD/internal/wwd_ap_common.c"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD/internal/wwd_ap_common.h"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD/internal/wwd_bcmendian.h"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD/internal/wwd_clm.c"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD/internal/wwd_clm.h"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD/internal/wwd_debug.c"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD/internal/wwd_eapol.c"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD/internal/wwd_internal.c"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD/internal/wwd_internal.h"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD/internal/wwd_logging.h"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD/internal/wwd_management.c"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD/internal/wwd_rtos_interface.c"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD/internal/wwd_sdpcm.c"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD/internal/wwd_sdpcm.h"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD/internal/wwd_thread.c"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD/internal/wwd_thread.h"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD/internal/wwd_thread_internal.c"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD/internal/wwd_thread_internal.h"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD/internal/wwd_wifi.c"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD/internal/wwd_wifi_chip_common.c"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD/internal/wwd_wifi_chip_common.h"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD/internal/wwd_wifi_sleep.c"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD/wwd.h"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/WWD/wwd_wiced.h"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/internal/wifi.c"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/network/LwIP/WWD/wwd_buffer.c"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/network/LwIP/WWD/wwd_buffer.h"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/network/LwIP/WWD/wwd_network.c"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/network/LwIP/WWD/wwd_network.h"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/platform/MCU/LPC/WWD/wwd_SDIO.c"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/platform/MCU/LPC/WWD/wwd_platform.c"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/platform/MCU/LPC/platform_mcu_peripheral.h"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/platform/MCU/wwd_platform_common.h"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/platform/MCU/wwd_platform_separate_mcu.c"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/platform/MCU/wwd_resources.c"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/platform/include/platform_cache_def.h"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/platform/include/platform_peripheral.h"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/WICED/platform/platform_toolchain.h"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/generated_mac_address.txt"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/include/wiced_resource.h"
"${ProjDirPath}/../../../../../rtos/freertos/freertos_kernel/include/freertos_tasks_c_additions.h"
"${ProjDirPath}/../../../../../middleware/sln_iot_common_platform/flash_mgmt/sln_flash_mgmt.c"
"${ProjDirPath}/../../../../../middleware/sln_iot_common_platform/flash_mgmt/sln_flash_mgmt.h"
"${ProjDirPath}/../../../../../middleware/sln_iot_common_platform/flash_mgmt/sln_flash.c"
"${ProjDirPath}/../../../../../middleware/sln_iot_common_platform/flash_mgmt/sln_flash.h"
"${ProjDirPath}/../../../../../middleware/usb/include/usb.h"
"${ProjDirPath}/../../../../../middleware/usb/include/usb_misc.h"
"${ProjDirPath}/../../../../../middleware/usb/include/usb_spec.h"
"${ProjDirPath}/../../../../../middleware/usb/device/usb_device.h"
"${ProjDirPath}/../../../../../middleware/usb/device/usb_device_dci.h"
"${ProjDirPath}/../../../../../middleware/sdmmc/osa/fsl_sdmmc_osa.c"
"${ProjDirPath}/../../../../../middleware/sdmmc/osa/fsl_sdmmc_osa.h"
"${ProjDirPath}/../../../../../components/osa/fsl_os_abstraction.h"
"${ProjDirPath}/../../../../../components/osa/fsl_os_abstraction_config.h"
"${ProjDirPath}/../../../../../components/osa/fsl_os_abstraction_free_rtos.c"
"${ProjDirPath}/../../../../../components/osa/fsl_os_abstraction_free_rtos.h"
"${ProjDirPath}/../../../../../devices/MIMXRT106A/utilities/fsl_sbrk.c"
)


set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -T${ProjDirPath}/sln_alexa_iot_bootloader_xip_oc_no_cache.ld -static")

set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -T${ProjDirPath}/sln_alexa_iot_bootloader_xip_oc_no_cache.ld -static")

TARGET_LINK_LIBRARIES(bootloader_43438.elf -Wl,--start-group)
target_link_libraries(bootloader_43438.elf debug m)

target_link_libraries(bootloader_43438.elf debug c)

target_link_libraries(bootloader_43438.elf debug gcc)

target_link_libraries(bootloader_43438.elf debug nosys)

target_link_libraries(bootloader_43438.elf optimized m)

target_link_libraries(bootloader_43438.elf optimized c)

target_link_libraries(bootloader_43438.elf optimized gcc)

target_link_libraries(bootloader_43438.elf optimized nosys)

TARGET_LINK_LIBRARIES(bootloader_43438.elf -Wl,--end-group)


