include_guard(GLOBAL)
message("middleware_azure_iot_iothub_client component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/iothub_client/src/iothub_client_retry_control.c
    ${CMAKE_CURRENT_LIST_DIR}/iothub_client/src/iothub_transport_ll_private.c
    ${CMAKE_CURRENT_LIST_DIR}/iothub_client/src/blob.c
    ${CMAKE_CURRENT_LIST_DIR}/iothub_client/src/iothub.c
    ${CMAKE_CURRENT_LIST_DIR}/iothub_client/src/iothub_client.c
    ${CMAKE_CURRENT_LIST_DIR}/iothub_client/src/iothub_client_authorization.c
    ${CMAKE_CURRENT_LIST_DIR}/iothub_client/src/iothub_client_core.c
    ${CMAKE_CURRENT_LIST_DIR}/iothub_client/src/iothub_client_core_ll.c
    ${CMAKE_CURRENT_LIST_DIR}/iothub_client/src/iothub_client_diagnostic.c
    ${CMAKE_CURRENT_LIST_DIR}/iothub_client/src/iothub_client_ll.c
    ${CMAKE_CURRENT_LIST_DIR}/iothub_client/src/iothub_client_ll_uploadtoblob.c
    ${CMAKE_CURRENT_LIST_DIR}/iothub_client/src/iothub_message.c
    ${CMAKE_CURRENT_LIST_DIR}/iothub_client/src/iothubtransport.c
    ${CMAKE_CURRENT_LIST_DIR}/iothub_client/src/message_queue.c
    ${CMAKE_CURRENT_LIST_DIR}/iothub_client/src/version_azure.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/iothub_client/inc/internal
    ${CMAKE_CURRENT_LIST_DIR}/iothub_client/inc
)


include(middleware_azure_iot_MIMXRT1052)

