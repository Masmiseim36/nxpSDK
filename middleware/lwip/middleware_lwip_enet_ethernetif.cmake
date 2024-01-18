# Add set(CONFIG_USE_middleware_lwip_enet_ethernetif true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_lwip AND CONFIG_USE_middleware_lwip_template AND CONFIG_USE_driver_phy-common AND (CONFIG_USE_component_rt_gpio_adapter OR CONFIG_USE_component_lpc_gpio_adapter OR CONFIG_USE_component_igpio_adapter OR CONFIG_USE_component_rgpio_adapter OR CONFIG_USE_component_gpio_adapter) AND (CONFIG_USE_driver_lpc_enet OR CONFIG_USE_driver_enet_qos))

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/port/enet_ethernetif.c
  ${CMAKE_CURRENT_LIST_DIR}/port/ethernetif.c
  ${CMAKE_CURRENT_LIST_DIR}/port/ethernetif_mmac.c
)

if(CONFIG_USE_driver_lpc_enet)
  target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
      ${CMAKE_CURRENT_LIST_DIR}/port/enet_ethernetif_lpc.c
  )
endif()

if(CONFIG_USE_driver_enet_qos)
  target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
      ${CMAKE_CURRENT_LIST_DIR}/port/enet_ethernetif_qos.c
  )
endif()

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/port
)

if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

  target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
    -DLWIP_DISABLE_PBUF_POOL_SIZE_SANITY_CHECKS=1
  )

endif()

else()

message(SEND_ERROR "middleware_lwip_enet_ethernetif dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
