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

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DLWIP_TIMEVAL_PRIVATE=0")

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

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DUSB_STACK_FREERTOS")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DBOARD_USE_VIRTUALCOM=1")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG_CONSOLE_IO_USBCDC=1")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG_CONSOLE_TRANSFER_NON_BLOCKING")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DSERIAL_PORT_TYPE_USBCDC=1")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DUSB_CDC_SERIAL_MANAGER_RUN_NO_HOST=1")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DUSE_COMMON_CODEC=1")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DRELOCATE_VECTOR_TABLE=1")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DHAL_UART_TRANSFER_MODE=1")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DAPP_MAJ_VER=0x01")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DAPP_MIN_VER=0x01")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DAPP_BLD_VER=0x0001")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -D__SEMIHOST_HARDFAULT_DISABLE=1")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DSERIAL_PORT_TYPE_USBCDC=1")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DSERIAL_MANAGER_NON_BLOCKING_MODE=1")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DUART1_BLE_SERIAL_MANANGER_BLOCKING_MODE=1")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DUSB_STACK_FREERTOS_HEAP_SIZE=65536")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DARM_MATH_CM7")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DSDK_DEBUGCONSOLE=1")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DCR_INTEGER_PRINTF")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DSDK_I2C_BASED_COMPONENT_USED=1")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DSDK_SAI_BASED_COMPONENT_USED=1")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DUSE_TFA9894_PUI=1")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DUSE_RTOS=1")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DWICED_BLUETOOTH_PLATFORM")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DFSL_RTOS_FREE_RTOS")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DLWIP_DNS=1")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DLWIP_DHCP=1")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DMBEDTLS_CONFIG_FILE='\"aws_mbedtls_config.h\"'")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DAWS_LOGGING_INCLUDE='\"iot_logging_task.h\"'")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DAWS_SYS_INIT_INCLUDE='\"iot_system_init.h\"'")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DAWS_MQTT_AGENT_INCLUDE='\"iot_mqtt_agent.h\"'")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DAWS_VERSION_INCLUDE='\"iot_appversion32.h\"'")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEMO_NETWORK_TYPE=AWSIOT_NETWORK_TYPE_WIFI")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DCONFIG_FLEXRAM_AT_STARTUP=1")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DUSB_DEVICE_CONFIG_CDC_ACM=1")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG_CONSOLE_RX_ENABLE=0")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DSDIO_ENABLED")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DSDK_DEBUGCONSOLE_UART")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DSDK_I2C_BASED_COMPONENT_USED=1")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DBOARD_USE_CODEC=1")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DCODEC_TFA9XXX_ENABLE")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DSLN_AFE_LIB")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DSLN_DSP_TOOLBOX_LIB")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DSERIAL_PORT_TYPE_UART=1")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DWIFI_WICED")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O2")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -mno-unaligned-access")

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

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DLWIP_TIMEVAL_PRIVATE=0")

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

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DUSB_STACK_FREERTOS")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DBOARD_USE_VIRTUALCOM=1")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DDEBUG_CONSOLE_IO_USBCDC=1")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DDEBUG_CONSOLE_TRANSFER_NON_BLOCKING")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DSERIAL_PORT_TYPE_USBCDC=1")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DUSB_CDC_SERIAL_MANAGER_RUN_NO_HOST=1")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DUSE_COMMON_CODEC=1")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DRELOCATE_VECTOR_TABLE=1")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DHAL_UART_TRANSFER_MODE=1")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DAPP_MAJ_VER=0x01")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DAPP_MIN_VER=0x01")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DAPP_BLD_VER=0x0001")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -D__SEMIHOST_HARDFAULT_DISABLE=1")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DSERIAL_PORT_TYPE_USBCDC=1")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DSERIAL_MANAGER_NON_BLOCKING_MODE=1")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DUART1_BLE_SERIAL_MANANGER_BLOCKING_MODE=1")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DUSB_STACK_FREERTOS_HEAP_SIZE=65536")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DARM_MATH_CM7")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DSDK_DEBUGCONSOLE=1")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DCR_INTEGER_PRINTF")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DSDK_I2C_BASED_COMPONENT_USED=1")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DSDK_SAI_BASED_COMPONENT_USED=1")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DUSE_TFA9894_PUI=1")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DUSE_RTOS=1")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DWICED_BLUETOOTH_PLATFORM")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DFSL_RTOS_FREE_RTOS")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DLWIP_DNS=1")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DLWIP_DHCP=1")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DMBEDTLS_CONFIG_FILE='\"aws_mbedtls_config.h\"'")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DAWS_LOGGING_INCLUDE='\"iot_logging_task.h\"'")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DAWS_SYS_INIT_INCLUDE='\"iot_system_init.h\"'")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DAWS_MQTT_AGENT_INCLUDE='\"iot_mqtt_agent.h\"'")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DAWS_VERSION_INCLUDE='\"iot_appversion32.h\"'")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DDEMO_NETWORK_TYPE=AWSIOT_NETWORK_TYPE_WIFI")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DCONFIG_FLEXRAM_AT_STARTUP=1")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DUSB_DEVICE_CONFIG_CDC_ACM=1")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DDEBUG_CONSOLE_RX_ENABLE=0")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DSDIO_ENABLED")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DSDK_DEBUGCONSOLE_UART")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DSDK_I2C_BASED_COMPONENT_USED=1")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DBOARD_USE_CODEC=1")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DCODEC_TFA9XXX_ENABLE")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DSLN_AFE_LIB")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DSLN_DSP_TOOLBOX_LIB")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DSERIAL_PORT_TYPE_UART=1")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DWIFI_WICED")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Os")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O2")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -g")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -mno-unaligned-access")

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

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DSERIAL_PORT_TYPE_USBCDC=1")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG_CONSOLE_TRANSFER_NON_BLOCKING")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DUSB_DEVICE_CONFIG_CDC_ACM=1")

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

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DSERIAL_PORT_TYPE_USBCDC=1")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DDEBUG_CONSOLE_TRANSFER_NON_BLOCKING")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DUSB_DEVICE_CONFIG_CDC_ACM=1")

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

include_directories(${ProjDirPath}/../../../common)

include_directories(${ProjDirPath}/..)

include_directories(${ProjDirPath}/../../../../../middleware/sln_iot_common_platform/security)

include_directories(${ProjDirPath}/../../ais_demo)

include_directories(${ProjDirPath}/../config_files)

include_directories(${ProjDirPath}/../../../../src/sln_voice_examples/common)

include_directories(${ProjDirPath}/../../../../../platform/utilities/debug_console)

include_directories(${ProjDirPath}/../../../../../platform/utilities/debug_console/str)

include_directories(${ProjDirPath}/../../../../../platform/utilities/shell)

include_directories(${ProjDirPath}/../../../../../rtos/freertos/libraries/abstractions/pkcs11/mbedtls)

include_directories(${ProjDirPath}/../../../../../rtos/freertos/libraries/c_sdk/standard/common/include/types)

include_directories(${ProjDirPath}/../../../../../rtos/freertos/libraries/c_sdk/standard/mqtt/include/types)

include_directories(${ProjDirPath}/../../../../../rtos/freertos/libraries/abstractions/pkcs11/include)

include_directories(${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/app)

include_directories(${ProjDirPath}/../../..)

include_directories(${ProjDirPath}/../../../../../CMSIS/Include)

include_directories(${ProjDirPath}/../../../../../devices)

include_directories(${ProjDirPath}/../../../../../rtos/freertos/freertos_kernel/include)

include_directories(${ProjDirPath}/../../../../../middleware/mbedtls/include)

include_directories(${ProjDirPath}/../../../../../middleware/mbedtls/port/ksdk)

include_directories(${ProjDirPath}/../../../../../rtos/freertos/libraries/freertos_plus/standard/utils/include)

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

include_directories(${ProjDirPath}/../../../../../rtos/freertos/demos/include)

include_directories(${ProjDirPath}/../../../../../rtos/freertos/demos/network_manager)

include_directories(${ProjDirPath}/../../../../../rtos/freertos/libraries/abstractions/wifi/include)

include_directories(${ProjDirPath}/../../../../src/sln_voice_examples/ais_demo/config_files)

include_directories(${ProjDirPath}/../../../../src/sln_voice_examples/ais_demo/config_files/wifi)

include_directories(${ProjDirPath}/../../../../../components/serial_manager)

include_directories(${ProjDirPath}/../../../../../devices/MIMXRT106A/drivers)

include_directories(${ProjDirPath}/../../../../../components/lists)

include_directories(${ProjDirPath}/../../../../../components/serial_manager/usb_cdc_adapter)

include_directories(${ProjDirPath}/../../../../../middleware/usb/phy)

include_directories(${ProjDirPath}/../../../../../components/osa)

include_directories(${ProjDirPath}/../../../../../middleware/usb/device)

include_directories(${ProjDirPath}/../../../../../middleware/usb/include)

include_directories(${ProjDirPath}/../../../../../middleware/sln_iot_common_platform/custom_config/flash/is26kl256s)

include_directories(${ProjDirPath}/../../../../../middleware/sln_iot_common_platform/drivers)

include_directories(${ProjDirPath}/../../../../../devices/MIMXRT106A/utilities)

include_directories(${ProjDirPath}/../../../../../middleware/lwip/port)

include_directories(${ProjDirPath}/../../../../../middleware/lwip/src)

include_directories(${ProjDirPath}/../../../../../middleware/lwip/src/include)

include_directories(${ProjDirPath}/../../../../../devices/MIMXRT106A/utilities/str)

include_directories(${ProjDirPath}/../../../../../devices/MIMXRT106A/utilities/debug_console)

include_directories(${ProjDirPath}/../../../../../middleware/wiced/43xxx_Wi-Fi/platforms/AW_NM372SM)

include_directories(${ProjDirPath}/../../../../../middleware/sln_iot_common_platform/network)

include_directories(${ProjDirPath}/../../../../../middleware/sdmmc/sdio)

include_directories(${ProjDirPath}/../../../../../middleware/sdmmc/common)

include_directories(${ProjDirPath}/../../../../../middleware/sdmmc/host/usdhc)

include_directories(${ProjDirPath}/../../../../../middleware/wiced/43xxx_BLE/bt_app_inc)

include_directories(${ProjDirPath}/../../../../../middleware/wiced/43xxx_BLE/wiced_bt/BTE/WICED)

include_directories(${ProjDirPath}/../../../../../middleware/wiced/43xxx_BLE/wiced_bt/BTE/Components/stack/include)

include_directories(${ProjDirPath}/../../../../../middleware/wiced/43xxx_BLE/wiced_bt/BTE/Projects/bte/main)

include_directories(${ProjDirPath}/../../../../../middleware/wiced/43xxx_BLE/wiced_bt/imxrt_port)

include_directories(${ProjDirPath}/../../../../../components/codec/i2c)

include_directories(${ProjDirPath}/../../../../../components/i2c)

include_directories(${ProjDirPath}/../../../../../middleware/cjson)

include_directories(${ProjDirPath}/../../../../../middleware/amzn_ww/source)

include_directories(${ProjDirPath}/../../../../../middleware/amzn_ww/cortexM7)

include_directories(${ProjDirPath}/../../../../../components/codec/tfa9xxx)

include_directories(${ProjDirPath}/../../../../../components/codec/tfa9xxx/vas_tfa_drv)

include_directories(${ProjDirPath}/../../../../../components/codec)

include_directories(${ProjDirPath}/../../../../../components/codec/port/tfa9xxx)

include_directories(${ProjDirPath}/../../../../../middleware/sln_iot_common_platform/audio)

include_directories(${ProjDirPath}/../../../../../middleware/aws_ais)

include_directories(${ProjDirPath}/../../../../../middleware/audio_streamer/inc)

include_directories(${ProjDirPath}/../../../../../middleware/audio_streamer/mcu-libs/osa)

include_directories(${ProjDirPath}/../../../../../middleware/audio_streamer/mcu-libs/osa/freertos)

include_directories(${ProjDirPath}/../../../../../middleware/audio_streamer/mcu-libs/common)

include_directories(${ProjDirPath}/../../../../../middleware/audio_streamer/streamer/inc)

include_directories(${ProjDirPath}/../../../../../middleware/sln_iot_common_platform/utils)

include_directories(${ProjDirPath}/../../../../../middleware/mcu_voice_libs_public/afe)

include_directories(${ProjDirPath}/../../../../../middleware/mcu_voice_libs_public/dsp)

include_directories(${ProjDirPath}/../../../../../middleware/alexa_ux_audio)

include_directories(${ProjDirPath}/../../../../../devices/MIMXRT106A)

include_directories(${ProjDirPath}/../../../../../components/uart)

include_directories(${ProjDirPath}/../../../../../devices/MIMXRT106A/xip)

include_directories(${ProjDirPath}/../../../xip)

include_directories(${ProjDirPath}/../../../../../middleware/usb/output/source/device/class)

include_directories(${ProjDirPath}/../../../../../middleware/usb/output/source/device)

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

include_directories(${ProjDirPath}/../../../../../middleware/sln_iot_common_platform/flexspi)

include_directories(${ProjDirPath}/../../../../../middleware/sdmmc/osa)

add_executable(ais_demo_43438.elf 
"${ProjDirPath}/../../../tfa_config_TFA9894N2A1_48kHz_PUI.h"
"${ProjDirPath}/../../../common/sln_file_table.h"
"${ProjDirPath}/../../../common/sln_cfg_file.h"
"${ProjDirPath}/../../../source/perf.c"
"${ProjDirPath}/../threading_alt.h"
"${ProjDirPath}/../iot_wifi.c"
"${ProjDirPath}/../iot_pkcs11_pal.h"
"${ProjDirPath}/../iot_demo_logging.h"
"${ProjDirPath}/../aws_demo.h"
"${ProjDirPath}/../iot_freertos_init.c"
"${ProjDirPath}/../sln_iot_pkcs11_pal.c"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../main.c"
"${ProjDirPath}/../../../sdmmc_config.c"
"${ProjDirPath}/../../../sdmmc_config.h"
"${ProjDirPath}/../config_files/iot_config_common.h"
"${ProjDirPath}/../config_files/FreeRTOSConfig.h"
"${ProjDirPath}/../config_files/FreeRTOSIPConfig.h"
"${ProjDirPath}/../config_files/audio.conf.h"
"${ProjDirPath}/../config_files/audio_cfg.h"
"${ProjDirPath}/../config_files/aws_iot_network_config.h"
"${ProjDirPath}/../config_files/aws_mbedtls_config.h"
"${ProjDirPath}/../config_files/aws_mqtt_config.h"
"${ProjDirPath}/../config_files/aws_demo_config.h"
"${ProjDirPath}/../config_files/aws_secure_sockets_config.h"
"${ProjDirPath}/../config_files/aws_shadow_config.h"
"${ProjDirPath}/../config_files/aws_wifi_config.h"
"${ProjDirPath}/../config_files/iot_config.h"
"${ProjDirPath}/../config_files/iot_mqtt_agent_config.h"
"${ProjDirPath}/../config_files/iot_pkcs11_config.h"
"${ProjDirPath}/../config_files/os_cfg.h"
"${ProjDirPath}/../config_files/pdm_pcm_definitions.h"
"${ProjDirPath}/../config_files/wiced_bt_cfg.c"
"${ProjDirPath}/../config_files/lwipopts.h"
"${ProjDirPath}/../app_aws_shadow.c"
"${ProjDirPath}/../app_aws_shadow.h"
"${ProjDirPath}/../aws_iot_demo_shadow.c"
"${ProjDirPath}/../dhcp_server.c"
"${ProjDirPath}/../dhcp_server.h"
"${ProjDirPath}/../fault_handlers.c"
"${ProjDirPath}/../fault_handlers.h"
"${ProjDirPath}/../aisv2_app.c"
"${ProjDirPath}/../aisv2_app.h"
"${ProjDirPath}/../ais_alerts.c"
"${ProjDirPath}/../ais_alerts.h"
"${ProjDirPath}/../reconnection_task.c"
"${ProjDirPath}/../reconnection_task.h"
"${ProjDirPath}/../ais_authentication_task.c"
"${ProjDirPath}/../ais_authentication_task.h"
"${ProjDirPath}/../udp_message_handler.c"
"${ProjDirPath}/../udp_message_handler.h"
"${ProjDirPath}/../udp_server.c"
"${ProjDirPath}/../udp_server.h"
"${ProjDirPath}/../httpsclient.c"
"${ProjDirPath}/../httpsclient.h"
"${ProjDirPath}/../streamer_pcm.c"
"${ProjDirPath}/../streamer_pcm.h"
"${ProjDirPath}/../ais_continuous_utterance.c"
"${ProjDirPath}/../ais_continuous_utterance.h"
"${ProjDirPath}/../audio_processing_task.c"
"${ProjDirPath}/../audio_processing_task.h"
"${ProjDirPath}/../ux_attention_system.c"
"${ProjDirPath}/../ux_attention_system.h"
"${ProjDirPath}/../switch.c"
"${ProjDirPath}/../switch.h"
"${ProjDirPath}/../aws_ota_check.c"
"${ProjDirPath}/../aws_ota_check.h"
"${ProjDirPath}/../ble_connection.c"
"${ProjDirPath}/../ble_connection.h"
"${ProjDirPath}/../ble_server.c"
"${ProjDirPath}/../ble_server.h"
"${ProjDirPath}/../sln_shell.c"
"${ProjDirPath}/../sln_shell.h"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
"${ProjDirPath}/../dcd.c"
"${ProjDirPath}/../dcd.h"
"${ProjDirPath}/../../../../../components/serial_manager/serial_manager.c"
"${ProjDirPath}/../../../../../components/serial_manager/serial_manager.h"
"${ProjDirPath}/../../../../../components/serial_manager/serial_port_internal.h"
"${ProjDirPath}/../../../../../devices/MIMXRT106A/drivers/fsl_common.c"
"${ProjDirPath}/../../../../../devices/MIMXRT106A/drivers/fsl_common.h"
"${ProjDirPath}/../../../../../components/lists/generic_list.c"
"${ProjDirPath}/../../../../../components/lists/generic_list.h"
"${ProjDirPath}/../../../../../components/serial_manager/serial_port_usb.c"
"${ProjDirPath}/../../../../../components/serial_manager/serial_port_usb.h"
"${ProjDirPath}/../../../../../components/serial_manager/usb_cdc_adapter/usb_device_descriptor.c"
"${ProjDirPath}/../../../../../components/serial_manager/usb_cdc_adapter/usb_device_descriptor.h"
"${ProjDirPath}/../usb_device_config.h"
"${ProjDirPath}/../../../../../middleware/usb/phy/usb_phy.c"
"${ProjDirPath}/../../../../../middleware/usb/phy/usb_phy.h"
"${ProjDirPath}/../../../../../middleware/usb/device/usb_device.h"
"${ProjDirPath}/../../../../../middleware/usb/device/usb_device_dci.c"
"${ProjDirPath}/../../../../../middleware/usb/device/usb_device_dci.h"
"${ProjDirPath}/../../../../../components/osa/fsl_os_abstraction.h"
"${ProjDirPath}/../../../../../components/osa/fsl_os_abstraction_config.h"
"${ProjDirPath}/../../../../../components/osa/fsl_os_abstraction_free_rtos.c"
"${ProjDirPath}/../../../../../components/osa/fsl_os_abstraction_free_rtos.h"
"${ProjDirPath}/../../../../../middleware/usb/device/usb_device_ehci.c"
"${ProjDirPath}/../../../../../middleware/usb/device/usb_device_ehci.h"
"${ProjDirPath}/../../../../../devices/MIMXRT106A/drivers/fsl_cache.c"
"${ProjDirPath}/../../../../../devices/MIMXRT106A/drivers/fsl_cache.h"
"${ProjDirPath}/../../../../../devices/MIMXRT106A/drivers/fsl_gpt.c"
"${ProjDirPath}/../../../../../devices/MIMXRT106A/drivers/fsl_gpt.h"
"${ProjDirPath}/../../../../../middleware/sln_iot_common_platform/custom_config/flash/is26kl256s/sln_flash_config.c"
"${ProjDirPath}/../../../../../middleware/sln_iot_common_platform/custom_config/flash/is26kl256s/sln_flash_config.h"
"${ProjDirPath}/../../../../../middleware/sln_iot_common_platform/drivers/sln_RT10xx_RGB_LED_driver_pwm.c"
"${ProjDirPath}/../../../../../devices/MIMXRT106A/utilities/fsl_shell.c"
"${ProjDirPath}/../../../../../devices/MIMXRT106A/utilities/fsl_shell.h"
"${ProjDirPath}/../../../../../devices/MIMXRT106A/drivers/fsl_pit.c"
"${ProjDirPath}/../../../../../devices/MIMXRT106A/drivers/fsl_pit.h"
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
"${ProjDirPath}/../../../../../rtos/freertos/libraries/freertos_plus/standard/utils/include/iot_pki_utils.h"
"${ProjDirPath}/../../../../../rtos/freertos/libraries/freertos_plus/standard/utils/include/iot_system_init.h"
"${ProjDirPath}/../../../../../rtos/freertos/libraries/freertos_plus/standard/utils/src/iot_pki_utils.c"
"${ProjDirPath}/../../../../../rtos/freertos/libraries/freertos_plus/standard/utils/src/iot_system_init.c"
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
"${ProjDirPath}/../../../../../devices/MIMXRT106A/utilities/debug_console/fsl_debug_console.c"
"${ProjDirPath}/../../../../../devices/MIMXRT106A/utilities/debug_console/fsl_debug_console.h"
"${ProjDirPath}/../../../../../devices/MIMXRT106A/utilities/debug_console/fsl_debug_console_conf.h"
"${ProjDirPath}/../../../../../devices/MIMXRT106A/utilities/str/fsl_str.c"
"${ProjDirPath}/../../../../../devices/MIMXRT106A/utilities/str/fsl_str.h"
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
"${ProjDirPath}/../../../../../devices/MIMXRT106A/drivers/fsl_lpuart_freertos.c"
"${ProjDirPath}/../../../../../devices/MIMXRT106A/drivers/fsl_lpuart_freertos.h"
"${ProjDirPath}/../../../../../middleware/sdmmc/sdio/fsl_sdio.c"
"${ProjDirPath}/../../../../../middleware/sdmmc/sdio/fsl_sdio.h"
"${ProjDirPath}/../../../../../middleware/sdmmc/common/fsl_sdmmc_common.c"
"${ProjDirPath}/../../../../../middleware/sdmmc/common/fsl_sdmmc_common.h"
"${ProjDirPath}/../../../../../middleware/sdmmc/common/fsl_sdmmc_spec.h"
"${ProjDirPath}/../../../../../middleware/sdmmc/host/usdhc/fsl_sdmmc_host.h"
"${ProjDirPath}/../../../../../middleware/sdmmc/host/usdhc/fsl_sdmmc_host.h"
"${ProjDirPath}/../../../../../middleware/sdmmc/host/usdhc/non_blocking/fsl_sdmmc_host.c"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_BLE/bt_app_inc/wiced_bt_ble.h"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_BLE/bt_app_inc/wiced_bt_cfg.h"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_BLE/bt_app_inc/wiced_bt_dev.h"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_BLE/bt_app_inc/wiced_bt_gatt.h"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_BLE/bt_app_inc/wiced_bt_stack.h"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_BLE/bt_app_inc/wiced_bt_types.h"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_BLE/wiced_bt/BTE/Components/stack/include/bt_types.h"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_BLE/wiced_bt/BTE/Components/stack/include/dyn_mem.h"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_BLE/wiced_bt/BTE/Components/stack/include/gattdefs.h"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_BLE/wiced_bt/BTE/Components/stack/include/hcidefs.h"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_BLE/wiced_bt/BTE/Components/stack/include/hcimsgs.h"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_BLE/wiced_bt/BTE/Components/stack/include/hiddefs.h"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_BLE/wiced_bt/BTE/Components/stack/include/l2cdefs.h"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_BLE/wiced_bt/BTE/Components/stack/include/sdpdefs.h"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_BLE/wiced_bt/BTE/Projects/bte/main/bt_target.h"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_BLE/wiced_bt/BTE/Projects/bte/main/bt_trace.h"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_BLE/wiced_bt/BTE/Projects/bte/main/gki_target.h"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_BLE/wiced_bt/BTE/WICED/data_types.h"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_BLE/wiced_bt/BTE/WICED/wiced_bt_int.h"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_BLE/wiced_bt/imxrt_port/bt_firmware_controller.c"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_BLE/wiced_bt/imxrt_port/buildcfg.h"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_BLE/wiced_bt/imxrt_port/platform_bluetooth.c"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_BLE/wiced_bt/imxrt_port/platform_bluetooth.h"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_BLE/wiced_bt/imxrt_port/platform_imxrt.c"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_BLE/wiced_bt/imxrt_port/platform_imxrt.h"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_BLE/wiced_bt/imxrt_port/rtos.h"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_BLE/wiced_bt/imxrt_port/wiced_bluetooth_result.h"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_BLE/wiced_bt/imxrt_port/wiced_bt_sco.h"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_BLE/wiced_bt/imxrt_port/wiced_bt_uuid.h"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_BLE/wiced_bt/imxrt_port/wiced_result.h"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_BLE/wiced_bt/imxrt_port/wiced_rtos.c"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_BLE/wiced_bt/imxrt_port/wiced_rtos.h"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_BLE/wiced_bt/imxrt_port/wiced_time.h"
"${ProjDirPath}/../../../../../middleware/wiced/43xxx_BLE/wiced_bt/imxrt_port/wiced_utilities.h"
"${ProjDirPath}/../../../../../components/codec/i2c/fsl_codec_i2c.c"
"${ProjDirPath}/../../../../../components/codec/i2c/fsl_codec_i2c.h"
"${ProjDirPath}/../../../../../components/i2c/i2c.h"
"${ProjDirPath}/../../../../../components/i2c/lpi2c_adapter.c"
"${ProjDirPath}/../../../../../middleware/cjson/cJSON.c"
"${ProjDirPath}/../../../../../middleware/cjson/cJSON.h"
"${ProjDirPath}/../../../../../middleware/cjson/cJSON_Utils.c"
"${ProjDirPath}/../../../../../middleware/cjson/cJSON_Utils.h"
"${ProjDirPath}/../../../../../middleware/amzn_ww/cortexM7/pryon_lite.h"
"${ProjDirPath}/../../../../../middleware/amzn_ww/cortexM7/pryon_lite_PRL1000.h"
"${ProjDirPath}/../../../../../middleware/amzn_ww/cortexM7/pryon_lite_error.h"
"${ProjDirPath}/../../../../../middleware/amzn_ww/cortexM7/pryon_lite_metadata.h"
"${ProjDirPath}/../../../../../middleware/amzn_ww/cortexM7/pryon_lite_vad.h"
"${ProjDirPath}/../../../../../middleware/amzn_ww/cortexM7/pryon_lite_ww.h"
"${ProjDirPath}/../../../../../middleware/amzn_ww/source/amazon_wake_word.c"
"${ProjDirPath}/../../../../../middleware/amzn_ww/source/amazon_wake_word.h"
"${ProjDirPath}/../../../../../middleware/amzn_ww/source/models/D.en-US.alexa.c"
"${ProjDirPath}/../../../../../middleware/amzn_ww/source/models/D.es-ES.alexa.c"
"${ProjDirPath}/../../../../../middleware/amzn_ww/source/models/U_1S_50k.de-DE.alexa.c"
"${ProjDirPath}/../../../../../middleware/amzn_ww/source/models/U_1S_50k.en-AU.alexa.c"
"${ProjDirPath}/../../../../../middleware/amzn_ww/source/models/U_1S_50k.en-CA.alexa.c"
"${ProjDirPath}/../../../../../middleware/amzn_ww/source/models/U_1S_50k.en-GB.alexa.c"
"${ProjDirPath}/../../../../../middleware/amzn_ww/source/models/U_1S_50k.en-IN.alexa.c"
"${ProjDirPath}/../../../../../middleware/amzn_ww/source/models/U_1S_50k.en-US.alexa.c"
"${ProjDirPath}/../../../../../middleware/amzn_ww/source/models/U_1S_50k.es-ES.alexa.c"
"${ProjDirPath}/../../../../../middleware/amzn_ww/source/models/U_1S_50k.es-MX.alexa.c"
"${ProjDirPath}/../../../../../middleware/amzn_ww/source/models/U_1S_50k.fr-CA.alexa.c"
"${ProjDirPath}/../../../../../middleware/amzn_ww/source/models/U_1S_50k.fr-FR.alexa.c"
"${ProjDirPath}/../../../../../middleware/amzn_ww/source/models/U_1S_50k.it-IT.alexa.c"
"${ProjDirPath}/../../../../../middleware/amzn_ww/source/models/U_1S_50k.ja-JP.alexa.c"
"${ProjDirPath}/../../../../../middleware/amzn_ww/source/models/U_250k.es-ES.alexa.c"
"${ProjDirPath}/../../../../../middleware/amzn_ww/source/models/WR_250k.en-AU.alexa.c"
"${ProjDirPath}/../../../../../middleware/amzn_ww/source/models/WR_250k.en-US.alexa.c"
"${ProjDirPath}/../../../../../middleware/amzn_ww/source/models/WR_250k.fr-CA.alexa.c"
"${ProjDirPath}/../../../../../middleware/amzn_ww/source/models/WR_250k.fr-FR.alexa.c"
"${ProjDirPath}/../../../../../middleware/amzn_ww/source/models/WS_250k.de-DE.alexa.c"
"${ProjDirPath}/../../../../../middleware/amzn_ww/source/models/WS_250k.en-GB.alexa.c"
"${ProjDirPath}/../../../../../middleware/amzn_ww/source/models/WS_250k.en-IN.alexa.c"
"${ProjDirPath}/../../../../../middleware/amzn_ww/source/models/WS_250k.it-IT.alexa.c"
"${ProjDirPath}/../../../../../middleware/amzn_ww/source/models/WS_250k.ja-JP.alexa.c"
"${ProjDirPath}/../../../../../components/codec/tfa9xxx/README.md"
"${ProjDirPath}/../../../../../components/codec/tfa9xxx/fsl_tfa9xxx.c"
"${ProjDirPath}/../../../../../components/codec/tfa9xxx/fsl_tfa9xxx.h"
"${ProjDirPath}/../../../../../components/codec/tfa9xxx/fsl_tfa9xxx_IMX.c"
"${ProjDirPath}/../../../../../components/codec/tfa9xxx/tfa_config_TFA9892N1.h"
"${ProjDirPath}/../../../../../components/codec/tfa9xxx/tfa_config_TFA9894N1.h"
"${ProjDirPath}/../../../../../components/codec/tfa9xxx/tfa_config_TFA9894N2.h"
"${ProjDirPath}/../../../../../components/codec/tfa9xxx/vas_tfa_drv/config.h"
"${ProjDirPath}/../../../../../components/codec/tfa9xxx/vas_tfa_drv/tfa2_container.c"
"${ProjDirPath}/../../../../../components/codec/tfa9xxx/vas_tfa_drv/tfa2_container.h"
"${ProjDirPath}/../../../../../components/codec/tfa9xxx/vas_tfa_drv/tfa2_dev.c"
"${ProjDirPath}/../../../../../components/codec/tfa9xxx/vas_tfa_drv/tfa2_dev.h"
"${ProjDirPath}/../../../../../components/codec/tfa9xxx/vas_tfa_drv/tfa2_dsp_fw.h"
"${ProjDirPath}/../../../../../components/codec/tfa9xxx/vas_tfa_drv/tfa2_haptic.c"
"${ProjDirPath}/../../../../../components/codec/tfa9xxx/vas_tfa_drv/tfa2_haptic.h"
"${ProjDirPath}/../../../../../components/codec/tfa9xxx/vas_tfa_drv/tfa2_init.c"
"${ProjDirPath}/../../../../../components/codec/tfa9xxx/vas_tfa_drv/tfa9xxx_parameters.h"
"${ProjDirPath}/../../../../../components/codec/tfa9xxx/vas_tfa_drv/tfa_container_crc32.c"
"${ProjDirPath}/../../../../../components/codec/tfa9xxx/vas_tfa_drv/tfa_haptic_fw_defs.h"
"${ProjDirPath}/../../../../../components/codec/fsl_codec_common.c"
"${ProjDirPath}/../../../../../components/codec/fsl_codec_common.h"
"${ProjDirPath}/../../../../../components/codec/port/tfa9xxx/fsl_codec_adapter.c"
"${ProjDirPath}/../../../../../components/codec/port/tfa9xxx/fsl_codec_adapter.h"
"${ProjDirPath}/../../../../../middleware/sln_iot_common_platform/audio/sln_amplifier.c"
"${ProjDirPath}/../../../../../middleware/sln_iot_common_platform/audio/sln_amplifier.h"
"${ProjDirPath}/../../../../../middleware/sln_iot_common_platform/drivers/sln_pdm_mic.c"
"${ProjDirPath}/../../../../../middleware/sln_iot_common_platform/drivers/sln_pdm_mic.h"
"${ProjDirPath}/../../../../../middleware/sln_iot_common_platform/audio/pdm_to_pcm_task.c"
"${ProjDirPath}/../../../../../middleware/sln_iot_common_platform/audio/pdm_to_pcm_task.h"
"${ProjDirPath}/../../../../../middleware/aws_ais/ais_streamer.c"
"${ProjDirPath}/../../../../../middleware/aws_ais/ais_streamer.h"
"${ProjDirPath}/../../../../../middleware/aws_ais/aisv2.c"
"${ProjDirPath}/../../../../../middleware/aws_ais/aisv2.h"
"${ProjDirPath}/../../../../../middleware/aws_ais/aisv2_event.c"
"${ProjDirPath}/../../../../../middleware/aws_ais/aisv2_json.c"
"${ProjDirPath}/../../../../../middleware/aws_ais/aisv2_msg.c"
"${ProjDirPath}/../../../../../middleware/aws_ais/aisv2_registration.c"
"${ProjDirPath}/../../../../../middleware/audio_streamer/inc/container.h"
"${ProjDirPath}/../../../../../middleware/audio_streamer/inc/error.h"
"${ProjDirPath}/../../../../../middleware/audio_streamer/inc/file_utils.h"
"${ProjDirPath}/../../../../../middleware/audio_streamer/inc/general_utils.h"
"${ProjDirPath}/../../../../../middleware/audio_streamer/inc/lib_common.h"
"${ProjDirPath}/../../../../../middleware/audio_streamer/inc/logging.h"
"${ProjDirPath}/../../../../../middleware/audio_streamer/inc/osa_common.h"
"${ProjDirPath}/../../../../../middleware/audio_streamer/inc/osa_cond.h"
"${ProjDirPath}/../../../../../middleware/audio_streamer/inc/osa_dir.h"
"${ProjDirPath}/../../../../../middleware/audio_streamer/inc/osa_event.h"
"${ProjDirPath}/../../../../../middleware/audio_streamer/inc/osa_freertos.h"
"${ProjDirPath}/../../../../../middleware/audio_streamer/inc/osa_init.h"
"${ProjDirPath}/../../../../../middleware/audio_streamer/inc/osa_isr.h"
"${ProjDirPath}/../../../../../middleware/audio_streamer/inc/osa_memory.h"
"${ProjDirPath}/../../../../../middleware/audio_streamer/inc/osa_message.h"
"${ProjDirPath}/../../../../../middleware/audio_streamer/inc/osa_misc.h"
"${ProjDirPath}/../../../../../middleware/audio_streamer/inc/osa_mutex.h"
"${ProjDirPath}/../../../../../middleware/audio_streamer/inc/osa_sem.h"
"${ProjDirPath}/../../../../../middleware/audio_streamer/inc/osa_thread.h"
"${ProjDirPath}/../../../../../middleware/audio_streamer/inc/osa_time.h"
"${ProjDirPath}/../../../../../middleware/audio_streamer/inc/osa_types.h"
"${ProjDirPath}/../../../../../middleware/audio_streamer/inc/ringbuffer.h"
"${ProjDirPath}/../../../../../middleware/audio_streamer/inc/streamer_api.h"
"${ProjDirPath}/../../../../../middleware/audio_streamer/inc/streamer_element_properties.h"
"${ProjDirPath}/../../../../../middleware/audio_streamer/inc/streamer_error.h"
"${ProjDirPath}/../../../../../middleware/audio_streamer/inc/streamer_message.h"
"${ProjDirPath}/../../../../../middleware/audio_streamer/inc/streamer_pcm.h"
"${ProjDirPath}/../../../../../middleware/audio_streamer/inc/vector.h"
"${ProjDirPath}/../../../../../middleware/audio_streamer/streamer/inc/af_error.h"
"${ProjDirPath}/../../../../../middleware/audio_streamer/streamer/inc/memtypes.h"
"${ProjDirPath}/../../../../../middleware/audio_streamer/streamer/inc/metadata_error.h"
"${ProjDirPath}/../../../../../middleware/sln_iot_common_platform/utils/device_utils.c"
"${ProjDirPath}/../../../../../middleware/sln_iot_common_platform/utils/device_utils.h"
"${ProjDirPath}/../../../../../middleware/sln_iot_common_platform/network/mqtt_connection.c"
"${ProjDirPath}/../../../../../middleware/sln_iot_common_platform/network/mqtt_connection.h"
"${ProjDirPath}/../../../../../middleware/mcu_voice_libs_public/afe/sln_afe.h"
"${ProjDirPath}/../../../../../middleware/mcu_voice_libs_public/dsp/sln_dsp_toolbox.h"
"${ProjDirPath}/../../../../../middleware/sln_iot_common_platform/drivers/sln_RT10xx_RGB_LED_driver.c"
"${ProjDirPath}/../../../../../middleware/sln_iot_common_platform/drivers/sln_RT10xx_RGB_LED_driver.h"
"${ProjDirPath}/../../../../../middleware/sln_iot_common_platform/security/sln_encrypt.c"
"${ProjDirPath}/../../../../../middleware/sln_iot_common_platform/security/sln_encrypt.h"
"${ProjDirPath}/../../../../../rtos/freertos/demos/include/aws_wifi_connect_task.h"
"${ProjDirPath}/../../../../../rtos/freertos/demos/include/iot_ble_numericComparison.h"
"${ProjDirPath}/../../../../../rtos/freertos/demos/network_manager/aws_iot_network_manager.c"
"${ProjDirPath}/../../../../../rtos/freertos/demos/network_manager/iot_network_manager_private.h"
"${ProjDirPath}/../../../../../rtos/freertos/libraries/abstractions/wifi/include/iot_wifi.h"
"${ProjDirPath}/../../../../../middleware/alexa_ux_audio/README.md"
"${ProjDirPath}/../../../../../middleware/alexa_ux_audio/audio_samples.h"
"${ProjDirPath}/../../../../../middleware/alexa_ux_audio/wake_word_prompt.c"
"${ProjDirPath}/../../../../../middleware/alexa_ux_audio/wav_alerts_melodic_01.c"
"${ProjDirPath}/../../../../../middleware/alexa_ux_audio/wav_alerts_melodic_01_short.c"
"${ProjDirPath}/../../../../../middleware/alexa_ux_audio/wav_alerts_melodic_02.c"
"${ProjDirPath}/../../../../../middleware/alexa_ux_audio/wav_alerts_melodic_02_short.c"
"${ProjDirPath}/../../../../../middleware/alexa_ux_audio/wav_alerts_notification_01.c"
"${ProjDirPath}/../../../../../middleware/alexa_ux_audio/wav_alerts_notification_03.c"
"${ProjDirPath}/../../../../../middleware/alexa_ux_audio/wav_state_privacy_off.c"
"${ProjDirPath}/../../../../../middleware/alexa_ux_audio/wav_state_privacy_on.c"
"${ProjDirPath}/../../../../../middleware/alexa_ux_audio/wav_ui_endpointing.c"
"${ProjDirPath}/../../../../../middleware/alexa_ux_audio/wav_ui_wakesound.c"
"${ProjDirPath}/../../../../../devices/MIMXRT106A/drivers/fsl_clock.c"
"${ProjDirPath}/../../../../../devices/MIMXRT106A/drivers/fsl_clock.h"
"${ProjDirPath}/../../../../../devices/MIMXRT106A/MIMXRT106A.h"
"${ProjDirPath}/../../../../../devices/MIMXRT106A/MIMXRT106A_features.h"
"${ProjDirPath}/../../../../../devices/MIMXRT106A/fsl_device_registers.h"
"${ProjDirPath}/../../../../../components/uart/lpuart_adapter.c"
"${ProjDirPath}/../../../../../components/uart/uart.h"
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
"${ProjDirPath}/../../../../../middleware/usb/output/source/device/class/usb_device_cdc_acm.c"
"${ProjDirPath}/../../../../../middleware/usb/output/source/device/class/usb_device_cdc_acm.h"
"${ProjDirPath}/../../../../../middleware/usb/output/source/device/class/usb_device_class.c"
"${ProjDirPath}/../../../../../middleware/usb/output/source/device/class/usb_device_class.h"
"${ProjDirPath}/../../../../../middleware/usb/output/source/device/usb_device_ch9.c"
"${ProjDirPath}/../../../../../middleware/usb/output/source/device/usb_device_ch9.h"
"${ProjDirPath}/../../../../../middleware/usb/include/usb.h"
"${ProjDirPath}/../../../../../middleware/usb/include/usb_misc.h"
"${ProjDirPath}/../../../../../middleware/usb/include/usb_spec.h"
"${ProjDirPath}/../../../../../middleware/usb/device/usb_device.h"
"${ProjDirPath}/../../../../../middleware/usb/device/usb_device_dci.h"
"${ProjDirPath}/../../../../../devices/MIMXRT106A/drivers/fsl_xbara.c"
"${ProjDirPath}/../../../../../devices/MIMXRT106A/drivers/fsl_xbara.h"
"${ProjDirPath}/../../../../../devices/MIMXRT106A/drivers/fsl_pwm.c"
"${ProjDirPath}/../../../../../devices/MIMXRT106A/drivers/fsl_pwm.h"
"${ProjDirPath}/../../../../../rtos/freertos/freertos_kernel/include/freertos_tasks_c_additions.h"
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
"${ProjDirPath}/../../../../../middleware/sln_iot_common_platform/flash_mgmt/sln_flash_mgmt.c"
"${ProjDirPath}/../../../../../middleware/sln_iot_common_platform/flash_mgmt/sln_flash_mgmt.h"
"${ProjDirPath}/../../../../../middleware/sln_iot_common_platform/flexspi/flexspi_hyper_flash_ops.c"
"${ProjDirPath}/../../../../../middleware/sln_iot_common_platform/flexspi/flexspi_hyper_flash_ops.h"
"${ProjDirPath}/../../../../../middleware/sln_iot_common_platform/flash_mgmt/sln_flash.c"
"${ProjDirPath}/../../../../../middleware/sln_iot_common_platform/flash_mgmt/sln_flash.h"
"${ProjDirPath}/../../../../../middleware/sdmmc/osa/fsl_sdmmc_osa.c"
"${ProjDirPath}/../../../../../middleware/sdmmc/osa/fsl_sdmmc_osa.h"
"${ProjDirPath}/../../../../../components/osa/fsl_os_abstraction.h"
"${ProjDirPath}/../../../../../components/osa/fsl_os_abstraction_config.h"
"${ProjDirPath}/../../../../../components/osa/fsl_os_abstraction_free_rtos.c"
"${ProjDirPath}/../../../../../components/osa/fsl_os_abstraction_free_rtos.h"
"${ProjDirPath}/../../../../../devices/MIMXRT106A/drivers/fsl_usdhc.c"
"${ProjDirPath}/../../../../../devices/MIMXRT106A/drivers/fsl_usdhc.h"
"${ProjDirPath}/../../../../../devices/MIMXRT106A/drivers/fsl_lpi2c.c"
"${ProjDirPath}/../../../../../devices/MIMXRT106A/drivers/fsl_lpi2c.h"
"${ProjDirPath}/../../../../../devices/MIMXRT106A/drivers/fsl_dmamux.c"
"${ProjDirPath}/../../../../../devices/MIMXRT106A/drivers/fsl_dmamux.h"
"${ProjDirPath}/../../../../../devices/MIMXRT106A/drivers/fsl_edma.c"
"${ProjDirPath}/../../../../../devices/MIMXRT106A/drivers/fsl_edma.h"
"${ProjDirPath}/../../../../../devices/MIMXRT106A/drivers/fsl_sai.c"
"${ProjDirPath}/../../../../../devices/MIMXRT106A/drivers/fsl_sai.h"
"${ProjDirPath}/../../../../../devices/MIMXRT106A/drivers/fsl_sai_edma.c"
"${ProjDirPath}/../../../../../devices/MIMXRT106A/drivers/fsl_sai_edma.h"
"${ProjDirPath}/../../../../../devices/MIMXRT106A/utilities/fsl_sbrk.c"
)


set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -T${ProjDirPath}/sln_alexa_iot_xip.ld -static")

set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -T${ProjDirPath}/sln_alexa_iot_xip.ld -static")

TARGET_LINK_LIBRARIES(ais_demo_43438.elf -Wl,--start-group)
target_link_libraries(ais_demo_43438.elf debug m)

target_link_libraries(ais_demo_43438.elf debug c)

target_link_libraries(ais_demo_43438.elf debug gcc)

target_link_libraries(ais_demo_43438.elf debug nosys)

target_link_libraries(ais_demo_43438.elf optimized m)

target_link_libraries(ais_demo_43438.elf optimized c)

target_link_libraries(ais_demo_43438.elf optimized gcc)

target_link_libraries(ais_demo_43438.elf optimized nosys)

link_directories(${ProjDirPath}/../../../../../middleware/wiced/43xxx_BLE/wiced_bt/BTE)

target_link_libraries(ais_demo_43438.elf debug ${ProjDirPath}/../../../../../middleware/wiced/43xxx_BLE/wiced_bt/BTE/libwicedble.a)

link_directories(${ProjDirPath}/../../../../../middleware/amzn_ww/cortexM7/U)

target_link_libraries(ais_demo_43438.elf debug ${ProjDirPath}/../../../../../middleware/amzn_ww/cortexM7/U/libpryon_lite-U.a)

link_directories(${ProjDirPath}/../../../../../middleware/audio_streamer/libs/cm7f/gcc)

target_link_libraries(ais_demo_43438.elf debug ${ProjDirPath}/../../../../../middleware/audio_streamer/libs/cm7f/gcc/libccicontainer.a)

link_directories(${ProjDirPath}/../../../../../middleware/audio_streamer/libs/cm7f/gcc)

target_link_libraries(ais_demo_43438.elf debug ${ProjDirPath}/../../../../../middleware/audio_streamer/libs/cm7f/gcc/libccimeta.a)

link_directories(${ProjDirPath}/../../../../../middleware/audio_streamer/libs/cm7f/gcc)

target_link_libraries(ais_demo_43438.elf debug ${ProjDirPath}/../../../../../middleware/audio_streamer/libs/cm7f/gcc/libcommon.a)

link_directories(${ProjDirPath}/../../../../../middleware/audio_streamer/libs/cm7f/gcc)

target_link_libraries(ais_demo_43438.elf debug ${ProjDirPath}/../../../../../middleware/audio_streamer/libs/cm7f/gcc/libmp3dec.a)

link_directories(${ProjDirPath}/../../../../../middleware/audio_streamer/libs/cm7f/gcc)

target_link_libraries(ais_demo_43438.elf debug ${ProjDirPath}/../../../../../middleware/audio_streamer/libs/cm7f/gcc/libopusdec.a)

link_directories(${ProjDirPath}/../../../../../middleware/audio_streamer/libs/cm7f/gcc)

target_link_libraries(ais_demo_43438.elf debug ${ProjDirPath}/../../../../../middleware/audio_streamer/libs/cm7f/gcc/libopusenc.a)

link_directories(${ProjDirPath}/../../../../../middleware/audio_streamer/libs/cm7f/gcc)

target_link_libraries(ais_demo_43438.elf debug ${ProjDirPath}/../../../../../middleware/audio_streamer/libs/cm7f/gcc/libosa.a)

link_directories(${ProjDirPath}/../../../../../middleware/audio_streamer/libs/cm7f/gcc)

target_link_libraries(ais_demo_43438.elf debug ${ProjDirPath}/../../../../../middleware/audio_streamer/libs/cm7f/gcc/libsrc48k.a)

link_directories(${ProjDirPath}/../../../../../middleware/audio_streamer/libs/cm7f/gcc)

target_link_libraries(ais_demo_43438.elf debug ${ProjDirPath}/../../../../../middleware/audio_streamer/libs/cm7f/gcc/libstreamer.a)

link_directories(${ProjDirPath}/../../../../../middleware/audio_streamer/libs/cm7f/gcc)

target_link_libraries(ais_demo_43438.elf debug ${ProjDirPath}/../../../../../middleware/audio_streamer/libs/cm7f/gcc/libwavdec.a)

link_directories(${ProjDirPath}/../../../../../middleware/mcu_voice_libs_public/afe)

target_link_libraries(ais_demo_43438.elf debug ${ProjDirPath}/../../../../../middleware/mcu_voice_libs_public/afe/libsln_afe.a)

link_directories(${ProjDirPath}/../../../../../middleware/mcu_voice_libs_public/dsp)

target_link_libraries(ais_demo_43438.elf debug ${ProjDirPath}/../../../../../middleware/mcu_voice_libs_public/dsp/libsln_dsp_toolbox.a)

link_directories(${ProjDirPath}/../../../../../middleware/wiced/43xxx_BLE/wiced_bt/BTE)

target_link_libraries(ais_demo_43438.elf optimized ${ProjDirPath}/../../../../../middleware/wiced/43xxx_BLE/wiced_bt/BTE/libwicedble.a)

link_directories(${ProjDirPath}/../../../../../middleware/amzn_ww/cortexM7/U)

target_link_libraries(ais_demo_43438.elf optimized ${ProjDirPath}/../../../../../middleware/amzn_ww/cortexM7/U/libpryon_lite-U.a)

link_directories(${ProjDirPath}/../../../../../middleware/audio_streamer/libs/cm7f/gcc)

target_link_libraries(ais_demo_43438.elf optimized ${ProjDirPath}/../../../../../middleware/audio_streamer/libs/cm7f/gcc/libccicontainer.a)

link_directories(${ProjDirPath}/../../../../../middleware/audio_streamer/libs/cm7f/gcc)

target_link_libraries(ais_demo_43438.elf optimized ${ProjDirPath}/../../../../../middleware/audio_streamer/libs/cm7f/gcc/libccimeta.a)

link_directories(${ProjDirPath}/../../../../../middleware/audio_streamer/libs/cm7f/gcc)

target_link_libraries(ais_demo_43438.elf optimized ${ProjDirPath}/../../../../../middleware/audio_streamer/libs/cm7f/gcc/libcommon.a)

link_directories(${ProjDirPath}/../../../../../middleware/audio_streamer/libs/cm7f/gcc)

target_link_libraries(ais_demo_43438.elf optimized ${ProjDirPath}/../../../../../middleware/audio_streamer/libs/cm7f/gcc/libmp3dec.a)

link_directories(${ProjDirPath}/../../../../../middleware/audio_streamer/libs/cm7f/gcc)

target_link_libraries(ais_demo_43438.elf optimized ${ProjDirPath}/../../../../../middleware/audio_streamer/libs/cm7f/gcc/libopusdec.a)

link_directories(${ProjDirPath}/../../../../../middleware/audio_streamer/libs/cm7f/gcc)

target_link_libraries(ais_demo_43438.elf optimized ${ProjDirPath}/../../../../../middleware/audio_streamer/libs/cm7f/gcc/libopusenc.a)

link_directories(${ProjDirPath}/../../../../../middleware/audio_streamer/libs/cm7f/gcc)

target_link_libraries(ais_demo_43438.elf optimized ${ProjDirPath}/../../../../../middleware/audio_streamer/libs/cm7f/gcc/libosa.a)

link_directories(${ProjDirPath}/../../../../../middleware/audio_streamer/libs/cm7f/gcc)

target_link_libraries(ais_demo_43438.elf optimized ${ProjDirPath}/../../../../../middleware/audio_streamer/libs/cm7f/gcc/libsrc48k.a)

link_directories(${ProjDirPath}/../../../../../middleware/audio_streamer/libs/cm7f/gcc)

target_link_libraries(ais_demo_43438.elf optimized ${ProjDirPath}/../../../../../middleware/audio_streamer/libs/cm7f/gcc/libstreamer.a)

link_directories(${ProjDirPath}/../../../../../middleware/audio_streamer/libs/cm7f/gcc)

target_link_libraries(ais_demo_43438.elf optimized ${ProjDirPath}/../../../../../middleware/audio_streamer/libs/cm7f/gcc/libwavdec.a)

link_directories(${ProjDirPath}/../../../../../middleware/mcu_voice_libs_public/afe)

target_link_libraries(ais_demo_43438.elf optimized ${ProjDirPath}/../../../../../middleware/mcu_voice_libs_public/afe/libsln_afe.a)

link_directories(${ProjDirPath}/../../../../../middleware/mcu_voice_libs_public/dsp)

target_link_libraries(ais_demo_43438.elf optimized ${ProjDirPath}/../../../../../middleware/mcu_voice_libs_public/dsp/libsln_dsp_toolbox.a)

TARGET_LINK_LIBRARIES(ais_demo_43438.elf -Wl,--end-group)


