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


project(combine_usbx_netxduo_filex)

set(MCUX_BUILD_TYPES flexspi_nor_sdram_debug flexspi_nor_sdram_release sdram_debug sdram_release)

set(MCUX_SDK_PROJECT_NAME combine_usbx_netxduo_filex.elf)

if (NOT DEFINED SdkRootDirPath)
    SET(SdkRootDirPath ${ProjDirPath}/../../../../..)
endif()

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
"${ProjDirPath}/../usbx_device_controllers/ux_dcd_nxp_dci.h"
"${ProjDirPath}/../usbx_device_controllers/ux_dcd_nxp_dci_callback.c"
"${ProjDirPath}/../usbx_device_controllers/ux_dcd_nxp_dci_control_callback.c"
"${ProjDirPath}/../usbx_device_controllers/ux_dcd_nxp_dci_endpoint_create.c"
"${ProjDirPath}/../usbx_device_controllers/ux_dcd_nxp_dci_endpoint_destroy.c"
"${ProjDirPath}/../usbx_device_controllers/ux_dcd_nxp_dci_endpoint_reset.c"
"${ProjDirPath}/../usbx_device_controllers/ux_dcd_nxp_dci_endpoint_stall.c"
"${ProjDirPath}/../usbx_device_controllers/ux_dcd_nxp_dci_endpoint_status.c"
"${ProjDirPath}/../usbx_device_controllers/ux_dcd_nxp_dci_frame_number_get.c"
"${ProjDirPath}/../usbx_device_controllers/ux_dcd_nxp_dci_function.c"
"${ProjDirPath}/../usbx_device_controllers/ux_dcd_nxp_dci_initialize.c"
"${ProjDirPath}/../usbx_device_controllers/ux_dcd_nxp_dci_initialize_complete.c"
"${ProjDirPath}/../usbx_device_controllers/ux_dcd_nxp_dci_transfer_callback.c"
"${ProjDirPath}/../usbx_device_controllers/ux_dcd_nxp_dci_transfer_request.c"
"${ProjDirPath}/../usbx_device_controllers/ux_dcd_nxp_dci_uninitialize.c"
"${ProjDirPath}/../board_setup.c"
"${ProjDirPath}/../board_setup.h"
"${ProjDirPath}/../usb_device_config.h"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../combine_usbx_netxduo_filex.c"
"${ProjDirPath}/../tx_user.h"
"${ProjDirPath}/../fx_user.h"
"${ProjDirPath}/../nx_driver_imxrt_irq.c"
"${ProjDirPath}/../nx_driver_imxrt.c"
"${ProjDirPath}/../nx_driver_imxrt.h"
"${ProjDirPath}/../nx_user.h"
"${ProjDirPath}/../ux_user.h"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
"${ProjDirPath}/../dcd.c"
"${ProjDirPath}/../dcd.h"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${ProjDirPath}/../usbx_device_controllers
    ${ProjDirPath}/..
)

set(CMAKE_MODULE_PATH
    ${SdkRootDirPath}/rtos/azure-rtos
    ${SdkRootDirPath}/devices/MIMXRT1021/drivers
    ${SdkRootDirPath}/components/phy/device/phyksz8081
    ${SdkRootDirPath}/components/osa
    ${SdkRootDirPath}/middleware/usb
    ${SdkRootDirPath}/devices/MIMXRT1021
    ${SdkRootDirPath}/devices/MIMXRT1021/utilities
    ${SdkRootDirPath}/components/uart
    ${SdkRootDirPath}/components/serial_manager
    ${SdkRootDirPath}/components/lists
    ${SdkRootDirPath}/devices/MIMXRT1021/xip
    ${SdkRootDirPath}/boards/evkmimxrt1020/xip
    ${SdkRootDirPath}/CMSIS/Core/Include
    ${SdkRootDirPath}/components/phy
)

# include modules
include(middleware_azure_rtos_ux_MIMXRT1021)

include(driver_enet_MIMXRT1021)

include(driver_phy-device-ksz8081_MIMXRT1021)

include(component_osa_thread_MIMXRT1021)

include(middleware_usb_device_controller_driver_MIMXRT1021)

include(component_osa_MIMXRT1021)

include(middleware_usb_device_ehci_MIMXRT1021)

include(middleware_usb_phy_MIMXRT1021)

include(driver_clock_MIMXRT1021)

include(driver_common_MIMXRT1021)

include(device_MIMXRT1021_CMSIS_MIMXRT1021)

include(utility_debug_console_MIMXRT1021)

include(component_lpuart_adapter_MIMXRT1021)

include(component_serial_manager_MIMXRT1021)

include(component_lists_MIMXRT1021)

include(component_serial_manager_uart_MIMXRT1021)

include(driver_lpuart_MIMXRT1021)

include(device_MIMXRT1021_startup_MIMXRT1021)

include(driver_iomuxc_MIMXRT1021)

include(utility_assert_MIMXRT1021)

include(driver_igpio_MIMXRT1021)

include(driver_xip_device_MIMXRT1021)

include(driver_xip_board_evkmimxrt1020_MIMXRT1021)

include(middleware_azure_rtos_ux_template_MIMXRT1021)

include(middleware_azure_rtos_nxd_MIMXRT1021)

include(middleware_azure_rtos_nxd_template_MIMXRT1021)

include(middleware_azure_rtos_fx_MIMXRT1021)

include(middleware_azure_rtos_fx_template_MIMXRT1021)

include(middleware_azure_rtos_tx_MIMXRT1021)

include(middleware_azure_rtos_tx_template_MIMXRT1021)

include(CMSIS_Include_core_cm_MIMXRT1021)

include(driver_phy-common_MIMXRT1021)

include(middleware_usb_common_header_MIMXRT1021)

include(middleware_usb_device_common_header_MIMXRT1021)

include(utilities_misc_utilities_MIMXRT1021)

include(device_MIMXRT1021_system_MIMXRT1021)


add_subdirectory(${SdkRootDirPath}/rtos/azure-rtos/usbx ./CMakeFiles/libusbx)

add_subdirectory(${SdkRootDirPath}/rtos/azure-rtos/netxduo ./CMakeFiles/libnetxduo)

add_subdirectory(${SdkRootDirPath}/rtos/azure-rtos/filex ./CMakeFiles/libfilex)

add_subdirectory(${SdkRootDirPath}/rtos/azure-rtos/threadx ./CMakeFiles/libthreadx)

IF(NOT DEFINED TARGET_LINK_SYSTEM_LIBRARIES)  
    SET(TARGET_LINK_SYSTEM_LIBRARIES "-lm -lc -lgcc -lnosys")  
ENDIF()  

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE "azrtos::usbx")
target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE "azrtos::netxduo")
target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE "azrtos::filex")
target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE "azrtos::threadx")

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${TARGET_LINK_SYSTEM_LIBRARIES})

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)


