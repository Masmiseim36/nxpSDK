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


project(flashloader)

set(MCUX_BUILD_TYPES release)

set(MCUX_SDK_PROJECT_NAME flashloader.elf)

if (NOT DEFINED SdkRootDirPath)
    SET(SdkRootDirPath ${ProjDirPath}/../../../../..)
endif()

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../usb_device_hid.c"
"${ProjDirPath}/../usb_device_hid.h"
"${ProjDirPath}/../usb_device_ch9.c"
"${ProjDirPath}/../usb_device_ch9.h"
"${ProjDirPath}/../usb_device_class.c"
"${ProjDirPath}/../usb_device_class.h"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${ProjDirPath}/..
)

set(CMAKE_MODULE_PATH
    ${SdkRootDirPath}/middleware/mcu-boot
    ${SdkRootDirPath}/middleware
    ${SdkRootDirPath}/middleware/usb
    ${SdkRootDirPath}/components/osa
    ${SdkRootDirPath}/CMSIS/Core/Include
    ${SdkRootDirPath}/devices/MIMXRT1011/drivers
    ${SdkRootDirPath}/devices/MIMXRT1011
    ${SdkRootDirPath}/components/lists
)

# include modules
include(middleware_mcu-boot_MIMXRT1011_startup_MIMXRT1011)

include(middleware_mcu-boot_bootsources_MIMXRT1011)

include(middleware_mcu-boot_MIMXRT1011_drivers_MIMXRT1011)

include(middleware_mcu-boot_MIMXRT1011_sources_MIMXRT1011)

include(middleware_baremetal_MIMXRT1011)

include(middleware_mcu-boot_src_bm_usb_hid_MIMXRT1011)

include(middleware_usb_device_controller_driver_MIMXRT1011)

include(component_osa_MIMXRT1011)

include(middleware_usb_device_ehci_MIMXRT1011)

include(middleware_mcu-boot_CMSIS_cm7_headers_MIMXRT1011)

include(middleware_mcu-boot_src_startup_MIMXRT1011)

include(middleware_mcu-boot_drv_microseconds_pit_MIMXRT1011)

include(middleware_mcu-boot_drv_ocotp_MIMXRT1011)

include(driver_common_MIMXRT1011)

include(driver_clock_MIMXRT1011)

include(device_MIMXRT1011_CMSIS_MIMXRT1011)

include(CMSIS_Include_core_cm_MIMXRT1011)

include(component_lists_MIMXRT1011)

include(middleware_usb_phy_MIMXRT1011)

include(middleware_usb_common_header_MIMXRT1011)

include(middleware_usb_device_common_header_MIMXRT1011)


IF(NOT DEFINED TARGET_LINK_SYSTEM_LIBRARIES)  
    SET(TARGET_LINK_SYSTEM_LIBRARIES "-lm -lc -lgcc -lnosys")  
ENDIF()  

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${TARGET_LINK_SYSTEM_LIBRARIES})

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)

ADD_CUSTOM_COMMAND(TARGET ${MCUX_SDK_PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_OBJCOPY}
-Osrec ${EXECUTABLE_OUTPUT_PATH}/${MCUX_SDK_PROJECT_NAME} ${EXECUTABLE_OUTPUT_PATH}/flashloader.srec)

