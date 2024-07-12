# Add set(CONFIG_USE_middleware_mbedtls3x_crypto_no_psa true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/library/aes.c
  ${CMAKE_CURRENT_LIST_DIR}/library/aesni.c
  ${CMAKE_CURRENT_LIST_DIR}/library/aesce.c
  ${CMAKE_CURRENT_LIST_DIR}/library/aria.c
  ${CMAKE_CURRENT_LIST_DIR}/library/asn1parse.c
  ${CMAKE_CURRENT_LIST_DIR}/library/asn1write.c
  ${CMAKE_CURRENT_LIST_DIR}/library/base64.c
  ${CMAKE_CURRENT_LIST_DIR}/library/bignum.c
  ${CMAKE_CURRENT_LIST_DIR}/library/bignum_core.c
  ${CMAKE_CURRENT_LIST_DIR}/library/bignum_mod.c
  ${CMAKE_CURRENT_LIST_DIR}/library/bignum_mod_raw.c
  ${CMAKE_CURRENT_LIST_DIR}/library/block_cipher.c
  ${CMAKE_CURRENT_LIST_DIR}/library/camellia.c
  ${CMAKE_CURRENT_LIST_DIR}/library/ccm.c
  ${CMAKE_CURRENT_LIST_DIR}/library/chacha20.c
  ${CMAKE_CURRENT_LIST_DIR}/library/chachapoly.c
  ${CMAKE_CURRENT_LIST_DIR}/library/cipher.c
  ${CMAKE_CURRENT_LIST_DIR}/library/cipher_wrap.c
  ${CMAKE_CURRENT_LIST_DIR}/library/cmac.c
  ${CMAKE_CURRENT_LIST_DIR}/library/constant_time.c
  ${CMAKE_CURRENT_LIST_DIR}/library/ctr_drbg.c
  ${CMAKE_CURRENT_LIST_DIR}/library/des.c
  ${CMAKE_CURRENT_LIST_DIR}/library/dhm.c
  ${CMAKE_CURRENT_LIST_DIR}/library/ecdh.c
  ${CMAKE_CURRENT_LIST_DIR}/library/ecdsa.c
  ${CMAKE_CURRENT_LIST_DIR}/library/ecjpake.c
  ${CMAKE_CURRENT_LIST_DIR}/library/ecp.c
  ${CMAKE_CURRENT_LIST_DIR}/library/ecp_curves.c
  ${CMAKE_CURRENT_LIST_DIR}/library/ecp_curves_new.c
  ${CMAKE_CURRENT_LIST_DIR}/library/entropy.c
  ${CMAKE_CURRENT_LIST_DIR}/library/entropy_poll.c
  ${CMAKE_CURRENT_LIST_DIR}/library/error.c
  ${CMAKE_CURRENT_LIST_DIR}/library/gcm.c
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
  ${CMAKE_CURRENT_LIST_DIR}/library/pk_ecc.c
  ${CMAKE_CURRENT_LIST_DIR}/library/pk_wrap.c
  ${CMAKE_CURRENT_LIST_DIR}/library/pkcs5.c
  ${CMAKE_CURRENT_LIST_DIR}/library/pkcs12.c
  ${CMAKE_CURRENT_LIST_DIR}/library/pkparse.c
  ${CMAKE_CURRENT_LIST_DIR}/library/pkwrite.c
  ${CMAKE_CURRENT_LIST_DIR}/library/platform.c
  ${CMAKE_CURRENT_LIST_DIR}/library/platform_util.c
  ${CMAKE_CURRENT_LIST_DIR}/library/poly1305.c
  ${CMAKE_CURRENT_LIST_DIR}/library/psa_util.c
  ${CMAKE_CURRENT_LIST_DIR}/library/ripemd160.c
  ${CMAKE_CURRENT_LIST_DIR}/library/rsa.c
  ${CMAKE_CURRENT_LIST_DIR}/library/rsa_alt_helpers.c
  ${CMAKE_CURRENT_LIST_DIR}/library/sha1.c
  ${CMAKE_CURRENT_LIST_DIR}/library/sha3.c
  ${CMAKE_CURRENT_LIST_DIR}/library/sha256.c
  ${CMAKE_CURRENT_LIST_DIR}/library/sha512.c
  ${CMAKE_CURRENT_LIST_DIR}/library/threading.c
  ${CMAKE_CURRENT_LIST_DIR}/library/timing.c
  ${CMAKE_CURRENT_LIST_DIR}/library/version.c
  ${CMAKE_CURRENT_LIST_DIR}/library/version_features.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/include
  ${CMAKE_CURRENT_LIST_DIR}/library
)

