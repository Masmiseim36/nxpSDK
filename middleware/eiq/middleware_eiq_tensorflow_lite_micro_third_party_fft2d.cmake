# Add set(CONFIG_USE_middleware_eiq_tensorflow_lite_micro_third_party_fft2d true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow-lite/third_party/fft2d/fftsg.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow-lite/third_party/fft2d
)

