# Add set(CONFIG_USE_middleware_edgefast_bluetooth_template true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

            add_config_file(${CMAKE_CURRENT_LIST_DIR}/source/impl/ethermind/controller/configs/mbedtls/mbedtls_config_client.h ${CMAKE_CURRENT_LIST_DIR}/source/impl/ethermind/controller/configs/mbedtls middleware_edgefast_bluetooth_template)
    
  

