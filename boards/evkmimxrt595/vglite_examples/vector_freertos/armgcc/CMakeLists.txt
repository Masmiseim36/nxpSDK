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


project(vector_freertos)

set(MCUX_SDK_PROJECT_NAME vector_freertos.elf)

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${ProjDirPath}/../vector_freertos.c"
"${ProjDirPath}/../display_support.h"
"${ProjDirPath}/../display_support.c"
"${ProjDirPath}/../../vglite_support/vglite_support.h"
"${ProjDirPath}/../../vglite_support/vglite_support.c"
"${ProjDirPath}/../../vglite_window/vglite_window.h"
"${ProjDirPath}/../../vglite_window/vglite_window.c"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../FreeRTOSConfig.h"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${ProjDirPath}/.. debug
    ${ProjDirPath}/..
    ${ProjDirPath}/../../vglite_support
    ${ProjDirPath}/../../vglite_window
)

set(CMAKE_MODULE_PATH
    ${ProjDirPath}/../../../../../devices/MIMXRT595S/drivers
    ${ProjDirPath}/../../../../../components/ssd1963
    ${ProjDirPath}/../../../../../components/video/display/dbi
    ${ProjDirPath}/../../../../../components/video/display/dbi/flexio
    ${ProjDirPath}/../../../../../components/video/display/dc/ssd1963
    ${ProjDirPath}/../../../../../rtos/freertos
    ${ProjDirPath}/../../../../../middleware/vglite
    ${ProjDirPath}/../../../../../components/video/display/dc/lcdif
    ${ProjDirPath}/../../../../../components/video/display
    ${ProjDirPath}/../../../../../components/video/display/rm68191
    ${ProjDirPath}/../../../../../components/video/display/mipi_dsi_cmd
    ${ProjDirPath}/../../../../../components/video/display/rm68200
    ${ProjDirPath}/../../../../../components/video/display/rm67162
    ${ProjDirPath}/../../../../../components/video/display/dc/dsi_cmd
    ${ProjDirPath}/../../../../../components/video/display/fbdev
    ${ProjDirPath}/../../../../../components/video
    ${ProjDirPath}/../../../../../devices/MIMXRT595S
    ${ProjDirPath}/../../../../../devices/MIMXRT595S/utilities
    ${ProjDirPath}/../../../../../components/uart
    ${ProjDirPath}/../../../../../components/serial_manager
    ${ProjDirPath}/../../../../../components/lists
    ${ProjDirPath}/../../../flash_config
    ${ProjDirPath}/../../../../../CMSIS/Include
    ${ProjDirPath}/../../../../../components/video/display/dc
)

# include modules
include(driver_flexio_MIMXRT595S_cm33)

include(driver_flexio_mculcd_MIMXRT595S_cm33)

include(driver_lpc_dma_MIMXRT595S_cm33)

include(driver_ssd1963_MIMXRT595S_cm33)

include(driver_clock_MIMXRT595S_cm33)

include(driver_lpc_gpio_MIMXRT595S_cm33)

include(driver_dbi_MIMXRT595S_cm33)

include(driver_dbi_flexio_smartdma_MIMXRT595S_cm33)

include(driver_dc-fb-ssd1963_MIMXRT595S_cm33)

include(driver_power_MIMXRT595S_cm33)

include(middleware_freertos-kernel_cm33_nonsecure_port_MIMXRT595S_cm33)

include(driver_inputmux_MIMXRT595S_cm33)

include(driver_flexio_mculcd_smartdma_MIMXRT595S_cm33)

include(middleware_vglite_MIMXRT595S_cm33)

include(middleware_freertos-kernel_heap_4_MIMXRT595S_cm33)

include(driver_dc-fb-lcdif_MIMXRT595S_cm33)

include(driver_display-common_MIMXRT595S_cm33)

include(driver_display-rm68191_MIMXRT595S_cm33)

include(driver_mipi_dsi_MIMXRT595S_cm33)

include(driver_lcdif_MIMXRT595S_cm33)

include(driver_display-mipi-dsi-cmd_MIMXRT595S_cm33)

include(driver_display-rm68200_MIMXRT595S_cm33)

include(driver_display-rm67162_MIMXRT595S_cm33)

include(driver_dc-fb-dsi-cmd_MIMXRT595S_cm33)

include(driver_mipi_dsi_smartdma_MIMXRT595S_cm33)

include(driver_fbdev_MIMXRT595S_cm33)

include(driver_video-common_MIMXRT595S_cm33)

include(driver_common_MIMXRT595S_cm33)

include(driver_reset_MIMXRT595S_cm33)

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

include(driver_flash_config_MIMXRT595S_cm33)

include(device_MIMXRT595S_startup_MIMXRT595S_cm33)

include(driver_flexcomm_MIMXRT595S_cm33)

include(driver_lpc_iopctl_MIMXRT595S_cm33)

include(CMSIS_Include_core_cm33_MIMXRT595S_cm33)

include(CMSIS_Include_common_MIMXRT595S_cm33)

include(CMSIS_Include_dsp_MIMXRT595S_cm33)

include(driver_lpc_smartdma_MIMXRT595S_cm33)

include(driver_dc-fb-common_MIMXRT595S_cm33)

include(middleware_freertos-kernel_MIMXRT595S_cm33)

include(middleware_freertos-kernel_extension_MIMXRT595S_cm33)

include(driver_inputmux_connections_MIMXRT595S_cm33)

include(utilities_misc_utilities_MIMXRT595S_cm33)

include(driver_iap_MIMXRT595S_cm33)


TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE m)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE c)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE gcc)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE nosys)

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)

ADD_CUSTOM_COMMAND(TARGET ${MCUX_SDK_PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_OBJCOPY}
-Obinary ${EXECUTABLE_OUTPUT_PATH}/${MCUX_SDK_PROJECT_NAME} ${EXECUTABLE_OUTPUT_PATH}/sdk20-app.bin)

