# Add set(CONFIG_USE_middleware_freertos_corepkcs11 true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_pkcs11 AND CONFIG_USE_middleware_freertos_corepkcs11_template)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/source/core_pkcs11.c
  ${CMAKE_CURRENT_LIST_DIR}/source/core_pki_utils.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/source/include
)

else()

message(SEND_ERROR "middleware_freertos_corepkcs11 dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
