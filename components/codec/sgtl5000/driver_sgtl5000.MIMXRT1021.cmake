# Add set(CONFIG_USE_driver_sgtl5000 true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_driver_common AND CONFIG_USE_component_codec_i2c)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/fsl_sgtl5000.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/.
)

if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

  target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
    -DSDK_I2C_BASED_COMPONENT_USED=1
    -DBOARD_USE_CODEC=1
    -DCODEC_SGTL5000_ENABLE
  )

endif()

else()

message(SEND_ERROR "driver_sgtl5000.MIMXRT1021 dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
