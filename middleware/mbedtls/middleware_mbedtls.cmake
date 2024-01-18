# Add set(CONFIG_USE_middleware_mbedtls true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_utility_debug_console AND (CONFIG_USE_middleware_mbedtls_port_ksdk OR CONFIG_USE_middleware_mbedtls_port_els_pkc))

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/library/aes.c
  ${CMAKE_CURRENT_LIST_DIR}/library/aesni.c
  ${CMAKE_CURRENT_LIST_DIR}/library/arc4.c
  ${CMAKE_CURRENT_LIST_DIR}/library/aria.c
  ${CMAKE_CURRENT_LIST_DIR}/library/asn1parse.c
  ${CMAKE_CURRENT_LIST_DIR}/library/asn1write.c
  ${CMAKE_CURRENT_LIST_DIR}/library/base64.c
  ${CMAKE_CURRENT_LIST_DIR}/library/bignum.c
  ${CMAKE_CURRENT_LIST_DIR}/library/blowfish.c
  ${CMAKE_CURRENT_LIST_DIR}/library/camellia.c
  ${CMAKE_CURRENT_LIST_DIR}/library/ccm.c
  ${CMAKE_CURRENT_LIST_DIR}/library/certs.c
  ${CMAKE_CURRENT_LIST_DIR}/library/chacha20.c
  ${CMAKE_CURRENT_LIST_DIR}/library/chachapoly.c
  ${CMAKE_CURRENT_LIST_DIR}/library/cipher.c
  ${CMAKE_CURRENT_LIST_DIR}/library/cipher_wrap.c
  ${CMAKE_CURRENT_LIST_DIR}/library/cmac.c
  ${CMAKE_CURRENT_LIST_DIR}/library/constant_time.c
  ${CMAKE_CURRENT_LIST_DIR}/library/ctr_drbg.c
  ${CMAKE_CURRENT_LIST_DIR}/library/debug.c
  ${CMAKE_CURRENT_LIST_DIR}/library/des.c
  ${CMAKE_CURRENT_LIST_DIR}/library/dhm.c
  ${CMAKE_CURRENT_LIST_DIR}/library/ecdh.c
  ${CMAKE_CURRENT_LIST_DIR}/library/ecdsa.c
  ${CMAKE_CURRENT_LIST_DIR}/library/ecjpake.c
  ${CMAKE_CURRENT_LIST_DIR}/library/ecp.c
  ${CMAKE_CURRENT_LIST_DIR}/library/ecp_curves.c
  ${CMAKE_CURRENT_LIST_DIR}/library/entropy.c
  ${CMAKE_CURRENT_LIST_DIR}/library/entropy_poll.c
  ${CMAKE_CURRENT_LIST_DIR}/library/error.c
  ${CMAKE_CURRENT_LIST_DIR}/library/gcm.c
  ${CMAKE_CURRENT_LIST_DIR}/library/havege.c
  ${CMAKE_CURRENT_LIST_DIR}/library/hkdf.c
  ${CMAKE_CURRENT_LIST_DIR}/library/hmac_drbg.c
  ${CMAKE_CURRENT_LIST_DIR}/library/md.c
  ${CMAKE_CURRENT_LIST_DIR}/library/md2.c
  ${CMAKE_CURRENT_LIST_DIR}/library/md4.c
  ${CMAKE_CURRENT_LIST_DIR}/library/md5.c
  ${CMAKE_CURRENT_LIST_DIR}/library/memory_buffer_alloc.c
  ${CMAKE_CURRENT_LIST_DIR}/library/mps_reader.c
  ${CMAKE_CURRENT_LIST_DIR}/library/mps_trace.c
  ${CMAKE_CURRENT_LIST_DIR}/library/net_sockets.c
  ${CMAKE_CURRENT_LIST_DIR}/library/nist_kw.c
  ${CMAKE_CURRENT_LIST_DIR}/library/oid.c
  ${CMAKE_CURRENT_LIST_DIR}/library/padlock.c
  ${CMAKE_CURRENT_LIST_DIR}/library/pem.c
  ${CMAKE_CURRENT_LIST_DIR}/library/pk.c
  ${CMAKE_CURRENT_LIST_DIR}/library/pk_wrap.c
  ${CMAKE_CURRENT_LIST_DIR}/library/pkcs5.c
  ${CMAKE_CURRENT_LIST_DIR}/library/pkcs11.c
  ${CMAKE_CURRENT_LIST_DIR}/library/pkcs12.c
  ${CMAKE_CURRENT_LIST_DIR}/library/pkparse.c
  ${CMAKE_CURRENT_LIST_DIR}/library/pkwrite.c
  ${CMAKE_CURRENT_LIST_DIR}/library/platform.c
  ${CMAKE_CURRENT_LIST_DIR}/library/platform_util.c
  ${CMAKE_CURRENT_LIST_DIR}/library/poly1305.c
  ${CMAKE_CURRENT_LIST_DIR}/library/psa_crypto.c
  ${CMAKE_CURRENT_LIST_DIR}/library/psa_crypto_aead.c
  ${CMAKE_CURRENT_LIST_DIR}/library/psa_crypto_cipher.c
  ${CMAKE_CURRENT_LIST_DIR}/library/psa_crypto_client.c
  ${CMAKE_CURRENT_LIST_DIR}/library/psa_crypto_driver_wrappers.c
  ${CMAKE_CURRENT_LIST_DIR}/library/psa_crypto_ecp.c
  ${CMAKE_CURRENT_LIST_DIR}/library/psa_crypto_hash.c
  ${CMAKE_CURRENT_LIST_DIR}/library/psa_crypto_mac.c
  ${CMAKE_CURRENT_LIST_DIR}/library/psa_crypto_rsa.c
  ${CMAKE_CURRENT_LIST_DIR}/library/psa_crypto_se.c
  ${CMAKE_CURRENT_LIST_DIR}/library/psa_crypto_slot_management.c
  ${CMAKE_CURRENT_LIST_DIR}/library/psa_crypto_storage.c
  ${CMAKE_CURRENT_LIST_DIR}/library/psa_its_file.c
  ${CMAKE_CURRENT_LIST_DIR}/library/ripemd160.c
  ${CMAKE_CURRENT_LIST_DIR}/library/rsa.c
  ${CMAKE_CURRENT_LIST_DIR}/library/rsa_internal.c
  ${CMAKE_CURRENT_LIST_DIR}/library/sha1.c
  ${CMAKE_CURRENT_LIST_DIR}/library/sha256.c
  ${CMAKE_CURRENT_LIST_DIR}/library/sha512.c
  ${CMAKE_CURRENT_LIST_DIR}/library/ssl_cache.c
  ${CMAKE_CURRENT_LIST_DIR}/library/ssl_ciphersuites.c
  ${CMAKE_CURRENT_LIST_DIR}/library/ssl_cli.c
  ${CMAKE_CURRENT_LIST_DIR}/library/ssl_cookie.c
  ${CMAKE_CURRENT_LIST_DIR}/library/ssl_msg.c
  ${CMAKE_CURRENT_LIST_DIR}/library/ssl_srv.c
  ${CMAKE_CURRENT_LIST_DIR}/library/ssl_ticket.c
  ${CMAKE_CURRENT_LIST_DIR}/library/ssl_tls.c
  ${CMAKE_CURRENT_LIST_DIR}/library/ssl_tls13_keys.c
  ${CMAKE_CURRENT_LIST_DIR}/library/threading.c
  ${CMAKE_CURRENT_LIST_DIR}/library/timing.c
  ${CMAKE_CURRENT_LIST_DIR}/library/version.c
  ${CMAKE_CURRENT_LIST_DIR}/library/version_features.c
  ${CMAKE_CURRENT_LIST_DIR}/library/x509.c
  ${CMAKE_CURRENT_LIST_DIR}/library/x509_create.c
  ${CMAKE_CURRENT_LIST_DIR}/library/x509_crl.c
  ${CMAKE_CURRENT_LIST_DIR}/library/x509_crt.c
  ${CMAKE_CURRENT_LIST_DIR}/library/x509_csr.c
  ${CMAKE_CURRENT_LIST_DIR}/library/x509write_crt.c
  ${CMAKE_CURRENT_LIST_DIR}/library/x509write_csr.c
  ${CMAKE_CURRENT_LIST_DIR}/library/xtea.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/include
  ${CMAKE_CURRENT_LIST_DIR}/library
)

if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

  if(CONFIG_TOOLCHAIN STREQUAL iar)
    target_compile_options(${MCUX_SDK_PROJECT_NAME} PUBLIC
      --diag_suppress Pa167,Pe177,Pe191,Pe546
    )
  endif()
  if(CONFIG_TOOLCHAIN STREQUAL armgcc)
    target_compile_options(${MCUX_SDK_PROJECT_NAME} PUBLIC
      -fomit-frame-pointer
      -Wno-unused-function
    )
  endif()

endif()

else()

message(SEND_ERROR "middleware_mbedtls dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
