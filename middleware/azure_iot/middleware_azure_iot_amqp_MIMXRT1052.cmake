if(NOT MIDDLEWARE_AZURE_IOT_AMQP_MIMXRT1052_INCLUDED)
    
    set(MIDDLEWARE_AZURE_IOT_AMQP_MIMXRT1052_INCLUDED true CACHE BOOL "middleware_azure_iot_amqp component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/uamqp/src/amqpvalue.c
        ${CMAKE_CURRENT_LIST_DIR}/uamqp/src/amqpvalue_to_string.c
        ${CMAKE_CURRENT_LIST_DIR}/uamqp/src/amqp_definitions.c
        ${CMAKE_CURRENT_LIST_DIR}/uamqp/src/amqp_frame_codec.c
        ${CMAKE_CURRENT_LIST_DIR}/uamqp/src/amqp_management.c
        ${CMAKE_CURRENT_LIST_DIR}/uamqp/src/async_operation.c
        ${CMAKE_CURRENT_LIST_DIR}/uamqp/src/cbs.c
        ${CMAKE_CURRENT_LIST_DIR}/uamqp/src/connection.c
        ${CMAKE_CURRENT_LIST_DIR}/uamqp/src/frame_codec.c
        ${CMAKE_CURRENT_LIST_DIR}/uamqp/src/header_detect_io.c
        ${CMAKE_CURRENT_LIST_DIR}/uamqp/src/link.c
        ${CMAKE_CURRENT_LIST_DIR}/uamqp/src/message.c
        ${CMAKE_CURRENT_LIST_DIR}/uamqp/src/message_receiver.c
        ${CMAKE_CURRENT_LIST_DIR}/uamqp/src/message_sender.c
        ${CMAKE_CURRENT_LIST_DIR}/uamqp/src/messaging.c
        ${CMAKE_CURRENT_LIST_DIR}/uamqp/src/saslclientio.c
        ${CMAKE_CURRENT_LIST_DIR}/uamqp/src/sasl_anonymous.c
        ${CMAKE_CURRENT_LIST_DIR}/uamqp/src/sasl_frame_codec.c
        ${CMAKE_CURRENT_LIST_DIR}/uamqp/src/sasl_mechanism.c
        ${CMAKE_CURRENT_LIST_DIR}/uamqp/src/sasl_mssbcbs.c
        ${CMAKE_CURRENT_LIST_DIR}/uamqp/src/sasl_plain.c
        ${CMAKE_CURRENT_LIST_DIR}/uamqp/src/sasl_server_mechanism.c
        ${CMAKE_CURRENT_LIST_DIR}/uamqp/src/session.c
        ${CMAKE_CURRENT_LIST_DIR}/uamqp/src/socket_listener_berkeley.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/uamqp/inc
    )


    include(middleware_azure_iot_MIMXRT1052)

endif()
