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


project(lvgl_demo_widgets_bm)

set(MCUX_SDK_PROJECT_NAME lvgl_demo_widgets_bm.elf)

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${ProjDirPath}/../../RTE_Device.h"
"${ProjDirPath}/../lvgl_demo_widgets_bm.c"
"${ProjDirPath}/../lv_conf.h"
"${ProjDirPath}/../lv_demo_conf.h"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../../../../../middleware/lvgl/lv_examples/src/lv_demo_widgets/lv_demo_widgets.c"
"${ProjDirPath}/../../../../../middleware/lvgl/lv_examples/src/lv_demo_widgets/lv_demo_widgets.h"
"${ProjDirPath}/../../../../../middleware/lvgl/lv_examples/src/lv_demo_widgets/assets/img_clothes.c"
"${ProjDirPath}/../../../../../middleware/lvgl/lv_examples/src/lv_demo_widgets/assets/img_demo_widgets_avatar.c"
"${ProjDirPath}/../../../../../middleware/lvgl/lv_examples/src/lv_demo_widgets/assets/img_lvgl_logo.c"
"${ProjDirPath}/../../../../../middleware/lvgl/lv_examples/lv_demo.h"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
"${ProjDirPath}/../../lvgl_support.h"
"${ProjDirPath}/../../lvgl_support.c"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${ProjDirPath}/../..
    ${ProjDirPath}/..
    ${ProjDirPath}/../../../../../middleware/lvgl/lv_examples/src/lv_demo_widgets
    ${ProjDirPath}/../../../../../middleware/lvgl/lv_examples/src/lv_demo_widgets/assets
    ${ProjDirPath}/../../../../../middleware/lvgl/lv_examples
)

set(CMAKE_MODULE_PATH
    ${ProjDirPath}/../../../../../components/ili9341
    ${ProjDirPath}/../../../../../components/ft6x06
    ${ProjDirPath}/../../../../../devices/MIMXRT685S/drivers
    ${ProjDirPath}/../../../../../devices/MIMXRT685S/cmsis_drivers
    ${ProjDirPath}/../../../../../devices/MIMXRT685S/utilities/debug_console_lite
    ${ProjDirPath}/../../../../../middleware/lvgl
    ${ProjDirPath}/../../../../../devices/MIMXRT685S
    ${ProjDirPath}/../../../../../components/uart
    ${ProjDirPath}/../../../flash_config
    ${ProjDirPath}/../../../../../components/lists
    ${ProjDirPath}/../../../../../CMSIS/Core/Include
    ${ProjDirPath}/../../../../../CMSIS/Driver/Include
    ${ProjDirPath}/../../../../../devices/MIMXRT685S/utilities
)

# include modules
include(driver_ili9341_MIMXRT685S_cm33)

include(driver_ft6x06_MIMXRT685S_cm33)

include(driver_clock_MIMXRT685S_cm33)

include(driver_flexcomm_spi_dma_MIMXRT685S_cm33)

include(driver_flexcomm_i2c_MIMXRT685S_cm33)

include(driver_flexcomm_spi_MIMXRT685S_cm33)

include(driver_cmsis_flexcomm_spi_MIMXRT685S_cm33)

include(driver_inputmux_MIMXRT685S_cm33)

include(driver_cmsis_flexcomm_i2c_MIMXRT685S_cm33)

include(driver_inputmux_connections_MIMXRT685S_cm33)

include(driver_flexcomm_i2c_dma_MIMXRT685S_cm33)

include(driver_lpc_dma_MIMXRT685S_cm33)

include(utility_debug_console_lite_MIMXRT685S_cm33)

include(utility_assert_lite_MIMXRT685S_cm33)

include(driver_common_MIMXRT685S_cm33)

include(middleware_lvgl_MIMXRT685S_cm33)

include(device_MIMXRT685S_CMSIS_MIMXRT685S_cm33)

include(component_usart_adapter_MIMXRT685S_cm33)

include(driver_power_MIMXRT685S_cm33)

include(driver_flash_config_evkmimxrt685_MIMXRT685S_cm33)

include(driver_flexspi_MIMXRT685S_cm33)

include(driver_cache_cache64_MIMXRT685S_cm33)

include(component_lists_MIMXRT685S_cm33)

include(driver_flexcomm_usart_MIMXRT685S_cm33)

include(device_MIMXRT685S_startup_MIMXRT685S_cm33)

include(driver_flexcomm_MIMXRT685S_cm33)

include(driver_lpc_iopctl_MIMXRT685S_cm33)

include(driver_lpc_gpio_MIMXRT685S_cm33)

include(driver_reset_MIMXRT685S_cm33)

include(CMSIS_Include_core_cm_MIMXRT685S_cm33)

include(CMSIS_Driver_Include_I2C_MIMXRT685S_cm33)

include(CMSIS_Driver_Include_Common_MIMXRT685S_cm33)

include(CMSIS_Driver_Include_SPI_MIMXRT685S_cm33)

include(utilities_misc_utilities_MIMXRT685S_cm33)


TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE m)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE c)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE gcc)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE nosys)

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)

ADD_CUSTOM_COMMAND(TARGET ${MCUX_SDK_PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_OBJCOPY}
-Obinary ${EXECUTABLE_OUTPUT_PATH}/${MCUX_SDK_PROJECT_NAME} ${EXECUTABLE_OUTPUT_PATH}/sdk20-app.bin)

