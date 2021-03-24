if(NOT MIDDLEWARE_AZURE_RTOS_NXD_TEMPLATE_MIMXRT1052_INCLUDED)
    
    set(MIDDLEWARE_AZURE_RTOS_NXD_TEMPLATE_MIMXRT1052_INCLUDED true CACHE BOOL "middleware_azure_rtos_nxd_template component is included.")


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/netxduo/addons/auto_ip
        ${CMAKE_CURRENT_LIST_DIR}/netxduo/addons/cloud
        ${CMAKE_CURRENT_LIST_DIR}/netxduo/addons/BSD
        ${CMAKE_CURRENT_LIST_DIR}/netxduo/addons/dhcp
        ${CMAKE_CURRENT_LIST_DIR}/netxduo/addons/dns
        ${CMAKE_CURRENT_LIST_DIR}/netxduo/addons/ftp
        ${CMAKE_CURRENT_LIST_DIR}/netxduo/addons/http
        ${CMAKE_CURRENT_LIST_DIR}/netxduo/addons/mdns
        ${CMAKE_CURRENT_LIST_DIR}/netxduo/addons/mqtt
        ${CMAKE_CURRENT_LIST_DIR}/netxduo/addons/nat
        ${CMAKE_CURRENT_LIST_DIR}/netxduo/addons/pop3
        ${CMAKE_CURRENT_LIST_DIR}/netxduo/addons/ppp
        ${CMAKE_CURRENT_LIST_DIR}/netxduo/addons/pppoe
        ${CMAKE_CURRENT_LIST_DIR}/netxduo/addons/smtp
        ${CMAKE_CURRENT_LIST_DIR}/netxduo/addons/snmp
        ${CMAKE_CURRENT_LIST_DIR}/netxduo/addons/sntp
        ${CMAKE_CURRENT_LIST_DIR}/netxduo/addons/telnet
        ${CMAKE_CURRENT_LIST_DIR}/netxduo/addons/tftp
        ${CMAKE_CURRENT_LIST_DIR}/netxduo/addons/web
        ${CMAKE_CURRENT_LIST_DIR}/netxduo/common/inc
        ${CMAKE_CURRENT_LIST_DIR}/netxduo/crypto_libraries/inc
        ${CMAKE_CURRENT_LIST_DIR}/netxduo/nx_secure/inc
        ${CMAKE_CURRENT_LIST_DIR}/netxduo/nx_secure/ports
        ${CMAKE_CURRENT_LIST_DIR}/netxduo/ports/cortex_m7/gnu/inc
    )

endif()
