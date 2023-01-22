include_guard()
message("middleware_freertos-kernel_mpu_wrappers component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/portable/Common/mpu_wrappers.c
)


include(middleware_freertos-kernel_MIMXRT685S_cm33)

