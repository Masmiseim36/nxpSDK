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


project(host_audio_speaker_bm)

set(MCUX_SDK_PROJECT_NAME host_audio_speaker_bm.elf)

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${ProjDirPath}/../app.h"
"${ProjDirPath}/../app.c"
"${ProjDirPath}/../audio_speaker.c"
"${ProjDirPath}/../audio_speaker.h"
"${ProjDirPath}/../audio8k16S.c"
"${ProjDirPath}/../host_keypad.c"
"${ProjDirPath}/../host_keypad.h"
"${ProjDirPath}/../usb_host_config.h"
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
    ${ProjDirPath}/..
)

set(CMAKE_MODULE_PATH
    ${ProjDirPath}/../../../../../../middleware/usb
    ${ProjDirPath}/../../../../../../devices/MIMXRT1064/drivers
    ${ProjDirPath}/../../../../../../devices/MIMXRT1064/utilities/debug_console_lite
    ${ProjDirPath}/../../../../../../middleware
    ${ProjDirPath}/../../../../../../devices/MIMXRT1064
    ${ProjDirPath}/../../../../../../devices/MIMXRT1064/xip
    ${ProjDirPath}/../../../../xip
    ${ProjDirPath}/../../../../../../components/uart
    ${ProjDirPath}/../../../../../../components/lists
    ${ProjDirPath}/../../../../../../components/osa
    ${ProjDirPath}/../../../../../../CMSIS/Include
    ${ProjDirPath}/../../../../../../devices/MIMXRT1064/utilities
)

# include modules
include(middleware_usb_host_ehci_MIMXRT1064)

include(middleware_usb_phy_MIMXRT1064)

include(driver_cache_armv7_m7_MIMXRT1064)

include(utility_debug_console_lite_MIMXRT1064)

include(utility_assert_lite_MIMXRT1064)

include(middleware_baremetal_MIMXRT1064)

include(middleware_usb_host_audio_MIMXRT1064)

include(middleware_usb_host_hid_MIMXRT1064)

include(driver_clock_MIMXRT1064)

include(driver_common_MIMXRT1064)

include(device_MIMXRT1064_CMSIS_MIMXRT1064)

include(device_MIMXRT1064_startup_MIMXRT1064)

include(driver_lpuart_MIMXRT1064)

include(driver_iomuxc_MIMXRT1064)

include(driver_igpio_MIMXRT1064)

include(driver_xip_device_MIMXRT1064)

include(driver_xip_board_MIMXRT1064)

include(component_lpuart_adapter_MIMXRT1064)

include(component_lists_MIMXRT1064)

include(middleware_usb_host_stack_MIMXRT1064)

include(component_osa_MIMXRT1064)

include(middleware_usb_common_header_MIMXRT1064)

include(CMSIS_Include_core_cm7_MIMXRT1064)

include(CMSIS_Include_common_MIMXRT1064)

include(CMSIS_Include_dsp_MIMXRT1064)

include(middleware_usb_host_common_header_MIMXRT1064)

include(utilities_misc_utilities_MIMXRT1064)

include(device_MIMXRT1064_system_MIMXRT1064)


TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE m)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE c)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE gcc)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE nosys)

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)


