include_guard()
message("middleware_amazon-freertos_libraries_c_sdk_standard_common_include component is included.")


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/libraries/c_sdk/standard/common/include
    ${CMAKE_CURRENT_LIST_DIR}/libraries/c_sdk/standard/common/include/private
    ${CMAKE_CURRENT_LIST_DIR}/libraries/c_sdk/standard/common/include/types
)

include(middleware_freertos-kernel_MIMXRT595S_cm33)

include(middleware_amazon-freertos_libraries_abstractions_platform_MIMXRT595S_cm33)

