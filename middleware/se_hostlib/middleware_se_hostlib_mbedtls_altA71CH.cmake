# Add set(CONFIG_USE_middleware_se_hostlib_mbedtls_altA71CH true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_se_hostlib_common_A71CH AND CONFIG_USE_middleware_se_hostlib_A71CHhostCrypto)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/hostlib/hostLib/mbedtls/src/ecdh_alt.c
  ${CMAKE_CURRENT_LIST_DIR}/hostlib/hostLib/mbedtls/src/ecdh_alt_ax.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/hostlib/hostLib/mbedtls/inc
  ${CMAKE_CURRENT_LIST_DIR}/hostlib/hostLib/libCommon/hostCrypto
)

if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

  target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
    -DMBEDTLS_CONFIG_FILE="ksdk_mbedtls_config.h"
    -DMBEDTLS_USER_CONFIG_FILE="sss_ksdk_mbedtls_config.h"
    -DMBEDTLS
  )

endif()

else()

message(SEND_ERROR "middleware_se_hostlib_mbedtls_altA71CH dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
