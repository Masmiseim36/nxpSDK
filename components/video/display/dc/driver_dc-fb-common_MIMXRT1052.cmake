include_guard()
message("driver_dc-fb-common component is included.")


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/.
)

include(driver_video-common_MIMXRT1052)

