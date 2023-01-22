include_guard()
message("middleware_rtcesl_cm7f component is included.")


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/CM7F_RTCESL_4.7.1_MCUX/AMCLIB/Include
    ${CMAKE_CURRENT_LIST_DIR}/CM7F_RTCESL_4.7.1_MCUX/GDFLIB/Include
    ${CMAKE_CURRENT_LIST_DIR}/CM7F_RTCESL_4.7.1_MCUX/GFLIB/Include
    ${CMAKE_CURRENT_LIST_DIR}/CM7F_RTCESL_4.7.1_MCUX/GMCLIB/Include
    ${CMAKE_CURRENT_LIST_DIR}/CM7F_RTCESL_4.7.1_MCUX/MLIB/Include
)

