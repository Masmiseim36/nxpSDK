# Add set(CONFIG_USE_middleware_fmstr_template_cfg_gen32le true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_fmstr_platform_gen32le)

add_config_file(${CMAKE_CURRENT_LIST_DIR}/src/template/gen32le/freemaster_cfg.h ${CMAKE_CURRENT_LIST_DIR}/src/template/gen32le middleware_fmstr_template_cfg_gen32le)

else()

message(SEND_ERROR "middleware_fmstr_template_cfg_gen32le dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
