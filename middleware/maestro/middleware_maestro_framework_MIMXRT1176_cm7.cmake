include_guard(GLOBAL)
message("middleware_maestro_framework component is included.")


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/inc
    ${CMAKE_CURRENT_LIST_DIR}/mcu-libs/osa
    ${CMAKE_CURRENT_LIST_DIR}/mcu-libs/osa/freertos
    ${CMAKE_CURRENT_LIST_DIR}/mcu-libs/common
    ${CMAKE_CURRENT_LIST_DIR}/mcu-audio/opus/include
    ${CMAKE_CURRENT_LIST_DIR}/streamer/inc
    ${CMAKE_CURRENT_LIST_DIR}/streamer/encoders/cei
)

include(middleware_maestro_framework_doc_MIMXRT1176_cm7)

