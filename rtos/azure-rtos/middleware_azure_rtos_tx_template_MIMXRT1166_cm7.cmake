if(NOT MIDDLEWARE_AZURE_RTOS_TX_TEMPLATE_MIMXRT1166_cm7_INCLUDED)
    
    set(MIDDLEWARE_AZURE_RTOS_TX_TEMPLATE_MIMXRT1166_cm7_INCLUDED true CACHE BOOL "middleware_azure_rtos_tx_template component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/threadx/ports/cortex_m7/gnu/src/tx_thread_schedule.S
        ${CMAKE_CURRENT_LIST_DIR}/threadx/ports/cortex_m7/gnu/src/tx_initialize_low_level.S
        ${CMAKE_CURRENT_LIST_DIR}/threadx/ports/cortex_m7/gnu/src/tx_thread_stack_build.S
        ${CMAKE_CURRENT_LIST_DIR}/threadx/ports/cortex_m7/gnu/src/tx_thread_system_return.S
        ${CMAKE_CURRENT_LIST_DIR}/threadx/ports/cortex_m7/gnu/src/tx_timer_interrupt.S
        ${CMAKE_CURRENT_LIST_DIR}/threadx/ports/cortex_m7/gnu/src/tx_thread_interrupt_control.S
        ${CMAKE_CURRENT_LIST_DIR}/threadx/ports/cortex_m7/gnu/src/tx_thread_context_restore.S
        ${CMAKE_CURRENT_LIST_DIR}/threadx/ports/cortex_m7/gnu/src/tx_thread_context_save.S
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/threadx/common/inc
        ${CMAKE_CURRENT_LIST_DIR}/threadx/ports/cortex_m7/gnu/inc
    )


endif()
