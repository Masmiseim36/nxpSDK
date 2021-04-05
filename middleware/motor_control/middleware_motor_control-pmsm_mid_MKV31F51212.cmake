if(NOT MIDDLEWARE_MOTOR_CONTROL-PMSM_MID_MKV31F51212_INCLUDED)
    
    set(MIDDLEWARE_MOTOR_CONTROL-PMSM_MID_MKV31F51212_INCLUDED true CACHE BOOL "middleware_motor_control-pmsm_mid component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/pmsm/pmsm_safe/mc_identification/hall_calib.c
        ${CMAKE_CURRENT_LIST_DIR}/pmsm/pmsm_safe/mc_identification/ke_measure.c
        ${CMAKE_CURRENT_LIST_DIR}/pmsm/pmsm_safe/mc_identification/ls_measure.c
        ${CMAKE_CURRENT_LIST_DIR}/pmsm/pmsm_safe/mc_identification/mech_measure.c
        ${CMAKE_CURRENT_LIST_DIR}/pmsm/pmsm_safe/mc_identification/mid_auxiliary.c
        ${CMAKE_CURRENT_LIST_DIR}/pmsm/pmsm_safe/mc_identification/mid_sm_states.c
        ${CMAKE_CURRENT_LIST_DIR}/pmsm/pmsm_safe/mc_identification/mid_mc_api_connector.c
        ${CMAKE_CURRENT_LIST_DIR}/pmsm/pmsm_safe/mc_identification/pp_measure.c
        ${CMAKE_CURRENT_LIST_DIR}/pmsm/pmsm_safe/mc_identification/pwrstg_characteristic.c
        ${CMAKE_CURRENT_LIST_DIR}/pmsm/pmsm_safe/mc_identification/rs_measure.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/pmsm/pmsm_safe/mc_identification
    )


endif()
