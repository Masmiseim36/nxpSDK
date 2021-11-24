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


project(mco_slave)

set(MCUX_SDK_PROJECT_NAME mco_slave.elf)

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${ProjDirPath}/../main.c"
"${ProjDirPath}/../user_LPC5xxxx.c"
"${ProjDirPath}/../mco_config/EDS/entriesandreplies.h"
"${ProjDirPath}/../mco_config/EDS/pimg.h"
"${ProjDirPath}/../mco_config/EDS/stackinit.h"
"${ProjDirPath}/../mco_config/nodecfg.h"
"${ProjDirPath}/../mco_config/procimg.h"
"${ProjDirPath}/../mco_config/user_cbdata.c"
"${ProjDirPath}/../mco_config/user_od.c"
"${ProjDirPath}/../mco_types.h"
"${ProjDirPath}/../mcohw_cfg.h"
"${ProjDirPath}/../mcohw_com_generic.c"
"${ProjDirPath}/../mcohw_LEDs.h"
"${ProjDirPath}/../mcohw_nvol_sim.c"
"${ProjDirPath}/../mco_libcb.c"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${ProjDirPath}/../mco_config/EDS
    ${ProjDirPath}/../mco_config
    ${ProjDirPath}/..
    ${ProjDirPath}/../..
)

set(CMAKE_MODULE_PATH
    ${ProjDirPath}/../../../../../devices/LPC55S06/drivers
    ${ProjDirPath}/../../../../../devices/LPC55S06/utilities/debug_console_lite
    ${ProjDirPath}/../../../../../middleware/canopen/mco
    ${ProjDirPath}/../../../../../devices/LPC55S06
    ${ProjDirPath}/../../../../../components/uart
    ${ProjDirPath}/../../../../../components/lists
    ${ProjDirPath}/../../../../../CMSIS/Core/Include
    ${ProjDirPath}/../../../../../devices/LPC55S06/utilities
)

# include modules
include(driver_inputmux_LPC55S06)

include(driver_wwdt_LPC55S06)

include(utility_debug_console_lite_LPC55S06)

include(utility_assert_lite_LPC55S06)

include(driver_common_LPC55S06)

include(driver_clock_LPC55S06)

include(driver_mcan_LPC55S06)

include(middleware_canopen_LPC55S06)

include(device_LPC55S06_CMSIS_LPC55S06)

include(device_LPC55S06_startup_LPC55S06)

include(driver_flexcomm_usart_LPC55S06)

include(driver_flexcomm_LPC55S06)

include(driver_lpc_iocon_LPC55S06)

include(driver_lpc_gpio_LPC55S06)

include(driver_power_LPC55S06)

include(component_usart_adapter_LPC55S06)

include(component_lists_LPC55S06)

include(CMSIS_Include_core_cm_LPC55S06)

include(driver_reset_LPC55S06)

include(driver_inputmux_connections_LPC55S06)

include(utilities_misc_utilities_LPC55S06)

include(device_LPC55S06_system_LPC55S06)


TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE m)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE c)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE gcc)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE nosys)

if(CMAKE_BUILD_TYPE STREQUAL debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../middleware/canopen/mcolibs/LPC55xx/mco_slave/libMCO_LPC55xx_SDK.a)
endif(CMAKE_BUILD_TYPE STREQUAL debug)

if(CMAKE_BUILD_TYPE STREQUAL release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../middleware/canopen/mcolibs/LPC55xx/mco_slave/libMCO_LPC55xx_SDK.a)
endif(CMAKE_BUILD_TYPE STREQUAL release)

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)


