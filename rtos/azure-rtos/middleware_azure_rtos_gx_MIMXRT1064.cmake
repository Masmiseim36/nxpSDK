if(NOT MIDDLEWARE_AZURE_RTOS_GX_MIMXRT1064_INCLUDED)
    
    set(MIDDLEWARE_AZURE_RTOS_GX_MIMXRT1064_INCLUDED true CACHE BOOL "middleware_azure_rtos_gx component is included.")


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/guix/common/inc
        ${CMAKE_CURRENT_LIST_DIR}/guix/ports/cortex_m7/gnu/inc
    )

    include(middleware_azure_rtos_tx_MIMXRT1064)

endif()
