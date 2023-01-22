include_guard()
message("middleware_lwip_contrib_ping component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/contrib/apps/ping/ping.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/contrib/apps/ping
)


include(middleware_lwip_MIMXRT595S_cm33)

