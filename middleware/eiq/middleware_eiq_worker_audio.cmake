# Add set(CONFIG_USE_middleware_eiq_worker_audio true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_eiq_worker)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/common/audio/eiq_audio_worker.c
  ${CMAKE_CURRENT_LIST_DIR}/common/audio/eiq_micro.c
  ${CMAKE_CURRENT_LIST_DIR}/common/audio/eiq_speaker.c
  ${CMAKE_CURRENT_LIST_DIR}/common/audio/eiq_speaker_conf.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/common/audio
)

else()

message(SEND_ERROR "middleware_eiq_worker_audio dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
