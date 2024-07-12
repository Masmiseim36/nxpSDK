# Add set(CONFIG_USE_middleware_wireless_framework_platform_MIMXRT595S true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_wireless_framework_fwk_debug AND ((CONFIG_USE_component_timer_manager AND CONFIG_USE_component_ostimer_time_stamp_adapter AND (CONFIG_USE_component_mrt_adapter OR CONFIG_USE_component_ctimer_adapter) AND (CONFIG_USE_component_mflash_rdrw610 OR CONFIG_USE_component_mflash_frdmrw612) AND CONFIG_USE_middleware_mbedtls_port_els_pkc AND (CONFIG_DEVICE_ID STREQUAL RW612 OR CONFIG_DEVICE_ID STREQUAL RW610)) OR ((CONFIG_DEVICE_ID STREQUAL K32W1480xxxA OR CONFIG_DEVICE_ID STREQUAL KW45B41Z52xxxA OR CONFIG_DEVICE_ID STREQUAL KW45B41Z53xxxA OR CONFIG_DEVICE_ID STREQUAL KW45B41Z82xxxA OR CONFIG_DEVICE_ID STREQUAL KW45B41Z83xxxA OR CONFIG_DEVICE_ID STREQUAL MCXW716CxxxA OR CONFIG_DEVICE_ID STREQUAL MCXW716AxxxA OR CONFIG_DEVICE_ID STREQUAL MIMXRT595S))))

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/MIMXRT595S/fwk_platform.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/include
  ${CMAKE_CURRENT_LIST_DIR}/MIMXRT595S
  ${CMAKE_CURRENT_LIST_DIR}/MIMXRT595S/configs
)

else()

message(SEND_ERROR "middleware_wireless_framework_platform_MIMXRT595S dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
