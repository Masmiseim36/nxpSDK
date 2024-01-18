# Add set(CONFIG_USE_middleware_wireless_framework_platform_rt_ot_coex true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/framework/platform/imx_rt/fwk_platform_ot.c
  ${CMAKE_CURRENT_LIST_DIR}/framework/platform/imx_rt/fwk_platform_hdlc.c
  ${CMAKE_CURRENT_LIST_DIR}/framework/platform/imx_rt/k32w0/fwk_platform_coex.c
  ${CMAKE_CURRENT_LIST_DIR}/framework/platform/imx_rt/iw612/fwk_platform_coex.c
  ${CMAKE_CURRENT_LIST_DIR}/framework/platform/imx_rt/configs/fwk_lfs_config.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/framework/platform/imx_rt/configs
)

