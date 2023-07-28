# Add set(CONFIG_USE_middleware_lwip_contrib_udpecho true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_lwip)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/contrib/apps/udpecho/udpecho.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/contrib/apps/udpecho
)

else()

message(SEND_ERROR "middleware_lwip_contrib_udpecho dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
