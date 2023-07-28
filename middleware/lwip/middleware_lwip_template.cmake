# Add set(CONFIG_USE_middleware_lwip_template true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

add_config_file(${CMAKE_CURRENT_LIST_DIR}/template/lwipopts.h ${CMAKE_CURRENT_LIST_DIR}/template middleware_lwip_template)

