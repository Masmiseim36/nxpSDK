include_guard()
message("middleware_mcu-boot_MIMXRT1064_drivers component is included.")


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/targets/MIMXRT1064/src
)

