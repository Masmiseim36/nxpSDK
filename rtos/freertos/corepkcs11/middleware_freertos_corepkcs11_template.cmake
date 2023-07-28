# Add set(CONFIG_USE_middleware_freertos_corepkcs11_template true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_freertos-kernel)

add_config_file(${CMAKE_CURRENT_LIST_DIR}/template/core_pkcs11_config.h ${CMAKE_CURRENT_LIST_DIR}/template middleware_freertos_corepkcs11_template)

else()

message(SEND_ERROR "middleware_freertos_corepkcs11_template dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
