include_guard(GLOBAL)
message("component_messaging component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/fsl_component_messaging.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/.
)


include(driver_common_MIMXRT685S_cm33)

include(component_lists_MIMXRT685S_cm33)

include(component_mem_manager_MIMXRT685S_cm33)

