include_guard(GLOBAL)
message("device_MKE15Z7_CMSIS component is included.")


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/.
)

include(CMSIS_Include_core_cm_MKE15Z7)

