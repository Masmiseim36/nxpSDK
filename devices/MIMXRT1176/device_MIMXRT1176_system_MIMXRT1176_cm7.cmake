include_guard(GLOBAL)
message("device_MIMXRT1176_system component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/system_MIMXRT1176_cm7.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/.
)


include(device_MIMXRT1176_CMSIS_MIMXRT1176_cm7)

