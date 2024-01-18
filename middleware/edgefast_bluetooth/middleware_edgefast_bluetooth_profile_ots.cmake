# Add set(CONFIG_USE_middleware_edgefast_bluetooth_profile_ots true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_edgefast_bluetooth_pal)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/source/services/ots/ots.c
  ${CMAKE_CURRENT_LIST_DIR}/source/services/ots/ots_client.c
  ${CMAKE_CURRENT_LIST_DIR}/source/services/ots/ots_dir_list.c
  ${CMAKE_CURRENT_LIST_DIR}/source/services/ots/ots_l2cap.c
  ${CMAKE_CURRENT_LIST_DIR}/source/services/ots/ots_oacp.c
  ${CMAKE_CURRENT_LIST_DIR}/source/services/ots/ots_obj_manager.c
  ${CMAKE_CURRENT_LIST_DIR}/source/services/ots/ots_olcp.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/source/services
  ${CMAKE_CURRENT_LIST_DIR}/source/services/ots
  ${CMAKE_CURRENT_LIST_DIR}/include/bluetooth/services
  ${CMAKE_CURRENT_LIST_DIR}/include/bluetooth/services/config
)

else()

message(SEND_ERROR "middleware_edgefast_bluetooth_profile_ots dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
