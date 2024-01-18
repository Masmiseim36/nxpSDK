# Add set(CONFIG_USE_middleware_mc_pmsm_enc_float true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if((CONFIG_CORE STREQUAL cm4f OR CONFIG_CORE STREQUAL cm7f OR CONFIG_CORE STREQUAL cm33) AND CONFIG_USE_middleware_motor_control_pmsm_float_template AND (CONFIG_USE_middleware_rtcesl_cm33f OR CONFIG_USE_middleware_rtcesl_cm7f OR CONFIG_USE_middleware_rtcesl_cm4f))

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/pmsm/pmsm_float/mc_algorithms/pmsm_control.c
  ${CMAKE_CURRENT_LIST_DIR}/pmsm/pmsm_float/state_machine/state_machine.c
  ${CMAKE_CURRENT_LIST_DIR}/pmsm/pmsm_float/mc_state_machine/m1_sm_snsless_enc.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/pmsm/pmsm_float/mc_algorithms
  ${CMAKE_CURRENT_LIST_DIR}/pmsm/pmsm_float/state_machine
  ${CMAKE_CURRENT_LIST_DIR}/pmsm/pmsm_float/mc_state_machine
  ${CMAKE_CURRENT_LIST_DIR}/pmsm/pmsm_float
  ${CMAKE_CURRENT_LIST_DIR}/freemaster
  ${CMAKE_CURRENT_LIST_DIR}/docs
)

if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

  target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
    -DPMSM_SNSLESS
    -DPMSM_SNSLESS_ENC
  )

endif()

else()

message(SEND_ERROR "middleware_mc_pmsm_enc_float dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
