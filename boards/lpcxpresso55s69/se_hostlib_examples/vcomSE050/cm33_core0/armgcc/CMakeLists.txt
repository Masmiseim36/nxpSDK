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

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DCPU_LPC55S69JBD100_cm33_core0")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DLPC_55x=1")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DSDK_DEBUGCONSOLE_UART")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DUSB_STACK_BM")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DUSB_ISR_REDIRECT")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DPRINTF_ADVANCED_ENABLE=1")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DSERIAL_PORT_TYPE_USBCDC=1")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG_CONSOLE_TRANSFER_NON_BLOCKING")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DUSB_DEVICE_CONFIG_CDC_ACM=1")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DMBEDTLS_CONFIG_FILE='\"sss_mbedtls_frdmk64f_config.h\"'")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DMBEDTLS")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DSCP_MODE=C_MAC_C_ENC_R_MAC_R_ENC")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DSERIAL_PORT_TYPE_UART=1")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DT1oI2C")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DT1oI2C_UM11225")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DSSS_USE_FTR_FILE")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DNO_SECURE_CHANNEL_SUPPORT")

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

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DCPU_LPC55S69JBD100_cm33_core0")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DLPC_55x=1")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DSDK_DEBUGCONSOLE_UART")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DUSB_STACK_BM")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DUSB_ISR_REDIRECT")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DPRINTF_ADVANCED_ENABLE=1")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DSERIAL_PORT_TYPE_USBCDC=1")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DDEBUG_CONSOLE_TRANSFER_NON_BLOCKING")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DUSB_DEVICE_CONFIG_CDC_ACM=1")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DMBEDTLS_CONFIG_FILE='\"sss_mbedtls_frdmk64f_config.h\"'")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DMBEDTLS")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DSCP_MODE=C_MAC_C_ENC_R_MAC_R_ENC")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DSERIAL_PORT_TYPE_UART=1")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DT1oI2C")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DT1oI2C_UM11225")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DSSS_USE_FTR_FILE")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DNO_SECURE_CHANNEL_SUPPORT")

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

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DSERIAL_PORT_TYPE_USBCDC=1")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG_CONSOLE_TRANSFER_NON_BLOCKING")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DUSB_DEVICE_CONFIG_CDC_ACM=1")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DSERIAL_PORT_TYPE_UART=1")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DT1oI2C")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DT1oI2C_UM11225")

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

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DSERIAL_PORT_TYPE_USBCDC=1")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DDEBUG_CONSOLE_TRANSFER_NON_BLOCKING")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DUSB_DEVICE_CONFIG_CDC_ACM=1")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DSERIAL_PORT_TYPE_UART=1")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DT1oI2C")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DT1oI2C_UM11225")

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

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} --defsym=__stack_size__=0x2000")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} --defsym=__heap_size__=0x8000")

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

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} --defsym=__stack_size__=0x2000")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} --defsym=__heap_size__=0x8000")

include_directories(${ProjDirPath}/../../../se_hostlib_vcomSE050/cm33_core0)

include_directories(${ProjDirPath}/..)

include_directories(${ProjDirPath}/../..)

include_directories(${ProjDirPath}/../../../../../../middleware/se_hostlib/hostlib/hostLib/vcom/ksdk)

include_directories(${ProjDirPath}/../../../../../../middleware/se_hostlib/demos/ksdk/se050/default)

include_directories(${ProjDirPath}/../../../../../../middleware/se_hostlib/hostlib/hostLib/libCommon/smCom)

include_directories(${ProjDirPath}/../../../../../../middleware/se_hostlib/sss/inc)

include_directories(${ProjDirPath}/../../../../../../middleware/se_hostlib/sss/port/ksdk)

include_directories(${ProjDirPath}/../../../../../../middleware/mbedtls/include/mbedtls)

include_directories(${ProjDirPath}/../../../../../../middleware/se_hostlib/hostlib/hostLib/se05x_03_xx_xx)

include_directories(${ProjDirPath}/../../../../../../middleware/se_hostlib/sss/ex/inc)

include_directories(${ProjDirPath}/../../../mainA71CH)

include_directories(${ProjDirPath}/../../../../../../middleware/se_hostlib/sss/plugin/mbedtls)

include_directories(${ProjDirPath}/../../../../../../middleware/se_hostlib/hostlib/hostLib/tstUtil)

include_directories(${ProjDirPath}/../../../../../../middleware/se_hostlib/hostlib/hostLib/mbedtls/inc)

include_directories(${ProjDirPath}/../../../../../../middleware/se_hostlib/demos/ksdk/common)

include_directories(${ProjDirPath}/../../../../../../middleware/se_hostlib/hostlib/hostLib/platform/inc)

include_directories(${ProjDirPath}/../../../../../../middleware/se_hostlib/demos/ksdk/common/boards/lpcxpresso55s69/se_hostlib_examples/cloud_demo)

include_directories(${ProjDirPath}/../../../../../../middleware/usb/include)

include_directories(${ProjDirPath}/../../../..)

include_directories(${ProjDirPath}/../../../../../../CMSIS/Include)

include_directories(${ProjDirPath}/../../../../../../devices)

include_directories(${ProjDirPath}/../../../../../../devices/LPC55S69)

include_directories(${ProjDirPath}/../../../../../../middleware/mbedtls/include)

include_directories(${ProjDirPath}/../../../../../../middleware/mbedtls/port/ksdk)

include_directories(${ProjDirPath}/../../../../../../devices/LPC55S69/drivers)

include_directories(${ProjDirPath}/../../../../../../components/osa)

include_directories(${ProjDirPath}/../../../../../../middleware/usb/device)

include_directories(${ProjDirPath}/../../../../../../components/serial_manager/usb_cdc_adapter)

include_directories(${ProjDirPath}/../../../../../../devices/LPC55S69/utilities/str)

include_directories(${ProjDirPath}/../../../../../../devices/LPC55S69/utilities/debug_console)

include_directories(${ProjDirPath}/../../../../../../components/uart)

include_directories(${ProjDirPath}/../../../../../../components/serial_manager)

include_directories(${ProjDirPath}/../../../../../../components/lists)

include_directories(${ProjDirPath}/../../../../../../middleware/usb/output/source/device/class)

include_directories(${ProjDirPath}/../../../../../../middleware/usb/output/source/device)

include_directories(${ProjDirPath}/../../../../../../middleware/se_hostlib)

include_directories(${ProjDirPath}/../../../../../../middleware/se_hostlib/hostlib/hostLib/inc)

include_directories(${ProjDirPath}/../../../../../../middleware/se_hostlib/platform)

include_directories(${ProjDirPath}/../../../../../../middleware/se_hostlib/hostlib/hostLib/libCommon/smCom/T1oI2C)

include_directories(${ProjDirPath}/../../../../../../middleware/se_hostlib/hostlib/hostLib/libCommon/infra)

include_directories(${ProjDirPath}/../../../../../../devices/LPC55S69/utilities)

add_executable(se_hostlib_vcomSE050.elf 
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../fsl_sss_ftr.h"
"${ProjDirPath}/../smComT1oI2C.h"
"${ProjDirPath}/../smComSCI2C.h"
"${ProjDirPath}/../vcom2i2c.c"
"${ProjDirPath}/../vcom2i2c.h"
"${ProjDirPath}/../virtual_com.c"
"${ProjDirPath}/../virtual_com.h"
"${ProjDirPath}/../se_pit_config.c"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
"${ProjDirPath}/../../../../../../devices/LPC55S69/drivers/fsl_casper.c"
"${ProjDirPath}/../../../../../../devices/LPC55S69/drivers/fsl_casper.h"
"${ProjDirPath}/../../../../../../devices/LPC55S69/drivers/fsl_hashcrypt.c"
"${ProjDirPath}/../../../../../../devices/LPC55S69/drivers/fsl_hashcrypt.h"
"${ProjDirPath}/../../../../../../devices/LPC55S69/drivers/fsl_rng.c"
"${ProjDirPath}/../../../../../../devices/LPC55S69/drivers/fsl_rng.h"
"${ProjDirPath}/../../../../../../devices/LPC55S69/drivers/fsl_i2c.c"
"${ProjDirPath}/../../../../../../devices/LPC55S69/drivers/fsl_i2c.h"
"${ProjDirPath}/../../../../../../middleware/usb/device/usb_device.h"
"${ProjDirPath}/../../../../../../middleware/usb/device/usb_device_dci.c"
"${ProjDirPath}/../../../../../../middleware/usb/device/usb_device_dci.h"
"${ProjDirPath}/../../../../../../components/osa/fsl_os_abstraction.h"
"${ProjDirPath}/../../../../../../components/osa/fsl_os_abstraction_bm.c"
"${ProjDirPath}/../../../../../../components/osa/fsl_os_abstraction_bm.h"
"${ProjDirPath}/../../../../../../components/osa/fsl_os_abstraction_config.h"
"${ProjDirPath}/../../../../../../middleware/usb/device/usb_device_lpcip3511.c"
"${ProjDirPath}/../../../../../../middleware/usb/device/usb_device_lpcip3511.h"
"${ProjDirPath}/../../../../../../components/serial_manager/serial_port_usb.c"
"${ProjDirPath}/../../../../../../components/serial_manager/serial_port_usb.h"
"${ProjDirPath}/../../../../../../components/serial_manager/usb_cdc_adapter/usb_device_descriptor.c"
"${ProjDirPath}/../../../../../../components/serial_manager/usb_cdc_adapter/usb_device_descriptor.h"
"${ProjDirPath}/../usb_device_config.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/sss/plugin/mbedtls/ecdh_alt_ax.c"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/sss/plugin/mbedtls/ecp_alt_sss.c"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/sss/plugin/mbedtls/sss_mbedtls.c"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/sss/plugin/mbedtls/sss_mbedtls_rsa.c"
"${ProjDirPath}/../../../../../../devices/LPC55S69/drivers/fsl_common.c"
"${ProjDirPath}/../../../../../../devices/LPC55S69/drivers/fsl_common.h"
"${ProjDirPath}/../../../../../../devices/LPC55S69/drivers/fsl_clock.c"
"${ProjDirPath}/../../../../../../devices/LPC55S69/drivers/fsl_clock.h"
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
"${ProjDirPath}/../../../../../../middleware/usb/device/usb_device.h"
"${ProjDirPath}/../../../../../../middleware/usb/device/usb_device_dci.h"
"${ProjDirPath}/../../../../../../middleware/usb/include/usb.h"
"${ProjDirPath}/../../../../../../middleware/usb/include/usb_misc.h"
"${ProjDirPath}/../../../../../../middleware/usb/include/usb_spec.h"
"${ProjDirPath}/../../../../../../middleware/usb/output/source/device/class/usb_device_cdc_acm.c"
"${ProjDirPath}/../../../../../../middleware/usb/output/source/device/class/usb_device_cdc_acm.h"
"${ProjDirPath}/../../../../../../middleware/usb/output/source/device/class/usb_device_class.c"
"${ProjDirPath}/../../../../../../middleware/usb/output/source/device/class/usb_device_class.h"
"${ProjDirPath}/../../../../../../middleware/usb/output/source/device/usb_device_ch9.c"
"${ProjDirPath}/../../../../../../middleware/usb/output/source/device/usb_device_ch9.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/hostlib/hostLib/inc/Applet_SE050_Ver.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/hostlib/hostLib/inc/HLSEAPI.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/hostlib/hostLib/inc/HLSEComm.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/hostlib/hostLib/inc/HLSECrypto.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/hostlib/hostLib/inc/HLSEMisc.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/hostlib/hostLib/inc/HLSEObjects.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/hostlib/hostLib/inc/HLSETypes.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/hostlib/hostLib/inc/PlugAndTrust_HostLib_Ver.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/hostlib/hostLib/inc/PlugAndTrust_Pkg_Ver.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/hostlib/hostLib/inc/SE_HAL_Lib.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/hostlib/hostLib/inc/a71ch_api.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/hostlib/hostLib/inc/a71ch_util.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/hostlib/hostLib/inc/a71cl_api.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/hostlib/hostLib/inc/a71cl_util.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/hostlib/hostLib/inc/ax_api.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/hostlib/hostLib/inc/ax_common.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/hostlib/hostLib/inc/ax_common_a71ch.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/hostlib/hostLib/inc/ax_common_a71cl.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/hostlib/hostLib/inc/ax_common_private.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/hostlib/hostLib/inc/ax_scp.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/hostlib/hostLib/inc/ax_scp_private.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/hostlib/hostLib/inc/ax_sss_scp.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/hostlib/hostLib/inc/ax_util.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/hostlib/hostLib/inc/ledHandler.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/hostlib/hostLib/inc/nxEnsure.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/hostlib/hostLib/inc/nxScp03_Apis.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/hostlib/hostLib/inc/nxScp03_Const.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/hostlib/hostLib/inc/nxScp03_Types.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/hostlib/hostLib/inc/scp.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/hostlib/hostLib/inc/se05x_const.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/hostlib/hostLib/inc/se05x_ecc_curves.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/hostlib/hostLib/inc/se05x_ecc_curves_inc.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/hostlib/hostLib/inc/se05x_ecc_curves_values.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/hostlib/hostLib/inc/se05x_enums.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/hostlib/hostLib/inc/se05x_ftr.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/hostlib/hostLib/inc/se05x_tlv.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/hostlib/hostLib/inc/sm_const.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/hostlib/hostLib/se05x/src/se05x_ECC_curves.c"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/hostlib/hostLib/se05x/src/se05x_mw.c"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/hostlib/hostLib/se05x/src/se05x_tlv.c"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/sss/inc/fsl_sscp.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/sss/inc/fsl_sscp_a71ch.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/sss/inc/fsl_sscp_a71cl.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/sss/inc/fsl_sscp_commands.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/sss/inc/fsl_sscp_mu.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/sss/inc/fsl_sss_api.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/sss/inc/fsl_sss_api_ver.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/sss/inc/fsl_sss_config.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/sss/inc/fsl_sss_ftr_default.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/sss/inc/fsl_sss_keyid_map.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/sss/inc/fsl_sss_lpc55s_apis.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/sss/inc/fsl_sss_lpc55s_types.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/sss/inc/fsl_sss_mbedtls_apis.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/sss/inc/fsl_sss_mbedtls_types.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/sss/inc/fsl_sss_openssl_apis.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/sss/inc/fsl_sss_openssl_types.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/sss/inc/fsl_sss_policy.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/sss/inc/fsl_sss_se05x_apis.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/sss/inc/fsl_sss_se05x_policy.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/sss/inc/fsl_sss_se05x_scp03.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/sss/inc/fsl_sss_se05x_types.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/sss/inc/fsl_sss_sscp.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/sss/inc/fsl_sss_sscp_config.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/sss/inc/fsl_sss_user_apis.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/sss/inc/fsl_sss_user_types.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/sss/inc/fsl_sss_util_asn1_der.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/sss/inc/fsl_sss_util_rsa_sign_utils.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/sss/port/ksdk/fsl_sss_types.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/sss/src/fsl_sss_apis.c"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/sss/src/fsl_sss_util_asn1_der.c"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/sss/src/fsl_sss_util_rsa_sign_utils.c"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/sss/src/se05x/fsl_sss_se05x_apis.c"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/sss/src/se05x/fsl_sss_se05x_eckey.c"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/sss/src/se05x/fsl_sss_se05x_mw.c"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/sss/src/se05x/fsl_sss_se05x_policy.c"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/sss/src/se05x/fsl_sss_se05x_scp03.c"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/hostlib/hostLib/platform/inc/ax_reset.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/hostlib/hostLib/platform/inc/i2c_a7.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/hostlib/hostLib/platform/inc/se05x_apis.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/hostlib/hostLib/platform/inc/se_pit_config.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/hostlib/hostLib/platform/inc/se_reset_config.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/hostlib/hostLib/platform/inc/sm_printf.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/hostlib/hostLib/platform/inc/sm_timer.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/hostlib/hostLib/platform/ksdk/ax_reset.c"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/hostlib/hostLib/platform/ksdk/i2c_frdm.c"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/hostlib/hostLib/platform/ksdk/i2c_imxrt10xx.c"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/hostlib/hostLib/platform/ksdk/i2c_lpc54xxx.c"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/hostlib/hostLib/platform/ksdk/i2c_lpc55sxx.c"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/hostlib/hostLib/platform/ksdk/se05x_reset.c"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/hostlib/hostLib/platform/ksdk/timer_kinetis.c"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/hostlib/hostLib/platform/ksdk/timer_kinetis_bm.c"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/hostlib/hostLib/platform/ksdk/timer_kinetis_freertos.c"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/hostlib/hostLib/libCommon/smCom/T1oI2C/phEseStatus.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/hostlib/hostLib/libCommon/smCom/T1oI2C/phEseTypes.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/hostlib/hostLib/libCommon/smCom/T1oI2C/phNxpEsePal_i2c.c"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/hostlib/hostLib/libCommon/smCom/T1oI2C/phNxpEsePal_i2c.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/hostlib/hostLib/libCommon/smCom/T1oI2C/phNxpEseProto7816_3.c"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/hostlib/hostLib/libCommon/smCom/T1oI2C/phNxpEseProto7816_3.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/hostlib/hostLib/libCommon/smCom/T1oI2C/phNxpEse_Api.c"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/hostlib/hostLib/libCommon/smCom/T1oI2C/phNxpEse_Api.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/hostlib/hostLib/libCommon/smCom/T1oI2C/phNxpEse_Internal.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/hostlib/hostLib/libCommon/smCom/apduComm.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/hostlib/hostLib/libCommon/smCom/sci2c_cfg.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/hostlib/hostLib/libCommon/smCom/smCom.c"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/hostlib/hostLib/libCommon/smCom/smCom.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/hostlib/hostLib/libCommon/smCom/smComT1oI2C.c"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/hostlib/hostLib/libCommon/smCom/smComT1oI2C.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/hostlib/hostLib/libCommon/infra/a71_debug.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/hostlib/hostLib/libCommon/infra/app_boot.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/hostlib/hostLib/libCommon/infra/app_boot_nfc.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/hostlib/hostLib/libCommon/infra/global_platf.c"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/hostlib/hostLib/libCommon/infra/global_platf.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/hostlib/hostLib/libCommon/infra/nxLog.c"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/hostlib/hostLib/libCommon/infra/nxLog.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/hostlib/hostLib/libCommon/infra/nxLog_App.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/hostlib/hostLib/libCommon/infra/nxLog_DefaultConfig.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/hostlib/hostLib/libCommon/infra/nxLog_UseCases.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/hostlib/hostLib/libCommon/infra/nxLog_VCOM.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/hostlib/hostLib/libCommon/infra/nxLog_hostLib.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/hostlib/hostLib/libCommon/infra/nxLog_mbedtls.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/hostlib/hostLib/libCommon/infra/nxLog_scp.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/hostlib/hostLib/libCommon/infra/nxLog_smCom.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/hostlib/hostLib/libCommon/infra/nxLog_sscp.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/hostlib/hostLib/libCommon/infra/nxLog_sss.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/hostlib/hostLib/libCommon/infra/sm_apdu.c"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/hostlib/hostLib/libCommon/infra/sm_apdu.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/hostlib/hostLib/libCommon/infra/sm_api.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/hostlib/hostLib/libCommon/infra/sm_connect.c"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/hostlib/hostLib/libCommon/infra/sm_demo_utils.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/hostlib/hostLib/libCommon/infra/sm_errors.c"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/hostlib/hostLib/libCommon/infra/sm_errors.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/hostlib/hostLib/libCommon/infra/sm_printf.c"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/hostlib/hostLib/libCommon/infra/sm_types.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/sss/ex/inc/ex_a71ch_scp03.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/sss/ex/inc/ex_scp03_puf.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/sss/ex/inc/ex_sss.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/sss/ex/inc/ex_sss_auth.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/sss/ex/inc/ex_sss_boot.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/sss/ex/inc/ex_sss_main_freeRTOS_inc.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/sss/ex/inc/ex_sss_main_inc.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/sss/ex/inc/ex_sss_main_inc_frdmk64f.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/sss/ex/inc/ex_sss_main_inc_imx_rt.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/sss/ex/inc/ex_sss_main_inc_ksdk.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/sss/ex/inc/ex_sss_main_inc_linux.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/sss/ex/inc/ex_sss_main_inc_lpcxpresso55s.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/sss/ex/inc/ex_sss_objid.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/sss/ex/inc/ex_sss_ports.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/sss/ex/inc/ex_sss_scp03_keys.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/sss/ex/inc/ex_sss_tp_scp03_keys.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/sss/ex/src/ex_sss_boot.c"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/sss/ex/src/ex_sss_boot_connectstring.c"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/sss/ex/src/ex_sss_boot_int.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/sss/ex/src/ex_sss_se05x.c"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/sss/ex/src/ex_sss_se05x_auth.c"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/hostlib/hostLib/se05x_03_xx_xx/se05x_APDU.c"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/hostlib/hostLib/se05x_03_xx_xx/se05x_APDU.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/hostlib/hostLib/se05x_03_xx_xx/se05x_APDU_apis.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/hostlib/hostLib/se05x_03_xx_xx/se05x_APDU_impl.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/hostlib/hostLib/libCommon/nxScp/nxScp03_Com.c"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/hostlib/hostLib/libCommon/scp/scp.c"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/hostlib/hostLib/mbedtls/inc/a71ch_mbedtls_evkbimxrt1050_config.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/hostlib/hostLib/mbedtls/inc/a71ch_mbedtls_evkmimxrt1050_config.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/hostlib/hostLib/mbedtls/inc/a71ch_mbedtls_frdm_config.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/hostlib/hostLib/mbedtls/inc/a71ch_mbedtls_frdmk64f_config.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/hostlib/hostLib/mbedtls/inc/a71ch_mbedtls_frdmk82f_config.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/hostlib/hostLib/mbedtls/inc/a71ch_mbedtls_frdmkw41z_config.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/hostlib/hostLib/mbedtls/inc/a71ch_mbedtls_lpc54018iotmodule_config.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/hostlib/hostLib/mbedtls/inc/a71ch_mbedtls_lpcxpresso54018_config.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/hostlib/hostLib/mbedtls/inc/a71ch_mbedtls_lpcxpresso55s69_config.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/hostlib/hostLib/mbedtls/inc/a71ch_mbedtls_pi_config.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/hostlib/hostLib/mbedtls/inc/a71ch_mbedtls_x86_config.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/hostlib/hostLib/mbedtls/inc/ax_mbedtls.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/hostlib/hostLib/mbedtls/inc/ecp_alt.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/hostlib/hostLib/mbedtls/src/ecdh_alt.c"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/hostlib/hostLib/mbedtls/src/rsa_alt.c"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/sss/plugin/mbedtls/ecp_alt.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/sss/plugin/mbedtls/rsa_alt.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/sss/plugin/mbedtls/sss_mbedtls.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/sss/plugin/mbedtls/sss_mbedtls_evkbimxrt1050_config.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/sss/plugin/mbedtls/sss_mbedtls_frdmk64f_config.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/sss/plugin/mbedtls/sss_mbedtls_frdmkw41z_config.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/sss/plugin/mbedtls/sss_mbedtls_lpcxpresso55s69_config.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/sss/plugin/mbedtls/sss_mbedtls_x86_config.h"
"${ProjDirPath}/../../../../../../middleware/se_hostlib/sss/src/mbedtls/fsl_sss_mbedtls_apis.c"
"${ProjDirPath}/../../../../../../devices/LPC55S69/utilities/fsl_sbrk.c"
)


set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -T${ProjDirPath}/LPC55S69_cm33_core0_flash.ld -static")

set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -T${ProjDirPath}/LPC55S69_cm33_core0_flash.ld -static")

TARGET_LINK_LIBRARIES(se_hostlib_vcomSE050.elf -Wl,--start-group)
target_link_libraries(se_hostlib_vcomSE050.elf debug m)

target_link_libraries(se_hostlib_vcomSE050.elf debug c)

target_link_libraries(se_hostlib_vcomSE050.elf debug gcc)

target_link_libraries(se_hostlib_vcomSE050.elf debug nosys)

target_link_libraries(se_hostlib_vcomSE050.elf optimized m)

target_link_libraries(se_hostlib_vcomSE050.elf optimized c)

target_link_libraries(se_hostlib_vcomSE050.elf optimized gcc)

target_link_libraries(se_hostlib_vcomSE050.elf optimized nosys)

link_directories(${ProjDirPath}/../../../../../../devices/LPC55S69/gcc)

target_link_libraries(se_hostlib_vcomSE050.elf debug ${ProjDirPath}/../../../../../../devices/LPC55S69/gcc/libpower_hardabi.a)

link_directories(${ProjDirPath}/../../../../../../devices/LPC55S69/gcc)

target_link_libraries(se_hostlib_vcomSE050.elf optimized ${ProjDirPath}/../../../../../../devices/LPC55S69/gcc/libpower_hardabi.a)

TARGET_LINK_LIBRARIES(se_hostlib_vcomSE050.elf -Wl,--end-group)


