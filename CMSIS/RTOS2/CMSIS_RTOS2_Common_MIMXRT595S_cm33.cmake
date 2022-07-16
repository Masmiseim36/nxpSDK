include_guard()
message("CMSIS_RTOS2_Common component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/RTX/Source/rtx_lib.c
    ${CMAKE_CURRENT_LIST_DIR}/RTX/Config/RTX_Config.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/RTX/Source
    ${CMAKE_CURRENT_LIST_DIR}/RTX/Include
    ${CMAKE_CURRENT_LIST_DIR}/RTX/Config
)


include(CMSIS_Device_API_OSTick_MIMXRT595S_cm33)

include(CMSIS_Device_API_RTOS2_MIMXRT595S_cm33)

