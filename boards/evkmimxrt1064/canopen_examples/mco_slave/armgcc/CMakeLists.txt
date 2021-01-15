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
"${ProjDirPath}/../../mco_config/procimg.h"
"${ProjDirPath}/../../mco_config/user_cbdata.c"
"${ProjDirPath}/../../mco_config/user_od.c"
"${ProjDirPath}/../../mco_types.h"
"${ProjDirPath}/../../mcohw_cfg.h"
"${ProjDirPath}/../../mcohw_com_generic.c"
"${ProjDirPath}/../../mcohw_LEDs.h"
"${ProjDirPath}/../../mco_config/mcohw_nvol_sim.c"
"${ProjDirPath}/../../mco_libcb.c"
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
    ${ProjDirPath}/../../mco_config
    ${ProjDirPath}/../..
    ${ProjDirPath}/..
)

set(CMAKE_MODULE_PATH
    ${ProjDirPath}/../../../../../devices/MIMXRT1064/drivers
    ${ProjDirPath}/../../../../../devices/MIMXRT1064/utilities/debug_console_lite
    ${ProjDirPath}/../../../../../middleware/canopen
    ${ProjDirPath}/../../../../../devices/MIMXRT1064
    ${ProjDirPath}/../../../../../components/uart
    ${ProjDirPath}/../../../../../components/lists
    ${ProjDirPath}/../../../../../devices/MIMXRT1064/xip
    ${ProjDirPath}/../../../xip
    ${ProjDirPath}/../../../../../CMSIS/Include
    ${ProjDirPath}/../../../../../devices/MIMXRT1064/utilities
)

# include modules
include(driver_lpuart_MIMXRT1064)

include(utility_debug_console_lite_MIMXRT1064)

include(utility_assert_lite_MIMXRT1064)

include(driver_common_MIMXRT1064)

include(driver_clock_MIMXRT1064)

include(driver_flexcan_MIMXRT1064)

include(middleware_canopen_MIMXRT1064)

include(device_MIMXRT1064_CMSIS_MIMXRT1064)

include(component_lpuart_adapter_MIMXRT1064)

include(component_lists_MIMXRT1064)

include(device_MIMXRT1064_startup_MIMXRT1064)

include(driver_iomuxc_MIMXRT1064)

include(driver_igpio_MIMXRT1064)

include(driver_xip_device_MIMXRT1064)

include(driver_xip_board_MIMXRT1064)

include(CMSIS_Include_core_cm7_MIMXRT1064)

include(CMSIS_Include_common_MIMXRT1064)

include(CMSIS_Include_dsp_MIMXRT1064)

include(utilities_misc_utilities_MIMXRT1064)

include(device_MIMXRT1064_system_MIMXRT1064)


TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE m)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE c)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE gcc)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE nosys)

if(CMAKE_BUILD_TYPE STREQUAL debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../middleware/canopen/mcolibs/imxrt/mco_slave/libMCO_IMXRT10xx_SDK_3.a)
endif()

if(CMAKE_BUILD_TYPE STREQUAL release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../middleware/canopen/mcolibs/imxrt/mco_slave/libMCO_IMXRT10xx_SDK_3.a)
endif()

if(CMAKE_BUILD_TYPE STREQUAL sdram_debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../middleware/canopen/mcolibs/imxrt/mco_slave/libMCO_IMXRT10xx_SDK_3.a)
endif()

if(CMAKE_BUILD_TYPE STREQUAL sdram_release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../middleware/canopen/mcolibs/imxrt/mco_slave/libMCO_IMXRT10xx_SDK_3.a)
endif()

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../middleware/canopen/mcolibs/imxrt/mco_slave/libMCO_IMXRT10xx_SDK_3.a)
endif()

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../middleware/canopen/mcolibs/imxrt/mco_slave/libMCO_IMXRT10xx_SDK_3.a)
endif()

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)


