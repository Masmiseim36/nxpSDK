# Add set(CONFIG_USE_middleware_eiq_tensorflow_lite_micro_examples_microspeech true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_eiq_tensorflow_lite_micro_third_party_kissfft)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/examples/micro_speech/audio_provider.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/examples/micro_speech/command_responder.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/examples/micro_speech/feature_provider.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/examples/micro_speech/recognize_commands.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/examples/micro_speech/micro_features/micro_features_generator.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/examples/micro_speech/micro_features/micro_model_settings.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/experimental/microfrontend/lib/filterbank.c
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/experimental/microfrontend/lib/filterbank_util.c
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/experimental/microfrontend/lib/frontend.c
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/experimental/microfrontend/lib/frontend_util.c
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/experimental/microfrontend/lib/log_lut.c
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/experimental/microfrontend/lib/log_scale.c
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/experimental/microfrontend/lib/log_scale_util.c
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/experimental/microfrontend/lib/noise_reduction.c
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/experimental/microfrontend/lib/noise_reduction_util.c
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/experimental/microfrontend/lib/pcan_gain_control.c
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/experimental/microfrontend/lib/pcan_gain_control_util.c
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/experimental/microfrontend/lib/window.c
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/experimental/microfrontend/lib/window_util.c
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/experimental/microfrontend/lib/fft.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/experimental/microfrontend/lib/fft_util.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/experimental/microfrontend/lib/kiss_fft_int16.cpp
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/.
)

else()

message(SEND_ERROR "middleware_eiq_tensorflow_lite_micro_examples_microspeech dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
