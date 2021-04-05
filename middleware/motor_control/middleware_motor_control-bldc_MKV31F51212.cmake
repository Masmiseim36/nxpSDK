if(NOT MIDDLEWARE_MOTOR_CONTROL-BLDC_MKV31F51212_INCLUDED)
    
    set(MIDDLEWARE_MOTOR_CONTROL-BLDC_MKV31F51212_INCLUDED true CACHE BOOL "middleware_motor_control-bldc component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/bldc/mc_algorithms/bldc_control.c
        ${CMAKE_CURRENT_LIST_DIR}/bldc/mc_state_machine/m1_sm_ref_sol.c
        ${CMAKE_CURRENT_LIST_DIR}/bldc/state_machine/state_machine.c
        ${CMAKE_CURRENT_LIST_DIR}/bldc/fm_tsa_bldc.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/bldc/mc_algorithms
        ${CMAKE_CURRENT_LIST_DIR}/bldc/mc_state_machine
        ${CMAKE_CURRENT_LIST_DIR}/bldc/state_machine
        ${CMAKE_CURRENT_LIST_DIR}/bldc
    )


endif()
