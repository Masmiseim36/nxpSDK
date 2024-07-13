# Add set(CONFIG_USE_middleware_motor_control_pmsm_frac_tsa true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_fmstr AND (CONFIG_CORE STREQUAL cm0p OR CONFIG_CORE STREQUAL cm33 OR CONFIG_CORE STREQUAL dsp56800ex OR CONFIG_CORE STREQUAL dsp56800ef))

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/pmsm/pmsm_frac/fm_tsa_pmsm.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/pmsm/pmsm_frac
)

else()

message(SEND_ERROR "middleware_motor_control_pmsm_frac_tsa dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
