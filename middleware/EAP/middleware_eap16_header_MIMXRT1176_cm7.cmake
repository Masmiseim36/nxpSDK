include_guard()
message("middleware_eap16_header component is included.")


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/EAP_Includes
)

include(middleware_eap_MIMXRT1176_cm7)

