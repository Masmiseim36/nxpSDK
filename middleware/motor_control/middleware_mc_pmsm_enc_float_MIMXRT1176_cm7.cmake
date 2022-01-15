include_guard(GLOBAL)
message("middleware_mc_pmsm_enc_float component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/pmsm/pmsm_float/mc_identification/hall_calib.c
    ${CMAKE_CURRENT_LIST_DIR}/pmsm/pmsm_float/mc_identification/ke_measure.c
    ${CMAKE_CURRENT_LIST_DIR}/pmsm/pmsm_float/mc_identification/mech_measure.c
    ${CMAKE_CURRENT_LIST_DIR}/pmsm/pmsm_float/mc_identification/mid_auxiliary.c
    ${CMAKE_CURRENT_LIST_DIR}/pmsm/pmsm_float/mc_identification/mid_mc_api_connector.c
    ${CMAKE_CURRENT_LIST_DIR}/pmsm/pmsm_float/mc_identification/mid_sm_states.c
    ${CMAKE_CURRENT_LIST_DIR}/pmsm/pmsm_float/mc_identification/pp_measure.c
    ${CMAKE_CURRENT_LIST_DIR}/pmsm/pmsm_float/mc_algorithms/pmsm_control.c
    ${CMAKE_CURRENT_LIST_DIR}/pmsm/pmsm_float/state_machine/state_machine.c
    ${CMAKE_CURRENT_LIST_DIR}/pmsm/pmsm_float/mc_state_machine/m1_sm_snsless_enc.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/pmsm/pmsm_float/mc_identification
    ${CMAKE_CURRENT_LIST_DIR}/pmsm/pmsm_float/mc_algorithms
    ${CMAKE_CURRENT_LIST_DIR}/pmsm/pmsm_float/state_machine
    ${CMAKE_CURRENT_LIST_DIR}/pmsm/pmsm_float/mc_state_machine
    ${CMAKE_CURRENT_LIST_DIR}/pmsm/pmsm_float/mc_drivers
    ${CMAKE_CURRENT_LIST_DIR}/pmsm/pmsm_float
)


