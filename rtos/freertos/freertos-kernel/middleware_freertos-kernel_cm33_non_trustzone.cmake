# Add set(CONFIG_USE_middleware_freertos-kernel_cm33_non_trustzone true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_freertos-kernel AND (CONFIG_USE_middleware_freertos-kernel_template OR (NOT CONFIG_USE_middleware_freertos-kernel_template)) AND (CONFIG_CORE STREQUAL cm33) AND (NOT CONFIG_USE_middleware_freertos-kernel_cm33_trustzone_non_secure) AND (NOT CONFIG_USE_middleware_freertos-kernel_cm33_trustzone_secure))

if(CONFIG_CORE STREQUAL cm33 AND CONFIG_FPU STREQUAL SP_FPU AND (CONFIG_TOOLCHAIN STREQUAL armgcc OR CONFIG_TOOLCHAIN STREQUAL mcux OR CONFIG_TOOLCHAIN STREQUAL mdk))
  target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
      ${CMAKE_CURRENT_LIST_DIR}/portable/GCC/ARM_CM33_NTZ/non_secure/port.c
      ${CMAKE_CURRENT_LIST_DIR}/portable/GCC/ARM_CM33_NTZ/non_secure/portasm.c
  )
endif()

if(CONFIG_CORE STREQUAL cm33 AND CONFIG_FPU STREQUAL SP_FPU AND CONFIG_TOOLCHAIN STREQUAL iar)
  target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
      ${CMAKE_CURRENT_LIST_DIR}/portable/IAR/ARM_CM33_NTZ/non_secure/port.c
      ${CMAKE_CURRENT_LIST_DIR}/portable/IAR/ARM_CM33_NTZ/non_secure/portasm.s
  )
endif()

if(CONFIG_CORE STREQUAL cm33 AND CONFIG_FPU STREQUAL SP_FPU AND CONFIG_TOOLCHAIN STREQUAL iar)
target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/portable/IAR/ARM_CM33_NTZ/non_secure
)
endif()

if(CONFIG_CORE STREQUAL cm33 AND CONFIG_FPU STREQUAL SP_FPU AND (CONFIG_TOOLCHAIN STREQUAL armgcc OR CONFIG_TOOLCHAIN STREQUAL mcux OR CONFIG_TOOLCHAIN STREQUAL mdk))
target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/portable/GCC/ARM_CM33_NTZ/non_secure
)
endif()

else()

message(SEND_ERROR "middleware_freertos-kernel_cm33_non_trustzone dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
