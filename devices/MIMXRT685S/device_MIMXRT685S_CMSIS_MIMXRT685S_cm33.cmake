include_guard()
message("device_MIMXRT685S_CMSIS component is included.")


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/.
)

include(CMSIS_Include_core_cm_MIMXRT685S_cm33)

