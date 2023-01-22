include_guard()
message("middleware_amazon-freertos_libraries_abstractions_wifi component is included.")


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/libraries/abstractions/wifi/include
)

include(middleware_freertos-kernel_MIMXRT595S_cm33)

