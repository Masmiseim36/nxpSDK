# Add set(CONFIG_USE_middleware_soem true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_baremetal OR CONFIG_USE_middleware_freertos-kernel)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/soem/ethercateoe.c
  ${CMAKE_CURRENT_LIST_DIR}/soem/ethercatconfig.c
  ${CMAKE_CURRENT_LIST_DIR}/soem/ethercatsoe.c
  ${CMAKE_CURRENT_LIST_DIR}/soem/ethercatcoe.c
  ${CMAKE_CURRENT_LIST_DIR}/soem/ethercatprint.c
  ${CMAKE_CURRENT_LIST_DIR}/soem/ethercatfoe.c
  ${CMAKE_CURRENT_LIST_DIR}/soem/ethercatmain.c
  ${CMAKE_CURRENT_LIST_DIR}/soem/ethercatdc.c
  ${CMAKE_CURRENT_LIST_DIR}/soem/ethercatbase.c
  ${CMAKE_CURRENT_LIST_DIR}/oshw/mcux-sdk/enet/enet.c
  ${CMAKE_CURRENT_LIST_DIR}/oshw/mcux-sdk/oshw.c
  ${CMAKE_CURRENT_LIST_DIR}/oshw/mcux-sdk/soem_port.c
  ${CMAKE_CURRENT_LIST_DIR}/oshw/mcux-sdk/nicdrv.c
)

if(CONFIG_USE_middleware_baremetal)
  target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
      ${CMAKE_CURRENT_LIST_DIR}/osal/baremetal/osal.c
  )
endif()

if(CONFIG_USE_middleware_freertos-kernel)
  target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
      ${CMAKE_CURRENT_LIST_DIR}/osal/freertos/osal.c
  )
endif()

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/soem
  ${CMAKE_CURRENT_LIST_DIR}/oshw/mcux-sdk
  ${CMAKE_CURRENT_LIST_DIR}/oshw/mcux-sdk/enet
  ${CMAKE_CURRENT_LIST_DIR}/osal
)

else()

message(SEND_ERROR "middleware_soem dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
