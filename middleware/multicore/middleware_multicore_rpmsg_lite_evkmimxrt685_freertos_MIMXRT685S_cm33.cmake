include_guard()
message("middleware_multicore_rpmsg_lite_evkmimxrt685_freertos component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/rpmsg_lite/lib/rpmsg_lite/porting/platform/imxrt600_m33/rpmsg_platform.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/rpmsg_lite/lib/include/platform/imxrt600_m33
)


include(middleware_freertos-kernel_MIMXRT685S_cm33)

include(middleware_freertos-kernel_heap_4_MIMXRT685S_cm33)

