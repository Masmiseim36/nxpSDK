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


project(guix_washing_machine)

set(MCUX_SDK_PROJECT_NAME guix_washing_machine.elf)

SET(CMAKE_STATIC_LIBRARY_PREFIX "lib")

SET(CMAKE_STATIC_LIBRARY_SUFFIX ".a")

SET(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

SET(THREADX_ARCH "cortex_m7")

SET(THREADX_TOOLCHAIN "gnu")

SET(GX_USER_FILE ${ProjDirPath}/../gx_user.h)

SET(TX_USER_FILE ${ProjDirPath}/../tx_user.h)

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${ProjDirPath}/../display_support.h"
"${ProjDirPath}/../tx_user.h"
"${ProjDirPath}/../gx_user.h"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../guix_washing_machine.c"
"${ProjDirPath}/../guix_washing_machine.h"
"${ProjDirPath}/../garments.c"
"${ProjDirPath}/../sample_guix_washing_machine_resources.c"
"${ProjDirPath}/../sample_guix_washing_machine_resources.h"
"${ProjDirPath}/../sample_guix_washing_machine_specifications.c"
"${ProjDirPath}/../sample_guix_washing_machine_specifications.h"
"${ProjDirPath}/../temperature.c"
"${ProjDirPath}/../washer_on.c"
"${ProjDirPath}/../water_level.c"
"${ProjDirPath}/../gx_touch_driver_imxrt10xx_capacitive.c"
"${ProjDirPath}/../gx_display_driver_imxrt10xx_565rgb.c"
"${ProjDirPath}/../gx_driver_imxrt10xx_irq.c"
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
    ${ProjDirPath}/../../../../../rtos/azure-rtos
    ${ProjDirPath}/../../../../../components/ft5406_rt
    ${ProjDirPath}/../../../../../components/gt911
    ${ProjDirPath}/../../../../../components/video/display/dc/elcdif
    ${ProjDirPath}/../../../../../devices/MIMXRT1062/drivers
    ${ProjDirPath}/../../../../../devices/MIMXRT1062
    ${ProjDirPath}/../../../../../devices/MIMXRT1062/utilities
    ${ProjDirPath}/../../../../../components/uart
    ${ProjDirPath}/../../../../../components/serial_manager
    ${ProjDirPath}/../../../../../components/lists
    ${ProjDirPath}/../../../../../devices/MIMXRT1062/xip
    ${ProjDirPath}/../../../xip
    ${ProjDirPath}/../../../../../CMSIS/Core/Include
    ${ProjDirPath}/../../../../../components/video/display/dc
    ${ProjDirPath}/../../../../../components/video
)

# include modules
include(middleware_azure_rtos_gx_MIMXRT1062)

include(driver_ft5406_rt_MIMXRT1062)

include(driver_gt911_MIMXRT1062)

include(driver_dc-fb-elcdif_MIMXRT1062)

include(driver_lpi2c_MIMXRT1062)

include(driver_pxp_MIMXRT1062)

include(driver_clock_MIMXRT1062)

include(driver_common_MIMXRT1062)

include(device_MIMXRT1062_CMSIS_MIMXRT1062)

include(utility_debug_console_MIMXRT1062)

include(component_lpuart_adapter_MIMXRT1062)

include(component_serial_manager_MIMXRT1062)

include(component_lists_MIMXRT1062)

include(component_serial_manager_uart_MIMXRT1062)

include(driver_lpuart_MIMXRT1062)

include(device_MIMXRT1062_startup_MIMXRT1062)

include(driver_iomuxc_MIMXRT1062)

include(utility_assert_MIMXRT1062)

include(driver_igpio_MIMXRT1062)

include(driver_xip_device_MIMXRT1062)

include(driver_xip_board_evkmimxrt1060_MIMXRT1062)

include(middleware_azure_rtos_tx_MIMXRT1062)

include(middleware_azure_rtos_tx_template_MIMXRT1062)

include(CMSIS_Include_core_cm_MIMXRT1062)

include(driver_dc-fb-common_MIMXRT1062)

include(driver_video-common_MIMXRT1062)

include(driver_elcdif_MIMXRT1062)

include(utilities_misc_utilities_MIMXRT1062)

include(device_MIMXRT1062_system_MIMXRT1062)


add_subdirectory(${ProjDirPath}/../../../../../rtos/azure-rtos/threadx ./CMakeFiles/libthreadx)

add_subdirectory(${ProjDirPath}/../../../../../rtos/azure-rtos/guix ./CMakeFiles/libguix)

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE m)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE c)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE gcc)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE nosys)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE "azrtos::guix")

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE "azrtos::threadx")

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)


