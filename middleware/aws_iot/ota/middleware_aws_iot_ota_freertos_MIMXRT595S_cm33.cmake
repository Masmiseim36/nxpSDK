include_guard()
message("middleware_aws_iot_ota_freertos component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/source/portable/os/ota_os_freertos.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/source/portable/os
)


include(middleware_freertos-kernel_MIMXRT595S_cm33)

