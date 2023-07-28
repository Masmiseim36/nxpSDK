# Add set(CONFIG_USE_middleware_se_hostlib_mbedtls_sss true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_se_hostlib_commonSe050 AND CONFIG_USE_middleware_se_hostlib_mbedtls_alt_demo_common)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/sss/plugin/mbedtls/ecdh_alt_ax.c
  ${CMAKE_CURRENT_LIST_DIR}/sss/plugin/mbedtls/ecdsa_verify_alt.c
  ${CMAKE_CURRENT_LIST_DIR}/sss/plugin/mbedtls/ecp_alt_sss.c
  ${CMAKE_CURRENT_LIST_DIR}/sss/plugin/mbedtls/sss_mbedtls.c
  ${CMAKE_CURRENT_LIST_DIR}/sss/plugin/mbedtls/sss_mbedtls_rsa.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/sss/plugin/mbedtls
  ${CMAKE_CURRENT_LIST_DIR}/sss/port/ksdk
)

if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

  target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
    -DMBEDTLS_CONFIG_FILE="ksdk_mbedtls_config.h"
    -DMBEDTLS_USER_CONFIG_FILE="sss_ksdk_mbedtls_config.h"
    -DMBEDTLS
    -DSCP_MODE=C_MAC_C_ENC_R_MAC_R_ENC
  )

endif()

else()

message(SEND_ERROR "middleware_se_hostlib_mbedtls_sss dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
