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


project(usb_pd_alt_mode_dp_host_bm)

set(MCUX_BUILD_TYPES debug release flash_debug flash_release)

set(MCUX_SDK_PROJECT_NAME usb_pd_alt_mode_dp_host_bm.elf)

if (NOT DEFINED SdkRootDirPath)
    SET(SdkRootDirPath ${ProjDirPath}/../../../../../..)
endif()

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${ProjDirPath}/../pd_board_config.h"
"${ProjDirPath}/../pd_app.c"
"${ProjDirPath}/../pd_app.h"
"${ProjDirPath}/../pd_app_demo.c"
"${ProjDirPath}/../pd_command_app.c"
"${ProjDirPath}/../pd_command_interface.c"
"${ProjDirPath}/../pd_command_interface.h"
"${ProjDirPath}/../pd_power_app.c"
"${ProjDirPath}/../pd_power_interface.c"
"${ProjDirPath}/../pd_power_interface.h"
"${ProjDirPath}/../usb_pd_config.h"
"${ProjDirPath}/../pd_app_misc.h"
"${ProjDirPath}/../pd_dp_board_chip.c"
"${ProjDirPath}/../pd_dp_board_chip.h"
"${ProjDirPath}/../pd_crossbar_cbtl.h"
"${ProjDirPath}/../pd_crossbar_cbtl.c"
"${ProjDirPath}/../pd_ptn36502.h"
"${ProjDirPath}/../pd_ptn36502.c"
"${ProjDirPath}/../pd_dp_hpd_driver.h"
"${ProjDirPath}/../pd_dp_hpd_driver.c"
"${ProjDirPath}/../pd_power_nx20p3483.h"
"${ProjDirPath}/../pd_power_nx20p3483.c"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${ProjDirPath}/..
)

set(CMAKE_MODULE_PATH
    ${SdkRootDirPath}/devices/MIMXRT685S/drivers
    ${SdkRootDirPath}/middleware/usb
    ${SdkRootDirPath}/middleware
    ${SdkRootDirPath}/components/osa
    ${SdkRootDirPath}/devices/MIMXRT685S
    ${SdkRootDirPath}/devices/MIMXRT685S/utilities
    ${SdkRootDirPath}/components/uart
    ${SdkRootDirPath}/components/serial_manager
    ${SdkRootDirPath}/components/lists
    ${SdkRootDirPath}/components/i2c
    ${SdkRootDirPath}/boards/evkmimxrt685/flash_config
    ${SdkRootDirPath}/components/gpio
    ${SdkRootDirPath}/components/timer
    ${SdkRootDirPath}/CMSIS/Core/Include
)

# include modules
include(driver_mrt_MIMXRT685S_cm33)

include(middleware_usb_pd_phy_ptn5110_MIMXRT685S_cm33)

include(middleware_baremetal_MIMXRT685S_cm33)

include(middleware_usb_pd_MIMXRT685S_cm33)

include(middleware_usb_pd_altmode_MIMXRT685S_cm33)

include(component_osa_bm_MIMXRT685S_cm33)

include(driver_clock_MIMXRT685S_cm33)

include(driver_common_MIMXRT685S_cm33)

include(device_MIMXRT685S_CMSIS_MIMXRT685S_cm33)

include(utility_debug_console_MIMXRT685S_cm33)

include(utility_assert_MIMXRT685S_cm33)

include(component_usart_adapter_MIMXRT685S_cm33)

include(component_serial_manager_MIMXRT685S_cm33)

include(component_lists_MIMXRT685S_cm33)

include(component_serial_manager_uart_MIMXRT685S_cm33)

include(driver_flexcomm_usart_MIMXRT685S_cm33)

include(device_MIMXRT685S_startup_MIMXRT685S_cm33)

include(driver_flexcomm_MIMXRT685S_cm33)

include(driver_lpc_iopctl_MIMXRT685S_cm33)

include(driver_lpc_gpio_MIMXRT685S_cm33)

include(driver_pint_MIMXRT685S_cm33)

include(driver_cache_cache64_MIMXRT685S_cm33)

include(driver_inputmux_MIMXRT685S_cm33)

include(driver_flexcomm_i2c_MIMXRT685S_cm33)

include(component_flexcomm_i2c_adapter_MIMXRT685S_cm33)

include(driver_reset_MIMXRT685S_cm33)

include(driver_power_MIMXRT685S_cm33)

include(driver_flash_config_evkmimxrt685_MIMXRT685S_cm33)

include(driver_flexspi_MIMXRT685S_cm33)

include(component_lpc_gpio_adapter_MIMXRT685S_cm33)

include(component_mrt_adapter_MIMXRT685S_cm33)

include(CMSIS_Include_core_cm_MIMXRT685S_cm33)

include(driver_inputmux_connections_MIMXRT685S_cm33)

include(component_osa_MIMXRT685S_cm33)

include(middleware_usb_pd_common_header_MIMXRT685S_cm33)

include(utilities_misc_utilities_MIMXRT685S_cm33)


IF(NOT DEFINED TARGET_LINK_SYSTEM_LIBRARIES)  
    SET(TARGET_LINK_SYSTEM_LIBRARIES "-lm -lc -lgcc -lnosys")  
ENDIF()  

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${TARGET_LINK_SYSTEM_LIBRARIES})

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)


