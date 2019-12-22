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


SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -DNDEBUG")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -D__STARTUP_CLEAR_BSS")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -g")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -mcpu=cortex-m7")

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

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DNDEBUG")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DCPU_MIMXRT1064DVL6A")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DCPU_IS_ARM_CORTEX_M7=1")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DBL_TARGET_RAM")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DMIMXRT1064")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DUSB_STACK_BM")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -g")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Os")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -mcpu=cortex-m7")

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

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DNDEBUG")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -g")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Os")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -mcpu=cortex-m7")

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

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -g")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -mcpu=cortex-m7")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -mfloat-abi=hard")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -mfpu=fpv5-d16")

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

include_directories(${ProjDirPath}/../../../../../CMSIS/Include)

include_directories(${ProjDirPath}/../../../../../middleware/mcu-boot/src)

include_directories(${ProjDirPath}/../../../../../middleware/mcu-boot/src/bootloader)

include_directories(${ProjDirPath}/../../../../../middleware/mcu-boot/src/bm_usb)

include_directories(${ProjDirPath}/../../../../../middleware/mcu-boot/src/include)

include_directories(${ProjDirPath}/../../../../../middleware/mcu-boot/src/startup)

include_directories(${ProjDirPath}/../../../../../middleware/mcu-boot/src/utilities)

include_directories(${ProjDirPath}/../../../../../middleware/mcu-boot/src/drivers)

include_directories(${ProjDirPath}/../../../../../middleware/mcu-boot/src/drivers/crc)

include_directories(${ProjDirPath}/../../../../../middleware/mcu-boot/src/drivers/microseconds)

include_directories(${ProjDirPath}/../../../../../middleware/mcu-boot/src/drivers/smc)

include_directories(${ProjDirPath}/../../../../../middleware/mcu-boot/targets/MIMXRT1064/src)

include_directories(${ProjDirPath}/../../../../../middleware/mcu-boot/src/autobaud)

include_directories(${ProjDirPath}/../../../../../middleware/mcu-boot/src/drivers/sdmmc/inc)

include_directories(${ProjDirPath}/../../../../../middleware/mcu-boot/src/drivers/lpuart)

include_directories(${ProjDirPath}/../../../../../middleware/mcu-boot/targets/MIMXRT1064)

include_directories(${ProjDirPath}/../../../../../middleware/mcu-boot/targets/MIMXRT1064/drivers)

include_directories(${ProjDirPath}/..)

include_directories(${ProjDirPath}/../../../../../devices/MIMXRT1064)

include_directories(${ProjDirPath}/../../../../../middleware/usb/device)

include_directories(${ProjDirPath}/../../../../../middleware/usb/include)

include_directories(${ProjDirPath}/../../../../../middleware/usb/phy)

include_directories(${ProjDirPath}/../../../../../components/osa)

include_directories(${ProjDirPath}/../../../../../devices/MIMXRT1064/drivers)

include_directories(${ProjDirPath}/../../../../../components/lists)

add_executable(flashloader.elf 
"${ProjDirPath}/../../../../../devices/MIMXRT1064/MIMXRT1064.h"
"${ProjDirPath}/../../../../../devices/MIMXRT1064/MIMXRT1064_features.h"
"${ProjDirPath}/../../../../../devices/MIMXRT1064/fsl_device_registers.h"
"${ProjDirPath}/../../../../../devices/MIMXRT1064/system_MIMXRT1064.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/startup/crt0_gcc.S"
"${ProjDirPath}/../../../../../middleware/mcu-boot/targets/MIMXRT1064/src/startup/gcc/startup_MIMXRT1064.S"
"${ProjDirPath}/../../../../../middleware/mcu-boot/targets/MIMXRT1064/src/startup/system_MIMXRT1064.c"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/bm_usb/bootloader_hid_report_ids.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/bm_usb/composite.c"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/bm_usb/composite.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/bm_usb/fat_directory_entry.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/bm_usb/hid_bootloader.c"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/bm_usb/hid_bootloader.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/bm_usb/msc_disk.c"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/bm_usb/msc_disk.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/bm_usb/usb_descriptor.c"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/bm_usb/usb_descriptor.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/autobaud/autobaud.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/autobaud/src/autobaud_irq.c"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/bootloader/bl_app_crc_check.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/bootloader/bl_command.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/bootloader/bl_context.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/bootloader/bl_irq_common.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/bootloader/bl_peripheral.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/bootloader/bl_peripheral_interface.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/bootloader/bl_shutdown_cleanup.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/bootloader/bl_user_entry.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/bootloader/bl_version.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/bootloader/bootloader.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/bootloader/src/bl_app_crc_check.c"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/bootloader/src/bl_command.c"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/bootloader/src/bl_context.c"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/bootloader/src/bl_exception_handler.c"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/bootloader/src/bl_main.c"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/bootloader/src/bl_misc.c"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/bootloader/src/bl_shutdown_cleanup.c"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/bootloader/src/bl_tree_root.c"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/bootloader/src/bl_user_entry.c"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/crc/crc16.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/crc/crc32.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/crc/src/crc16.c"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/crc/src/crc32.c"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/include/bootloader_common.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/memory/memory.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/memory/src/device_memory.c"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/memory/src/device_memory.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/memory/src/memory.c"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/memory/src/normal_memory.c"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/memory/src/normal_memory.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/memory/src/pattern_fill.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/memory/src/pattern_fill_gcc.S"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/memory/src/sram_init.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/packet/command_packet.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/packet/serial_packet.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/packet/src/serial_packet.c"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/property/property.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/sbloader/sb_file_format.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/sbloader/sbloader.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/sbloader/src/sbloader.c"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/startup/ar_asm_macros.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/utilities/fsl_assert.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/utilities/fsl_rtos_abstraction.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/utilities/src/debug_util.c"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/utilities/src/fsl_assert.c"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/utilities/src/fsl_rtos_abstraction.c"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/utilities/vector_table_info.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/bootloader/bl_keyblob.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/bootloader/bl_nor_encrypt.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/bootloader/src/bl_keyblob.c"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/bootloader/src/bl_keyblob_dcp.c"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/bootloader/src/bl_nor_encrypt_bee.c"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/bootloader/src/lpuart_peripheral_interface.c"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/bootloader/src/usb_hid_msc_peripheral_interface.c"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/drivers/dcp/fsl_dcp.c"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/drivers/dcp/fsl_dcp.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/drivers/igpio/fsl_gpio.c"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/drivers/igpio/fsl_gpio.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/drivers/lpuart/fsl_lpuart.c"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/drivers/lpuart/fsl_lpuart.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/drivers/port/fsl_port.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/drivers/smc/smc.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/drivers/trng/fsl_trng.c"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/drivers/trng/fsl_trng.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/property/src/property_imx.c"
"${ProjDirPath}/../../../../../middleware/mcu-boot/targets/MIMXRT1064/src/fsl_rtwdog.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/targets/MIMXRT1064/src/fsl_wdog.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/targets/MIMXRT1064/src/bl_api.c"
"${ProjDirPath}/../../../../../middleware/mcu-boot/targets/MIMXRT1064/src/bl_api.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/targets/MIMXRT1064/src/bl_lpuart_irq_config_MIMXRT1064.c"
"${ProjDirPath}/../../../../../middleware/mcu-boot/targets/MIMXRT1064/src/bootloader_config.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/targets/MIMXRT1064/src/clock_config_MIMXRT1064.c"
"${ProjDirPath}/../../../../../middleware/mcu-boot/targets/MIMXRT1064/src/external_memory_property_map_MIMXRT1064.c"
"${ProjDirPath}/../../../../../middleware/mcu-boot/targets/MIMXRT1064/src/flexspi_nand_config_MIMXRT1064.c"
"${ProjDirPath}/../../../../../middleware/mcu-boot/targets/MIMXRT1064/src/flexspi_nor_config_MIMXRT1064.c"
"${ProjDirPath}/../../../../../middleware/mcu-boot/targets/MIMXRT1064/src/fsl_iomuxc.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/targets/MIMXRT1064/src/fusemap.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/targets/MIMXRT1064/src/hardware_init_MIMXRT1064.c"
"${ProjDirPath}/../../../../../middleware/mcu-boot/targets/MIMXRT1064/src/memory_map_MIMXRT1064.c"
"${ProjDirPath}/../../../../../middleware/mcu-boot/targets/MIMXRT1064/src/peripherals_MIMXRT1064.c"
"${ProjDirPath}/../../../../../middleware/mcu-boot/targets/MIMXRT1064/src/peripherals_pinmux.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/targets/MIMXRT1064/src/sdmmc_config_MIMXRT1064.c"
"${ProjDirPath}/../../../../../middleware/mcu-boot/targets/MIMXRT1064/src/sdmmc_init.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/targets/MIMXRT1064/src/semc_nand_config_MIMXRT1064.c"
"${ProjDirPath}/../../../../../middleware/mcu-boot/targets/MIMXRT1064/src/semc_nor_config_MIMXRT1064.c"
"${ProjDirPath}/../../../../../middleware/mcu-boot/targets/MIMXRT1064/src/sram_init_MIMXRT1064.c"
"${ProjDirPath}/../../../../../middleware/mcu-boot/targets/MIMXRT1064/src/target_config.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/targets/MIMXRT1064/src/usb_device_config.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/targets/common/src/pinmux_utility_imxrt_series.c"
"${ProjDirPath}/../../../../../middleware/usb/device/usb_device_ehci.c"
"${ProjDirPath}/../../../../../middleware/usb/device/usb_device_ehci.h"
"${ProjDirPath}/../../../../../middleware/usb/include/usb.h"
"${ProjDirPath}/../../../../../middleware/usb/include/usb_misc.h"
"${ProjDirPath}/../../../../../middleware/usb/include/usb_spec.h"
"${ProjDirPath}/../usb_device_hid.c"
"${ProjDirPath}/../usb_device_hid.h"
"${ProjDirPath}/../usb_device_msc.c"
"${ProjDirPath}/../usb_device_msc.h"
"${ProjDirPath}/../usb_device_msc_ufi.c"
"${ProjDirPath}/../usb_device_msc_ufi.h"
"${ProjDirPath}/../usb_device_ch9.c"
"${ProjDirPath}/../usb_device_ch9.h"
"${ProjDirPath}/../usb_device_class.c"
"${ProjDirPath}/../usb_device_class.h"
"${ProjDirPath}/../../../../../middleware/usb/device/usb_device.h"
"${ProjDirPath}/../../../../../middleware/usb/device/usb_device_dci.c"
"${ProjDirPath}/../../../../../middleware/usb/device/usb_device_dci.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/include/bootloader_core.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/startup/startup.c"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/drivers/microseconds/microseconds.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/drivers/microseconds/src/microseconds_pit.c"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/memory/src/flexspi_nor_memory.c"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/memory/src/flexspi_nor_memory.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/drivers/flexspi/fsl_flexspi.c"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/drivers/flexspi/fsl_flexspi.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/drivers/flexspi_nand/flexspi_nand_flash.c"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/drivers/flexspi_nand/flexspi_nand_flash.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/memory/src/spinand_memory.c"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/memory/src/spinand_memory.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/memory/src/semc_nor_memory.c"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/memory/src/semc_nor_memory.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/memory/src/semc_nand_memory.c"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/memory/src/semc_nand_memory.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/memory/src/mmc_memory.c"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/memory/src/mmc_memory.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/memory/src/sd_memory.c"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/memory/src/sd_memory.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/memory/src/spi_nor_eeprom_memory.c"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/memory/src/spi_nor_eeprom_memory.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/drivers/ocotp/fsl_ocotp.c"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/drivers/ocotp/fsl_ocotp.h"
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
"${ProjDirPath}/../../../../../middleware/usb/phy/usb_phy.c"
"${ProjDirPath}/../../../../../middleware/usb/phy/usb_phy.h"
"${ProjDirPath}/../../../../../components/osa/fsl_os_abstraction.h"
"${ProjDirPath}/../../../../../components/osa/fsl_os_abstraction_bm.c"
"${ProjDirPath}/../../../../../components/osa/fsl_os_abstraction_bm.h"
"${ProjDirPath}/../../../../../components/osa/fsl_os_abstraction_config.h"
"${ProjDirPath}/../../../../../devices/MIMXRT1064/drivers/fsl_common.c"
"${ProjDirPath}/../../../../../devices/MIMXRT1064/drivers/fsl_common.h"
"${ProjDirPath}/../../../../../devices/MIMXRT1064/drivers/fsl_clock.c"
"${ProjDirPath}/../../../../../devices/MIMXRT1064/drivers/fsl_clock.h"
"${ProjDirPath}/../../../../../components/lists/generic_list.c"
"${ProjDirPath}/../../../../../components/lists/generic_list.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/drivers/flexspi/fsl_flexspi.c"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/drivers/flexspi/fsl_flexspi.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/drivers/flexspi_nor/flexspi_nor_flash.c"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/drivers/flexspi_nor/flexspi_nor_flash.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/drivers/semc/fsl_semc.c"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/drivers/semc/fsl_semc.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/drivers/semc_nor/semc_nor_flash.c"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/drivers/semc_nor/semc_nor_flash.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/drivers/nand_ecc/nand_ecc.c"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/drivers/nand_ecc/nand_ecc.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/drivers/semc/fsl_semc.c"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/drivers/semc/fsl_semc.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/drivers/semc_nand/semc_nand_flash.c"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/drivers/semc_nand/semc_nand_flash.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/drivers/gpio/fsl_gpio.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/drivers/sdmmc/inc/fsl_card.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/drivers/sdmmc/inc/fsl_host.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/drivers/sdmmc/inc/fsl_sdmmc.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/drivers/sdmmc/inc/fsl_specification.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/drivers/sdmmc/port/usdhc/polling/fsl_host.c"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/drivers/sdmmc/src/fsl_mmc.c"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/drivers/sdmmc/src/fsl_sd.c"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/drivers/sdmmc/src/fsl_sdmmc.c"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/drivers/usdhc/fsl_usdhc.c"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/drivers/usdhc/fsl_usdhc.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/drivers/lpspi/fsl_lpspi.c"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/drivers/lpspi/fsl_lpspi.h"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/drivers/serial_nor_eeprom/serial_nor_eeprom.c"
"${ProjDirPath}/../../../../../middleware/mcu-boot/src/drivers/serial_nor_eeprom/serial_nor_eeprom.h"
)


set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -T${ProjDirPath}/MIMXRT1064_ram_flashloader.ld -static")

TARGET_LINK_LIBRARIES(flashloader.elf -Wl,--start-group)
target_link_libraries(flashloader.elf optimized m)

target_link_libraries(flashloader.elf optimized c)

target_link_libraries(flashloader.elf optimized gcc)

target_link_libraries(flashloader.elf optimized nosys)

TARGET_LINK_LIBRARIES(flashloader.elf -Wl,--end-group)

ADD_CUSTOM_COMMAND(TARGET flashloader.elf POST_BUILD COMMAND ${CMAKE_OBJCOPY}
-Osrec ${EXECUTABLE_OUTPUT_PATH}/flashloader.elf ${EXECUTABLE_OUTPUT_PATH}/flashloader.srec)

