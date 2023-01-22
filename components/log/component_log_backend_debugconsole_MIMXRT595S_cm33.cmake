include_guard()
message("component_log_backend_debugconsole component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/fsl_component_log_backend_debugconsole.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/.
)


include(driver_common_MIMXRT595S_cm33)

include(component_log_MIMXRT595S_cm33)

include(utility_debug_console_MIMXRT595S_cm33)

