# Add set(CONFIG_USE_middleware_mbedtls3x_x509 true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/library/x509.c
  ${CMAKE_CURRENT_LIST_DIR}/library/x509_create.c
  ${CMAKE_CURRENT_LIST_DIR}/library/x509_crl.c
  ${CMAKE_CURRENT_LIST_DIR}/library/x509_crt.c
  ${CMAKE_CURRENT_LIST_DIR}/library/x509_csr.c
  ${CMAKE_CURRENT_LIST_DIR}/library/x509write_crt.c
  ${CMAKE_CURRENT_LIST_DIR}/library/x509write_csr.c
  ${CMAKE_CURRENT_LIST_DIR}/library/pkcs7.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/include
  ${CMAKE_CURRENT_LIST_DIR}/library
)

