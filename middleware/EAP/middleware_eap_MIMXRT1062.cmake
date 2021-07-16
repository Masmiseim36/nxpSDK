include_guard(GLOBAL)
message("middleware_eap component is included.")


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/Example_Application/CONFIG_HEADER
    ${CMAKE_CURRENT_LIST_DIR}/EAP_Includes
)

