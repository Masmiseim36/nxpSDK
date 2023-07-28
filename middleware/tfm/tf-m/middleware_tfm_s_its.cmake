# Add set(CONFIG_USE_middleware_tfm_s_its true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/interface/src/tfm_its_api.c
  ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/internal_trusted_storage/auto_generated/intermedia_tfm_internal_trusted_storage.c
  ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/internal_trusted_storage/auto_generated/load_info_tfm_internal_trusted_storage.c
  ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/internal_trusted_storage/its_utils.c
  ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/internal_trusted_storage/tfm_internal_trusted_storage.c
  ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/internal_trusted_storage/tfm_its_req_mngr.c
  ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/internal_trusted_storage/flash/its_flash.c
  ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/internal_trusted_storage/flash/its_flash_nand.c
  ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/internal_trusted_storage/flash/its_flash_nor.c
  ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/internal_trusted_storage/flash/its_flash_ram.c
  ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/internal_trusted_storage/flash_fs/its_flash_fs.c
  ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/internal_trusted_storage/flash_fs/its_flash_fs_dblock.c
  ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/internal_trusted_storage/flash_fs/its_flash_fs_mblock.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/interface/include
  ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/internal_trusted_storage
  ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/internal_trusted_storage/flash
  ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/internal_trusted_storage/flash_fs
  ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/protected_storage
  ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/platform
)

if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

  target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
    -DTFM_PARTITION_INTERNAL_TRUSTED_STORAGE
  )

endif()

