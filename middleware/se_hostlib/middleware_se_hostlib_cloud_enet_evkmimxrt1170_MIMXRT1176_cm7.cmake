include_guard(GLOBAL)
message("middleware_se_hostlib_cloud_enet_evkmimxrt1170 component is included.")


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/demos/ksdk/common/freertos/boards/evkmimxrt1170
)

include(driver_enet_MIMXRT1176_cm7)

include(middleware_freertos-aws_iot_vendor_nxp_secure_sockets_lwip_MIMXRT1176_cm7)

include(middleware_lwip_MIMXRT1176_cm7)

