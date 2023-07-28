# Add set(CONFIG_USE_middleware_tfm_s_platform true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/interface/src/tfm_platform_api.c
  ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/platform/auto_generated/intermedia_tfm_platform.c
  ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/platform/auto_generated/load_info_tfm_platform.c
  ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/platform/platform_sp.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/platform
)

if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

  target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
    -DTFM_PARTITION_PLATFORM
  )

endif()

