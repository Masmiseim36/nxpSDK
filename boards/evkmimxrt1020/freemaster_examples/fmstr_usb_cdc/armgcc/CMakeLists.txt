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


project(fmstr_example_usb_cdc)

set(MCUX_BUILD_TYPES flexspi_nor_debug flexspi_nor_release)

set(MCUX_SDK_PROJECT_NAME fmstr_example_usb_cdc.elf)

if (NOT DEFINED SdkRootDirPath)
    SET(SdkRootDirPath ${ProjDirPath}/../../../../..)
endif()

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${ProjDirPath}/../main.c"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../freemaster_cfg.h"
"${SdkRootDirPath}/boards/evkmimxrt1020/freemaster_examples/common/fmstr_example/freemaster_example.c"
"${SdkRootDirPath}/boards/evkmimxrt1020/freemaster_examples/common/fmstr_example/freemaster_example.h"
"${SdkRootDirPath}/middleware/freemaster/src/support/mcuxsdk_usb/freemaster_usb.c"
"${SdkRootDirPath}/middleware/freemaster/src/support/mcuxsdk_usb/freemaster_usb.h"
"${SdkRootDirPath}/middleware/freemaster/src/support/mcuxsdk_usb/freemaster_usb_device_descriptor.c"
"${SdkRootDirPath}/middleware/freemaster/src/support/mcuxsdk_usb/freemaster_usb_device_descriptor.h"
"${SdkRootDirPath}/middleware/freemaster/src/support/mcuxsdk_usb/usb_device_config.h"
"${SdkRootDirPath}/middleware/freemaster/src/support/mcuxsdk_usb/usb_device_descriptor.h"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../board.h"
"${SdkRootDirPath}/boards/evkmimxrt1020/freemaster_examples/common/fmstr_example/data_example_pmp.h"
"${SdkRootDirPath}/boards/evkmimxrt1020/freemaster_examples/common/fmstr_example/data_img_logo.h"
"${SdkRootDirPath}/boards/evkmimxrt1020/freemaster_examples/common/fmstr_example/data_img_file.h"
"${SdkRootDirPath}/boards/evkmimxrt1020/freemaster_examples/common/fmstr_example/data_img_fmstrlogo.h"
"${SdkRootDirPath}/boards/evkmimxrt1020/freemaster_examples/common/fmstr_example/data_html_index.h"
"${SdkRootDirPath}/boards/evkmimxrt1020/freemaster_examples/common/fmstr_example/example_data.h"
"${ProjDirPath}/../dcd.c"
"${ProjDirPath}/../dcd.h"
"${ProjDirPath}/../usb_device_ch9.c"
"${ProjDirPath}/../usb_device_ch9.h"
"${ProjDirPath}/../usb_device_class.c"
"${ProjDirPath}/../usb_device_class.h"
"${ProjDirPath}/../usb_device_cdc_acm.c"
"${ProjDirPath}/../usb_device_cdc_acm.h"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${ProjDirPath}/..
    ${SdkRootDirPath}/boards/evkmimxrt1020/freemaster_examples/common/fmstr_example
    ${SdkRootDirPath}/middleware/freemaster/src/support/mcuxsdk_usb
)

set(CMAKE_MODULE_PATH
    ${SdkRootDirPath}/middleware
    ${SdkRootDirPath}/middleware/usb
    ${SdkRootDirPath}/components/osa
    ${SdkRootDirPath}/middleware/freemaster
    ${SdkRootDirPath}/devices/MIMXRT1021/utilities
    ${SdkRootDirPath}/devices/MIMXRT1021/utilities/debug_console_lite
    ${SdkRootDirPath}/devices/MIMXRT1021/drivers
    ${SdkRootDirPath}/devices/MIMXRT1021
    ${SdkRootDirPath}/components/uart
    ${SdkRootDirPath}/components/lists
    ${SdkRootDirPath}/devices/MIMXRT1021/xip
    ${SdkRootDirPath}/boards/evkmimxrt1020/xip
    ${SdkRootDirPath}/CMSIS/Core/Include
)

# include modules
include(middleware_baremetal_MIMXRT1021)

include(middleware_usb_device_controller_driver_MIMXRT1021)

include(component_osa_MIMXRT1021)

include(middleware_usb_device_ehci_MIMXRT1021)

include(middleware_fmstr_MIMXRT1021)

include(utility_debug_console_lite_MIMXRT1021)

include(utility_assert_lite_MIMXRT1021)

include(driver_clock_MIMXRT1021)

include(driver_common_MIMXRT1021)

include(device_MIMXRT1021_CMSIS_MIMXRT1021)

include(component_lpuart_adapter_MIMXRT1021)

include(component_lists_MIMXRT1021)

include(driver_lpuart_MIMXRT1021)

include(device_MIMXRT1021_startup_MIMXRT1021)

include(driver_iomuxc_MIMXRT1021)

include(driver_igpio_MIMXRT1021)

include(driver_xip_device_MIMXRT1021)

include(driver_xip_board_evkmimxrt1020_MIMXRT1021)

include(middleware_fmstr_platform_gen32le_MIMXRT1021)

include(CMSIS_Include_core_cm_MIMXRT1021)

include(middleware_usb_phy_MIMXRT1021)

include(middleware_usb_common_header_MIMXRT1021)

include(middleware_usb_device_common_header_MIMXRT1021)

include(utilities_misc_utilities_MIMXRT1021)

include(device_MIMXRT1021_system_MIMXRT1021)


IF(NOT DEFINED TARGET_LINK_SYSTEM_LIBRARIES)  
    SET(TARGET_LINK_SYSTEM_LIBRARIES "-lm -lc -lgcc -lnosys")  
ENDIF()  

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${TARGET_LINK_SYSTEM_LIBRARIES})

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)


