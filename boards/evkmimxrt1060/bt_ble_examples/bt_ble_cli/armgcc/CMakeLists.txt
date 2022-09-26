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


project(bt_ble_cli)

set(MCUX_SDK_PROJECT_NAME bt_ble_cli.elf)

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${ProjDirPath}/../lwipopts.h"
"${ProjDirPath}/../lwippools.h"
"${ProjDirPath}/../mbedtls_config_client.h"
"${ProjDirPath}/../sdmmc_config.c"
"${ProjDirPath}/../sdmmc_config.h"
"${ProjDirPath}/../wifi_bt_config.c"
"${ProjDirPath}/../wifi_bt_config.h"
"${ProjDirPath}/../appl/reference/FreeRTOSConfig.h"
"${ProjDirPath}/../appl/reference/appl_main.c"
"${ProjDirPath}/../appl/reference/appl_main.h"
"${ProjDirPath}/../appl/reference/appl_profile.h"
"${ProjDirPath}/../appl/reference/appl_protocol.h"
"${ProjDirPath}/../appl/reference/appl_utils.c"
"${ProjDirPath}/../appl/reference/appl_utils.h"
"${ProjDirPath}/../main.c"
"${ProjDirPath}/../wifi_config.h"
"${ProjDirPath}/../a2dp_pl_ext.c"
"${ProjDirPath}/../sco_audio_pl_ext.c"
"${ProjDirPath}/../ringtone.h"
"${ProjDirPath}/../db_gen.c"
"${ProjDirPath}/../app.h"
"${ProjDirPath}/../ffconf.h"
"${ProjDirPath}/../host_msd_fatfs.c"
"${ProjDirPath}/../host_msd_fatfs.h"
"${ProjDirPath}/../usb_host_config.h"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../dcd.c"
"${ProjDirPath}/../dcd.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server/hts/appl_hts.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server/hts/appl_hts.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server/hts/appl_hts_gap_config_params.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server/hts/appl_hts_mops.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server/hts/appl_hts_mops.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server/hts/gatt_db_dyn_hts.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server/hts/gatt_db_hts.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server/hts/gatt_db_hts_handler.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server/hts/gatt_db_stat_hts.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server/pxr/appl_pxr.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server/pxr/appl_pxr.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server/pxr/appl_pxr_gap_config_params.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server/pxr/appl_pxr_mops.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server/pxr/appl_pxr_mops.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server/pxr/gatt_db_dyn_pxr.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server/pxr/gatt_db_pxr.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server/pxr/gatt_db_pxr_handler.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server/pxr/gatt_db_stat_pxr.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server/ipspn/appl_ipspn.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server/ipspn/appl_ipspn.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server/ipspn/appl_ipspn_gap_config_params.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server/ipspn/appl_ipspn_mops.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server/ipspn/appl_ipspn_mops.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server/ipspn/gatt_db_dyn_ipspn.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server/ipspn/gatt_db_ipspn.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server/ipspn/gatt_db_ipspn_handler.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server/ipspn/gatt_db_stat_ipspn.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server/hps/appl_hps.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server/hps/appl_hps.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server/hps/appl_hps_gap_config_params.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server/hps/appl_hps_mops.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server/hps/appl_hps_mops.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server/hps/gatt_db_dyn_hps.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server/hps/gatt_db_hps.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server/hps/gatt_db_hps_handler.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server/hps/gatt_db_stat_hps.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/port/pal/mcux/bluetooth/niface.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/port/pal/mcux/bluetooth/niface.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/client/appl_bac.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/client/appl_bpc.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/client/appl_cpmc.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/client/appl_cscc.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/client/appl_dic.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/client/appl_fml.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/client/appl_glc.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/client/appl_hidh.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/client/appl_hrc.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/client/appl_htc.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/client/appl_pasp.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/client/appl_pxm.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/client/appl_rscc.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/client/appl_hpc.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/client/appl_tic.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/client/appl_ipspr.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/br/profile_appl/appl_a2dp.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/br/profile_appl/appl_a2dp.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/br/profile_appl/appl_bip.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/br/profile_appl/appl_bip_initiator.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/br/profile_appl/appl_bip_responder.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/br/profile_appl/appl_bpp.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/br/profile_appl/appl_bpp_printer.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/br/profile_appl/appl_bpp_sender.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/br/profile_appl/appl_ctn.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/br/profile_appl/appl_ctn_cce.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/br/profile_appl/appl_ctn_cse.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/br/profile_appl/appl_did.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/br/profile_appl/appl_did_client.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/br/profile_appl/appl_did_server.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/br/profile_appl/appl_ftp.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/br/profile_appl/appl_ftp_client.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/br/profile_appl/appl_ftp_server.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/br/profile_appl/appl_hdp.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/br/profile_appl/appl_hdp.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/br/profile_appl/appl_hfp_ag.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/br/profile_appl/appl_hfp_ag.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/br/profile_appl/appl_hfp_ag_atrsp.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/br/profile_appl/appl_hfp_unit.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/br/profile_appl/appl_hfp_unit.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/br/profile_appl/appl_hid_device.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/br/profile_appl/appl_hid_device.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/br/profile_appl/appl_hid_host.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/br/profile_appl/appl_hid_host.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/br/profile_appl/appl_map.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/br/profile_appl/appl_map_mce.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/br/profile_appl/appl_map_mse.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/br/profile_appl/appl_opp.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/br/profile_appl/appl_opp_client.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/br/profile_appl/appl_opp_server.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/br/profile_appl/appl_pbap_pce.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/br/profile_appl/appl_pbap_pce.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/br/profile_appl/appl_pbap_pse.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/br/profile_appl/appl_pbap_pse.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/br/profile_appl/appl_sap_client.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/br/profile_appl/appl_sap_client.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/br/profile_appl/appl_sap_server.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/br/profile_appl/appl_sap_server.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/br/profile_appl/appl_spp.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/br/profile_appl/appl_spp.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/br/profile_appl/appl_pan.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/br/profile_appl/appl_pan.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/port/pal/mcux/bluetooth/hps_pl.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/port/pal/mcux/bluetooth/hps_pl.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/port/pal/mcux/bluetooth/https_pl.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/port/pal/mcux/bluetooth/https_pl.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/port/pal/mcux/bluetooth/ipsp_pl.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/port/pal/mcux/bluetooth/ipsp_pl.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/port/pal/mcux/bluetooth/ipspr_ping.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/port/pal/mcux/bluetooth/pan_pl.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/port/pal/mcux/bluetooth/pan_pl.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/port/pal/mcux/bluetooth/hci_uart.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/port/pal/mcux/bluetooth/hci_uart.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/port/pal/mcux/bluetooth/hci_uart_internal.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/eOSAL/eBT_assert.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/eOSAL/eBT_config.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/eOSAL/eBT_debug_api.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/eOSAL/eBT_fops.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/eOSAL/eBT_os.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/eOSAL/eBT_serial.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/eOSAL/eBT_timer.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/include/BT_a2dp_api.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/include/BT_api.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/include/BT_assigned_numbers.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/include/BT_at_parser_api.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/include/BT_att_api.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/include/BT_avctp_api.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/include/BT_avdtp_api.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/include/BT_avrcp_api.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/include/BT_avrcp_ca_api.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/include/BT_bip_api.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/include/BT_bnep_api.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/include/BT_bottom_half.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/include/BT_bpp_api.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/include/BT_common.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/include/BT_ctn_api.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/include/BT_dbase_api.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/include/BT_device_queue.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/include/BT_error.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/include/BT_ftp_api.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/include/BT_gap_api.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/include/BT_gatt_db_api.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/include/BT_hci_api.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/include/BT_hdp_api.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/include/BT_hfp_ag_api.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/include/BT_hfp_unit_api.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/include/BT_hid_device_api.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/include/BT_hid_host_api.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/include/BT_irobex_api.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/include/BT_jpl_api.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/include/BT_map_api.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/include/BT_mcap_api.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/include/BT_obex_api.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/include/BT_opp_api.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/include/BT_pan_api.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/include/BT_pbap_api.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/include/BT_racp.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/include/BT_rfcomm_api.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/include/BT_sap_api.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/include/BT_sdp_api.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/include/BT_security.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/include/BT_sm_api.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/include/BT_smp_api.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/include/BT_snoop.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/include/BT_spp_api.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/include/BT_status.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/include/BT_storage.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/include/BT_version.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/include/fsm_defines.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/include/fsm_engine.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/include/gatt_defines.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/include/hci_transport.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/include/l2cap.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/include/write_task.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/vendor/vendor_specific_commands.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/vendor/vendor_specific_init.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/vendor/vendor_specific_init.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/private/protocols/att/att.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/private/protocols/avctp/avctp.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/private/protocols/avdtp/avdtp.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/private/protocols/bnep/bnep.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/private/protocols/dbase/database.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/private/protocols/hci_1.2/hci_h.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/private/protocols/mcap/mcap.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/private/protocols/obex/obex.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/private/protocols/rfcomm/rfcomm.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/private/protocols/sdp/sdp.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/private/protocols/sm/sm.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/private/protocols/sm/sm_extern.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/private/protocols/sm/sm_internal.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/private/protocols/smp/smp.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/private/protocols/smp/smp_extern.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/private/protocols/smp/smp_fsm.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/private/protocols/smp/smp_fsm_engine.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/private/protocols/smp/smp_internal.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/private/utils/aes_cmac/aes_cmac.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/private/utils/at_parser/at_parser_hsp_hfp.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/private/utils/object_parser/object_parser_api.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/private/utils/object_parser/tokenizer.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/private/utils/racp/racp.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/private/utils/storage/storage.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/private/utils/xml_parser/xml_common_defines.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/private/utils/xml_parser/xml_encoder.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/private/utils/xml_parser/xml_parser.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/port/osal/src/freertos/EM_assert.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/port/osal/src/freertos/EM_config.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/port/osal/src/freertos/EM_config.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/port/osal/src/freertos/EM_debug.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/port/osal/src/freertos/EM_debug.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/port/osal/src/freertos/EM_debug_api.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/port/osal/src/freertos/EM_debug_internal.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/port/osal/src/freertos/EM_fops.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/port/osal/src/freertos/EM_fops.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/port/osal/src/freertos/EM_os.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/port/osal/src/freertos/EM_os.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/port/osal/src/freertos/EM_serial.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/port/osal/src/freertos/EM_serial.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/port/osal/src/freertos/EM_timer.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/port/osal/src/freertos/EM_timer.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/port/osal/src/freertos/EM_timer_internal.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/port/pal/mcux/bluetooth/BT_assert.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/port/pal/mcux/bluetooth/BT_config.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/port/pal/mcux/bluetooth/BT_debug.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/port/pal/mcux/bluetooth/BT_debug_api.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/port/pal/mcux/bluetooth/BT_fops.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/port/pal/mcux/bluetooth/BT_os.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/port/pal/mcux/bluetooth/BT_status_pl.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/port/pal/mcux/bluetooth/BT_storage_pl.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/port/pal/mcux/bluetooth/BT_storage_pl.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/port/pal/mcux/bluetooth/BT_timer.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/port/pal/mcux/bluetooth/btsnoop_pl.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/port/pal/mcux/bluetooth/btsnoop_pl.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/port/pal/mcux/bluetooth/ctn_pl.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/port/pal/mcux/bluetooth/ctn_pl.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/port/pal/mcux/bluetooth/ftp_pl.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/port/pal/mcux/bluetooth/ftp_pl.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/port/pal/mcux/bluetooth/ht_read_task_pl.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/port/pal/mcux/bluetooth/map_pl.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/port/pal/mcux/bluetooth/map_pl.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/port/pal/mcux/bluetooth/obex_pl.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/port/pal/mcux/bluetooth/obex_pl.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/port/pal/mcux/bluetooth/pbap_pl.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/port/pal/mcux/bluetooth/pbap_pl.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/port/pal/mcux/bluetooth/write_task_extern_pl.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/port/pal/mcux/bluetooth/write_task_pl.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/port/pal/mcux/bluetooth/write_task_pl.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/port/pal/mcux/bluetooth/littlefs_pl.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/port/pal/mcux/bluetooth/littlefs_pl.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/port/pal/mcux/bluetooth/controller.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/port/pal/mcux/bluetooth/controller.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/port/pal/mcux/bluetooth/controller/controller_wifi_nxp.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/port/pal/mcux/bluetooth/controller/controller_wifi_nxp.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/port/pal/mcux/ethal/ethal_assert.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/port/pal/mcux/ethal/ethal_config.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/port/pal/mcux/ethal/ethal_config.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/port/pal/mcux/ethal/ethal_debug.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/port/pal/mcux/ethal/ethal_debug.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/port/pal/mcux/ethal/ethal_debug_api.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/port/pal/mcux/ethal/ethal_debug_internal.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/port/pal/mcux/ethal/ethal_fops.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/port/pal/mcux/ethal/ethal_fops.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/port/pal/mcux/ethal/ethal_os.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/port/pal/mcux/ethal/ethal_os.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/port/pal/mcux/ethal/ethal_serial.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/port/pal/mcux/ethal/ethal_serial.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/port/pal/mcux/ethal/ethal_timer.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/port/pal/mcux/ethal/ethal_timer.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/port/pal/mcux/ethal/ethal_timer_internal.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/port/pal/mcux/sbc/sbc_os.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/port/pal/mcux/EM_platform.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/port/pal/mcux/EM_platform.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/private/utils/sbc/sbc_api.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/br/protocol_appl/appl_avctp.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/br/protocol_appl/appl_avctp.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/br/protocol_appl/appl_avdtp.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/br/protocol_appl/appl_avdtp.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/br/protocol_appl/appl_bnep.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/br/protocol_appl/appl_bnep.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/br/protocol_appl/appl_dbase.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/br/protocol_appl/appl_dbase_dyn.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/br/protocol_appl/appl_hci.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/br/protocol_appl/appl_hci.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/br/protocol_appl/appl_hci_api.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/br/protocol_appl/appl_l2cap.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/br/protocol_appl/appl_l2cap.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/br/protocol_appl/appl_mcap.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/br/protocol_appl/appl_mcap.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/br/protocol_appl/appl_obex_client.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/br/protocol_appl/appl_obex_client.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/br/protocol_appl/appl_obex_common.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/br/protocol_appl/appl_obex_server.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/br/protocol_appl/appl_obex_server.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/br/protocol_appl/appl_rfcomm.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/br/protocol_appl/appl_rfcomm.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/br/protocol_appl/appl_sdp.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/br/protocol_appl/appl_sdp.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/br/protocol_appl/appl_sm.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/br/protocol_appl/appl_sm.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/br/al/appl_avrcp.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/br/al/appl_avrcp.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/br/al/appl_avrcp_ct.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/br/al/appl_avrcp_ct.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/br/al/appl_avrcp_tg.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/br/al/appl_avrcp_tg.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/br/al/appl_avrcp_tg_virtual_media_player.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/br/al/appl_avrcp_tg_virtual_media_player.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/br/al/avrcp_al_api.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/br/al/avrcp_al_internal.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/br/al/avrcp_al_internal.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/br/al/BT_avrcp_al_api.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/appl_le.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/appl_le.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/port/pal/mcux/bluetooth/hfp_ag_pl.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/port/pal/mcux/bluetooth/hfp_ag_pl.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/port/pal/mcux/bluetooth/nwsim.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/port/pal/mcux/bluetooth/nwsim.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/port/pal/mcux/bluetooth/sco_audio_pl.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/port/pal/mcux/bluetooth/sco_audio_pl.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server/appl_service.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server/appl_service.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server/appl_service_mops.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server/appl_service_mops.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server/gatt_db.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/private/lib/mcux/default/config/BT_features.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/private/lib/mcux/default/config/BT_limits.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/private/lib/mcux/default/config/db_gen.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/extension/aes_cmac_pl.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/extension/aes_cmac_pl.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/extension/BT_common_pl.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/extension/BT_common_pl.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/extension/device_queue_pl.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/extension/device_queue_pl.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/extension/sm_pl.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/extension/sm_pl.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/extension/sm_ssp_pl.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/extension/sm_ssp_pl.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/extension/smp_pl.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/extension/smp_pl.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/extension/gatt_db_pl.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/extension/gatt_db_pl.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/port/pal/mcux/bluetooth/a2dp_pl.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/port/pal/mcux/bluetooth/a2dp_pl.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/port/pal/mcux/bluetooth/a2dp_pl_media_16KHz.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/port/pal/mcux/bluetooth/a2dp_pl_media_48KHz.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/port/pal/mcux/bluetooth/vcom_pl.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/port/pal/mcux/bluetooth/vcom_pl.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server/bps/appl_bps.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server/bps/appl_bps.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server/bps/appl_bps_gap_config_params.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server/bps/appl_bps_mops.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server/bps/appl_bps_mops.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server/bps/gatt_db_bps.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server/bps/gatt_db_bps_handler.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server/bps/gatt_db_dyn_bps.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server/bps/gatt_db_stat_bps.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server/cpms/appl_cpms.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server/cpms/appl_cpms.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server/cpms/appl_cpms_gap_config_params.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server/cpms/appl_cpms_mops.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server/cpms/appl_cpms_mops.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server/cpms/gatt_db_cpms.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server/cpms/gatt_db_cpms_handler.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server/cpms/gatt_db_dyn_cpms.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server/cpms/gatt_db_stat_cpms.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server/cscs/appl_cscs.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server/cscs/appl_cscs.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server/cscs/appl_cscs_gap_config_params.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server/cscs/appl_cscs_mops.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server/cscs/appl_cscs_mops.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server/cscs/gatt_db_cscs.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server/cscs/gatt_db_cscs_handler.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server/cscs/gatt_db_dyn_cscs.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server/cscs/gatt_db_stat_cscs.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server/gls/appl_gls.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server/gls/appl_gls.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server/gls/appl_gls_gap_config_params.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server/gls/appl_gls_mops.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server/gls/appl_gls_mops.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server/gls/gatt_db_dyn_gls.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server/gls/gatt_db_gls.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server/gls/gatt_db_gls_handler.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server/gls/gatt_db_stat_gls.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server/hid/appl_hid.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server/hid/appl_hid.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server/hid/appl_hid_gap_config_params.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server/hid/appl_hid_mops.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server/hid/appl_hid_mops.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server/hid/gatt_db_dyn_hid.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server/hid/gatt_db_hid.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server/hid/gatt_db_hid_handler.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server/hid/gatt_db_stat_hid.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server/hrs/appl_hrs.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server/hrs/appl_hrs.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server/hrs/appl_hrs_gap_config_params.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server/hrs/appl_hrs_mops.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server/hrs/appl_hrs_mops.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server/hrs/gatt_db_dyn_hrs.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server/hrs/gatt_db_hrs.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server/hrs/gatt_db_hrs_handler.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server/hrs/gatt_db_stat_hrs.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server/tis/appl_tis.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server/tis/appl_tis.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server/tis/appl_tis_gap_config_params.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server/tis/appl_tis_mops.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server/tis/appl_tis_mops.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server/tis/gatt_db_dyn_tis.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server/tis/gatt_db_stat_tis.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server/tis/gatt_db_tis.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server/tis/gatt_db_tis_handler.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server/pass/appl_pass.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server/pass/appl_pass.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server/pass/appl_pass_gap_config_params.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server/pass/appl_pass_mops.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server/pass/appl_pass_mops.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server/pass/gatt_db_dyn_pass.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server/pass/gatt_db_pass.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server/pass/gatt_db_pass_handler.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server/pass/gatt_db_stat_pass.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server/rscs/appl_rscs.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server/rscs/appl_rscs.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server/rscs/appl_rscs_gap_config_params.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server/rscs/appl_rscs_mops.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server/rscs/appl_rscs_mops.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server/rscs/gatt_db_dyn_rscs.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server/rscs/gatt_db_rscs.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server/rscs/gatt_db_rscs_handler.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server/rscs/gatt_db_stat_rscs.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/a2dp/a2dp_source.conf"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/a2dp/sample.wav"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/avrcp/Image_200_200.jpg"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/avrcp/Image_640_480.gif"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/avrcp/Image_640_480.jpg"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/avrcp/Image_75_75.jpg"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/avrcp/avrcp_cap_company_id_list.dat"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/avrcp/avrcp_cap_event_id_list.dat"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/avrcp/avrcp_folder_depth_1_items_list.dat"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/avrcp/avrcp_media_element_attr_info.dat"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/avrcp/avrcp_media_element_attr_info_large.dat"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/avrcp/avrcp_media_element_attr_info_new_track.dat"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/avrcp/avrcp_media_element_list_file.dat"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/avrcp/avrcp_media_player_list_info.dat"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/avrcp/avrcp_new_media_element_file.dat"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/avrcp/avrcp_player_appl_attr_info.dat"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/avrcp/avrcp_player_cur_media_play_status.dat"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/avrcp/avrcp_root_folder_items_list.dat"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/avrcp/company_id_list_1.dat"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/avrcp/event_id_list_1.dat"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/avrcp/folder_items_list.dat"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/avrcp/media_element_list_file.dat"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/avrcp/media_player_list_file.dat"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/avrcp/new_folder_item.dat"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/avrcp/new_song_info.dat"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/avrcp/player_attr_file_1.dat"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/avrcp/player_status_file.dat"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/avrcp/player_vfs.dat"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/avrcp/song_info_1.dat"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/avrcp/song_info_1_for_fragmentation_handling.dat"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/bip/BIP_1_160_120.jpg"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/bip/BIP_1_160_120_jpg_desc.xml"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/bip/BIP_1_160_120_thumbnail.jpg"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/bip/BIP_1_640_480.jpg"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/bip/BIP_1_640_480_jpg_desc.xml"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/bip/BIP_1_640_480_thumbnail.jpg"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/bip/image_list.xml"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/bip/image_properties.xml"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/bip/images_capabilities.xml"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/bip/monitoring_image.jpg"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/bip/sample.jpg"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/bip/sample1.png"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/bip/sample2.jpg"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/bip/sample3.jpg"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/bip/sample_thumbnail.jpg"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/bpp/bigpic.jpg"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/bpp/feynman.jpg"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/bpp/sample_ref_push_file.xml"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/bpp/vCal_multiple_object.vcs"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/bpp/vCal_single_object.vcs"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/bpp/vCard_multiple.vcf"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/bpp/vCard_single.vcf"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/bpp/vmsg.vmg"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/ctn/CTN-account-info.ics"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/ctn/CTN-event.ics"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/ctn/CTN-listing_large.xml"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/ctn/new_calendar.ics"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/ctn/push_calendar.ics"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/ctn/push_notes.ics"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/ctn/push_task.ics"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/ctn/root/telecom/CTN/notes/20000000000000000000000000000001.ics"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/ctn/root/telecom/CTN/notes/20000000000000000000000000000002.ics"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/ctn/root/telecom/CTN/notes/CTN-listing.xml"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/ctn/root/telecom/CTN/tasks/30000000000000000000000000000001.ics"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/ctn/root/telecom/CTN/tasks/30000000000000000000000000000002.ics"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/ctn/root/telecom/CTN/tasks/30000000000000000000000000000003.ics"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/ctn/root/telecom/CTN/tasks/CTN-listing.xml"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/ftp/food.jpeg"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/ftp/sachin.jpeg"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/ftp/sample.bmp"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/map/event_email.xml"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/map/event_im.xml"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/map/event_mms.xml"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/map/event_sms_cdma.xml"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/map/event_sms_gsm.xml"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/map/map_event_report_v11.xml"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/map/map_event_report_v12.xml"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/map/map_mse_participant_data.xml"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/map/message_email.msg"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/map/message_email.vmg"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/map/message_im.vmg"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/map/message_mms.msg"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/map/message_mms.vmg"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/map/message_sms_cdma.msg"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/map/message_sms_cdma.vmg"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/map/message_sms_gsm.msg"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/map/message_sms_gsm.vmg"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/opp/bmp2MB.bmp"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/opp/mybmp.bmp"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/opp/mycal.vcs"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/opp/mycard.vcf"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/opp/mymsg.vmg"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/opp/mynote.vnt"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/opp/vCal-1.vcs"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/opp/vCal-2.vcs"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/opp/vCal-multiple.vcs"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/opp/vCard-1.vcf"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/opp/vCard-2.vcf"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/opp/vCard-multiple.vcf"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/opp/vMsg-1.vmg"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/opp/vMsg-2.vmg"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/opp/vMsg-multiple.vmg"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/opp/vNote-1.vnt"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/opp/vNote-2.vnt"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/opp/vNote-multiple.vnt"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/pan/arp_gratuitous_reply.dat"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/pan/arp_request.dat"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/pan/arp_response.dat"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/pan/dhcp_ack.dat"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/pan/dhcp_discover.dat"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/pan/dhcp_offer.dat"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/pan/dhcp_request.dat"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/pan/dns_query.dat"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/pan/dns_response.dat"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/pan/eth_payload_60_0x34d.dat"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/pan/eth_payload_60_0x44a.dat"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/pan/eth_payload_60_0x5a0.dat"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/pan/eth_payload_ipv4ping.dat"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/pan/http_request.dat"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/pan/http_response.dat"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/pan/llmnr_query.dat"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/pan/llmnr_query_ipv6.dat"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/pan/llmnr_response.dat"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/pan/ndns_request.dat"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/pan/ping_reply.dat"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/pan/ping_request.dat"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/pan/wap_request.dat"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/pan/wap_response.dat"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/pbap/phonebook/pb_0.vcf"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/pbap/phonebook/pb_1.vcf"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/pbap/phonebook/root/SIM1/telecom/cch/1.vcf"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/pbap/phonebook/root/SIM1/telecom/cch/2.vcf"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/pbap/phonebook/root/SIM1/telecom/cch/3.vcf"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/pbap/phonebook/root/SIM1/telecom/cch/4.vcf"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/pbap/phonebook/root/SIM1/telecom/cch/5.vcf"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/pbap/phonebook/root/SIM1/telecom/cch/6.vcf"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/pbap/phonebook/root/SIM1/telecom/cch/7.vcf"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/pbap/phonebook/root/SIM1/telecom/fav/1.vcf"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/pbap/phonebook/root/SIM1/telecom/fav/2.vcf"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/pbap/phonebook/root/SIM1/telecom/fav/3.vcf"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/pbap/phonebook/root/SIM1/telecom/mch/1.vcf"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/pbap/phonebook/root/SIM1/telecom/mch/2.vcf"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/pbap/phonebook/root/SIM1/telecom/mch/3.vcf"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/pbap/phonebook/root/SIM1/telecom/mch/4.vcf"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/pbap/phonebook/root/SIM1/telecom/och/1.vcf"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/pbap/phonebook/root/SIM1/telecom/och/2.vcf"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/pbap/phonebook/root/SIM1/telecom/och/3.vcf"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/pbap/phonebook/root/SIM1/telecom/och/4.vcf"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/pbap/phonebook/root/SIM1/telecom/och/5.vcf"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/pbap/phonebook/root/SIM1/telecom/pb/0.vcf"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/pbap/phonebook/root/SIM1/telecom/pb/1.vcf"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/pbap/phonebook/root/SIM1/telecom/pb/10.vcf"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/pbap/phonebook/root/SIM1/telecom/pb/11.vcf"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/pbap/phonebook/root/SIM1/telecom/pb/12.vcf"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/pbap/phonebook/root/SIM1/telecom/pb/13.vcf"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/pbap/phonebook/root/SIM1/telecom/pb/14.vcf"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/pbap/phonebook/root/SIM1/telecom/pb/15.vcf"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/pbap/phonebook/root/SIM1/telecom/pb/16.vcf"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/pbap/phonebook/root/SIM1/telecom/pb/17.vcf"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/pbap/phonebook/root/SIM1/telecom/pb/2.vcf"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/pbap/phonebook/root/SIM1/telecom/pb/3.vcf"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/pbap/phonebook/root/SIM1/telecom/pb/4.vcf"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/pbap/phonebook/root/SIM1/telecom/pb/5.vcf"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/pbap/phonebook/root/SIM1/telecom/pb/6.vcf"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/pbap/phonebook/root/SIM1/telecom/pb/7.vcf"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/pbap/phonebook/root/SIM1/telecom/pb/8.vcf"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/pbap/phonebook/root/SIM1/telecom/pb/9.vcf"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/pbap/phonebook/root/SIM1/telecom/spd/1.vcf"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/pbap/phonebook/root/SIM1/telecom/spd/2.vcf"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/pbap/phonebook/root/SIM1/telecom/spd/3.vcf"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/pbap/phonebook/root/telecom/cch/1.vcf"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/pbap/phonebook/root/telecom/cch/16.vcf"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/pbap/phonebook/root/telecom/cch/17.vcf"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/pbap/phonebook/root/telecom/cch/2.vcf"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/pbap/phonebook/root/telecom/cch/3.vcf"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/pbap/phonebook/root/telecom/cch/4.vcf"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/pbap/phonebook/root/telecom/cch/5.vcf"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/pbap/phonebook/root/telecom/cch/6.vcf"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/pbap/phonebook/root/telecom/cch/7.vcf"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/pbap/phonebook/root/telecom/fav/1.vcf"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/pbap/phonebook/root/telecom/fav/2.vcf"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/pbap/phonebook/root/telecom/fav/3.vcf"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/pbap/phonebook/root/telecom/ich/1.vcf"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/pbap/phonebook/root/telecom/ich/2.vcf"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/pbap/phonebook/root/telecom/ich/3.vcf"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/pbap/phonebook/root/telecom/ich/4.vcf"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/pbap/phonebook/root/telecom/ich/5.vcf"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/pbap/phonebook/root/telecom/ich/6.vcf"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/pbap/phonebook/root/telecom/mch/1.vcf"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/pbap/phonebook/root/telecom/mch/2.vcf"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/pbap/phonebook/root/telecom/mch/3.vcf"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/pbap/phonebook/root/telecom/mch/4.vcf"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/pbap/phonebook/root/telecom/och/1.vcf"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/pbap/phonebook/root/telecom/och/2.vcf"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/pbap/phonebook/root/telecom/och/3.vcf"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/pbap/phonebook/root/telecom/och/4.vcf"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/pbap/phonebook/root/telecom/och/5.vcf"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/pbap/phonebook/root/telecom/pb/0.vcf"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/pbap/phonebook/root/telecom/pb/1.vcf"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/pbap/phonebook/root/telecom/pb/10.vcf"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/pbap/phonebook/root/telecom/pb/11.vcf"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/pbap/phonebook/root/telecom/pb/12.vcf"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/pbap/phonebook/root/telecom/pb/13.vcf"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/pbap/phonebook/root/telecom/pb/14.vcf"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/pbap/phonebook/root/telecom/pb/15.vcf"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/pbap/phonebook/root/telecom/pb/16.vcf"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/pbap/phonebook/root/telecom/pb/17.vcf"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/pbap/phonebook/root/telecom/pb/2.vcf"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/pbap/phonebook/root/telecom/pb/3.vcf"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/pbap/phonebook/root/telecom/pb/4.vcf"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/pbap/phonebook/root/telecom/pb/5.vcf"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/pbap/phonebook/root/telecom/pb/6.vcf"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/pbap/phonebook/root/telecom/pb/7.vcf"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/pbap/phonebook/root/telecom/pb/8.vcf"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/pbap/phonebook/root/telecom/pb/9.vcf"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/pbap/phonebook/root/telecom/spd/1.vcf"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/pbap/phonebook/root/telecom/spd/2.vcf"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/pbap/phonebook/root/telecom/spd/3.vcf"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/protocol_appl/appl_att_client.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/protocol_appl/appl_att_client.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/protocol_appl/appl_gap_le.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/protocol_appl/appl_gap_le.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/protocol_appl/appl_hci_le.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/protocol_appl/appl_hci_le.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/protocol_appl/appl_hci_le_api.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/protocol_appl/appl_l2cap_le.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/protocol_appl/appl_l2cap_le.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/protocol_appl/appl_smp.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/protocol_appl/appl_smp.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/appl_gatt_client.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/appl_gatt_client.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/appl_gatt_server.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/appl_gatt_server.h"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/gatt.c"
"${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/gatt.h"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${ProjDirPath}/..
    ${ProjDirPath}/../appl/reference
    ${ProjDirPath}/../../..
    ${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server/hts
    ${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server/pxr
    ${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server/ipspn
    ${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server/hps
    ${ProjDirPath}/../../../../../middleware/wireless/ethermind/port/pal/mcux/bluetooth
    ${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/client
    ${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/br/profile_appl
    ${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/eOSAL
    ${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/include
    ${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/vendor
    ${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/private/protocols/att
    ${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/private/protocols/avctp
    ${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/private/protocols/avdtp
    ${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/private/protocols/bnep
    ${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/private/protocols/common
    ${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/private/protocols/dbase
    ${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/private/protocols/hci_1.2
    ${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/private/protocols/mcap
    ${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/private/protocols/obex
    ${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/private/protocols/rfcomm
    ${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/private/protocols/sdp
    ${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/private/protocols/sm
    ${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/private/protocols/smp
    ${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/private/protocols/write_task
    ${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/private/utils/aes_cmac
    ${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/private/utils/at_parser
    ${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/private/utils/btsnoop
    ${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/private/utils/object_parser
    ${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/private/utils/racp
    ${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/private/utils/security
    ${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/private/utils/status
    ${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/private/utils/storage
    ${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/private/utils/xml_parser
    ${ProjDirPath}/../../../../../middleware/wireless/ethermind/port/pal/mcux/bluetooth/controller
    ${ProjDirPath}/../../../../../middleware/wireless/ethermind/port/pal/mcux/ethal
    ${ProjDirPath}/../../../../../middleware/wireless/ethermind/port/pal/mcux/sbc
    ${ProjDirPath}/../../../../../middleware/wireless/ethermind/port/osal/src/freertos
    ${ProjDirPath}/../../../../../middleware/wireless/ethermind/port/pal/mcux
    ${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/private/utils/sbc
    ${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/br/protocol_appl
    ${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/br/al
    ${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le
    ${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server
    ${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/private/lib/mcux/default/config
    ${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/extension
    ${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server/bps
    ${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server/cpms
    ${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server/cscs
    ${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server/gls
    ${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server/hid
    ${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server/hrs
    ${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server/tis
    ${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server/pass
    ${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl/server/rscs
    ${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/a2dp
    ${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/avrcp
    ${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/bip
    ${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/bpp
    ${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/ctn
    ${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/ftp
    ${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/map
    ${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/opp
    ${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/pan
    ${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/tools/resources/pbap
    ${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/protocol_appl
    ${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/export/appl/reference/le/profile_appl
)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)
     target_compile_definitions(${MCUX_SDK_PROJECT_NAME}  PRIVATE MBEDTLS_CONFIG_FILE="mbedtls_config_client.h")
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)
     target_compile_definitions(${MCUX_SDK_PROJECT_NAME}  PRIVATE MBEDTLS_CONFIG_FILE="mbedtls_config_client.h")
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)

set(CMAKE_MODULE_PATH
    ${ProjDirPath}/../../../../../devices/MIMXRT1062/drivers
    ${ProjDirPath}/../../../../../devices/MIMXRT1062
    ${ProjDirPath}/../../../../../components/uart
    ${ProjDirPath}/../../../../../devices/MIMXRT1062/utilities
    ${ProjDirPath}/../../../../../devices/MIMXRT1062/xip
    ${ProjDirPath}/../../../xip
    ${ProjDirPath}/../../../../../components/serial_manager
    ${ProjDirPath}/../../../../../components/lists
    ${ProjDirPath}/../../../../../middleware/sdmmc
    ${ProjDirPath}/../../../../../components/phy/mdio/enet
    ${ProjDirPath}/../../../../../components/phy/device/phyksz8081
    ${ProjDirPath}/../../../../../components/codec/wm8960
    ${ProjDirPath}/../../../../../components/codec
    ${ProjDirPath}/../../../../../components/codec/i2c
    ${ProjDirPath}/../../../../../components/i2c
    ${ProjDirPath}/../../../../../middleware/usb
    ${ProjDirPath}/../../../../../components/osa
    ${ProjDirPath}/../../../../../middleware/fatfs
    ${ProjDirPath}/../../../../../middleware/mbedtls
    ${ProjDirPath}/../../../../../middleware/lwip
    ${ProjDirPath}/../../../../../components/phy
    ${ProjDirPath}/../../../../../rtos/freertos/freertos_kernel
    ${ProjDirPath}/../../../../../middleware/wifi_nxp
    ${ProjDirPath}/../../../../../components/internal_flash
    ${ProjDirPath}/../../../../../middleware/wireless
    ${ProjDirPath}/../../../../../middleware/littlefs
    ${ProjDirPath}/../../../../../components/flash/mflash
    ${ProjDirPath}/../../../../../components/flash/mflash/mimxrt1062
    ${ProjDirPath}/../../../../../components/gpio
    ${ProjDirPath}/../../../../../CMSIS/Core/Include
    ${ProjDirPath}/../../../../../components/wifi_bt_module
)

# include modules
include(driver_clock_MIMXRT1062)

include(driver_common_MIMXRT1062)

include(device_MIMXRT1062_CMSIS_MIMXRT1062)

include(device_MIMXRT1062_startup_MIMXRT1062)

include(driver_lpuart_MIMXRT1062)

include(driver_igpio_MIMXRT1062)

include(component_lpuart_adapter_MIMXRT1062)

include(utility_assert_MIMXRT1062)

include(utility_debug_console_MIMXRT1062)

include(driver_xip_device_MIMXRT1062)

include(driver_xip_board_evkmimxrt1060_MIMXRT1062)

include(driver_iomuxc_MIMXRT1062)

include(component_serial_manager_MIMXRT1062)

include(component_lists_MIMXRT1062)

include(component_serial_manager_uart_MIMXRT1062)

include(middleware_sdmmc_sdio_MIMXRT1062)

include(middleware_sdmmc_common_MIMXRT1062)

include(middleware_sdmmc_host_usdhc_MIMXRT1062)

include(middleware_sdmmc_host_usdhc_freertos_MIMXRT1062)

include(driver_mdio-enet_MIMXRT1062)

include(driver_phy-device-ksz8081_MIMXRT1062)

include(driver_cache_armv7_m7_MIMXRT1062)

include(driver_lpi2c_MIMXRT1062)

include(driver_wm8960_MIMXRT1062)

include(driver_codec_MIMXRT1062)

include(component_wm8960_adapter_MIMXRT1062)

include(component_codec_i2c_MIMXRT1062)

include(component_lpi2c_adapter_MIMXRT1062)

include(driver_sai_MIMXRT1062)

include(driver_sai_edma_MIMXRT1062)

include(driver_dmamux_MIMXRT1062)

include(driver_edma_MIMXRT1062)

include(middleware_usb_host_stack_MIMXRT1062)

include(component_osa_MIMXRT1062)

include(middleware_usb_host_ehci_MIMXRT1062)

include(middleware_usb_phy_MIMXRT1062)

include(middleware_fatfs_MIMXRT1062)

include(middleware_fatfs_usb_MIMXRT1062)

include(middleware_mbedtls_MIMXRT1062)

include(driver_trng_MIMXRT1062)

include(driver_dcp_MIMXRT1062)

include(middleware_mbedtls_port_ksdk_MIMXRT1062)

include(middleware_mbedtls_rt_MIMXRT1062)

include(middleware_lwip_contrib_ping_MIMXRT1062)

include(middleware_lwip_MIMXRT1062)

include(middleware_lwip_enet_ethernetif_MIMXRT1062)

include(driver_phy-common_MIMXRT1062)

include(driver_enet_MIMXRT1062)

include(middleware_freertos-kernel_heap_4_MIMXRT1062)

include(middleware_wifi_fwdnld_MIMXRT1062)

include(middleware_freertos-kernel_MIMXRT1062)

include(driver_flexspi_MIMXRT1062)

include(component_flexspi_nor_flash_adapter_MIMXRT1062)

include(component_flexspi_nor_flash_adapter_rt1060evk_MIMXRT1062)

include(middleware_wireless_framework_function_lib_MIMXRT1062)

include(middleware_wireless_framework_Common_MIMXRT1062)

include(middleware_littlefs_MIMXRT1062)

include(component_mflash_file_MIMXRT1062)

include(component_mflash_rt1060_MIMXRT1062)

include(component_igpio_adapter_MIMXRT1062)

include(utilities_misc_utilities_MIMXRT1062)

include(CMSIS_Include_core_cm_MIMXRT1062)

include(device_MIMXRT1062_system_MIMXRT1062)

include(middleware_sdmmc_osa_freertos_MIMXRT1062)

include(component_osa_free_rtos_MIMXRT1062)

include(middleware_freertos-kernel_extension_MIMXRT1062)

include(driver_usdhc_MIMXRT1062)

include(driver_mdio-common_MIMXRT1062)

include(middleware_usb_common_header_MIMXRT1062)

include(middleware_usb_host_common_header_MIMXRT1062)

include(middleware_usb_host_msd_MIMXRT1062)

include(component_wifi_bt_module_tx_pwr_limits_MIMXRT1062)

include(middleware_wifi_sdio-2_MIMXRT1062)

include(component_mflash_common_MIMXRT1062)


TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE m)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE c)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE gcc)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE nosys)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm7f/gcc/libethermind_bt_a2dp.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm7f/gcc/libethermind_bt_avrcp.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm7f/gcc/libethermind_bt_bip.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm7f/gcc/libethermind_bt_bpp.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm7f/gcc/libethermind_bt_core.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm7f/gcc/libethermind_bt_ctn.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm7f/gcc/libethermind_bt_ftp.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm7f/gcc/libethermind_bt_gatt.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm7f/gcc/libethermind_bt_hfp.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm7f/gcc/libethermind_bt_hid.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm7f/gcc/libethermind_bt_map.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm7f/gcc/libethermind_bt_opp.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm7f/gcc/libethermind_bt_pan.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm7f/gcc/libethermind_bt_pbap.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm7f/gcc/libethermind_bt_protocol.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm7f/gcc/libethermind_bt_sap.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm7f/gcc/libethermind_bt_spp.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm7f/gcc/libethermind_bt_util.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm7f/gcc/libethermind_sbc_common.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm7f/gcc/libethermind_sbc_decoder.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm7f/gcc/libethermind_sbc_encoder.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm7f/gcc/libethermind_bt_a2dp.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm7f/gcc/libethermind_bt_avrcp.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm7f/gcc/libethermind_bt_bip.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm7f/gcc/libethermind_bt_bpp.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm7f/gcc/libethermind_bt_core.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm7f/gcc/libethermind_bt_ctn.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm7f/gcc/libethermind_bt_ftp.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm7f/gcc/libethermind_bt_gatt.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm7f/gcc/libethermind_bt_hfp.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm7f/gcc/libethermind_bt_hid.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm7f/gcc/libethermind_bt_map.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm7f/gcc/libethermind_bt_opp.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm7f/gcc/libethermind_bt_pan.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm7f/gcc/libethermind_bt_pbap.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm7f/gcc/libethermind_bt_protocol.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm7f/gcc/libethermind_bt_sap.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm7f/gcc/libethermind_bt_spp.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm7f/gcc/libethermind_bt_util.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm7f/gcc/libethermind_sbc_common.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm7f/gcc/libethermind_sbc_decoder.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../middleware/wireless/ethermind/bluetooth/private/lib/mcux/default/btdm/cm7f/gcc/libethermind_sbc_encoder.a)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)


