include_guard()
message("CMSIS_Driver_Include_USART component is included.")


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/.
)

include(CMSIS_Driver_Include_Common_MIMXRT1052)

