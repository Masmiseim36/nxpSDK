include_guard(GLOBAL)
message("middleware_motor_control_mc_drivers_adc16sngl_ftm_frac component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/pmsm/pmsm_frac/mc_drivers/mcdrv_adc_single_adc16.c
    ${CMAKE_CURRENT_LIST_DIR}/pmsm/pmsm_frac/mc_drivers/mcdrv_pwm3ph_ftm.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/pmsm/pmsm_frac/mc_drivers
)


