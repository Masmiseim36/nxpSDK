# Add set(CONFIG_USE_middleware_freertos_corepkcs11_mbedtls_utils true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_pkcs11 AND CONFIG_USE_middleware_mbedtls)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/source/dependency/3rdparty/mbedtls_utils/mbedtls_utils.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/source/dependency/3rdparty/mbedtls_utils
)

else()

message(SEND_ERROR "middleware_freertos_corepkcs11_mbedtls_utils dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
