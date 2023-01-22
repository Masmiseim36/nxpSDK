include_guard()
message("middleware_maestro_framework component is included.")


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/config
    ${CMAKE_CURRENT_LIST_DIR}/inc
)

include(middleware_maestro_framework_doc_MIMXRT1062)

include(middleware_maestro_framework_streamer_MIMXRT1062)

