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


project(usbx_host_hid_keyboard)

set(MCUX_SDK_PROJECT_NAME usbx_host_hid_keyboard.elf)

SET(CMAKE_STATIC_LIBRARY_PREFIX "lib")

SET(CMAKE_STATIC_LIBRARY_SUFFIX ".a")

SET(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

SET(THREADX_ARCH "cortex_m7")

SET(THREADX_TOOLCHAIN "gnu")

SET(UX_USER_FILE ${ProjDirPath}/../ux_user.h)

SET(NX_USER_FILE ${ProjDirPath}/../nx_user.h)

SET(FX_USER_FILE ${ProjDirPath}/../fx_user.h)

SET(TX_USER_FILE ${ProjDirPath}/../tx_user.h)

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../board_setup.c"
"${ProjDirPath}/../board_setup.h"
"${ProjDirPath}/../../../../../../middleware/usb/include/usb.h"
"${ProjDirPath}/../../../../../../middleware/usb/include/usb_misc.h"
"${ProjDirPath}/../../../../../../middleware/usb/include/usb_spec.h"
"${ProjDirPath}/../../../../../../middleware/usb/phy/usb_phy.c"
"${ProjDirPath}/../../../../../../middleware/usb/phy/usb_phy.h"
"${ProjDirPath}/../../../../../../components/osa/fsl_os_abstraction.h"
"${ProjDirPath}/../../../../../../components/osa/fsl_os_abstraction_bm.h"
"${ProjDirPath}/../../../../../../components/osa/fsl_os_abstraction_config.h"
"${ProjDirPath}/../usbx_host_hid_keyboard.c"
"${ProjDirPath}/../tx_user.h"
"${ProjDirPath}/../fx_user.h"
"${ProjDirPath}/../nx_user.h"
"${ProjDirPath}/../ux_hcd_interrupt_handler.c"
"${ProjDirPath}/../ux_user.h"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
"${ProjDirPath}/../dcd.c"
"${ProjDirPath}/../dcd.h"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${ProjDirPath}/../../../../../../middleware/usb/include
    ${ProjDirPath}/../../../../../../middleware/usb/phy
    ${ProjDirPath}/../../../../../../components/osa
    ${ProjDirPath}/..
)

set(CMAKE_MODULE_PATH
    ${ProjDirPath}/../../../../../../rtos/azure-rtos
    ${ProjDirPath}/../../../../../../components/lists
    ${ProjDirPath}/../../../../../../components/uart
    ${ProjDirPath}/../../../../../../components/serial_manager
    ${ProjDirPath}/../../../../../../devices/MIMXRT1176/drivers
    ${ProjDirPath}/../../../../../../devices/MIMXRT1176
    ${ProjDirPath}/../../../../../../devices/MIMXRT1176/drivers/cm7
    ${ProjDirPath}/../../../../xip
    ${ProjDirPath}/../../../../../../devices/MIMXRT1176/xip
    ${ProjDirPath}/../../../../../../devices/MIMXRT1176/utilities
    ${ProjDirPath}/../../../../../../CMSIS/Core/Include
)

# include modules
include(middleware_azure_rtos_ux_MIMXRT1176_cm7)

include(component_lists_MIMXRT1176_cm7)

include(component_lpuart_adapter_MIMXRT1176_cm7)

include(component_serial_manager_MIMXRT1176_cm7)

include(driver_common_MIMXRT1176_cm7)

include(component_serial_manager_uart_MIMXRT1176_cm7)

include(driver_lpuart_MIMXRT1176_cm7)

include(device_MIMXRT1176_CMSIS_MIMXRT1176_cm7)

include(device_MIMXRT1176_startup_MIMXRT1176_cm7)

include(driver_cache_armv7_m7_MIMXRT1176_cm7)

include(driver_clock_MIMXRT1176_cm7)

include(driver_dcdc_soc_MIMXRT1176_cm7)

include(driver_igpio_MIMXRT1176_cm7)

include(driver_iomuxc_MIMXRT1176_cm7)

include(driver_lpi2c_MIMXRT1176_cm7)

include(driver_pmu_1_MIMXRT1176_cm7)

include(driver_xip_board_evkmimxrt1170_MIMXRT1176_cm7)

include(driver_xip_device_MIMXRT1176_cm7)

include(utility_assert_MIMXRT1176_cm7)

include(utility_debug_console_MIMXRT1176_cm7)

include(middleware_azure_rtos_ux_template_MIMXRT1176_cm7)

include(middleware_azure_rtos_nxd_MIMXRT1176_cm7)

include(middleware_azure_rtos_nxd_template_MIMXRT1176_cm7)

include(middleware_azure_rtos_fx_MIMXRT1176_cm7)

include(middleware_azure_rtos_fx_template_MIMXRT1176_cm7)

include(middleware_azure_rtos_tx_MIMXRT1176_cm7)

include(middleware_azure_rtos_tx_template_MIMXRT1176_cm7)

include(utilities_misc_utilities_MIMXRT1176_cm7)

include(driver_anatop_ai_MIMXRT1176_cm7)

include(CMSIS_Include_core_cm_MIMXRT1176_cm7)

include(device_MIMXRT1176_system_MIMXRT1176_cm7)


add_subdirectory(${ProjDirPath}/../../../../../../rtos/azure-rtos/threadx ./CMakeFiles/libthreadx)

add_subdirectory(${ProjDirPath}/../../../../../../rtos/azure-rtos/filex ./CMakeFiles/libfilex)

add_subdirectory(${ProjDirPath}/../../../../../../rtos/azure-rtos/netxduo ./CMakeFiles/libnetxduo)

add_subdirectory(${ProjDirPath}/../../../../../../rtos/azure-rtos/usbx ./CMakeFiles/libusbx)

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE m)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE c)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE gcc)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE nosys)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE "azrtos::usbx")

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE "azrtos::netxduo")

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE "azrtos::filex")

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE "azrtos::threadx")

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)


