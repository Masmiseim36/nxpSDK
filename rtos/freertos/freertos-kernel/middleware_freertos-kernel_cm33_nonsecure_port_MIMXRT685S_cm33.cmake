include_guard()
message("middleware_freertos-kernel_cm33_nonsecure_port component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/portable/GCC/ARM_CM33_NTZ/non_secure/port.c
    ${CMAKE_CURRENT_LIST_DIR}/portable/GCC/ARM_CM33_NTZ/non_secure/portasm.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/portable/GCC/ARM_CM33_NTZ/non_secure
)


include(middleware_freertos-kernel_MIMXRT685S_cm33)

