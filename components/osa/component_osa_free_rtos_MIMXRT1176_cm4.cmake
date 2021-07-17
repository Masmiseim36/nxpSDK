include_guard(GLOBAL)
message("component_osa_free_rtos component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/fsl_os_abstraction_free_rtos.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/.
)


include(middleware_freertos-kernel_MIMXRT1176_cm4)

include(driver_common_MIMXRT1176_cm4)

include(component_lists_MIMXRT1176_cm4)

