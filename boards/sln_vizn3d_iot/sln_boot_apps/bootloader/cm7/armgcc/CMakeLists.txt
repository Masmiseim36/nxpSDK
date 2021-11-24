# CROSS COMPILER SETTING
SET(CMAKE_SYSTEM_NAME Generic)
CMAKE_MINIMUM_REQUIRED (VERSION 3.1.1)

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

SET(EXECUTABLE_OUTPUT_PATH ${ProjDirPath}/${CMAKE_BUILD_TYPE})
SET(LIBRARY_OUTPUT_PATH ${ProjDirPath}/${CMAKE_BUILD_TYPE})


project(bootloader)

set(MCUX_SDK_PROJECT_NAME bootloader.elf)

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${ProjDirPath}/../board_define.h"
"${ProjDirPath}/../../../../sdmmc_config.c"
"${ProjDirPath}/../../../../sdmmc_config.h"
"${ProjDirPath}/../main.c"
"${ProjDirPath}/../bootloader.c"
"${ProjDirPath}/../bootloader.h"
"${ProjDirPath}/../sln_update.h"
"${ProjDirPath}/../sln_app_specific.h"
"${ProjDirPath}/../fusemap.h"
"${ProjDirPath}/../os_hooks.c"
"${ProjDirPath}/../usb_device_config.h"
"${ProjDirPath}/../usb_device_descriptor.c"
"${ProjDirPath}/../usb_device_descriptor.h"
"${ProjDirPath}/../usb_device_board_config.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT117H/utilities/str/fsl_str.c"
"${ProjDirPath}/../../../../../../devices/MIMXRT117H/utilities/str/fsl_str.h"
"${ProjDirPath}/../../../../../../devices/MIMXRT117H/utilities/fsl_debug_console_conf.h"
"${ProjDirPath}/../config_files/FreeRTOSConfigBoard.h"
"${ProjDirPath}/../config_files/FreeRTOSConfig.h"
"${ProjDirPath}/../config_files/FreeRTOSIPConfig.h"
"${ProjDirPath}/../config_files/bootloader_mbedtls_config.h"
"${ProjDirPath}/../source/iot_logging_task_dynamic_buffers.c"
"${ProjDirPath}/../source/iot_logging_task.h"
"${ProjDirPath}/../source/iot_appversion32.h"
"${ProjDirPath}/../../sln_update_msd.c"
"${ProjDirPath}/../../sln_smart_lock_code.c"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
"${ProjDirPath}/../dcd.c"
"${ProjDirPath}/../dcd.h"
"${ProjDirPath}/../usb_device_msc.c"
"${ProjDirPath}/../usb_device_msc.h"
"${ProjDirPath}/../usb_device_msc_ufi.c"
"${ProjDirPath}/../usb_device_msc_ufi.h"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${ProjDirPath}/../config_files release
    ${ProjDirPath}/../../../../common
    ${ProjDirPath}/..
    ${ProjDirPath}/../../../../../../devices/MIMXRT117H/utilities
    ${ProjDirPath}/../../../../../../devices/MIMXRT117H/utilities/str
    ${ProjDirPath}/../config_files
    ${ProjDirPath}/../source
)

if(CMAKE_BUILD_TYPE STREQUAL release)
     target_compile_definitions(${MCUX_SDK_PROJECT_NAME}  PRIVATE MBEDTLS_CONFIG_FILE="bootloader_mbedtls_config.h" AWS_LOGGING_INCLUDE="iot_logging_task.h" AWS_SYS_INIT_INCLUDE="iot_system_init.h" AWS_VERSION_INCLUDE="iot_appversion32.h")
endif(CMAKE_BUILD_TYPE STREQUAL release)

if(CMAKE_BUILD_TYPE STREQUAL debug)
     target_compile_definitions(${MCUX_SDK_PROJECT_NAME}  PRIVATE MBEDTLS_CONFIG_FILE="bootloader_mbedtls_config.h" AWS_LOGGING_INCLUDE="iot_logging_task.h" AWS_SYS_INIT_INCLUDE="iot_system_init.h" AWS_VERSION_INCLUDE="iot_appversion32.h")
endif(CMAKE_BUILD_TYPE STREQUAL debug)

set(CMAKE_MODULE_PATH
    ${ProjDirPath}/../../../../../../middleware/sln_iot_common_platform/flash_mgmt
    ${ProjDirPath}/../../../../../../middleware/sln_iot_common_platform/custom_config/flash/w25q256jvs
    ${ProjDirPath}/../../../../../../devices/MIMXRT117H/drivers
    ${ProjDirPath}/../../../../../../devices/MIMXRT117H/utilities
    ${ProjDirPath}/../../../../../../middleware/sdmmc
    ${ProjDirPath}/../../../../../../middleware/usb
    ${ProjDirPath}/../../../../../../components/osa
    ${ProjDirPath}/../../../../../../middleware/mbedtls
    ${ProjDirPath}/../../../../../../rtos/freertos
    ${ProjDirPath}/../../../../../../middleware/sln_iot_common_platform/utils
    ${ProjDirPath}/../../../../../../middleware/sln_iot_common_platform/boot_code/common
    ${ProjDirPath}/../../../../../../middleware/sln_iot_common_platform/drivers
    ${ProjDirPath}/../../../../../../middleware/sln_iot_common_platform/msc_disk_vfs
    ${ProjDirPath}/../../../../../../devices/MIMXRT117H
    ${ProjDirPath}/../../../../../../CMSIS/Include
    ${ProjDirPath}/../../../../../../components/uart
    ${ProjDirPath}/../../../../../../components/serial_manager
    ${ProjDirPath}/../../../../../../components/lists
    ${ProjDirPath}/../../../../../../devices/MIMXRT117H/xip
    ${ProjDirPath}/../../../../xip
    ${ProjDirPath}/../../../../../../devices/MIMXRT117H/drivers/cm7
)

# include modules
include(middleware_sln_iot_common_platform_sln_flash_MIMXRT117H)

include(middleware_sln_iot_common_platform_flexspi_nor_flash_config_w25q256jvs_MIMXRT117H)

include(driver_common_MIMXRT117H)

include(driver_flexspi_MIMXRT117H)

include(driver_ocotp_MIMXRT117H)

include(driver_flexram_MIMXRT117H)

include(driver_soc_flexram_allocate_MIMXRT117H)

include(driver_lpi2c_freertos_MIMXRT117H)

include(utility_assert_MIMXRT117H)

include(middleware_sdmmc_host_usdhc_MIMXRT117H)

include(middleware_sdmmc_host_usdhc_freertos_MIMXRT117H)

include(middleware_usb_device_controller_driver_MIMXRT117H)

include(component_osa_MIMXRT117H)

include(middleware_usb_device_ehci_MIMXRT117H)

include(middleware_usb_device_stack_external_MIMXRT117H)

include(middleware_mbedtls_MIMXRT117H)

include(utility_debug_console_MIMXRT117H)

include(middleware_mbedtls_port_ksdk_MIMXRT117H)

include(middleware_mbedtls_rt2_MIMXRT117H)

include(driver_clock_MIMXRT117H)

include(driver_gpt_MIMXRT117H)

include(driver_lpi2c_MIMXRT117H)

include(driver_pit_MIMXRT117H)

include(driver_pwm_MIMXRT117H)

include(driver_usdhc_MIMXRT117H)

include(driver_xbara_MIMXRT117H)

include(middleware_freertos-kernel_MIMXRT117H)

include(middleware_freertos-kernel_heap_4_MIMXRT117H)

include(middleware_sln_iot_common_platform_device_utils_MIMXRT117H)

include(middleware_sln_iot_common_platform_fica_definition_MIMXRT117H)

include(middleware_sln_iot_common_platform_fica_driver_MIMXRT117H)

include(middleware_sln_iot_common_platform_sln_rgb_led_MIMXRT117H)

include(middleware_sln_iot_common_platform_sln_push_buttons_MIMXRT117H)

include(middleware_sln_iot_common_platform_msc_disk_vfs_MIMXRT117H)

include(device_MIMXRT117H_CMSIS_MIMXRT117H)

include(CMSIS_Include_core_cm7_MIMXRT117H)

include(component_lpuart_adapter_MIMXRT117H)

include(component_serial_manager_MIMXRT117H)

include(component_lists_MIMXRT117H)

include(component_serial_manager_uart_MIMXRT117H)

include(driver_lpuart_MIMXRT117H)

include(device_MIMXRT117H_startup_MIMXRT117H)

include(driver_iomuxc_MIMXRT117H)

include(driver_igpio_MIMXRT117H)

include(driver_xip_device_MIMXRT117H)

include(driver_xip_board_MIMXRT117H)

include(driver_pmu_1_MIMXRT117H)

include(driver_dcdc_soc_MIMXRT117H)

include(driver_cache_armv7_m7_MIMXRT117H)

include(driver_anatop_ai_MIMXRT117H)

include(middleware_freertos-kernel_extension_MIMXRT117H)

include(middleware_sdmmc_common_MIMXRT117H)

include(middleware_sdmmc_osa_freertos_MIMXRT117H)

include(component_osa_free_rtos_MIMXRT117H)

include(middleware_usb_phy_MIMXRT117H)

include(middleware_usb_common_header_MIMXRT117H)

include(middleware_usb_device_common_header_MIMXRT117H)

include(driver_caam_MIMXRT117H)

include(utilities_misc_utilities_MIMXRT117H)

include(CMSIS_Include_common_MIMXRT117H)

include(CMSIS_Include_dsp_MIMXRT117H)

include(device_MIMXRT117H_system_MIMXRT117H)


TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE m)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE c)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE gcc)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE nosys)

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)


