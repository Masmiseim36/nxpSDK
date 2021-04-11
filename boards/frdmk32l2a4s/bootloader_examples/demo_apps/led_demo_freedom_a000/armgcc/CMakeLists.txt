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


project(led_demo_freedom_a000)

set(MCUX_SDK_PROJECT_NAME led_demo_freedom_a000.elf)

include(${ProjDirPath}/flags.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../main.c"
"${ProjDirPath}/../../../../../../middleware/mcu-boot/apps/led_demo/src/milliseconds_delay.h"
"${ProjDirPath}/../../../../../../middleware/mcu-boot/apps/led_demo/src/milliseconds_delay.c"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${ProjDirPath}/..
    ${ProjDirPath}/../../../../../../middleware/mcu-boot/apps/led_demo/src
)

set(CMAKE_MODULE_PATH
    ${ProjDirPath}/../../../../../../devices/K32L2A41A/drivers
    ${ProjDirPath}/../../../../../../devices/K32L2A41A
    ${ProjDirPath}/../../../../../../middleware/mcu-boot
    ${ProjDirPath}/../../../../../../CMSIS/Include
)

# include modules
include(driver_port_K32L2A41A)

include(driver_gpio_K32L2A41A)

include(device_K32L2A41A_CMSIS_K32L2A41A)

include(middleware_mcu-boot_K32L2A41A_startup_K32L2A41A)

include(driver_clock_K32L2A41A)

include(driver_wdog32_K32L2A41A)

include(middleware_mcu-boot_src_startup_K32L2A41A)

include(driver_common_K32L2A41A)

include(CMSIS_Include_core_cm0plus_K32L2A41A)

include(CMSIS_Include_common_K32L2A41A)

include(CMSIS_Include_dsp_K32L2A41A)


TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE m)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE c)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE gcc)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE nosys)

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)


