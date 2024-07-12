# Add set(CONFIG_USE_middleware_wireless_framework_settings true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/framework/Settings/Source/settings.c
  ${CMAKE_CURRENT_LIST_DIR}/framework/Settings/Source/settings_init.c
  ${CMAKE_CURRENT_LIST_DIR}/framework/Settings/Source/settings_runtime.c
  ${CMAKE_CURRENT_LIST_DIR}/framework/Settings/Source/settings_store.c
  ${CMAKE_CURRENT_LIST_DIR}/framework/Settings/Source/settings_nvs.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/framework/Settings/Interface
  ${CMAKE_CURRENT_LIST_DIR}/framework/Settings/Source
)

