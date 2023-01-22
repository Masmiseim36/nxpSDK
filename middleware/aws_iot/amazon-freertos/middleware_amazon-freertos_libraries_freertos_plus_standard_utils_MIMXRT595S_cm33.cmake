include_guard()
message("middleware_amazon-freertos_libraries_freertos_plus_standard_utils component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/libraries/freertos_plus/standard/utils/src/iot_system_init.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/libraries/freertos_plus/standard/utils/include
)


include(middleware_freertos-kernel_MIMXRT595S_cm33)

include(middleware_amazon-freertos_libraries_freertos_plus_standard_crypto_MIMXRT595S_cm33)

include(middleware_amazon-freertos_libraries_abstractions_secure_sockets_MIMXRT595S_cm33)

