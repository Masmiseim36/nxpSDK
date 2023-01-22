include_guard()
message("middleware_eap component is included.")


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/Example_Application/CONFIG_HEADER
    ${CMAKE_CURRENT_LIST_DIR}/EAP_Includes
)

include(middleware_eap_exapp_MIMXRT1176_cm7)

