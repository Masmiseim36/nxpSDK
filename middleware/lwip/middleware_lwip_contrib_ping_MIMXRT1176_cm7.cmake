include_guard(GLOBAL)
message("middleware_lwip_contrib_ping component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/contrib/apps/ping/ping.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/contrib/apps/ping
)


include(middleware_lwip_MIMXRT1176_cm7)

