include_guard()
message("middleware_vglite_elementary component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/elementary/src/elm_buffer.c
    ${CMAKE_CURRENT_LIST_DIR}/elementary/src/elm_draw.c
    ${CMAKE_CURRENT_LIST_DIR}/elementary/src/elm_init.c
    ${CMAKE_CURRENT_LIST_DIR}/elementary/src/elm_object.c
    ${CMAKE_CURRENT_LIST_DIR}/elementary/src/elm_os.c
    ${CMAKE_CURRENT_LIST_DIR}/elementary/src/elm_text.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/elementary/inc
    ${CMAKE_CURRENT_LIST_DIR}/elementary/src
)


