# Add set(CONFIG_USE_middleware_edgefast_bluetooth_liblc3codec true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/liblc3codec/src/attdet.c
  ${CMAKE_CURRENT_LIST_DIR}/liblc3codec/src/bits.c
  ${CMAKE_CURRENT_LIST_DIR}/liblc3codec/src/bwdet.c
  ${CMAKE_CURRENT_LIST_DIR}/liblc3codec/src/energy.c
  ${CMAKE_CURRENT_LIST_DIR}/liblc3codec/src/lc3.c
  ${CMAKE_CURRENT_LIST_DIR}/liblc3codec/src/ltpf.c
  ${CMAKE_CURRENT_LIST_DIR}/liblc3codec/src/mdct.c
  ${CMAKE_CURRENT_LIST_DIR}/liblc3codec/src/plc.c
  ${CMAKE_CURRENT_LIST_DIR}/liblc3codec/src/sns.c
  ${CMAKE_CURRENT_LIST_DIR}/liblc3codec/src/spec.c
  ${CMAKE_CURRENT_LIST_DIR}/liblc3codec/src/tables.c
  ${CMAKE_CURRENT_LIST_DIR}/liblc3codec/src/tns.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/liblc3codec/include
  ${CMAKE_CURRENT_LIST_DIR}/liblc3codec
  ${CMAKE_CURRENT_LIST_DIR}/.
)

if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

  if(CONFIG_TOOLCHAIN STREQUAL iar)
    target_compile_options(${MCUX_SDK_PROJECT_NAME} PUBLIC
      --vla
    )
  endif()

endif()

