# Add set(CONFIG_USE_device_MIMXRT1189_system true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_device_MIMXRT1189_CMSIS)

if(CONFIG_CORE STREQUAL cm33)
  target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
      ${CMAKE_CURRENT_LIST_DIR}/system_MIMXRT1189_cm33.c
  )
endif()

if(CONFIG_CORE STREQUAL cm7f)
  target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
      ${CMAKE_CURRENT_LIST_DIR}/system_MIMXRT1189_cm7.c
  )
endif()

if(CONFIG_CORE STREQUAL cm33)
target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/.
)
endif()

if(CONFIG_CORE STREQUAL cm7f)
target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/.
)
endif()

else()

message(SEND_ERROR "device_MIMXRT1189_system.MIMXRT1189 dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
