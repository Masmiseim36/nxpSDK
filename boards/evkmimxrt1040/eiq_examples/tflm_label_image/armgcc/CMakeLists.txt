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


project(tflm_label_image)

set(MCUX_BUILD_TYPES flexspi_nor_sdram_release flexspi_nor_sdram_debug)

set(MCUX_SDK_PROJECT_NAME tflm_label_image.elf)

if (NOT DEFINED SdkRootDirPath)
    SET(SdkRootDirPath ${ProjDirPath}/../../../../..)
endif()

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${SdkRootDirPath}/middleware/eiq/common/demo_info.h"
"${SdkRootDirPath}/middleware/eiq/common/image/image.h"
"${SdkRootDirPath}/middleware/eiq/common/image/image_decode_raw.c"
"${SdkRootDirPath}/middleware/eiq/common/image/image_utils.h"
"${SdkRootDirPath}/middleware/eiq/common/model/get_top_n.cpp"
"${SdkRootDirPath}/middleware/eiq/common/model/get_top_n.h"
"${SdkRootDirPath}/middleware/eiq/common/model/model.h"
"${SdkRootDirPath}/middleware/eiq/common/model/output_postproc.h"
"${SdkRootDirPath}/middleware/eiq/tensorflow-lite/examples/common/model/output_postproc.cpp"
"${SdkRootDirPath}/middleware/eiq/tensorflow-lite/examples/label_image/demo_config.h"
"${SdkRootDirPath}/middleware/eiq/tensorflow-lite/examples/common/demo_info.cpp"
"${SdkRootDirPath}/middleware/eiq/tensorflow-lite/examples/label_image/image_data.h"
"${SdkRootDirPath}/middleware/eiq/tensorflow-lite/examples/label_image/labels.h"
"${SdkRootDirPath}/middleware/eiq/tensorflow-lite/examples/model.cpp"
"${SdkRootDirPath}/middleware/eiq/tensorflow-lite/examples/image/main.cpp"
"${SdkRootDirPath}/middleware/eiq/tensorflow-lite/examples/model_all_ops_micro.cpp"
"${SdkRootDirPath}/middleware/eiq/tensorflow-lite/examples/label_image/pcq/model_data.h"
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
"${SdkRootDirPath}/middleware/eiq/common/image/image_load.c"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${ProjDirPath}/..
    ${SdkRootDirPath}/middleware/eiq/tensorflow-lite/examples/label_image
    ${SdkRootDirPath}/middleware/eiq/common
    ${SdkRootDirPath}/middleware/eiq/common/image
    ${SdkRootDirPath}/middleware/eiq/common/model
    ${SdkRootDirPath}/middleware/eiq/tensorflow-lite/examples/label_image/pcq
)

set(CMAKE_MODULE_PATH
    ${SdkRootDirPath}/devices/MIMXRT1042/utilities
    ${SdkRootDirPath}/devices/MIMXRT1042/utilities/debug_console_lite
    ${SdkRootDirPath}/middleware/eiq/tensorflow-lite
    ${SdkRootDirPath}/components/lists
    ${SdkRootDirPath}/components/uart
    ${SdkRootDirPath}/devices/MIMXRT1042/drivers
    ${SdkRootDirPath}/devices/MIMXRT1042
    ${SdkRootDirPath}/devices/MIMXRT1042/xip
    ${SdkRootDirPath}/boards/evkmimxrt1040/xip
    ${SdkRootDirPath}/CMSIS/Core/Include
    ${SdkRootDirPath}/middleware/eiq
    ${SdkRootDirPath}/middleware/eiq/tensorflow-lite/third_party/cmsis
)

# include modules
include(utility_debug_console_lite_MIMXRT1042)

include(utility_assert_lite_MIMXRT1042)

include(middleware_eiq_tensorflow_lite_micro_MIMXRT1042)

include(component_lists_MIMXRT1042)

include(component_lpuart_adapter_MIMXRT1042)

include(driver_common_MIMXRT1042)

include(driver_lpuart_MIMXRT1042)

include(device_MIMXRT1042_CMSIS_MIMXRT1042)

include(device_MIMXRT1042_startup_MIMXRT1042)

include(driver_clock_MIMXRT1042)

include(driver_igpio_MIMXRT1042)

include(driver_iomuxc_MIMXRT1042)

include(driver_lpi2c_MIMXRT1042)

include(driver_xip_device_MIMXRT1042)

include(driver_xip_board_evkmimxrt1040_MIMXRT1042)

include(middleware_eiq_tensorflow_lite_micro_cmsis_nn_MIMXRT1042)

include(CMSIS_Include_core_cm_MIMXRT1042)

include(middleware_eiq_worker_MIMXRT1042)

include(middleware_eiq_tensorflow_lite_micro_third_party_flatbuffers_MIMXRT1042)

include(middleware_eiq_tensorflow_lite_micro_third_party_gemmlowp_MIMXRT1042)

include(middleware_eiq_tensorflow_lite_micro_third_party_ruy_MIMXRT1042)

include(utilities_misc_utilities_MIMXRT1042)

include(device_MIMXRT1042_system_MIMXRT1042)

include(middleware_eiq_tensorflow_lite_micro_third_party_cmsis_nn_MIMXRT1042)


IF(NOT DEFINED TARGET_LINK_SYSTEM_LIBRARIES)  
    SET(TARGET_LINK_SYSTEM_LIBRARIES "-lm -lc -lgcc -lnosys")  
ENDIF()  

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${TARGET_LINK_SYSTEM_LIBRARIES})

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)


