# Add set(CONFIG_USE_middleware_wireless_framework_sec_lib_mbedtls true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/framework/SecLib/SecLib_mbedTLS.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/framework/SecLib
  ${CMAKE_CURRENT_LIST_DIR}/framework/platform/MIMXRT1062/configs
)

