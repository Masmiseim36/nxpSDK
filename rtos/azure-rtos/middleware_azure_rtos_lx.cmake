# Add set(CONFIG_USE_middleware_azure_rtos_lx true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_azure_rtos_fx OR CONFIG_USE_middleware_azure_rtos_fx_sp)

if((CONFIG_TOOLCHAIN STREQUAL iar OR CONFIG_TOOLCHAIN STREQUAL mdk OR CONFIG_TOOLCHAIN STREQUAL mcux))
  target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
      ${CMAKE_CURRENT_LIST_DIR}/levelx/common/src/fx_nand_flash_simulated_driver.c
      ${CMAKE_CURRENT_LIST_DIR}/levelx/common/src/fx_nor_flash_simulator_driver.c
      ${CMAKE_CURRENT_LIST_DIR}/levelx/common/src/lx_nand_flash_256byte_ecc_check.c
      ${CMAKE_CURRENT_LIST_DIR}/levelx/common/src/lx_nand_flash_256byte_ecc_compute.c
      ${CMAKE_CURRENT_LIST_DIR}/levelx/common/src/lx_nand_flash_block_allocate.c
      ${CMAKE_CURRENT_LIST_DIR}/levelx/common/src/lx_nand_flash_block_data_move.c
      ${CMAKE_CURRENT_LIST_DIR}/levelx/common/src/lx_nand_flash_block_find.c
      ${CMAKE_CURRENT_LIST_DIR}/levelx/common/src/lx_nand_flash_block_mapping_set.c
      ${CMAKE_CURRENT_LIST_DIR}/levelx/common/src/lx_nand_flash_block_status_set.c
      ${CMAKE_CURRENT_LIST_DIR}/levelx/common/src/lx_nand_flash_close.c
      ${CMAKE_CURRENT_LIST_DIR}/levelx/common/src/lx_nand_flash_data_page_copy.c
      ${CMAKE_CURRENT_LIST_DIR}/levelx/common/src/lx_nand_flash_defragment.c
      ${CMAKE_CURRENT_LIST_DIR}/levelx/common/src/lx_nand_flash_driver_block_erase.c
      ${CMAKE_CURRENT_LIST_DIR}/levelx/common/src/lx_nand_flash_driver_block_erased_verify.c
      ${CMAKE_CURRENT_LIST_DIR}/levelx/common/src/lx_nand_flash_driver_block_status_get.c
      ${CMAKE_CURRENT_LIST_DIR}/levelx/common/src/lx_nand_flash_driver_block_status_set.c
      ${CMAKE_CURRENT_LIST_DIR}/levelx/common/src/lx_nand_flash_driver_page_erased_verify.c
      ${CMAKE_CURRENT_LIST_DIR}/levelx/common/src/lx_nand_flash_erase_count_set.c
      ${CMAKE_CURRENT_LIST_DIR}/levelx/common/src/lx_nand_flash_extended_cache_enable.c
      ${CMAKE_CURRENT_LIST_DIR}/levelx/common/src/lx_nand_flash_format.c
      ${CMAKE_CURRENT_LIST_DIR}/levelx/common/src/lx_nand_flash_free_block_list_add.c
      ${CMAKE_CURRENT_LIST_DIR}/levelx/common/src/lx_nand_flash_initialize.c
      ${CMAKE_CURRENT_LIST_DIR}/levelx/common/src/lx_nand_flash_mapped_block_list_add.c
      ${CMAKE_CURRENT_LIST_DIR}/levelx/common/src/lx_nand_flash_mapped_block_list_get.c
      ${CMAKE_CURRENT_LIST_DIR}/levelx/common/src/lx_nand_flash_mapped_block_list_remove.c
      ${CMAKE_CURRENT_LIST_DIR}/levelx/common/src/lx_nand_flash_memory_initialize.c
      ${CMAKE_CURRENT_LIST_DIR}/levelx/common/src/lx_nand_flash_metadata_allocate.c
      ${CMAKE_CURRENT_LIST_DIR}/levelx/common/src/lx_nand_flash_metadata_build.c
      ${CMAKE_CURRENT_LIST_DIR}/levelx/common/src/lx_nand_flash_metadata_write.c
      ${CMAKE_CURRENT_LIST_DIR}/levelx/common/src/lx_nand_flash_open.c
      ${CMAKE_CURRENT_LIST_DIR}/levelx/common/src/lx_nand_flash_page_ecc_check.c
      ${CMAKE_CURRENT_LIST_DIR}/levelx/common/src/lx_nand_flash_page_ecc_compute.c
      ${CMAKE_CURRENT_LIST_DIR}/levelx/common/src/lx_nand_flash_partial_defragment.c
      ${CMAKE_CURRENT_LIST_DIR}/levelx/common/src/lx_nand_flash_sector_read.c
      ${CMAKE_CURRENT_LIST_DIR}/levelx/common/src/lx_nand_flash_sector_release.c
      ${CMAKE_CURRENT_LIST_DIR}/levelx/common/src/lx_nand_flash_sector_write.c
      ${CMAKE_CURRENT_LIST_DIR}/levelx/common/src/lx_nand_flash_sectors_read.c
      ${CMAKE_CURRENT_LIST_DIR}/levelx/common/src/lx_nand_flash_sectors_release.c
      ${CMAKE_CURRENT_LIST_DIR}/levelx/common/src/lx_nand_flash_sectors_write.c
      ${CMAKE_CURRENT_LIST_DIR}/levelx/common/src/lx_nand_flash_simulator.c
      ${CMAKE_CURRENT_LIST_DIR}/levelx/common/src/lx_nand_flash_system_error.c
      ${CMAKE_CURRENT_LIST_DIR}/levelx/common/src/lx_nor_flash_block_reclaim.c
      ${CMAKE_CURRENT_LIST_DIR}/levelx/common/src/lx_nor_flash_close.c
      ${CMAKE_CURRENT_LIST_DIR}/levelx/common/src/lx_nor_flash_defragment.c
      ${CMAKE_CURRENT_LIST_DIR}/levelx/common/src/lx_nor_flash_driver_block_erase.c
      ${CMAKE_CURRENT_LIST_DIR}/levelx/common/src/lx_nor_flash_driver_read.c
      ${CMAKE_CURRENT_LIST_DIR}/levelx/common/src/lx_nor_flash_driver_write.c
      ${CMAKE_CURRENT_LIST_DIR}/levelx/common/src/lx_nor_flash_extended_cache_enable.c
      ${CMAKE_CURRENT_LIST_DIR}/levelx/common/src/lx_nor_flash_initialize.c
      ${CMAKE_CURRENT_LIST_DIR}/levelx/common/src/lx_nor_flash_logical_sector_find.c
      ${CMAKE_CURRENT_LIST_DIR}/levelx/common/src/lx_nor_flash_next_block_to_erase_find.c
      ${CMAKE_CURRENT_LIST_DIR}/levelx/common/src/lx_nor_flash_open.c
      ${CMAKE_CURRENT_LIST_DIR}/levelx/common/src/lx_nor_flash_partial_defragment.c
      ${CMAKE_CURRENT_LIST_DIR}/levelx/common/src/lx_nor_flash_physical_sector_allocate.c
      ${CMAKE_CURRENT_LIST_DIR}/levelx/common/src/lx_nor_flash_sector_mapping_cache_invalidate.c
      ${CMAKE_CURRENT_LIST_DIR}/levelx/common/src/lx_nor_flash_sector_read.c
      ${CMAKE_CURRENT_LIST_DIR}/levelx/common/src/lx_nor_flash_sector_release.c
      ${CMAKE_CURRENT_LIST_DIR}/levelx/common/src/lx_nor_flash_sector_write.c
      ${CMAKE_CURRENT_LIST_DIR}/levelx/common/src/lx_nor_flash_simulator.c
      ${CMAKE_CURRENT_LIST_DIR}/levelx/common/src/lx_nor_flash_system_error.c
  )
endif()

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/levelx/common/inc
)

if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

  target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
    -DLX_THREAD_SAFE_ENABLE
  )

  if(CONFIG_TOOLCHAIN STREQUAL armgcc)
    target_compile_options(${MCUX_SDK_PROJECT_NAME} PUBLIC
    )
  endif()

endif()

else()

message(SEND_ERROR "middleware_azure_rtos_lx dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
