# Add set(CONFIG_USE_middleware_motor_control_pmsm_float_template true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if((CONFIG_CORE STREQUAL cm4f OR CONFIG_CORE STREQUAL cm7f OR CONFIG_CORE STREQUAL cm33) AND (CONFIG_USE_middleware_rtcesl_cm33f OR CONFIG_USE_middleware_rtcesl_cm7f OR CONFIG_USE_middleware_rtcesl_cm4f))

add_config_file(${CMAKE_CURRENT_LIST_DIR}/pmsm/pmsm_float/mc_cfg_template/m1_pmsm_appconfig.h "" middleware_motor_control_pmsm_float_template)
add_config_file(${CMAKE_CURRENT_LIST_DIR}/pmsm/pmsm_float/mc_cfg_template/m2_pmsm_appconfig.h "" middleware_motor_control_pmsm_float_template)
add_config_file(${CMAKE_CURRENT_LIST_DIR}/pmsm/pmsm_float/mc_cfg_template/mc_periph_init.c "" middleware_motor_control_pmsm_float_template)
add_config_file(${CMAKE_CURRENT_LIST_DIR}/pmsm/pmsm_float/mc_cfg_template/mc_periph_init.h "" middleware_motor_control_pmsm_float_template)

else()

message(SEND_ERROR "middleware_motor_control_pmsm_float_template dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
