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


project(fmstr_example_usb_cdc_cm4)

set(MCUX_SDK_PROJECT_NAME fmstr_example_usb_cdc_cm4.elf)

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${ProjDirPath}/../main.c"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../freemaster_cfg.h"
"${ProjDirPath}/../../../common/fmstr_example/freemaster_example.c"
"${ProjDirPath}/../../../common/fmstr_example/freemaster_example.h"
"${ProjDirPath}/../../../../../../middleware/freemaster/src/support/mcuxsdk_usb/freemaster_usb.c"
"${ProjDirPath}/../../../../../../middleware/freemaster/src/support/mcuxsdk_usb/freemaster_usb.h"
"${ProjDirPath}/../../../../../../middleware/freemaster/src/support/mcuxsdk_usb/freemaster_usb_device_descriptor.c"
"${ProjDirPath}/../../../../../../middleware/freemaster/src/support/mcuxsdk_usb/freemaster_usb_device_descriptor.h"
"${ProjDirPath}/../../../../../../middleware/freemaster/src/support/mcuxsdk_usb/usb_device_config.h"
"${ProjDirPath}/../../../../../../middleware/freemaster/src/support/mcuxsdk_usb/usb_device_descriptor.h"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../../../common/fmstr_example/data_example_pmp.h"
"${ProjDirPath}/../../../common/fmstr_example/data_img_logo.h"
"${ProjDirPath}/../../../common/fmstr_example/data_img_file.h"
"${ProjDirPath}/../../../common/fmstr_example/data_img_fmstrlogo.h"
"${ProjDirPath}/../../../common/fmstr_example/data_html_index.h"
"${ProjDirPath}/../../../common/fmstr_example/example_data.h"
"${ProjDirPath}/../dcd.c"
"${ProjDirPath}/../dcd.h"
"${ProjDirPath}/../evkmimxrt1170_connect_cm4_cm4side.jlinkscript"
"${ProjDirPath}/../evkmimxrt1170_connect_cm4_cm4side_sdram.jlinkscript"
"${ProjDirPath}/../usb_device_ch9.c"
"${ProjDirPath}/../usb_device_ch9.h"
"${ProjDirPath}/../usb_device_class.c"
"${ProjDirPath}/../usb_device_class.h"
"${ProjDirPath}/../usb_device_cdc_acm.c"
"${ProjDirPath}/../usb_device_cdc_acm.h"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${ProjDirPath}/..
    ${ProjDirPath}/../../../common/fmstr_example
    ${ProjDirPath}/../../../../../../middleware/freemaster/src/support/mcuxsdk_usb
)

set(CMAKE_MODULE_PATH
    ${ProjDirPath}/../../../../../../middleware
    ${ProjDirPath}/../../../../../../middleware/usb
    ${ProjDirPath}/../../../../../../components/osa
    ${ProjDirPath}/../../../../../../middleware/freemaster
    ${ProjDirPath}/../../../../../../devices/MIMXRT1176/utilities/debug_console_lite
    ${ProjDirPath}/../../../../../../devices/MIMXRT1176/drivers
    ${ProjDirPath}/../../../../../../devices/MIMXRT1176
    ${ProjDirPath}/../../../../../../components/uart
    ${ProjDirPath}/../../../../../../components/lists
    ${ProjDirPath}/../../../../../../devices/MIMXRT1176/xip
    ${ProjDirPath}/../../../../xip
    ${ProjDirPath}/../../../../../../devices/MIMXRT1176/drivers/cm4
    ${ProjDirPath}/../../../../../../CMSIS/Core/Include
    ${ProjDirPath}/../../../../../../devices/MIMXRT1176/utilities
)

# include modules
include(middleware_baremetal_MIMXRT1176_cm4)

include(middleware_usb_device_controller_driver_MIMXRT1176_cm4)

include(component_osa_MIMXRT1176_cm4)

include(middleware_usb_device_ehci_MIMXRT1176_cm4)

include(middleware_fmstr_MIMXRT1176_cm4)

include(utility_debug_console_lite_MIMXRT1176_cm4)

include(utility_assert_lite_MIMXRT1176_cm4)

include(driver_clock_MIMXRT1176_cm4)

include(driver_common_MIMXRT1176_cm4)

include(device_MIMXRT1176_CMSIS_MIMXRT1176_cm4)

include(component_lpuart_adapter_MIMXRT1176_cm4)

include(component_lists_MIMXRT1176_cm4)

include(driver_lpuart_MIMXRT1176_cm4)

include(device_MIMXRT1176_startup_MIMXRT1176_cm4)

include(driver_iomuxc_MIMXRT1176_cm4)

include(driver_igpio_MIMXRT1176_cm4)

include(driver_xip_device_MIMXRT1176_cm4)

include(driver_xip_board_evkmimxrt1170_MIMXRT1176_cm4)

include(driver_pmu_1_MIMXRT1176_cm4)

include(driver_dcdc_soc_MIMXRT1176_cm4)

include(driver_cache_lmem_MIMXRT1176_cm4)

include(middleware_fmstr_platform_gen32le_MIMXRT1176_cm4)

include(driver_anatop_ai_MIMXRT1176_cm4)

include(CMSIS_Include_core_cm_MIMXRT1176_cm4)

include(middleware_usb_phy_MIMXRT1176_cm4)

include(middleware_usb_common_header_MIMXRT1176_cm4)

include(middleware_usb_device_common_header_MIMXRT1176_cm4)

include(utilities_misc_utilities_MIMXRT1176_cm4)

include(device_MIMXRT1176_system_MIMXRT1176_cm4)


TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE m)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE c)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE gcc)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE nosys)

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)


