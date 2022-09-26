# CROSS COMPILER SETTING
SET(CMAKE_SYSTEM_NAME Generic)
CMAKE_MINIMUM_REQUIRED (VERSION 3.10.0)

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


project(mpp_camera_mobilenet_view_glow_cm7)

set(MCUX_SDK_PROJECT_NAME mpp_camera_mobilenet_view_glow_cm7.elf)

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${ProjDirPath}/../../../../../middleware/eiq/mpp/examples/camera_mobilenet_view.c"
"${ProjDirPath}/../../../../../middleware/eiq/mpp/examples/stopwatch168_208_vuyx.h"
"${ProjDirPath}/../../../../../middleware/eiq/mpp/examples/examples_common.h"
"${ProjDirPath}/../../../../../middleware/eiq/mpp/examples/models/get_top_n.cpp"
"${ProjDirPath}/../../../../../middleware/eiq/mpp/examples/models/mobilenet_v1_0.25_128_quant_int8_cm7/mobilenet_v1_glow.h"
"${ProjDirPath}/../../../../../middleware/eiq/mpp/examples/models/mobilenet_v1_0.25_128_quant_int8_cm7/mobilenet_v1_glow.o"
"${ProjDirPath}/../../../../../middleware/eiq/mpp/examples/models/mobilenet_v1_0.25_128_quant_int8_cm7/mobilenetv1_labels.h"
"${ProjDirPath}/../../../../../middleware/eiq/mpp/examples/models/mobilenet_v1_0.25_128_quant_int8_cm7/mobilenet_v1_weights_glow.h"
"${ProjDirPath}/../../../../../middleware/eiq/mpp/examples/models/mobilenet_v1_0.25_128_quant_int8_cm7/mobilenetv1_output_postproc.cpp"
"${ProjDirPath}/../../../../../middleware/eiq/mpp/examples/models/mobilenet_v1_0.25_128_quant_int8_cm7/mobilenetv1_output_postproc.h"
"${ProjDirPath}/../../../../../middleware/eiq/mpp/examples/models/get_top_n.h"
"${ProjDirPath}/../../../../../middleware/eiq/mpp/examples/models/utils.h"
"${ProjDirPath}/../FreeRTOSConfig.h"
"${ProjDirPath}/../board_init.h"
"${ProjDirPath}/../board_init.c"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
"${ProjDirPath}/../../../../../middleware/eiq/mpp/README.md"
"${ProjDirPath}/../dcd.c"
"${ProjDirPath}/../dcd.h"
"${ProjDirPath}/../display_support.c"
"${ProjDirPath}/../display_support.h"
"${ProjDirPath}/../camera_support.c"
"${ProjDirPath}/../camera_support.h"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${ProjDirPath}/..
)

set(CMAKE_MODULE_PATH
    ${ProjDirPath}/../../../../../components/video/camera/device
    ${ProjDirPath}/../../../../../components/video/camera/receiver
    ${ProjDirPath}/../../../../../components/video/camera/device/sccb
    ${ProjDirPath}/../../../../../components/video/camera/device/ov5640
    ${ProjDirPath}/../../../../../components/video/display/rm68191
    ${ProjDirPath}/../../../../../components/video/display/rm68200
    ${ProjDirPath}/../../../../../components/video/display/hx8394
    ${ProjDirPath}/../../../../../components/video/camera
    ${ProjDirPath}/../../../../../components/video/camera/receiver/csi
    ${ProjDirPath}/../../../../../devices/MIMXRT1176/drivers
    ${ProjDirPath}/../../../../../components/video
    ${ProjDirPath}/../../../../../components/video/i2c
    ${ProjDirPath}/../../../../../components/video/display/dc
    ${ProjDirPath}/../../../../../components/video/display/dc/elcdif
    ${ProjDirPath}/../../../../../components/video/display/dc/lcdifv2
    ${ProjDirPath}/../../../../../components/video/display
    ${ProjDirPath}/../../../../../components/video/display/mipi_dsi_cmd
    ${ProjDirPath}/../../../../../devices/MIMXRT1176/utilities/debug_console_lite
    ${ProjDirPath}/../../../../../middleware/eiq
    ${ProjDirPath}/../../../../../components/lists
    ${ProjDirPath}/../../../../../components/uart
    ${ProjDirPath}/../../../../../devices/MIMXRT1176
    ${ProjDirPath}/../../../../../devices/MIMXRT1176/xip
    ${ProjDirPath}/../../../xip
    ${ProjDirPath}/../../../xmcd
    ${ProjDirPath}/../../../../../CMSIS/Core/Include
    ${ProjDirPath}/../../../../../rtos/freertos/freertos_kernel
    ${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite
    ${ProjDirPath}/../../../../../devices/MIMXRT1176/utilities
)

# include modules
include(driver_camera-device-common_MIMXRT1176_cm7)

include(driver_camera-receiver-common_MIMXRT1176_cm7)

include(driver_camera-device-sccb_MIMXRT1176_cm7)

include(driver_camera-device-ov5640_MIMXRT1176_cm7)

include(driver_display-rm68191_MIMXRT1176_cm7)

include(driver_display-rm68200_MIMXRT1176_cm7)

include(driver_display-hx8394_MIMXRT1176_cm7)

include(driver_camera-common_MIMXRT1176_cm7)

include(driver_camera-receiver-csi_MIMXRT1176_cm7)

include(driver_csi_MIMXRT1176_cm7)

include(driver_video-common_MIMXRT1176_cm7)

include(driver_video-i2c_MIMXRT1176_cm7)

include(driver_elcdif_MIMXRT1176_cm7)

include(driver_pxp_MIMXRT1176_cm7)

include(driver_dc-fb-common_MIMXRT1176_cm7)

include(driver_dc-fb-elcdif_MIMXRT1176_cm7)

include(driver_dc-fb-lcdifv2_MIMXRT1176_cm7)

include(driver_display-common_MIMXRT1176_cm7)

include(driver_display-mipi-dsi-cmd_MIMXRT1176_cm7)

include(driver_common_MIMXRT1176_cm7)

include(driver_mipi_dsi_split_MIMXRT1176_cm7)

include(driver_igpio_MIMXRT1176_cm7)

include(driver_mipi_csi2rx_MIMXRT1176_cm7)

include(driver_soc_mipi_csi2rx_MIMXRT1176_cm7)

include(driver_soc_src_MIMXRT1176_cm7)

include(utility_debug_console_lite_MIMXRT1176_cm7)

include(utility_assert_lite_MIMXRT1176_cm7)

include(middleware_eiq_mpp_MIMXRT1176_cm7)

include(component_lists_MIMXRT1176_cm7)

include(component_lpuart_adapter_MIMXRT1176_cm7)

include(driver_lpuart_MIMXRT1176_cm7)

include(device_MIMXRT1176_CMSIS_MIMXRT1176_cm7)

include(device_MIMXRT1176_startup_MIMXRT1176_cm7)

include(driver_dcdc_soc_MIMXRT1176_cm7)

include(driver_pmu_1_MIMXRT1176_cm7)

include(driver_clock_MIMXRT1176_cm7)

include(driver_iomuxc_MIMXRT1176_cm7)

include(driver_lpi2c_MIMXRT1176_cm7)

include(driver_xip_device_MIMXRT1176_cm7)

include(driver_xip_board_evkmimxrt1170_MIMXRT1176_cm7)

include(driver_xmcd_evkmimxrt1170_MIMXRT1176_cm7)

include(driver_anatop_ai_MIMXRT1176_cm7)

include(CMSIS_Include_core_cm_MIMXRT1176_cm7)

include(driver_memory_MIMXRT1176_cm7)

include(driver_lcdifv2_MIMXRT1176_cm7)

include(middleware_freertos-kernel_MIMXRT1176_cm7)

include(middleware_freertos-kernel_extension_MIMXRT1176_cm7)

include(middleware_freertos-kernel_heap_4_MIMXRT1176_cm7)

include(middleware_eiq_tensorflow_lite_micro_MIMXRT1176_cm7)

include(middleware_eiq_tensorflow_lite_micro_third_party_flatbuffers_MIMXRT1176_cm7)

include(middleware_eiq_tensorflow_lite_micro_third_party_gemmlowp_MIMXRT1176_cm7)

include(middleware_eiq_tensorflow_lite_micro_third_party_ruy_MIMXRT1176_cm7)

include(utilities_misc_utilities_MIMXRT1176_cm7)

include(device_MIMXRT1176_system_MIMXRT1176_cm7)


TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE m)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE c)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE gcc)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE nosys)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_sdram_debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../middleware/eiq/mpp/lib/libmpp.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_sdram_debug)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_sdram_release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../middleware/eiq/mpp/lib/libmpp.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_sdram_release)

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)


