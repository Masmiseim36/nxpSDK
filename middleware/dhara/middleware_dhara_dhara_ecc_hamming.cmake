# Add set(CONFIG_USE_middleware_dhara_dhara_ecc_hamming true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_dhara)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/ecc/hamming.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/ecc
)

else()

message(SEND_ERROR "middleware_dhara_dhara_ecc_hamming dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
