# Add set(CONFIG_USE_middleware_usb_host_video true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_usb_host_stack)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/host/class/usb_host_video.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/host/class
)

else()

message(SEND_ERROR "middleware_usb_host_video dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
