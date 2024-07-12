# Add set(CONFIG_USE_middleware_tfm_s_its_load_info_common true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/internal_trusted_storage/auto_generated/load_info_tfm_internal_trusted_storage.c
)

