# Add set(CONFIG_USE_middleware_tfm_s_ps true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/interface/src/tfm_ps_api.c
  ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/protected_storage/auto_generated/intermedia_tfm_protected_storage.c
  ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/protected_storage/auto_generated/load_info_tfm_protected_storage.c
  ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/protected_storage/crypto/ps_crypto_interface.c
  ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/protected_storage/ps_encrypted_object.c
  ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/protected_storage/ps_object_system.c
  ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/protected_storage/ps_object_table.c
  ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/protected_storage/ps_utils.c
  ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/protected_storage/tfm_protected_storage.c
  ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/protected_storage/tfm_ps_req_mngr.c
  ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/protected_storage/nv_counters/ps_nv_counters.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/interface/include
  ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/protected_storage/crypto
  ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/protected_storage
  ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/protected_storage/nv_counters
  ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/platform
  ${CMAKE_CURRENT_LIST_DIR}/.
)

if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

  target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
    -DTFM_PARTITION_PROTECTED_STORAGE
  )

endif()

