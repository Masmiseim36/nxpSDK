
if(CONFIG_MCUX_COMPONENT_middleware.mbedtls.port.ele_s400)
    mcux_add_macro(
        "-DMBEDTLS_MCUX_ELE_S400_API -DMBEDTLS_CONFIG_FILE=\\\"ele_s400_mbedtls_config.h\\\""
    )
    mcux_add_source(
        SOURCES port/ele_s400/ele_mbedtls.c
                port/ele_s400/ele_mbedtls.h
                port/ele_s400/ele_s400_mbedtls_config.h
                port/ele_s400/ele_fw.h
                port/ele_s400/ele_entropy.c
                port/ele_s400/aes_alt.h
                port/ele_s400/aes_alt.c
                port/ele_s400/ccm_alt.h
                port/ele_s400/ccm_alt.c
                port/ele_s400/gcm_alt.h
                port/ele_s400/gcm_alt.c
                port/ele_s400/rsa_alt.h
                port/ele_s400/rsa_alt.c
                port/ele_s400/threading_alt.h
                port/ele_s400/sha256_alt.c
                port/ele_s400/sha256_alt.h
                port/ele_s400/sha512_alt.c
                port/ele_s400/sha512_alt.h
                port/ele_s400/hmac_alt.c
        BASE_PATH ${SdkRootDirPath}/middleware/mbedtls/
    )
    mcux_add_include(
        INCLUDES port/ele_s400
        BASE_PATH ${SdkRootDirPath}/middleware/mbedtls/
    )
endif()

if(CONFIG_MCUX_COMPONENT_middleware.mbedtls.port.ele_s400_ecc_opaque)
    mcux_add_macro(
        "-DMBEDTLS_MCUX_ELE_S400_API -DMBEDTLS_CONFIG_FILE=\\\"ele_ecc_opaque_mbedtls_config.h\\\""
    )
    mcux_add_source(
        SOURCES port/ele_s400/ele_fw.h
                port/ele_s400/ele_entropy.c
                port/ele_s400/aes_alt.h
                port/ele_s400/aes_alt.c
                port/ele_s400/ccm_alt.h
                port/ele_s400/ccm_alt.c
                port/ele_s400/gcm_alt.h
                port/ele_s400/gcm_alt.c
                port/ele_s400/rsa_alt.h
                port/ele_s400/rsa_alt.c
                port/ele_s400/threading_alt.h
                port/ele_s400/sha256_alt.c
                port/ele_s400/sha256_alt.h
                port/ele_s400/sha512_alt.c
                port/ele_s400/sha512_alt.h
                port/ele_s400/hmac_alt.c
                port/ele_s400/ecc_opaque/ele_ecc_opaque_mbedtls_config.h
                port/ele_s400/ecc_opaque/ele_mbedtls.c
                port/ele_s400/ecc_opaque/ele_mbedtls.h
                port/ele_s400/ecc_opaque/ecdsa_alt.h
                port/ele_s400/ecc_opaque/ecdsa_alt.c
                port/ele_s400/ecc_opaque/pk_alt.c
        BASE_PATH ${SdkRootDirPath}/middleware/mbedtls/
    )
    mcux_add_include(
        INCLUDES port/ele_s400
                 port/ele_s400/ecc_opaque
        BASE_PATH ${SdkRootDirPath}/middleware/mbedtls/
    )
endif()

if(CONFIG_MCUX_COMPONENT_middleware.mbedtls.port.els_pkc)
    mcux_add_macro(
        "-DMBEDTLS_MCUX_ELS_PKC_API -DMBEDTLS_MCUX_USE_PKC -DMBEDTLS_CONFIG_FILE=\\\"els_pkc_mbedtls_config.h\\\""
    )
    mcux_add_source(
        SOURCES port/pkc/els_pkc_mbedtls_config.h
                port/pkc/els_pkc_mbedtls_thread_config.h
                port/pkc/ecc_alt.c
                port/pkc/ecc_alt.h
                port/pkc/ecdh_alt.c
                port/pkc/ecdsa_alt.c
                port/pkc/rsa_alt.c
                port/pkc/rsa_alt.h
                port/pkc/els_pkc_mbedtls.c
                port/pkc/els_pkc_mbedtls.h
                port/pkc/threading_alt.h
        BASE_PATH ${SdkRootDirPath}/middleware/mbedtls/
    )
    mcux_add_include(
        INCLUDES port/pkc
        BASE_PATH ${SdkRootDirPath}/middleware/mbedtls/
    )
endif()

if(CONFIG_MCUX_COMPONENT_middleware.mbedtls.port.els)
    mcux_add_macro(
        "-DMBEDTLS_MCUX_ELS_API -DMBEDTLS_MCUX_USE_ELS -DMCUXCL_FEATURE_CSSL_MEMORY_C_FALLBACK"
    )
    mcux_add_source(
        SOURCES port/els/aes_alt.c
                port/els/aes_alt.h
                port/els/cbc_mac_alt.c
                port/els/cbc_mac_alt.h
                port/els/cmac_alt.h
                port/els/cmac_alt.c
                port/els/els_mbedtls_config.h
                port/els/ctr_drbg_alt.c
                port/els/ctr_drbg_alt.h
                port/els/gcm_alt.c
                port/els/gcm_alt.h
                port/els/platform_hw_ip.h
                port/els/sha256_alt.c
                port/els/sha256_alt.h
                port/els/sha512_alt.c
                port/els/sha512_alt.h
                port/els/entropy_poll_alt.c
                port/els/els_mbedtls.c
                port/els/els_mbedtls.h
        BASE_PATH ${SdkRootDirPath}/middleware/mbedtls/
    )
    mcux_add_include(
        INCLUDES port/els
        BASE_PATH ${SdkRootDirPath}/middleware/mbedtls/
    )
endif()

if(CONFIG_MCUX_COMPONENT_middleware.mbedtls.port.sssapi)
    mcux_add_macro(
        "-DMBEDTLS_NXP_SSSAPI -DMBEDTLS_NXP_ELE200 -DMBEDTLS_CONFIG_FILE=\\\"sssapi_mbedtls_config.h\\\""
    )
    mcux_add_source(
        SOURCES port/sssapi/sssapi_mbedtls.c
                port/sssapi/sssapi_mbedtls.h
                port/sssapi/sssapi_mbedtls_config.h
                port/sssapi/aes_alt.c
                port/sssapi/aes_alt.h
                port/sssapi/ccm_alt.h
                port/sssapi/ccm_alt.c
                port/sssapi/cmac_alt.h
                port/sssapi/cmac_alt.c
                port/sssapi/sha256_alt.h
                port/sssapi/sha256_alt.c
                port/sssapi/sha512_alt.h
                port/sssapi/sha512_alt.c
                port/sssapi/ecdh_alt.h
                port/sssapi/ecdh_alt.c
                port/sssapi/ecdsa_alt.h
                port/sssapi/ecdsa_alt.c
                port/sssapi/entropy_poll_alt.c
                port/sssapi/threading_alt.h
        BASE_PATH ${SdkRootDirPath}/middleware/mbedtls/
    )
    mcux_add_include(
        INCLUDES port/sssapi
        BASE_PATH ${SdkRootDirPath}/middleware/mbedtls/
    )
endif()

if(CONFIG_MCUX_COMPONENT_middleware.mbedtls.port.ksdk)
    mcux_add_macro(
        "-DMBEDTLS_CONFIG_FILE=\\\"ksdk_mbedtls_config.h\\\""
    )
    mcux_add_source(
        SOURCES port/ksdk/ksdk_mbedtls.c
                port/ksdk/ksdk_mbedtls.h
                port/ksdk/ksdk_mbedtls_config.h
                port/ksdk/sha1_alt.h
                port/ksdk/sha256_alt.h
                port/ksdk/des_alt.h
                port/ksdk/des_alt.c
                port/ksdk/aes_alt.h
                port/ksdk/aes_alt.c
                port/ksdk/ccm_alt.h
                port/ksdk/ecp_alt.h
                port/ksdk/ecp_alt.c
                port/ksdk/ecp_curves_alt.c
                port/ksdk/ecp_alt_ksdk.c
                port/ksdk/threading_alt.h
        BASE_PATH ${SdkRootDirPath}/middleware/mbedtls/
    )
    mcux_add_include(
        INCLUDES port/ksdk
        BASE_PATH ${SdkRootDirPath}/middleware/mbedtls/
    )
endif()

if(CONFIG_MCUX_COMPONENT_middleware.mbedtls)
    mcux_add_iar_configuration(
        CC  "--diag_suppress Pa167,Pe177,Pe191,Pe546"
    )
    mcux_add_armgcc_configuration(
        CC  "-fomit-frame-pointer -Wno-unused-function"
    )
    mcux_add_source(
        SOURCES include/mbedtls/aes.h
                include/mbedtls/aesni.h
                include/mbedtls/arc4.h
                include/mbedtls/aria.h
                include/mbedtls/asn1.h
                include/mbedtls/asn1write.h
                include/mbedtls/base64.h
                include/mbedtls/bignum.h
                include/mbedtls/blowfish.h
                include/mbedtls/bn_mul.h
                include/mbedtls/camellia.h
                include/mbedtls/ccm.h
                include/mbedtls/certs.h
                include/mbedtls/chacha20.h
                include/mbedtls/chachapoly.h
                include/mbedtls/check_config.h
                include/mbedtls/cipher.h
                include/mbedtls/cipher_internal.h
                include/mbedtls/cmac.h
                include/mbedtls/compat-1.3.h
                include/mbedtls/config.h
                include/mbedtls/config_psa.h
                include/mbedtls/constant_time.h
                include/mbedtls/ctr_drbg.h
                include/mbedtls/debug.h
                include/mbedtls/des.h
                include/mbedtls/dhm.h
                include/mbedtls/ecdh.h
                include/mbedtls/ecdsa.h
                include/mbedtls/ecjpake.h
                include/mbedtls/ecp.h
                include/mbedtls/ecp_internal.h
                include/mbedtls/entropy.h
                include/mbedtls/entropy_poll.h
                include/mbedtls/error.h
                include/mbedtls/gcm.h
                include/mbedtls/havege.h
                include/mbedtls/hkdf.h
                include/mbedtls/hmac_drbg.h
                include/mbedtls/md.h
                include/mbedtls/md_internal.h
                include/mbedtls/md2.h
                include/mbedtls/md4.h
                include/mbedtls/md5.h
                include/mbedtls/memory_buffer_alloc.h
                include/mbedtls/net.h
                include/mbedtls/net_sockets.h
                include/mbedtls/nist_kw.h
                include/mbedtls/oid.h
                include/mbedtls/padlock.h
                include/mbedtls/pem.h
                include/mbedtls/pk.h
                include/mbedtls/pk_internal.h
                include/mbedtls/pkcs5.h
                include/mbedtls/pkcs11.h
                include/mbedtls/pkcs12.h
                include/mbedtls/platform.h
                include/mbedtls/platform_time.h
                include/mbedtls/platform_util.h
                include/mbedtls/poly1305.h
                include/mbedtls/psa_util.h
                include/mbedtls/ripemd160.h
                include/mbedtls/rsa.h
                include/mbedtls/rsa_internal.h
                include/mbedtls/sha1.h
                include/mbedtls/sha256.h
                include/mbedtls/sha512.h
                include/mbedtls/ssl.h
                include/mbedtls/ssl_cache.h
                include/mbedtls/ssl_ciphersuites.h
                include/mbedtls/ssl_cookie.h
                include/mbedtls/ssl_internal.h
                include/mbedtls/ssl_ticket.h
                include/mbedtls/threading.h
                include/mbedtls/timing.h
                include/mbedtls/version.h
                include/mbedtls/x509.h
                include/mbedtls/x509_crl.h
                include/mbedtls/x509_crt.h
                include/mbedtls/x509_csr.h
                include/mbedtls/xtea.h
                include/psa/crypto.h
                include/psa/crypto_builtin_composites.h
                include/psa/crypto_builtin_primitives.h
                include/psa/crypto_compat.h
                include/psa/crypto_config.h
                include/psa/crypto_driver_common.h
                include/psa/crypto_driver_contexts_composites.h
                include/psa/crypto_driver_contexts_primitives.h
                include/psa/crypto_extra.h
                include/psa/crypto_platform.h
                include/psa/crypto_se_driver.h
                include/psa/crypto_sizes.h
                include/psa/crypto_struct.h
                include/psa/crypto_types.h
                include/psa/crypto_values.h
                library/aes.c
                library/aesni.c
                library/arc4.c
                library/aria.c
                library/asn1parse.c
                library/asn1write.c
                library/base64.c
                library/bignum_internal.h
                library/bignum.c
                library/blowfish.c
                library/camellia.c
                library/ccm.c
                library/certs.c
                library/chacha20.c
                library/chachapoly.c
                library/check_crypto_config.h
                library/cipher.c
                library/cipher_wrap.c
                library/cmac.c
                library/common.h
                library/constant_time.c
                library/constant_time_internal.h
                library/constant_time_invasive.h
                library/ctr_drbg.c
                library/debug.c
                library/des.c
                library/dhm.c
                library/ecdh.c
                library/ecdsa.c
                library/ecjpake.c
                library/ecp.c
                library/ecp_curves.c
                library/ecp_invasive.h
                library/entropy.c
                library/entropy_poll.c
                library/error.c
                library/gcm.c
                library/havege.c
                library/hkdf.c
                library/hmac_drbg.c
                library/md.c
                library/md2.c
                library/md4.c
                library/md5.c
                library/memory_buffer_alloc.c
                library/mps_common.h
                library/mps_error.h
                library/mps_reader.c
                library/mps_reader.h
                library/mps_trace.c
                library/mps_trace.h
                library/net_sockets.c
                library/nist_kw.c
                library/oid.c
                library/padlock.c
                library/pem.c
                library/pk.c
                library/pk_wrap.c
                library/pkcs5.c
                library/pkcs11.c
                library/pkcs12.c
                library/pkparse.c
                library/pkwrite.c
                library/platform.c
                library/platform_util.c
                library/poly1305.c
                library/psa_crypto.c
                library/psa_crypto_aead.c
                library/psa_crypto_aead.h
                library/psa_crypto_cipher.c
                library/psa_crypto_cipher.h
                library/psa_crypto_client.c
                library/psa_crypto_core.h
                library/psa_crypto_driver_wrappers.c
                library/psa_crypto_driver_wrappers.h
                library/psa_crypto_ecp.c
                library/psa_crypto_ecp.h
                library/psa_crypto_hash.c
                library/psa_crypto_hash.h
                library/psa_crypto_invasive.h
                library/psa_crypto_its.h
                library/psa_crypto_mac.c
                library/psa_crypto_mac.h
                library/psa_crypto_random_impl.h
                library/psa_crypto_rsa.c
                library/psa_crypto_rsa.h
                library/psa_crypto_se.c
                library/psa_crypto_se.h
                library/psa_crypto_slot_management.c
                library/psa_crypto_slot_management.h
                library/psa_crypto_storage.c
                library/psa_crypto_storage.h
                library/psa_its_file.c
                library/ripemd160.c
                library/rsa.c
                library/rsa_internal.c
                library/sha1.c
                library/sha256.c
                library/sha512.c
                library/ssl_cache.c
                library/ssl_ciphersuites.c
                library/ssl_cli.c
                library/ssl_cookie.c
                library/ssl_msg.c
                library/ssl_srv.c
                library/ssl_ticket.c
                library/ssl_tls.c
                library/ssl_tls13_keys.c
                library/ssl_tls13_keys.h
                library/threading.c
                library/timing.c
                library/version.c
                library/version_features.c
                library/x509.c
                library/x509_create.c
                library/x509_crl.c
                library/x509_crt.c
                library/x509_csr.c
                library/x509write_crt.c
                library/x509write_csr.c
                library/xtea.c
        BASE_PATH ${SdkRootDirPath}/middleware/mbedtls/
    )
    mcux_add_include(
        INCLUDES include
                 library
        BASE_PATH ${SdkRootDirPath}/middleware/mbedtls/
    )
endif()

if(CONFIG_MCUX_PRJSEG_middleware.mbedtls.default_heap_stack)
    mcux_add_mdk_configuration(
        CC  "--diag_suppress=177"
    )
endif()

if(CONFIG_MCUX_COMPONENT_middleware.mbedtls.tests)
    mcux_add_macro(
        "-DMBEDTLS_CONFIG_FILE=\\\"mcux_mbedtls_testsuite_config.h\\\""
    )
    mcux_add_source(
        SOURCES tests/src/asn1_helpers.c
                tests/src/helpers.c
                tests/src/psa_crypto_helpers.c
                tests/src/psa_exercise_key.c
                tests/src/random.c
                tests/src/threading_helpers.c
                tests/src/fake_external_rng_for_test.c
                tests/src/test_helpers/ssl_helpers.c
                tests/include/test/arguments.h
                tests/include/test/asn1_helpers.h
                tests/include/test/constant_flow.h
                tests/include/test/fake_external_rng_for_test.h
                tests/include/test/helpers.h
                tests/include/test/macros.h
                tests/include/test/psa_crypto_helpers.h
                tests/include/test/psa_exercise_key.h
                tests/include/test/psa_helpers.h
                tests/include/test/random.h
                tests/include/test/ssl_helpers.h
        BASE_PATH ${SdkRootDirPath}/middleware/mbedtls/
    )
    mcux_add_include(
        INCLUDES tests/include/test
                 tests/include
        BASE_PATH ${SdkRootDirPath}/middleware/mbedtls/
    )
endif()

if(CONFIG_MCUX_COMPONENT_middleware.mbedtls.3rdparty)
    mcux_add_source(
        SOURCES 3rdparty/*.*
                3rdparty/everest/**
        BASE_PATH ${SdkRootDirPath}/middleware/mbedtls/
    )
    mcux_add_include(
        INCLUDES 3rdparty/everest/include
                 3rdparty/everest/include/everest
                 3rdparty/everest/include/everest/kremlib
                 3rdparty/everest/include/everest/kremlin
                 3rdparty/everest/include/everest/kremlin/internal
        BASE_PATH ${SdkRootDirPath}/middleware/mbedtls/
    )
endif()
