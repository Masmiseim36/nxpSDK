
if(CONFIG_MCUX_COMPONENT_middleware.mbedtls3x.template)
    mcux_component_version(${mbedtls3x_version})
    mcux_add_configuration(
        CC  "-DMBEDTLS_CONFIG_FILE=\\\"mcux_mbedtls_config.h\\\""
    )
    mcux_add_source(
        SOURCES port/mcux_mbedtls_config.h
                port/mcux_mbedtls_accelerator_config.h
        BASE_PATH ${SdkRootDirPath}/middleware/mbedtls3x/
    )
    mcux_add_include(
        TARGET_FILES mcux_mbedtls_config.h
                     mcux_mbedtls_accelerator_config.h
                     mcux_psa_defines.h
        INCLUDES port
        BASE_PATH ${SdkRootDirPath}/middleware/mbedtls3x/
    )
endif()

if(CONFIG_MCUX_COMPONENT_middleware.mbedtls3x.port.config)
    mcux_component_version(${mbedtls3x_version})
    mcux_add_configuration(
        CC  "-DMBEDTLS_CONFIG_FILE=\\\"mcux_mbedtls_config.h\\\""
    )
    mcux_add_source(
        SOURCES port/mcux_mbedtls_config.h
                port/mcux_mbedtls_accelerator_config.h
                port/mcux_psa_defines.h
        BASE_PATH ${SdkRootDirPath}/middleware/mbedtls3x/
    )
    mcux_add_include(
        INCLUDES port
        BASE_PATH ${SdkRootDirPath}/middleware/mbedtls3x/
    )
endif()

if(CONFIG_MCUX_COMPONENT_middleware.mbedtls3x.port.psa_crypto_config)
    mcux_component_version(${mbedtls3x_version})
    mcux_add_configuration(
        CC  "-DMBEDTLS_CONFIG_FILE=\\\"mcux_mbedtls_psa_crypto_config.h\\\""
    )
    mcux_add_source(
        SOURCES port/mcux_mbedtls_psa_crypto_config.h
                port/mcux_mbedtls_accelerator_config.h
                port/mcux_psa_defines.h
        BASE_PATH ${SdkRootDirPath}/middleware/mbedtls3x/
    )
    mcux_add_include(
        INCLUDES port
        BASE_PATH ${SdkRootDirPath}/middleware/mbedtls3x/
    )
endif()

if(CONFIG_MCUX_COMPONENT_middleware.mbedtls3x.port.hw_init)
    mcux_component_version(${mbedtls3x_version})
    mcux_add_source(
        SOURCES port/hw_init/hw_init.c
                port/hw_init/hw_init.h
        BASE_PATH ${SdkRootDirPath}/middleware/mbedtls3x/
    )
    mcux_add_include(
        INCLUDES port/hw_init
        BASE_PATH ${SdkRootDirPath}/middleware/mbedtls3x/
    )
endif()

if(CONFIG_MCUX_COMPONENT_middleware.mbedtls3x.port.rng)
    mcux_component_version(${mbedtls3x_version})
    mcux_add_source(
        SOURCES port/rng/psa_mcux_entropy.c
        BASE_PATH ${SdkRootDirPath}/middleware/mbedtls3x/
    )
    mcux_add_include(
        INCLUDES port/rng
        BASE_PATH ${SdkRootDirPath}/middleware/mbedtls3x/
    )
endif()

if(CONFIG_MCUX_COMPONENT_middleware.mbedtls3x.port.tfm)
    mcux_component_version(${mbedtls3x_version})
    mcux_add_source(
        SOURCES port/tfm/entropy_poll_alt.c
        BASE_PATH ${SdkRootDirPath}/middleware/mbedtls3x/
    )
    mcux_add_include(
        INCLUDES port/tfm
        BASE_PATH ${SdkRootDirPath}/middleware/mbedtls3x/
    )
endif()

if(CONFIG_MCUX_COMPONENT_middleware.mbedtls3x.tests)
    mcux_component_version(${mbedtls3x_version})
    mcux_add_source(
        SOURCES tests/src/asn1_helpers.c
                tests/src/bignum_helpers.c
                tests/src/certs.c
                tests/src/helpers.c
                tests/src/psa_crypto_helpers.c
                tests/src/psa_crypto_stubs.c
                tests/src/psa_exercise_key.c
                tests/src/psa_memory_poisoning_wrappers.c
                tests/src/psa_test_wrappers.c
                tests/src/random.c
                tests/src/test_memory.c
                tests/src/threading_helpers.c
                tests/src/fake_external_rng_for_test.c
                tests/src/test_helpers/ssl_helpers.c
                tests/include/test/asn1_helpers.h
                tests/include/test/bignum_helpers.h
                tests/include/test/certs.h
                tests/include/test/constant_flow.h
                tests/include/test/fake_external_rng_for_test.h
                tests/include/test/helpers.h
                tests/include/test/macros.h
                tests/include/test/memory.h
                tests/include/test/psa_crypto_helpers.h
                tests/include/test/psa_exercise_key.h
                tests/include/test/psa_helpers.h
                tests/include/test/psa_memory_poisoning_wrappers.h
                tests/include/test/psa_test_wrappers.h
                tests/include/test/random.h
                tests/include/test/ssl_helpers.h
                tests/include/test/threading_helpers.h
        BASE_PATH ${SdkRootDirPath}/middleware/mbedtls3x/
    )
    mcux_add_include(
        INCLUDES tests/include/test
                 tests/include
        BASE_PATH ${SdkRootDirPath}/middleware/mbedtls3x/
    )
endif()

if(CONFIG_MCUX_COMPONENT_middleware.mbedtls3x.crypto_storage_default)
    mcux_component_version(${mbedtls3x_version})
    mcux_add_source(
        SOURCES library/psa_its_file.c
                library/psa_crypto_its.h
        BASE_PATH ${SdkRootDirPath}/middleware/mbedtls3x/
    )
    mcux_add_include(
        INCLUDES include
                 library
        BASE_PATH ${SdkRootDirPath}/middleware/mbedtls3x/
    )
endif()

if(CONFIG_MCUX_COMPONENT_middleware.mbedtls3x.crypto_storage_fatfs)
    mcux_component_version(${mbedtls3x_version})
    mcux_add_configuration(
        CC  "-DMBEDTLS_PSA_ITS_FILE_FATFS"
    )
    mcux_add_source(
        SOURCES port/psa_its/psa_its_fatfs.c
                port/psa_its/psa_its_fatfs.h
                library/psa_crypto_its.h
        BASE_PATH ${SdkRootDirPath}/middleware/mbedtls3x/
    )
    mcux_add_include(
        INCLUDES include
                 library
                 port/psa_its
        BASE_PATH ${SdkRootDirPath}/middleware/mbedtls3x/
    )
endif()

if(CONFIG_MCUX_COMPONENT_middleware.mbedtls3x.crypto_storage_ram)
    mcux_component_version(${mbedtls3x_version})
    mcux_add_configuration(
        CC  "-DMBEDTLS_PSA_ITS_RAM"
    )
    mcux_add_source(
        SOURCES port/psa_its/psa_its_ram.c
                library/psa_crypto_its.h
        BASE_PATH ${SdkRootDirPath}/middleware/mbedtls3x/
    )
    mcux_add_include(
        INCLUDES include
                 library
        BASE_PATH ${SdkRootDirPath}/middleware/mbedtls3x/
    )
endif()

if(CONFIG_MCUX_COMPONENT_middleware.mbedtls3x.crypto.no_psa)
    mcux_component_version(${mbedtls3x_version})
    mcux_add_source(
        SOURCES include/mbedtls/aes.h
                include/mbedtls/aria.h
                include/mbedtls/asn1.h
                include/mbedtls/asn1write.h
                include/mbedtls/base64.h
                include/mbedtls/bignum.h
                include/mbedtls/block_cipher.h
                include/mbedtls/build_info.h
                include/mbedtls/camellia.h
                include/mbedtls/ccm.h
                include/mbedtls/chacha20.h
                include/mbedtls/chachapoly.h
                include/mbedtls/check_config.h
                include/mbedtls/cipher.h
                include/mbedtls/cmac.h
                include/mbedtls/compat-2.x.h
                include/mbedtls/config_adjust_legacy_crypto.h
                include/mbedtls/config_adjust_legacy_from_psa.h
                include/mbedtls/config_adjust_psa_from_legacy.h
                include/mbedtls/config_adjust_psa_superset_legacy.h
                include/mbedtls/config_adjust_ssl.h
                include/mbedtls/config_adjust_x509.h
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
                include/mbedtls/entropy.h
                include/mbedtls/error.h
                include/mbedtls/gcm.h
                include/mbedtls/hkdf.h
                include/mbedtls/hmac_drbg.h
                include/mbedtls/lms.h
                include/mbedtls/mbedtls_config.h
                include/mbedtls/md.h
                include/mbedtls/md5.h
                include/mbedtls/memory_buffer_alloc.h
                include/mbedtls/nist_kw.h
                include/mbedtls/oid.h
                include/mbedtls/pem.h
                include/mbedtls/pk.h
                include/mbedtls/pkcs5.h
                include/mbedtls/pkcs12.h
                include/mbedtls/platform.h
                include/mbedtls/platform_time.h
                include/mbedtls/platform_util.h
                include/mbedtls/poly1305.h
                include/mbedtls/private_access.h
                include/mbedtls/psa_util.h
                include/mbedtls/ripemd160.h
                include/mbedtls/rsa.h
                include/mbedtls/sha1.h
                include/mbedtls/sha3.h
                include/mbedtls/sha256.h
                include/mbedtls/sha512.h
                include/mbedtls/threading.h
                include/mbedtls/timing.h
                include/mbedtls/version.h
                include/psa/build_info.h
                include/psa/crypto.h
                include/psa/crypto_adjust_auto_enabled.h
                include/psa/crypto_adjust_config_key_pair_types.h
                include/psa/crypto_adjust_config_synonyms.h
                include/psa/crypto_builtin_composites.h
                include/psa/crypto_builtin_key_derivation.h
                include/psa/crypto_builtin_primitives.h
                include/psa/crypto_compat.h
                include/psa/crypto_config.h
                include/psa/crypto_driver_common.h
                include/psa/crypto_driver_contexts_composites.h
                include/psa/crypto_driver_contexts_key_derivation.h
                include/psa/crypto_driver_contexts_primitives.h
                include/psa/crypto_extra.h
                include/psa/crypto_legacy.h
                include/psa/crypto_platform.h
                include/psa/crypto_se_driver.h
                include/psa/crypto_sizes.h
                include/psa/crypto_struct.h
                include/psa/crypto_types.h
                include/psa/crypto_values.h
                library/aes.c
                library/aesni.c
                library/aesni.h
                library/alignment.h
                library/aesce.c
                library/aesce.h
                library/aria.c
                library/asn1parse.c
                library/asn1write.c
                library/base64.c
                library/base64_internal.h
                library/bignum.c
                library/bignum_core.h
                library/bignum_core.c
                library/bignum_mod.c
                library/bignum_mod.h
                library/bignum_mod_raw.c
                library/bignum_mod_raw.h
                library/bignum_mod_raw_invasive.h
                library/block_cipher.c
                library/block_cipher_internal.h
                library/bn_mul.h
                library/camellia.c
                library/ccm.c
                library/chacha20.c
                library/chachapoly.c
                library/check_crypto_config.h
                library/cipher.c
                library/cipher_wrap.c
                library/cipher_wrap.h
                library/cmac.c
                library/common.h
                library/constant_time.c
                library/constant_time_impl.h
                library/constant_time_internal.h
                library/ctr_drbg.c
                library/ctr.h
                library/des.c
                library/dhm.c
                library/ecdh.c
                library/ecdsa.c
                library/ecjpake.c
                library/ecp.c
                library/ecp_curves.c
                library/ecp_curves_new.c
                library/ecp_internal_alt.h
                library/ecp_invasive.h
                library/entropy.c
                library/entropy_poll.c
                library/entropy_poll.h
                library/error.c
                library/gcm.c
                library/hkdf.c
                library/hmac_drbg.c
                library/lmots.c
                library/lmots.h
                library/lms.c
                library/md.c
                library/md_psa.h
                library/md_wrap.h
                library/md5.c
                library/memory_buffer_alloc.c
                library/nist_kw.c
                library/oid.c
                library/padlock.c
                library/padlock.h
                library/pem.c
                library/pk.c
                library/pk_ecc.c
                library/pk_internal.h
                library/pk_wrap.c
                library/pk_wrap.h
                library/pkcs5.c
                library/pkcs12.c
                library/pkparse.c
                library/pkwrite.c
                library/pkwrite.h
                library/platform.c
                library/platform_util.c
                library/poly1305.c
                library/psa_crypto_aead.h
                library/psa_crypto_cipher.h
                library/psa_crypto_core.h
                library/psa_crypto_core_common.h
                library/psa_crypto_driver_wrappers_no_static.h
                library/psa_crypto_driver_wrappers.h
                library/psa_crypto_ecp.h
                library/psa_crypto_ffdh.h
                library/psa_crypto_hash.h
                library/psa_crypto_invasive.h
                library/psa_crypto_its.h
                library/psa_crypto_mac.h
                library/psa_crypto_pake.h
                library/psa_crypto_random_impl.h
                library/psa_crypto_rsa.h
                library/psa_crypto_se.h
                library/psa_crypto_slot_management.h
                library/psa_crypto_storage.h
                library/psa_util_internal.h
                library/psa_util.c
                library/ripemd160.c
                library/rsa.c
                library/rsa_alt_helpers.c
                library/rsa_alt_helpers.h
                library/rsa_internal.h
                library/sha1.c
                library/sha3.c
                library/sha256.c
                library/sha512.c
                library/threading.c
                library/timing.c
                library/version.c
                library/version_features.c
        BASE_PATH ${SdkRootDirPath}/middleware/mbedtls3x/
    )
    mcux_add_include(
        INCLUDES include
                 library
        BASE_PATH ${SdkRootDirPath}/middleware/mbedtls3x/
    )
endif()

if(CONFIG_MCUX_COMPONENT_middleware.mbedtls3x.crypto)
    mcux_component_version(${mbedtls3x_version})
    mcux_add_source(
        SOURCES library/psa_crypto.c
                library/psa_crypto_aead.c
                library/psa_crypto_cipher.c
                library/psa_crypto_client.c
                library/psa_crypto_driver_wrappers_no_static.c
                library/psa_crypto_ecp.c
                library/psa_crypto_ffdh.c
                library/psa_crypto_hash.c
                library/psa_crypto_mac.c
                library/psa_crypto_pake.c
                library/psa_crypto_rsa.c
                library/psa_crypto_se.c
                library/psa_crypto_slot_management.c
                library/psa_crypto_storage.c
        BASE_PATH ${SdkRootDirPath}/middleware/mbedtls3x/
    )
    mcux_add_include(
        INCLUDES include
                 library
        BASE_PATH ${SdkRootDirPath}/middleware/mbedtls3x/
    )
endif()

if(CONFIG_MCUX_COMPONENT_middleware.mbedtls3x.x509)
    mcux_component_version(${mbedtls3x_version})
    mcux_add_source(
        SOURCES include/mbedtls/pkcs7.h
                include/mbedtls/x509.h
                include/mbedtls/x509_crl.h
                include/mbedtls/x509_crt.h
                include/mbedtls/x509_csr.h
                library/x509.c
                library/x509_create.c
                library/x509_crl.c
                library/x509_crt.c
                library/x509_csr.c
                library/x509write.c
                library/x509write_crt.c
                library/x509write_csr.c
                library/x509_internal.h
                library/pkcs7.c
        BASE_PATH ${SdkRootDirPath}/middleware/mbedtls3x/
    )
    mcux_add_include(
        INCLUDES include
                 library
        BASE_PATH ${SdkRootDirPath}/middleware/mbedtls3x/
    )
endif()

if(CONFIG_MCUX_COMPONENT_middleware.mbedtls3x.ssl.no_psa)
    mcux_component_version(${mbedtls3x_version})
    mcux_add_source(
        SOURCES include/mbedtls/ssl.h
                include/mbedtls/ssl_cache.h
                include/mbedtls/ssl_ciphersuites.h
                include/mbedtls/ssl_cookie.h
                include/mbedtls/ssl_ticket.h
                include/mbedtls/net_sockets.h
                library/debug.c
                library/debug_internal.h
                library/mps_common.h
                library/mps_error.h
                library/mps_reader.c
                library/mps_reader.h
                library/mps_trace.c
                library/mps_trace.h
                library/net_sockets.c
                library/ssl_cache.c
                library/ssl_ciphersuites.c
                library/ssl_ciphersuites_internal.h
                library/ssl_client.c
                library/ssl_client.h
                library/ssl_cookie.c
                library/ssl_debug_helpers.h
                library/ssl_debug_helpers_generated.c
                library/ssl_misc.h
                library/ssl_msg.c
                library/ssl_ticket.c
                library/ssl_tls.c
                library/ssl_tls12_client.c
                library/ssl_tls12_server.c
                library/ssl_tls13_client.c
                library/ssl_tls13_generic.c
                library/ssl_tls13_invasive.h
                library/ssl_tls13_keys.c
                library/ssl_tls13_keys.h
                library/ssl_tls13_server.c
        BASE_PATH ${SdkRootDirPath}/middleware/mbedtls3x/
    )
    mcux_add_include(
        INCLUDES include
                 library
        BASE_PATH ${SdkRootDirPath}/middleware/mbedtls3x/
    )
endif()

if(CONFIG_MCUX_COMPONENT_middleware.mbedtls3x.ssl)
    mcux_component_version(${mbedtls3x_version})
    mcux_add_include(
        INCLUDES include
                 library
        BASE_PATH ${SdkRootDirPath}/middleware/mbedtls3x/
    )
endif()

if(CONFIG_MCUX_COMPONENT_middleware.mbedtls3x.no_psa)
    mcux_component_version(${mbedtls3x_version})
    mcux_add_include(
        INCLUDES include
                 library
        BASE_PATH ${SdkRootDirPath}/middleware/mbedtls3x/
    )
endif()

if(CONFIG_MCUX_COMPONENT_middleware.mbedtls3x)
    mcux_component_version(${mbedtls3x_version})
    mcux_add_include(
        INCLUDES include
                 library
        BASE_PATH ${SdkRootDirPath}/middleware/mbedtls3x/
    )
endif()

if(CONFIG_MCUX_PRJSEG_middleware.mbedtls3x.default_heap_stack)
endif()
