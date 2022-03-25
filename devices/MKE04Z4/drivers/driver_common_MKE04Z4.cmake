include_guard(GLOBAL)
message("driver_common component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/fsl_common.c
    ${CMAKE_CURRENT_LIST_DIR}/fsl_common_arm.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/.
)


include(driver_clock_MKE04Z4)

include(device_MKE04Z4_CMSIS_MKE04Z4)

