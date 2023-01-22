include_guard()
message("middleware_lwip_apps_httpsrv component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/src/apps/httpsrv/httpsrv.c
    ${CMAKE_CURRENT_LIST_DIR}/src/apps/httpsrv/httpsrv_base64.c
    ${CMAKE_CURRENT_LIST_DIR}/src/apps/httpsrv/httpsrv_fs.c
    ${CMAKE_CURRENT_LIST_DIR}/src/apps/httpsrv/httpsrv_script.c
    ${CMAKE_CURRENT_LIST_DIR}/src/apps/httpsrv/httpsrv_sha1.c
    ${CMAKE_CURRENT_LIST_DIR}/src/apps/httpsrv/httpsrv_supp.c
    ${CMAKE_CURRENT_LIST_DIR}/src/apps/httpsrv/httpsrv_task.c
    ${CMAKE_CURRENT_LIST_DIR}/src/apps/httpsrv/httpsrv_tls.c
    ${CMAKE_CURRENT_LIST_DIR}/src/apps/httpsrv/httpsrv_utf8.c
    ${CMAKE_CURRENT_LIST_DIR}/src/apps/httpsrv/httpsrv_ws.c
    ${CMAKE_CURRENT_LIST_DIR}/src/apps/httpsrv/httpsrv_ws_api.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/src/apps/httpsrv
)


include(middleware_lwip_MIMXRT595S_cm33)

include(middleware_freertos-kernel_MIMXRT595S_cm33)

