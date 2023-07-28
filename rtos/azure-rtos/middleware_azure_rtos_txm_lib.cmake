# Add set(CONFIG_USE_middleware_azure_rtos_txm_lib true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/threadx/common_modules/module_lib/src/txm_block_allocate.c
  ${CMAKE_CURRENT_LIST_DIR}/threadx/common_modules/module_lib/src/txm_block_pool_create.c
  ${CMAKE_CURRENT_LIST_DIR}/threadx/common_modules/module_lib/src/txm_block_pool_delete.c
  ${CMAKE_CURRENT_LIST_DIR}/threadx/common_modules/module_lib/src/txm_block_pool_info_get.c
  ${CMAKE_CURRENT_LIST_DIR}/threadx/common_modules/module_lib/src/txm_block_pool_performance_info_get.c
  ${CMAKE_CURRENT_LIST_DIR}/threadx/common_modules/module_lib/src/txm_block_pool_performance_system_info_get.c
  ${CMAKE_CURRENT_LIST_DIR}/threadx/common_modules/module_lib/src/txm_block_pool_prioritize.c
  ${CMAKE_CURRENT_LIST_DIR}/threadx/common_modules/module_lib/src/txm_block_release.c
  ${CMAKE_CURRENT_LIST_DIR}/threadx/common_modules/module_lib/src/txm_byte_allocate.c
  ${CMAKE_CURRENT_LIST_DIR}/threadx/common_modules/module_lib/src/txm_byte_pool_create.c
  ${CMAKE_CURRENT_LIST_DIR}/threadx/common_modules/module_lib/src/txm_byte_pool_delete.c
  ${CMAKE_CURRENT_LIST_DIR}/threadx/common_modules/module_lib/src/txm_byte_pool_info_get.c
  ${CMAKE_CURRENT_LIST_DIR}/threadx/common_modules/module_lib/src/txm_byte_pool_performance_info_get.c
  ${CMAKE_CURRENT_LIST_DIR}/threadx/common_modules/module_lib/src/txm_byte_pool_performance_system_info_get.c
  ${CMAKE_CURRENT_LIST_DIR}/threadx/common_modules/module_lib/src/txm_byte_pool_prioritize.c
  ${CMAKE_CURRENT_LIST_DIR}/threadx/common_modules/module_lib/src/txm_byte_release.c
  ${CMAKE_CURRENT_LIST_DIR}/threadx/common_modules/module_lib/src/txm_event_flags_create.c
  ${CMAKE_CURRENT_LIST_DIR}/threadx/common_modules/module_lib/src/txm_event_flags_delete.c
  ${CMAKE_CURRENT_LIST_DIR}/threadx/common_modules/module_lib/src/txm_event_flags_get.c
  ${CMAKE_CURRENT_LIST_DIR}/threadx/common_modules/module_lib/src/txm_event_flags_info_get.c
  ${CMAKE_CURRENT_LIST_DIR}/threadx/common_modules/module_lib/src/txm_event_flags_performance_info_get.c
  ${CMAKE_CURRENT_LIST_DIR}/threadx/common_modules/module_lib/src/txm_event_flags_performance_system_info_get.c
  ${CMAKE_CURRENT_LIST_DIR}/threadx/common_modules/module_lib/src/txm_event_flags_set.c
  ${CMAKE_CURRENT_LIST_DIR}/threadx/common_modules/module_lib/src/txm_event_flags_set_notify.c
  ${CMAKE_CURRENT_LIST_DIR}/threadx/common_modules/module_lib/src/txm_module_application_request.c
  ${CMAKE_CURRENT_LIST_DIR}/threadx/common_modules/module_lib/src/txm_module_callback_request_thread_entry.c
  ${CMAKE_CURRENT_LIST_DIR}/threadx/common_modules/module_lib/src/txm_module_object_allocate.c
  ${CMAKE_CURRENT_LIST_DIR}/threadx/common_modules/module_lib/src/txm_module_object_deallocate.c
  ${CMAKE_CURRENT_LIST_DIR}/threadx/common_modules/module_lib/src/txm_module_object_pointer_get.c
  ${CMAKE_CURRENT_LIST_DIR}/threadx/common_modules/module_lib/src/txm_module_object_pointer_get_extended.c
  ${CMAKE_CURRENT_LIST_DIR}/threadx/common_modules/module_lib/src/txm_module_thread_system_suspend.c
  ${CMAKE_CURRENT_LIST_DIR}/threadx/common_modules/module_lib/src/txm_mutex_create.c
  ${CMAKE_CURRENT_LIST_DIR}/threadx/common_modules/module_lib/src/txm_mutex_delete.c
  ${CMAKE_CURRENT_LIST_DIR}/threadx/common_modules/module_lib/src/txm_mutex_get.c
  ${CMAKE_CURRENT_LIST_DIR}/threadx/common_modules/module_lib/src/txm_mutex_info_get.c
  ${CMAKE_CURRENT_LIST_DIR}/threadx/common_modules/module_lib/src/txm_mutex_performance_info_get.c
  ${CMAKE_CURRENT_LIST_DIR}/threadx/common_modules/module_lib/src/txm_mutex_performance_system_info_get.c
  ${CMAKE_CURRENT_LIST_DIR}/threadx/common_modules/module_lib/src/txm_mutex_prioritize.c
  ${CMAKE_CURRENT_LIST_DIR}/threadx/common_modules/module_lib/src/txm_mutex_put.c
  ${CMAKE_CURRENT_LIST_DIR}/threadx/common_modules/module_lib/src/txm_queue_create.c
  ${CMAKE_CURRENT_LIST_DIR}/threadx/common_modules/module_lib/src/txm_queue_delete.c
  ${CMAKE_CURRENT_LIST_DIR}/threadx/common_modules/module_lib/src/txm_queue_flush.c
  ${CMAKE_CURRENT_LIST_DIR}/threadx/common_modules/module_lib/src/txm_queue_front_send.c
  ${CMAKE_CURRENT_LIST_DIR}/threadx/common_modules/module_lib/src/txm_queue_info_get.c
  ${CMAKE_CURRENT_LIST_DIR}/threadx/common_modules/module_lib/src/txm_queue_performance_info_get.c
  ${CMAKE_CURRENT_LIST_DIR}/threadx/common_modules/module_lib/src/txm_queue_performance_system_info_get.c
  ${CMAKE_CURRENT_LIST_DIR}/threadx/common_modules/module_lib/src/txm_queue_prioritize.c
  ${CMAKE_CURRENT_LIST_DIR}/threadx/common_modules/module_lib/src/txm_queue_receive.c
  ${CMAKE_CURRENT_LIST_DIR}/threadx/common_modules/module_lib/src/txm_queue_send.c
  ${CMAKE_CURRENT_LIST_DIR}/threadx/common_modules/module_lib/src/txm_queue_send_notify.c
  ${CMAKE_CURRENT_LIST_DIR}/threadx/common_modules/module_lib/src/txm_semaphore_ceiling_put.c
  ${CMAKE_CURRENT_LIST_DIR}/threadx/common_modules/module_lib/src/txm_semaphore_create.c
  ${CMAKE_CURRENT_LIST_DIR}/threadx/common_modules/module_lib/src/txm_semaphore_delete.c
  ${CMAKE_CURRENT_LIST_DIR}/threadx/common_modules/module_lib/src/txm_semaphore_get.c
  ${CMAKE_CURRENT_LIST_DIR}/threadx/common_modules/module_lib/src/txm_semaphore_info_get.c
  ${CMAKE_CURRENT_LIST_DIR}/threadx/common_modules/module_lib/src/txm_semaphore_performance_info_get.c
  ${CMAKE_CURRENT_LIST_DIR}/threadx/common_modules/module_lib/src/txm_semaphore_performance_system_info_get.c
  ${CMAKE_CURRENT_LIST_DIR}/threadx/common_modules/module_lib/src/txm_semaphore_prioritize.c
  ${CMAKE_CURRENT_LIST_DIR}/threadx/common_modules/module_lib/src/txm_semaphore_put.c
  ${CMAKE_CURRENT_LIST_DIR}/threadx/common_modules/module_lib/src/txm_semaphore_put_notify.c
  ${CMAKE_CURRENT_LIST_DIR}/threadx/common_modules/module_lib/src/txm_thread_create.c
  ${CMAKE_CURRENT_LIST_DIR}/threadx/common_modules/module_lib/src/txm_thread_delete.c
  ${CMAKE_CURRENT_LIST_DIR}/threadx/common_modules/module_lib/src/txm_thread_entry_exit_notify.c
  ${CMAKE_CURRENT_LIST_DIR}/threadx/common_modules/module_lib/src/txm_thread_identify.c
  ${CMAKE_CURRENT_LIST_DIR}/threadx/common_modules/module_lib/src/txm_thread_info_get.c
  ${CMAKE_CURRENT_LIST_DIR}/threadx/common_modules/module_lib/src/txm_thread_interrupt_control.c
  ${CMAKE_CURRENT_LIST_DIR}/threadx/common_modules/module_lib/src/txm_thread_performance_info_get.c
  ${CMAKE_CURRENT_LIST_DIR}/threadx/common_modules/module_lib/src/txm_thread_performance_system_info_get.c
  ${CMAKE_CURRENT_LIST_DIR}/threadx/common_modules/module_lib/src/txm_thread_preemption_change.c
  ${CMAKE_CURRENT_LIST_DIR}/threadx/common_modules/module_lib/src/txm_thread_priority_change.c
  ${CMAKE_CURRENT_LIST_DIR}/threadx/common_modules/module_lib/src/txm_thread_relinquish.c
  ${CMAKE_CURRENT_LIST_DIR}/threadx/common_modules/module_lib/src/txm_thread_reset.c
  ${CMAKE_CURRENT_LIST_DIR}/threadx/common_modules/module_lib/src/txm_thread_resume.c
  ${CMAKE_CURRENT_LIST_DIR}/threadx/common_modules/module_lib/src/txm_thread_sleep.c
  ${CMAKE_CURRENT_LIST_DIR}/threadx/common_modules/module_lib/src/txm_thread_stack_error_notify.c
  ${CMAKE_CURRENT_LIST_DIR}/threadx/common_modules/module_lib/src/txm_thread_suspend.c
  ${CMAKE_CURRENT_LIST_DIR}/threadx/common_modules/module_lib/src/txm_thread_terminate.c
  ${CMAKE_CURRENT_LIST_DIR}/threadx/common_modules/module_lib/src/txm_thread_time_slice_change.c
  ${CMAKE_CURRENT_LIST_DIR}/threadx/common_modules/module_lib/src/txm_thread_wait_abort.c
  ${CMAKE_CURRENT_LIST_DIR}/threadx/common_modules/module_lib/src/txm_time_get.c
  ${CMAKE_CURRENT_LIST_DIR}/threadx/common_modules/module_lib/src/txm_time_set.c
  ${CMAKE_CURRENT_LIST_DIR}/threadx/common_modules/module_lib/src/txm_timer_activate.c
  ${CMAKE_CURRENT_LIST_DIR}/threadx/common_modules/module_lib/src/txm_timer_change.c
  ${CMAKE_CURRENT_LIST_DIR}/threadx/common_modules/module_lib/src/txm_timer_create.c
  ${CMAKE_CURRENT_LIST_DIR}/threadx/common_modules/module_lib/src/txm_timer_deactivate.c
  ${CMAKE_CURRENT_LIST_DIR}/threadx/common_modules/module_lib/src/txm_timer_delete.c
  ${CMAKE_CURRENT_LIST_DIR}/threadx/common_modules/module_lib/src/txm_timer_info_get.c
  ${CMAKE_CURRENT_LIST_DIR}/threadx/common_modules/module_lib/src/txm_timer_performance_info_get.c
  ${CMAKE_CURRENT_LIST_DIR}/threadx/common_modules/module_lib/src/txm_timer_performance_system_info_get.c
  ${CMAKE_CURRENT_LIST_DIR}/threadx/common_modules/module_lib/src/txm_trace_buffer_full_notify.c
  ${CMAKE_CURRENT_LIST_DIR}/threadx/common_modules/module_lib/src/txm_trace_disable.c
  ${CMAKE_CURRENT_LIST_DIR}/threadx/common_modules/module_lib/src/txm_trace_enable.c
  ${CMAKE_CURRENT_LIST_DIR}/threadx/common_modules/module_lib/src/txm_trace_event_filter.c
  ${CMAKE_CURRENT_LIST_DIR}/threadx/common_modules/module_lib/src/txm_trace_event_unfilter.c
  ${CMAKE_CURRENT_LIST_DIR}/threadx/common_modules/module_lib/src/txm_trace_interrupt_control.c
  ${CMAKE_CURRENT_LIST_DIR}/threadx/common_modules/module_lib/src/txm_trace_isr_enter_insert.c
  ${CMAKE_CURRENT_LIST_DIR}/threadx/common_modules/module_lib/src/txm_trace_isr_exit_insert.c
  ${CMAKE_CURRENT_LIST_DIR}/threadx/common_modules/module_lib/src/txm_trace_user_event_insert.c
)

if(CONFIG_TOOLCHAIN STREQUAL iar AND CONFIG_CORE STREQUAL cm7f)
  target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
      ${CMAKE_CURRENT_LIST_DIR}/threadx/ports_module/cortex_m7/iar/module_lib/src/txm_module_thread_shell_entry.c
  )
endif()

if(CONFIG_TOOLCHAIN STREQUAL mdk AND CONFIG_CORE STREQUAL cm7f)
  target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
      ${CMAKE_CURRENT_LIST_DIR}/threadx/ports_module/cortex_m7/ac6/module_lib/src/txm_module_initialize.S
      ${CMAKE_CURRENT_LIST_DIR}/threadx/ports_module/cortex_m7/ac6/module_lib/src/txm_module_thread_shell_entry.c
  )
endif()

if(CONFIG_TOOLCHAIN STREQUAL mcux AND CONFIG_CORE STREQUAL cm7f)
  target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
      ${CMAKE_CURRENT_LIST_DIR}/threadx/ports_module/cortex_m7/gnu/module_lib/src/txm_module_thread_shell_entry.c
  )
endif()

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/threadx/common/inc
  ${CMAKE_CURRENT_LIST_DIR}/threadx/common_modules/inc
)

if(CONFIG_TOOLCHAIN STREQUAL iar AND CONFIG_CORE STREQUAL cm7f)
target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/threadx/ports_module/cortex_m7/iar/inc
)
endif()

if(CONFIG_TOOLCHAIN STREQUAL mdk AND CONFIG_CORE STREQUAL cm7f)
target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/threadx/ports_module/cortex_m7/ac6/inc
)
endif()

if(CONFIG_TOOLCHAIN STREQUAL mcux AND CONFIG_CORE STREQUAL cm7f)
target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/threadx/ports_module/cortex_m7/gnu/inc
)
endif()

if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

  target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
    -DTX_INCLUDE_USER_DEFINE_FILE
  )

endif()

