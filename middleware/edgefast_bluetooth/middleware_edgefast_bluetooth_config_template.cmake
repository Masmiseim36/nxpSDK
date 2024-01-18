# Add set(CONFIG_USE_middleware_edgefast_bluetooth_config_template true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

add_config_file(${CMAKE_CURRENT_LIST_DIR}/output/templates/config/edgefast_bluetooth_config.h ${CMAKE_CURRENT_LIST_DIR}/output/templates/config middleware_edgefast_bluetooth_config_template)
add_config_file(${CMAKE_CURRENT_LIST_DIR}/output/templates/config/edgefast_bluetooth_debug_config.h "" middleware_edgefast_bluetooth_config_template)
add_config_file(${CMAKE_CURRENT_LIST_DIR}/output/templates/app_bluetooth_config/app_bluetooth_config.h ${CMAKE_CURRENT_LIST_DIR}/output/templates/app_bluetooth_config middleware_edgefast_bluetooth_config_template)

