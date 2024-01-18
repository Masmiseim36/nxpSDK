# Add set(CONFIG_USE_middleware_wireless_framework_keystorage true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_wireless_framework_filecache)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/framework/KeyStorage/fwk_key_storage.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/framework/KeyStorage
)

else()

message(SEND_ERROR "middleware_wireless_framework_keystorage dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
