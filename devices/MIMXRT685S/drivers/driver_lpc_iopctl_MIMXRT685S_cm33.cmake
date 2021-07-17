include_guard(GLOBAL)
message("driver_lpc_iopctl component is included.")


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/.
)

include(driver_common_MIMXRT685S_cm33)

