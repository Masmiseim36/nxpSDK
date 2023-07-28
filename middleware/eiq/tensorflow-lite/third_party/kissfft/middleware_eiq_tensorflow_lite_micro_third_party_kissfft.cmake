# Add set(CONFIG_USE_middleware_eiq_tensorflow_lite_micro_third_party_kissfft true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/kiss_fft.c
  ${CMAKE_CURRENT_LIST_DIR}/tools/kfc.c
  ${CMAKE_CURRENT_LIST_DIR}/tools/kiss_fastfir.c
  ${CMAKE_CURRENT_LIST_DIR}/tools/kiss_fftnd.c
  ${CMAKE_CURRENT_LIST_DIR}/tools/kiss_fftndr.c
  ${CMAKE_CURRENT_LIST_DIR}/tools/kiss_fftr.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/.
)

