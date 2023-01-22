include_guard()
message("middleware_amazon-freertos_vendor_nxp_wifi component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/vendors/nxp/wifi/nxp/iot_wifi.c
)


include(middleware_amazon-freertos_libraries_abstractions_wifi_MIMXRT595S_cm33)

include(middleware_wifi_MIMXRT595S_cm33)

