# Add set(CONFIG_USE_middleware_mc_pmsm_periph_framework_float true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if((CONFIG_CORE STREQUAL cm4f OR CONFIG_CORE STREQUAL cm7f OR CONFIG_CORE STREQUAL cm33) AND CONFIG_USE_middleware_motor_control_pmsm_float_template)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/freemaster
  ${CMAKE_CURRENT_LIST_DIR}/docs
)

else()

message(SEND_ERROR "middleware_mc_pmsm_periph_framework_float dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
