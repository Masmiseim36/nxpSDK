include_guard()
message("device_MIMXRT1166_CMSIS component is included.")


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/.
)

include(CMSIS_Include_core_cm_MIMXRT1166_cm7)

