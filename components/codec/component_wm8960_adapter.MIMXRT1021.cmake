# Add set(CONFIG_USE_component_wm8960_adapter true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_driver_wm8960 AND CONFIG_USE_driver_codec)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/port/wm8960/fsl_codec_wm8960_adapter.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/port/wm8960
  ${CMAKE_CURRENT_LIST_DIR}/port
)

else()

message(SEND_ERROR "component_wm8960_adapter.MIMXRT1021 dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
