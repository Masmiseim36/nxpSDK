# Add set(CONFIG_USE_middleware_usb_host_cdc_rndis true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_usb_host_stack AND CONFIG_USE_middleware_usb_host_cdc)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/host/class/usb_host_cdc_rndis.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/host/class
)

else()

message(SEND_ERROR "middleware_usb_host_cdc_rndis dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
