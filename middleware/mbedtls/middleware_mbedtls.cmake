# Add set(CONFIG_USE_middleware_mbedtls true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/library/debug.c
          ${CMAKE_CURRENT_LIST_DIR}/library/mbedtls_config.c
          ${CMAKE_CURRENT_LIST_DIR}/library/mps_reader.c
          ${CMAKE_CURRENT_LIST_DIR}/library/mps_trace.c
          ${CMAKE_CURRENT_LIST_DIR}/library/net_sockets.c
          ${CMAKE_CURRENT_LIST_DIR}/library/pkcs7.c
          ${CMAKE_CURRENT_LIST_DIR}/library/ssl_cache.c
          ${CMAKE_CURRENT_LIST_DIR}/library/ssl_ciphersuites.c
          ${CMAKE_CURRENT_LIST_DIR}/library/ssl_client.c
          ${CMAKE_CURRENT_LIST_DIR}/library/ssl_cookie.c
          ${CMAKE_CURRENT_LIST_DIR}/library/ssl_msg.c
          ${CMAKE_CURRENT_LIST_DIR}/library/ssl_ticket.c
          ${CMAKE_CURRENT_LIST_DIR}/library/ssl_tls.c
          ${CMAKE_CURRENT_LIST_DIR}/library/ssl_tls12_client.c
          ${CMAKE_CURRENT_LIST_DIR}/library/ssl_tls12_server.c
          ${CMAKE_CURRENT_LIST_DIR}/library/ssl_tls13_client.c
          ${CMAKE_CURRENT_LIST_DIR}/library/ssl_tls13_generic.c
          ${CMAKE_CURRENT_LIST_DIR}/library/ssl_tls13_keys.c
          ${CMAKE_CURRENT_LIST_DIR}/library/ssl_tls13_server.c
          ${CMAKE_CURRENT_LIST_DIR}/library/timing.c
          ${CMAKE_CURRENT_LIST_DIR}/library/version.c
          ${CMAKE_CURRENT_LIST_DIR}/library/x509.c
          ${CMAKE_CURRENT_LIST_DIR}/library/x509_create.c
          ${CMAKE_CURRENT_LIST_DIR}/library/x509_crl.c
          ${CMAKE_CURRENT_LIST_DIR}/library/x509_crt.c
          ${CMAKE_CURRENT_LIST_DIR}/library/x509_csr.c
          ${CMAKE_CURRENT_LIST_DIR}/library/x509_oid.c
          ${CMAKE_CURRENT_LIST_DIR}/library/x509write.c
          ${CMAKE_CURRENT_LIST_DIR}/library/x509write_crt.c
          ${CMAKE_CURRENT_LIST_DIR}/library/x509write_csr.c
        )

  
      target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/include
          ${CMAKE_CURRENT_LIST_DIR}/include/mbedtls
          ${CMAKE_CURRENT_LIST_DIR}/include/mbedtls/private
          ${CMAKE_CURRENT_LIST_DIR}/library
          ${CMAKE_CURRENT_LIST_DIR}/../../components/psa_crypto_driver/generated_files_mbedtls
        )

    if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

  
            if(CONFIG_TOOLCHAIN STREQUAL iar)
      target_compile_options(${MCUX_SDK_PROJECT_NAME} PUBLIC
              --diag_suppress=Pa167,Pe177,Pe191,Pe546
            )
      endif()
          if(CONFIG_TOOLCHAIN STREQUAL armgcc)
      target_compile_options(${MCUX_SDK_PROJECT_NAME} PUBLIC
              -fomit-frame-pointer
              -Wno-unused-function
            )
      endif()
      
  endif()

