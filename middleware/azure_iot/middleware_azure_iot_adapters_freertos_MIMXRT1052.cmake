if(NOT MIDDLEWARE_AZURE_IOT_ADAPTERS_FREERTOS_MIMXRT1052_INCLUDED)
    
    set(MIDDLEWARE_AZURE_IOT_ADAPTERS_FREERTOS_MIMXRT1052_INCLUDED true CACHE BOOL "middleware_azure_iot_adapters_freertos component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/c-utility/pal/freertos/tickcounter.c
        ${CMAKE_CURRENT_LIST_DIR}/c-utility/pal/freertos/threadapi.c
        ${CMAKE_CURRENT_LIST_DIR}/c-utility/pal/freertos/lock.c
    )


    include(middleware_azure_iot_MIMXRT1052)

    include(middleware_freertos-kernel_MIMXRT1052)

endif()
