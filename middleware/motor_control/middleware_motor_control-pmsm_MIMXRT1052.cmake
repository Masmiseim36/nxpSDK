if(NOT MIDDLEWARE_MOTOR_CONTROL-PMSM_MIMXRT1052_INCLUDED)
    
    set(MIDDLEWARE_MOTOR_CONTROL-PMSM_MIMXRT1052_INCLUDED true CACHE BOOL "middleware_motor_control-pmsm component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/pmsm/pmsm_float/mc_identification/hall_calib.c
        ${CMAKE_CURRENT_LIST_DIR}/pmsm/pmsm_float/mc_identification/ke_measure.c
        ${CMAKE_CURRENT_LIST_DIR}/pmsm/pmsm_float/mc_identification/ls_measure.c
        ${CMAKE_CURRENT_LIST_DIR}/pmsm/pmsm_float/mc_identification/mech_measure.c
        ${CMAKE_CURRENT_LIST_DIR}/pmsm/pmsm_float/mc_identification/mid_auxiliary.c
        ${CMAKE_CURRENT_LIST_DIR}/pmsm/pmsm_float/mc_identification/mid_sm_ctrl.c
        ${CMAKE_CURRENT_LIST_DIR}/pmsm/pmsm_float/mc_identification/mid_sm_states.c
        ${CMAKE_CURRENT_LIST_DIR}/pmsm/pmsm_float/mc_identification/pp_measure.c
        ${CMAKE_CURRENT_LIST_DIR}/pmsm/pmsm_float/mc_identification/pwrstg_characteristic.c
        ${CMAKE_CURRENT_LIST_DIR}/pmsm/pmsm_float/mc_identification/rs_measure.c
        ${CMAKE_CURRENT_LIST_DIR}/pmsm/pmsm_float/mc_algorithms/pmsm_control.c
        ${CMAKE_CURRENT_LIST_DIR}/pmsm/pmsm_float/state_machine/state_machine.c
        ${CMAKE_CURRENT_LIST_DIR}/pmsm/pmsm_float/fm_tsa_pmsm.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/pmsm/pmsm_float/mc_identification
        ${CMAKE_CURRENT_LIST_DIR}/pmsm/pmsm_float/mc_algorithms
        ${CMAKE_CURRENT_LIST_DIR}/pmsm/pmsm_float/state_machine
        ${CMAKE_CURRENT_LIST_DIR}/pmsm/pmsm_float
    )


endif()
