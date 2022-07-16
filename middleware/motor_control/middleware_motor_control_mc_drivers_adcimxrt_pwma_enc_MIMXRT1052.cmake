include_guard()
message("middleware_motor_control_mc_drivers_adcimxrt_pwma_enc component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/pmsm/pmsm_float/mc_drivers/mcdrv_adcetc_imxrt.c
    ${CMAKE_CURRENT_LIST_DIR}/pmsm/pmsm_float/mc_drivers/mcdrv_pwm3ph_pwma.c
    ${CMAKE_CURRENT_LIST_DIR}/pmsm/pmsm_float/mc_drivers/mcdrv_enc_qd.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/pmsm/pmsm_float/mc_drivers
)


