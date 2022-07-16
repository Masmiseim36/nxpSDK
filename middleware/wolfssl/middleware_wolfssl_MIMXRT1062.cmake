include_guard()
message("middleware_wolfssl component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/wolfcrypt/src/wc_port.c
    ${CMAKE_CURRENT_LIST_DIR}/wolfcrypt/src/wc_encrypt.c
    ${CMAKE_CURRENT_LIST_DIR}/wolfcrypt/src/random.c
    ${CMAKE_CURRENT_LIST_DIR}/wolfcrypt/src/logging.c
    ${CMAKE_CURRENT_LIST_DIR}/wolfcrypt/src/sp_int.c
    ${CMAKE_CURRENT_LIST_DIR}/wolfcrypt/src/sp_cortexm.c
    ${CMAKE_CURRENT_LIST_DIR}/wolfcrypt/src/ecc.c
    ${CMAKE_CURRENT_LIST_DIR}/wolfcrypt/src/rsa.c
    ${CMAKE_CURRENT_LIST_DIR}/wolfcrypt/src/aes.c
    ${CMAKE_CURRENT_LIST_DIR}/wolfcrypt/src/asn.c
    ${CMAKE_CURRENT_LIST_DIR}/wolfcrypt/src/sha.c
    ${CMAKE_CURRENT_LIST_DIR}/wolfcrypt/src/sha256.c
    ${CMAKE_CURRENT_LIST_DIR}/wolfcrypt/src/sha512.c
    ${CMAKE_CURRENT_LIST_DIR}/wolfcrypt/src/sha3.c
    ${CMAKE_CURRENT_LIST_DIR}/wolfcrypt/src/chacha.c
    ${CMAKE_CURRENT_LIST_DIR}/wolfcrypt/src/md5.c
    ${CMAKE_CURRENT_LIST_DIR}/wolfcrypt/src/hmac.c
    ${CMAKE_CURRENT_LIST_DIR}/wolfcrypt/src/coding.c
    ${CMAKE_CURRENT_LIST_DIR}/wolfcrypt/src/ge_low_mem.c
    ${CMAKE_CURRENT_LIST_DIR}/wolfcrypt/src/hash.c
    ${CMAKE_CURRENT_LIST_DIR}/wolfcrypt/src/pwdbased.c
    ${CMAKE_CURRENT_LIST_DIR}/wolfcrypt/src/wolfmath.c
    ${CMAKE_CURRENT_LIST_DIR}/wolfcrypt/src/misc.c
    ${CMAKE_CURRENT_LIST_DIR}/wolfcrypt/src/ed25519.c
    ${CMAKE_CURRENT_LIST_DIR}/wolfcrypt/src/fe_operations.c
    ${CMAKE_CURRENT_LIST_DIR}/wolfcrypt/src/fe_low_mem.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/.
)


include(utility_debug_console_MIMXRT1062)

include(middleware_wolfssl_docs_MIMXRT1062)

