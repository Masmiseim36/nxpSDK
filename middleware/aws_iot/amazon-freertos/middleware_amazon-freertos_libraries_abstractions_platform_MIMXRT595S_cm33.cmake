include_guard()
message("middleware_amazon-freertos_libraries_abstractions_platform component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/libraries/abstractions/platform/freertos/iot_clock_freertos.c
    ${CMAKE_CURRENT_LIST_DIR}/libraries/abstractions/platform/freertos/iot_threads_freertos.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/libraries/abstractions/platform/freertos/include/platform
    ${CMAKE_CURRENT_LIST_DIR}/libraries/abstractions/platform/include/platform
    ${CMAKE_CURRENT_LIST_DIR}/libraries/abstractions/platform/include/types
    ${CMAKE_CURRENT_LIST_DIR}/libraries/abstractions/platform/freertos/include
    ${CMAKE_CURRENT_LIST_DIR}/libraries/abstractions/platform/include
)


include(middleware_amazon-freertos_libraries_c_sdk_standard_common_MIMXRT595S_cm33)

include(middleware_amazon-freertos_libraries_abstractions_secure_sockets_MIMXRT595S_cm33)

include(middleware_freertos-kernel_MIMXRT595S_cm33)

