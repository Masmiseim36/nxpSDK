# Add set(CONFIG_USE_middleware_mbedtls3x_port_osal_frtos true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_mbedtls3x_port_osal)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/port/osal/frtos/osal_mutex.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/port/osal/frtos
)

else()

message(SEND_ERROR "middleware_mbedtls3x_port_osal_frtos dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
