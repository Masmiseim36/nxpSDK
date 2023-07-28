# Add set(CONFIG_USE_middleware_motor_control_pmsm_float_template true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if((CONFIG_CORE STREQUAL cm4f OR CONFIG_CORE STREQUAL cm7f OR CONFIG_CORE STREQUAL cm33))

add_config_file(${CMAKE_CURRENT_LIST_DIR}/pmsm/pmsm_float/mc_cfg_template/m1_pmsm_appconfig.h ${CMAKE_CURRENT_LIST_DIR}/pmsm/pmsm_float/mc_cfg_template middleware_motor_control_pmsm_float_template)
add_config_file(${CMAKE_CURRENT_LIST_DIR}/pmsm/pmsm_float/mc_cfg_template/mc_periph_init.c "" middleware_motor_control_pmsm_float_template)
add_config_file(${CMAKE_CURRENT_LIST_DIR}/pmsm/pmsm_float/mc_cfg_template/mc_periph_init.h ${CMAKE_CURRENT_LIST_DIR}/pmsm/pmsm_float/mc_cfg_template middleware_motor_control_pmsm_float_template)

else()

message(SEND_ERROR "middleware_motor_control_pmsm_float_template dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
