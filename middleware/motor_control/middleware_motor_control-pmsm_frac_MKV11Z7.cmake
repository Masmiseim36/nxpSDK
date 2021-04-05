if(NOT MIDDLEWARE_MOTOR_CONTROL-PMSM_FRAC_MKV11Z7_INCLUDED)
    
    set(MIDDLEWARE_MOTOR_CONTROL-PMSM_FRAC_MKV11Z7_INCLUDED true CACHE BOOL "middleware_motor_control-pmsm_frac component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/pmsm/pmsm_frac/mc_identification/ke_measure.c
        ${CMAKE_CURRENT_LIST_DIR}/pmsm/pmsm_frac/mc_identification/ls_measure.c
        ${CMAKE_CURRENT_LIST_DIR}/pmsm/pmsm_frac/mc_identification/mid_auxiliary.c
        ${CMAKE_CURRENT_LIST_DIR}/pmsm/pmsm_frac/mc_identification/mid_sm_ctrl.c
        ${CMAKE_CURRENT_LIST_DIR}/pmsm/pmsm_frac/mc_identification/mid_sm_states.c
        ${CMAKE_CURRENT_LIST_DIR}/pmsm/pmsm_frac/mc_identification/pp_measure.c
        ${CMAKE_CURRENT_LIST_DIR}/pmsm/pmsm_frac/mc_identification/pwrstg_characteristic.c
        ${CMAKE_CURRENT_LIST_DIR}/pmsm/pmsm_frac/mc_identification/rs_measure.c
        ${CMAKE_CURRENT_LIST_DIR}/pmsm/pmsm_frac/mc_algorithms/pmsm_control.c
        ${CMAKE_CURRENT_LIST_DIR}/pmsm/pmsm_frac/state_machine/state_machine.c
        ${CMAKE_CURRENT_LIST_DIR}/pmsm/pmsm_frac/fm_tsa_pmsm.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/pmsm/pmsm_frac/mc_identification
        ${CMAKE_CURRENT_LIST_DIR}/pmsm/pmsm_frac/mc_algorithms
        ${CMAKE_CURRENT_LIST_DIR}/pmsm/pmsm_frac/state_machine
        ${CMAKE_CURRENT_LIST_DIR}/pmsm/pmsm_frac
    )


endif()
