include_guard()
message("middleware_mmcau_cm4_cm7 component is included.")


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/.
)

include(driver_clock_MIMXRT1176_cm4)

