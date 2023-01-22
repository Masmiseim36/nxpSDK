include_guard()
message("component_timer_manager component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/fsl_component_timer_manager.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/.
)


#OR Logic component
if(CONFIG_USE_component_tpm_adapter_MIMXRT595S_cm33)
     include(component_tpm_adapter_MIMXRT595S_cm33)
endif()
if(CONFIG_USE_component_ctimer_adapter_MIMXRT595S_cm33)
     include(component_ctimer_adapter_MIMXRT595S_cm33)
endif()
if(CONFIG_USE_component_ftm_adapter_MIMXRT595S_cm33)
     include(component_ftm_adapter_MIMXRT595S_cm33)
endif()
if(CONFIG_USE_component_gpt_adapter_MIMXRT595S_cm33)
     include(component_gpt_adapter_MIMXRT595S_cm33)
endif()
if(CONFIG_USE_component_lpit_adapter_MIMXRT595S_cm33)
     include(component_lpit_adapter_MIMXRT595S_cm33)
endif()
if(CONFIG_USE_component_lptmr_adapter_MIMXRT595S_cm33)
     include(component_lptmr_adapter_MIMXRT595S_cm33)
endif()
if(CONFIG_USE_component_mrt_adapter_MIMXRT595S_cm33)
     include(component_mrt_adapter_MIMXRT595S_cm33)
endif()
if(CONFIG_USE_component_pit_adapter_MIMXRT595S_cm33)
     include(component_pit_adapter_MIMXRT595S_cm33)
endif()
if(NOT (CONFIG_USE_component_tpm_adapter_MIMXRT595S_cm33 OR CONFIG_USE_component_ctimer_adapter_MIMXRT595S_cm33 OR CONFIG_USE_component_ftm_adapter_MIMXRT595S_cm33 OR CONFIG_USE_component_gpt_adapter_MIMXRT595S_cm33 OR CONFIG_USE_component_lpit_adapter_MIMXRT595S_cm33 OR CONFIG_USE_component_lptmr_adapter_MIMXRT595S_cm33 OR CONFIG_USE_component_mrt_adapter_MIMXRT595S_cm33 OR CONFIG_USE_component_pit_adapter_MIMXRT595S_cm33))
    message(WARNING "Since component_tpm_adapter_MIMXRT595S_cm33/component_ctimer_adapter_MIMXRT595S_cm33/component_ftm_adapter_MIMXRT595S_cm33/component_gpt_adapter_MIMXRT595S_cm33/component_lpit_adapter_MIMXRT595S_cm33/component_lptmr_adapter_MIMXRT595S_cm33/component_mrt_adapter_MIMXRT595S_cm33/component_pit_adapter_MIMXRT595S_cm33 is not included at first or config in config.cmake file, use component_tpm_adapter_MIMXRT595S_cm33 by default.")
    include(component_tpm_adapter_MIMXRT595S_cm33)
endif()

include(driver_common_MIMXRT595S_cm33)

include(component_lists_MIMXRT595S_cm33)

