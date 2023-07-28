# Add set(CONFIG_USE_CMSIS_RTOS2_Secure true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_CMSIS_Device_API_OSTick AND CONFIG_USE_CMSIS_Device_API_RTOS2)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/RTX/Source/rtx_lib.c
  ${CMAKE_CURRENT_LIST_DIR}/RTX/Config/RTX_Config.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/RTX/Source
  ${CMAKE_CURRENT_LIST_DIR}/RTX/Include
  ${CMAKE_CURRENT_LIST_DIR}/RTX/Config
)

if(CONFIG_TOOLCHAIN STREQUAL iar AND CONFIG_CORE STREQUAL cm0p)
  target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
      ${CMAKE_CURRENT_LIST_DIR}/RTX/Library/IAR/RTX_CM0.a
      -Wl,--end-group
  )
endif()

if(CONFIG_TOOLCHAIN STREQUAL iar AND (CONFIG_CORE STREQUAL cm4f OR CONFIG_CORE STREQUAL cm7f))
  target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
      ${CMAKE_CURRENT_LIST_DIR}/RTX/Library/IAR/RTX_CM4F.a
      -Wl,--end-group
  )
endif()

if((CONFIG_TOOLCHAIN STREQUAL armgcc OR CONFIG_TOOLCHAIN STREQUAL mcux) AND CONFIG_CORE STREQUAL cm0p)
  target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
      ${CMAKE_CURRENT_LIST_DIR}/RTX/Library/GCC/libRTX_CM0.a
      -Wl,--end-group
  )
endif()

if((CONFIG_TOOLCHAIN STREQUAL armgcc OR CONFIG_TOOLCHAIN STREQUAL mcux) AND (CONFIG_CORE STREQUAL cm4f OR CONFIG_CORE STREQUAL cm7f))
  target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
      ${CMAKE_CURRENT_LIST_DIR}/RTX/Library/GCC/libRTX_CM4F.a
      -Wl,--end-group
  )
endif()

if(CONFIG_TOOLCHAIN STREQUAL mdk AND CONFIG_CORE STREQUAL cm0p)
  target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
      ${CMAKE_CURRENT_LIST_DIR}/RTX/Library/ARM/RTX_CM0.lib
      -Wl,--end-group
  )
endif()

if(CONFIG_TOOLCHAIN STREQUAL mdk AND (CONFIG_CORE STREQUAL cm4f OR CONFIG_CORE STREQUAL cm7f))
  target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
      ${CMAKE_CURRENT_LIST_DIR}/RTX/Library/ARM/RTX_CM4F.lib
      -Wl,--end-group
  )
endif()

if(CONFIG_COMPILER STREQUAL iar AND CONFIG_CORE STREQUAL cm33 AND CONFIG_FPU STREQUAL SP_FPU)
  target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
      ${CMAKE_CURRENT_LIST_DIR}/RTX/Library/IAR/RTX_V8MMF.a
      -Wl,--end-group
  )
endif()

if(CONFIG_COMPILER STREQUAL gcc AND CONFIG_CORE STREQUAL cm33 AND CONFIG_FPU STREQUAL SP_FPU)
  target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
      ${CMAKE_CURRENT_LIST_DIR}/RTX/Library/GCC/libRTX_V8MMF.a
      -Wl,--end-group
  )
endif()

if(CONFIG_COMPILER STREQUAL armclang AND CONFIG_CORE STREQUAL cm33 AND CONFIG_FPU STREQUAL SP_FPU)
  target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
      ${CMAKE_CURRENT_LIST_DIR}/RTX/Library/ARM/RTX_V8MMF.lib
      -Wl,--end-group
  )
endif()

else()

message(SEND_ERROR "CMSIS_RTOS2_Secure dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
