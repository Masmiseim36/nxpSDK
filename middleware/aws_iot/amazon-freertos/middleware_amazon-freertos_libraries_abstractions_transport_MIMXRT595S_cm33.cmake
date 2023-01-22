include_guard()
message("middleware_amazon-freertos_libraries_abstractions_transport component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/libraries/abstractions/transport/secure_sockets/transport_secure_sockets.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/libraries/abstractions/transport/secure_sockets
)


#OR Logic component
if(CONFIG_USE_middleware_freertos_coremqtt_MIMXRT595S_cm33)
     include(middleware_freertos_coremqtt_MIMXRT595S_cm33)
endif()
if(CONFIG_USE_middleware_freertos_corehttp_MIMXRT595S_cm33)
     include(middleware_freertos_corehttp_MIMXRT595S_cm33)
endif()
if(NOT (CONFIG_USE_middleware_freertos_coremqtt_MIMXRT595S_cm33 OR CONFIG_USE_middleware_freertos_corehttp_MIMXRT595S_cm33))
    message(WARNING "Since middleware_freertos_coremqtt_MIMXRT595S_cm33/middleware_freertos_corehttp_MIMXRT595S_cm33 is not included at first or config in config.cmake file, use middleware_freertos_coremqtt_MIMXRT595S_cm33 by default.")
    include(middleware_freertos_coremqtt_MIMXRT595S_cm33)
endif()

include(middleware_freertos-kernel_MIMXRT595S_cm33)

include(middleware_amazon-freertos_libraries_abstractions_secure_sockets_MIMXRT595S_cm33)

include(middleware_amazon-freertos_libraries_logging_MIMXRT595S_cm33)

