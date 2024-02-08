# Add set(CONFIG_USE_middleware_lwip_eoe_ethernetif true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_lwip AND CONFIG_USE_middleware_lwip_template AND CONFIG_USE_middleware_freertos-kernel)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/port/eoe_ethernetif.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/port
)

if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

  target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
    -DCHECKSUM_GEN_IP=1
    -DCHECKSUM_GEN_UDP=1
    -DCHECKSUM_GEN_TCP=1
    -DCHECKSUM_GEN_ICMP=1
    -DCHECKSUM_CHECK_ICMP=1
  )

endif()

else()

message(SEND_ERROR "middleware_lwip_eoe_ethernetif dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
