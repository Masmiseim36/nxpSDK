# Add set(CONFIG_USE_middleware_freertos-kernel_mpu_wrappers_v2 true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_freertos-kernel)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/portable/Common/mpu_wrappers_v2.c
)

else()

message(SEND_ERROR "middleware_freertos-kernel_mpu_wrappers_v2 dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
