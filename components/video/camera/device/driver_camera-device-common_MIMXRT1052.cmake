include_guard()
message("driver_camera-device-common component is included.")


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/.
)

include(driver_camera-common_MIMXRT1052)

include(driver_common_MIMXRT1052)

