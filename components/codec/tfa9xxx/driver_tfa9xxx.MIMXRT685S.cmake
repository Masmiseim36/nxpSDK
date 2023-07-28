# Add set(CONFIG_USE_driver_tfa9xxx true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_driver_common AND CONFIG_USE_component_codec_i2c)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/fsl_tfa9xxx.c
  ${CMAKE_CURRENT_LIST_DIR}/fsl_tfa9xxx_IMX.c
  ${CMAKE_CURRENT_LIST_DIR}/vas_tfa_drv/tfa2_container.c
  ${CMAKE_CURRENT_LIST_DIR}/vas_tfa_drv/tfa2_dev.c
  ${CMAKE_CURRENT_LIST_DIR}/vas_tfa_drv/tfa2_haptic.c
  ${CMAKE_CURRENT_LIST_DIR}/vas_tfa_drv/tfa2_init.c
  ${CMAKE_CURRENT_LIST_DIR}/vas_tfa_drv/tfa_container_crc32.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/.
  ${CMAKE_CURRENT_LIST_DIR}/vas_tfa_drv
)

if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

  target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
    -DSDK_I2C_BASED_COMPONENT_USED=1
    -DBOARD_USE_CODEC=1
    -DCODEC_TFA9XXX_ENABLE
  )

endif()

else()

message(SEND_ERROR "driver_tfa9xxx.MIMXRT685S dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
