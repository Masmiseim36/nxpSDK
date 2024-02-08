# Add set(CONFIG_USE_component_ele_nvm_manager true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_component_ele_crypto)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/ele_nvm_manager.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/.
)

else()

message(SEND_ERROR "component_ele_nvm_manager dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
