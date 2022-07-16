include_guard()
message("middleware_freertos-aws_iot_libraries_abstractions_transport component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/libraries/abstractions/transport/secure_sockets/transport_secure_sockets.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/libraries/abstractions/transport/secure_sockets
)


#OR Logic component
if(CONFIG_USE_middleware_freertos-aws_iot_libraries_coremqtt_MIMXRT1021)
     include(middleware_freertos-aws_iot_libraries_coremqtt_MIMXRT1021)
endif()
if(CONFIG_USE_middleware_freertos-aws_iot_libraries_corehttp_MIMXRT1021)
     include(middleware_freertos-aws_iot_libraries_corehttp_MIMXRT1021)
endif()
if(NOT (CONFIG_USE_middleware_freertos-aws_iot_libraries_coremqtt_MIMXRT1021 OR CONFIG_USE_middleware_freertos-aws_iot_libraries_corehttp_MIMXRT1021))
    message(WARNING "Since middleware_freertos-aws_iot_libraries_coremqtt_MIMXRT1021/middleware_freertos-aws_iot_libraries_corehttp_MIMXRT1021 is not included at first or config in config.cmake file, use middleware_freertos-aws_iot_libraries_coremqtt_MIMXRT1021 by default.")
    include(middleware_freertos-aws_iot_libraries_coremqtt_MIMXRT1021)
endif()

include(middleware_freertos-kernel_MIMXRT1021)

include(middleware_freertos-aws_iot_libraries_abstractions_secure_sockets_MIMXRT1021)

include(middleware_freertos-aws_iot_libraries_logging_MIMXRT1021)

