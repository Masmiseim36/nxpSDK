if(NOT COMPONENT_TIMER_MANAGER_K32L2B31A_INCLUDED)
    
    set(COMPONENT_TIMER_MANAGER_K32L2B31A_INCLUDED true CACHE BOOL "component_timer_manager component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_component_timer_manager.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    #OR Logic component
    if(CONFIG_USE_component_tpm_adapter_K32L2B31A)
         include(component_tpm_adapter_K32L2B31A)
    endif()
    if(CONFIG_USE_component_ctimer_adapter_K32L2B31A)
         include(component_ctimer_adapter_K32L2B31A)
    endif()
    if(CONFIG_USE_component_ftm_adapter_K32L2B31A)
         include(component_ftm_adapter_K32L2B31A)
    endif()
    if(CONFIG_USE_component_gpt_adapter_K32L2B31A)
         include(component_gpt_adapter_K32L2B31A)
    endif()
    if(CONFIG_USE_component_lpit_adapter_K32L2B31A)
         include(component_lpit_adapter_K32L2B31A)
    endif()
    if(CONFIG_USE_component_lptmr_adapter_K32L2B31A)
         include(component_lptmr_adapter_K32L2B31A)
    endif()
    if(CONFIG_USE_component_mrt_adapter_K32L2B31A)
         include(component_mrt_adapter_K32L2B31A)
    endif()
    if(CONFIG_USE_component_pit_adapter_K32L2B31A)
         include(component_pit_adapter_K32L2B31A)
    endif()
    if(NOT (CONFIG_USE_component_tpm_adapter_K32L2B31A OR CONFIG_USE_component_ctimer_adapter_K32L2B31A OR CONFIG_USE_component_ftm_adapter_K32L2B31A OR CONFIG_USE_component_gpt_adapter_K32L2B31A OR CONFIG_USE_component_lpit_adapter_K32L2B31A OR CONFIG_USE_component_lptmr_adapter_K32L2B31A OR CONFIG_USE_component_mrt_adapter_K32L2B31A OR CONFIG_USE_component_pit_adapter_K32L2B31A))
        message(WARNING "Since component_tpm_adapter_K32L2B31A/component_ctimer_adapter_K32L2B31A/component_ftm_adapter_K32L2B31A/component_gpt_adapter_K32L2B31A/component_lpit_adapter_K32L2B31A/component_lptmr_adapter_K32L2B31A/component_mrt_adapter_K32L2B31A/component_pit_adapter_K32L2B31A is not included at first or config in config.cmake file, use component_tpm_adapter_K32L2B31A by default.")
        include(component_tpm_adapter_K32L2B31A)
    endif()

    include(driver_common_K32L2B31A)

    include(component_lists_K32L2B31A)

endif()
