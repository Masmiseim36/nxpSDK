# Add set(CONFIG_USE_middleware_edgefast_wifi_nxp true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_wifi AND CONFIG_USE_middleware_freertos-kernel)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/source/wpl_nxp.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/include
)

else()

message(SEND_ERROR "middleware_edgefast_wifi_nxp dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
