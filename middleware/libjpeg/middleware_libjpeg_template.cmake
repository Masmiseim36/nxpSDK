# Add set(CONFIG_USE_middleware_libjpeg_template true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

add_config_file(${CMAKE_CURRENT_LIST_DIR}/template/jconfig.h ${CMAKE_CURRENT_LIST_DIR}/template middleware_libjpeg_template)

