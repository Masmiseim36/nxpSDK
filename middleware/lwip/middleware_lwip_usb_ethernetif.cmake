# Add set(CONFIG_USE_middleware_lwip_usb_ethernetif true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_lwip AND CONFIG_USE_middleware_usb_host_cdc AND CONFIG_USE_middleware_usb_host_cdc_rndis AND (CONFIG_USE_middleware_usb_host_khci OR CONFIG_USE_middleware_usb_host_ehci OR CONFIG_USE_middleware_usb_host_ohci))

if(CONFIG_USE_middleware_baremetal)
  target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
      ${CMAKE_CURRENT_LIST_DIR}/port/usb_ethernetif_bm.c
  )
endif()

if(CONFIG_USE_middleware_freertos-kernel)
  target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
      ${CMAKE_CURRENT_LIST_DIR}/port/usb_ethernetif_freertos.c
  )
endif()

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/port
)

else()

message(SEND_ERROR "middleware_lwip_usb_ethernetif dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
