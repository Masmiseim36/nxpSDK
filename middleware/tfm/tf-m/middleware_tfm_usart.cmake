# Add set(CONFIG_USE_middleware_tfm_usart true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_driver_flexcomm_usart)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/common/CMSIS_Driver/Driver_USART.c
)

else()

message(SEND_ERROR "middleware_tfm_usart dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
