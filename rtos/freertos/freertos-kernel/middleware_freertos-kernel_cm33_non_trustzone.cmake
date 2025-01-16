# Add set(CONFIG_USE_middleware_freertos-kernel_cm33_non_trustzone true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      if((CONFIG_TOOLCHAIN STREQUAL armgcc OR CONFIG_TOOLCHAIN STREQUAL mcux OR CONFIG_TOOLCHAIN STREQUAL mdk) AND CONFIG_CORE STREQUAL cm33 AND (CONFIG_FPU STREQUAL NO_FPU OR CONFIG_FPU STREQUAL SP_FPU))
    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/portable/GCC/ARM_CM33_NTZ/non_secure/mpu_wrappers_v2_asm.c
          ${CMAKE_CURRENT_LIST_DIR}/portable/GCC/ARM_CM33_NTZ/non_secure/port.c
          ${CMAKE_CURRENT_LIST_DIR}/portable/GCC/ARM_CM33_NTZ/non_secure/portasm.c
        )
    endif()

        if(CONFIG_TOOLCHAIN STREQUAL iar AND CONFIG_CORE STREQUAL cm33 AND (CONFIG_FPU STREQUAL NO_FPU OR CONFIG_FPU STREQUAL SP_FPU))
    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/portable/IAR/ARM_CM33_NTZ/non_secure/mpu_wrappers_v2_asm.S
          ${CMAKE_CURRENT_LIST_DIR}/portable/IAR/ARM_CM33_NTZ/non_secure/port.c
          ${CMAKE_CURRENT_LIST_DIR}/portable/IAR/ARM_CM33_NTZ/non_secure/portasm.s
        )
    endif()

  
      if(CONFIG_TOOLCHAIN STREQUAL iar AND CONFIG_CORE STREQUAL cm33 AND (CONFIG_FPU STREQUAL NO_FPU OR CONFIG_FPU STREQUAL SP_FPU))
    target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/portable/IAR/ARM_CM33_NTZ/non_secure
        )
    endif()

        if((CONFIG_TOOLCHAIN STREQUAL armgcc OR CONFIG_TOOLCHAIN STREQUAL mcux OR CONFIG_TOOLCHAIN STREQUAL mdk) AND CONFIG_CORE STREQUAL cm33 AND (CONFIG_FPU STREQUAL NO_FPU OR CONFIG_FPU STREQUAL SP_FPU))
    target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/portable/GCC/ARM_CM33_NTZ/non_secure
        )
    endif()

  
