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


project(lvgl_demo_widgets)

set(MCUX_BUILD_TYPES debug release flash_debug flash_release)

set(MCUX_SDK_PROJECT_NAME lvgl_demo_widgets.elf)

if (NOT DEFINED SdkRootDirPath)
    SET(SdkRootDirPath ${ProjDirPath}/../../../../..)
endif()

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${SdkRootDirPath}/boards/evkmimxrt685/lvgl_examples/RTE_Device.h"
"${ProjDirPath}/../lvgl_demo_widgets.c"
"${ProjDirPath}/../lv_conf.h"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${SdkRootDirPath}/middleware/lvgl/lvgl/demos/widgets/lv_demo_widgets.c"
"${SdkRootDirPath}/middleware/lvgl/lvgl/demos/widgets/lv_demo_widgets.h"
"${SdkRootDirPath}/middleware/lvgl/lvgl/demos/widgets/assets/img_clothes.c"
"${SdkRootDirPath}/middleware/lvgl/lvgl/demos/widgets/assets/img_demo_widgets_avatar.c"
"${SdkRootDirPath}/middleware/lvgl/lvgl/demos/widgets/assets/img_lvgl_logo.c"
"${SdkRootDirPath}/middleware/lvgl/lvgl/demos/lv_demos.h"
"${SdkRootDirPath}/boards/evkmimxrt685/lvgl_examples/lvgl_freertos.h"
"${SdkRootDirPath}/boards/evkmimxrt685/lvgl_examples/lvgl_freertos.c"
"${ProjDirPath}/../FreeRTOSConfig.h"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
"${SdkRootDirPath}/boards/evkmimxrt685/lvgl_examples/lvgl_support.h"
"${SdkRootDirPath}/boards/evkmimxrt685/lvgl_examples/lvgl_support.c"
"${SdkRootDirPath}/boards/evkmimxrt685/lvgl_examples/lvgl_demo_utils.h"
"${SdkRootDirPath}/boards/evkmimxrt685/lvgl_examples/lvgl_demo_utils.c"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${ProjDirPath}/..
    ${SdkRootDirPath}/boards/evkmimxrt685/lvgl_examples
    ${SdkRootDirPath}/middleware/lvgl/lvgl/demos/widgets
    ${SdkRootDirPath}/middleware/lvgl/lvgl/demos/widgets/assets
    ${SdkRootDirPath}/middleware/lvgl/lvgl/demos
)

set(CMAKE_MODULE_PATH
    ${SdkRootDirPath}/components/ili9341
    ${SdkRootDirPath}/components/ft6x06
    ${SdkRootDirPath}/devices/MIMXRT685S/drivers
    ${SdkRootDirPath}/devices/MIMXRT685S/cmsis_drivers
    ${SdkRootDirPath}/rtos/freertos/freertos-kernel
    ${SdkRootDirPath}/middleware/lvgl
    ${SdkRootDirPath}/devices/MIMXRT685S
    ${SdkRootDirPath}/devices/MIMXRT685S/utilities
    ${SdkRootDirPath}/components/uart
    ${SdkRootDirPath}/boards/evkmimxrt685/flash_config
    ${SdkRootDirPath}/components/serial_manager
    ${SdkRootDirPath}/components/lists
    ${SdkRootDirPath}/CMSIS/Core/Include
    ${SdkRootDirPath}/CMSIS/Driver/Include
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

include(middleware_freertos-kernel_cm33_nonsecure_port_MIMXRT685S_cm33)

include(middleware_freertos-kernel_heap_4_MIMXRT685S_cm33)

include(driver_common_MIMXRT685S_cm33)

include(middleware_lvgl_MIMXRT685S_cm33)

include(device_MIMXRT685S_CMSIS_MIMXRT685S_cm33)

include(utility_debug_console_MIMXRT685S_cm33)

include(utility_assert_MIMXRT685S_cm33)

include(component_usart_adapter_MIMXRT685S_cm33)

include(driver_power_MIMXRT685S_cm33)

include(driver_flash_config_evkmimxrt685_MIMXRT685S_cm33)

include(driver_flexspi_MIMXRT685S_cm33)

include(driver_cache_cache64_MIMXRT685S_cm33)

include(component_serial_manager_MIMXRT685S_cm33)

include(component_lists_MIMXRT685S_cm33)

include(component_serial_manager_uart_MIMXRT685S_cm33)

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

include(middleware_freertos-kernel_MIMXRT685S_cm33)

include(middleware_freertos-kernel_extension_MIMXRT685S_cm33)

include(utilities_misc_utilities_MIMXRT685S_cm33)


IF(NOT DEFINED TARGET_LINK_SYSTEM_LIBRARIES)  
    SET(TARGET_LINK_SYSTEM_LIBRARIES "-lm -lc -lgcc -lnosys")  
ENDIF()  

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${TARGET_LINK_SYSTEM_LIBRARIES})

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)

ADD_CUSTOM_COMMAND(TARGET ${MCUX_SDK_PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_OBJCOPY}
-Obinary ${EXECUTABLE_OUTPUT_PATH}/${MCUX_SDK_PROJECT_NAME} ${EXECUTABLE_OUTPUT_PATH}/sdk20-app.bin)

