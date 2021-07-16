include_guard(GLOBAL)
message("middleware_nghttp2 component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/lib/nghttp2_buf.c
    ${CMAKE_CURRENT_LIST_DIR}/lib/nghttp2_callbacks.c
    ${CMAKE_CURRENT_LIST_DIR}/lib/nghttp2_debug.c
    ${CMAKE_CURRENT_LIST_DIR}/lib/nghttp2_frame.c
    ${CMAKE_CURRENT_LIST_DIR}/lib/nghttp2_hd.c
    ${CMAKE_CURRENT_LIST_DIR}/lib/nghttp2_hd_huffman.c
    ${CMAKE_CURRENT_LIST_DIR}/lib/nghttp2_hd_huffman_data.c
    ${CMAKE_CURRENT_LIST_DIR}/lib/nghttp2_helper.c
    ${CMAKE_CURRENT_LIST_DIR}/lib/nghttp2_http.c
    ${CMAKE_CURRENT_LIST_DIR}/lib/nghttp2_map.c
    ${CMAKE_CURRENT_LIST_DIR}/lib/nghttp2_mem.c
    ${CMAKE_CURRENT_LIST_DIR}/lib/nghttp2_npn.c
    ${CMAKE_CURRENT_LIST_DIR}/lib/nghttp2_option.c
    ${CMAKE_CURRENT_LIST_DIR}/lib/nghttp2_outbound_item.c
    ${CMAKE_CURRENT_LIST_DIR}/lib/nghttp2_pq.c
    ${CMAKE_CURRENT_LIST_DIR}/lib/nghttp2_priority_spec.c
    ${CMAKE_CURRENT_LIST_DIR}/lib/nghttp2_queue.c
    ${CMAKE_CURRENT_LIST_DIR}/lib/nghttp2_rcbuf.c
    ${CMAKE_CURRENT_LIST_DIR}/lib/nghttp2_session.c
    ${CMAKE_CURRENT_LIST_DIR}/lib/nghttp2_stream.c
    ${CMAKE_CURRENT_LIST_DIR}/lib/nghttp2_submit.c
    ${CMAKE_CURRENT_LIST_DIR}/lib/nghttp2_version.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/lib/includes/nghttp2
    ${CMAKE_CURRENT_LIST_DIR}/lib
    ${CMAKE_CURRENT_LIST_DIR}/lib/includes
)


include(middleware_mbedtls_MIMXRT1052)

