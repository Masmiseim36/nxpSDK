# Add set(CONFIG_USE_middleware_tfm_s_platform true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(((CONFIG_USE_middleware_tfm_s_platform_sp_rw61x AND (CONFIG_DEVICE_ID STREQUAL RW610 OR CONFIG_DEVICE_ID STREQUAL RW612)) OR (CONFIG_USE_middleware_tfm_s_platform_sp_common AND (NOT (CONFIG_NOT STREQUAL RW610 OR CONFIG_NOT STREQUAL RW612)))))

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/interface/src/tfm_platform_api.c
  ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/platform/auto_generated/intermedia_tfm_platform.c
  ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/platform/auto_generated/load_info_tfm_platform.c
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

else()

message(SEND_ERROR "middleware_tfm_s_platform dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
