# Add set(CONFIG_USE_middleware_edgefast_bluetooth_asrc_sw true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/asrc_sw/srCvtFrm.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/asrc_sw/include
  ${CMAKE_CURRENT_LIST_DIR}/asrc_sw
  ${CMAKE_CURRENT_LIST_DIR}/.
)

