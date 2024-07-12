# Add set(CONFIG_USE_middleware_mbedtls_tests true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/tests/src/asn1_helpers.c
  ${CMAKE_CURRENT_LIST_DIR}/tests/src/helpers.c
  ${CMAKE_CURRENT_LIST_DIR}/tests/src/psa_crypto_helpers.c
  ${CMAKE_CURRENT_LIST_DIR}/tests/src/psa_exercise_key.c
  ${CMAKE_CURRENT_LIST_DIR}/tests/src/random.c
  ${CMAKE_CURRENT_LIST_DIR}/tests/src/threading_helpers.c
  ${CMAKE_CURRENT_LIST_DIR}/tests/src/fake_external_rng_for_test.c
  ${CMAKE_CURRENT_LIST_DIR}/tests/src/test_helpers/ssl_helpers.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/tests/include/test
  ${CMAKE_CURRENT_LIST_DIR}/tests/include
)

