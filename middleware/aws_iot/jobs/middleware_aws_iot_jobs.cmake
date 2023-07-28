# Add set(CONFIG_USE_middleware_aws_iot_jobs true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_llhttp)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/source/jobs.c
)

else()

message(SEND_ERROR "middleware_aws_iot_jobs dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
