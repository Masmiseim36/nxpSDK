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


project(emwin_slide_show)

set(MCUX_SDK_PROJECT_NAME emwin_slide_show.elf)

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${ProjDirPath}/../emwin_slide_show.c"
"${ProjDirPath}/../Alarm.c"
"${ProjDirPath}/../Info.c"
"${ProjDirPath}/../Weather.c"
"${ProjDirPath}/../Resource.h"
"${ProjDirPath}/../bmNXP_110x44.c"
"${ProjDirPath}/../bmSeggerLogo_110x55.c"
"${ProjDirPath}/../bmBell_40x40.c"
"${ProjDirPath}/../bmBell_60x60.c"
"${ProjDirPath}/../bmArrowLeft_23x24.c"
"${ProjDirPath}/../bmArrowRight_23x24.c"
"${ProjDirPath}/../bmRainy_45x34.c"
"${ProjDirPath}/../bmStormy_45x34.c"
"${ProjDirPath}/../bmSunny_45x34.c"
"${ProjDirPath}/../AA4_B_20.c"
"${ProjDirPath}/../AA4_B_40.c"
"${ProjDirPath}/../AA4_B_100.c"
"${ProjDirPath}/../Font_13_R.c"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../emwin_support.c"
"${ProjDirPath}/../emwin_support.h"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
"${ProjDirPath}/../dcd.c"
"${ProjDirPath}/../dcd.h"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${ProjDirPath}/..
)

set(CMAKE_MODULE_PATH
    ${ProjDirPath}/../../../../../devices/MIMXRT1052/drivers
    ${ProjDirPath}/../../../../../components/ft5406_rt
    ${ProjDirPath}/../../../../../devices/MIMXRT1052/utilities/debug_console_lite
    ${ProjDirPath}/../../../../../middleware/emwin
    ${ProjDirPath}/../../../../../devices/MIMXRT1052
    ${ProjDirPath}/../../../../../components/uart
    ${ProjDirPath}/../../../../../components/lists
    ${ProjDirPath}/../../../../../devices/MIMXRT1052/xip
    ${ProjDirPath}/../../../xip
    ${ProjDirPath}/../../../../../CMSIS/Include
    ${ProjDirPath}/../../../../../devices/MIMXRT1052/utilities
)

# include modules
include(driver_elcdif_MIMXRT1052)

include(driver_pxp_MIMXRT1052)

include(driver_lpi2c_MIMXRT1052)

include(driver_ft5406_rt_MIMXRT1052)

include(driver_gpt_MIMXRT1052)

include(utility_debug_console_lite_MIMXRT1052)

include(utility_assert_lite_MIMXRT1052)

include(middleware_emwin_MIMXRT1052)

include(driver_common_MIMXRT1052)

include(driver_clock_MIMXRT1052)

include(device_MIMXRT1052_CMSIS_MIMXRT1052)

include(component_lpuart_adapter_MIMXRT1052)

include(component_lists_MIMXRT1052)

include(driver_lpuart_MIMXRT1052)

include(device_MIMXRT1052_startup_MIMXRT1052)

include(driver_iomuxc_MIMXRT1052)

include(driver_igpio_MIMXRT1052)

include(driver_xip_device_MIMXRT1052)

include(driver_xip_board_MIMXRT1052)

include(CMSIS_Include_core_cm7_MIMXRT1052)

include(CMSIS_Include_common_MIMXRT1052)

include(CMSIS_Include_dsp_MIMXRT1052)

include(utilities_misc_utilities_MIMXRT1052)

include(device_MIMXRT1052_system_MIMXRT1052)


TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE m)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE c)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE gcc)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE nosys)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_sdram_debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../middleware/emwin/emWin_library/ARMGCC/libemWin_M7.a)
endif()

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_sdram_release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../middleware/emwin/emWin_library/ARMGCC/libemWin_M7.a)
endif()

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)


