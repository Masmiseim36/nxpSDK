include_guard(GLOBAL)
message("driver_memory component is included.")


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/.
)

include(driver_common_MIMXRT1176_cm4)

