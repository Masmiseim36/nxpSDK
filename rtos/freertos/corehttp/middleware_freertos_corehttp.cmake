# Add set(CONFIG_USE_middleware_freertos_corehttp true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_llhttp)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/source/core_http_client.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/source/include
  ${CMAKE_CURRENT_LIST_DIR}/source/interface
)

else()

message(SEND_ERROR "middleware_freertos_corehttp dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
