# Add set(CONFIG_USE_middleware_motor_control_mc_drivers_adcimxrt11xx_pwma_bldc true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/bldc/mc_drivers/mcdrv_pwm3ph_pwma.c
          ${CMAKE_CURRENT_LIST_DIR}/bldc/mc_drivers/mcdrv_adcetc_imxrt11xx.c
          ${CMAKE_CURRENT_LIST_DIR}/bldc/mc_drivers/mcdrv_cmt_pwma.c
        )

  
      target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/bldc/mc_drivers
        )

  
