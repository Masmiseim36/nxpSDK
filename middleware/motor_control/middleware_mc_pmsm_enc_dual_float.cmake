# Add set(CONFIG_USE_middleware_mc_pmsm_enc_dual_float true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/pmsm/pmsm_float/mc_algorithms/pmsm_control.c
          ${CMAKE_CURRENT_LIST_DIR}/pmsm/pmsm_float/state_machine/state_machine.c
          ${CMAKE_CURRENT_LIST_DIR}/pmsm/pmsm_float/mc_state_machine/m1_sm_snsless_enc.c
          ${CMAKE_CURRENT_LIST_DIR}/pmsm/pmsm_float/mc_state_machine/m2_sm_snsless_enc.c
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
                        -DDUAL_MOTOR
              )
  
  
  endif()

