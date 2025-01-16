# Add set(CONFIG_USE_middleware_freertos_corepkcs11_template true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

            add_config_file(${CMAKE_CURRENT_LIST_DIR}/template/core_pkcs11_config.h "" middleware_freertos_corepkcs11_template)
    
  

