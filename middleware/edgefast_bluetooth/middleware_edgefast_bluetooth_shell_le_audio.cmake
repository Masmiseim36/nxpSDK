# Add set(CONFIG_USE_middleware_edgefast_bluetooth_shell_le_audio true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_edgefast_bluetooth_pal AND CONFIG_USE_component_common_task AND ((CONFIG_USE_middleware_edgefast_bluetooth_btble_ethermind_cm7f AND (CONFIG_CORE STREQUAL cm7f)) OR (CONFIG_USE_middleware_edgefast_bluetooth_btble_ethermind_cm4f AND (CONFIG_CORE STREQUAL cm4f)) OR (CONFIG_USE_middleware_edgefast_bluetooth_btble_ethermind_cm33 AND (CONFIG_CORE STREQUAL cm33) AND (CONFIG_DSP STREQUAL DSP)) OR (CONFIG_USE_middleware_edgefast_bluetooth_btble_ethermind_cm33nodsp AND (CONFIG_CORE STREQUAL cm33) AND (CONFIG_DSP STREQUAL NO_DSP))))

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/source/impl/ethermind/audio/shell/bap_broadcast_assistant.c
  ${CMAKE_CURRENT_LIST_DIR}/source/impl/ethermind/audio/shell/bap_scan_delegator.c
  ${CMAKE_CURRENT_LIST_DIR}/source/impl/ethermind/audio/shell/bap.c
  ${CMAKE_CURRENT_LIST_DIR}/source/impl/ethermind/audio/shell/cap_acceptor.c
  ${CMAKE_CURRENT_LIST_DIR}/source/impl/ethermind/audio/shell/cap_commander.c
  ${CMAKE_CURRENT_LIST_DIR}/source/impl/ethermind/audio/shell/cap_initiator.c
  ${CMAKE_CURRENT_LIST_DIR}/source/impl/ethermind/audio/shell/csip_set_coordinator.c
  ${CMAKE_CURRENT_LIST_DIR}/source/impl/ethermind/audio/shell/csip_set_member.c
  ${CMAKE_CURRENT_LIST_DIR}/source/impl/ethermind/audio/shell/has_client.c
  ${CMAKE_CURRENT_LIST_DIR}/source/impl/ethermind/audio/shell/has.c
  ${CMAKE_CURRENT_LIST_DIR}/source/impl/ethermind/audio/shell/mcc.c
  ${CMAKE_CURRENT_LIST_DIR}/source/impl/ethermind/audio/shell/media_controller.c
  ${CMAKE_CURRENT_LIST_DIR}/source/impl/ethermind/audio/shell/micp_mic_ctlr.c
  ${CMAKE_CURRENT_LIST_DIR}/source/impl/ethermind/audio/shell/micp_mic_dev.c
  ${CMAKE_CURRENT_LIST_DIR}/source/impl/ethermind/audio/shell/mpl.c
  ${CMAKE_CURRENT_LIST_DIR}/source/impl/ethermind/audio/shell/tbs_client.c
  ${CMAKE_CURRENT_LIST_DIR}/source/impl/ethermind/audio/shell/tbs.c
  ${CMAKE_CURRENT_LIST_DIR}/source/impl/ethermind/audio/shell/tmap.c
  ${CMAKE_CURRENT_LIST_DIR}/source/impl/ethermind/audio/shell/gmap.c
  ${CMAKE_CURRENT_LIST_DIR}/source/impl/ethermind/audio/shell/pbp.c
  ${CMAKE_CURRENT_LIST_DIR}/source/impl/ethermind/audio/shell/vcp_vol_ctlr.c
  ${CMAKE_CURRENT_LIST_DIR}/source/impl/ethermind/audio/shell/vcp_vol_rend.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/include
  ${CMAKE_CURRENT_LIST_DIR}/source/shell
  ${CMAKE_CURRENT_LIST_DIR}/source/impl/ethermind/audio/shell
)

if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

  target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
    -DSHELL_ADVANCE=1
    -DDEBUG_CONSOLE_RX_ENABLE=0
    -DOSA_USED=1
    -DSHELL_USE_COMMON_TASK=0
  )

endif()

else()

message(SEND_ERROR "middleware_edgefast_bluetooth_shell_le_audio dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
