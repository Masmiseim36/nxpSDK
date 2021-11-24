include_guard(GLOBAL)
message("middleware_ntag_i2c_plus_lpc component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/HAL_NTAG/ntag_bridge.c
    ${CMAKE_CURRENT_LIST_DIR}/HAL_NTAG/ntag_driver.c
    ${CMAKE_CURRENT_LIST_DIR}/HAL_I2C/i2c_lpc_fsl.c
    ${CMAKE_CURRENT_LIST_DIR}/HAL_ISR/isr_common.c
    ${CMAKE_CURRENT_LIST_DIR}/HAL_ISR/isr_lpc.c
    ${CMAKE_CURRENT_LIST_DIR}/HAL_TMR/timer_driver.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/HAL_NTAG/inc
    ${CMAKE_CURRENT_LIST_DIR}/HAL_NTAG
    ${CMAKE_CURRENT_LIST_DIR}/HAL_I2C/inc
    ${CMAKE_CURRENT_LIST_DIR}/HAL_ISR/inc
    ${CMAKE_CURRENT_LIST_DIR}/HAL_ISR
    ${CMAKE_CURRENT_LIST_DIR}/HAL_TMR/inc
    ${CMAKE_CURRENT_LIST_DIR}/inc
)


