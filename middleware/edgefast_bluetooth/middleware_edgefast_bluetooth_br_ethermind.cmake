# Add set(CONFIG_USE_middleware_edgefast_bluetooth_br_ethermind true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/source/impl/ethermind/host/bt_pal_a2dp.c
          ${CMAKE_CURRENT_LIST_DIR}/source/impl/ethermind/host/a2dp_codec/sbc/a2dp_codec_sbc.c
          ${CMAKE_CURRENT_LIST_DIR}/source/impl/ethermind/host/bt_pal_avrcp.c
          ${CMAKE_CURRENT_LIST_DIR}/source/impl/ethermind/host/bt_pal_avrcp_al_internal.c
          ${CMAKE_CURRENT_LIST_DIR}/source/impl/ethermind/host/bt_pal_avrcp_al_api.c
          ${CMAKE_CURRENT_LIST_DIR}/source/impl/ethermind/host/bt_pal_conn.c
          ${CMAKE_CURRENT_LIST_DIR}/source/impl/ethermind/host/bt_pal_data.c
          ${CMAKE_CURRENT_LIST_DIR}/source/impl/ethermind/host/bt_pal_ead.c
          ${CMAKE_CURRENT_LIST_DIR}/source/impl/ethermind/host/bt_pal_crypto.c
          ${CMAKE_CURRENT_LIST_DIR}/source/impl/ethermind/host/bt_pal_aes_ccm.c
          ${CMAKE_CURRENT_LIST_DIR}/source/impl/ethermind/host/bt_pal_hci_core.c
          ${CMAKE_CURRENT_LIST_DIR}/source/impl/ethermind/host/bt_pal_adv.c
          ${CMAKE_CURRENT_LIST_DIR}/source/impl/ethermind/host/bt_pal_br.c
          ${CMAKE_CURRENT_LIST_DIR}/source/impl/ethermind/host/bt_pal_id.c
          ${CMAKE_CURRENT_LIST_DIR}/source/impl/ethermind/host/bt_pal_long_wq.c
          ${CMAKE_CURRENT_LIST_DIR}/source/impl/ethermind/host/bt_pal_scan.c
          ${CMAKE_CURRENT_LIST_DIR}/source/impl/ethermind/host/bt_pal_ecc.c
          ${CMAKE_CURRENT_LIST_DIR}/source/impl/ethermind/host/bt_pal_addr.c
          ${CMAKE_CURRENT_LIST_DIR}/source/impl/ethermind/host/bt_pal_buf.c
          ${CMAKE_CURRENT_LIST_DIR}/source/impl/ethermind/host/bt_pal_direction.c
          ${CMAKE_CURRENT_LIST_DIR}/source/impl/ethermind/host/bt_pal_keys.c
          ${CMAKE_CURRENT_LIST_DIR}/source/impl/ethermind/host/bt_pal_keys_br.c
          ${CMAKE_CURRENT_LIST_DIR}/source/impl/ethermind/host/bt_pal_l2cap.c
          ${CMAKE_CURRENT_LIST_DIR}/source/impl/ethermind/host/bt_pal_l2cap_br.c
          ${CMAKE_CURRENT_LIST_DIR}/source/impl/ethermind/host/bt_pal_map_mce.c
          ${CMAKE_CURRENT_LIST_DIR}/source/impl/ethermind/host/bt_pal_map_mse.c
          ${CMAKE_CURRENT_LIST_DIR}/source/impl/ethermind/host/bt_pal_monitor.c
          ${CMAKE_CURRENT_LIST_DIR}/source/impl/ethermind/host/bt_pal_rfcomm.c
          ${CMAKE_CURRENT_LIST_DIR}/source/impl/ethermind/host/bt_pal_spp.c
          ${CMAKE_CURRENT_LIST_DIR}/source/impl/ethermind/host/bt_pal_rpa.c
          ${CMAKE_CURRENT_LIST_DIR}/source/impl/ethermind/host/bt_pal_settings.c
          ${CMAKE_CURRENT_LIST_DIR}/source/impl/ethermind/host/bt_pal_sdp.c
          ${CMAKE_CURRENT_LIST_DIR}/source/impl/ethermind/host/bt_pal_smp.c
          ${CMAKE_CURRENT_LIST_DIR}/source/impl/ethermind/host/bt_pal_ssp.c
          ${CMAKE_CURRENT_LIST_DIR}/source/impl/ethermind/host/bt_pal_uuid.c
          ${CMAKE_CURRENT_LIST_DIR}/source/impl/ethermind/host/bt_pal_hfp_ag.c
          ${CMAKE_CURRENT_LIST_DIR}/source/impl/ethermind/host/bt_pal_hfp_hf.c
          ${CMAKE_CURRENT_LIST_DIR}/source/impl/ethermind/host/bt_pal_pbap_pce.c
          ${CMAKE_CURRENT_LIST_DIR}/source/impl/ethermind/host/bt_pal_pbap_pse.c
          ${CMAKE_CURRENT_LIST_DIR}/source/impl/ethermind/host/bt_pal_sco.c
          ${CMAKE_CURRENT_LIST_DIR}/source/impl/ethermind/common/addr.c
        )

  
      target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/source/impl/ethermind/host
          ${CMAKE_CURRENT_LIST_DIR}/include
        )

    if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

      target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
                  -DCFG_CLASSIC
              )
  
  
  endif()

