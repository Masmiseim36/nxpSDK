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


project(wireless_uart_bridge)

set(MCUX_SDK_PROJECT_NAME wireless_uart_bridge.elf)

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${ProjDirPath}/../app_config.c"
"${ProjDirPath}/../app_config.h"
"${ProjDirPath}/../FreeRTOSConfig.h"
"${ProjDirPath}/../rpmsg_config.h"
"${ProjDirPath}/../wireless_uart_bridge.c"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../ble/ApplMain.c"
"${ProjDirPath}/../ble/ApplMain.h"
"${ProjDirPath}/../ble/ble_conn_manager.c"
"${ProjDirPath}/../ble/ble_conn_manager.h"
"${ProjDirPath}/../ble/ble_service_discovery.c"
"${ProjDirPath}/../ble/ble_service_discovery.h"
"${ProjDirPath}/../ble/ble_init.h"
"${ProjDirPath}/../ble/framework/Common/EmbeddedTypes.h"
"${ProjDirPath}/../ble/framework/FSCI/Interface/FsciInterface.h"
"${ProjDirPath}/../ble/framework/FSCI/Source/FsciCommands.c"
"${ProjDirPath}/../ble/framework/FSCI/Source/FsciCommands.h"
"${ProjDirPath}/../ble/framework/FSCI/Source/FsciCommunication.c"
"${ProjDirPath}/../ble/framework/FSCI/Source/FsciCommunication.h"
"${ProjDirPath}/../ble/framework/FSCI/Source/FsciLogging.c"
"${ProjDirPath}/../ble/framework/FSCI/Source/FsciMain.c"
"${ProjDirPath}/../ble/framework/FunctionLib/FunctionLib.c"
"${ProjDirPath}/../ble/framework/FunctionLib/FunctionLib.h"
"${ProjDirPath}/../ble/framework/Lists/GenericList.h"
"${ProjDirPath}/../ble/framework/Lists/GenericList.c"
"${ProjDirPath}/../ble/framework/MemManager/Interface/MemManager.h"
"${ProjDirPath}/../ble/framework/MemManager/Source/MemManager.c"
"${ProjDirPath}/../ble/framework/Messaging/Source/Messaging.c"
"${ProjDirPath}/../ble/framework/Messaging/Interface/Messaging.h"
"${ProjDirPath}/../ble/framework/OSAbstraction/Source/fsl_os_abstraction_free_rtos.c"
"${ProjDirPath}/../ble/framework/OSAbstraction/Interface/fsl_os_abstraction.h"
"${ProjDirPath}/../ble/framework/OSAbstraction/Interface/fsl_os_abstraction_config.h"
"${ProjDirPath}/../ble/framework/OSAbstraction/Interface/fsl_os_abstraction_free_rtos.h"
"${ProjDirPath}/../ble/framework/Panic/Source/Panic.c"
"${ProjDirPath}/../ble/framework/Panic/Interface/Panic.h"
"${ProjDirPath}/../ble/framework/SerialManager/Interface/SerialManager.h"
"${ProjDirPath}/../ble/framework/SerialManager/Source/SerialManager.c"
"${ProjDirPath}/../ble/framework/SerialManager/Source/UART_Adapter.c"
"${ProjDirPath}/../ble/framework/SerialManager/Source/UART_Adapter.h"
"${ProjDirPath}/../ble/gatt_db/gatt_database.c"
"${ProjDirPath}/../ble/gatt_db/gatt_database_dynamic.c"
"${ProjDirPath}/../ble/gatt_db/gatt_database_dynamic.h"
"${ProjDirPath}/../ble/fsci_host/fsci/fsci_ble.c"
"${ProjDirPath}/../ble/fsci_host/fsci/fsci_ble.h"
"${ProjDirPath}/../ble/fsci_host/fsci/fsci_ble_types.c"
"${ProjDirPath}/../ble/fsci_host/fsci/fsci_ble_types.h"
"${ProjDirPath}/../ble/fsci_host/fsci/fsci_ble_gap.c"
"${ProjDirPath}/../ble/fsci_host/fsci/fsci_ble_gap.h"
"${ProjDirPath}/../ble/fsci_host/fsci/fsci_ble_gap_types.c"
"${ProjDirPath}/../ble/fsci_host/fsci/fsci_ble_gap_types.h"
"${ProjDirPath}/../ble/fsci_host/fsci/fsci_ble_gatt.c"
"${ProjDirPath}/../ble/fsci_host/fsci/fsci_ble_gatt.h"
"${ProjDirPath}/../ble/fsci_host/fsci/fsci_ble_gatt_types.c"
"${ProjDirPath}/../ble/fsci_host/fsci/fsci_ble_gatt_types.h"
"${ProjDirPath}/../ble/fsci_host/fsci/fsci_ble_gatt_db_app.c"
"${ProjDirPath}/../ble/fsci_host/fsci/fsci_ble_gatt_db_app.h"
"${ProjDirPath}/../ble/fsci_host/fsci/fsci_ble_gatt_db_app_types.c"
"${ProjDirPath}/../ble/fsci_host/fsci/fsci_ble_gatt_db_app_types.h"
"${ProjDirPath}/../ble/fsci_host/fsci/fsci_ble_l2cap.c"
"${ProjDirPath}/../ble/fsci_host/fsci/fsci_ble_l2cap.h"
"${ProjDirPath}/../ble/fsci_host/fsci/fsci_ble_l2cap_types.c"
"${ProjDirPath}/../ble/fsci_host/fsci/fsci_ble_l2cap_types.h"
"${ProjDirPath}/../ble/fsci_host/host/host_ble.c"
"${ProjDirPath}/../ble/fsci_host/host/host_ble.h"
"${ProjDirPath}/../ble/fsci_host/host/host_gap.c"
"${ProjDirPath}/../ble/fsci_host/host/host_gatt.c"
"${ProjDirPath}/../ble/fsci_host/host/host_gatt_db.c"
"${ProjDirPath}/../ble/fsci_host/host/host_l2cap.c"
"${ProjDirPath}/../ble/fsci_host/host/config/ble_config.h"
"${ProjDirPath}/../ble/fsci_host/host/config/ble_globals.c"
"${ProjDirPath}/../ble/fsci_host/host/interface/att_callbacks.h"
"${ProjDirPath}/../ble/fsci_host/host/interface/att_errors.h"
"${ProjDirPath}/../ble/fsci_host/host/interface/att_interface.h"
"${ProjDirPath}/../ble/fsci_host/host/interface/att_params.h"
"${ProjDirPath}/../ble/fsci_host/host/interface/att_types.h"
"${ProjDirPath}/../ble/fsci_host/host/interface/ble_constants.h"
"${ProjDirPath}/../ble/fsci_host/host/interface/ble_general.h"
"${ProjDirPath}/../ble/fsci_host/host/interface/ble_host_tasks.h"
"${ProjDirPath}/../ble/fsci_host/host/interface/ble_sig_defines.h"
"${ProjDirPath}/../ble/fsci_host/host/interface/ble_utils.h"
"${ProjDirPath}/../ble/fsci_host/host/interface/fsci_ble_interface.h"
"${ProjDirPath}/../ble/fsci_host/host/interface/gap_interface.h"
"${ProjDirPath}/../ble/fsci_host/host/interface/gap_types.h"
"${ProjDirPath}/../ble/fsci_host/host/interface/gatt_client_interface.h"
"${ProjDirPath}/../ble/fsci_host/host/interface/gatt_database.h"
"${ProjDirPath}/../ble/fsci_host/host/interface/gatt_db_app_interface.h"
"${ProjDirPath}/../ble/fsci_host/host/interface/gatt_db_att_interface.h"
"${ProjDirPath}/../ble/fsci_host/host/interface/gatt_db_dynamic.h"
"${ProjDirPath}/../ble/fsci_host/host/interface/gatt_interface.h"
"${ProjDirPath}/../ble/fsci_host/host/interface/gatt_server_interface.h"
"${ProjDirPath}/../ble/fsci_host/host/interface/gatt_types.h"
"${ProjDirPath}/../ble/fsci_host/host/interface/hci_interface.h"
"${ProjDirPath}/../ble/fsci_host/host/interface/hci_types.h"
"${ProjDirPath}/../ble/fsci_host/host/interface/l2ca_cb_interface.h"
"${ProjDirPath}/../ble/fsci_host/host/interface/l2ca_interface.h"
"${ProjDirPath}/../ble/fsci_host/host/interface/l2ca_types.h"
"${ProjDirPath}/../ble/fsci_host/host/interface/vendor_debug_commands.h"
"${ProjDirPath}/../ble/profiles/device_info/device_info_interface.h"
"${ProjDirPath}/../ble/profiles/device_info/device_info_service.c"
"${ProjDirPath}/../ble/profiles/wireless_uart/wireless_uart_interface.h"
"${ProjDirPath}/../ble/profiles/wireless_uart/wireless_uart_service.c"
"${ProjDirPath}/../ble/profiles/battery/battery_interface.h"
"${ProjDirPath}/../ble/profiles/battery/battery_service.c"
"${ProjDirPath}/../rsc_table.c"
"${ProjDirPath}/../rsc_table.h"
"${ProjDirPath}/../remoteproc.h"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
"${ProjDirPath}/../fsl_iomuxc.h"
"${ProjDirPath}/../app_srtm.c"
"${ProjDirPath}/../app_srtm.h"
"${ProjDirPath}/../srtm/channels/srtm_rpmsg_endpoint.h"
"${ProjDirPath}/../srtm/channels/srtm_rpmsg_endpoint.c"
"${ProjDirPath}/../srtm/services/srtm_lfcl_service.h"
"${ProjDirPath}/../srtm/services/srtm_lfcl_service.c"
"${ProjDirPath}/../srtm/services/srtm_pmic_service.h"
"${ProjDirPath}/../srtm/services/srtm_pmic_service.c"
"${ProjDirPath}/../srtm/services/srtm_pf1550_adapter.h"
"${ProjDirPath}/../srtm/services/srtm_pf1550_adapter.c"
"${ProjDirPath}/../srtm/services/srtm_audio_service.h"
"${ProjDirPath}/../srtm/services/srtm_audio_service.c"
"${ProjDirPath}/../srtm/services/srtm_sai_edma_adapter.h"
"${ProjDirPath}/../srtm/services/srtm_sai_edma_adapter.c"
"${ProjDirPath}/../srtm/services/srtm_i2c_codec_adapter.h"
"${ProjDirPath}/../srtm/services/srtm_i2c_codec_adapter.c"
"${ProjDirPath}/../srtm/services/srtm_io_service.h"
"${ProjDirPath}/../srtm/services/srtm_io_service.c"
"${ProjDirPath}/../srtm/services/srtm_keypad_service.h"
"${ProjDirPath}/../srtm/services/srtm_keypad_service.c"
"${ProjDirPath}/../srtm/services/srtm_rtc_service.h"
"${ProjDirPath}/../srtm/services/srtm_rtc_service.c"
"${ProjDirPath}/../srtm/services/srtm_snvs_lp_rtc_adapter.h"
"${ProjDirPath}/../srtm/services/srtm_snvs_lp_rtc_adapter.c"
"${ProjDirPath}/../srtm/services/srtm_sensor_service.h"
"${ProjDirPath}/../srtm/services/srtm_sensor_service.c"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${ProjDirPath}/.. release
    ${ProjDirPath}/..
    ${ProjDirPath}/../ble
    ${ProjDirPath}/../ble/framework/Common
    ${ProjDirPath}/../ble/framework/FSCI/Interface
    ${ProjDirPath}/../ble/framework/FSCI/Source
    ${ProjDirPath}/../ble/framework/FunctionLib
    ${ProjDirPath}/../ble/framework/Lists
    ${ProjDirPath}/../ble/framework/MemManager/Interface
    ${ProjDirPath}/../ble/framework/Messaging/Interface
    ${ProjDirPath}/../ble/framework/OSAbstraction/Interface
    ${ProjDirPath}/../ble/framework/Panic/Interface
    ${ProjDirPath}/../ble/framework/SerialManager/Interface
    ${ProjDirPath}/../ble/framework/SerialManager/Source
    ${ProjDirPath}/../ble/gatt_db
    ${ProjDirPath}/../ble/fsci_host/fsci
    ${ProjDirPath}/../ble/fsci_host/host
    ${ProjDirPath}/../ble/fsci_host/host/config
    ${ProjDirPath}/../ble/fsci_host/host/interface
    ${ProjDirPath}/../ble/profiles/device_info
    ${ProjDirPath}/../ble/profiles/wireless_uart
    ${ProjDirPath}/../ble/profiles/battery
    ${ProjDirPath}/../srtm/channels
    ${ProjDirPath}/../srtm/services
)

set(CMAKE_MODULE_PATH
    ${ProjDirPath}/../../../../../components/codec/i2c
    ${ProjDirPath}/../../../../../devices/MCIMX7U5/drivers
    ${ProjDirPath}/../../../../../components/i2c
    ${ProjDirPath}/../../../../../components/codec
    ${ProjDirPath}/../../../../../components/codec/port/wm8960
    ${ProjDirPath}/../../../../../devices/MCIMX7U5
    ${ProjDirPath}/../../../../../devices/MCIMX7U5/utilities
    ${ProjDirPath}/../../../../../components/uart
    ${ProjDirPath}/../../../../../components/serial_manager
    ${ProjDirPath}/../../../../../components/lists
    ${ProjDirPath}/../../../../../CMSIS/Include
    ${ProjDirPath}/../../../../../rtos/freertos
    ${ProjDirPath}/../../../../../components/pf1550
    ${ProjDirPath}/../../../../../components/codec/wm8960
    ${ProjDirPath}/../../../../../components/srtm/port
    ${ProjDirPath}/../../../../../components/srtm
    ${ProjDirPath}/../../../../../middleware/multicore
    ${ProjDirPath}/../../../../../middleware/issdk
    ${ProjDirPath}/../../../../../CMSIS/Driver/Include
)

# include modules
include(component_codec_i2c_MCIMX7U5)

include(driver_common_MCIMX7U5)

include(component_lpi2c_adapter_MCIMX7U5)

include(driver_codec_MCIMX7U5)

include(component_wm8960_adapter_MCIMX7U5)

include(driver_lptmr_MCIMX7U5)

include(driver_clock_MCIMX7U5)

include(device_MCIMX7U5_CMSIS_MCIMX7U5)

include(utility_debug_console_MCIMX7U5)

include(component_lpuart_adapter_MCIMX7U5)

include(component_serial_manager_MCIMX7U5)

include(component_lists_MCIMX7U5)

include(component_serial_manager_uart_MCIMX7U5)

include(driver_lpuart_MCIMX7U5)

include(device_MCIMX7U5_startup_MCIMX7U5)

include(driver_gpio_MCIMX7U5)

include(driver_msmc_MCIMX7U5)

include(utility_assert_MCIMX7U5)

include(driver_dmamux_MCIMX7U5)

include(CMSIS_Include_core_cm4_MCIMX7U5)

include(CMSIS_Include_common_MCIMX7U5)

include(CMSIS_Include_dsp_MCIMX7U5)

include(driver_edma_MCIMX7U5)

include(driver_lpi2c_MCIMX7U5)

include(driver_lpi2c_freertos_MCIMX7U5)

include(middleware_freertos-kernel_MCIMX7U5)

include(middleware_freertos-kernel_extension_MCIMX7U5)

include(driver_pmc0_MCIMX7U5)

include(driver_port_MCIMX7U5)

include(driver_llwu_MCIMX7U5)

include(driver_snvs_lp_MCIMX7U5)

include(driver_mu_MCIMX7U5)

include(driver_pf1550_MCIMX7U5)

include(driver_sai_MCIMX7U5)

include(driver_sai_edma_MCIMX7U5)

include(driver_wm8960_MCIMX7U5)

include(driver_srtm_freertos_MCIMX7U5)

include(driver_srtm_MCIMX7U5)

include(middleware_multicore_rpmsg_lite_imx7ulp_m4_freertos_MCIMX7U5)

include(middleware_multicore_rpmsg_lite_MCIMX7U5)

include(middleware_freertos-kernel_heap_4_MCIMX7U5)

include(middleware_issdk_sensor_allregdefs_MCIMX7U5)

include(CMSIS_Driver_Include_I2C_MCIMX7U5)

include(CMSIS_Driver_Include_Common_MCIMX7U5)

include(CMSIS_Driver_Include_SPI_MCIMX7U5)

include(middleware_issdk_algorithms_pedometer_lib_cm4_MCIMX7U5)

include(middleware_issdk_algorithms_pedometer_common_MCIMX7U5)

include(utilities_misc_utilities_MCIMX7U5)


TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE m)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE c)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE gcc)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE nosys)

if(CMAKE_BUILD_TYPE STREQUAL release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../middleware/issdk/algorithms/pedometer/lib/libpedometerm4.a)
endif()

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)

ADD_CUSTOM_COMMAND(TARGET ${MCUX_SDK_PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_OBJCOPY}
-Obinary ${EXECUTABLE_OUTPUT_PATH}/${MCUX_SDK_PROJECT_NAME} ${EXECUTABLE_OUTPUT_PATH}/wireless_uart_bridge.bin)

