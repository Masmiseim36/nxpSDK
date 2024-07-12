# Add set(CONFIG_USE_middleware_wireless_framework_matter_config_MIMXRT595S true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/boards/MIMXRT595S/board_lp.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/boards/MIMXRT595S
  ${CMAKE_CURRENT_LIST_DIR}/Common/devices/MIMXRT595S/gcc
)

