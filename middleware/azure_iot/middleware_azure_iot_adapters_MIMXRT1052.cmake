if(NOT MIDDLEWARE_AZURE_IOT_ADAPTERS_MIMXRT1052_INCLUDED)
    
    set(MIDDLEWARE_AZURE_IOT_ADAPTERS_MIMXRT1052_INCLUDED true CACHE BOOL "middleware_azure_iot_adapters component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/c-utility/adapters/agenttime_msdk.c
        ${CMAKE_CURRENT_LIST_DIR}/c-utility/adapters/platform_msdk.c
        ${CMAKE_CURRENT_LIST_DIR}/c-utility/adapters/socketio_berkeley_msdk.c
        ${CMAKE_CURRENT_LIST_DIR}/c-utility/adapters/uniqueid_stub.c
        ${CMAKE_CURRENT_LIST_DIR}/c-utility/adapters/httpapi_compact.c
        ${CMAKE_CURRENT_LIST_DIR}/c-utility/pal/lwip/sntp_lwip.c
        ${CMAKE_CURRENT_LIST_DIR}/c-utility/pal/tlsio_options.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/c-utility/pal/generic
        ${CMAKE_CURRENT_LIST_DIR}/c-utility/pal/inc
        ${CMAKE_CURRENT_LIST_DIR}/c-utility/pal/lwip
    )


    include(middleware_azure_iot_MIMXRT1052)

    include(middleware_lwip_MIMXRT1052)

    include(middleware_lwip_apps_sntp_MIMXRT1052)

endif()
