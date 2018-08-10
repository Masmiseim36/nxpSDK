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

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DCPU_MK64FN1M0VLL12")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DFRDM_K64F")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DFREEDOM")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DUSB_STACK_BM")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DNO_SECURE_CHANNEL_SUPPORT")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DSCP_MODE=C_MAC_C_ENC_R_MAC_R_ENC")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DTGT_A71CH")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DI2C")

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

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DCPU_MK64FN1M0VLL12")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DFRDM_K64F")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DFREEDOM")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DUSB_STACK_BM")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DNO_SECURE_CHANNEL_SUPPORT")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DSCP_MODE=C_MAC_C_ENC_R_MAC_R_ENC")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DTGT_A71CH")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DI2C")

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

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DCPU_MK64FN1M0VLL12")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DTGT_A71CH")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DI2C")

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

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DCPU_MK64FN1M0VLL12")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DTGT_A71CH")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DI2C")

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

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -mcpu=cortex-m4")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -mfloat-abi=hard")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -mfpu=fpv4-sp-d16")

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

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -mcpu=cortex-m4")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -mfloat-abi=hard")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -mfpu=fpv4-sp-d16")

include_directories(${ProjDirPath}/../../../../../CMSIS/Include)

include_directories(${ProjDirPath}/../../../../../devices)

include_directories(${ProjDirPath}/../../..)

include_directories(${ProjDirPath}/..)

include_directories(${ProjDirPath}/../../../usb_examples/usb_device_cdc_vcom/bm)

include_directories(${ProjDirPath}/../../../../../middleware/usb/example/usb_device_cdc_vcom/bm)

include_directories(${ProjDirPath}/../../../../../middleware/se_hostlib/hostLib/tstUtil)

include_directories(${ProjDirPath}/../../../../../middleware/se_hostlib/platform)

include_directories(${ProjDirPath}/../../../../../middleware/se_hostlib/ax)

include_directories(${ProjDirPath}/../../../../../middleware/mmcau)

include_directories(${ProjDirPath}/../../../../../middleware/se_hostlib)

include_directories(${ProjDirPath}/../../../../../middleware/usb/device)

include_directories(${ProjDirPath}/../../../../../middleware/usb/include)

include_directories(${ProjDirPath}/../../../../../middleware/usb/osa)

include_directories(${ProjDirPath}/../../../../../devices/MK64F12/drivers)

include_directories(${ProjDirPath}/../../../../../devices/MK64F12/utilities/io)

include_directories(${ProjDirPath}/../../../../../devices/MK64F12/utilities/str)

include_directories(${ProjDirPath}/../../../../../devices/MK64F12/utilities/log)

include_directories(${ProjDirPath}/../../../../../devices/MK64F12/utilities)

include_directories(${ProjDirPath}/../../../../../devices/MK64F12)

include_directories(${ProjDirPath}/../../../../../middleware/se_hostlib/hostLib/libCommon/hostCrypto)

include_directories(${ProjDirPath}/../../../../../middleware/se_hostlib/hostLib/libCommon/infra)

include_directories(${ProjDirPath}/../../../../../middleware/se_hostlib/hostLib/libCommon/scp)

include_directories(${ProjDirPath}/../../../../../middleware/se_hostlib/hostLib/libCommon/smCom)

include_directories(${ProjDirPath}/../../../../../middleware/se_hostlib/hostLib/platform/inc)

add_executable(vcomA71CH.elf 
"${ProjDirPath}/../virtual_com.c"
"${ProjDirPath}/../virtual_com.h"
"${ProjDirPath}/../vcom2i2c.c"
"${ProjDirPath}/../vcom2i2c.h"
"${ProjDirPath}/../usb_device_descriptor.c"
"${ProjDirPath}/../usb_device_config.h"
"${ProjDirPath}/../usb_device_descriptor.h"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
"${ProjDirPath}/../../../../../middleware/usb/device/usb_device_khci.c"
"${ProjDirPath}/../../../../../middleware/usb/device/usb_device_khci.h"
"${ProjDirPath}/../../../../../middleware/usb/include/usb_khci.h"
"${ProjDirPath}/../../../../../middleware/usb/include/usb.h"
"${ProjDirPath}/../../../../../middleware/usb/include/usb_misc.h"
"${ProjDirPath}/../../../../../middleware/usb/include/usb_spec.h"
"${ProjDirPath}/../../../../../middleware/usb/osa/usb_osa.h"
"${ProjDirPath}/../../../../../middleware/usb/osa/usb_osa_bm.h"
"${ProjDirPath}/../../../../../middleware/usb/osa/usb_osa_bm.c"
"${ProjDirPath}/../../../../../devices/MK64F12/drivers/fsl_sysmpu.h"
"${ProjDirPath}/../../../../../devices/MK64F12/drivers/fsl_sysmpu.c"
"${ProjDirPath}/../../../../../devices/MK64F12/drivers/fsl_common.h"
"${ProjDirPath}/../../../../../devices/MK64F12/drivers/fsl_common.c"
"${ProjDirPath}/../../../../../devices/MK64F12/drivers/fsl_clock.h"
"${ProjDirPath}/../../../../../devices/MK64F12/drivers/fsl_clock.c"
"${ProjDirPath}/../../../../../middleware/mmcau/cau_api.h"
"${ProjDirPath}/../../../../../middleware/mmcau/fsl_mmcau.h"
"${ProjDirPath}/../../../../../middleware/mmcau/fsl_mmcau.c"
"${ProjDirPath}/../../../../../devices/MK64F12/drivers/fsl_rnga.c"
"${ProjDirPath}/../../../../../devices/MK64F12/drivers/fsl_rnga.h"
"${ProjDirPath}/../../../../../devices/MK64F12/drivers/fsl_i2c.h"
"${ProjDirPath}/../../../../../devices/MK64F12/drivers/fsl_i2c.c"
"${ProjDirPath}/../../../../../devices/MK64F12/drivers/fsl_ftfx_adapter.h"
"${ProjDirPath}/../../../../../devices/MK64F12/drivers/fsl_ftfx_utilities.h"
"${ProjDirPath}/../../../../../devices/MK64F12/drivers/fsl_ftfx_features.h"
"${ProjDirPath}/../../../../../devices/MK64F12/drivers/fsl_ftfx_controller.c"
"${ProjDirPath}/../../../../../devices/MK64F12/drivers/fsl_ftfx_controller.h"
"${ProjDirPath}/../../../../../devices/MK64F12/drivers/fsl_ftfx_flash.c"
"${ProjDirPath}/../../../../../devices/MK64F12/drivers/fsl_ftfx_flash.h"
"${ProjDirPath}/../../../../../devices/MK64F12/drivers/fsl_ftfx_cache.c"
"${ProjDirPath}/../../../../../devices/MK64F12/drivers/fsl_ftfx_cache.h"
"${ProjDirPath}/../../../../../devices/MK64F12/drivers/fsl_ftfx_flexnvm.c"
"${ProjDirPath}/../../../../../devices/MK64F12/drivers/fsl_ftfx_flexnvm.h"
"${ProjDirPath}/../../../../../devices/MK64F12/drivers/fsl_flash.h"
"${ProjDirPath}/../../../../../devices/MK64F12/drivers/fsl_gpio.h"
"${ProjDirPath}/../../../../../devices/MK64F12/drivers/fsl_gpio.c"
"${ProjDirPath}/../../../../../devices/MK64F12/utilities/io/fsl_io.c"
"${ProjDirPath}/../../../../../devices/MK64F12/utilities/io/fsl_io.h"
"${ProjDirPath}/../../../../../devices/MK64F12/utilities/log/fsl_log.c"
"${ProjDirPath}/../../../../../devices/MK64F12/utilities/log/fsl_log.h"
"${ProjDirPath}/../../../../../devices/MK64F12/utilities/str/fsl_str.c"
"${ProjDirPath}/../../../../../devices/MK64F12/utilities/str/fsl_str.h"
"${ProjDirPath}/../../../../../devices/MK64F12/utilities/fsl_debug_console.c"
"${ProjDirPath}/../../../../../devices/MK64F12/utilities/fsl_debug_console.h"
"${ProjDirPath}/../../../../../devices/MK64F12/utilities/fsl_debug_console_conf.h"
"${ProjDirPath}/../../../../../devices/MK64F12/drivers/fsl_port.h"
"${ProjDirPath}/../../../../../devices/MK64F12/drivers/fsl_uart.h"
"${ProjDirPath}/../../../../../devices/MK64F12/drivers/fsl_uart.c"
"${ProjDirPath}/../../../../../devices/MK64F12/drivers/fsl_smc.h"
"${ProjDirPath}/../../../../../devices/MK64F12/drivers/fsl_smc.c"
"${ProjDirPath}/../../../../../devices/MK64F12/system_MK64F12.c"
"${ProjDirPath}/../../../../../devices/MK64F12/system_MK64F12.h"
"${ProjDirPath}/../../../../../devices/MK64F12/gcc/startup_MK64F12.S"
"${ProjDirPath}/../../../../../devices/MK64F12/utilities/fsl_assert.c"
"${ProjDirPath}/../../../../../middleware/se_hostlib/hostLib/libCommon/infra/global_platf.c"
"${ProjDirPath}/../../../../../middleware/se_hostlib/hostLib/libCommon/infra/global_platf.h"
"${ProjDirPath}/../../../../../middleware/se_hostlib/hostLib/libCommon/infra/sm_apdu.c"
"${ProjDirPath}/../../../../../middleware/se_hostlib/hostLib/libCommon/infra/sm_apdu.h"
"${ProjDirPath}/../../../../../middleware/se_hostlib/hostLib/libCommon/infra/sm_errors.c"
"${ProjDirPath}/../../../../../middleware/se_hostlib/hostLib/libCommon/infra/sm_errors.h"
"${ProjDirPath}/../../../../../middleware/se_hostlib/hostLib/libCommon/infra/sm_types.h"
"${ProjDirPath}/../../../../../middleware/se_hostlib/hostLib/libCommon/scp/scp.c"
"${ProjDirPath}/../../../../../middleware/se_hostlib/hostLib/libCommon/scp/scp.h"
"${ProjDirPath}/../../../../../middleware/se_hostlib/hostLib/libCommon/smCom/apduComm.h"
"${ProjDirPath}/../../../../../middleware/se_hostlib/hostLib/libCommon/smCom/sci2c.c"
"${ProjDirPath}/../../../../../middleware/se_hostlib/hostLib/libCommon/smCom/sci2c.h"
"${ProjDirPath}/../../../../../middleware/se_hostlib/hostLib/libCommon/smCom/sci2c_cfg.h"
"${ProjDirPath}/../../../../../middleware/se_hostlib/hostLib/libCommon/smCom/smCom.c"
"${ProjDirPath}/../../../../../middleware/se_hostlib/hostLib/libCommon/smCom/smComSCI2C.c"
"${ProjDirPath}/../../../../../middleware/se_hostlib/hostLib/platform/generic/sm_printf.c"
"${ProjDirPath}/../../../../../middleware/se_hostlib/hostLib/platform/inc/i2c_a7.h"
"${ProjDirPath}/../../../../../middleware/se_hostlib/hostLib/platform/inc/sm_printf.h"
"${ProjDirPath}/../../../../../middleware/se_hostlib/hostLib/platform/inc/sm_timer.h"
"${ProjDirPath}/../../../../../middleware/se_hostlib/hostLib/platform/kinetis/i2c_kinetis.c"
"${ProjDirPath}/../../../../../middleware/se_hostlib/hostLib/platform/kinetis/timer_kinetis.c"
"${ProjDirPath}/../../../../../middleware/se_hostlib/hostLib/platform/kinetis/timer_kinetis_bm.c"
"${ProjDirPath}/../../../../../middleware/se_hostlib/hostLib/platform/kinetis/timer_kinetis_freertos.c"
"${ProjDirPath}/../../../../../middleware/se_hostlib/hostLib/libCommon/smCom/smCom.h"
"${ProjDirPath}/../../../../../middleware/se_hostlib/hostLib/libCommon/smCom/smComSCI2C.h"
"${ProjDirPath}/../../../../../middleware/se_hostlib/hostLib/libCommon/smCom/sci2c_cfg.h"
"${ProjDirPath}/../../../../../middleware/usb/device/usb_device.h"
"${ProjDirPath}/../../../../../middleware/usb/device/usb_device_dci.c"
"${ProjDirPath}/../../../../../middleware/usb/device/usb_device_dci.h"
"${ProjDirPath}/../usb_device_ch9.c"
"${ProjDirPath}/../usb_device_ch9.h"
"${ProjDirPath}/../usb_device_class.c"
"${ProjDirPath}/../usb_device_class.h"
"${ProjDirPath}/../usb_device_cdc_acm.c"
"${ProjDirPath}/../usb_device_cdc_acm.h"
)


set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -T${ProjDirPath}/MK64FN1M0xxx12_flash.ld -static")

set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -T${ProjDirPath}/MK64FN1M0xxx12_flash.ld -static")

TARGET_LINK_LIBRARIES(vcomA71CH.elf -Wl,--start-group)
target_link_libraries(vcomA71CH.elf debug m)

target_link_libraries(vcomA71CH.elf debug c)

target_link_libraries(vcomA71CH.elf debug gcc)

target_link_libraries(vcomA71CH.elf debug nosys)

target_link_libraries(vcomA71CH.elf optimized m)

target_link_libraries(vcomA71CH.elf optimized c)

target_link_libraries(vcomA71CH.elf optimized gcc)

target_link_libraries(vcomA71CH.elf optimized nosys)

link_directories(${ProjDirPath}/../../../../../middleware/mmcau/asm-cm4-cm7)

target_link_libraries(vcomA71CH.elf debug ${ProjDirPath}/../../../../../middleware/mmcau/asm-cm4-cm7/lib_mmcau.a)

link_directories(${ProjDirPath}/../../../../../middleware/mmcau/asm-cm4-cm7)

target_link_libraries(vcomA71CH.elf optimized ${ProjDirPath}/../../../../../middleware/mmcau/asm-cm4-cm7/lib_mmcau.a)

TARGET_LINK_LIBRARIES(vcomA71CH.elf -Wl,--end-group)

