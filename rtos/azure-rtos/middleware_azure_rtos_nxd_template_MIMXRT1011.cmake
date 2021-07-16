include_guard(GLOBAL)
message("middleware_azure_rtos_nxd_template component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/netxduo/addons/auto_ip/nx_auto_ip.c
    ${CMAKE_CURRENT_LIST_DIR}/netxduo/addons/cloud/nx_cloud.c
    ${CMAKE_CURRENT_LIST_DIR}/netxduo/addons/dhcp/nxd_dhcp_client.c
    ${CMAKE_CURRENT_LIST_DIR}/netxduo/addons/dhcp/nxd_dhcp_server.c
    ${CMAKE_CURRENT_LIST_DIR}/netxduo/addons/dhcp/nxd_dhcpv6_client.c
    ${CMAKE_CURRENT_LIST_DIR}/netxduo/addons/dhcp/nxd_dhcpv6_server.c
    ${CMAKE_CURRENT_LIST_DIR}/netxduo/addons/dns/nxd_dns.c
    ${CMAKE_CURRENT_LIST_DIR}/netxduo/addons/ftp/nxd_ftp_client.c
    ${CMAKE_CURRENT_LIST_DIR}/netxduo/addons/ftp/nxd_ftp_server.c
    ${CMAKE_CURRENT_LIST_DIR}/netxduo/addons/http/nxd_http_client.c
    ${CMAKE_CURRENT_LIST_DIR}/netxduo/addons/http/nxd_http_server.c
    ${CMAKE_CURRENT_LIST_DIR}/netxduo/addons/mdns/nxd_mdns.c
    ${CMAKE_CURRENT_LIST_DIR}/netxduo/addons/mqtt/nxd_mqtt_client.c
    ${CMAKE_CURRENT_LIST_DIR}/netxduo/addons/nat/nx_nat.c
    ${CMAKE_CURRENT_LIST_DIR}/netxduo/addons/pop3/nxd_pop3_client.c
    ${CMAKE_CURRENT_LIST_DIR}/netxduo/addons/ppp/nx_ppp.c
    ${CMAKE_CURRENT_LIST_DIR}/netxduo/addons/pppoe/nx_pppoe_client.c
    ${CMAKE_CURRENT_LIST_DIR}/netxduo/addons/pppoe/nx_pppoe_server.c
    ${CMAKE_CURRENT_LIST_DIR}/netxduo/addons/smtp/nxd_smtp_client.c
    ${CMAKE_CURRENT_LIST_DIR}/netxduo/addons/snmp/nx_des.c
    ${CMAKE_CURRENT_LIST_DIR}/netxduo/addons/snmp/nx_sha1.c
    ${CMAKE_CURRENT_LIST_DIR}/netxduo/addons/snmp/nxd_snmp.c
    ${CMAKE_CURRENT_LIST_DIR}/netxduo/addons/sntp/nxd_sntp_client.c
    ${CMAKE_CURRENT_LIST_DIR}/netxduo/addons/telnet/nxd_telnet_client.c
    ${CMAKE_CURRENT_LIST_DIR}/netxduo/addons/telnet/nxd_telnet_server.c
    ${CMAKE_CURRENT_LIST_DIR}/netxduo/addons/tftp/nxd_tftp_client.c
    ${CMAKE_CURRENT_LIST_DIR}/netxduo/addons/tftp/nxd_tftp_server.c
    ${CMAKE_CURRENT_LIST_DIR}/netxduo/addons/web/nx_tcpserver.c
    ${CMAKE_CURRENT_LIST_DIR}/netxduo/addons/web/nx_web_http_client.c
    ${CMAKE_CURRENT_LIST_DIR}/netxduo/addons/web/nx_web_http_server.c
)


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


