# Add set(CONFIG_USE_component_tfa9xxx_adapter true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_driver_tfa9xxx AND CONFIG_USE_driver_codec)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/port/tfa9xxx/fsl_codec_tfa9xxx_adapter.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/port/tfa9xxx
  ${CMAKE_CURRENT_LIST_DIR}/port
)

else()

message(SEND_ERROR "component_tfa9xxx_adapter.MIMXRT685S dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
