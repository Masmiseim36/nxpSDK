# Add set(CONFIG_USE_middleware_usb_hsdcd true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_usb_hsdcd_config_header AND (CONFIG_DEVICE_ID STREQUAL MIMXRT1181xxxxx OR CONFIG_DEVICE_ID STREQUAL MIMXRT1182xxxxx OR CONFIG_DEVICE_ID STREQUAL MIMXRT1187xxxxx OR CONFIG_DEVICE_ID STREQUAL MIMXRT1189xxxxx))

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/dcd/usb_hsdcd.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/dcd
)

else()

message(SEND_ERROR "middleware_usb_hsdcd dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
