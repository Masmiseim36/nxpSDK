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


project(smart_lock)

set(MCUX_SDK_PROJECT_NAME smart_lock.elf)

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${ProjDirPath}/../../hardware_init.c"
"${ProjDirPath}/../smart_lock_input_push_buttons.c"
"${ProjDirPath}/../smart_lock_input_shell_commands.c"
"${ProjDirPath}/../smart_lock_mqs.c"
"${ProjDirPath}/../data.ldt"
"${ProjDirPath}/../main_text.ldt"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../main.cpp"
"${ProjDirPath}/../os_hooks.c"
"${ProjDirPath}/../virtual_camera.h"
"${ProjDirPath}/../virtual_com.h"
"${ProjDirPath}/../virtual_com.c"
"${ProjDirPath}/../composite.h"
"${ProjDirPath}/../composite.c"
"${ProjDirPath}/../../../../../../components/serial_manager/fsl_component_serial_port_usb.h"
"${ProjDirPath}/../config_files/board_define.h"
"${ProjDirPath}/../config_files/board_hal_registration.c"
"${ProjDirPath}/../config_files/cpp_config.cpp"
"${ProjDirPath}/../config_files/FreeRTOSConfig.h"
"${ProjDirPath}/../config_files/usb_device_config.h"
"${ProjDirPath}/../usb_device_descriptor.c"
"${ProjDirPath}/../usb_device_descriptor.h"
"${ProjDirPath}/../dcd.c"
"${ProjDirPath}/../dcd.h"
"${ProjDirPath}/../display_support.h"
"${ProjDirPath}/../display_support.c"
"${ProjDirPath}/../sln_time.h"
"${ProjDirPath}/../sln_time.c"
"${ProjDirPath}/../ble_connected.h"
"${ProjDirPath}/../ble_connected_cn.h"
"${ProjDirPath}/../ble_disconnected.h"
"${ProjDirPath}/../ble_disconnected_cn.h"
"${ProjDirPath}/../can_I_help.h"
"${ProjDirPath}/../deregistration_failed.h"
"${ProjDirPath}/../deregistration_failed_cn.h"
"${ProjDirPath}/../deregistration_successful.h"
"${ProjDirPath}/../deregistration_successful_cn.h"
"${ProjDirPath}/../enter_sleep.h"
"${ProjDirPath}/../enter_sleep_cn.h"
"${ProjDirPath}/../fake_face.h"
"${ProjDirPath}/../fake_face_cn.h"
"${ProjDirPath}/../look_at_the_camera.h"
"${ProjDirPath}/../look_at_the_camera_cn.h"
"${ProjDirPath}/../move_face_closer.h"
"${ProjDirPath}/../move_face_closer_cn.h"
"${ProjDirPath}/../ok.h"
"${ProjDirPath}/../ok_cn.h"
"${ProjDirPath}/../recognition_failed.h"
"${ProjDirPath}/../recognition_failed_cn.h"
"${ProjDirPath}/../recognition_successful.h"
"${ProjDirPath}/../recognition_successful_cn.h"
"${ProjDirPath}/../registration_failed.h"
"${ProjDirPath}/../registration_failed_cn.h"
"${ProjDirPath}/../registration_successful.h"
"${ProjDirPath}/../registration_successful_cn.h"
"${ProjDirPath}/../starting_deregistration.h"
"${ProjDirPath}/../starting_deregistration_cn.h"
"${ProjDirPath}/../starting_registration.h"
"${ProjDirPath}/../starting_registration_cn.h"
"${ProjDirPath}/../tone_timeout.h"
"${ProjDirPath}/../turn_face_to_left.h"
"${ProjDirPath}/../turn_face_to_left_cn.h"
"${ProjDirPath}/../turn_face_to_right.h"
"${ProjDirPath}/../turn_face_to_right_cn.h"
"${ProjDirPath}/../sln_device_utils.c"
"${ProjDirPath}/../sln_device_utils.h"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${ProjDirPath}/../..
    ${ProjDirPath}/..
    ${ProjDirPath}/../../../../common
    ${ProjDirPath}/../config_files
)

set(CMAKE_MODULE_PATH
    ${ProjDirPath}/../../../../../../devices/MIMXRT117H/utilities
    ${ProjDirPath}/../../../../../../devices/MIMXRT117H/drivers
    ${ProjDirPath}/../../../../../../components/i2c
    ${ProjDirPath}/../../../../../../components/common_task
    ${ProjDirPath}/../../../../../../components/video/display/mipi_dsi_cmd
    ${ProjDirPath}/../../../../../../components/video/display/rm68191
    ${ProjDirPath}/../../../../../../components/video/display/rm68200
    ${ProjDirPath}/../../../../../../components/video/display/dc/elcdif
    ${ProjDirPath}/../../../../../../components/video/display/dc/lcdifv2
    ${ProjDirPath}/../../../../../../components/video/camera/device/ov5640
    ${ProjDirPath}/../../../../../../components/video/i2c
    ${ProjDirPath}/../../../../../../components/video/camera/receiver/csi
    ${ProjDirPath}/../../../../../../middleware/usb
    ${ProjDirPath}/../../../../../../components/osa
    ${ProjDirPath}/../../../../../../middleware/sln_iot_common_platform/framework/sln_framework/build/release/rt117f
    ${ProjDirPath}/../../../../../../middleware/sln_iot_common_platform/framework/sln_hal_common
    ${ProjDirPath}/../../../../../../middleware/sln_iot_common_platform/framework/sln_hal_face_rec
    ${ProjDirPath}/../../../../../../middleware/sln_iot_common_platform/drivers
    ${ProjDirPath}/../../../../../../rtos/freertos
    ${ProjDirPath}/../../../../../../middleware/sln_iot_common_platform/boot_code/common
    ${ProjDirPath}/../../../../../../middleware/littlefs
    ${ProjDirPath}/../../../../../../middleware/sln_iot_common_platform/ui_resource
    ${ProjDirPath}/../../../../../../middleware/sln_iot_common_platform/ui_resource/icons
    ${ProjDirPath}/../../../../../../middleware/orbbec
    ${ProjDirPath}/../../../../../../middleware/oasis/oasis_3d
    ${ProjDirPath}/../../../../../../middleware/oasis/oasis_2d
    ${ProjDirPath}/../../../../../../middleware/sln_iot_common_platform/flash_mgmt
    ${ProjDirPath}/../../../../../../middleware/sln_iot_common_platform/custom_config/flash/w25q256jvs
    ${ProjDirPath}/../../../../../../devices/MIMXRT117H
    ${ProjDirPath}/../../../../../../CMSIS/Include
    ${ProjDirPath}/../../../../../../components/uart
    ${ProjDirPath}/../../../../../../components/serial_manager
    ${ProjDirPath}/../../../../../../components/lists
    ${ProjDirPath}/../../../../../../devices/MIMXRT117H/xip
    ${ProjDirPath}/../../../../xip
    ${ProjDirPath}/../../../../../../devices/MIMXRT117H/drivers/cm7
    ${ProjDirPath}/../../../../../../components/video/display
    ${ProjDirPath}/../../../../../../components/video
    ${ProjDirPath}/../../../../../../components/video/display/dc
    ${ProjDirPath}/../../../../../../components/video/camera
    ${ProjDirPath}/../../../../../../components/video/camera/device
    ${ProjDirPath}/../../../../../../components/video/camera/device/sccb
    ${ProjDirPath}/../../../../../../components/video/camera/receiver
)

# include modules
include(utility_shell_MIMXRT117H)

include(driver_pit_MIMXRT117H)

include(driver_csi_MIMXRT117H)

include(driver_dmamux_MIMXRT117H)

include(driver_edma_MIMXRT117H)

include(driver_elcdif_MIMXRT117H)

include(driver_flexio_MIMXRT117H)

include(component_lpi2c_adapter_MIMXRT117H)

include(component_common_task_MIMXRT117H)

include(driver_flexram_MIMXRT117H)

include(driver_flexio_camera_MIMXRT117H)

include(driver_flexio_camera_edma_MIMXRT117H)

include(driver_flexspi_MIMXRT117H)

include(driver_lcdifv2_MIMXRT117H)

include(driver_lpspi_MIMXRT117H)

include(driver_soc_flexram_allocate_MIMXRT117H)

include(driver_lpi2c_freertos_MIMXRT117H)

include(driver_lpspi_freertos_MIMXRT117H)

include(driver_gpc_3_MIMXRT117H)

include(driver_pwm_MIMXRT117H)

include(driver_pxp_MIMXRT117H)

include(driver_soc_src_MIMXRT117H)

include(driver_xbara_MIMXRT117H)

include(driver_lpuart_freertos_MIMXRT117H)

include(driver_sai_edma_MIMXRT117H)

include(driver_display-mipi-dsi-cmd_MIMXRT117H)

include(driver_common_MIMXRT117H)

include(driver_mipi_dsi_split_MIMXRT117H)

include(driver_display-rm68191_MIMXRT117H)

include(driver_display-rm68200_MIMXRT117H)

include(driver_dc-fb-elcdif_MIMXRT117H)

include(driver_dc-fb-lcdifv2_MIMXRT117H)

include(driver_camera-device-ov5640_MIMXRT117H)

include(driver_video-i2c_MIMXRT117H)

include(driver_camera-receiver-csi_MIMXRT117H)

include(driver_mipi_csi2rx_MIMXRT117H)

include(middleware_usb_phy_MIMXRT117H)

include(middleware_usb_device_controller_driver_MIMXRT117H)

include(component_osa_MIMXRT117H)

include(middleware_usb_device_ehci_MIMXRT117H)

include(middleware_usb_device_video_external_MIMXRT117H)

include(middleware_usb_device_cdc_external_MIMXRT117H)

include(middleware_sln_iot_common_platform_sln_framework_release_MIMXRT117H)

include(middleware_sln_iot_common_platform_sln_hal_common_MIMXRT117H)

include(middleware_sln_iot_common_platform_sln_hal_face_rec_MIMXRT117H)

include(middleware_sln_iot_common_platform_sln_gc0308_MIMXRT117H)

include(middleware_sln_iot_common_platform_sln_mt9m114_MIMXRT117H)

include(middleware_sln_iot_common_platform_sln_ili9341_MIMXRT117H)

include(middleware_sln_iot_common_platform_sln_orbbecU1S_MIMXRT117H)

include(middleware_sln_iot_common_platform_sln_tc358748xbg_MIMXRT117H)

include(middleware_freertos-kernel_MIMXRT117H)

include(middleware_freertos-kernel_heap_4_MIMXRT117H)

include(middleware_sln_iot_common_platform_fica_definition_MIMXRT117H)

include(middleware_sln_iot_common_platform_sln_camera_adapter_MIMXRT117H)

include(middleware_littlefs_MIMXRT117H)

include(middleware_sln_iot_common_platform_fonts_MIMXRT117H)

include(middleware_sln_iot_common_platform_icons_MIMXRT117H)

include(middleware_orbbec_MIMXRT117H)

include(middleware_oasis_3d_MIMXRT117H)

include(middleware_oasis_2d_MIMXRT117H)

include(middleware_sln_iot_common_platform_sln_flash_MIMXRT117H)

include(middleware_sln_iot_common_platform_flexspi_nor_flash_config_w25q256jvs_MIMXRT117H)

include(driver_clock_MIMXRT117H)

include(device_MIMXRT117H_CMSIS_MIMXRT117H)

include(CMSIS_Include_core_cm7_MIMXRT117H)

include(utility_debug_console_MIMXRT117H)

include(component_lpuart_adapter_MIMXRT117H)

include(component_serial_manager_MIMXRT117H)

include(component_lists_MIMXRT117H)

include(component_serial_manager_uart_MIMXRT117H)

include(driver_lpuart_MIMXRT117H)

include(device_MIMXRT117H_startup_MIMXRT117H)

include(driver_iomuxc_MIMXRT117H)

include(utility_assert_MIMXRT117H)

include(driver_igpio_MIMXRT117H)

include(driver_xip_device_MIMXRT117H)

include(driver_xip_board_MIMXRT117H)

include(driver_pmu_1_MIMXRT117H)

include(driver_dcdc_soc_MIMXRT117H)

include(driver_cache_armv7_m7_MIMXRT117H)

include(driver_anatop_ai_MIMXRT117H)

include(driver_lpi2c_MIMXRT117H)

include(middleware_freertos-kernel_extension_MIMXRT117H)

include(driver_sai_MIMXRT117H)

include(driver_soc_mipi_csi2rx_MIMXRT117H)

include(driver_display-common_MIMXRT117H)

include(driver_video-common_MIMXRT117H)

include(driver_dc-fb-common_MIMXRT117H)

include(driver_camera-common_MIMXRT117H)

include(driver_camera-device-common_MIMXRT117H)

include(driver_camera-device-sccb_MIMXRT117H)

include(driver_camera-receiver-common_MIMXRT117H)

include(middleware_usb_common_header_MIMXRT117H)

include(middleware_usb_device_common_header_MIMXRT117H)

include(middleware_usb_device_stack_external_MIMXRT117H)

include(utilities_misc_utilities_MIMXRT117H)

include(CMSIS_Include_common_MIMXRT117H)

include(CMSIS_Include_dsp_MIMXRT117H)

include(device_MIMXRT117H_system_MIMXRT117H)


TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE m)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE c)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE gcc)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE nosys)

if(CMAKE_BUILD_TYPE STREQUAL debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../../middleware/sln_iot_common_platform/framework/sln_framework/build/release/rt117f/libsln_framework.a)
endif()

if(CMAKE_BUILD_TYPE STREQUAL debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../../middleware/orbbec/libodepth.a)
endif()

if(CMAKE_BUILD_TYPE STREQUAL debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../../middleware/oasis/oasis_3d/liboasis_lite3D_DEFAULT_117f_ae.a)
endif()

if(CMAKE_BUILD_TYPE STREQUAL debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../../middleware/oasis/oasis_2d/liboasis_lite2D_DEFAULT_117f_ae.a)
endif()

if(CMAKE_BUILD_TYPE STREQUAL release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../../middleware/sln_iot_common_platform/framework/sln_framework/build/release/rt117f/libsln_framework.a)
endif()

if(CMAKE_BUILD_TYPE STREQUAL release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../../middleware/orbbec/libodepth.a)
endif()

if(CMAKE_BUILD_TYPE STREQUAL release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../../middleware/oasis/oasis_3d/liboasis_lite3D_DEFAULT_117f_ae.a)
endif()

if(CMAKE_BUILD_TYPE STREQUAL release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../../middleware/oasis/oasis_2d/liboasis_lite2D_DEFAULT_117f_ae.a)
endif()

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)


