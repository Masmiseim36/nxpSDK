# Add set(CONFIG_USE_middleware_azure_rtos_tx_template true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if((CONFIG_TOOLCHAIN STREQUAL mcux OR CONFIG_TOOLCHAIN STREQUAL armgcc) AND CONFIG_CORE STREQUAL cm7f)
  target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
      ${CMAKE_CURRENT_LIST_DIR}/threadx/ports/cortex_m7/gnu/src/tx_misra.S
      ${CMAKE_CURRENT_LIST_DIR}/threadx/ports/cortex_m7/gnu/src/tx_thread_context_restore.S
      ${CMAKE_CURRENT_LIST_DIR}/threadx/ports/cortex_m7/gnu/src/tx_thread_context_save.S
      ${CMAKE_CURRENT_LIST_DIR}/threadx/ports/cortex_m7/gnu/src/tx_thread_interrupt_control.S
      ${CMAKE_CURRENT_LIST_DIR}/threadx/ports/cortex_m7/gnu/src/tx_thread_interrupt_disable.S
      ${CMAKE_CURRENT_LIST_DIR}/threadx/ports/cortex_m7/gnu/src/tx_thread_interrupt_restore.S
      ${CMAKE_CURRENT_LIST_DIR}/threadx/ports/cortex_m7/gnu/src/tx_thread_schedule.S
      ${CMAKE_CURRENT_LIST_DIR}/threadx/ports/cortex_m7/gnu/src/tx_thread_stack_build.S
      ${CMAKE_CURRENT_LIST_DIR}/threadx/ports/cortex_m7/gnu/src/tx_thread_system_return.S
      ${CMAKE_CURRENT_LIST_DIR}/threadx/ports/cortex_m7/gnu/src/tx_timer_interrupt.S
  )
endif()

if((CONFIG_TOOLCHAIN STREQUAL mcux OR CONFIG_TOOLCHAIN STREQUAL armgcc) AND CONFIG_CORE STREQUAL cm4f)
  target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
      ${CMAKE_CURRENT_LIST_DIR}/threadx/ports/cortex_m4/gnu/src/tx_misra.S
      ${CMAKE_CURRENT_LIST_DIR}/threadx/ports/cortex_m4/gnu/src/tx_thread_context_restore.S
      ${CMAKE_CURRENT_LIST_DIR}/threadx/ports/cortex_m4/gnu/src/tx_thread_context_save.S
      ${CMAKE_CURRENT_LIST_DIR}/threadx/ports/cortex_m4/gnu/src/tx_thread_interrupt_control.S
      ${CMAKE_CURRENT_LIST_DIR}/threadx/ports/cortex_m4/gnu/src/tx_thread_interrupt_disable.S
      ${CMAKE_CURRENT_LIST_DIR}/threadx/ports/cortex_m4/gnu/src/tx_thread_interrupt_restore.S
      ${CMAKE_CURRENT_LIST_DIR}/threadx/ports/cortex_m4/gnu/src/tx_thread_schedule.S
      ${CMAKE_CURRENT_LIST_DIR}/threadx/ports/cortex_m4/gnu/src/tx_thread_stack_build.S
      ${CMAKE_CURRENT_LIST_DIR}/threadx/ports/cortex_m4/gnu/src/tx_thread_system_return.S
      ${CMAKE_CURRENT_LIST_DIR}/threadx/ports/cortex_m4/gnu/src/tx_timer_interrupt.S
  )
endif()

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/threadx/ports/common/tx_initialize_low_level.c
)

if(CONFIG_TOOLCHAIN STREQUAL iar AND CONFIG_CORE STREQUAL cm7f)
  target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
      ${CMAKE_CURRENT_LIST_DIR}/threadx/ports/cortex_m7/iar/src/tx_thread_context_restore.s
      ${CMAKE_CURRENT_LIST_DIR}/threadx/ports/cortex_m7/iar/src/tx_thread_context_save.s
      ${CMAKE_CURRENT_LIST_DIR}/threadx/ports/cortex_m7/iar/src/tx_thread_interrupt_control.s
      ${CMAKE_CURRENT_LIST_DIR}/threadx/ports/cortex_m7/iar/src/tx_thread_interrupt_disable.s
      ${CMAKE_CURRENT_LIST_DIR}/threadx/ports/cortex_m7/iar/src/tx_thread_interrupt_restore.s
      ${CMAKE_CURRENT_LIST_DIR}/threadx/ports/cortex_m7/iar/src/tx_thread_schedule.s
      ${CMAKE_CURRENT_LIST_DIR}/threadx/ports/cortex_m7/iar/src/tx_thread_stack_build.s
      ${CMAKE_CURRENT_LIST_DIR}/threadx/ports/cortex_m7/iar/src/tx_thread_system_return.s
      ${CMAKE_CURRENT_LIST_DIR}/threadx/ports/cortex_m7/iar/src/tx_timer_interrupt.s
  )
endif()

if(CONFIG_TOOLCHAIN STREQUAL iar AND CONFIG_CORE STREQUAL cm4f)
  target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
      ${CMAKE_CURRENT_LIST_DIR}/threadx/ports/cortex_m4/iar/src/tx_thread_context_restore.s
      ${CMAKE_CURRENT_LIST_DIR}/threadx/ports/cortex_m4/iar/src/tx_thread_context_save.s
      ${CMAKE_CURRENT_LIST_DIR}/threadx/ports/cortex_m4/iar/src/tx_thread_interrupt_control.s
      ${CMAKE_CURRENT_LIST_DIR}/threadx/ports/cortex_m4/iar/src/tx_thread_interrupt_disable.s
      ${CMAKE_CURRENT_LIST_DIR}/threadx/ports/cortex_m4/iar/src/tx_thread_interrupt_restore.s
      ${CMAKE_CURRENT_LIST_DIR}/threadx/ports/cortex_m4/iar/src/tx_thread_schedule.s
      ${CMAKE_CURRENT_LIST_DIR}/threadx/ports/cortex_m4/iar/src/tx_thread_stack_build.s
      ${CMAKE_CURRENT_LIST_DIR}/threadx/ports/cortex_m4/iar/src/tx_thread_system_return.s
      ${CMAKE_CURRENT_LIST_DIR}/threadx/ports/cortex_m4/iar/src/tx_timer_interrupt.s
  )
endif()

if(CONFIG_TOOLCHAIN STREQUAL mdk AND CONFIG_CORE STREQUAL cm7f)
  target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
      ${CMAKE_CURRENT_LIST_DIR}/threadx/ports/cortex_m7/ac6/src/tx_thread_context_restore.S
      ${CMAKE_CURRENT_LIST_DIR}/threadx/ports/cortex_m7/ac6/src/tx_thread_context_save.S
      ${CMAKE_CURRENT_LIST_DIR}/threadx/ports/cortex_m7/ac6/src/tx_thread_interrupt_control.S
      ${CMAKE_CURRENT_LIST_DIR}/threadx/ports/cortex_m7/ac6/src/tx_thread_interrupt_disable.S
      ${CMAKE_CURRENT_LIST_DIR}/threadx/ports/cortex_m7/ac6/src/tx_thread_interrupt_restore.S
      ${CMAKE_CURRENT_LIST_DIR}/threadx/ports/cortex_m7/ac6/src/tx_thread_schedule.S
      ${CMAKE_CURRENT_LIST_DIR}/threadx/ports/cortex_m7/ac6/src/tx_thread_stack_build.S
      ${CMAKE_CURRENT_LIST_DIR}/threadx/ports/cortex_m7/ac6/src/tx_thread_system_return.S
      ${CMAKE_CURRENT_LIST_DIR}/threadx/ports/cortex_m7/ac6/src/tx_timer_interrupt.S
  )
endif()

if(CONFIG_TOOLCHAIN STREQUAL mdk AND CONFIG_CORE STREQUAL cm4f)
  target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
      ${CMAKE_CURRENT_LIST_DIR}/threadx/ports/cortex_m4/ac6/src/tx_thread_context_restore.S
      ${CMAKE_CURRENT_LIST_DIR}/threadx/ports/cortex_m4/ac6/src/tx_thread_context_save.S
      ${CMAKE_CURRENT_LIST_DIR}/threadx/ports/cortex_m4/ac6/src/tx_thread_interrupt_control.S
      ${CMAKE_CURRENT_LIST_DIR}/threadx/ports/cortex_m4/ac6/src/tx_thread_interrupt_disable.S
      ${CMAKE_CURRENT_LIST_DIR}/threadx/ports/cortex_m4/ac6/src/tx_thread_interrupt_restore.S
      ${CMAKE_CURRENT_LIST_DIR}/threadx/ports/cortex_m4/ac6/src/tx_thread_schedule.S
      ${CMAKE_CURRENT_LIST_DIR}/threadx/ports/cortex_m4/ac6/src/tx_thread_stack_build.S
      ${CMAKE_CURRENT_LIST_DIR}/threadx/ports/cortex_m4/ac6/src/tx_thread_system_return.S
      ${CMAKE_CURRENT_LIST_DIR}/threadx/ports/cortex_m4/ac6/src/tx_timer_interrupt.S
  )
endif()

if((CONFIG_TOOLCHAIN STREQUAL mcux OR CONFIG_TOOLCHAIN STREQUAL armgcc) AND CONFIG_CORE STREQUAL cm33)
  target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
      ${CMAKE_CURRENT_LIST_DIR}/threadx/ports/cortex_m33/gnu/src/tx_thread_context_restore.S
      ${CMAKE_CURRENT_LIST_DIR}/threadx/ports/cortex_m33/gnu/src/tx_thread_context_save.S
      ${CMAKE_CURRENT_LIST_DIR}/threadx/ports/cortex_m33/gnu/src/tx_thread_interrupt_control.S
      ${CMAKE_CURRENT_LIST_DIR}/threadx/ports/cortex_m33/gnu/src/tx_thread_interrupt_disable.S
      ${CMAKE_CURRENT_LIST_DIR}/threadx/ports/cortex_m33/gnu/src/tx_thread_interrupt_restore.S
      ${CMAKE_CURRENT_LIST_DIR}/threadx/ports/cortex_m33/gnu/src/tx_thread_schedule.S
      ${CMAKE_CURRENT_LIST_DIR}/threadx/ports/cortex_m33/gnu/src/tx_thread_stack_build.S
      ${CMAKE_CURRENT_LIST_DIR}/threadx/ports/cortex_m33/gnu/src/tx_thread_system_return.S
      ${CMAKE_CURRENT_LIST_DIR}/threadx/ports/cortex_m33/gnu/src/tx_timer_interrupt.S
  )
endif()

if(CONFIG_TOOLCHAIN STREQUAL iar AND CONFIG_CORE STREQUAL cm33)
  target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
      ${CMAKE_CURRENT_LIST_DIR}/threadx/ports/cortex_m33/iar/src/tx_iar.c
      ${CMAKE_CURRENT_LIST_DIR}/threadx/ports/cortex_m33/iar/src/tx_misra.s
      ${CMAKE_CURRENT_LIST_DIR}/threadx/ports/cortex_m33/iar/src/tx_thread_context_restore.s
      ${CMAKE_CURRENT_LIST_DIR}/threadx/ports/cortex_m33/iar/src/tx_thread_context_save.s
      ${CMAKE_CURRENT_LIST_DIR}/threadx/ports/cortex_m33/iar/src/tx_thread_interrupt_control.s
      ${CMAKE_CURRENT_LIST_DIR}/threadx/ports/cortex_m33/iar/src/tx_thread_interrupt_disable.s
      ${CMAKE_CURRENT_LIST_DIR}/threadx/ports/cortex_m33/iar/src/tx_thread_interrupt_restore.s
      ${CMAKE_CURRENT_LIST_DIR}/threadx/ports/cortex_m33/iar/src/tx_thread_schedule.s
      ${CMAKE_CURRENT_LIST_DIR}/threadx/ports/cortex_m33/iar/src/tx_thread_secure_stack.c
      ${CMAKE_CURRENT_LIST_DIR}/threadx/ports/cortex_m33/iar/src/tx_thread_secure_stack_allocate.s
      ${CMAKE_CURRENT_LIST_DIR}/threadx/ports/cortex_m33/iar/src/tx_thread_secure_stack_free.s
      ${CMAKE_CURRENT_LIST_DIR}/threadx/ports/cortex_m33/iar/src/tx_thread_secure_stack_initialize.s
      ${CMAKE_CURRENT_LIST_DIR}/threadx/ports/cortex_m33/iar/src/tx_thread_stack_build.s
      ${CMAKE_CURRENT_LIST_DIR}/threadx/ports/cortex_m33/iar/src/tx_thread_system_return.s
      ${CMAKE_CURRENT_LIST_DIR}/threadx/ports/cortex_m33/iar/src/tx_timer_interrupt.s
      ${CMAKE_CURRENT_LIST_DIR}/threadx/ports/cortex_m33/iar/src/txe_thread_secure_stack_allocate.c
      ${CMAKE_CURRENT_LIST_DIR}/threadx/ports/cortex_m33/iar/src/txe_thread_secure_stack_free.c
  )
endif()

if(CONFIG_TOOLCHAIN STREQUAL mdk AND CONFIG_CORE STREQUAL cm33)
  target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
      ${CMAKE_CURRENT_LIST_DIR}/threadx/ports/cortex_m33/ac6/src/tx_misra.S
      ${CMAKE_CURRENT_LIST_DIR}/threadx/ports/cortex_m33/ac6/src/tx_thread_context_restore.S
      ${CMAKE_CURRENT_LIST_DIR}/threadx/ports/cortex_m33/ac6/src/tx_thread_context_save.S
      ${CMAKE_CURRENT_LIST_DIR}/threadx/ports/cortex_m33/ac6/src/tx_thread_interrupt_control.S
      ${CMAKE_CURRENT_LIST_DIR}/threadx/ports/cortex_m33/ac6/src/tx_thread_interrupt_disable.S
      ${CMAKE_CURRENT_LIST_DIR}/threadx/ports/cortex_m33/ac6/src/tx_thread_interrupt_restore.S
      ${CMAKE_CURRENT_LIST_DIR}/threadx/ports/cortex_m33/ac6/src/tx_thread_schedule.S
      ${CMAKE_CURRENT_LIST_DIR}/threadx/ports/cortex_m33/ac6/src/tx_thread_secure_stack.c
      ${CMAKE_CURRENT_LIST_DIR}/threadx/ports/cortex_m33/ac6/src/tx_thread_secure_stack_allocate.S
      ${CMAKE_CURRENT_LIST_DIR}/threadx/ports/cortex_m33/ac6/src/tx_thread_secure_stack_free.S
      ${CMAKE_CURRENT_LIST_DIR}/threadx/ports/cortex_m33/ac6/src/tx_thread_secure_stack_initialize.S
      ${CMAKE_CURRENT_LIST_DIR}/threadx/ports/cortex_m33/ac6/src/tx_thread_stack_build.S
      ${CMAKE_CURRENT_LIST_DIR}/threadx/ports/cortex_m33/ac6/src/tx_thread_system_return.S
      ${CMAKE_CURRENT_LIST_DIR}/threadx/ports/cortex_m33/ac6/src/tx_timer_interrupt.S
      ${CMAKE_CURRENT_LIST_DIR}/threadx/ports/cortex_m33/ac6/src/txe_thread_secure_stack_allocate.c
      ${CMAKE_CURRENT_LIST_DIR}/threadx/ports/cortex_m33/ac6/src/txe_thread_secure_stack_free.c
  )
endif()

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/threadx/common/inc
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

if((CONFIG_TOOLCHAIN STREQUAL armgcc OR CONFIG_TOOLCHAIN STREQUAL mcux) AND CONFIG_CORE STREQUAL cm7f)
target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/threadx/ports/cortex_m7/gnu/inc
)
endif()

if((CONFIG_TOOLCHAIN STREQUAL armgcc OR CONFIG_TOOLCHAIN STREQUAL mcux) AND CONFIG_CORE STREQUAL cm4f)
target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/threadx/ports/cortex_m4/gnu/inc
)
endif()

if(CONFIG_TOOLCHAIN STREQUAL iar AND CONFIG_CORE STREQUAL cm33)
target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/threadx/ports/cortex_m33/iar/inc
)
endif()

if(CONFIG_TOOLCHAIN STREQUAL mdk AND CONFIG_CORE STREQUAL cm33)
target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/threadx/ports/cortex_m33/ac6/inc
)
endif()

if((CONFIG_TOOLCHAIN STREQUAL armgcc OR CONFIG_TOOLCHAIN STREQUAL mcux) AND CONFIG_CORE STREQUAL cm33)
target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/threadx/ports/cortex_m33/gnu/inc
)
endif()

if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

  target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
    -DTX_INCLUDE_USER_DEFINE_FILE
    -DFSL_RTOS_THREADX
  )

  if(CONFIG_TOOLCHAIN STREQUAL armgcc)
    target_compile_options(${MCUX_SDK_PROJECT_NAME} PUBLIC
    )
  endif()

endif()

