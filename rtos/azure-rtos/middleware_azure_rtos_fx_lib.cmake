# Add set(CONFIG_USE_middleware_azure_rtos_fx_lib true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_directory_attributes_read.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_directory_attributes_set.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_directory_create.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_directory_default_get.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_directory_default_get_copy.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_directory_default_set.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_directory_delete.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_directory_entry_read.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_directory_entry_write.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_directory_exFAT_entry_read.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_directory_exFAT_entry_write.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_directory_exFAT_free_search.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_directory_exFAT_unicode_entry_write.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_directory_first_entry_find.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_directory_first_full_entry_find.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_directory_free_search.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_directory_information_get.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_directory_local_path_clear.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_directory_local_path_get.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_directory_local_path_get_copy.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_directory_local_path_restore.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_directory_local_path_set.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_directory_long_name_get.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_directory_long_name_get_extended.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_directory_name_extract.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_directory_name_test.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_directory_next_entry_find.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_directory_next_full_entry_find.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_directory_rename.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_directory_search.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_directory_short_name_get.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_directory_short_name_get_extended.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_fault_tolerant_add_FAT_log.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_fault_tolerant_add_bitmap_log.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_fault_tolerant_add_checksum_log.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_fault_tolerant_add_dir_log.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_fault_tolerant_apply_logs.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_fault_tolerant_calculate_checksum.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_fault_tolerant_cleanup_FAT_chain.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_fault_tolerant_create_log_file.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_fault_tolerant_enable.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_fault_tolerant_read_FAT.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_fault_tolerant_read_directory_sector.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_fault_tolerant_read_log_file.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_fault_tolerant_recover.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_fault_tolerant_reset_log_file.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_fault_tolerant_set_FAT_chain.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_fault_tolerant_transaction_end.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_fault_tolerant_transaction_fail.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_fault_tolerant_transaction_start.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_fault_tolerant_write_log_file.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_file_allocate.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_file_attributes_read.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_file_attributes_set.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_file_best_effort_allocate.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_file_close.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_file_create.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_file_date_time_set.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_file_delete.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_file_extended_allocate.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_file_extended_best_effort_allocate.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_file_extended_relative_seek.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_file_extended_seek.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_file_extended_truncate.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_file_extended_truncate_release.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_file_open.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_file_read.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_file_relative_seek.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_file_rename.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_file_seek.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_file_truncate.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_file_truncate_release.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_file_write.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_file_write_notify_set.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_media_abort.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_media_boot_info_extract.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_media_cache_invalidate.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_media_check.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_media_check_FAT_chain_check.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_media_check_lost_cluster_check.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_media_close.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_media_close_notify_set.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_media_exFAT_format.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_media_extended_space_available.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_media_flush.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_media_format.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_media_format_oem_name_set.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_media_format_type_set.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_media_format_volume_id_set.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_media_open.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_media_open_notify_set.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_media_read.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_media_space_available.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_media_volume_get.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_media_volume_get_extended.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_media_volume_set.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_media_write.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_partition_offset_calculate.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_ram_driver.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_system_date_get.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_system_date_set.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_system_initialize.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_system_time_get.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_system_time_set.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_system_timer_entry.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_trace_event_insert.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_trace_event_update.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_trace_object_register.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_trace_object_unregister.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_unicode_directory_create.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_unicode_directory_entry_change.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_unicode_directory_entry_read.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_unicode_directory_rename.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_unicode_directory_search.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_unicode_file_create.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_unicode_file_rename.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_unicode_length_get.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_unicode_length_get_extended.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_unicode_name_get.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_unicode_name_get_extended.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_unicode_short_name_get.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_unicode_short_name_get_extended.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_utility_16_unsigned_read.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_utility_16_unsigned_write.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_utility_32_unsigned_read.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_utility_32_unsigned_write.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_utility_64_unsigned_read.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_utility_64_unsigned_write.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_utility_FAT_entry_read.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_utility_FAT_entry_write.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_utility_FAT_flush.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_utility_FAT_map_flush.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_utility_FAT_sector_get.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_utility_absolute_path_get.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_utility_exFAT_allocate_new_cluster.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_utility_exFAT_bitmap_cache_prepare.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_utility_exFAT_bitmap_cache_update.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_utility_exFAT_bitmap_flush.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_utility_exFAT_bitmap_free_cluster_find.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_utility_exFAT_bitmap_initialize.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_utility_exFAT_bitmap_start_sector_get.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_utility_exFAT_cluster_free.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_utility_exFAT_cluster_state_get.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_utility_exFAT_cluster_state_set.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_utility_exFAT_geometry_check.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_utility_exFAT_name_hash_get.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_utility_exFAT_size_calculate.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_utility_exFAT_system_area_checksum_verify.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_utility_exFAT_system_area_checksum_write.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_utility_exFAT_system_area_format.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_utility_exFAT_system_sector_write.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_utility_exFAT_unicode_name_hash_get.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_utility_exFAT_upcase_table.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_utility_logical_sector_cache_entry_read.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_utility_logical_sector_flush.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_utility_logical_sector_read.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_utility_logical_sector_write.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_utility_memory_copy.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_utility_memory_set.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_utility_string_length_get.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fx_utility_token_length_get.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fxe_directory_attributes_read.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fxe_directory_attributes_set.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fxe_directory_create.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fxe_directory_default_get.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fxe_directory_default_get_copy.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fxe_directory_default_set.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fxe_directory_delete.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fxe_directory_first_entry_find.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fxe_directory_first_full_entry_find.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fxe_directory_information_get.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fxe_directory_local_path_clear.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fxe_directory_local_path_get.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fxe_directory_local_path_get_copy.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fxe_directory_local_path_restore.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fxe_directory_local_path_set.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fxe_directory_long_name_get.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fxe_directory_long_name_get_extended.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fxe_directory_name_test.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fxe_directory_next_entry_find.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fxe_directory_next_full_entry_find.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fxe_directory_rename.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fxe_directory_short_name_get.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fxe_directory_short_name_get_extended.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fxe_fault_tolerant_enable.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fxe_file_allocate.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fxe_file_attributes_read.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fxe_file_attributes_set.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fxe_file_best_effort_allocate.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fxe_file_close.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fxe_file_create.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fxe_file_date_time_set.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fxe_file_delete.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fxe_file_extended_allocate.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fxe_file_extended_best_effort_allocate.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fxe_file_extended_relative_seek.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fxe_file_extended_seek.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fxe_file_extended_truncate.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fxe_file_extended_truncate_release.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fxe_file_open.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fxe_file_read.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fxe_file_relative_seek.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fxe_file_rename.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fxe_file_seek.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fxe_file_truncate.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fxe_file_truncate_release.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fxe_file_write.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fxe_file_write_notify_set.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fxe_media_abort.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fxe_media_cache_invalidate.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fxe_media_check.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fxe_media_close.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fxe_media_close_notify_set.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fxe_media_exFAT_format.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fxe_media_extended_space_available.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fxe_media_flush.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fxe_media_format.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fxe_media_open.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fxe_media_open_notify_set.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fxe_media_read.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fxe_media_space_available.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fxe_media_volume_get.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fxe_media_volume_get_extended.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fxe_media_volume_set.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fxe_media_write.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fxe_system_date_get.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fxe_system_date_set.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fxe_system_time_get.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fxe_system_time_set.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fxe_unicode_directory_create.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fxe_unicode_directory_rename.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fxe_unicode_file_create.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fxe_unicode_file_rename.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fxe_unicode_name_get.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fxe_unicode_name_get_extended.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fxe_unicode_short_name_get.c
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/src/fxe_unicode_short_name_get_extended.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/threadx/common/inc
  ${CMAKE_CURRENT_LIST_DIR}/filex/common/inc
)

if(CONFIG_TOOLCHAIN STREQUAL iar AND CONFIG_CORE STREQUAL cm7f)
target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/threadx/ports/cortex_m7/iar/inc
)
endif()

if(CONFIG_TOOLCHAIN STREQUAL iar AND CONFIG_CORE STREQUAL cm4f)
target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/threadx/ports/cortex_m4/iar/inc
)
endif()

if(CONFIG_TOOLCHAIN STREQUAL mcux AND CONFIG_CORE STREQUAL cm7f)
target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/threadx/ports/cortex_m7/gnu/inc
)
endif()

if(CONFIG_TOOLCHAIN STREQUAL mcux AND CONFIG_CORE STREQUAL cm4f)
target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/threadx/ports/cortex_m4/gnu/inc
)
endif()

if(CONFIG_TOOLCHAIN STREQUAL mdk AND CONFIG_CORE STREQUAL cm7f)
target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/threadx/ports/cortex_m7/ac6/inc
)
endif()

if(CONFIG_TOOLCHAIN STREQUAL mdk AND CONFIG_CORE STREQUAL cm4f)
target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/threadx/ports/cortex_m4/ac6/inc
)
endif()

if(CONFIG_TOOLCHAIN STREQUAL iar AND CONFIG_CORE STREQUAL cm33)
target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/threadx/ports/cortex_m33/iar/inc
)
endif()

if((CONFIG_TOOLCHAIN STREQUAL mcux OR CONFIG_TOOLCHAIN STREQUAL armgcc) AND CONFIG_CORE STREQUAL cm33)
target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/threadx/ports/cortex_m33/gnu/inc
  ${CMAKE_CURRENT_LIST_DIR}/filex/ports/cortex_m33/gnu/inc
)
endif()

if(CONFIG_TOOLCHAIN STREQUAL mdk AND CONFIG_CORE STREQUAL cm33)
target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/threadx/ports/cortex_m33/ac6/inc
)
endif()

if((CONFIG_TOOLCHAIN STREQUAL mcux OR CONFIG_TOOLCHAIN STREQUAL armgcc) AND CONFIG_CORE STREQUAL cm7f)
target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/filex/ports/cortex_m7/gnu/inc
)
endif()

if((CONFIG_TOOLCHAIN STREQUAL mcux OR CONFIG_TOOLCHAIN STREQUAL armgcc) AND CONFIG_CORE STREQUAL cm4f)
target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/filex/ports/cortex_m4/gnu/inc
)
endif()

if((CONFIG_TOOLCHAIN STREQUAL mdk OR CONFIG_TOOLCHAIN STREQUAL iar) AND (CONFIG_CORE STREQUAL cm7f OR CONFIG_CORE STREQUAL cm33 OR CONFIG_CORE STREQUAL cm4f))
target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/filex/ports/generic/inc
)
endif()

if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

  target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
    -DTX_INCLUDE_USER_DEFINE_FILE
    -DFX_INCLUDE_USER_DEFINE_FILE
  )

endif()

