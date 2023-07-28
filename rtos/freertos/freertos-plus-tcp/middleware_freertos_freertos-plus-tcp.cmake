# Add set(CONFIG_USE_middleware_freertos_freertos-plus-tcp true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_freertos-kernel)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/source/FreeRTOS_ARP.c
  ${CMAKE_CURRENT_LIST_DIR}/source/FreeRTOS_DHCP.c
  ${CMAKE_CURRENT_LIST_DIR}/source/FreeRTOS_DNS.c
  ${CMAKE_CURRENT_LIST_DIR}/source/FreeRTOS_DNS_Cache.c
  ${CMAKE_CURRENT_LIST_DIR}/source/FreeRTOS_DNS_Callback.c
  ${CMAKE_CURRENT_LIST_DIR}/source/FreeRTOS_DNS_Networking.c
  ${CMAKE_CURRENT_LIST_DIR}/source/FreeRTOS_DNS_Parser.c
  ${CMAKE_CURRENT_LIST_DIR}/source/FreeRTOS_ICMP.c
  ${CMAKE_CURRENT_LIST_DIR}/source/FreeRTOS_IP.c
  ${CMAKE_CURRENT_LIST_DIR}/source/FreeRTOS_IP_Timers.c
  ${CMAKE_CURRENT_LIST_DIR}/source/FreeRTOS_IP_Utils.c
  ${CMAKE_CURRENT_LIST_DIR}/source/FreeRTOS_Sockets.c
  ${CMAKE_CURRENT_LIST_DIR}/source/FreeRTOS_Stream_Buffer.c
  ${CMAKE_CURRENT_LIST_DIR}/source/FreeRTOS_TCP_IP.c
  ${CMAKE_CURRENT_LIST_DIR}/source/FreeRTOS_TCP_Reception.c
  ${CMAKE_CURRENT_LIST_DIR}/source/FreeRTOS_TCP_State_Handling.c
  ${CMAKE_CURRENT_LIST_DIR}/source/FreeRTOS_TCP_Transmission.c
  ${CMAKE_CURRENT_LIST_DIR}/source/FreeRTOS_TCP_Utils.c
  ${CMAKE_CURRENT_LIST_DIR}/source/FreeRTOS_TCP_WIN.c
  ${CMAKE_CURRENT_LIST_DIR}/source/FreeRTOS_Tiny_TCP.c
  ${CMAKE_CURRENT_LIST_DIR}/source/FreeRTOS_UDP_IP.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/source/include
)

else()

message(SEND_ERROR "middleware_freertos_freertos-plus-tcp dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
