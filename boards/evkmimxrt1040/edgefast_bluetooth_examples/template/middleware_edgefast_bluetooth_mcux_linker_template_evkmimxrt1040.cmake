# Add set(CONFIG_USE_middleware_edgefast_bluetooth_mcux_linker_template_evkmimxrt1040 true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if((CONFIG_DEVICE_ID STREQUAL MIMXRT1042xxxxB))

if(CONFIG_TOOLCHAIN STREQUAL mcux)
  add_config_file(${CMAKE_CURRENT_LIST_DIR}/data.ldt "" middleware_edgefast_bluetooth_mcux_linker_template_evkmimxrt1040)
  add_config_file(${CMAKE_CURRENT_LIST_DIR}/end_text.ldt "" middleware_edgefast_bluetooth_mcux_linker_template_evkmimxrt1040)
  add_config_file(${CMAKE_CURRENT_LIST_DIR}/main_data.ldt "" middleware_edgefast_bluetooth_mcux_linker_template_evkmimxrt1040)
  add_config_file(${CMAKE_CURRENT_LIST_DIR}/main_text.ldt "" middleware_edgefast_bluetooth_mcux_linker_template_evkmimxrt1040)
  add_config_file(${CMAKE_CURRENT_LIST_DIR}/symbols.ldt "" middleware_edgefast_bluetooth_mcux_linker_template_evkmimxrt1040)
endif()

else()

message(SEND_ERROR "middleware_edgefast_bluetooth_mcux_linker_template_evkmimxrt1040 dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
