# Add set(CONFIG_USE_middleware_mbedcrypto true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_mbedcrypto_port)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/library/aes.c
  ${CMAKE_CURRENT_LIST_DIR}/library/aesce.c
  ${CMAKE_CURRENT_LIST_DIR}/library/aesni.c
  ${CMAKE_CURRENT_LIST_DIR}/library/asn1parse.c
  ${CMAKE_CURRENT_LIST_DIR}/library/asn1write.c
  ${CMAKE_CURRENT_LIST_DIR}/library/base64.c
  ${CMAKE_CURRENT_LIST_DIR}/library/bignum.c
  ${CMAKE_CURRENT_LIST_DIR}/library/bignum_core.c
  ${CMAKE_CURRENT_LIST_DIR}/library/bignum_mod.c
  ${CMAKE_CURRENT_LIST_DIR}/library/bignum_mod_raw.c
  ${CMAKE_CURRENT_LIST_DIR}/library/camellia.c
  ${CMAKE_CURRENT_LIST_DIR}/library/ccm.c
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
  ${CMAKE_CURRENT_LIST_DIR}/library/hash_info.c
  ${CMAKE_CURRENT_LIST_DIR}/library/hkdf.c
  ${CMAKE_CURRENT_LIST_DIR}/library/hmac_drbg.c
  ${CMAKE_CURRENT_LIST_DIR}/library/lmots.c
  ${CMAKE_CURRENT_LIST_DIR}/library/lms.c
  ${CMAKE_CURRENT_LIST_DIR}/library/md.c
  ${CMAKE_CURRENT_LIST_DIR}/library/md5.c
  ${CMAKE_CURRENT_LIST_DIR}/library/memory_buffer_alloc.c
  ${CMAKE_CURRENT_LIST_DIR}/library/nist_kw.c
  ${CMAKE_CURRENT_LIST_DIR}/library/oid.c
  ${CMAKE_CURRENT_LIST_DIR}/library/padlock.c
  ${CMAKE_CURRENT_LIST_DIR}/library/pem.c
  ${CMAKE_CURRENT_LIST_DIR}/library/pk.c
  ${CMAKE_CURRENT_LIST_DIR}/library/pk_wrap.c
  ${CMAKE_CURRENT_LIST_DIR}/library/pkcs5.c
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
  ${CMAKE_CURRENT_LIST_DIR}/library/psa_crypto_ecp.c
  ${CMAKE_CURRENT_LIST_DIR}/library/psa_crypto_hash.c
  ${CMAKE_CURRENT_LIST_DIR}/library/psa_crypto_mac.c
  ${CMAKE_CURRENT_LIST_DIR}/library/psa_crypto_pake.c
  ${CMAKE_CURRENT_LIST_DIR}/library/psa_crypto_rsa.c
  ${CMAKE_CURRENT_LIST_DIR}/library/psa_crypto_se.c
  ${CMAKE_CURRENT_LIST_DIR}/library/psa_crypto_slot_management.c
  ${CMAKE_CURRENT_LIST_DIR}/library/psa_crypto_storage.c
  ${CMAKE_CURRENT_LIST_DIR}/library/psa_its_file.c
  ${CMAKE_CURRENT_LIST_DIR}/library/psa_util.c
  ${CMAKE_CURRENT_LIST_DIR}/library/ripemd160.c
  ${CMAKE_CURRENT_LIST_DIR}/library/rsa.c
  ${CMAKE_CURRENT_LIST_DIR}/library/rsa_alt_helpers.c
  ${CMAKE_CURRENT_LIST_DIR}/library/sha1.c
  ${CMAKE_CURRENT_LIST_DIR}/library/sha256.c
  ${CMAKE_CURRENT_LIST_DIR}/library/sha512.c
  ${CMAKE_CURRENT_LIST_DIR}/library/threading.c
  ${CMAKE_CURRENT_LIST_DIR}/library/timing.c
  ${CMAKE_CURRENT_LIST_DIR}/library/version.c
  ${CMAKE_CURRENT_LIST_DIR}/library/version_features.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/include/mbedtls
  ${CMAKE_CURRENT_LIST_DIR}/include/psa
  ${CMAKE_CURRENT_LIST_DIR}/library
  ${CMAKE_CURRENT_LIST_DIR}/port
  ${CMAKE_CURRENT_LIST_DIR}/include
)

else()

message(SEND_ERROR "middleware_mbedcrypto dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
