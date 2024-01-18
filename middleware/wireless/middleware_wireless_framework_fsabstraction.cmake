# Add set(CONFIG_USE_middleware_wireless_framework_fsabstraction true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_wireless_framework_fsabstraction_littlefs)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/framework/FSAbstraction/fwk_fs_abstraction.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/framework/FSAbstraction
)

else()

message(SEND_ERROR "middleware_wireless_framework_fsabstraction dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
