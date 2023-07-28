# Add set(CONFIG_USE_middleware_usb_host_stack true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_component_osa AND (CONFIG_USE_middleware_usb_host_khci OR CONFIG_USE_middleware_usb_host_ehci OR CONFIG_USE_middleware_usb_host_ohci OR CONFIG_USE_middleware_usb_host_ip3516hs))

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/host/usb_host_hci.c
  ${CMAKE_CURRENT_LIST_DIR}/host/usb_host_devices.c
  ${CMAKE_CURRENT_LIST_DIR}/host/usb_host_framework.c
  ${CMAKE_CURRENT_LIST_DIR}/host/class/usb_host_hub.c
  ${CMAKE_CURRENT_LIST_DIR}/host/class/usb_host_hub_app.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/host
  ${CMAKE_CURRENT_LIST_DIR}/host/class
  ${CMAKE_CURRENT_LIST_DIR}/include
)

else()

message(SEND_ERROR "middleware_usb_host_stack dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
