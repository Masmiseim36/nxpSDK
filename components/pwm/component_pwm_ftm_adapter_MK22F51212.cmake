if(NOT COMPONENT_PWM_FTM_ADAPTER_MK22F51212_INCLUDED)
    
    set(COMPONENT_PWM_FTM_ADAPTER_MK22F51212_INCLUDED true CACHE BOOL "component_pwm_ftm_adapter component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_adapter_pwm_ftm.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_common_MK22F51212)

    include(driver_ftm_MK22F51212)

endif()
