# Add set(CONFIG_USE_middleware_edgefast_bluetooth_mcux_linker_template_evkcmimxrt1060 true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if((CONFIG_DEVICE_ID STREQUAL MIMXRT1062xxxxA OR CONFIG_DEVICE_ID STREQUAL MIMXRT1062xxxxB) AND (CONFIG_BOARD STREQUAL evkcmimxrt1060))

if(CONFIG_TOOLCHAIN STREQUAL mcux)
  add_config_file(${CMAKE_CURRENT_LIST_DIR}/data.ldt "" middleware_edgefast_bluetooth_mcux_linker_template_evkcmimxrt1060)
  add_config_file(${CMAKE_CURRENT_LIST_DIR}/end_text.ldt "" middleware_edgefast_bluetooth_mcux_linker_template_evkcmimxrt1060)
  add_config_file(${CMAKE_CURRENT_LIST_DIR}/main_data.ldt "" middleware_edgefast_bluetooth_mcux_linker_template_evkcmimxrt1060)
  add_config_file(${CMAKE_CURRENT_LIST_DIR}/main_text.ldt "" middleware_edgefast_bluetooth_mcux_linker_template_evkcmimxrt1060)
  add_config_file(${CMAKE_CURRENT_LIST_DIR}/symbols.ldt "" middleware_edgefast_bluetooth_mcux_linker_template_evkcmimxrt1060)
endif()

else()

message(SEND_ERROR "middleware_edgefast_bluetooth_mcux_linker_template_evkcmimxrt1060 dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
