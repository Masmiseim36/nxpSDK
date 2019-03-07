INCLUDE(CMakeForceCompiler)

# CROSS COMPILER SETTING
SET(CMAKE_SYSTEM_NAME Generic)
CMAKE_MINIMUM_REQUIRED (VERSION 2.6)

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


SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -DNDEBUG")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -D__STARTUP_CLEAR_BSS")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -DNO_CRP")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -Wall")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -fno-common")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -ffunction-sections")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -fdata-sections")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -ffreestanding")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -fno-builtin")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -mthumb")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -mapcs")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -std=gnu99")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -mcpu=cortex-m4")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -mfloat-abi=hard")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -mfpu=fpv4-sp-d16")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DNDEBUG")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DCPU_MCIMX7U5DVP08")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DENABLE_RAM_VECTOR_TABLE")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DFSL_RTOS_FREE_RTOS")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DSDK_I2C_BASED_COMPONENT_USED=1")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DBOARD_USE_CODEC=1")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Os")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Wall")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -fno-common")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -ffunction-sections")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -fdata-sections")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -ffreestanding")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -fno-builtin")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -mthumb")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -mapcs")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -std=gnu99")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -mcpu=cortex-m4")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -mfloat-abi=hard")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -mfpu=fpv4-sp-d16")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -MMD")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -MP")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DNDEBUG")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DCPU_MCIMX7U5DVP08")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Os")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fno-common")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -ffunction-sections")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fdata-sections")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -ffreestanding")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fno-builtin")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -mthumb")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -mapcs")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fno-rtti")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fno-exceptions")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -mcpu=cortex-m4")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -mfloat-abi=hard")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -mfpu=fpv4-sp-d16")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -MMD")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -MP")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} --specs=nano.specs")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} --specs=nosys.specs")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -Wall")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -fno-common")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -ffunction-sections")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -fdata-sections")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -ffreestanding")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -fno-builtin")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -mthumb")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -mapcs")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} --gc-sections")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -static")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -z")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} muldefs")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -mcpu=cortex-m4")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -mfloat-abi=hard")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -mfpu=fpv4-sp-d16")

include_directories(${ProjDirPath}/..)

include_directories(${ProjDirPath}/../../..)

include_directories(${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/FreeRTOS/portable/GCC/ARM_CM4F)

include_directories(${ProjDirPath}/../ble)

include_directories(${ProjDirPath}/../ble/framework/Common)

include_directories(${ProjDirPath}/../ble/framework/FunctionLib)

include_directories(${ProjDirPath}/../ble/framework/Lists)

include_directories(${ProjDirPath}/../ble/framework/Panic/Interface)

include_directories(${ProjDirPath}/../ble/framework/OSAbstraction/Interface)

include_directories(${ProjDirPath}/../ble/framework/FSCI/Interface)

include_directories(${ProjDirPath}/../ble/framework/FSCI/Source)

include_directories(${ProjDirPath}/../ble/framework/Messaging/Interface)

include_directories(${ProjDirPath}/../ble/framework/MemManager/Interface)

include_directories(${ProjDirPath}/../ble/framework/SerialManager/Interface)

include_directories(${ProjDirPath}/../ble/framework/SerialManager/Source)

include_directories(${ProjDirPath}/../ble/fsci_host/fsci)

include_directories(${ProjDirPath}/../ble/fsci_host/host)

include_directories(${ProjDirPath}/../ble/fsci_host/host/config)

include_directories(${ProjDirPath}/../ble/fsci_host/host/interface)

include_directories(${ProjDirPath}/../ble/gatt_db)

include_directories(${ProjDirPath}/../ble/profiles/wireless_uart)

include_directories(${ProjDirPath}/../ble/profiles/battery)

include_directories(${ProjDirPath}/../ble/profiles/device_info)

include_directories(${ProjDirPath}/../../..)

include_directories(${ProjDirPath}/..)

include_directories(${ProjDirPath}/../../../../../CMSIS/Include)

include_directories(${ProjDirPath}/../../../../../devices)

include_directories(${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include)

include_directories(${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include/private)

include_directories(${ProjDirPath}/../srtm/channels)

include_directories(${ProjDirPath}/../srtm/services)

include_directories(${ProjDirPath}/../../../../../devices/MCIMX7U5/drivers)

include_directories(${ProjDirPath}/../../../../../devices/MCIMX7U5)

include_directories(${ProjDirPath}/../../../../../devices/MCIMX7U5/utilities/str)

include_directories(${ProjDirPath}/../../../../../devices/MCIMX7U5/utilities)

include_directories(${ProjDirPath}/../../../../../components/uart)

include_directories(${ProjDirPath}/../../../../../components/serial_manager)

include_directories(${ProjDirPath}/../../../../../components/lists)

include_directories(${ProjDirPath}/../../../../../components/pf1550)

include_directories(${ProjDirPath}/../../../../../components/codec/wm8960)

include_directories(${ProjDirPath}/../../../../../components/codec)

include_directories(${ProjDirPath}/../../../../../components/srtm/include)

include_directories(${ProjDirPath}/../../../../../components/srtm/srtm)

include_directories(${ProjDirPath}/../../../../../middleware/multicore/rpmsg_lite/lib/include/platform/imx7ulp_m4)

include_directories(${ProjDirPath}/../../../../../middleware/multicore/rpmsg_lite/lib/include)

include_directories(${ProjDirPath}/../../../../../middleware/issdk/sensors)

include_directories(${ProjDirPath}/../../../../../CMSIS/Driver/Include)

include_directories(${ProjDirPath}/../../../../../middleware/issdk/algorithms/pedometer/source)

include_directories(${ProjDirPath}/../../../../../middleware/issdk/algorithms/pedometer/include)

include_directories(${ProjDirPath}/../../../../../middleware/issdk/algorithms/pedometer/include/libinclude)

add_executable(wireless_uart_bridge.elf 
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
"${ProjDirPath}/../../../../../devices/MCIMX7U5/drivers/fsl_lptmr.c"
"${ProjDirPath}/../../../../../devices/MCIMX7U5/drivers/fsl_lptmr.h"
"${ProjDirPath}/../../../../../devices/MCIMX7U5/drivers/fsl_clock.h"
"${ProjDirPath}/../../../../../devices/MCIMX7U5/drivers/fsl_clock.c"
"${ProjDirPath}/../../../../../devices/MCIMX7U5/drivers/fsl_common.h"
"${ProjDirPath}/../../../../../devices/MCIMX7U5/drivers/fsl_common.c"
"${ProjDirPath}/../../../../../devices/MCIMX7U5/fsl_device_registers.h"
"${ProjDirPath}/../../../../../devices/MCIMX7U5/MCIMX7U5_cm4.h"
"${ProjDirPath}/../../../../../devices/MCIMX7U5/MCIMX7U5_cm4_features.h"
"${ProjDirPath}/../../../../../devices/MCIMX7U5/system_MCIMX7U5_cm4.c"
"${ProjDirPath}/../../../../../devices/MCIMX7U5/system_MCIMX7U5_cm4.h"
"${ProjDirPath}/../../../../../devices/MCIMX7U5/utilities/str/fsl_str.c"
"${ProjDirPath}/../../../../../devices/MCIMX7U5/utilities/str/fsl_str.h"
"${ProjDirPath}/../../../../../devices/MCIMX7U5/utilities/fsl_debug_console.c"
"${ProjDirPath}/../../../../../devices/MCIMX7U5/utilities/fsl_debug_console.h"
"${ProjDirPath}/../../../../../devices/MCIMX7U5/utilities/fsl_debug_console_conf.h"
"${ProjDirPath}/../../../../../components/uart/uart.h"
"${ProjDirPath}/../../../../../components/uart/lpuart_adapter.c"
"${ProjDirPath}/../../../../../components/serial_manager/serial_manager.h"
"${ProjDirPath}/../../../../../components/serial_manager/serial_manager.c"
"${ProjDirPath}/../../../../../components/serial_manager/serial_port_uart.h"
"${ProjDirPath}/../../../../../components/serial_manager/serial_port_uart.c"
"${ProjDirPath}/../../../../../devices/MCIMX7U5/drivers/fsl_lpuart.h"
"${ProjDirPath}/../../../../../devices/MCIMX7U5/drivers/fsl_lpuart.c"
"${ProjDirPath}/../../../../../components/lists/generic_list.h"
"${ProjDirPath}/../../../../../components/lists/generic_list.c"
"${ProjDirPath}/../../../../../devices/MCIMX7U5/gcc/startup_MCIMX7U5_cm4.S"
"${ProjDirPath}/../../../../../devices/MCIMX7U5/drivers/fsl_gpio.h"
"${ProjDirPath}/../../../../../devices/MCIMX7U5/drivers/fsl_gpio.c"
"${ProjDirPath}/../../../../../devices/MCIMX7U5/drivers/fsl_msmc.h"
"${ProjDirPath}/../../../../../devices/MCIMX7U5/drivers/fsl_msmc.c"
"${ProjDirPath}/../../../../../devices/MCIMX7U5/utilities/fsl_assert.c"
"${ProjDirPath}/../../../../../devices/MCIMX7U5/drivers/fsl_dmamux.h"
"${ProjDirPath}/../../../../../devices/MCIMX7U5/drivers/fsl_dmamux.c"
"${ProjDirPath}/../../../../../CMSIS/Include/core_cm4.h"
"${ProjDirPath}/../../../../../CMSIS/Include/cmsis_armcc.h"
"${ProjDirPath}/../../../../../CMSIS/Include/cmsis_armclang.h"
"${ProjDirPath}/../../../../../CMSIS/Include/cmsis_gcc.h"
"${ProjDirPath}/../../../../../CMSIS/Include/cmsis_compiler.h"
"${ProjDirPath}/../../../../../CMSIS/Include/cmsis_version.h"
"${ProjDirPath}/../../../../../CMSIS/Include/mpu_armv7.h"
"${ProjDirPath}/../../../../../CMSIS/Include/core_armv8mbl.h"
"${ProjDirPath}/../../../../../CMSIS/Include/core_armv8mml.h"
"${ProjDirPath}/../../../../../CMSIS/Include/arm_common_tables.h"
"${ProjDirPath}/../../../../../CMSIS/Include/arm_const_structs.h"
"${ProjDirPath}/../../../../../CMSIS/Include/arm_math.h"
"${ProjDirPath}/../../../../../CMSIS/Include/cmsis_iccarm.h"
"${ProjDirPath}/../../../../../CMSIS/Include/mpu_armv8.h"
"${ProjDirPath}/../../../../../devices/MCIMX7U5/drivers/fsl_edma.h"
"${ProjDirPath}/../../../../../devices/MCIMX7U5/drivers/fsl_edma.c"
"${ProjDirPath}/../../../../../devices/MCIMX7U5/drivers/fsl_lpi2c.c"
"${ProjDirPath}/../../../../../devices/MCIMX7U5/drivers/fsl_lpi2c.h"
"${ProjDirPath}/../../../../../devices/MCIMX7U5/drivers/fsl_lpi2c_freertos.h"
"${ProjDirPath}/../../../../../devices/MCIMX7U5/drivers/fsl_lpi2c_freertos.c"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/LICENSE"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include/event_groups.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include/FreeRTOS.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include/message_buffer.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include/queue.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include/semphr.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include/stream_buffer.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include/task.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include/timers.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include/freertos_tasks_c_additions.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include/private/deprecated_definitions.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include/private/list.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include/private/mpu_wrappers.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include/private/portable.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include/private/projdefs.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/include/private/stack_macros.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/FreeRTOS/event_groups.c"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/FreeRTOS/list.c"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/FreeRTOS/queue.c"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/FreeRTOS/stream_buffer.c"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/FreeRTOS/tasks.c"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/FreeRTOS/timers.c"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/FreeRTOS/portable/GCC/ARM_CM4F/fsl_tickless_generic.h"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/FreeRTOS/portable/GCC/ARM_CM4F/fsl_tickless_systick.c"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/FreeRTOS/portable/GCC/ARM_CM4F/port.c"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/FreeRTOS/portable/GCC/ARM_CM4F/portmacro.h"
"${ProjDirPath}/../../../../../devices/MCIMX7U5/drivers/fsl_pmc0.h"
"${ProjDirPath}/../../../../../devices/MCIMX7U5/drivers/fsl_port.h"
"${ProjDirPath}/../../../../../devices/MCIMX7U5/drivers/fsl_llwu.h"
"${ProjDirPath}/../../../../../devices/MCIMX7U5/drivers/fsl_llwu.c"
"${ProjDirPath}/../../../../../devices/MCIMX7U5/drivers/fsl_snvs_lp.c"
"${ProjDirPath}/../../../../../devices/MCIMX7U5/drivers/fsl_snvs_lp.h"
"${ProjDirPath}/../../../../../devices/MCIMX7U5/drivers/fsl_mu.h"
"${ProjDirPath}/../../../../../devices/MCIMX7U5/drivers/fsl_mu.c"
"${ProjDirPath}/../../../../../components/pf1550/fsl_pf1550.h"
"${ProjDirPath}/../../../../../components/pf1550/fsl_pf1550.c"
"${ProjDirPath}/../../../../../components/pf1550/fsl_pf1550_charger.h"
"${ProjDirPath}/../../../../../components/pf1550/fsl_pf1550_charger.c"
"${ProjDirPath}/../../../../../devices/MCIMX7U5/drivers/fsl_sai.h"
"${ProjDirPath}/../../../../../devices/MCIMX7U5/drivers/fsl_sai.c"
"${ProjDirPath}/../../../../../devices/MCIMX7U5/drivers/fsl_sai_edma.h"
"${ProjDirPath}/../../../../../devices/MCIMX7U5/drivers/fsl_sai_edma.c"
"${ProjDirPath}/../../../../../components/codec/wm8960/fsl_wm8960.h"
"${ProjDirPath}/../../../../../components/codec/wm8960/fsl_wm8960.c"
"${ProjDirPath}/../../../../../components/codec/fsl_codec_common.h"
"${ProjDirPath}/../../../../../components/codec/fsl_codec_common.c"
"${ProjDirPath}/../../../../../components/srtm/port/srtm_heap_freertos.c"
"${ProjDirPath}/../../../../../components/srtm/port/srtm_mutex_freertos.c"
"${ProjDirPath}/../../../../../components/srtm/port/srtm_sem_freertos.c"
"${ProjDirPath}/../../../../../components/srtm/include/srtm_channel.h"
"${ProjDirPath}/../../../../../components/srtm/include/srtm_defs.h"
"${ProjDirPath}/../../../../../components/srtm/include/srtm_dispatcher.h"
"${ProjDirPath}/../../../../../components/srtm/include/srtm_message.h"
"${ProjDirPath}/../../../../../components/srtm/include/srtm_peercore.h"
"${ProjDirPath}/../../../../../components/srtm/include/srtm_service.h"
"${ProjDirPath}/../../../../../components/srtm/srtm/srtm_channel.c"
"${ProjDirPath}/../../../../../components/srtm/srtm/srtm_channel_struct.h"
"${ProjDirPath}/../../../../../components/srtm/srtm/srtm_dispatcher.c"
"${ProjDirPath}/../../../../../components/srtm/srtm/srtm_dispatcher_struct.h"
"${ProjDirPath}/../../../../../components/srtm/srtm/srtm_message.c"
"${ProjDirPath}/../../../../../components/srtm/srtm/srtm_message_struct.h"
"${ProjDirPath}/../../../../../components/srtm/srtm/srtm_peercore.c"
"${ProjDirPath}/../../../../../components/srtm/srtm/srtm_peercore_struct.h"
"${ProjDirPath}/../../../../../components/srtm/srtm/srtm_service.c"
"${ProjDirPath}/../../../../../components/srtm/srtm/srtm_service_struct.h"
"${ProjDirPath}/../../../../../components/srtm/srtm/srtm_heap.h"
"${ProjDirPath}/../../../../../components/srtm/srtm/srtm_message_pool.h"
"${ProjDirPath}/../../../../../components/srtm/srtm/srtm_list.h"
"${ProjDirPath}/../../../../../components/srtm/srtm/srtm_mutex.h"
"${ProjDirPath}/../../../../../components/srtm/srtm/srtm_sem.h"
"${ProjDirPath}/../../../../../components/srtm/port/srtm_message_pool.c"
"${ProjDirPath}/../../../../../middleware/multicore/rpmsg_lite/lib/include/platform/imx7ulp_m4/rpmsg_platform.h"
"${ProjDirPath}/../../../../../middleware/multicore/rpmsg_lite/lib/rpmsg_lite/porting/platform/imx7ulp_m4/rpmsg_platform.c"
"${ProjDirPath}/../../../../../middleware/multicore/rpmsg_lite/lib/rpmsg_lite/porting/environment/rpmsg_env_freertos.c"
"${ProjDirPath}/../../../../../middleware/multicore/rpmsg_lite/lib/include/rpmsg_env.h"
"${ProjDirPath}/../../../../../middleware/multicore/rpmsg_lite/lib/include/rpmsg_queue.h"
"${ProjDirPath}/../../../../../middleware/multicore/rpmsg_lite/lib/rpmsg_lite/rpmsg_queue.c"
"${ProjDirPath}/../../../../../middleware/multicore/rpmsg_lite/lib/include/rpmsg_lite.h"
"${ProjDirPath}/../../../../../middleware/multicore/rpmsg_lite/lib/include/rpmsg_ns.h"
"${ProjDirPath}/../../../../../middleware/multicore/rpmsg_lite/lib/include/llist.h"
"${ProjDirPath}/../../../../../middleware/multicore/rpmsg_lite/lib/include/rpmsg_default_config.h"
"${ProjDirPath}/../../../../../middleware/multicore/rpmsg_lite/lib/include/virtio_ring.h"
"${ProjDirPath}/../../../../../middleware/multicore/rpmsg_lite/lib/include/virtqueue.h"
"${ProjDirPath}/../../../../../middleware/multicore/rpmsg_lite/lib/include/rpmsg_compiler.h"
"${ProjDirPath}/../../../../../middleware/multicore/rpmsg_lite/lib/common/llist.c"
"${ProjDirPath}/../../../../../middleware/multicore/rpmsg_lite/lib/rpmsg_lite/rpmsg_lite.c"
"${ProjDirPath}/../../../../../middleware/multicore/rpmsg_lite/lib/rpmsg_lite/rpmsg_ns.c"
"${ProjDirPath}/../../../../../middleware/multicore/rpmsg_lite/lib/virtio/virtqueue.c"
"${ProjDirPath}/../../../../../rtos/amazon-freertos/lib/FreeRTOS/portable/MemMang/heap_4.c"
"${ProjDirPath}/../../../../../middleware/issdk/sensors/fxas21002.h"
"${ProjDirPath}/../../../../../middleware/issdk/sensors/fxos8700.h"
"${ProjDirPath}/../../../../../middleware/issdk/sensors/mpl3115.h"
"${ProjDirPath}/../../../../../middleware/issdk/sensors/fxlc95000.h"
"${ProjDirPath}/../../../../../middleware/issdk/sensors/fxls8471q.h"
"${ProjDirPath}/../../../../../middleware/issdk/sensors/fxls8962.h"
"${ProjDirPath}/../../../../../middleware/issdk/sensors/fxpq3115.h"
"${ProjDirPath}/../../../../../middleware/issdk/sensors/mag3110.h"
"${ProjDirPath}/../../../../../middleware/issdk/sensors/mma845x.h"
"${ProjDirPath}/../../../../../middleware/issdk/sensors/mma865x.h"
"${ProjDirPath}/../../../../../middleware/issdk/sensors/mma8491q.h"
"${ProjDirPath}/../../../../../middleware/issdk/sensors/mma9553.h"
"${ProjDirPath}/../../../../../middleware/issdk/sensors/diff_p.h"
"${ProjDirPath}/../../../../../CMSIS/Driver/Include/Driver_I2C.h"
"${ProjDirPath}/../../../../../CMSIS/Driver/Include/Driver_Common.h"
"${ProjDirPath}/../../../../../CMSIS/Driver/Include/Driver_SPI.h"
"${ProjDirPath}/../../../../../middleware/issdk/algorithms/pedometer/source/pedometer.c"
"${ProjDirPath}/../../../../../middleware/issdk/algorithms/pedometer/include/pedometer.h"
"${ProjDirPath}/../../../../../middleware/issdk/algorithms/pedometer/include/libinclude/KeynetikPedometer.h"
)


set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -T${ProjDirPath}/MCIMX7U5xxxxx_cm4_ram.ld -static")

TARGET_LINK_LIBRARIES(wireless_uart_bridge.elf -Wl,--start-group)
target_link_libraries(wireless_uart_bridge.elf optimized m)

target_link_libraries(wireless_uart_bridge.elf optimized c)

target_link_libraries(wireless_uart_bridge.elf optimized gcc)

target_link_libraries(wireless_uart_bridge.elf optimized nosys)

link_directories(${ProjDirPath}/../../../../../middleware/issdk/algorithms/pedometer/lib)

target_link_libraries(wireless_uart_bridge.elf optimized ${ProjDirPath}/../../../../../middleware/issdk/algorithms/pedometer/lib/libpedometerm4.a)

TARGET_LINK_LIBRARIES(wireless_uart_bridge.elf -Wl,--end-group)

ADD_CUSTOM_COMMAND(TARGET wireless_uart_bridge.elf POST_BUILD COMMAND ${CMAKE_OBJCOPY}
-Obinary ${EXECUTABLE_OUTPUT_PATH}/wireless_uart_bridge.elf ${EXECUTABLE_OUTPUT_PATH}/sdk20-app.bin)
