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


project(lvgl_demo_benchmark)

set(MCUX_BUILD_TYPES flash_debug flash_release)

set(MCUX_SDK_PROJECT_NAME lvgl_demo_benchmark.elf)

if (NOT DEFINED SdkRootDirPath)
    SET(SdkRootDirPath ${ProjDirPath}/../../../../..)
endif()

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${ProjDirPath}/../display_support.h"
"${ProjDirPath}/../display_support.c"
"${ProjDirPath}/../vglite_support.h"
"${ProjDirPath}/../vglite_support.c"
"${ProjDirPath}/../lvgl_demo_benchmark.c"
"${ProjDirPath}/../lv_conf.h"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${SdkRootDirPath}/middleware/lvgl/lvgl/demos/benchmark/lv_demo_benchmark.c"
"${SdkRootDirPath}/middleware/lvgl/lvgl/demos/benchmark/lv_demo_benchmark.h"
"${SdkRootDirPath}/middleware/lvgl/lvgl/demos/lv_demos.h"
"${SdkRootDirPath}/middleware/lvgl/lvgl/demos/benchmark/assets/img_benchmark_cogwheel_alpha16.c"
"${SdkRootDirPath}/middleware/lvgl/lvgl/demos/benchmark/assets/img_benchmark_cogwheel_argb.c"
"${SdkRootDirPath}/middleware/lvgl/lvgl/demos/benchmark/assets/img_benchmark_cogwheel_chroma_keyed.c"
"${SdkRootDirPath}/middleware/lvgl/lvgl/demos/benchmark/assets/img_benchmark_cogwheel_indexed16.c"
"${SdkRootDirPath}/middleware/lvgl/lvgl/demos/benchmark/assets/img_benchmark_cogwheel_rgb.c"
"${SdkRootDirPath}/middleware/lvgl/lvgl/demos/benchmark/assets/lv_font_bechmark_montserrat_16_compr_az.c.c"
"${SdkRootDirPath}/middleware/lvgl/lvgl/demos/benchmark/assets/lv_font_bechmark_montserrat_12_compr_az.c.c"
"${SdkRootDirPath}/middleware/lvgl/lvgl/demos/benchmark/assets/lv_font_bechmark_montserrat_28_compr_az.c.c"
"${SdkRootDirPath}/boards/evkmimxrt595/lvgl_examples/lvgl_freertos.h"
"${SdkRootDirPath}/boards/evkmimxrt595/lvgl_examples/lvgl_freertos.c"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
"${SdkRootDirPath}/boards/evkmimxrt595/lvgl_examples/lvgl_support.h"
"${SdkRootDirPath}/boards/evkmimxrt595/lvgl_examples/lvgl_support.c"
"${SdkRootDirPath}/boards/evkmimxrt595/lvgl_examples/lvgl_demo_utils.h"
"${SdkRootDirPath}/boards/evkmimxrt595/lvgl_examples/lvgl_demo_utils.c"
"${ProjDirPath}/../FreeRTOSConfig.h"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${ProjDirPath}/..
    ${SdkRootDirPath}/middleware/lvgl/lvgl/demos/benchmark
    ${SdkRootDirPath}/middleware/lvgl/lvgl/demos
    ${SdkRootDirPath}/middleware/lvgl/lvgl/examples/assets
    ${SdkRootDirPath}/middleware/lvgl/lvgl/demos/stress/assets
    ${SdkRootDirPath}/boards/evkmimxrt595/lvgl_examples
)

set(CMAKE_MODULE_PATH
    ${SdkRootDirPath}/devices/MIMXRT595S/drivers
    ${SdkRootDirPath}/components/ssd1963
    ${SdkRootDirPath}/components/video/display/dbi
    ${SdkRootDirPath}/components/video/display/dc/ssd1963
    ${SdkRootDirPath}/rtos/freertos/freertos-kernel
    ${SdkRootDirPath}/components/video/display/dc/lcdif
    ${SdkRootDirPath}/components/video
    ${SdkRootDirPath}/components/video/display
    ${SdkRootDirPath}/components/video/display/rm68191
    ${SdkRootDirPath}/components/video/display/mipi_dsi_cmd
    ${SdkRootDirPath}/components/video/display/rm68200
    ${SdkRootDirPath}/components/video/display/rm67162
    ${SdkRootDirPath}/components/video/display/dc/dsi_cmd
    ${SdkRootDirPath}/components/ft3267
    ${SdkRootDirPath}/components/gt911
    ${SdkRootDirPath}/components/ft5406
    ${SdkRootDirPath}/components/video/display/dbi/flexio
    ${SdkRootDirPath}/middleware/vglite
    ${SdkRootDirPath}/components/video/display/hx8394
    ${SdkRootDirPath}/middleware/lvgl
    ${SdkRootDirPath}/devices/MIMXRT595S
    ${SdkRootDirPath}/devices/MIMXRT595S/utilities
    ${SdkRootDirPath}/components/uart
    ${SdkRootDirPath}/components/serial_manager
    ${SdkRootDirPath}/components/lists
    ${SdkRootDirPath}/boards/evkmimxrt595/flash_config
    ${SdkRootDirPath}/CMSIS/Core/Include
    ${SdkRootDirPath}/components/video/display/dc
)

# include modules
include(driver_flexio_MIMXRT595S_cm33)

include(driver_flexio_mculcd_MIMXRT595S_cm33)

include(driver_lpc_dma_MIMXRT595S_cm33)

include(driver_ssd1963_MIMXRT595S_cm33)

include(driver_clock_MIMXRT595S_cm33)

include(driver_lpc_gpio_MIMXRT595S_cm33)

include(driver_dbi_MIMXRT595S_cm33)

include(driver_dc-fb-ssd1963_MIMXRT595S_cm33)

include(driver_power_MIMXRT595S_cm33)

include(middleware_freertos-kernel_cm33_nonsecure_port_MIMXRT595S_cm33)

include(driver_inputmux_MIMXRT595S_cm33)

include(driver_flexio_mculcd_smartdma_MIMXRT595S_cm33)

include(driver_dc-fb-lcdif_MIMXRT595S_cm33)

include(driver_video-common_MIMXRT595S_cm33)

include(driver_display-common_MIMXRT595S_cm33)

include(driver_display-rm68191_MIMXRT595S_cm33)

include(driver_mipi_dsi_MIMXRT595S_cm33)

include(driver_lcdif_MIMXRT595S_cm33)

include(driver_display-mipi-dsi-cmd_MIMXRT595S_cm33)

include(driver_common_MIMXRT595S_cm33)

include(driver_display-rm68200_MIMXRT595S_cm33)

include(driver_display-rm67162_MIMXRT595S_cm33)

include(driver_dc-fb-dsi-cmd_MIMXRT595S_cm33)

include(driver_flexcomm_i2c_MIMXRT595S_cm33)

include(driver_ft3267_MIMXRT595S_cm33)

include(driver_gt911_MIMXRT595S_cm33)

include(driver_mipi_dsi_smartdma_MIMXRT595S_cm33)

include(driver_ft5406_MIMXRT595S_cm33)

include(driver_dbi_flexio_smartdma_MIMXRT595S_cm33)

include(middleware_vglite_MIMXRT595S_cm33)

include(driver_display-hx8394_MIMXRT595S_cm33)

include(middleware_freertos-kernel_heap_4_MIMXRT595S_cm33)

include(middleware_lvgl_MIMXRT595S_cm33)

include(device_MIMXRT595S_CMSIS_MIMXRT595S_cm33)

include(utility_debug_console_MIMXRT595S_cm33)

include(utility_assert_MIMXRT595S_cm33)

include(component_usart_adapter_MIMXRT595S_cm33)

include(driver_flexspi_MIMXRT595S_cm33)

include(driver_cache_cache64_MIMXRT595S_cm33)

include(component_serial_manager_MIMXRT595S_cm33)

include(component_lists_MIMXRT595S_cm33)

include(component_serial_manager_uart_MIMXRT595S_cm33)

include(driver_flexcomm_usart_MIMXRT595S_cm33)

include(driver_flash_config_evkmimxrt595_MIMXRT595S_cm33)

include(device_MIMXRT595S_startup_MIMXRT595S_cm33)

include(driver_flexcomm_MIMXRT595S_cm33)

include(driver_lpc_iopctl_MIMXRT595S_cm33)

include(driver_reset_MIMXRT595S_cm33)

include(CMSIS_Include_core_cm_MIMXRT595S_cm33)

include(driver_dc-fb-common_MIMXRT595S_cm33)

include(middleware_freertos-kernel_MIMXRT595S_cm33)

include(middleware_freertos-kernel_extension_MIMXRT595S_cm33)

include(driver_inputmux_connections_MIMXRT595S_cm33)

include(driver_lpc_smartdma_MIMXRT595S_cm33)

include(driver_soc_mipi_dsi_MIMXRT595S_cm33)

include(utilities_misc_utilities_MIMXRT595S_cm33)

include(driver_iap_MIMXRT595S_cm33)


IF(NOT DEFINED TARGET_LINK_SYSTEM_LIBRARIES)  
    SET(TARGET_LINK_SYSTEM_LIBRARIES "-lm -lc -lgcc -lnosys")  
ENDIF()  

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${TARGET_LINK_SYSTEM_LIBRARIES})

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)

ADD_CUSTOM_COMMAND(TARGET ${MCUX_SDK_PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_OBJCOPY}
-Obinary ${EXECUTABLE_OUTPUT_PATH}/${MCUX_SDK_PROJECT_NAME} ${EXECUTABLE_OUTPUT_PATH}/sdk20-app.bin)

