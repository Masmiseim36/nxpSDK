# Add set(CONFIG_USE_middleware_motor_control_mc_drivers_adcimxrt1180_pwma_enc true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if((CONFIG_DEVICE_ID STREQUAL MIMXRT1187xxxxx OR CONFIG_DEVICE_ID STREQUAL MIMXRT1189xxxxx))

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/pmsm/pmsm_float/mc_drivers/mcdrv_adc_imxrt118x.c
  ${CMAKE_CURRENT_LIST_DIR}/pmsm/pmsm_float/mc_drivers/mcdrv_pwm3ph_pwma.c
  ${CMAKE_CURRENT_LIST_DIR}/pmsm/pmsm_float/mc_drivers/mcdrv_enc_eqd2.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/pmsm/pmsm_float/mc_drivers
)

else()

message(SEND_ERROR "middleware_motor_control_mc_drivers_adcimxrt1180_pwma_enc dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
