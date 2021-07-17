include_guard(GLOBAL)
message("middleware_lwip_apps_mdns component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/src/apps/mdns/mdns.c
    ${CMAKE_CURRENT_LIST_DIR}/src/apps/mdns/mdns_domain.c
    ${CMAKE_CURRENT_LIST_DIR}/src/apps/mdns/mdns_out.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/src/include/lwip/apps
)


include(middleware_lwip_MIMXRT1176_cm4)

