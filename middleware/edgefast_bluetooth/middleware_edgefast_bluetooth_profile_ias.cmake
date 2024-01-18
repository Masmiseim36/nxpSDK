# Add set(CONFIG_USE_middleware_edgefast_bluetooth_profile_ias true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_edgefast_bluetooth_pal)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/source/services/ias/ias.c
  ${CMAKE_CURRENT_LIST_DIR}/source/services/ias/ias_client.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/source/services
  ${CMAKE_CURRENT_LIST_DIR}/include
)

else()

message(SEND_ERROR "middleware_edgefast_bluetooth_profile_ias dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
