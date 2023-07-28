# Add set(CONFIG_USE_middleware_wireless_framework_platfrom_rt_ot_coex true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/framework/platform/rt/fwk_platform_ot.c
  ${CMAKE_CURRENT_LIST_DIR}/framework/platform/rt/iw612/fwk_platform_coex.c
)

