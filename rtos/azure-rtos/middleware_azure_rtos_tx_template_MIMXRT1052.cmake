if(NOT MIDDLEWARE_AZURE_RTOS_TX_TEMPLATE_MIMXRT1052_INCLUDED)
    
    set(MIDDLEWARE_AZURE_RTOS_TX_TEMPLATE_MIMXRT1052_INCLUDED true CACHE BOOL "middleware_azure_rtos_tx_template component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/threadx/ports/cortex_m7/gnu/src/tx_initialize_low_level.S
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/threadx/common/inc
        ${CMAKE_CURRENT_LIST_DIR}/threadx/ports/cortex_m7/gnu/inc
    )


endif()
