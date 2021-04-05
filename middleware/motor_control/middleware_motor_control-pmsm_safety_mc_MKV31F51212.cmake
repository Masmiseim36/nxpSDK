if(NOT MIDDLEWARE_MOTOR_CONTROL-PMSM_SAFETY_MC_MKV31F51212_INCLUDED)
    
    set(MIDDLEWARE_MOTOR_CONTROL-PMSM_SAFETY_MC_MKV31F51212_INCLUDED true CACHE BOOL "middleware_motor_control-pmsm_safety_mc component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/pmsm/pmsm_safe/safety_routines/safety_routines.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/pmsm/pmsm_safe/safety_routines
    )


endif()
