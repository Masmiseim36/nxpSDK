# Add set(CONFIG_USE_middleware_usb_device_controller_driver true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_component_osa AND (CONFIG_USE_middleware_usb_device_khci OR CONFIG_USE_middleware_usb_device_ehci OR CONFIG_USE_middleware_usb_device_ip3511fs OR CONFIG_USE_middleware_usb_device_ip3511hs))

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/device/usb_device_dci.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/device
)

else()

message(SEND_ERROR "middleware_usb_device_controller_driver dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
