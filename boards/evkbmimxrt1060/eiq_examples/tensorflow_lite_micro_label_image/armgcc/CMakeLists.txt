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


project(tensorflow_lite_micro_label_image)

set(MCUX_SDK_PROJECT_NAME tensorflow_lite_micro_label_image.elf)

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${ProjDirPath}/../../../../../middleware/eiq/common/image/image_conv.c"
"${ProjDirPath}/../../../../../middleware/eiq/common/demo_info.h"
"${ProjDirPath}/../../../../../middleware/eiq/common/image/image.h"
"${ProjDirPath}/../../../../../middleware/eiq/common/image/image_decode_raw.c"
"${ProjDirPath}/../../../../../middleware/eiq/common/image/image_utils.h"
"${ProjDirPath}/../../../../../middleware/eiq/common/model/get_top_n.cpp"
"${ProjDirPath}/../../../../../middleware/eiq/common/model/get_top_n.h"
"${ProjDirPath}/../../../../../middleware/eiq/common/model/model.h"
"${ProjDirPath}/../../../../../middleware/eiq/common/model/output_postproc.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/examples/common/model/output_postproc.cpp"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/examples/label_image/demo_config.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/examples/common/demo_info.cpp"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/examples/label_image/image_data.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/examples/label_image/labels.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/examples/model_all_ops_micro.cpp"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/examples/model.cpp"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/examples/image/main.cpp"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/examples/label_image/pcq/model_data.h"
"${ProjDirPath}/../../../../../middleware/eiq/common/image/image_capture.c"
"${ProjDirPath}/../../../../../middleware/eiq/common/image/image_draw.c"
"${ProjDirPath}/../../../../../middleware/eiq/common/image/image_resize_nearest.c"
"${ProjDirPath}/../../../../../middleware/eiq/common/timer.h"
"${ProjDirPath}/../../../../../middleware/eiq/common/timer.c"
"${ProjDirPath}/../board_init.c"
"${ProjDirPath}/../board_init.h"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
"${ProjDirPath}/../dcd.c"
"${ProjDirPath}/../dcd.h"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/examples/label_image
    ${ProjDirPath}/../../../../../middleware/eiq/common
    ${ProjDirPath}/../../../../../middleware/eiq/common/image
    ${ProjDirPath}/../../../../../middleware/eiq/common/model
    ${ProjDirPath}/..
    ${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/examples/label_image/pcq
)

set(CMAKE_MODULE_PATH
    ${ProjDirPath}/../../../../../components/video/camera/device
    ${ProjDirPath}/../../../../../components/video/camera/receiver
    ${ProjDirPath}/../../../../../components/video/camera/device/sccb
    ${ProjDirPath}/../../../../../components/video/camera/device/ov7725
    ${ProjDirPath}/../../../../../components/video/camera/device/mt9m114
    ${ProjDirPath}/../../../../../components/video/camera
    ${ProjDirPath}/../../../../../components/video/camera/receiver/csi
    ${ProjDirPath}/../../../../../devices/MIMXRT1062/drivers
    ${ProjDirPath}/../../../../../components/video
    ${ProjDirPath}/../../../../../components/video/i2c
    ${ProjDirPath}/../../../../../components/video/display/dc
    ${ProjDirPath}/../../../../../components/video/display/dc/elcdif
    ${ProjDirPath}/../../../../../devices/MIMXRT1062/utilities/debug_console_lite
    ${ProjDirPath}/../../../../../middleware/eiq
    ${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite
    ${ProjDirPath}/../../../../../components/lists
    ${ProjDirPath}/../../../../../components/uart
    ${ProjDirPath}/../../../../../devices/MIMXRT1062
    ${ProjDirPath}/../../../../../devices/MIMXRT1062/xip
    ${ProjDirPath}/../../../xip
    ${ProjDirPath}/../../../../../CMSIS/Core/Include
    ${ProjDirPath}/../../../../../devices/MIMXRT1062/utilities
    ${ProjDirPath}/../../../../../middleware/eiq/tensorflow-lite/third_party/cmsis
)

# include modules
include(driver_camera-device-common_MIMXRT1062)

include(driver_camera-receiver-common_MIMXRT1062)

include(driver_camera-device-sccb_MIMXRT1062)

include(driver_camera-device-ov7725_MIMXRT1062)

include(driver_camera-device-mt9m114_MIMXRT1062)

include(driver_camera-common_MIMXRT1062)

include(driver_camera-receiver-csi_MIMXRT1062)

include(driver_csi_MIMXRT1062)

include(driver_video-common_MIMXRT1062)

include(driver_video-i2c_MIMXRT1062)

include(driver_elcdif_MIMXRT1062)

include(driver_pxp_MIMXRT1062)

include(driver_dc-fb-common_MIMXRT1062)

include(driver_dc-fb-elcdif_MIMXRT1062)

include(driver_igpio_MIMXRT1062)

include(utility_debug_console_lite_MIMXRT1062)

include(utility_assert_lite_MIMXRT1062)

include(middleware_eiq_worker_video_MIMXRT1062)

include(middleware_eiq_gui_printf_MIMXRT1062)

include(middleware_eiq_tensorflow_lite_micro_MIMXRT1062)

include(component_lists_MIMXRT1062)

include(component_lpuart_adapter_MIMXRT1062)

include(driver_common_MIMXRT1062)

include(driver_lpuart_MIMXRT1062)

include(device_MIMXRT1062_CMSIS_MIMXRT1062)

include(device_MIMXRT1062_startup_MIMXRT1062)

include(driver_clock_MIMXRT1062)

include(driver_iomuxc_MIMXRT1062)

include(driver_lpi2c_MIMXRT1062)

include(driver_xip_device_MIMXRT1062)

include(driver_xip_board_evkbmimxrt1060_MIMXRT1062)

include(middleware_eiq_tensorflow_lite_micro_cmsis_nn_MIMXRT1062)

include(CMSIS_Include_core_cm_MIMXRT1062)

include(middleware_eiq_worker_MIMXRT1062)

include(middleware_eiq_tensorflow_lite_micro_third_party_flatbuffers_MIMXRT1062)

include(middleware_eiq_tensorflow_lite_micro_third_party_gemmlowp_MIMXRT1062)

include(middleware_eiq_tensorflow_lite_micro_third_party_ruy_MIMXRT1062)

include(utilities_misc_utilities_MIMXRT1062)

include(device_MIMXRT1062_system_MIMXRT1062)

include(middleware_eiq_tensorflow_lite_micro_third_party_cmsis_nn_MIMXRT1062)


TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE m)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE c)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE gcc)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE nosys)

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)


