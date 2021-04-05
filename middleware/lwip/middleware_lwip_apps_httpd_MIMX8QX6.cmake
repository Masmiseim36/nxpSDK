if(NOT MIDDLEWARE_LWIP_APPS_HTTPD_MIMX8QX6_INCLUDED)
    
    set(MIDDLEWARE_LWIP_APPS_HTTPD_MIMX8QX6_INCLUDED true CACHE BOOL "middleware_lwip_apps_httpd component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/src/apps/http/httpd.c
    )


    include(middleware_lwip_MIMX8QX6)

    include(middleware_lwip_apps_httpd_support_MIMX8QX6)

endif()
