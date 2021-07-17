include_guard(GLOBAL)
message("middleware_lwip_contrib_tcpecho_raw component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/contrib/apps/tcpecho_raw/tcpecho_raw.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/contrib/apps/tcpecho_raw
)


include(middleware_lwip_MIMXRT1176_cm7)

