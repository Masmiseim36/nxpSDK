# Add set(CONFIG_USE_middleware_motor_control_pmsm_framework_float_tsa true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_fmstr AND CONFIG_USE_middleware_fmstr_platform_gen32le AND (CONFIG_CORE STREQUAL cm4f OR CONFIG_CORE STREQUAL cm7f OR CONFIG_CORE STREQUAL cm33))

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/pmsm/pmsm_float/fm_tsa_pmsm.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/pmsm/pmsm_float
)

else()

message(SEND_ERROR "middleware_motor_control_pmsm_framework_float_tsa dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
