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
"${ProjDirPath}/../user_IMXRT10xx.c"
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
"${ProjDirPath}/../dcd.c"
"${ProjDirPath}/../dcd.h"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${ProjDirPath}/../mco_config/EDS
    ${ProjDirPath}/../mco_config
    ${ProjDirPath}/..
    ${ProjDirPath}/../..
)

set(CMAKE_MODULE_PATH
    ${ProjDirPath}/../../../../../devices/MIMXRT1042/drivers
    ${ProjDirPath}/../../../../../devices/MIMXRT1042/utilities/debug_console_lite
    ${ProjDirPath}/../../../../../middleware/canopen/mco
    ${ProjDirPath}/../../../../../devices/MIMXRT1042
    ${ProjDirPath}/../../../../../components/uart
    ${ProjDirPath}/../../../../../components/lists
    ${ProjDirPath}/../../../../../devices/MIMXRT1042/xip
    ${ProjDirPath}/../../../xip
    ${ProjDirPath}/../../../../../components/silicon_id
    ${ProjDirPath}/../../../../../CMSIS/Core/Include
    ${ProjDirPath}/../../../../../devices/MIMXRT1042/utilities
)

# include modules
include(driver_lpuart_MIMXRT1042)

include(utility_debug_console_lite_MIMXRT1042)

include(utility_assert_lite_MIMXRT1042)

include(driver_common_MIMXRT1042)

include(driver_clock_MIMXRT1042)

include(driver_flexcan_MIMXRT1042)

include(middleware_canopen_MIMXRT1042)

include(device_MIMXRT1042_CMSIS_MIMXRT1042)

include(component_lpuart_adapter_MIMXRT1042)

include(component_lists_MIMXRT1042)

include(device_MIMXRT1042_startup_MIMXRT1042)

include(driver_iomuxc_MIMXRT1042)

include(driver_igpio_MIMXRT1042)

include(driver_xip_device_MIMXRT1042)

include(driver_xip_board_evkmimxrt1040_MIMXRT1042)

include(component_silicon_id_MIMXRT1042)

include(CMSIS_Include_core_cm_MIMXRT1042)

include(utilities_misc_utilities_MIMXRT1042)

include(device_MIMXRT1042_system_MIMXRT1042)


TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE m)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE c)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE gcc)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE nosys)

if(CMAKE_BUILD_TYPE STREQUAL debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../middleware/canopen/mcolibs/imxrt/mco_slave/libMCO_IMXRT10xx_SDK_2.a)
endif(CMAKE_BUILD_TYPE STREQUAL debug)

if(CMAKE_BUILD_TYPE STREQUAL release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../middleware/canopen/mcolibs/imxrt/mco_slave/libMCO_IMXRT10xx_SDK_2.a)
endif(CMAKE_BUILD_TYPE STREQUAL release)

if(CMAKE_BUILD_TYPE STREQUAL sdram_debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../middleware/canopen/mcolibs/imxrt/mco_slave/libMCO_IMXRT10xx_SDK_2.a)
endif(CMAKE_BUILD_TYPE STREQUAL sdram_debug)

if(CMAKE_BUILD_TYPE STREQUAL sdram_release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../middleware/canopen/mcolibs/imxrt/mco_slave/libMCO_IMXRT10xx_SDK_2.a)
endif(CMAKE_BUILD_TYPE STREQUAL sdram_release)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../middleware/canopen/mcolibs/imxrt/mco_slave/libMCO_IMXRT10xx_SDK_2.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../middleware/canopen/mcolibs/imxrt/mco_slave/libMCO_IMXRT10xx_SDK_2.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)


