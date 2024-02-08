# Add set(CONFIG_USE_utility_crypto_benchmark true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/crypto_benchmark.c
  ${CMAKE_CURRENT_LIST_DIR}/crypto_benchmark_hash.c
  ${CMAKE_CURRENT_LIST_DIR}/crypto_benchmark_mac.c
  ${CMAKE_CURRENT_LIST_DIR}/crypto_benchmark_aes.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/.
)

