include_guard()
message("middleware_lwip_apps_httpssrv component is included.")


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/src/apps/httpsrv
)

include(middleware_lwip_apps_httpsrv_MIMXRT595S_cm33)

include(middleware_mbedtls_MIMXRT595S_cm33)

