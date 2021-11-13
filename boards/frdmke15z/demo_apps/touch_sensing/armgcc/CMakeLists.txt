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


project(touch_sensing)

set(MCUX_SDK_PROJECT_NAME touch_sensing.elf)

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${ProjDirPath}/../../../../../middleware/touch/source/drivers/tsi/nt_drv_tsi_common.c"
"${ProjDirPath}/../../../../../middleware/touch/source/drivers/tsi/nt_drv_tsi_driver.c"
"${ProjDirPath}/../../../../../middleware/touch/source/drivers/tsi/nt_drv_tsi_driver.h"
"${ProjDirPath}/../../../../../middleware/touch/source/drivers/tsi/nt_drv_tsi_v5_driver_specific.c"
"${ProjDirPath}/../../../../../middleware/touch/source/drivers/tsi/nt_drv_tsi_v5_driver_specific.h"
"${ProjDirPath}/../../../../../middleware/touch/source/modules/nt_module_tsi.c"
"${ProjDirPath}/../../../../../middleware/touch/source/modules/nt_module_tsi_prv.h"
"${ProjDirPath}/../../../../../middleware/touch/include/nt_module_tsi.h"
"${ProjDirPath}/../../../../../middleware/touch/source/safety/nt_safety.c"
"${ProjDirPath}/../../../../../middleware/touch/source/safety/nt_safety_gpio.c"
"${ProjDirPath}/../../../../../middleware/touch/source/safety/nt_safety_crc.c"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../freemaster_cfg.h"
"${ProjDirPath}/../main.c"
"${ProjDirPath}/../main.h"
"${ProjDirPath}/../nt_setup.c"
"${ProjDirPath}/../nt_setup.h"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${ProjDirPath}/../../../../../middleware/touch/source/drivers/tsi
    ${ProjDirPath}/../../../../../middleware/touch/source/modules
    ${ProjDirPath}/../../../../../middleware/touch/include
    ${ProjDirPath}/..
)

set(CMAKE_MODULE_PATH
    ${ProjDirPath}/../../../../../devices/MKE15Z7/drivers
    ${ProjDirPath}/../../../../../devices/MKE15Z7/utilities/debug_console_lite
    ${ProjDirPath}/../../../../../middleware/touch
    ${ProjDirPath}/../../../../../devices/MKE15Z7
    ${ProjDirPath}/../../../../../components/uart
    ${ProjDirPath}/../../../../../components/lists
    ${ProjDirPath}/../../../../../middleware/freemaster
    ${ProjDirPath}/../../../../../CMSIS/Core/Include
    ${ProjDirPath}/../../../../../devices/MKE15Z7/utilities
)

# include modules
include(driver_port_MKE15Z7)

include(driver_ftm_MKE15Z7)

include(driver_gpio_MKE15Z7)

include(driver_clock_MKE15Z7)

include(driver_lpit_MKE15Z7)

include(driver_lpuart_MKE15Z7)

include(driver_smc_MKE15Z7)

include(driver_tsi_v5_MKE15Z7)

include(driver_crc_MKE15Z7)

include(utility_debug_console_lite_MKE15Z7)

include(utility_assert_lite_MKE15Z7)

include(middleware_touch_MKE15Z7)

include(driver_common_MKE15Z7)

include(device_MKE15Z7_CMSIS_MKE15Z7)

include(device_MKE15Z7_startup_MKE15Z7)

include(component_lpuart_adapter_MKE15Z7)

include(component_lists_MKE15Z7)

include(middleware_fmstr_MKE15Z7)

include(middleware_fmstr_platform_gen32le_MKE15Z7)

include(CMSIS_Include_core_cm_MKE15Z7)

include(utilities_misc_utilities_MKE15Z7)

include(device_MKE15Z7_system_MKE15Z7)


TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE m)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE c)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE gcc)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE nosys)

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)


