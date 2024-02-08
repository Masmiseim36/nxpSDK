# Add set(CONFIG_USE_middleware_usb_device_ehci true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if((CONFIG_DEVICE_ID STREQUAL MIMXRT1181xxxxx OR CONFIG_DEVICE_ID STREQUAL MIMXRT1182xxxxx OR CONFIG_DEVICE_ID STREQUAL MIMXRT1187xxxxx OR CONFIG_DEVICE_ID STREQUAL MIMXRT1189xxxxx) AND CONFIG_USE_middleware_usb_device_ehci_config_header AND CONFIG_USE_middleware_usb_device_common_header AND ((CONFIG_USE_middleware_usb_phy AND (CONFIG_DEVICE_ID STREQUAL MIMXRT1187xxxxx OR CONFIG_DEVICE_ID STREQUAL MIMXRT1189xxxxx)) OR (NOT (CONFIG_NOT STREQUAL MIMXRT1187xxxxx OR CONFIG_NOT STREQUAL MIMXRT1189xxxxx))))

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/device/usb_device_ehci.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/device
  ${CMAKE_CURRENT_LIST_DIR}/include
)

else()

message(SEND_ERROR "middleware_usb_device_ehci dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
