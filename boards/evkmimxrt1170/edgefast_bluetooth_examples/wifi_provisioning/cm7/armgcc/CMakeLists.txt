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


project(wifi_provisioning)

set(MCUX_SDK_PROJECT_NAME wifi_provisioning.elf)

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${ProjDirPath}/../app_config.h"
"${ProjDirPath}/../sdmmc_config.c"
"${ProjDirPath}/../sdmmc_config.h"
"${ProjDirPath}/../config_files/FreeRTOSConfig.h"
"${ProjDirPath}/../config_files/aws_mbedtls_config.h"
"${ProjDirPath}/../config_files/aws_demo_config.h"
"${ProjDirPath}/../config_files/iot_secure_sockets_config.h"
"${ProjDirPath}/../config_files/aws_shadow_config.h"
"${ProjDirPath}/../config_files/FreeRTOSIPConfig.h"
"${ProjDirPath}/../config_files/iot_config.h"
"${ProjDirPath}/../config_files/core_mqtt_config.h"
"${ProjDirPath}/../config_files/core_pkcs11_config.h"
"${ProjDirPath}/../config_files/aws_wifi_config.h"
"${ProjDirPath}/../config_files/shadow_config.h"
"${ProjDirPath}/../config_files/shadow_demo_config.h"
"${ProjDirPath}/../ffconf.h"
"${ProjDirPath}/../host_msd_fatfs.c"
"${ProjDirPath}/../host_msd_fatfs.h"
"${ProjDirPath}/../usb_host_config.h"
"${ProjDirPath}/../wifi_config.h"
"${ProjDirPath}/../config_files/aws_iot_network_config.h"
"${ProjDirPath}/../config_files/iot_ble_config.h"
"${ProjDirPath}/../main.c"
"${ProjDirPath}/../aws_iot_demo_shadow.c"
"${ProjDirPath}/../lwipopts.h"
"${ProjDirPath}/../lwippools.h"
"${ProjDirPath}/../db_gen.c"
"${ProjDirPath}/../../aws_iot_wifi_acs_porting_layer/iot_wifi.c"
"${ProjDirPath}/../../aws_iot_bt_acs_porting_layer/appl_utils.c"
"${ProjDirPath}/../../aws_iot_bt_acs_porting_layer/appl_utils.h"
"${ProjDirPath}/../../aws_iot_bt_acs_porting_layer/ble/appl_le.c"
"${ProjDirPath}/../../aws_iot_bt_acs_porting_layer/ble/appl_le.h"
"${ProjDirPath}/../../aws_iot_bt_acs_porting_layer/ble/profile_appl/appl_gatt_client.c"
"${ProjDirPath}/../../aws_iot_bt_acs_porting_layer/ble/profile_appl/appl_gatt_client.h"
"${ProjDirPath}/../../aws_iot_bt_acs_porting_layer/ble/profile_appl/appl_gatt_server.c"
"${ProjDirPath}/../../aws_iot_bt_acs_porting_layer/ble/profile_appl/appl_gatt_server.h"
"${ProjDirPath}/../../aws_iot_bt_acs_porting_layer/ble/profile_appl/appl_service.c"
"${ProjDirPath}/../../aws_iot_bt_acs_porting_layer/ble/profile_appl/appl_service.h"
"${ProjDirPath}/../../aws_iot_bt_acs_porting_layer/ble/profile_appl/appl_service_mops.c"
"${ProjDirPath}/../../aws_iot_bt_acs_porting_layer/ble/profile_appl/appl_service_mops.h"
"${ProjDirPath}/../../aws_iot_bt_acs_porting_layer/ble/profile_appl/gatt.c"
"${ProjDirPath}/../../aws_iot_bt_acs_porting_layer/ble/profile_appl/gatt.h"
"${ProjDirPath}/../../aws_iot_bt_acs_porting_layer/ble/profile_appl/gatt_db.c"
"${ProjDirPath}/../../aws_iot_bt_acs_porting_layer/ble/protocol_appl/gatt_db.h"
"${ProjDirPath}/../../aws_iot_bt_acs_porting_layer/ble/protocol_appl/appl_att_client.c"
"${ProjDirPath}/../../aws_iot_bt_acs_porting_layer/ble/protocol_appl/appl_att_client.h"
"${ProjDirPath}/../../aws_iot_bt_acs_porting_layer/ble/protocol_appl/appl_gap_le.c"
"${ProjDirPath}/../../aws_iot_bt_acs_porting_layer/ble/protocol_appl/appl_gap_le.h"
"${ProjDirPath}/../../aws_iot_bt_acs_porting_layer/ble/protocol_appl/appl_hci_le.c"
"${ProjDirPath}/../../aws_iot_bt_acs_porting_layer/ble/protocol_appl/appl_hci_le.h"
"${ProjDirPath}/../../aws_iot_bt_acs_porting_layer/ble/protocol_appl/appl_hci_le_api.c"
"${ProjDirPath}/../../aws_iot_bt_acs_porting_layer/ble/protocol_appl/appl_l2cap_le.c"
"${ProjDirPath}/../../aws_iot_bt_acs_porting_layer/ble/protocol_appl/appl_l2cap_le.h"
"${ProjDirPath}/../../aws_iot_bt_acs_porting_layer/ble/protocol_appl/appl_smp.c"
"${ProjDirPath}/../../aws_iot_bt_acs_porting_layer/ble/protocol_appl/appl_smp.h"
"${ProjDirPath}/../../aws_iot_bt_acs_porting_layer/br/protocol_appl/appl_hci.c"
"${ProjDirPath}/../../aws_iot_bt_acs_porting_layer/br/protocol_appl/appl_hci.h"
"${ProjDirPath}/../../aws_iot_bt_acs_porting_layer/br/protocol_appl/appl_hci_api.c"
"${ProjDirPath}/../../aws_iot_bt_acs_porting_layer/gatt_db_pl.c"
"${ProjDirPath}/../../aws_iot_bt_acs_porting_layer/gatt_db_pl.h"
"${ProjDirPath}/../../aws_iot_bt_acs_porting_layer/iot_ble_hal_common_gap.c"
"${ProjDirPath}/../../aws_iot_bt_acs_porting_layer/iot_ble_hal_gap.c"
"${ProjDirPath}/../../aws_iot_bt_acs_porting_layer/iot_ble_hal_gatt_server.c"
"${ProjDirPath}/../../aws_iot_bt_acs_porting_layer/iot_ble_hal_internals.h"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../bt_module_config.h"
"${ProjDirPath}/../wifi_bt_config.c"
"${ProjDirPath}/../wifi_bt_config.h"
"${ProjDirPath}/../dcd.c"
"${ProjDirPath}/../dcd.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/export/eOSAL/eBT_assert.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/export/eOSAL/eBT_config.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/export/eOSAL/eBT_debug_api.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/export/eOSAL/eBT_fops.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/export/eOSAL/eBT_os.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/export/eOSAL/eBT_serial.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/export/eOSAL/eBT_timer.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/export/include/BT_a2dp_api.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/export/include/BT_api.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/export/include/BT_assigned_numbers.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/export/include/BT_at_parser_api.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/export/include/BT_att_api.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/export/include/BT_avctp_api.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/export/include/BT_avdtp_api.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/export/include/BT_avrcp_api.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/export/include/BT_avrcp_ca_api.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/export/include/BT_bip_api.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/export/include/BT_bnep_api.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/export/include/BT_bottom_half.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/export/include/BT_bpp_api.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/export/include/BT_common.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/export/include/BT_ctn_api.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/export/include/BT_dbase_api.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/export/include/BT_device_queue.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/export/include/BT_error.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/export/include/BT_ftp_api.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/export/include/BT_gap_api.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/export/include/BT_gatt_db_api.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/export/include/BT_hci_api.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/export/include/BT_hdp_api.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/export/include/BT_hfp_ag_api.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/export/include/BT_hfp_unit_api.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/export/include/BT_hid_device_api.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/export/include/BT_hid_host_api.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/export/include/BT_irobex_api.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/export/include/BT_jpl_api.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/export/include/BT_map_api.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/export/include/BT_mcap_api.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/export/include/BT_obex_api.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/export/include/BT_opp_api.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/export/include/BT_pan_api.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/export/include/BT_pbap_api.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/export/include/BT_racp.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/export/include/BT_rfcomm_api.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/export/include/BT_sap_api.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/export/include/BT_sdp_api.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/export/include/BT_security.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/export/include/BT_sm_api.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/export/include/BT_smp_api.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/export/include/BT_snoop.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/export/include/BT_spp_api.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/export/include/BT_status.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/export/include/BT_storage.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/export/include/BT_version.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/export/include/fsm_defines.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/export/include/fsm_engine.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/export/include/gatt_defines.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/export/include/hci_transport.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/export/include/l2cap.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/export/include/write_task.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/export/vendor/vendor_specific_commands.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/export/vendor/vendor_specific_init.c"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/export/vendor/vendor_specific_init.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/private/protocols/att/att.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/private/protocols/avctp/avctp.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/private/protocols/avdtp/avdtp.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/private/protocols/bnep/bnep.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/private/protocols/dbase/database.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/private/protocols/hci_1.2/hci_h.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/private/protocols/mcap/mcap.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/private/protocols/obex/obex.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/private/protocols/rfcomm/rfcomm.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/private/protocols/sdp/sdp.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/private/protocols/sm/sm.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/private/protocols/sm/sm_extern.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/private/protocols/sm/sm_internal.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/private/protocols/smp/smp.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/private/protocols/smp/smp_extern.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/private/protocols/smp/smp_fsm.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/private/protocols/smp/smp_fsm_engine.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/private/protocols/smp/smp_internal.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/private/utils/aes_cmac/aes_cmac.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/private/utils/at_parser/at_parser_hsp_hfp.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/private/utils/object_parser/object_parser_api.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/private/utils/object_parser/tokenizer.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/private/utils/racp/racp.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/private/utils/storage/storage.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/private/utils/xml_parser/xml_common_defines.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/private/utils/xml_parser/xml_encoder.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/private/utils/xml_parser/xml_parser.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/port/osal/src/freertos/EM_assert.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/port/osal/src/freertos/EM_config.c"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/port/osal/src/freertos/EM_config.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/port/osal/src/freertos/EM_debug.c"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/port/osal/src/freertos/EM_debug.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/port/osal/src/freertos/EM_debug_api.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/port/osal/src/freertos/EM_debug_internal.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/port/osal/src/freertos/EM_fops.c"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/port/osal/src/freertos/EM_fops.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/port/osal/src/freertos/EM_os.c"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/port/osal/src/freertos/EM_os.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/port/osal/src/freertos/EM_serial.c"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/port/osal/src/freertos/EM_serial.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/port/osal/src/freertos/EM_timer.c"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/port/osal/src/freertos/EM_timer.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/port/osal/src/freertos/EM_timer_internal.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/port/pal/mcux/bluetooth/BT_assert.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/port/pal/mcux/bluetooth/BT_config.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/port/pal/mcux/bluetooth/BT_debug.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/port/pal/mcux/bluetooth/BT_debug_api.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/port/pal/mcux/bluetooth/BT_fops.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/port/pal/mcux/bluetooth/BT_os.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/port/pal/mcux/bluetooth/BT_status_pl.c"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/port/pal/mcux/bluetooth/BT_storage_pl.c"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/port/pal/mcux/bluetooth/BT_storage_pl.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/port/pal/mcux/bluetooth/BT_timer.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/port/pal/mcux/bluetooth/btsnoop_pl.c"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/port/pal/mcux/bluetooth/btsnoop_pl.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/port/pal/mcux/bluetooth/ctn_pl.c"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/port/pal/mcux/bluetooth/ctn_pl.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/port/pal/mcux/bluetooth/ftp_pl.c"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/port/pal/mcux/bluetooth/ftp_pl.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/port/pal/mcux/bluetooth/hci_uart.c"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/port/pal/mcux/bluetooth/hci_uart.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/port/pal/mcux/bluetooth/hci_uart_internal.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/port/pal/mcux/bluetooth/ht_read_task_pl.c"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/port/pal/mcux/bluetooth/map_pl.c"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/port/pal/mcux/bluetooth/map_pl.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/port/pal/mcux/bluetooth/obex_pl.c"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/port/pal/mcux/bluetooth/obex_pl.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/port/pal/mcux/bluetooth/pbap_pl.c"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/port/pal/mcux/bluetooth/pbap_pl.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/port/pal/mcux/bluetooth/write_task_extern_pl.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/port/pal/mcux/bluetooth/write_task_pl.c"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/port/pal/mcux/bluetooth/write_task_pl.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/port/pal/mcux/bluetooth/littlefs_pl.c"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/port/pal/mcux/bluetooth/littlefs_pl.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/port/pal/mcux/bluetooth/controller.c"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/port/pal/mcux/bluetooth/controller.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/port/pal/mcux/bluetooth/controller/controller_wifi_nxp.c"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/port/pal/mcux/bluetooth/controller/controller_wifi_nxp.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/port/pal/mcux/ethal/ethal_assert.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/port/pal/mcux/ethal/ethal_config.c"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/port/pal/mcux/ethal/ethal_config.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/port/pal/mcux/ethal/ethal_debug.c"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/port/pal/mcux/ethal/ethal_debug.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/port/pal/mcux/ethal/ethal_debug_api.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/port/pal/mcux/ethal/ethal_debug_internal.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/port/pal/mcux/ethal/ethal_fops.c"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/port/pal/mcux/ethal/ethal_fops.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/port/pal/mcux/ethal/ethal_os.c"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/port/pal/mcux/ethal/ethal_os.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/port/pal/mcux/ethal/ethal_serial.c"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/port/pal/mcux/ethal/ethal_serial.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/port/pal/mcux/ethal/ethal_timer.c"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/port/pal/mcux/ethal/ethal_timer.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/port/pal/mcux/ethal/ethal_timer_internal.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/port/pal/mcux/sbc/sbc_os.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/port/pal/mcux/EM_platform.c"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/port/pal/mcux/EM_platform.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/private/utils/sbc/sbc_api.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/private/lib/mcux/default/config/BT_features.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/private/lib/mcux/default/config/BT_limits.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/private/lib/mcux/default/config/db_gen.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/export/extension/aes_cmac_pl.c"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/export/extension/aes_cmac_pl.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/export/extension/BT_common_pl.c"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/export/extension/BT_common_pl.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/export/extension/device_queue_pl.c"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/export/extension/device_queue_pl.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/export/extension/sm_pl.c"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/export/extension/sm_pl.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/export/extension/sm_ssp_pl.c"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/export/extension/sm_ssp_pl.h"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/export/extension/smp_pl.c"
"${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/export/extension/smp_pl.h"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${ProjDirPath}/..
    ${ProjDirPath}/../config_files
    ${ProjDirPath}/../..
    ${ProjDirPath}/../../aws_iot_wifi_acs_porting_layer
    ${ProjDirPath}/../../aws_iot_bt_acs_porting_layer
    ${ProjDirPath}/../../aws_iot_bt_acs_porting_layer/ble
    ${ProjDirPath}/../../aws_iot_bt_acs_porting_layer/ble/profile_appl
    ${ProjDirPath}/../../aws_iot_bt_acs_porting_layer/ble/protocol_appl
    ${ProjDirPath}/../../aws_iot_bt_acs_porting_layer/br/protocol_appl
    ${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/export/eOSAL
    ${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/export/include
    ${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/export/vendor
    ${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/private/protocols/att
    ${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/private/protocols/avctp
    ${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/private/protocols/avdtp
    ${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/private/protocols/bnep
    ${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/private/protocols/common
    ${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/private/protocols/dbase
    ${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/private/protocols/hci_1.2
    ${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/private/protocols/mcap
    ${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/private/protocols/obex
    ${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/private/protocols/rfcomm
    ${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/private/protocols/sdp
    ${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/private/protocols/sm
    ${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/private/protocols/smp
    ${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/private/protocols/write_task
    ${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/private/utils/aes_cmac
    ${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/private/utils/at_parser
    ${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/private/utils/btsnoop
    ${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/private/utils/object_parser
    ${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/private/utils/racp
    ${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/private/utils/security
    ${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/private/utils/status
    ${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/private/utils/storage
    ${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/private/utils/xml_parser
    ${ProjDirPath}/../../../../../../middleware/wireless/ethermind/port/pal/mcux/bluetooth
    ${ProjDirPath}/../../../../../../middleware/wireless/ethermind/port/pal/mcux/bluetooth/controller
    ${ProjDirPath}/../../../../../../middleware/wireless/ethermind/port/pal/mcux/ethal
    ${ProjDirPath}/../../../../../../middleware/wireless/ethermind/port/pal/mcux/sbc
    ${ProjDirPath}/../../../../../../middleware/wireless/ethermind/port/osal/src/freertos
    ${ProjDirPath}/../../../../../../middleware/wireless/ethermind/port/pal/mcux
    ${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/private/utils/sbc
    ${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/private/lib/mcux/default/config
    ${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/export/extension
)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)
     target_compile_definitions(${MCUX_SDK_PROJECT_NAME}  PRIVATE MBEDTLS_CONFIG_FILE="aws_mbedtls_config.h")
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)
     target_compile_definitions(${MCUX_SDK_PROJECT_NAME}  PRIVATE MBEDTLS_CONFIG_FILE="aws_mbedtls_config.h")
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)

set(CMAKE_MODULE_PATH
    ${ProjDirPath}/../../../../../../devices/MIMXRT1176/drivers
    ${ProjDirPath}/../../../../../../devices/MIMXRT1176
    ${ProjDirPath}/../../../../../../components/uart
    ${ProjDirPath}/../../../../../../devices/MIMXRT1176/utilities
    ${ProjDirPath}/../../../../../../devices/MIMXRT1176/xip
    ${ProjDirPath}/../../../../xip
    ${ProjDirPath}/../../../../../../components/serial_manager
    ${ProjDirPath}/../../../../../../components/lists
    ${ProjDirPath}/../../../../../../middleware/sdmmc
    ${ProjDirPath}/../../../../../../devices/MIMXRT1176/drivers/cm7
    ${ProjDirPath}/../../../../../../middleware/usb
    ${ProjDirPath}/../../../../../../components/osa
    ${ProjDirPath}/../../../../../../middleware/fatfs
    ${ProjDirPath}/../../../../../../components/flash/mflash
    ${ProjDirPath}/../../../../../../components/flash/mflash/mimxrt1170
    ${ProjDirPath}/../../../../../../middleware/mbedtls
    ${ProjDirPath}/../../../../../../rtos/freertos/freertos_kernel
    ${ProjDirPath}/../../../../../../rtos/freertos
    ${ProjDirPath}/../../../../../../middleware/lwip
    ${ProjDirPath}/../../../../../../middleware/wifi_nxp
    ${ProjDirPath}/../../../../../../components/wifi_bt_module
    ${ProjDirPath}/../../../../../../components/internal_flash
    ${ProjDirPath}/../../../../../../middleware/wireless
    ${ProjDirPath}/../../../../../../middleware/littlefs
    ${ProjDirPath}/../../../../../../CMSIS/Core/Include
)

# include modules
include(driver_clock_MIMXRT1176_cm7)

include(driver_common_MIMXRT1176_cm7)

include(device_MIMXRT1176_CMSIS_MIMXRT1176_cm7)

include(device_MIMXRT1176_startup_MIMXRT1176_cm7)

include(driver_lpuart_MIMXRT1176_cm7)

include(driver_igpio_MIMXRT1176_cm7)

include(component_lpuart_adapter_MIMXRT1176_cm7)

include(utility_assert_MIMXRT1176_cm7)

include(utility_debug_console_MIMXRT1176_cm7)

include(driver_xip_device_MIMXRT1176_cm7)

include(driver_xip_board_MIMXRT1176_cm7)

include(driver_iomuxc_MIMXRT1176_cm7)

include(component_serial_manager_MIMXRT1176_cm7)

include(component_lists_MIMXRT1176_cm7)

include(component_serial_manager_uart_MIMXRT1176_cm7)

include(middleware_sdmmc_sdio_MIMXRT1176_cm7)

include(middleware_sdmmc_common_MIMXRT1176_cm7)

include(middleware_sdmmc_host_usdhc_MIMXRT1176_cm7)

include(middleware_sdmmc_host_usdhc_freertos_MIMXRT1176_cm7)

include(driver_cache_armv7_m7_MIMXRT1176_cm7)

include(driver_flexspi_MIMXRT1176_cm7)

include(middleware_usb_host_stack_MIMXRT1176_cm7)

include(component_osa_MIMXRT1176_cm7)

include(middleware_usb_host_ehci_MIMXRT1176_cm7)

include(middleware_usb_phy_MIMXRT1176_cm7)

include(middleware_fatfs_MIMXRT1176_cm7)

include(middleware_fatfs_usb_MIMXRT1176_cm7)

include(component_mflash_file_MIMXRT1176_cm7)

include(component_mflash_rt1170_MIMXRT1176_cm7)

include(component_mflash_common_MIMXRT1176_cm7)

include(middleware_mbedtls_MIMXRT1176_cm7)

include(middleware_mbedtls_port_ksdk_MIMXRT1176_cm7)

include(middleware_mbedtls_rt2_MIMXRT1176_cm7)

include(middleware_freertos-kernel_heap_4_MIMXRT1176_cm7)

include(middleware_freertos-kernel_MIMXRT1176_cm7)

include(middleware_freertos-aws_iot_vendor_nxp_secure_sockets_lwip_MIMXRT1176_cm7)

include(middleware_freertos-aws_iot_demos_MIMXRT1176_cm7)

include(middleware_freertos-aws_iot_demos_dev_mode_key_provisioning_MIMXRT1176_cm7)

include(middleware_freertos-aws_iot_libraries_abstractions_wifi_MIMXRT1176_cm7)

include(middleware_lwip_MIMXRT1176_cm7)

include(middleware_wifi_MIMXRT1176_cm7)

include(middleware_freertos-aws_iot_libraries_abstractions_platform_MIMXRT1176_cm7)

include(middleware_freertos-aws_iot_libraries_abstractions_secure_sockets_MIMXRT1176_cm7)

include(middleware_freertos-aws_iot_libraries_freertos_plus_standard_crypto_MIMXRT1176_cm7)

include(middleware_freertos-aws_iot_libraries_freertos_plus_standard_utils_MIMXRT1176_cm7)

include(middleware_freertos-aws_iot_libraries_freertos_plus_standard_tls_MIMXRT1176_cm7)

include(middleware_freertos-aws_iot_libraries_abstractions_pkcs11_MIMXRT1176_cm7)

include(middleware_freertos-aws_iot_libraries_abstractions_pkcs11_mbedtls_MIMXRT1176_cm7)

include(middleware_freertos-aws_iot_libraries_3rdparty_pkcs11_MIMXRT1176_cm7)

include(middleware_freertos-aws_iot_libraries_3rdparty_jsmn_MIMXRT1176_cm7)

include(middleware_freertos-aws_iot_vendor_nxp_pkcs11_MIMXRT1176_cm7)

include(middleware_freertos-aws_iot_libraries_c_sdk_standard_ble_mt_MIMXRT1176_cm7)

include(middleware_freertos-aws_iot_demos_wifi_provisioning_MIMXRT1176_cm7)

include(middleware_freertos-aws_iot_libraries_abstractions_ble_hal_MIMXRT1176_cm7)

include(middleware_freertos-aws_iot_demos_unused_internal_mt_MIMXRT1176_cm7)

include(middleware_freertos-aws_iot_libraries_3rdparty_tinycbor_MIMXRT1176_cm7)

include(middleware_freertos-aws_iot_libraries_logging_MIMXRT1176_cm7)

include(middleware_freertos-aws_iot_libraries_device_shadow_for_aws_MIMXRT1176_cm7)

include(middleware_freertos-aws_iot_libraries_corejason_MIMXRT1176_cm7)

include(middleware_freertos-aws_iot_mqtt_demo_helpers_MIMXRT1176_cm7)

include(middleware_freertos-aws_iot_libraries_abstractions_transport_MIMXRT1176_cm7)

include(middleware_freertos-aws_iot_libraries_coremqtt_MIMXRT1176_cm7)

include(component_wifi_bt_module_tx_pwr_limits_MIMXRT1176_cm7)

include(driver_lpuart_edma_MIMXRT1176_cm7)

include(driver_pmu_1_MIMXRT1176_cm7)

include(driver_dcdc_soc_MIMXRT1176_cm7)

include(component_flexspi_nor_flash_adapter_MIMXRT1176_cm7)

include(component_flexspi_nor_flash_adapter_rt1060evk_MIMXRT1176_cm7)

include(middleware_wireless_framework_function_lib_MIMXRT1176_cm7)

include(middleware_wireless_framework_Common_MIMXRT1176_cm7)

include(middleware_littlefs_MIMXRT1176_cm7)

include(utilities_misc_utilities_MIMXRT1176_cm7)

include(driver_anatop_ai_MIMXRT1176_cm7)

include(CMSIS_Include_core_cm_MIMXRT1176_cm7)

include(device_MIMXRT1176_system_MIMXRT1176_cm7)

include(middleware_sdmmc_osa_freertos_MIMXRT1176_cm7)

include(component_osa_free_rtos_MIMXRT1176_cm7)

include(middleware_freertos-kernel_extension_MIMXRT1176_cm7)

include(driver_usdhc_MIMXRT1176_cm7)

include(driver_memory_MIMXRT1176_cm7)

include(middleware_usb_common_header_MIMXRT1176_cm7)

include(middleware_usb_host_common_header_MIMXRT1176_cm7)

include(middleware_usb_host_msd_MIMXRT1176_cm7)

include(driver_caam_MIMXRT1176_cm7)

include(middleware_freertos-aws_iot_libraries_c_sdk_standard_common_MIMXRT1176_cm7)

include(middleware_freertos-aws_iot_libraries_3rdparty_mbedtls_utils_MIMXRT1176_cm7)

include(middleware_freertos-aws_iot_libraries_3rdparty_mbedtls_config_MIMXRT1176_cm7)

include(middleware_freertos-aws_iot_network_manager_MIMXRT1176_cm7)

include(middleware_wifi_wifidriver_MIMXRT1176_cm7)

include(middleware_wifi_fwdnld_MIMXRT1176_cm7)

include(middleware_freertos-aws_iot_demos_ble_MIMXRT1176_cm7)

include(middleware_freertos-aws_iot_libraries_ble_MIMXRT1176_cm7)

include(middleware_freertos-aws_iot_libraries_abstractions_backoff_algorithm_MIMXRT1176_cm7)

include(middleware_freertos-aws_iot_pkcs11_helpers_MIMXRT1176_cm7)

include(driver_edma_MIMXRT1176_cm7)

include(driver_dmamux_MIMXRT1176_cm7)


TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE m)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE c)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE gcc)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE nosys)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm7f/gcc/libethermind_bt_a2dp.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm7f/gcc/libethermind_bt_avrcp.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm7f/gcc/libethermind_bt_bip.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm7f/gcc/libethermind_bt_bpp.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm7f/gcc/libethermind_bt_core.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm7f/gcc/libethermind_bt_ctn.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm7f/gcc/libethermind_bt_ftp.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm7f/gcc/libethermind_bt_gatt.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm7f/gcc/libethermind_bt_hfp.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm7f/gcc/libethermind_bt_hid.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm7f/gcc/libethermind_bt_map.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm7f/gcc/libethermind_bt_opp.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm7f/gcc/libethermind_bt_pan.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm7f/gcc/libethermind_bt_pbap.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm7f/gcc/libethermind_bt_protocol.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm7f/gcc/libethermind_bt_sap.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm7f/gcc/libethermind_bt_spp.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm7f/gcc/libethermind_bt_util.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm7f/gcc/libethermind_sbc_common.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm7f/gcc/libethermind_sbc_decoder.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm7f/gcc/libethermind_sbc_encoder.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm7f/gcc/libethermind_bt_a2dp.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm7f/gcc/libethermind_bt_avrcp.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm7f/gcc/libethermind_bt_bip.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm7f/gcc/libethermind_bt_bpp.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm7f/gcc/libethermind_bt_core.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm7f/gcc/libethermind_bt_ctn.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm7f/gcc/libethermind_bt_ftp.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm7f/gcc/libethermind_bt_gatt.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm7f/gcc/libethermind_bt_hfp.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm7f/gcc/libethermind_bt_hid.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm7f/gcc/libethermind_bt_map.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm7f/gcc/libethermind_bt_opp.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm7f/gcc/libethermind_bt_pan.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm7f/gcc/libethermind_bt_pbap.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm7f/gcc/libethermind_bt_protocol.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm7f/gcc/libethermind_bt_sap.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm7f/gcc/libethermind_bt_spp.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm7f/gcc/libethermind_bt_util.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm7f/gcc/libethermind_sbc_common.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm7f/gcc/libethermind_sbc_decoder.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../../middleware/wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm7f/gcc/libethermind_sbc_encoder.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)


