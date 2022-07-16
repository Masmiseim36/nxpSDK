include_guard()
message("middleware_edgefast_wifi_nxp component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/source/wpl_nxp.c
)


include(middleware_edgefast_wifi_MIMXRT595S_cm33)

include(middleware_wifi_MIMXRT595S_cm33)

include(middleware_freertos-kernel_MIMXRT595S_cm33)

