include_guard(GLOBAL)
message("middleware_azure_iot_adapters_freertos component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/c-utility/pal/freertos/tickcounter.c
    ${CMAKE_CURRENT_LIST_DIR}/c-utility/pal/freertos/threadapi.c
    ${CMAKE_CURRENT_LIST_DIR}/c-utility/pal/freertos/lock.c
)


include(middleware_azure_iot_MIMXRT1052)

include(middleware_freertos-kernel_MIMXRT1052)

