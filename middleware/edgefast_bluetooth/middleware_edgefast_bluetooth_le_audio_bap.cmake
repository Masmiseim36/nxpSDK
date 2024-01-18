# Add set(CONFIG_USE_middleware_edgefast_bluetooth_le_audio_bap true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/source/impl/ethermind/audio/ascs.c
  ${CMAKE_CURRENT_LIST_DIR}/source/impl/ethermind/audio/bap_iso.c
  ${CMAKE_CURRENT_LIST_DIR}/source/impl/ethermind/audio/audio.c
  ${CMAKE_CURRENT_LIST_DIR}/source/impl/ethermind/audio/bap_broadcast_sink.c
  ${CMAKE_CURRENT_LIST_DIR}/source/impl/ethermind/audio/bap_broadcast_source.c
  ${CMAKE_CURRENT_LIST_DIR}/source/impl/ethermind/audio/codec.c
  ${CMAKE_CURRENT_LIST_DIR}/source/impl/ethermind/audio/pacs.c
  ${CMAKE_CURRENT_LIST_DIR}/source/impl/ethermind/audio/bap_stream.c
  ${CMAKE_CURRENT_LIST_DIR}/source/impl/ethermind/audio/bap_unicast_client.c
  ${CMAKE_CURRENT_LIST_DIR}/source/impl/ethermind/audio/bap_unicast_server.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/include/bluetooth/audio
  ${CMAKE_CURRENT_LIST_DIR}/include/bluetooth/audio/config
  ${CMAKE_CURRENT_LIST_DIR}/source/impl/ethermind/audio
  ${CMAKE_CURRENT_LIST_DIR}/source/impl/ethermind
)

