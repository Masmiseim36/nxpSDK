# Add set(CONFIG_USE_middleware_edgefast_bluetooth_le_audio_micp true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_edgefast_bluetooth_le_audio_aics)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/source/impl/ethermind/audio/micp_mic_ctlr.c
  ${CMAKE_CURRENT_LIST_DIR}/source/impl/ethermind/audio/micp_mic_dev.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/include/bluetooth/audio
  ${CMAKE_CURRENT_LIST_DIR}/include/bluetooth/audio/config
  ${CMAKE_CURRENT_LIST_DIR}/source/impl/ethermind/audio
)

else()

message(SEND_ERROR "middleware_edgefast_bluetooth_le_audio_micp dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
