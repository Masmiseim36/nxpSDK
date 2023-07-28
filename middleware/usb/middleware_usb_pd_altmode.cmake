# Add set(CONFIG_USE_middleware_usb_pd_altmode true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_usb_pd)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/pd/alt_mode/usb_pd_alt_mode.c
  ${CMAKE_CURRENT_LIST_DIR}/pd/alt_mode/usb_pd_alt_mode_dp.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/pd/alt_mode
)

else()

message(SEND_ERROR "middleware_usb_pd_altmode dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
