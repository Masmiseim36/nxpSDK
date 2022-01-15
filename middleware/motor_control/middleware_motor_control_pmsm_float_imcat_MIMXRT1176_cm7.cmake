include_guard(GLOBAL)
message("middleware_motor_control_pmsm_float_imcat component is included.")


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/freemaster/pmsm_float/mcat
)

