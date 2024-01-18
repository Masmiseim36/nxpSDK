# Add set(CONFIG_USE_middleware_wireless_framework_OtaSupport true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_wireless_framework_platform_ota_rt1060 AND CONFIG_USE_middleware_wireless_framework_platform_extflash_rt1060)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/framework/OtaSupport/Source/OtaSupport.c
  ${CMAKE_CURRENT_LIST_DIR}/framework/OtaSupport/Source/OtaInternalFlash.c
  ${CMAKE_CURRENT_LIST_DIR}/framework/OtaSupport/Source/OtaExternalFlash.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/framework/OtaSupport/Interface
  ${CMAKE_CURRENT_LIST_DIR}/framework/OtaSupport/Source
)

else()

message(SEND_ERROR "middleware_wireless_framework_OtaSupport dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
