include_guard(GLOBAL)
message("component_nvm_adapter component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/nvm_adapter.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/.
)


include(driver_common_MIMXRT1176_cm4)

include(middleware_wireless_framework_function_lib_MIMXRT1176_cm4)

include(middleware_wireless_framework_NVM_MIMXRT1176_cm4)

include(middleware_wireless_framework_Common_MIMXRT1176_cm4)

