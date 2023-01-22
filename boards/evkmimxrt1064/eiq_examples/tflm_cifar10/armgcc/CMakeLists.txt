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


project(tflm_cifar10)

set(MCUX_BUILD_TYPES flexspi_nor_sdram_release flexspi_nor_sdram_debug)

set(MCUX_SDK_PROJECT_NAME tflm_cifar10.elf)

if (NOT DEFINED SdkRootDirPath)
    SET(SdkRootDirPath ${ProjDirPath}/../../../../..)
endif()

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${SdkRootDirPath}/middleware/eiq/common/image/image_conv.c"
"${SdkRootDirPath}/middleware/eiq/common/demo_info.h"
"${SdkRootDirPath}/middleware/eiq/common/image/image.h"
"${SdkRootDirPath}/middleware/eiq/common/image/image_decode_raw.c"
"${SdkRootDirPath}/middleware/eiq/common/image/image_utils.h"
"${SdkRootDirPath}/middleware/eiq/common/model/get_top_n.cpp"
"${SdkRootDirPath}/middleware/eiq/common/model/get_top_n.h"
"${SdkRootDirPath}/middleware/eiq/common/model/model.h"
"${SdkRootDirPath}/middleware/eiq/common/model/output_postproc.h"
"${SdkRootDirPath}/middleware/eiq/tensorflow-lite/examples/common/model/output_postproc.cpp"
"${SdkRootDirPath}/middleware/eiq/tensorflow-lite/examples/cifar10/demo_config.h"
"${SdkRootDirPath}/middleware/eiq/tensorflow-lite/examples/common/demo_info.cpp"
"${SdkRootDirPath}/middleware/eiq/tensorflow-lite/examples/cifar10/image_data.h"
"${SdkRootDirPath}/middleware/eiq/tensorflow-lite/examples/cifar10/labels.h"
"${SdkRootDirPath}/middleware/eiq/tensorflow-lite/examples/model.cpp"
"${SdkRootDirPath}/middleware/eiq/tensorflow-lite/examples/image/main.cpp"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${SdkRootDirPath}/middleware/eiq/tensorflow-lite/examples/cifar10/model_cifarnet_ops_micro.cpp"
"${SdkRootDirPath}/middleware/eiq/tensorflow-lite/examples/cifar10/pcq/model_data.h"
"${SdkRootDirPath}/middleware/eiq/common/image/image_capture.c"
"${SdkRootDirPath}/middleware/eiq/common/image/image_draw.c"
"${SdkRootDirPath}/middleware/eiq/common/image/image_resize_nearest.c"
"${SdkRootDirPath}/middleware/eiq/common/timer.h"
"${SdkRootDirPath}/middleware/eiq/common/timer.c"
"${ProjDirPath}/../board_init.c"
"${ProjDirPath}/../board_init.h"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
"${ProjDirPath}/../dcd.c"
"${ProjDirPath}/../dcd.h"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${SdkRootDirPath}/middleware/eiq/tensorflow-lite/examples/cifar10
    ${SdkRootDirPath}/middleware/eiq/common
    ${SdkRootDirPath}/middleware/eiq/common/image
    ${SdkRootDirPath}/middleware/eiq/common/model
    ${ProjDirPath}/..
    ${SdkRootDirPath}/middleware/eiq/tensorflow-lite/examples/cifar10/pcq
)

set(CMAKE_MODULE_PATH
    ${SdkRootDirPath}/components/video/camera/device
    ${SdkRootDirPath}/components/video/camera/receiver
    ${SdkRootDirPath}/components/video/camera/device/sccb
    ${SdkRootDirPath}/components/video/camera/device/ov7725
    ${SdkRootDirPath}/components/video/camera/device/mt9m114
    ${SdkRootDirPath}/components/video/camera
    ${SdkRootDirPath}/components/video/camera/receiver/csi
    ${SdkRootDirPath}/devices/MIMXRT1064/drivers
    ${SdkRootDirPath}/components/video
    ${SdkRootDirPath}/components/video/i2c
    ${SdkRootDirPath}/components/video/display/dc
    ${SdkRootDirPath}/components/video/display/dc/elcdif
    ${SdkRootDirPath}/devices/MIMXRT1064/utilities
    ${SdkRootDirPath}/devices/MIMXRT1064/utilities/debug_console_lite
    ${SdkRootDirPath}/middleware/eiq
    ${SdkRootDirPath}/middleware/eiq/tensorflow-lite
    ${SdkRootDirPath}/components/lists
    ${SdkRootDirPath}/components/uart
    ${SdkRootDirPath}/devices/MIMXRT1064
    ${SdkRootDirPath}/devices/MIMXRT1064/xip
    ${SdkRootDirPath}/boards/evkmimxrt1064/xip
    ${SdkRootDirPath}/CMSIS/Core/Include
    ${SdkRootDirPath}/middleware/eiq/tensorflow-lite/third_party/cmsis
)

# include modules
include(driver_camera-device-common_MIMXRT1064)

include(driver_camera-receiver-common_MIMXRT1064)

include(driver_camera-device-sccb_MIMXRT1064)

include(driver_camera-device-ov7725_MIMXRT1064)

include(driver_camera-device-mt9m114_MIMXRT1064)

include(driver_camera-common_MIMXRT1064)

include(driver_camera-receiver-csi_MIMXRT1064)

include(driver_csi_MIMXRT1064)

include(driver_video-common_MIMXRT1064)

include(driver_video-i2c_MIMXRT1064)

include(driver_elcdif_MIMXRT1064)

include(driver_pxp_MIMXRT1064)

include(driver_dc-fb-common_MIMXRT1064)

include(driver_dc-fb-elcdif_MIMXRT1064)

include(driver_igpio_MIMXRT1064)

include(utility_debug_console_lite_MIMXRT1064)

include(utility_assert_lite_MIMXRT1064)

include(middleware_eiq_worker_video_MIMXRT1064)

include(middleware_eiq_gui_printf_MIMXRT1064)

include(middleware_eiq_tensorflow_lite_micro_MIMXRT1064)

include(component_lists_MIMXRT1064)

include(component_lpuart_adapter_MIMXRT1064)

include(driver_common_MIMXRT1064)

include(driver_lpuart_MIMXRT1064)

include(device_MIMXRT1064_CMSIS_MIMXRT1064)

include(device_MIMXRT1064_startup_MIMXRT1064)

include(driver_clock_MIMXRT1064)

include(driver_iomuxc_MIMXRT1064)

include(driver_lpi2c_MIMXRT1064)

include(driver_xip_device_MIMXRT1064)

include(driver_xip_board_evkmimxrt1064_MIMXRT1064)

include(middleware_eiq_tensorflow_lite_micro_cmsis_nn_MIMXRT1064)

include(CMSIS_Include_core_cm_MIMXRT1064)

include(middleware_eiq_worker_MIMXRT1064)

include(middleware_eiq_tensorflow_lite_micro_third_party_flatbuffers_MIMXRT1064)

include(middleware_eiq_tensorflow_lite_micro_third_party_gemmlowp_MIMXRT1064)

include(middleware_eiq_tensorflow_lite_micro_third_party_ruy_MIMXRT1064)

include(utilities_misc_utilities_MIMXRT1064)

include(device_MIMXRT1064_system_MIMXRT1064)

include(middleware_eiq_tensorflow_lite_micro_third_party_cmsis_nn_MIMXRT1064)


IF(NOT DEFINED TARGET_LINK_SYSTEM_LIBRARIES)  
    SET(TARGET_LINK_SYSTEM_LIBRARIES "-lm -lc -lgcc -lnosys")  
ENDIF()  

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${TARGET_LINK_SYSTEM_LIBRARIES})

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)


