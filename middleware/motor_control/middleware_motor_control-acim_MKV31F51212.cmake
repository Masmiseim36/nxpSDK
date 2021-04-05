if(NOT MIDDLEWARE_MOTOR_CONTROL-ACIM_MKV31F51212_INCLUDED)
    
    set(MIDDLEWARE_MOTOR_CONTROL-ACIM_MKV31F51212_INCLUDED true CACHE BOOL "middleware_motor_control-acim component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/acim/mc_algorithms/acim_control.c
        ${CMAKE_CURRENT_LIST_DIR}/acim/mc_state_machine/m1_sm_ref_sol.c
        ${CMAKE_CURRENT_LIST_DIR}/acim/state_machine/state_machine.c
        ${CMAKE_CURRENT_LIST_DIR}/acim/mc_identification/src_common/mid_sm_ctrl.c
        ${CMAKE_CURRENT_LIST_DIR}/acim/mc_identification/src_common/mid_sm_states.c
        ${CMAKE_CURRENT_LIST_DIR}/acim/mc_identification/src_routines/blocked_test.c
        ${CMAKE_CURRENT_LIST_DIR}/acim/mc_identification/src_routines/calc_acim.c
        ${CMAKE_CURRENT_LIST_DIR}/acim/mc_identification/src_routines/calc_power.c
        ${CMAKE_CURRENT_LIST_DIR}/acim/mc_identification/src_routines/mech_measure.c
        ${CMAKE_CURRENT_LIST_DIR}/acim/mc_identification/src_routines/noload_test.c
        ${CMAKE_CURRENT_LIST_DIR}/acim/mc_identification/src_routines/pwrstg_characteristic.c
        ${CMAKE_CURRENT_LIST_DIR}/acim/mc_identification/src_routines/rs_measure.c
        ${CMAKE_CURRENT_LIST_DIR}/acim/fm_tsa_acim.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/acim/mc_algorithms
        ${CMAKE_CURRENT_LIST_DIR}/acim/mc_state_machine
        ${CMAKE_CURRENT_LIST_DIR}/acim/state_machine
        ${CMAKE_CURRENT_LIST_DIR}/acim/mc_identification/src_common
        ${CMAKE_CURRENT_LIST_DIR}/acim/mc_identification/src_routines
        ${CMAKE_CURRENT_LIST_DIR}/acim
    )


endif()
