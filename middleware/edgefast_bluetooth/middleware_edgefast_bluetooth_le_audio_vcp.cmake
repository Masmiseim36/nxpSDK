# Add set(CONFIG_USE_middleware_edgefast_bluetooth_le_audio_vcp true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_edgefast_bluetooth_le_audio_aics AND CONFIG_USE_middleware_edgefast_bluetooth_le_audio_vocs)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/source/impl/ethermind/audio/vcp_vol_ctlr.c
  ${CMAKE_CURRENT_LIST_DIR}/source/impl/ethermind/audio/vcp_vol_rend.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/include/bluetooth/audio
  ${CMAKE_CURRENT_LIST_DIR}/include/bluetooth/audio/config
  ${CMAKE_CURRENT_LIST_DIR}/source/impl/ethermind/audio
)

else()

message(SEND_ERROR "middleware_edgefast_bluetooth_le_audio_vcp dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
