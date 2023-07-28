# Add set(CONFIG_USE_middleware_se_hostlib_A71CHhostCrypto true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/hostlib/hostLib/libCommon/hostCrypto/axHostCryptombedtls.c
  ${CMAKE_CURRENT_LIST_DIR}/hostlib/hostLib/libCommon/hostCrypto/hcAsn.c
  ${CMAKE_CURRENT_LIST_DIR}/hostlib/hostLib/libCommon/hostCrypto/HostCryptoAPImbedtls.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/hostlib/hostLib/libCommon/hostCrypto
)

