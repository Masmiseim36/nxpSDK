include_guard(GLOBAL)
message("middleware_lwip_contrib_tcpecho component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/contrib/apps/tcpecho/tcpecho.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/contrib/apps/tcpecho
)


include(middleware_lwip_MIMXRT1176_cm7)

