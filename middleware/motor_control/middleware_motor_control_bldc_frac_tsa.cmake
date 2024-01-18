# Add set(CONFIG_USE_middleware_motor_control_bldc_frac_tsa true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_fmstr AND (CONFIG_CORE STREQUAL cm4f OR CONFIG_CORE STREQUAL cm7f OR CONFIG_CORE STREQUAL cm33))

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/bldc/fm_tsa_bldc.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/bldc
)

else()

message(SEND_ERROR "middleware_motor_control_bldc_frac_tsa dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
