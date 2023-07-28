# Add set(CONFIG_USE_middleware_tfm_s_profile_small true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_tfm_s_crypto AND CONFIG_USE_middleware_tfm_s_initial_attestation_symmetric AND CONFIG_USE_middleware_tfm_s_its)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/lib/ext/mbedcrypto/mbedcrypto_config
  ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/common
  ${CMAKE_CURRENT_LIST_DIR}/config/profile
)

if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

  target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
    -DTFM_LVL=1
    -DPROJECT_CONFIG_HEADER_FILE="config_profile_small.h"
    -DMBEDTLS_CONFIG_FILE="tfm_mbedcrypto_config_profile_small.h"
    -DMBEDTLS_PSA_CRYPTO_CONFIG_FILE="crypto_config_profile_small.h"
    -DSYMMETRIC_INITIAL_ATTESTATION
  )

endif()

else()

message(SEND_ERROR "middleware_tfm_s_profile_small dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
